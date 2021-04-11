#include <stdio.h>
#include "internals.h"

/* ====================================================

                STACK MAP

   ====================================================

    The Stack Map keeps track of which heap values are currently
    pointed to from the stack or registers.

    As we execute the program, functions grab pointers to heap values
    and manipulate them in local variables. When we interrupt to
    do GC, we need to mark those as live.

    Local heap pointers must either be:
     - on the C stack (in Wasm linear memory)
     - or in registers (represented as the 'stack machine' in Wasm)

    In Wasm we can't scan the registers because they are completely
    abstracted away! This is part of the Wasm security model.

    Instead we use a "stack map", our own structure that tracks which
    functions are active and which heap values they are using.

    A heap value is considered to be in use if
     - a live call allocated it
     - a live call received it from a completed child call
     - a live call received it as an argument

    We can ignore argument values, based on inductive reasoning!
    Any argument must come from the parent frame.
    If it was allocated or returned there, we already flagged it as live.
    Otherwise it comes from an even higher frame, etc...
    In the top frame, we start with a full Closure and flag it as an allocation.

*/

#define GC_STACK_VERBOSE 0

void* stack_values[GC_STACK_MAP_SIZE];
char stack_flags[GC_STACK_MAP_SIZE];  // flag which values are returns or allocations

void stack_clear() {
  GcStackMap* sm = &gc_state.stack_map;

  sm->frame = 0;
  sm->index = 0;

  for (int i = 0; i < GC_STACK_MAP_SIZE; ++i)
    stack_values[i] = NULL;
  for (int i = 0; i < GC_STACK_MAP_SIZE; ++i)
    stack_flags[i] = 0;
}


void stack_enter(Closure* c) {
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
  stack_values[sm->index] = value;
  stack_flags[sm->index] = 'A';
  sm->index++;
  assert(sm->index < GC_STACK_MAP_SIZE);
}


// Push a new frame onto the stack
void GC_stack_push_frame(EvalFunction evaluator) {
  GcStackMap* sm = &gc_state.stack_map;

  GcStackMapIndex i = sm->index;
  sm->frame = i;
  stack_flags[i] = 'F';
  stack_values[i] = evaluator;
  sm->index++;
#if GC_STACK_VERBOSE
  printf("Pushing new frame for %s at %d\n", Debug_evaluator_name(evaluator), i);
#endif
}


// Track when a function returns
void GC_stack_pop_frame(EvalFunction evaluator, void* result, GcStackMapIndex frame) {
  GcStackMap* sm = &gc_state.stack_map;
  assert(sanity_check(result));
  assert(stack_values[frame] == evaluator);
  assert(stack_flags[frame] == 'F');

  stack_values[frame] = result;
  stack_flags[frame] = 'R';
  sm->index = frame + 1;

  GcStackMapIndex parent = frame;
  while (parent != 0 && stack_flags[--parent] != 'F')
    ;
  sm->frame = parent;

#if GC_STACK_VERBOSE
  printf("Popping frame for %s, writing result to index %d, parent frame is %d\n",
      Debug_evaluator_name(evaluator),
      frame,
      parent);
#endif
}


// Track when a tail call occurs
void GC_stack_tailcall(GcStackMapIndex frame) {
  GcStackMap* sm = &gc_state.stack_map;
  assert(stack_flags[frame] == 'F');

  sm->frame = frame;
  sm->index = frame + 1;

#if GC_STACK_VERBOSE
  printf("Tail call in %s, frame %d\n", Debug_evaluator_name(evaluator), frame, );
#endif
}
