#include <emscripten/emscripten.h>
#include "../../../src/kernel/basics.h"
#include "../../../src/kernel/gc.h"
#include "../../../src/kernel/types.h"
#include "../../../src/kernel/utils.h"

void* eval_tce_count(void* args[1], void** gc_tce_data) {
  while (1) {
    void* remaining = args[0];
    const ElmInt literal_0 = {
        .header = HEADER_INT,
        .value = 0,
    };
    const ElmInt literal_1 = {
        .header = HEADER_INT,
        .value = 1,
    };
    if (A2(&Utils_eq, remaining, (void*)&literal_0) == &True) {
      return &Unit;
    } else {
      void* next_remaining = A2(&Basics_sub, remaining, (void*)&literal_1);
      CAN_THROW(GC_tce_iteration(1, gc_tce_data));
      args[0] = next_remaining;
    }
  }
}

const Closure count;  // pre-declaration
void* eval_count(void* args[1]) {
  return GC_tce_eval(&eval_tce_count, &count, args);
}
const Closure count = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_count,
    .max_values = 1,
};

int EMSCRIPTEN_KEEPALIVE export_count(int fromJS) {
  ElmInt* remaining = NEW_ELM_INT(fromJS);
  void* args[1] = {remaining};
  ElmValue* result;
  while (1) {
    result = Utils_apply(&count, 1, args);
    if (result->header.tag != Tag_GcException) break;
    GC_collect_onexception_full(1, args);
  }
  return 0;
}
