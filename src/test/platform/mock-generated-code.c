#include "../test.h"
#include "../../kernel/kernel.h"

enum {
  MANAGER_Browser_Events,
  MANAGER_Http,
  MANAGER_Task,
};
u32 Platform_managers_size = 3;
extern Closure mock_js_Browser_element;
DynamicArray* mock_tasks_executed;

void* eval_mockFunction(void* args[]) {
  ElmValue* returnValue = args[0];
  // DEBUG_PRETTY(returnValue);
  return returnValue;
}
Closure* mockFunction(u16 arity, void* returnValue) {
  return newClosure(1, arity + 1, eval_mockFunction, (void*[]){returnValue});
}

void* eval_mockTaskCallback(void* args[]) {
  ElmString* s = args[0];
  DynamicArray_push(&mock_tasks_executed, s);
  return eval_Scheduler_succeed(args);
}
Closure mockTaskCallback = {
    .header = HEADER_CLOSURE(1),
    .evaluator = eval_mockTaskCallback,
    .max_values = 1,
};

Task* mockTask(char* label) {
  ElmString* s = create_string(label);
  Task* task = eval_Scheduler_succeed((void*[]){s});
  return eval_Scheduler_andThen((void*[]){&mockTaskCallback, task});
}

void* createManager_Browser_Events() {
  return Platform_createManager(mockTask("Browser_Events_init"),
      mockFunction(3, mockTask("Browser_Events_onEffects")),
      mockFunction(3, mockTask("Browser_Events_onSelfMsg")),
      NULL,
      mockFunction(2, mockTask("Browser_Events_subMap")));
}

void* createManager_Http() {
  return Platform_createManager(mockTask("Http_init"),
      mockFunction(4, mockTask("Http_onEffects")),
      mockFunction(3, mockTask("Http_onSelfMsg")),
      mockFunction(2, mockTask("Http_cmdMap")),
      mockFunction(2, mockTask("Http_subMap")));
}

void* createManager_Task() {
  return Platform_createManager(mockTask("Task_init"),
      mockFunction(3, mockTask("Task_onEffects")),
      mockFunction(3, mockTask("Task_onSelfMsg")),
      mockFunction(2, mockTask("Task_cmdMap")),
      NULL);
}

Closure g_elm_core_Task_command = {
    .header = HEADER_CLOSURE(1),
    .n_values = 0x1,
    .max_values = 0x2,
    .evaluator = eval_Platform_leaf,
    .values = {(void*)MANAGER_Task},
};
Closure g_elm_browser_Browser_Events_subscription = {
    .header = HEADER_CLOSURE(1),
    .n_values = 0x1,
    .max_values = 0x2,
    .evaluator = eval_Platform_leaf,
    .values = {(void*)MANAGER_Browser_Events},
};
Closure g_elm_http_Http_subscription = {
    .header = HEADER_CLOSURE(1),
    .n_values = 0x1,
    .max_values = 0x2,
    .evaluator = eval_Platform_leaf,
    .values = {(void*)MANAGER_Http},
};
Closure g_elm_http_Http_command = {
    .header = HEADER_CLOSURE(1),
    .n_values = 0x1,
    .max_values = 0x2,
    .evaluator = eval_Platform_leaf,
    .values = {(void*)MANAGER_Http},
};

Closure* gptr_author_project_Main_main;
void* ginit_author_project_Main_main() {
  Closure* init = mockFunction(1,
      newTuple2(create_string("init"),
          A1(&g_elm_core_Task_command, create_string("init Cmd"))));
  Closure* subs = mockFunction(1,
      A1(&Platform_batch,
          List_create(2,
              (void*[]){
                  A1(&g_elm_browser_Browser_Events_subscription,
                      create_string("Browser Event sub")),
                  A1(&g_elm_http_Http_subscription, create_string("Http sub")),
              })));
  Closure* update = mockFunction(2,
      newTuple2(create_string("update"),
          A1(&g_elm_http_Http_command, create_string("update Cmd"))));
  Closure* view = mockFunction(0xffff, create_string("view"));
  Record* elmTeaRecord = newRecord(
      &fg_init_subscriptions_update_view, 4, ((void*[]){init, subs, update, view}));
  Record* wasmTeaRecord = A1(&g_author_project_WebAssembly_intercept, elmTeaRecord);
  void* result = A1(&mock_js_Browser_element, wasmTeaRecord);
  return result;
}


void* init_Platform_managerConfigs() {
  return newCustom(-1,
      3,
      ((void*[]){
          createManager_Browser_Events(),
          createManager_Http(),
          createManager_Task(),
      }));
}

void mock_app_init_globals() {
  GC_init_root((void**)&gptr_author_project_Main_main, &ginit_author_project_Main_main);
  GC_init_root((void**)&Platform_managerConfigs, &init_Platform_managerConfigs);
  GC_collect_major();
}

int mock_app_main() {
  return GC_init();
}