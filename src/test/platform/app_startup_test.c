#include "../test.h"

extern Closure * gptr_author_project_Main_main;

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

  Cons* portList = mock_js_Platform_initialize();
  (void)portList;
}
