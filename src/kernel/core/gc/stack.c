#include <stdio.h>
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

void* stack_values[GC_STACK_MAP_SIZE];
char stack_flags[GC_STACK_MAP_SIZE];  // flag which values are returns or allocations

void GC_stack_clear() {
  GcStackMap* sm = &gc_state.stack_map;

  sm->replay_until = 0;
  sm->frame = 0;
  sm->index = 0;

  for (int i = 0; i < GC_STACK_MAP_SIZE; ++i)
    stack_values[i] = NULL;
  for (int i = 0; i < GC_STACK_MAP_SIZE; ++i)
    stack_flags[i] = 0;
}


void GC_stack_enter(Closure* c) {
  GcStackMap* sm = &gc_state.stack_map;
  stack_flags[0] = 'F';
  stack_flags[1] = 'A';
  stack_values[1] = c;
  sm->index = 2;
}



// Get current stack index (before doing a call or tail call)
GcStackMapIndex GC_get_stack_frame() {
  return gc_state.stack_map.frame;
}


// Push a newly constructed value onto the stack
void GC_stack_push_value(void* value) {
  GcStackMap* sm = &gc_state.stack_map;
  if (sm->replay_until) {
    return;
  }

  stack_values[sm->index] = value;
  stack_flags[sm->index] = 'A';
  sm->index++;
  assert(sm->index < GC_STACK_MAP_SIZE);
}


// Push a new frame onto the stack or get a replay value
void* GC_stack_push_frame(EvalFunction evaluator) {
  GcStackMap* sm = &gc_state.stack_map;

  // normal execution mode
  if (!sm->replay_until) {
    GcStackMapIndex i = sm->index;
    sm->frame = i;
    stack_flags[i] = 'F';
    stack_values[i] = evaluator;
    sm->index++;
    // printf("Pushing new frame for %s at %d\n", Debug_evaluator_name(evaluator), i);
    return NULL;  // no replay value
  }

  // replay mode
  void* ret;
  char flag = stack_flags[sm->index];
  if (flag == 'R') {
    // Call had completed before GC. We have a saved return value.
    ElmValue* value = stack_values[sm->index];
    // printf("Replaying returned value at index %d = %p\n", sm->index, value);
    sm->index++;
    ret = value;
  } else {
    // Call was interrupted by Out-Of-Memory. Resume executing.
    assert(flag == 'F');
    assert(stack_values[sm->index] == evaluator);
    // printf("Resuming call at stack frame %d = %s\n", sm->index, Debug_evaluator_name(evaluator));
    sm->frame = sm->index; 
    sm->index++;
    ret = NULL;  // Return no replay value so that we'll execute the call
  }

  // maybe exit replay mode
  if (sm->index >= sm->replay_until) {
    sm->replay_until = 0;
    // printf("Exiting replay mode at stack index=%d, in a call to %s", sm->index, Debug_evaluator_name(evaluator));
    // if (ret) printf(" with recorded return value %p", ret);
    // printf("\n");
  }

  return ret;
}


// Track when a function returns
void GC_stack_pop_frame(EvalFunction evaluator, void* result, GcStackMapIndex frame) {
  GcStackMap* sm = &gc_state.stack_map;
  assert(!sm->replay_until);
  assert(sanity_check(result));
  assert(stack_values[frame] == evaluator);
  assert(stack_flags[frame] == 'F');

  stack_values[frame] = result;
  stack_flags[frame] = 'R';
  sm->index = frame + 1;

  GcStackMapIndex parent = frame;
  while (stack_flags[--parent] != 'F')
    ;
  sm->frame = parent;

  // printf("Popping frame for %s, writing result to index %d, parent frame is %d\n",
  //   Debug_evaluator_name(evaluator),
  //   frame,
  //   parent
  // );
}


// Track when a tail call occurs
Closure* GC_stack_tailcall(
    GcStackMapIndex frame, Closure* old, u32 n_explicit_args, void* explicit_args[]) {
  GcStackMap* sm = &gc_state.stack_map;
  assert(!sm->replay_until);
  assert(stack_flags[frame] == 'F');

  sm->frame = frame;
  sm->index = frame + 1;

  u16 max_values = old->max_values;
  u16 n_free = max_values - n_explicit_args;

  // printf("Tailcall in %s, rewinding to frame %d, writing closure at index %d\n",
  //   Debug_evaluator_name(old->evaluator),
  //   frame,
  //   sm->index
  // );

  // NEW_CLOSURE implicitly pushes a value to sm->index
  Closure* new = NEW_CLOSURE(max_values, max_values, old->evaluator, NULL);
  for (u32 i = 0; i < n_free; i++) {
    assert(sanity_check(old->values[i]));
    new->values[i] = old->values[i];
  }
  for (u32 i = 0; i < n_explicit_args; i++) {
    assert(sanity_check(explicit_args[i]));
    new->values[n_free + i] = explicit_args[i];
  }
  assert(sanity_check(new));

  return new;
}


// Handle allocations for replay mode
void* malloc_replay(ptrdiff_t bytes) {
  GcStackMap* sm = &gc_state.stack_map;

  u32 requested_words = bytes / sizeof(void*);
  assert(bytes % sizeof(void*) == 0);
  char flag = stack_flags[sm->index];
  assert(flag == 'A');

  ElmValue* saved = stack_values[sm->index];
  u32 saved_words = saved->header.size;
  assert(requested_words == saved_words);

  // printf("Replaying allocation at index %d %p\n", sm->index, saved);

  sm->index++;

  if (sm->index >= sm->replay_until) {
    sm->replay_until = 0;
    // printf(
    //     "Exiting replay mode at stack_index=%d, after substituting an allocation in %s"
    //     " with recorded value %p\n", sm->index,
    //     Debug_evaluator_name(stack_values[sm->frame]),
    //     saved);
  }

  return saved;
}
