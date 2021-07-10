#include "../test.h"

extern Closure* gptr_author_project_Main_main;
u32 allocateJsRef(void* value);

void* eval_Browser_element(void* args[]) {
  return args[0];
}
Closure mock_js_Browser_element = {
    .header = HEADER_CLOSURE(0),
    .max_values = 1,
    .evaluator = eval_Browser_element,
};

Cons* mock_js_Platform_initialize() {
  Record* wrappedTEA = gptr_author_project_Main_main->values[0];
  size_t FIELD_init = fg_init_subscriptions_update_view.fields[0];
  Closure* wrappedInit = Utils_access_eval((void*[]){(void*)FIELD_init, wrappedTEA});
  Custom* dummyFlags = &Unit;
  A1(wrappedInit, dummyFlags);
  Cons* portList = Platform_initializeEffects();
  return portList;
}

void test_app_startup() {
  mock_app_main();

  allocateJsRef(NULL); // leave slot 0 unused to avoid false test pass
  allocateJsRef(&mock_js_Browser_element); // slot 1

  mock_app_init_globals();

  Cons* portList = mock_js_Platform_initialize();
  (void)portList;
}
