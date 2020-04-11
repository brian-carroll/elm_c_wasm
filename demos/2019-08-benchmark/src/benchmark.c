#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#else
#define EMSCRIPTEN_KEEPALIVE
#endif
// #include <stdio.h>
#include "../../../src/kernel/basics.h"
#include "../../../src/kernel/debug.h"
#include "../../../src/kernel/gc-internals.h"
#include "../../../src/kernel/gc.h"
#include "../../../src/kernel/types.h"
#include "../../../src/kernel/utils.h"

extern GcState gc_state;

ElmInt literal_0 = {
    .header = HEADER_INT,
    .value = 0,
};
ElmInt literal_1 = {
    .header = HEADER_INT,
    .value = 1,
};

void* eval_tce_count(void* args[1], void** gc_tce_data) {
  while (1) {
    ElmInt* remaining = args[0];
    // printf("tce loop, remaining = %d\n", remaining->value);
    if (A2(&Utils_equal, remaining, (void*)&literal_0) == &True) {
      // printf("tce returning Unit at %p with tag %d\n", &Unit, Unit.header.tag);
      return &Unit;
    } else {
      ElmInt* next_remaining = A2(&Basics_sub, remaining, (void*)&literal_1);
      *gc_tce_data = GC_tce_iteration(1);
      if (gc_tce_data == pGcFull) {
        return pGcFull;
      }
      args[0] = next_remaining;
    }
  }
}

void* eval_count(void* args[1]) {
  return GC_tce_eval(&eval_tce_count, &eval_count, 1, args);
}
Closure count = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_count,
    .max_values = 1,
};

// int export_count_call_id = 0;
// int gc_id = 0;

int EMSCRIPTEN_KEEPALIVE export_count(int fromJS) {
  ElmValue* result;

  // export_count_call_id++;
  // int gc_id_on_call = gc_id;
  // printf("export_count: call %d with remaining=%d\n", export_count_call_id, fromJS);

  if (GC_stack_empty() == pGcFull) {
    // printf("export_count: GC point 1, call %d, gc %d\n", export_count_call_id,
    // ++gc_id);
    GC_collect_full();
    GC_stack_empty();
  }
  while (1) {
    void* args[1];
    ElmInt* remaining = ctorElmInt(fromJS);
    args[0] = remaining;
    if (remaining->header.tag == Tag_GcException) {
      // printf("export_count: GC point 2, call=%d, gc=%d\n", export_count_call_id,
      // ++gc_id);
      GC_collect_full();
      GC_prep_replay();
      continue;
    }
    result = Utils_apply(&count, 1, args);
    if (result->header.tag != Tag_GcException) break;
    // printf("export_count: GC point 3, call=%d, gc=%d\n", export_count_call_id,
    // ++gc_id);
    // if (gc_id - gc_id_on_call > 10) {
    //   log_error("Too many gc cycles\n");
    // }
    GC_collect_full();
    GC_prep_replay();
  }
  return (int)result->header.tag;  // makes no sense but it's the right type
}

Closure count_no_tce;  // pre-declaration for circular dependency
// int level = 0;

void* eval_count_no_tce(void* args[]) {
  // level++;
  ElmInt* remaining = args[0];
  // printf("eval_count_no_tce level %d, remaining=%d\n", level, remaining->value);
  if (A2(&Utils_equal, remaining, &literal_0) == &True) {
    // printf("eval_count_no_tce returning Unit at %p with tag %d\n", &Unit,
    //  Unit.header.tag);
    return &Unit;
  } else {
    ElmInt* next_remaining = A2(&Basics_sub, remaining, &literal_1);
    return A1(&count_no_tce, next_remaining);
  }
}

Closure count_no_tce = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_count_no_tce,
    .max_values = 1,
};

// int export_count_no_tce_call_id = 0;

int EMSCRIPTEN_KEEPALIVE export_count_no_tce(int fromJS) {
  // level = 0;
  // export_count_no_tce_call_id++;
  // printf("export_count_no_tce: call %d remaining=%d\n", export_count_no_tce_call_id,
  //        fromJS);

  GC_stack_empty();

  ElmValue* result;
  while (1) {
    ElmInt* remaining = ctorElmInt(fromJS);
    if (remaining->header.tag == Tag_GcException) {
      // printf("export_count_no_tce: GC point 1, call=%d, gc=%d\n",
      //        export_count_no_tce_call_id, ++gc_id);
      GC_collect_full();
      GC_prep_replay();
      continue;
    }
    result = Utils_apply(&count_no_tce, 1, (void* []){remaining});
    if (result->header.tag != Tag_GcException) break;
    // printf("export_count_no_tce: GC point 2, call=%d, gc=%d\n",
    //     export_count_no_tce_call_id,
    //     ++gc_id);
    GC_collect_full();
    GC_prep_replay();
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
      GC_collect_full();
      GC_prep_replay();
    } else {
      // print_state();
      // printf("stack_map->older = %p\n", gc_state.stack_map->older);
      // printf("export_add: returning with %d\n", result->value);
      return result->value;
    }
  }
}

void EMSCRIPTEN_KEEPALIVE dump() {
  print_heap();
  print_state();
}

int EMSCRIPTEN_KEEPALIVE export_add_unboxed(int a, int b) {
  return a + b;
}

int EMSCRIPTEN_KEEPALIVE main(int argc, char** argv) {
  int exit_code = GC_init();

#ifndef __EMSCRIPTEN__
  while (1) {
    for (int i = 0; i < 10; i++) {
      export_count(10);
    }
    for (int i = 0; i < 10; i++) {
      export_count_no_tce(10);
    }
  }
#endif

  return exit_code;
}
