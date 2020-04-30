#include "./types.h"
#include "./wrapper/wrapper.h"

static void* eval_run(void* args[]) {
  size_t jsIndex = (size_t)args[0];
  return Wrapper_callJsSync(jsIndex, 2, &args[1]);
}
Closure Json_run = {
    .header = HEADER_CLOSURE(0),
    .n_values = 1,
    .max_values = 3,
    .evaluator = &eval_run,
    .values = {NULL},
};

static void* eval_runOnString(void* args[]) {
  size_t jsIndex = (size_t)args[0];
  return Wrapper_callJsSync(jsIndex, 2, &args[1]);
}
Closure Json_runOnString = {
    .header = HEADER_CLOSURE(0),
    .n_values = 1,
    .max_values = 3,
    .evaluator = &eval_runOnString,
    .values = {NULL},
};
