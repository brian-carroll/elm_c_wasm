#include "../test.h"
#include "../../kernel/core/core.h"

ElmInt* callback_arg = NULL;

void* eval_task_callback(void* args[]) {
  callback_arg = args[0];
  return args[0];
}

void* eval_increment_task_value(void* args[]) {
  ElmInt* in = args[0];
  return A1(&Scheduler_succeed, newElmInt(in->value + 1));
}
Closure increment_task_value = {
    .header = HEADER_CLOSURE(0),
    .evaluator = eval_increment_task_value,
    .max_values = 1,
};


void test_scheduler() {
  GC_init();
  Platform_process_cache = newDynamicArray(8);
  GC_register_root((void**)&Platform_process_cache);

  Task* t_succeed = A1(&Scheduler_succeed, newElmInt(5));
  Task* t_fail = A1(&Scheduler_fail, newElmInt(123));

  Task* t_andThen = A2(&Scheduler_andThen, &increment_task_value, t_succeed);
  Process* p_andThen = A1(&Scheduler_rawSpawn, t_andThen);
  expect_equal("andThen (\\x -> succeed (x+1)) (succeed 5) == succeed 6",
      p_andThen->root,
      A1(&Scheduler_succeed, newElmInt(6)));


  Task* t_onError = A2(&Scheduler_onError, &increment_task_value, t_fail);
  Process* p_onError = A1(&Scheduler_rawSpawn, t_onError);
  expect_equal("onError (\\x -> succeed (x+1)) (fail 123) == succeed 124",
      p_onError->root,
      A1(&Scheduler_succeed, newElmInt(124)));
}
