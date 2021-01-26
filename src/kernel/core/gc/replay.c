#include "internals.h"

void GC_prep_replay() {
  GcState* state = &gc_state;
  reverse_stack_map(state);
  size_t* first_allocated = (size_t*)(state->stack_map_empty + 1);
  state->replay_ptr = first_allocated;
  state->stack_depth = 0;
  state->stack_map = state->stack_map_empty;
}


/* ====================================================

                STACK MAP

   ====================================================

    The Stack Map keeps track of which heap values are currently
    pointed to from the stack or registers.

    As we execute the program, functions grab pointers to heap values
    and manipulate them in local variables. But when we interrupt to
    do GC, we move values around, making those local pointers corrupt.
    We need a way to fix this up after GC.

    Local heap pointers must either be:
        - on the C stack (in the top part of Wasm linear memory)
        - or in registers (represented as the 'stack machine' in Wasm)

    People usually scan both, but Wasm's registers are completely
    abstracted away and un-scannable!

    So we do this:
        - When out of memory, abandon all functions (sort of like an exception)
        - Keep all heap values that were allocated by functions we interrupted
        - Keep any return values from functions that have finished running
        - Do a collection
        - Rerun the top-level function (e.g. `update` with same model and message)
        - **BUT** replace function calls with the values they returned last time
        - For all allocations, return same object as last time (at its new address)

    This fast-forwards back to where we interrupted execution, with all local
    variables referencing only the new moved pointers.

    The functions below are called from the `apply` operator
*/

void* GC_stack_empty() {
  GcState* state = &gc_state;
  GcStackMap* p = GC_malloc(sizeof(GcStackMap));
  if (p == pGcFull) return pGcFull;
  *p = (GcStackMap){
      .header = HEADER_GC_STACK_EMPTY,
      .newer = NULL,
      .older = NULL,
      .replay = NULL,
  };
  state->stack_map = p;
  state->stack_map_empty = p;
  state->stack_depth = 0;
  return p;
}

void* GC_stack_push() {
  GcState* state = &gc_state;
  GcStackMap* p = GC_malloc(sizeof(GcStackMap));
  if (p == pGcFull) return pGcFull;
  *p = (GcStackMap){
      .header = HEADER_GC_STACK_PUSH,
      .newer = NULL,
      .older = state->stack_map,
      .replay = NULL,
  };
  state->stack_map = p;
  state->stack_depth++;
  return p;
}

void* GC_stack_pop(ElmValue* result, void* push) {
  GcState* state = &gc_state;
  GcStackMap* p = GC_malloc(sizeof(GcStackMap));
  if (p == pGcFull) return pGcFull;
  *p = (GcStackMap){
      .header = HEADER_GC_STACK_POP,
      .newer = NULL,
      .older = push,
      .replay = result,
  };
  state->stack_map = p;
  state->stack_depth--;
  return p;
}

void* GC_stack_tailcall(Closure* c, void* push) {
  GcState* state = &gc_state;
  GcStackMap* p = GC_malloc(sizeof(GcStackMap));
  if (p == pGcFull) return pGcFull;
  *p = (GcStackMap){
      .header = HEADER_GC_STACK_TC,
      .newer = NULL,
      .older = push,
      .replay = c,
  };
  state->stack_map = p;
  // stack_depth stays the same
  return p;
}

/* ====================================================

                TAIL CALL ELIMINATION

   ==================================================== */

// GC operations for one iteration of a tail call elminated Elm function
// Allocates space for a Closure and GcStackMap so that during replay
// we can skip previous iterations (and their garbage)
// Creates lots of extra garbage in order to be able to clean it all up!
void* GC_tce_iteration(size_t n_args) {
  Closure* c = NEW_CLOSURE(n_args, n_args, NULL, NULL);
  for (size_t i = 0; i < n_args; ++i) {
    c->values[i] = NULL;
  }
  GC_stack_tailcall(c, NULL);
  return c;
}

// Evaluate a tail call elminated Elm function,
// managing all of the GC related stuff for it
void* GC_tce_eval(void* (*tce_eval)(void*[], void**),
    void* (*eval)(void*[]),
    u32 n_args,
    void* args[]) {
  GcState* state = &gc_state;
  GcStackMap* push;
  size_t closure_bytes = sizeof(Closure) + n_args * sizeof(void*);

  // Pointer to new space allocated by tce_eval on every iteration
  // to place tailcall and closure in case of exception
  void* gc_tce_data;

  // Make a local copy of the closure and mutate the args during iteration
  // Ensure it becomes garbage before exiting this function
  Closure* c_mutable;
  if (state->replay_ptr && state->stack_map->header.tag == Tag_GcStackTailCall) {
    // In replay mode, reuse the Closure created by previous run (c_replay)
    GcStackMap* tailcall_old = state->stack_map;
    push = tailcall_old->older;
    c_mutable = tailcall_old->replay;
    gc_tce_data = c_mutable;
  } else {
    push = state->stack_map;  // First run. No tailcall has occurred yet, just a Push.
    gc_tce_data = CAN_THROW(GC_tce_iteration(n_args));
    c_mutable = gc_tce_data;
    *c_mutable = (Closure){
        .header = HEADER_CLOSURE(n_args),
        .n_values = n_args,
        .max_values = n_args,
        .evaluator = eval,
    };
    GC_memcpy(c_mutable->values, args, n_args * sizeof(void*));
  }

  // Run the tail-call-eliminated evaluator
  void* result = (*tce_eval)(c_mutable->values, &gc_tce_data);
  if (result != pGcFull) {
    return result;
  }

  // GC Exception handling
  // Save the current args in a closure in the stack map
  // Then we can replay later, skipping earlier iterations
  // This space was already _allocated_ but not _written_
  // by GC_tce_iteration, called from tce_eval
  Closure* c_replay = (Closure*)gc_tce_data;
  GC_memcpy(c_replay, c_mutable, closure_bytes);

  GcStackMap* tailcall = (GcStackMap*)(gc_tce_data + closure_bytes);
  *tailcall = (GcStackMap){
      .header = HEADER_GC_STACK_TC,
      .newer = NULL,
      .older = push,
      .replay = c_replay,
  };
  return pGcFull;
}

/* ====================================================

                REPLAY

   ==================================================== */

static void* next_heap_value(void* current) {
  Header* h = current;
  size_t* words = current;
  return words + h->size;
}

/*
    Call stack was interrupted and unwound by a GC exception
    but we saved all the live values in the stack map.
    Elm functions are pure so we can "replay" most of the same
    values without actually re-calculating them.
    Only need to re-execute functions that were live when the
    exception was thrown.
    This is fast and creates no garbage until we exit replay mode.
    This function is called from the normal 'Utils_apply'.
*/

typedef enum {
  Finished,  // Call had finished. Replay saved return value from previous execution.
  Unfinished_Sat_Normal,  // Unfinished call. Saturated (all args in one go). Re-execute.
  Unfinished_Sat_Tail,  // Unfinished saturated tail call. Re-execute from last iteration.
  Unfinished_Curried_Normal,  // Unfinished call with curried values. Re-execute.
  Unfinished_Curried_Tail,    // Unfinished tail call. Re-execute from last iteration.
  Partial_Application,        // Partial app, unsaturated. Return saved Closure.
  Apply_Alloc_Failed,  // `apply` failed to allocate Closure or stackmap. Re-execute.
  BugScenario          // Heap doesn't make sense. Must be a bug somewhere.
} ReplayScenario;

#ifdef DEBUG_LOG
char* scenario_to_string(ReplayScenario scenario) {
  switch (scenario) {
    case Finished:
      return "Finished";
    case Unfinished_Sat_Normal:
      return "Unfinished_Sat_Normal";
    case Unfinished_Sat_Tail:
      return "Unfinished_Sat_Tail";
    case Unfinished_Curried_Normal:
      return "Unfinished_Curried_Normal";
    case Unfinished_Curried_Tail:
      return "Unfinished_Curried_Tail";
    case Partial_Application:
      return "Partial_Application";
    case Apply_Alloc_Failed:
      return "Apply_Alloc_Failed";
    case BugScenario:
      return "BugScenario";
  }
  return "";
}
#else
#define scenario_to_string(dummy) ""
#endif

void* GC_apply_replay(void** apply_push) {
  GcState* state = &gc_state;
  if (state->replay_ptr == NULL) return NULL;

  Tag replay_tag = ((Header*)state->replay_ptr)->tag;

  /*
  Analyse heap values to work out which of several scenarios we're in.

  replay_ptr      newer stackmap  scenario
  ----------      --------------  --------
  Push            Pop             Finished
  Push            Push            Unfinished saturated call
  Push            Tailcall        Unfinished saturated tail call
  Closure(sat)    Pop             Finished
  Closure(sat)    Push            Unfinished curried call
  Closure(sat)    Tailcall        Unfinished curried tail call
  Closure(unsat)  (N/A)           Partial application, unsaturated
  next_alloc      (N/A)           Failed to allocate Push or Closure
  */

  ReplayScenario scenario;
  GcStackMap* push = NULL;
  GcStackMap* newer;
  Closure* closure;
  size_t* after_closure;

  // Each time we advance to next heap value, need to check if we've passed end of heap.
  // Early breaks are handy here, so use do-while(0)
  do {
    if (state->replay_ptr >= state->next_alloc) {
      scenario = Apply_Alloc_Failed;
      break;
    }
    if (replay_tag == Tag_Closure) {
      // Replay points to a Closure that was allocated last time in Utils_apply
      // It was a partially-applied Closure that had some more values applied
      closure = (Closure*)state->replay_ptr;
      after_closure = next_heap_value(closure);

      if (closure->n_values != closure->max_values) {
        // Closure did not get enough new values to be saturated
        scenario = Partial_Application;
        break;
      }

      if (after_closure >= state->next_alloc) {
        scenario = Apply_Alloc_Failed;
        break;
      }

      // Closure got saturated and started executing
      push = (GcStackMap*)after_closure;
      newer = push->newer;

      if ((size_t*)newer >= state->next_alloc) {
        scenario = Unfinished_Curried_Normal;
        break;
      }

      switch (newer->header.tag) {
        case Tag_GcStackPop:
          scenario = Finished;
          break;

        case Tag_GcStackPush:
          scenario = Unfinished_Curried_Normal;
          break;

        case Tag_GcStackTailCall:
          scenario = Unfinished_Curried_Tail;
          break;

        default:
          scenario = BugScenario;
          log_error("GC_apply_replay: no newer stack item after %p\n", state->replay_ptr);
          break;
      }
    } else if (replay_tag == Tag_GcStackPush) {
      // This was a saturated call (all args applied at once)
      push = (GcStackMap*)state->replay_ptr;
      newer = push->newer;
      if ((size_t*)newer >= state->next_alloc) {
        scenario = Unfinished_Sat_Normal;
        break;
      }
      switch (newer->header.tag) {
        case Tag_GcStackPop:
          scenario = Finished;
          break;

        case Tag_GcStackPush:
          scenario = Unfinished_Sat_Normal;
          break;

        case Tag_GcStackTailCall:
          scenario = Unfinished_Sat_Tail;
          break;

        default:
          scenario = BugScenario;
          log_error("GC_apply_replay: expected stack map value at %p\n", newer);
          break;
      }
    } else {
      scenario = BugScenario;
      log_error("GC_apply_replay: expected Closure or Push at %p\n", state->replay_ptr);
    }
  } while (0);

  /*
  Decide what to do based on which scenario we're in.
  - What should Utils_apply return?
  - How to find the next position of the stackmap pointer?
  - How to find the next position of the replay pointer?

  scenario                  apply returns    stackmap         replay_next
  --------                  -------------    --------         -----------
  Finished                  Pop data         push-newer       stackmap-next
  Unfinished Sat. call      eval args        push             stackmap-next
  Unfinished Sat. tail      eval tc data     push-tc          stackmap-next
  Unfinished Curried        eval args        closure-push     stackmap-next
  Unfinished Curried tail   eval tc data     closure-push-tc  stackmap-next
  Part apply, unsaturated   closure(unsat)   (no change)      closure-next
  Part apply failed         fuller closure   (no change)      NULL (exit)
  */

  void* replay;
  size_t* replay_next;
  GcStackMap* stackmap_next;
  size_t stack_depth_increment;

  // Specific meanings for NULL
  GcStackMap* STACKMAP_UNCHANGED = NULL;
  void* RE_EXECUTE = NULL;
  void* EXIT_REPLAY_MODE = NULL;

  switch (scenario) {
    case Finished:
      replay = newer->replay;  // saved return value
      stackmap_next = newer;
      stack_depth_increment = 0;  // we've pushed AND popped
      replay_next = next_heap_value(stackmap_next);
      break;

    case Unfinished_Sat_Normal:
      replay = RE_EXECUTE;
      stackmap_next = push;
      stack_depth_increment = 1;
      replay_next = next_heap_value(stackmap_next);
      break;

    case Unfinished_Sat_Tail:
      replay = newer->replay;  // saved full closure
      stackmap_next = newer;
      stack_depth_increment = 1;  // push and tailcall = push
      replay_next = next_heap_value(stackmap_next);
      break;

    case Unfinished_Curried_Normal:
      replay = closure;
      stackmap_next = push;
      stack_depth_increment = 1;
      replay_next = next_heap_value(stackmap_next);
      break;

    case Unfinished_Curried_Tail:
      replay = newer->replay;  // saved full closure
      stackmap_next = newer;
      stack_depth_increment = 1;  // push and tailcall = push
      replay_next = next_heap_value(stackmap_next);
      break;

    case Partial_Application:
      replay = closure;
      stackmap_next = STACKMAP_UNCHANGED;
      stack_depth_increment = 0;
      replay_next = after_closure;
      break;

    case Apply_Alloc_Failed:
    case BugScenario:
      replay = RE_EXECUTE;
      stackmap_next = STACKMAP_UNCHANGED;
      stack_depth_increment = 0;
      replay_next = EXIT_REPLAY_MODE;
      break;
  }

  // Have we run out of values to replay?
  if (replay_next >= state->next_alloc) {
    replay_next = EXIT_REPLAY_MODE;
  }

  log_debug("GC_apply_replay:\n");
  log_debug("  replay_ptr = %p\n", state->replay_ptr);
  log_debug("  replay_tag = %x\n", replay_tag);
  log_debug("  scenario = %s\n", scenario_to_string(scenario));
  log_debug("  stackmap_next = %p\n", stackmap_next);
  log_debug("  stack_depth_increment = %zu\n", stack_depth_increment);
  log_debug("  replay = %p\n", replay);
  log_debug("  replay_next = %p\n", replay_next);
  log_debug("  push = %p\n", push);
  log_debug("\n");

  // Update the GC state
  // Don't do the memory write unless the value is changed
  if (stackmap_next != STACKMAP_UNCHANGED) {
    state->stack_map = stackmap_next;
  }
  if (stack_depth_increment) {
    state->stack_depth += stack_depth_increment;
  }
  state->replay_ptr = replay_next;

  // Write to Utils_apply local variable. Location for next pop to link back to
  if (push) {
    *apply_push = push;
  }
  return replay;
}

void reverse_stack_map(GcState* state) {
  GcStackMap* newer_item = (GcStackMap*)state->next_alloc;
  GcStackMap* stack_item = state->stack_map;
  while (stack_item > state->stack_map_empty) {
    Tag tag = stack_item->header.tag;
    assert(tag == Tag_GcStackEmpty || tag == Tag_GcStackPush || tag == Tag_GcStackPop || tag == Tag_GcStackTailCall);

    stack_item->newer = newer_item;
    newer_item = stack_item;
    stack_item = stack_item->older;
  }

  // GcStackEmpty
  stack_item->newer = newer_item;
}
