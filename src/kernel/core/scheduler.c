#include "types.h"

#define SIZE_TASK sizeof(Task) / SIZE_UNIT

// TASKS


Task* newTask(u32 ctor, void* value, Closure* callback, Closure* kill, Task* task) {
  Task* t = GC_allocate(true, SIZE_TASK);
  t->header = (Header){.tag = Tag_Custom, .size = SIZE_TASK};
  t->ctor = ctor;
  t->value = value;
  t->callback = callback;
  t->kill = kill;
  t->task = task;
  return t;
}


void* eval_Scheduler_succeed(void* args[]) {
  void* value = args[0];
  return newTask(TASK_SUCCEED, value, NULL, NULL, NULL);
}
Closure Scheduler_succeed = {
    .header = HEADER_CLOSURE(0),
    .evaluator = eval_Scheduler_succeed,
    .max_values = 1,
};


void* eval_Scheduler_fail(void* args[]) {
  void* value = args[0];
  return newTask(TASK_FAIL, value, NULL, NULL, NULL);
}
Closure Scheduler_fail = {
    .header = HEADER_CLOSURE(0),
    .evaluator = eval_Scheduler_fail,
    .max_values = 1,
};


void* eval_Scheduler_binding(void* args[]) {
  Closure* callback = args[0];
  return newTask(TASK_BINDING, NULL, callback, NULL, NULL);
}
Closure Scheduler_binding = {
    .header = HEADER_CLOSURE(0),
    .evaluator = eval_Scheduler_binding,
    .max_values = 1,
};


void* eval_Scheduler_andThen(void* args[]) {
  Closure* callback = args[0];
  Task* task = args[1];
  return newTask(TASK_AND_THEN, NULL, callback, NULL, task);
}
Closure Scheduler_andThen = {
    .header = HEADER_CLOSURE(0),
    .evaluator = eval_Scheduler_andThen,
    .max_values = 2,
};


void* eval_Scheduler_onError(void* args[]) {
  Closure* callback = args[0];
  Task* task = args[1];
  return newTask(TASK_ON_ERROR, NULL, callback, NULL, task);
}
Closure Scheduler_onError = {
    .header = HEADER_CLOSURE(0),
    .evaluator = eval_Scheduler_onError,
    .max_values = 2,
};


void* eval_Scheduler_receive(void* args[]) {
  Closure* callback = args[0];
  return newTask(TASK_RECEIVE, NULL, callback, NULL, NULL);
}
Closure Scheduler_receive = {
    .header = HEADER_CLOSURE(0),
    .evaluator = eval_Scheduler_receive,
    .max_values = 1,
};