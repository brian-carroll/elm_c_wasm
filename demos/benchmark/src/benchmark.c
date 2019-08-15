#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#else
#define EMSCRIPTEN_KEEPALIVE
#endif
// #include <stdio.h>
#include "../../../src/kernel/basics.h"
#include "../../../src/kernel/gc-internals.h"
#include "../../../src/kernel/gc.h"
#include "../../../src/kernel/types.h"
#include "../../../src/kernel/utils.h"
#include "../../../src/test/gc/print-heap.h"

extern GcState gc_state;

const ElmInt literal_0 = {
    .header = HEADER_INT,
    .value = 0,
};
const ElmInt literal_1 = {
    .header = HEADER_INT,
    .value = 1,
};

void* eval_tce_count(void* args[1], void** gc_tce_data) {
  // int i = 0;
  while (1) {
    ElmInt* remaining = args[0];
    // printf("tce loop, remaining = %d\n", remaining->value);
    if (A2(&Utils_eq, remaining, (void*)&literal_0) == &True) {
      // printf("tce returning Unit at %p with tag %d\n", &Unit, Unit.header.tag);
      return &Unit;
    } else {
      // i++;
      ElmInt* next_remaining = A2(&Basics_sub, remaining, (void*)&literal_1);
      void* maybeException = GC_tce_iteration(1, gc_tce_data);
      if (maybeException == pGcFull) {
        // printf("heap full after %d iterations\n", i);
        return maybeException;
      }
      args[0] = next_remaining;
    }
  }
}

const Closure count;  // pre-declaration for circular dependency
void* eval_count(void* args[1]) {
  return GC_tce_eval(&eval_tce_count, &count, args);
}
const Closure count = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_count,
    .max_values = 1,
};

int EMSCRIPTEN_KEEPALIVE export_count(int fromJS) {
  GC_stack_empty();

  ElmInt* remaining = ctorElmInt(fromJS);
  if (remaining == pGcFull) {
    GC_collect_onexception_full(0, (void* []){});
    remaining = ctorElmInt(fromJS);
  }

  void* args[1];
  args[0] = remaining;
  ElmValue* result = NULL;
  // printf("entering export_count with %d\n", fromJS);
  while (1) {
    // printf("\napply count to value %d at %p\n", fromJS, args[0]);
    result = Utils_apply(&count, 1, args);
    // printf("returning addr %p with tag %d\n", result, result->header.tag);
    if (result->header.tag != Tag_GcException) break;
    // printf("\n----- GC exception ------\n");
    // printf("Marking the heap before printing it out\n");
    // mark(&gc_state, gc_state.heap.start);
    // print_heap();
    // print_state();
    // printf("Collect garbage. remaining %p, tag=%d, value=%d\n", remaining,
    //  remaining->header.tag, remaining->value);
    GC_collect_onexception_full(1, args);
    // printf("finished GC_collect_onexception_full, remaining=%p\n", remaining);
    // print_heap();
    // print_state();
    // printf("marking cleaned up heap that is ready to run\n");
    // mark(&gc_state, gc_state.heap.start);
    // print_heap();
    // print_state();
    // printf("Done. remaining %p, tag=%d, value=%d\n", remaining,
    // remaining->header.tag,
    //        remaining->value);
    // printf("\n----- end of GC exception ------\n");
  }
  return (int)result->header.tag;  // makes no sense but it's the right type
}

int EMSCRIPTEN_KEEPALIVE export_add(int a, int b) {
  // printf("\n\n-----------------\n");
  // printf("export_add: entering\n");
  GC_stack_empty();
  while (1) {
    ElmInt* boxA = ctorElmInt(a);
    ElmInt* boxB = ctorElmInt(b);
    // printf("export_add: calling Basics_add with %d and %d\n", boxA->value,
    // boxB->value);
    // if (boxA->value != 123 || boxB->value != 456) {
    //   printf("oopsie");
    //   return -1;
    // }
    ElmInt* result = Utils_apply(&Basics_add, 2, (void* []){boxA, boxB});
    if (boxA == pGcFull || boxB == pGcFull || result == pGcFull) {
      // printf("export_add: GC\n");
      GC_collect_onexception_full(0, NULL);
    } else {
      // print_state();
      // printf("stack_map->older = %p\n", gc_state.stack_map->older);
      // printf("export_add: returning with %d\n", result->value);
      return result->value;
    }
  }
}

int EMSCRIPTEN_KEEPALIVE export_add_unboxed(int a, int b) {
  return a + b;
}

int EMSCRIPTEN_KEEPALIVE main(int argc, char** argv) {
  // printf("True: %p\n", &True);
  // printf("False: %p\n", &False);
  // printf("Unit: %p\n", &Unit);
  // printf("pGcFull: %p\n", pGcFull);
  // printf("eval_count: %p\n", &eval_count);
  // printf("entering main\n");
  GC_init();
  // while (export_add(123, 456) != -1)
  //   ;
  // export_count(100000);
  return 0;
}
