#include "../replay_test.h"

void* eval_mock_func_saturated(void* args[]) {
  static bool is_first_call = true;
  printf("eval_mock_func_saturated, is_first_call=%d\n", is_first_call);
  NEW_ELM_INT(TEST_INT_OFFSET + 1);
  NEW_ELM_INT(TEST_INT_OFFSET + 2);
  if (is_first_call) {
    is_first_call = false;
    return pGcFull;
  }
  NEW_ELM_INT(TEST_INT_OFFSET + 3);
  return pGcFull;
}

Closure mock_func_saturated = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0,
    .max_values = 1,
    .evaluator = &eval_mock_func_saturated,
};

char* test_replay_saturated() {
  if (verbose) {
    printf(
        "\n"
        "## test_replay_saturated\n"
        "\n");
  }

  // SETUP
  gc_test_reset();

  // RUN
  void* result1 = Utils_apply(&mock_func_saturated, 1, (void*[]){&Unit});
  mu_assert("Throws exception", result1 == pGcFull);

  void* h = gc_state.heap.start;

  // HEAP BEFORE GC
  void* heap_before_spec[] = {
      &(GcStackMap){
          .header = HEADER_GC_STACK_EMPTY,
      },
      &(GcStackMap){
          .header = HEADER_GC_STACK_PUSH,
          .older = h,
      },
      &(ElmInt){
          .header = HEADER_INT,
          .value = TEST_INT_OFFSET + 1,
      },
      &(ElmInt){
          .header = HEADER_INT,
          .value = TEST_INT_OFFSET + 2,
      },
      NULL,
  };
  char* heap_err_before_gc = assert_heap_values("Heap before GC", heap_before_spec);
  if (heap_err_before_gc) return heap_err_before_gc;

  // GC + REPLAY
  GC_collect_full();
  GC_prep_replay();

  Utils_apply(&mock_func_saturated, 1, (void*[]){&Unit});

  // HEAP AFTER GC
  void* heap_after_spec[] = {
      &(GcStackMap){
          .header = HEADER_GC_STACK_EMPTY,
          .newer = h + sizeof(GcStackMap),
      },
      &(GcStackMap){
          .header = HEADER_GC_STACK_PUSH,
          .older = h,
          .newer = h + 2 * sizeof(GcStackMap) + 2 * sizeof(ElmInt),
      },
      &(ElmInt){
          .header = HEADER_INT,
          .value = TEST_INT_OFFSET + 1,
      },
      &(ElmInt){
          .header = HEADER_INT,
          .value = TEST_INT_OFFSET + 2,
      },
      // Prove the function has resumed executing
      &(ElmInt){
          .header = HEADER_INT,
          .value = TEST_INT_OFFSET + 3,
      },
      NULL,
  };

  char* heap_err_after = assert_heap_values("Heap after GC", heap_after_spec);
  if (heap_err_after) return heap_err_after;

  mu_expect_equal("exited replay mode", gc_state.replay_ptr, NULL);
  mu_expect_equal("resumed at correct stack_depth", gc_state.stack_depth, 1);
  mu_expect_equal("stackmap pointing at the right place",
      (void*)gc_state.stack_map,
      h + sizeof(GcStackMap));

  return NULL;
}
