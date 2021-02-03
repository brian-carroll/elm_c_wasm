#include "internals.h"
#include <stdio.h>

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
        - When out of memory, return early from all functions (like throwing an exception)
        - Mark all heap values that were allocated by functions we interrupted
        - Mark any return values from functions that have finished running
        - Do a collection
        - Rerun the top-level function (e.g. `update` with same model and message)
        - **BUT** replace function calls with the values they returned last time
        - For all allocations, return same object as last time (at its new address)

    This fast-forwards back to where we interrupted execution, with all local
    variables referencing only the new moved pointers.
    This is only possible when there are no side-effects.
*/

GcLiveSection* GC_stack_push(EvalFunction evaluator) {
  GcState* state = &gc_state;

  if (!state->replay) {
    GcLiveSection* parent = state->current_live_section;
    parent->end = state->next_alloc;

    state->current_live_section++;
    bounds_check_live_section(state->current_live_section);

    GcLiveSection* current = state->current_live_section;
    current->evaluator = evaluator;
    current->start = state->next_alloc;
    current->end = state->heap.end;

    // printf("pushing from %s to %s current=%zd\n", Debug_evaluator_name(parent->evaluator), Debug_evaluator_name(current->evaluator), current - state->first_live_section);

    return current;
  } else {
    GcLiveSection* parent = state->replay_live_section;
    assert(parent->end == state->replay);

    state->replay_live_section++;
    bounds_check_live_section(state->replay_live_section);

    GcLiveSection* current = state->replay_live_section;
    assert(current->evaluator == evaluator);
    assert(current->start == state->replay);

    return current;
  }
}


void GC_stack_pop(EvalFunction evaluator, void* result, GcLiveSection* push) {
  GcState* state = &gc_state;
  assert(!state->replay);

  // The only live value in this section is the return value
  // (and its children, which we will trace later if we do a GC)
  assert(push->evaluator == evaluator);
  push->start = result;
  push->end = result;

  // Start a new live section for further allocations in the parent scope
  GcLiveSection* next = push + 1;
  bounds_check_live_section(next);
  next->start = state->next_alloc;
  next->end = state->heap.end;

  GcLiveSection* prev = push - 1;
  next->evaluator = prev->evaluator;

  state->current_live_section = next;

  // printf("popping from %s, saving return value (%p) in section %zd, setting current=%zd (%s)\n",
  //   Debug_evaluator_name(evaluator),
  //   result,
  //   push - state->first_live_section,
  //   next - state->first_live_section,
  //   Debug_evaluator_name(next->evaluator)
  // );
  // saving return value to section %zd
  // setting current section to %zd with evaluator %s
}

GcLiveSection* GC_stack_get_current_section() {
  GcState* state = &gc_state;
  return state->current_live_section;
}

void* GC_stack_tailcall(GcLiveSection* push, u32 n_free, void* free_vars[], u32 n_explicit_args, void* explicit_args[]) {
  GcState* state = &gc_state;

  u32 n = n_free + n_explicit_args;
  Closure* c = NEW_CLOSURE(n, n, push->evaluator, NULL);
  for (u32 i=0; i < n_free; i++) {
    c->values[i] = free_vars[i];    
  }
  for (u32 i=0; i < n_explicit_args; i++) {
    c->values[n_free + i] = explicit_args[i];
  }

  // Save current state of tail recursion for later replay
  // Can ignore everything else in this section
  push->start = (size_t*)c;
  push->end = (size_t*)c;

  // New live section for next iteration
  GcLiveSection* next = push + 1;
  bounds_check_live_section(next);
  next->evaluator = push->evaluator;
  next->start = state->next_alloc;
  next->end = state->heap.end;


  // printf("tailcall in %s with current=%zd push=%zd setting current=%zd\n",
  //   Debug_evaluator_name(push->evaluator),
  //   state->current_live_section - state->first_live_section,
  //   push - state->first_live_section,
  //   next - state->first_live_section
  // );


  state->current_live_section = next;

  // printf("tailcall push=%zd (%s) next=%zd (%s) c=%p\n", push - state->first_live_section, Debug_evaluator_name(next->evaluator), next - state->first_live_section, Debug_evaluator_name(push->evaluator), c);
  // printf("tailcall on %s push=%zd current=%zd c=%p\n", Debug_evaluator_name(evaluator), push - state->first_live_section, next - state->first_live_section, c);

  return NULL;
}


void* GC_apply_replay(EvalFunction evaluator) {
  GcState* state = &gc_state;
  if (!state->replay) {
    return NULL;
  }

  GcLiveSection* section = state->replay_live_section;
  assert(section->evaluator == evaluator);

  if (section->start == section->end) {
    void* saved = section->start;  // call result or tailcall Closure

    state->replay_live_section++;
    if (state->replay_live_section >= state->current_live_section) {
      state->replay = NULL;  // Exit replay mode.
      state->replay_live_section = state->first_live_section;
    } else {
      state->replay = state->replay_live_section->start;
    }

    return saved;
  }

  // Resume this interrupted call by saying "no replay value"
  return NULL;
}


void* malloc_replay(ptrdiff_t bytes) {
  GcState* state = &gc_state;
  u32 requested_words = bytes / sizeof(void*);
  assert(bytes % sizeof(void*) == 0);

  ElmValue* saved = (ElmValue*)state->replay;
  u32 saved_words = saved->header.size;
  assert(requested_words == saved_words);

  size_t* next_replay = state->replay + requested_words;

  if (next_replay >= state->next_alloc) {
    assert(state->replay_live_section == state->current_live_section);
    state->replay = NULL;
    state->replay_live_section = state->first_live_section;

  } else if (next_replay >= state->replay_live_section->end) {
    state->replay_live_section++;
    bounds_check_live_section(state->replay_live_section);
    state->replay = state->replay_live_section->start;

  } else {
    state->replay = next_replay;
  }

  return saved;
}
