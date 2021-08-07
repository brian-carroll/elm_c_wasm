#include "core.h"

extern DynamicArray* Platform_process_cache;

/* ====================================================

                TASKS

   ==================================================== */

Task* newTask(u32 ctor, void* value, Closure* callback, Closure* kill, Task* task) {
  const u32 size = sizeof(Task) / SIZE_UNIT;
  Task* t = GC_allocate(true, size);
  t->header = (Header){.tag = Tag_Task, .size = size};
  t->ctor = ctor;
  t->value = value;
  t->callback = callback;
  t->kill = kill;
  t->task = task;
  return t;
}


void* eval_Scheduler_succeed(void* args[]) {
  void* value = args[0];
  Task* t = newTask(TASK_SUCCEED, value, NULL, NULL, NULL);
  return t;
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


/* ====================================================

                PROCESSES

   ==================================================== */

static void Scheduler_enqueue(Process* proc);

u32 Scheduler_guid = 0;


void* eval_Scheduler_rawSpawn(void* args[]) {
  Task* task = args[0];

  const u32 size = sizeof(Process) / SIZE_UNIT;
  Process* proc = GC_allocate(true, size);
  proc->header = (Header){.tag = Tag_Process, .size = size};
  proc->id = Scheduler_guid++;
  proc->root = task;
  proc->stack = NULL;
  proc->mailbox.front = &Nil;
  proc->mailbox.back = &Nil;

  DynamicArray_push(&Platform_process_cache, proc);

  Scheduler_enqueue(proc);

  return proc;
}
Closure Scheduler_rawSpawn = {
    .header = HEADER_CLOSURE(0),
    .evaluator = eval_Scheduler_rawSpawn,
    .max_values = 1,
};


void* eval_Scheduler_spawn_lambda(void* args[]) {
  Task* task = args[0];
  Closure* callback = args[1];
  Process* proc = eval_Scheduler_rawSpawn((void*[]){task});
  Task* t = eval_Scheduler_succeed((void*[]){proc});
  A1(callback, t);
  return NULL;
}
void* eval_Scheduler_spawn(void* args[]) {
  // Task* task = args[0];
  // safe_printf("eval_Scheduler_spawn with task=%p\n");
  // DEBUG_PRETTY(task);
  Closure* lambda = newClosure(1, 2, eval_Scheduler_spawn_lambda, args);
  Task* tbinding = eval_Scheduler_binding((void*[]){lambda});
  return tbinding;
}
Closure Scheduler_spawn = {
    .header = HEADER_CLOSURE(0),
    .evaluator = eval_Scheduler_spawn,
    .max_values = 1,
};


void* eval_Scheduler_rawSend(void* args[]) {
  Process* proc = args[0];
  void* msg = args[1];
  Queue_push(&proc->mailbox, msg);
  Scheduler_enqueue(proc);
  return NULL;
}
Closure Scheduler_rawSend = {
    .header = HEADER_CLOSURE(0),
    .evaluator = eval_Scheduler_rawSend,
    .max_values = 2,
};


void* eval_Scheduler_send_lambda(void* args[]) {
  // Process* proc = args[0];
  // void* msg = args[1];
  Closure* callback = args[2];
  eval_Scheduler_rawSend(args);
  A1(callback, eval_Scheduler_succeed((void*[]){pUnit}));
  return NULL;
}
void* eval_Scheduler_send(void* args[]) {
  // Process* proc = args[0];
  // void* msg = args[1];
  Closure* lambda = newClosure(2, 3, eval_Scheduler_send_lambda, args);
  return eval_Scheduler_binding((void*[]){lambda});
}
Closure Scheduler_send = {
    .header = HEADER_CLOSURE(0),
    .evaluator = eval_Scheduler_send,
    .max_values = 2,
};


void* eval_Scheduler_kill_lambda(void* args[]) {
  Process* proc = args[0];
  Closure* callback = args[1];
  Task* task = proc->root;
  if (task->ctor == TASK_BINDING && task->kill && task->kill->header.tag == Tag_Closure) {
    Utils_apply(task->kill, 0, NULL);
  }
  proc->root = NULL;
  A1(callback, eval_Scheduler_succeed((void*[]){pUnit}));
  return NULL;
}
void* eval_Scheduler_kill(void* args[]) {
  // Process* proc = args[0];
  Closure* lambda = newClosure(1, 2, eval_Scheduler_kill_lambda, args);
  return eval_Scheduler_binding((void*[]){lambda});
}
Closure Scheduler_kill = {
    .header = HEADER_CLOSURE(0),
    .evaluator = eval_Scheduler_kill,
    .max_values = 1,
};


/* ====================================================

                STEP PROCESSES

   ==================================================== */

static void Scheduler_step(Process* proc);

bool Scheduler_working = false;
Queue Scheduler_queue = {
    .front = &Nil,
    .back = &Nil,
};


static void Scheduler_enqueue(Process* proc) {
  Queue_push(&Scheduler_queue, proc);
  // safe_printf("Enqueueing process #%d\n", proc->id);
  if (Scheduler_working) {
    return;
  }
  Scheduler_working = true;

  for (;;) {
    Process* proc = Queue_shift(&Scheduler_queue);
    if (!proc) break;
    // safe_printf("Stepping process #%d\n", proc->id);
    Scheduler_step(proc);
  }
  Scheduler_working = false;
}


// The binding `callback` for JS modules to return values from effects
// Its Closure will have a `proc` partially-applied when it is sent to JS
// The guid could provide a good way to do caching to avoid the encoding round-trip
// Keep a list of all active Processes and return a WasmProcess with the guid, like JsRef
// When JS calls back, look it up. When stepper kills the process, remove it from list
// Make the process list a GC root (prob need to anyway)
void* eval_Scheduler_step_lambda(void* args[]) {
  Process* proc = args[0];
  Task* newRoot = args[1];
  proc->root = newRoot;
  Scheduler_enqueue(proc);
  return NULL;
}


static ProcessStack* newProcessStack(
    TaskCtor ctor, Closure* callback, ProcessStack* rest) {
  const u32 size = sizeof(ProcessStack) / SIZE_UNIT;
  ProcessStack* stack = GC_allocate(true, size);
  stack->header = (Header){.tag = Tag_Custom, .size = size};
  stack->ctor = ctor;
  stack->callback = callback;
  stack->rest = rest;
  return stack;
}


char* tag_names[] = {
    "SUCCEED",
    "FAIL",
    "BINDING",
    "AND_THEN",
    "ON_ERROR",
    "RECEIVE",
};

static void Scheduler_step(Process* proc) {
  while (proc->root) {
    proc->root = proc->root;

    // safe_printf("Stepping process #%d: %s\n",
    //     proc->id,
    //     proc->root ? tag_names[proc->root->ctor] : "NULL");
    // DEBUG_PRETTY(proc->root);

    u32 rootTag = proc->root->ctor;
    if (rootTag == TASK_SUCCEED || rootTag == TASK_FAIL) {
      while (proc->stack && proc->stack->ctor != rootTag) {
        proc->stack = proc->stack->rest;
      }
      if (!proc->stack) {
        i32 cache_index = -1;
        for (u32 i = 0; i < Platform_process_cache->occupied; i++) {
          if (Platform_process_cache->values[i] == proc) {
            cache_index = i;
            break;
          }
        }
        if (cache_index == -1) {
          safe_printf("process cache miss for Process id %d @ %p\n", proc->id, proc);
        }
        // DynamicArray_remove_unordered(Platform_process_cache, cache_index);
        return;
      }
      proc->root = A1(proc->stack->callback, proc->root->value);
      proc->stack = proc->stack->rest;
    } else if (rootTag == TASK_BINDING) {
      Closure* lambda = newClosure(1, 2, eval_Scheduler_step_lambda, (void*[]){proc});
      proc->root->kill = A1(proc->root->callback, lambda);
      return;
    } else if (rootTag == TASK_RECEIVE) {
      void* received_msg = Queue_shift(&proc->mailbox);
      if (!received_msg) {
        return;
      }
      // DEBUG_PRETTY(received_msg);
      proc->root = A1(proc->root->callback, received_msg);
    } else {  // if (rootTag == TASK_AND_THEN || rootTag == TASK_ON_ERROR)
      proc->stack = newProcessStack(rootTag == TASK_AND_THEN ? TASK_SUCCEED : TASK_FAIL,
          proc->root->callback,
          proc->stack);
      proc->root = proc->root->task;
    }
  }
}
