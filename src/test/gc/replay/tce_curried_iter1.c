#include "../replay_test.h"

static void* tce_mock_curried(void* args[], void** gc_tce_data) {
  static bool should_throw = true;
tce_loop:;
  ElmInt* count = args[0];
  ElmInt* max_count = args[1];
  void* tmp0;
  do {
    if (count->value >= max_count->value) {
      // Throw again at the end so we can examine GC state
      tmp0 = pGcFull;
      break;
    } else {
      NEW_ELM_INT(count->value + 1);
      NEW_ELM_INT(count->value + 2);
      if (should_throw) {
        should_throw = false;
        return pGcFull;
      }
      void* tmp0 = NEW_ELM_INT(count->value + 3);
      *gc_tce_data = CAN_THROW(GC_tce_iteration(2));
      args[0] = tmp0;
      goto tce_loop;
      break;
    }
  } while (0);
  return tmp0;
}

static void* eval_mock_curried(void* args[2]) {
  return GC_tce_eval(&tce_mock_curried, &eval_mock_curried, 2, args);
}
static Closure mock_tce_sat = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0,
    .max_values = 2,
    .evaluator = &eval_mock_curried,
};

static ElmInt arg_init_count = {
    .header = HEADER_INT,
    .value = 0,
};

static ElmInt arg_max_count = {
    .header = HEADER_INT,
    .value = 3,
};

static Closure partial_spec = {
    .header = HEADER_CLOSURE(1),
    .max_values = 2,
    .n_values = 1,
    .evaluator = &eval_mock_curried,
    .values = {&arg_init_count},
};

static Closure full_spec = {
    .header = HEADER_CLOSURE(2),
    .max_values = 2,
    .n_values = 2,
    .evaluator = &eval_mock_curried,
    .values = {&arg_init_count, &arg_max_count},
};

static Closure full_spec2 = {
    .header = HEADER_CLOSURE(2),
    .max_values = 2,
    .n_values = 2,
    .evaluator = &eval_mock_curried,
    .values = {&arg_init_count, &arg_max_count},
};

char* test_replay_tce_curried_iter1() {
  if (verbose) {
    printf(
        "\n"
        "## test_replay_tce_curried_iter1\n"
        "\n");
  }

  // SETUP
  gc_test_reset();

  // RUN
  void* partial = Utils_apply(&mock_tce_sat, 1, (void* []){&arg_init_count});
  void* result1 = Utils_apply(partial, 1, (void* []){&arg_max_count});
  mu_assert("Throws exception", result1 == pGcFull);

  void* h = gc_state.heap.start;

  void* push = h + sizeof(GcStackMap) + (sizeof(Closure) + sizeof(void*)) +
               (sizeof(Closure) + 2 * sizeof(void*));

  // HEAP BEFORE GC
  void* heap_before_spec[] = {
      &(GcStackMap){
          .header = HEADER_GC_STACK_EMPTY,
      },
      &partial_spec,
      &full_spec,
      &(GcStackMap){
          .header = HEADER_GC_STACK_PUSH,
          .older = h,
      },
      &full_spec,
      &(GcStackMap){
          .header = HEADER_GC_STACK_TC,
          .older = push,
          .replay = push + sizeof(GcStackMap),
      },
      &(ElmInt){.header = HEADER_INT, .value = 1},
      &(ElmInt){.header = HEADER_INT, .value = 2},
      NULL,
  };
  char* heap_err_before_gc = assert_heap_values("Heap before GC", heap_before_spec);
  if (heap_err_before_gc) return heap_err_before_gc;

  // GC + REPLAY
  GC_collect_full();
  GC_prep_replay();
  partial = Utils_apply(&mock_tce_sat, 1, (void* []){&arg_init_count});
  Utils_apply(partial, 1, (void* []){&arg_max_count});

  mark(&gc_state, gc_state.heap.start);  // make it easier to understand print_heap

  void* tc1 = push + sizeof(GcStackMap) + (sizeof(Closure) + 2 * sizeof(void*));
  void* iter1_result = tc1 + sizeof(GcStackMap) + 2 * sizeof(ElmInt);

  full_spec2.values[0] = iter1_result;

  // HEAP AFTER GC
  void* heap_after_spec[] = {
      &(GcStackMap){
          .header = HEADER_GC_STACK_EMPTY,
          .newer = push,
      },
      &partial_spec,
      &full_spec,
      &(GcStackMap){
          .header = HEADER_GC_STACK_PUSH,
          .older = h,
          .newer = tc1,
      },
      &full_spec2,
      &(GcStackMap){
          .header = HEADER_GC_STACK_TC,
          .older = push,
          .replay = tc1 - (sizeof(Closure) + 2 * sizeof(void*)),
          .newer = (void*)GARBAGE,
      },
      &(ElmInt){.header = HEADER_INT, .value = 1},
      &(ElmInt){.header = HEADER_INT, .value = 2},
      &(ElmInt){.header = HEADER_INT, .value = 3},  // proves execution resumed
      &full_spec2,
      &(GcStackMap){
          .header = HEADER_GC_STACK_TC,
          .older = push,
          .replay = iter1_result + sizeof(ElmInt),
      },
      NULL,
  };

  char* heap_err_after = assert_heap_values("Heap after GC", heap_after_spec);
  if (heap_err_after) return heap_err_after;

  mu_expect_equal("GC State replay_ptr", gc_state.replay_ptr, NULL);
  mu_expect_equal("GC State stack_depth", gc_state.stack_depth, 1);
  mu_expect_equal("GC State stackmap",
      (void*)gc_state.stack_map,
      iter1_result + sizeof(ElmInt) + (sizeof(Closure) + 2 * sizeof(void*)));

  return NULL;
}
