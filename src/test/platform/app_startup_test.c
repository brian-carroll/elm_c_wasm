#include "../test.h"
#include "../../kernel/kernel.h"

void mock_app_init_globals();
int mock_app_main();

extern Closure* gptr_author_project_Main_main;
u32 allocateJsRef(void* value);
extern JsRef Browser_element;
extern DynamicArray* mock_tasks_executed;

Closure mock_js_Browser_element = {
    .header = HEADER_CLOSURE(0),
    .max_values = 4,
    .evaluator = NULL, // this mock never gets all of its args (would be in JS)
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

void* init_mock_tasks_executed() {
  return newDynamicArray(6);
}


/**
 * Test Platform initialization C code, without any JS.
 */
void test_app_startup() {
  mock_app_main();

  GC_init_root((void**)&mock_tasks_executed, &init_mock_tasks_executed);
  mock_app_init_globals();

  Cons* portList = mock_js_Platform_initialize();
  (void)portList;

  mu_expect_equal("should execute the correct number of tasks", mock_tasks_executed->occupied, 6);
  ElmString** names = (ElmString**)mock_tasks_executed->values;
  expect_equal("should execute Browser_Events_init", names[0], create_string("Browser_Events_init"));
  expect_equal("should execute Http_init", names[1], create_string("Http_init"));
  expect_equal("should execute Task_init", names[2], create_string("Task_init"));
  expect_equal("should execute Browser_Events_onEffects", names[3], create_string("Browser_Events_onEffects"));
  expect_equal("should execute Http_onEffects", names[4], create_string("Http_onEffects"));
  expect_equal("should execute Task_onEffects", names[5], create_string("Task_onEffects"));
}
