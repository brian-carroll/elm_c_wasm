#include "../core/core.h"

void* eval_Test_runThunk(void* args[]) {
  Closure* thunk = args[0];

  void* test_return_val = A1(thunk, &Unit);
  void* wrapped = eval_elm_core_Result_Ok((void*[]){test_return_val});
  return wrapped;
}

Closure Test_runThunk = {
    .header = HEADER_CLOSURE(0),
    .max_values = 1,
    .evaluator = &eval_Test_runThunk,
};
