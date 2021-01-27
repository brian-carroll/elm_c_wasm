#include "internals.h"

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

/*
  GcLiveSection* live_sections;
  void* replay;
  i16 current_live_section;
  i16 replay_live_section;
*/

/*
  GC_apply_replay
  GC_stack_push
  GC_stack_pop
 */


GcLiveSection* GC_stack_push(void* (*evaluator)(void**)) {
  GcState* state = &gc_state;

  GcLiveSection* parent = state->current_live_section;
  parent->end = state->next_alloc;

  state->current_live_section++;
  bounds_check_live_section(state->current_live_section);

  GcLiveSection* current = state->current_live_section;
  current->evaluator = evaluator;
  current->start = state->next_alloc;
  current->end = NULL;

  return current;
}


void GC_stack_pop(void* (*evaluator)(void**), void* result, GcLiveSection* push) {
  GcState* state = &gc_state;

  // The only live value in this section is the return value
  // (and its children, which we will trace later if we do a GC)
  assert(push->evaluator == evaluator);
  push->start = result;
  push->end = result;

  // Start a new live section for further allocations in the parent scope
  GcLiveSection* next = push + 1;
  bounds_check_live_section(next);
  next->start = state->next_alloc;
  next->end = NULL;

  GcLiveSection* prev = push - 1;
  next->evaluator = prev->evaluator;

  state->current_live_section = next;
}


void GC_stack_tailcall(Closure* c, GcLiveSection* push) {
  GcState* state = &gc_state;

  // The only live value in this section is the tail call closure
  // (and its children, which we will trace later if we do a GC)
  assert(push->evaluator == c->evaluator);
  push->start = c;
  push->end = c;

  // Start a new live section for further allocations in the next tail call
  GcLiveSection* next = push + 1;
  bounds_check_live_section(next);
  next->evaluator = c->evaluator;
  next->start = state->next_alloc;
  next->end = NULL;

  state->current_live_section = next;
}


void* GC_apply_replay(void* (*evaluator)(void**)) {
  GcState* state = &gc_state;
  GcLiveSection* section = state->replay_live_section;
  assert(section->evaluator == evaluator);

  if (section->start == section->end) {
    // This call was either a completed call or an incomplete tailcall
    // completed => we saved the result so we can skip the call (referential transparency)
    // tailcall => we saved a full Closure representing the next iteration
    // Utils_apply will check for a full Closure and decide what to do
    void* saved = section->start;

    // Done with this section, it only had one value in it
    state->replay_live_section++;
    if (state->replay_live_section >= state->current_live_section) {
      // We've run out of stuff to replay. Exit replay mode.
      state->replay = NULL;
      state->replay_live_section = state->first_live_section;
    } else {
      // Time to move on to the next live section
      state->replay = state->replay_live_section->start;
    }

    return saved;
  }

  // This call was still running when the heap overflow happened before GC
  // Now we resume execution of it by telling Utils_apply not to replay anything
  return NULL;
}


void* malloc_replay(ptrdiff_t bytes) {
  GcState* state = &gc_state;
  u32 requested_words = bytes / sizeof(void*);
  assert(bytes % sizeof(void*) == 0);

  ElmValue* saved = state->replay;
  u32 saved_words = saved->header.size;
  assert(requested_words == saved_words);

  void* next_replay = state->replay + bytes;

  if (next_replay >= state->next_alloc) {
    // We've run out of stuff to replay. Exit replay mode.
    assert(state->replay_live_section == state->current_live_section);
    state->replay = NULL;
    state->replay_live_section = state->first_live_section;

  } else if (next_replay >= state->replay_live_section->end) {
    // Time to move on to the next live section
    state->replay_live_section++;
    bounds_check_live_section(state->replay_live_section);
    state->replay = state->replay_live_section->start;

  } else {
    state->replay = next_replay;
  }

  return saved;
}
