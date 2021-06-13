#include "../test.h"

enum {
  MANAGER_Browser_Events,
  MANAGER_Http,
  MANAGER_Task,
};
u32 Platform_managers_size = 3;

void* eval_mockFunction(void* args[]) {
  ElmValue* returnValue = args[0];
  if (verbose) {
    DEBUG_PRETTY(returnValue);
  }
  return returnValue;
}
Closure* mockFunction(u16 arity, void* returnValue) {
  return newClosure(1, arity + 1, eval_mockFunction, (void*[]){returnValue});
}
Task* mockTask(char* label) {
  ElmString* s = create_string(label);
  return eval_Scheduler_succeed((void*[]){s});
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

ManagerMsg cmdNone = {
    .header = HEADER_CUSTOM(1),
    .ctor = MANAGER_MSG_NODE,
    .node = {.bags = &Nil},
};

ManagerMsg subNone = {
    .header = HEADER_CUSTOM(1),
    .ctor = MANAGER_MSG_NODE,
    .node = {.bags = &Nil},
};

Closure Browser_element = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0xffff,
    .evaluator = NULL,
};

Closure * gptr_author_project_Main_main;
void* ginit_author_project_Main_main() {
  Closure* init = mockFunction(1, newTuple2(create_string("init"), &cmdNone));
  Closure* subs = mockFunction(1, &subNone);
  Closure* update = mockFunction(2, newTuple2(create_string("update"), &cmdNone));
  Closure* view = mockFunction(0xffff, create_string("view"));
  Record* elmTeaRecord = newRecord(
      &fg_init_subscriptions_update_view, 4, ((void*[]){init, subs, update, view}));
  Record* wasmTeaRecord = A1(&g_author_project_WebAssembly_intercept, elmTeaRecord);
  return A1(&Browser_element, wasmTeaRecord);
}


void mock_app_main() {
  GC_init_root((void**)&gptr_author_project_Main_main, &ginit_author_project_Main_main);
  Platform_managerConfigs = newCustom(-1,
      3,
      ((void*[]){
          createManager_Browser_Events(),
          createManager_Http(),
          createManager_Task(),
      }));
  GC_register_root((void**)&Platform_managerConfigs);
  GC_collect_major();
}