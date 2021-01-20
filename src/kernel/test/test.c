#include "../core/core.h"
#include <stdio.h>

void* eval_Test_runThunk(void* args[]) {
  printf("entering eval_Test_runThunk\n");
  Closure* thunk = args[0];
  Debug_pretty("thunk", thunk);

  void* test_return_val = A1(thunk, &Unit);
  void* wrapped = eval_elm_core_Result_Ok((void*[]){test_return_val});
  return wrapped;
}

Closure Test_runThunk = {
    .header = HEADER_CLOSURE(0),
    .max_values = 1,
    .evaluator = &eval_Test_runThunk,
};
