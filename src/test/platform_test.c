#include "./test.h"

#include "./platform/scheduler_test.c"

#include "./platform/mock-generated-code.c"

#include "./platform/app_startup_test.c"
#include "./platform/app_update_test.c"


void platform_test() {
  if (verbose) {
    safe_printf("\n\n\n");
    safe_printf("####################################################\n");
    safe_printf("\n");
    safe_printf("Platform\n");
    safe_printf("========\n");
  }
  describe("test_scheduler", test_scheduler);
  // describe("test_app_startup", test_app_startup);
  //describe("test_app_update", test_app_update);// TODO: write some tests
}
