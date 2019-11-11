#include "../replay_test.h"

static ElmInt zero = {
    .header = HEADER_INT,
    .value = 0,
};

static Closure closure_spec_iter1 = {
    .header = HEADER_CLOSURE(2),
    .max_values = 2,
    .n_values = 2,
    .evaluator = &eval_mock_func_tail,
    .values = {NULL, NULL},
};

static Closure closure_spec_iter2 = {
    .header = HEADER_CLOSURE(2),
    .max_values = 2,
    .n_values = 2,
    .evaluator = &eval_mock_func_tail,
    .values = {NULL, NULL},
};

char* test_replay_tce_saturated_iter2() {
  if (verbose) {
    printf(
        "\n"
        "## test_replay_tce_saturated_iter2\n"
        "\n");
  }

  // SETUP
  gc_test_reset();
  memcpy(mock_func_ops,
      (Tag[]){
          Tag_Int,
          Tag_Int,
          Tag_Int,
          Tag_GcStackTailCall,
          Tag_Int,
          Tag_Int,
          Tag_GcException,
      },
      sizeof(Tag[7]));

  // RUN
  void* result1 = Utils_apply(&mock_func_tail, 2, (void* []){&zero, NULL});
  mu_assert("Throws exception", result1 == pGcFull);

  void* h = gc_state.heap.start;

  void* int4 = h + 3 * sizeof(GcStackMap) + sizeof(Closure) + 2 * sizeof(void*) +
               2 * sizeof(ElmInt);
  closure_spec_iter1.values[0] = int4;  // points into future due to mutate & copy
  closure_spec_iter2.values[0] = int4;

  // HEAP BEFORE GC
  void* heap_before_spec[] = {
      &(GcStackMap){
          .header = HEADER_GC_STACK_EMPTY,
      },
      &(GcStackMap){
          .header = HEADER_GC_STACK_PUSH,
          .older = h,
      },
      &closure_spec_iter1,
      &(GcStackMap){
          .header = HEADER_GC_STACK_TC,
          .older = NULL,  // never gets populated
          .replay = h + 2 * sizeof(GcStackMap),
      },
      &(ElmInt){
          .header = HEADER_INT,
          .value = 0,
      },
      &(ElmInt){
          .header = HEADER_INT,
          .value = 1,
      },
      &(ElmInt){
          .header = HEADER_INT,
          .value = 4,
      },
      &closure_spec_iter2,
      &(GcStackMap){
          .header = HEADER_GC_STACK_TC,
          .older = h + sizeof(GcStackMap),
          .replay = h + 3 * sizeof(GcStackMap) + sizeof(Closure) + 2 * sizeof(void*) +
                    3 * sizeof(ElmInt),
      },
      &(ElmInt){
          .header = HEADER_INT,
          .value = 4,
      },
      &(ElmInt){
          .header = HEADER_INT,
          .value = 5,
      },
      NULL,
  };
  char* heap_err_before_gc = assert_heap_values("Heap before GC", heap_before_spec);
  if (heap_err_before_gc) return heap_err_before_gc;

  // GC + REPLAY
  GC_collect_full();
  GC_start_replay();
  Utils_apply(&mock_func_tail, 2, (void* []){&zero, NULL});

  // pointer gets moved
  closure_spec_iter2.values[0] = h + 2 * sizeof(GcStackMap);

  // HEAP AFTER GC
  void* heap_after_spec[] = {
      &(GcStackMap){
          .header = HEADER_GC_STACK_EMPTY,
          .newer = h + sizeof(GcStackMap),
      },
      &(GcStackMap){
          .header = HEADER_GC_STACK_PUSH,
          .older = h,
          .newer = h + 2 * sizeof(GcStackMap) + sizeof(ElmInt) + sizeof(Closure) +
                   2 * sizeof(void*),
      },
      &(ElmInt){
          .header = HEADER_INT,
          .value = 4,
      },
      &closure_spec_iter2,
      &(GcStackMap){
          .header = HEADER_GC_STACK_TC,
          .older = h + sizeof(GcStackMap),
          .replay = h + 2 * sizeof(GcStackMap) + sizeof(ElmInt),
          .newer = h + 3 * sizeof(GcStackMap) + 3 * sizeof(ElmInt) + sizeof(Closure) +
                   2 * sizeof(void*),
      },
      &(ElmInt){
          .header = HEADER_INT,
          .value = 4,
      },
      &(ElmInt){
          .header = HEADER_INT,
          .value = 5,
      },
      NULL,
  };

  char* heap_err_after = assert_heap_values("Heap after GC", heap_after_spec);
  if (heap_err_after) return heap_err_after;

  mu_assert("GC State replay_ptr", gc_state.replay_ptr == NULL);
  mu_assert("GC State stack_depth", gc_state.stack_depth == 1);
  mu_assert("GC State stackmap",
      (void*)gc_state.stack_map - (void*)gc_state.heap.start ==
          2 * sizeof(GcStackMap) + sizeof(ElmInt) + sizeof(Closure) + 2 * sizeof(void*));

  return NULL;
}
