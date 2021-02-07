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

// Get current stack index (before doing a call or tail call)
GcStackMapIndex GC_stack_get_current_pos() {
  return gc_state.stack_index;
}

// Push a newly constructed value onto the stack
void GC_stack_push(void* value) {
  GcState* state = &gc_state;
  if (state->replay_until) {
    return;
  }

  state->stack_values[state->stack_index] = value;
  state->stack_flags[state->stack_index] = 'A';
  if (state->call_stack_index == -1) {
    // We haven't entered GC execution yet...
    // this is probably a test because in real life JS would be writing this
    state->stack_functions[state->stack_index] = NULL;
  } else {
    state->stack_functions[state->stack_index] = state->call_stack[state->call_stack_index];
  }
  state->stack_index++;
  assert(state->stack_index < GC_STACK_MAP_SIZE);
}

// Track when a function returns
void GC_stack_pop(EvalFunction evaluator, void* result, GcStackMapIndex push) {
  GcState* state = &gc_state;
  assert(!state->replay_until);
  assert(sanity_check(result));

  state->stack_index = push;
  state->stack_values[state->stack_index] = result;
  state->stack_flags[state->stack_index] = 'R'; // treat as return value for replay
  state->stack_functions[state->stack_index] = evaluator;
  state->stack_index++;
  assert(state->stack_index < GC_STACK_MAP_SIZE);

  // assert(state->call_stack[state->call_stack_index] == evaluator);
  if (state->call_stack[state->call_stack_index] != evaluator) {
    printf("Returning from %s but call stack expects %s\n",
      Debug_evaluator_name(evaluator),
      Debug_evaluator_name(state->call_stack[state->call_stack_index])
    );
  }

  state->call_stack_index--;
}

// Track when a tail call occurs
Closure* GC_stack_tailcall(GcStackMapIndex closure_stack_index, Closure* old, u32 n_explicit_args, void* explicit_args[]) {
  GcState* state = &gc_state;
  assert(state->stack_values[closure_stack_index] == old);
  u16 max_values = old->max_values;
  u16 n_free = max_values - n_explicit_args;

  state->stack_index = closure_stack_index;
  Closure* new = NEW_CLOSURE(max_values, max_values, old->evaluator, NULL);
  for (u32 i=0; i < n_free; i++) {
    assert(sanity_check(old->values[i]));
    new->values[i] = old->values[i];    
  }
  for (u32 i=0; i < n_explicit_args; i++) {
    assert(sanity_check(explicit_args[i]));
    new->values[n_free + i] = explicit_args[i];
  }
  assert(sanity_check(new));

  return new;
}


// Handle a function call in replay mode (replay return value or resume execution)
void* GC_apply_replay(EvalFunction evaluator) {
  GcState* state = &gc_state;
  state->call_stack_index++;

  if (state->replay_until) {
    char flag = state->stack_flags[state->stack_index];
    if (flag == 'R') {
      EvalFunction returned_from = state->stack_functions[state->stack_index];
      if (returned_from != evaluator) {
        printf("Replaying call to %s but next replay value is for %s\n",
          Debug_evaluator_name(evaluator), Debug_evaluator_name(returned_from)
        );
        return NULL; // This replay value is for another call deeper in the stack
      }
      ElmValue* value = state->stack_values[state->stack_index];
      state->stack_index++;
      if (state->stack_index >= state->replay_until) {
        state->replay_until = 0;
        printf("Exiting replay mode at stack_index=%d, after substituting a call to %s with recorded value %p\n", state->stack_index, Debug_evaluator_name(evaluator), value);
      }
      return value;
    }
    assert(flag == 'A');
  } else {
    state->call_stack[state->call_stack_index] = evaluator;
  }

  // Resume this interrupted call by saying "no replay value"
  return NULL;
}


// Handle allocations for replay mode
void* malloc_replay(ptrdiff_t bytes) {
  GcState* state = &gc_state;
  u32 requested_words = bytes / sizeof(void*);
  assert(bytes % sizeof(void*) == 0);
  char flag = state->stack_flags[state->stack_index];
  assert(flag == 'A');

  ElmValue* saved = state->stack_values[state->stack_index];
  u32 saved_words = saved->header.size;
  assert(requested_words == saved_words);
  EvalFunction f_value_stack = state->stack_functions[state->stack_index];
  EvalFunction f_call_stack = state->call_stack[state->call_stack_index];
  if (f_value_stack != f_call_stack) {
    printf("Mismatching functions: value stack is from %s, but call stack is %s\n",
      Debug_evaluator_name(f_value_stack), Debug_evaluator_name(f_call_stack)
    );
  }
  state->stack_index++;
  if (state->stack_index >= state->replay_until) {
    state->replay_until = 0;
    printf("Exiting replay mode at stack_index=%d, after substituting an allocation in %s with recorded value %p\n", state->stack_index,
      Debug_evaluator_name(state->call_stack[state->call_stack_index]),
      saved);
  }

  return saved;
}
