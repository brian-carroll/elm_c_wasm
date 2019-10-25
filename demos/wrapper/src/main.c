#include "../../../src/kernel/kernel.h"

enum {
  JS_Json_succeed,
  JS_Platform_batch,
  JS_Platform_leaf,
  JS_Scheduler_andThen,
  JS_Scheduler_succeed,
  JS_Time_now,
  JS_VirtualDom_node,
  JS_VirtualDom_on,
  JS_VirtualDom_text
}

int EMSCRIPTEN_KEEPALIVE
main(int argc, char** argv) {
  int exit_code = GC_init();
  if (exit_code) return exit_code;
  // initialise GC roots and stuff

  return exit_code;
}