#include "types.h"


// TASKS


Task* newTask(u32 ctor, void* value, Closure* callback, Closure* kill, Task* task) {
  const u32 SIZE_TASK = sizeof(Task) / SIZE_UNIT;
  Task* t = GC_allocate(true, SIZE_TASK);
  t->header = (Header){.tag = Tag_Custom, .size = SIZE_TASK};
  t->ctor = ctor;
  t->value = value;
  t->callback = callback;
  t->kill = kill;
  t->task = task;
  return t;
}


static void* eval_Scheduler_succeed(void* args[]) {
  void* value = args[0];
  return newTask(TASK_SUCCEED, value, NULL, NULL, NULL);
}
Closure Scheduler_succeed = {
    .header = HEADER_CLOSURE(0),
    .evaluator = eval_Scheduler_succeed,
    .max_values = 1,
};


static void* eval_Scheduler_fail(void* args[]) {
  void* value = args[0];
  return newTask(TASK_FAIL, value, NULL, NULL, NULL);
}
Closure Scheduler_fail = {
    .header = HEADER_CLOSURE(0),
    .evaluator = eval_Scheduler_fail,
    .max_values = 1,
};


static void* eval_Scheduler_binding(void* args[]) {
  Closure* callback = args[0];
  return newTask(TASK_BINDING, NULL, callback, NULL, NULL);
}
Closure Scheduler_binding = {
    .header = HEADER_CLOSURE(0),
    .evaluator = eval_Scheduler_binding,
    .max_values = 1,
};


static void* eval_Scheduler_andThen(void* args[]) {
  Closure* callback = args[0];
  Task* task = args[1];
  return newTask(TASK_AND_THEN, NULL, callback, NULL, task);
}
Closure Scheduler_andThen = {
    .header = HEADER_CLOSURE(0),
    .evaluator = eval_Scheduler_andThen,
    .max_values = 2,
};


static void* eval_Scheduler_onError(void* args[]) {
  Closure* callback = args[0];
  Task* task = args[1];
  return newTask(TASK_ON_ERROR, NULL, callback, NULL, task);
}
Closure Scheduler_onError = {
    .header = HEADER_CLOSURE(0),
    .evaluator = eval_Scheduler_onError,
    .max_values = 2,
};


static void* eval_Scheduler_receive(void* args[]) {
  Closure* callback = args[0];
  return newTask(TASK_RECEIVE, NULL, callback, NULL, NULL);
}
Closure Scheduler_receive = {
    .header = HEADER_CLOSURE(0),
    .evaluator = eval_Scheduler_receive,
    .max_values = 1,
};


// PROCESSES

static u32 Scheduler_guid;


static void Scheduler_enqueue(Process* proc) {}


static void* eval_Scheduler_rawSpawn(void* args[]) {
  Task* task = args[0];

  const u32 SIZE_PROCESS = sizeof(Process) / SIZE_UNIT;
  Process* proc = GC_allocate(true, SIZE_PROCESS);
  proc->header = (Header){.tag = Tag_Custom, .size = SIZE_PROCESS};
  proc->ctor = PROC_CTOR;
  proc->id = Scheduler_guid++;
  proc->root = task;
  proc->stack = NULL;
  proc->mailbox_head = &Nil;
  proc->mailbox_tail = &Nil;

  Scheduler_enqueue(proc);

  return proc;
}
Closure Scheduler_rawSpawn = {
    .header = HEADER_CLOSURE(0),
    .evaluator = eval_Scheduler_rawSpawn,
    .max_values = 1,
};


static void* eval_Scheduler_spawn_lambda(void* args[]) {
  Task* task = args[0];
  Closure* callback = args[1];
  Process* proc = eval_Scheduler_rawSpawn(&task);
  Task* t = eval_Scheduler_succeed(&proc);
  A1(callback, t);
  return NULL;
}

static void* eval_Scheduler_spawn(void* args[]) {
  Task* task = args[0];
  Closure* lambda = newClosure(1, 2, eval_Scheduler_spawn_lambda, args);
  return eval_Scheduler_binding(&lambda);
}
Closure Scheduler_spawn = {
    .header = HEADER_CLOSURE(0),
    .evaluator = eval_Scheduler_spawn,
    .max_values = 1,
};


static void* eval_Scheduler_rawSend(void* args[]) {
  Process* proc = args[0];
  void* msg = args[1];

  // push message onto end of mailbox
  Cons* newTail = newCons(msg, &Nil);
  if (proc->mailbox_tail != &Nil) {
    proc->mailbox_tail->tail = newTail;
  }
  proc->mailbox_tail = newTail;
  if (proc->mailbox_head == &Nil) {
    proc->mailbox_head = newTail;
  }

  Scheduler_enqueue(proc);
  return NULL;
}
Closure Scheduler_rawSend = {
    .header = HEADER_CLOSURE(0),
    .evaluator = eval_Scheduler_rawSend,
    .max_values = 2,
};


static void* eval_Scheduler_send_lambda(void* args[]) {
  Process* proc = args[0];
  void* msg = args[1];
  Closure* callback = args[2];
  eval_Scheduler_rawSend(args);
  A1(callback, eval_Scheduler_succeed(&pUnit));
  return NULL;
}
static void* eval_Scheduler_send(void* args[]) {
  Process* proc = args[0];
  void* msg = args[1];
  Closure* lambda = newClosure(2, 3, eval_Scheduler_send_lambda, args);
  return eval_Scheduler_binding(&lambda);
}
Closure Scheduler_send = {
    .header = HEADER_CLOSURE(0),
    .evaluator = eval_Scheduler_send,
    .max_values = 2,
};


static void* eval_Scheduler_kill_lambda(void* args[]) {
  Process* proc = args[0];
  Closure* callback = args[1];
  Task* task = proc->root;
  if (task->ctor == TASK_BINDING && task->kill) {
    Utils_apply(task->kill, 0, NULL);
  }
  proc->root = NULL;
  A1(callback, eval_Scheduler_succeed(&pUnit));
  return NULL;
}
static void* eval_Scheduler_kill(void* args[]) {
  Process* proc = args[0];
  Closure* lambda = newClosure(1, 2, eval_Scheduler_kill_lambda, args);
  return eval_Scheduler_binding(&lambda);
}
Closure Scheduler_kill = {
    .header = HEADER_CLOSURE(0),
    .evaluator = eval_Scheduler_kill,
    .max_values = 1,
};
