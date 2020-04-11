/*
    Garbage collector for Elm
    -------------------------
    A mark-compact collector, based on the Compressor algorithm.

    Notable features:
    ================
    - Compression uses only a single pass over the heap

    - Uses a bitmap of live/dead bits instead of marking the values themselves.
        => Many operations involve only a few localised memory accesses

    - Calculates 'forwarding addresses' of moved pointers on the fly
        - New location has the same number of live bytes below it as the old one did
        - Speeds up the calculation by storing a table of 'offsets' to the first
            live value in each 512-byte block of memory

    - Designed for both 32 and 64-bit platforms.
        Uses the size_t data type everywhere (32 or 64 bits as per platform)

    - Takes advantage of immutable-only data
        - Can do partial compression, starting at any point and only looking
        at values _above_ it, ignoring everything below.
        - Enabled by the fact that immutable values can only reference older
        values (lower addresses).
        - Offers flexibility to do quick partial collections.

    - Takes advantage of pure functions
        - A novel stack map idea: "replay" for pure functions
        - Wasm does not allow access to registers (which it models as a stack machine)
        - Therefore we can't find or modify pointers stored in registers. So how can
            we move things around safely?
        - Elm functions are pure. So we can abort execution, move values around,
            then "replay" the same function call with the same values, but at
            the new locations.
        - On this second "replay" run, no registers can possibly hold pointers
            to the old locations.
        - Now, on "replay", every call our top-level function makes can be replaced
            with the return value we remember from the first time.
        - This means replay is fast, and doesn't regenerate the same garbage again.
            It executes very little code, produces minimal garbage, and runs fast.
        - This only works with pure functions, but in Elm that's most of the program.
        - The `apply` operator has some hooks into the GC. The GC puts some extra
            marker values into the heap to track pushes and pops of the call stack.


    References
        - "The Garbage Collection Handbook" by Jones, Hosking, and Moss (section 3.4)
        - Kermany and Petrank, 2006
        - Abuaiadh et al, 2004
*/
#include "gc.h"
#include <errno.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include "gc-internals.h"
#include "types.h"

#if defined(DEBUG) || defined(DEBUG_LOG)
#include <stdio.h>
#include "./debug.h"
#else
#define log_error(...)
#endif

GcState gc_state;

#ifdef _WIN32
static void* sbrk(size_t size) {
  return malloc(GC_WASM_PAGE_BYTES * 5 + size);
}
#endif

/* ====================================================

                PROGRAM STARTUP

   ==================================================== */

// Call exactly once on program startup
int GC_init() {
  GcState* state = &gc_state;  // local reference for debugger to see

  // Get current max address of program data
  size_t* break_ptr = sbrk(0);

  // Align to next block boundary
  size_t break_aligned = (size_t)break_ptr;
  if (break_aligned % GC_BLOCK_BYTES) {
    break_aligned |= (GC_BLOCK_BYTES - 1);
    break_aligned++;
  }
  size_t* heap_bottom = (size_t*)break_aligned;

  // Initialise state with zero heap size
  *state = (GcState){
      .heap =
          (GcHeap){
              .start = heap_bottom,
              .end = heap_bottom,
              .system_end = heap_bottom,
              .bitmap = heap_bottom,
              .offsets = heap_bottom,
          },
      .next_alloc = heap_bottom,
      .roots = &Nil,
      .stack_map = NULL,
      .stack_depth = 0,
  };

  // Ask the system for more memory
  size_t top_of_current_page = (size_t)heap_bottom | (size_t)(GC_WASM_PAGE_BYTES - 1);
  size_t pages = 1;
  size_t* top_of_nth_page =
      (size_t*)(top_of_current_page + (pages * GC_WASM_PAGE_BYTES) + 1);

  int err = set_heap_end(&state->heap, top_of_nth_page);

  if (!err) {
    GC_stack_empty();
  }

  return err;
}

/*
  GC ROOTS
  --------
  Roots should be registered by Kernel modules once on program startup.

  Usage:
    ElmValue* my_mutable_heap_pointer;

    void My_init() {
        GC_register_root(&my_mutable_heap_pointer);
    }

  Once the mutable pointer is registered, the Kernel module
  can point it at any heap value it wants to keep alive.
  It can later update the pointer to point at a different
  ElmValue on the heap, as the program executes.
  Whenever the GC does a collection, it checks this pointer
  and preserves whatever heap value it is currently
  pointing to. If it moves the value, it will update the
  pointer to reference the new location.
*/
void* GC_register_root(void** ptr_to_mutable_ptr) {
  GcState* state = &gc_state;
  state->roots = NEW_CONS(ptr_to_mutable_ptr, state->roots);
  return ptr_to_mutable_ptr;  // return anything but pGcFull to show success
}

/* ====================================================

                ALLOCATE & COPY

   ==================================================== */

/*
  Allocate memory on the heap
  Same interface as malloc in stdlib.h
*/
void* GC_malloc(size_t bytes) {
  GcState* state = &gc_state;
  size_t words = bytes / sizeof(size_t);

#ifdef DEBUG
  if (bytes % sizeof(size_t)) {
    log_error("GC_malloc: Request for %zd bytes is misaligned\n", bytes);
  }
#endif

  size_t* replay = state->replay_ptr;
  if (replay != NULL) {  // replay mode

#ifdef DEBUG
    u32 replay_words = ((Header*)replay)->size;
    if (replay_words != words) {
      log_error(
          "GC_malloc: replay error. Requested size %zd doesn't match cached size %d "
          "at %p\n",
          words,
          replay_words,
          replay);
    }
#endif

    size_t* next_replay = replay + words;
    if (next_replay >= state->next_alloc) {
      next_replay = NULL;  // exit replay mode
    }
    state->replay_ptr = next_replay;
    return (void*)replay;

  } else {  // normal (non-replay) mode

    size_t* old_heap = state->next_alloc;
    size_t* new_heap = old_heap + words;

    if (new_heap < state->heap.end) {
      state->next_alloc = new_heap;
      return old_heap;
    } else {
      return pGcFull;
    }
  }
}

void* GC_memcpy(void* dest, void* src, size_t bytes) {
  size_t words = bytes / sizeof(size_t);

#ifdef DEBUG
  if (bytes % sizeof(size_t)) {
    log_error("GC_memcpy: Copy %zd bytes is misaligned\n", bytes);
  }
#endif

  size_t* src_words = (size_t*)src;
  size_t* dest_words = (size_t*)dest;

  for (size_t i = 0; i < words; i++) {
    dest_words[i] = src_words[i];
  }
  return dest;  // C standard lib returns this. Normally ignored.
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
  return p;
}

void* GC_stack_push() {
  GcState* state = &gc_state;
  if (state->replay_ptr != NULL) return state->replay_ptr;

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
  GcState* state = &gc_state;
  size_t closure_bytes = sizeof(Closure) + n_args * sizeof(void*);
  size_t cont_bytes = closure_bytes + sizeof(GcStackMap);

  void* tce_space = GC_malloc(cont_bytes);
  if (tce_space != pGcFull) {
    GcStackMap* tailcall = (GcStackMap*)(tce_space + closure_bytes);
    state->stack_map = tailcall;
  }
  return tce_space;
}

// Evaluate a tail call elminated Elm function,
// managing all of the GC related stuff for it
void* GC_tce_eval(void* (*tce_eval)(void* [], void**),
    void* (*eval)(void* []),
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
  tailcall->header = HEADER_GC_STACK_TC;
  tailcall->older = push;
  tailcall->replay = c_replay;

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

#ifdef DEBUG
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
#endif

void* GC_apply_replay() {
  GcState* state = &gc_state;
  if (state->replay_ptr == NULL) return NULL;

  Tag replay_tag = ((Header*)state->replay_ptr)->tag;
#ifdef DEBUG_LOG
  printf("GC_apply_replay: replay_ptr = %p, tag = %x\n", state->replay_ptr, replay_tag);
#endif

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
  GcStackMap* push;
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
      replay = RE_EXECUTE;
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

  // #ifdef DEBUG
  // printf("GC_apply_replay:\n");
  // printf("    scenario = %s\n", scenario_to_string(scenario));
  // printf("    replay = %p\n", replay);
  // printf("    stackmap_next = %p\n", stackmap_next);
  // printf("    stack_depth_increment = %zd\n", stack_depth_increment);
  // printf("    replay_next = %p\n", replay_next);
  // #endif

  // Update the state
  // Don't do the memory access unless value is changed

  if (stackmap_next != STACKMAP_UNCHANGED) {
    state->stack_map = stackmap_next;
  }

  if (stack_depth_increment) {
    state->stack_depth += stack_depth_increment;
  }

  if (replay_next >= state->next_alloc) {
    replay_next = EXIT_REPLAY_MODE;
  }
  state->replay_ptr = replay_next;

  return replay;
}

/* ====================================================

                COLLECT

   ==================================================== */

void GC_collect_full() {
  collect(&gc_state, gc_state.heap.start);
}

void GC_collect_nursery() {
  collect(&gc_state, gc_state.nursery);
}

void GC_prep_replay() {
  GcState* state = &gc_state;
  reverse_stack_map(state);
  size_t* first_allocated = (size_t*)(state->stack_map_empty + 1);
  state->replay_ptr = first_allocated;
  state->stack_depth = 0;
}

/* ====================================================

                SIZE CHECK

   ==================================================== */

#ifdef GC_SIZE_CHECK
// Check compiled size of GC in Wasm
// Only 6.4kB!!! :)
// I don't have replay or controller yet, but wow
// And this includes some fixed overhead that emcc generates
//
void* dummy_tce_eval(void* args[3], void** gc_tce_data) {
  size_t dummy = (size_t)args + (size_t)gc_tce_data;
  return (void*)dummy;
}
int main(int argc, char** argv) {
  // Dummy code to prevent dead code elimination

  GcState* state = &gc_state;

  // Create variables of particular types, with values
  // coming from outside world so compiler can't eliminate.
  // We don't need to run this, just compile it,
  // so segfaults are not an issue!
  ElmValue** root = (ElmValue**)argv[0];
  size_t word = (size_t)argc;
  size_t* pword = (size_t*)argv[3];
  Closure* c = (Closure*)argv[1];
  ElmValue* v = (ElmValue*)argv[2];
  void* dest = argv[4];
  void* src = argv[5];
  void** pointer_array = (void**)argv;

  GC_init();
  GC_register_root(root);
  GC_malloc(word);
  GC_memcpy(dest, src, word);

  void* push = GC_stack_push();
  // GC_stack_tailcall(c, push);
  GC_stack_pop(v, push);

  mark(state, pword);
  compact(state, pword);
  reverse_stack_map(state);

  GC_tce_iteration(word);
  GC_tce_eval(&dummy_tce_eval, NULL, 3, pointer_array);

  GC_apply_replay();

  return 0;
}
#endif
