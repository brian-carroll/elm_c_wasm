#include "../replay_test.h"

static const Closure partial_spec = {
    .header = HEADER_CLOSURE(1),
    .max_values = 2,
    .n_values = 1,
    .evaluator = &eval_mock_func,
    .values[0] = NULL,
};

static const Closure full_spec = {
    .header = HEADER_CLOSURE(2),
    .max_values = 2,
    .n_values = 2,
    .evaluator = &eval_mock_func,
    .values[0] = NULL,
    .values[1] = NULL,
};

char* test_replay_curried() {
  if (verbose) {
    printf(
        "\n"
        "## test_replay_curried\n"
        "\n");
  }

  // SETUP
  gc_test_reset();
  memcpy(mock_func_ops,
      (Tag[]){
          Tag_Int,
          Tag_Int,
          Tag_GcException,
      },
      sizeof(Tag[4]));

  // RUN
  void* curried = Utils_apply(&mock_func, 1, (void* []){NULL});
  void* result1 = Utils_apply(curried, 1, (void* []){NULL});
  mu_assert("Throws exception", result1 == pGcFull);

  // HEAP BEFORE GC
  const void* heap_before_spec[] = {
      &(GcStackMap){
          .header = HEADER_GC_STACK_EMPTY,
      },
      &partial_spec,
      &full_spec,
      &(GcStackMap){
          .header = HEADER_GC_STACK_PUSH,
          .older =
              (void*)(-sizeof(GcStackMap) - (2 * sizeof(Closure)) - (3 * sizeof(void*))),
      },
      &(ElmInt){
          .header = HEADER_INT,
          .value = INT_OFFSET,
      },
      &(ElmInt){
          .header = HEADER_INT,
          .value = INT_OFFSET + 1,
      },
      NULL,
  };
  char* heap_err_before_gc = assert_heap_values("Heap before GC", heap_before_spec);
  if (heap_err_before_gc) return heap_err_before_gc;

  // GC + REPLAY
  GC_collect_full();
  GC_start_replay();
  Utils_apply(&mock_func, 1, (void* []){NULL});
  Utils_apply(curried, 1, (void* []){NULL});

  // HEAP AFTER GC
  const void* heap_after_spec[] = {
      &(GcStackMap){
          .header = HEADER_GC_STACK_EMPTY,
          .newer =
              (void*)(sizeof(GcStackMap) + (2 * sizeof(Closure)) + (3 * sizeof(void*))),
      },
      &partial_spec,
      &full_spec,
      &(GcStackMap){
          .header = HEADER_GC_STACK_PUSH,
          .older =
              (void*)(-sizeof(GcStackMap) - (2 * sizeof(Closure)) - (3 * sizeof(void*))),
          .newer = (void*)(sizeof(GcStackMap) + 2 * sizeof(ElmInt)),
      },
      &(ElmInt){
          .header = HEADER_INT,
          .value = INT_OFFSET,
      },
      &(ElmInt){
          .header = HEADER_INT,
          .value = INT_OFFSET + 1,
      },
      NULL,
  };

  char* heap_err_after = assert_heap_values("Heap after GC", heap_after_spec);
  if (heap_err_after) return heap_err_after;

  print_heap();
  print_state();

  mu_assert("GC State replay_ptr", gc_state.replay_ptr == NULL);
  mu_assert("GC State stack_depth", gc_state.stack_depth == 1);
  mu_assert("GC State stackmap",
      (void*)gc_state.stack_map - (void*)gc_state.heap.start == sizeof(GcStackMap));

  return NULL;
}
