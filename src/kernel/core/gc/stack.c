#include <stdarg.h>
#include "../types.h"
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

void stack_reset() {
  GcStackMap* sm = &gc_state.stack_map;

  sm->frame = 0;
  sm->index = 0;

  for (int i = 0; i < GC_STACK_MAP_SIZE; ++i)
    stack_values[i] = NULL;
  for (int i = 0; i < GC_STACK_MAP_SIZE; ++i)
    stack_flags[i] = 0;
}


// Push a newly constructed value onto the stack
void GC_stack_push_value(void* value) {
  GcStackMap* sm = &gc_state.stack_map;
  stack_values[sm->index] = value;
  stack_flags[sm->index] = 'A';
#if GC_STACK_VERBOSE
  safe_printf("Pushing stack index %d in %s: %p\n",
      sm->index,
      Debug_evaluator_name(stack_values[sm->frame]),
      value);
#endif
  sm->index++;
  assert(sm->index < GC_STACK_MAP_SIZE);
}


// Push a new frame onto the stack
// frame index is returned just for debug purposes
GcStackMapIndex GC_stack_push_frame(char func_type_flag, void* func) {
  GcStackMap* sm = &gc_state.stack_map;
  GcStackMapIndex i = sm->index;

  stack_flags[i] = 'F';
  stack_values[i] = (void*)(size_t)sm->frame;
  i++;

  stack_flags[i] = func_type_flag;
  stack_values[i] = func;
  i++;

  sm->frame = sm->index;
  sm->index = i;

#if GC_STACK_VERBOSE
  safe_printf("Pushing new frame for %s at %d\n", Debug_evaluator_name(evaluator), i);
#endif

  return sm->frame;
}


// Track when a function returns
void GC_stack_pop_frame(void* func, void* result, GcStackMapIndex frame) {
  GcStackMap* sm = &gc_state.stack_map;
  ASSERT_SANITY(result);
  assert(stack_flags[frame] == 'F');
  assert(stack_values[frame + 1] == func);

  GcStackMapIndex parent = (size_t)stack_values[frame];

  stack_values[frame] = result;
  stack_flags[frame] = 'R';

  sm->index = frame + 1;
  sm->frame = parent;

#if GC_STACK_VERBOSE
  safe_printf("Popping frame for %s, writing result to index %d, parent frame is %d\n",
      Debug_evaluator_name(func),
      frame,
      parent);
#endif
}


void* GC_stack_pop_value() {
  GcStackMap* sm = &gc_state.stack_map;
  return stack_values[sm->index--];
}


// For tail call, restart the stack with the latest args
void GC_stack_tailcall(int count, ...) {
  GcStackMap* sm = &gc_state.stack_map;
  GcStackMapIndex index = sm->frame + 2;

  va_list args;
  va_start(args, count);
  for (int i = 0; i < count; ++i) {
    stack_values[index++] = va_arg(args, void*);
  }
  va_end(args);

  sm->index = index;

#if GC_STACK_VERBOSE
  safe_printf("Tail call in %s at stack index %d\n",
      Debug_evaluator_name(stack_values[sm->frame]),
      sm->frame);
#endif
}
