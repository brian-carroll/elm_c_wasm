#include "core.h"

void* eval_Process_sleep(void* args[]) {
  ElmFloat* time = args[0];
  Task* t = Wrapper_sleep(time->value);
  return t;
}
Closure Process_sleep = {
    .header = HEADER_CLOSURE(0),
    .max_values = 1,
    .evaluator = eval_Process_sleep,
};
