#include "../../gc_test.h"
#include "../../test.h"
#include "../replay_test.h"

void* eval_mock_func_inner(void* args[]) {
  NEW_ELM_INT(TEST_INT_OFFSET + 1);
  NEW_ELM_INT(TEST_INT_OFFSET + 2);
  return NEW_ELM_INT(TEST_INT_OFFSET + 3);
}

Closure mock_func_inner = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0,
    .max_values = 1,
    .evaluator = &eval_mock_func_inner,
};

void* eval_mock_func_outer(void* args[]) {
  static bool is_first_call = true;
  A1(&mock_func_inner, &Unit);
  // outer function fails to allocate after nested function returns
  if (is_first_call) {
    is_first_call = false;
    return pGcFull;
  }
  NEW_ELM_INT(TEST_INT_OFFSET + 4);
  return pGcFull;  // stop here so we can examine the GC state
}

Closure mock_func_outer = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0,
    .max_values = 1,
    .evaluator = &eval_mock_func_outer,
};

char* test_replay_finished() {
  if (verbose) {
    printf(
        "\n"
        "## test_gc_replay_finished\n"
        "\n");
  }

  // SETUP
  gc_test_reset();

  // RUN
  Utils_apply(&mock_func_outer, 1, (void* []){&Unit});
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
      &(GcStackMap){
          .header = HEADER_GC_STACK_PUSH,
          .older = h + sizeof(GcStackMap),
      },
      &(ElmInt){
          .header = HEADER_INT,
          .value = TEST_INT_OFFSET + 1,
      },
      &(ElmInt){
          .header = HEADER_INT,
          .value = TEST_INT_OFFSET + 2,
      },
      &(ElmInt){
          .header = HEADER_INT,
          .value = TEST_INT_OFFSET + 3,
      },
      &(GcStackMap){
          .header = HEADER_GC_STACK_POP,
          .older = h + 2 * sizeof(GcStackMap),
          .replay = h + 3 * sizeof(GcStackMap) + 2 * sizeof(ElmInt),
      },
      NULL,
  };
  char* heap_err_before_gc = assert_heap_values("heap before GC", heap_before_spec);
  if (heap_err_before_gc) return heap_err_before_gc;

  mu_expect_equal("should be at expected stack_depth", gc_state.stack_depth, 1);
  mu_expect_equal("stackmap pointer should be as expected",
      (void*)gc_state.stack_map - (void*)gc_state.heap.start,
      3 * sizeof(GcStackMap) + 3 * sizeof(ElmInt));

  // GC + REPLAY
  GC_collect_full();
  GC_prep_replay();

  mu_assert("should be in replay mode", gc_state.replay_ptr != NULL);
  mu_expect_equal("should have reset stack_depth for replay", gc_state.stack_depth, 0);
  mu_expect_equal("should have reset stackmap pointer for replay",
      (void*)gc_state.stack_map,
      (void*)gc_state.heap.start);

  Utils_apply(&mock_func_outer, 1, (void* []){&Unit});

  // HEAP AFTER GC & REPLAY
  void* heap_after_spec[] = {
      &(GcStackMap){
          .header = HEADER_GC_STACK_EMPTY,
          .newer = h + sizeof(GcStackMap),
      },
      &(GcStackMap){
          .header = HEADER_GC_STACK_PUSH,
          .older = h,
          .newer = h + 2 * sizeof(GcStackMap),
      },
      &(GcStackMap){
          .header = HEADER_GC_STACK_PUSH,
          .older = h + sizeof(GcStackMap),
          .newer = h + 3 * sizeof(GcStackMap) + sizeof(ElmInt),
      },
      // Absence of first two Ints proves the inner function is not re-executed
      &(ElmInt){
          .header = HEADER_INT,
          .value = TEST_INT_OFFSET + 3,
      },
      &(GcStackMap){
          .header = HEADER_GC_STACK_POP,
          .older = h + 2 * sizeof(GcStackMap),
          .replay = h + 3 * sizeof(GcStackMap),
          .newer = h + 4 * sizeof(GcStackMap) + sizeof(ElmInt),
      },
      // Existence of next int proves the outer function has resumed
      &(ElmInt){
          .header = HEADER_INT,
          .value = TEST_INT_OFFSET + 4,
      },
      NULL,
  };
  char* heap_err_after = assert_heap_values("heap after GC & replay", heap_after_spec);
  if (heap_err_after) return heap_err_after;

  mu_expect_equal("exited replay mode", gc_state.replay_ptr, NULL);
  mu_expect_equal("resumed at correct stack_depth", gc_state.stack_depth, 1);
  mu_expect_equal("stackmap pointing at the right place",
      (void*)gc_state.stack_map - (void*)gc_state.heap.start,
      3 * sizeof(GcStackMap) + sizeof(ElmInt));

  return NULL;
}
