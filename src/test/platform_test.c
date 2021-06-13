#include "./test.h"

#include "./platform/initializeEffects_test.c"
#include "./platform/scheduler_test.c"
#include "./platform/sendToApp_test.c"


void platform_test() {
  if (verbose) {
    safe_printf("\n\n\n");
    safe_printf("####################################################\n");
    safe_printf("\n");
    safe_printf("Platform\n");
    safe_printf("========\n");
  }
  describe("test_scheduler", test_scheduler);
  describe("test_initializeEffects", test_initializeEffects);
  describe("test_sendToApp", test_sendToApp);
}
