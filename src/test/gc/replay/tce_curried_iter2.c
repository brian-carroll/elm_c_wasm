#include "../replay_test.h"

static ElmInt zero = {
    .header = HEADER_INT,
    .value = 0,
};

static Closure partial_call = {
    .header = HEADER_CLOSURE(1),
    .max_values = 2,
    .n_values = 1,
    .evaluator = &eval_mock_func_tail,
    .values = {&zero},
};

static Closure full_call = {
    .header = HEADER_CLOSURE(2),
    .max_values = 2,
    .n_values = 2,
    .evaluator = &eval_mock_func_tail,
    .values = {&zero, NULL},
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

char* test_replay_tce_curried_iter2() {
  if (verbose) {
    printf(
        "\n"
        "## test_replay_tce_curried_iter2\n"
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
  void* curried = Utils_apply(&mock_func_tail, 1, (void* []){&zero});
  void* result1 = Utils_apply(curried, 1, (void* []){NULL});
  mu_assert("Throws exception", result1 == pGcFull);

  void* h = gc_state.heap.start;

  void* int4 = h + 3 * sizeof(GcStackMap) + 3 * sizeof(Closure) + 5 * sizeof(void*) +
               2 * sizeof(ElmInt);
  closure_spec_iter1.values[0] = int4;
  closure_spec_iter2.values[0] = int4;

  void* push = h + sizeof(GcStackMap) + 2 * sizeof(Closure) + 3 * sizeof(void*);

  size_t zero = 0;

  // HEAP BEFORE GC
  void* heap_before_spec[] = {
      &(GcStackMap){
          .header = HEADER_GC_STACK_EMPTY,
      },
      &partial_call,
      &full_call,
      &(GcStackMap){
          .header = HEADER_GC_STACK_PUSH,
          .older = h,
      },
      &closure_spec_iter1,
      &zero,
      &zero,
      &zero,
      &zero,
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
          .older = push,
          .replay = int4 + sizeof(ElmInt),
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
  Utils_apply(&mock_func_tail, 1, (void* []){&zero});
  Utils_apply(curried, 1, (void* []){NULL});

  // moved pointers
  int4 = push + sizeof(GcStackMap);  // push is still in the same place
  closure_spec_iter2.values[0] = int4;

  // HEAP AFTER GC
  void* heap_after_spec[] = {
      &(GcStackMap){
          .header = HEADER_GC_STACK_EMPTY,
          .newer = push,
      },
      &partial_call,
      &full_call,
      &(GcStackMap){
          .header = HEADER_GC_STACK_PUSH,
          .older = h,
          .newer = int4 + sizeof(ElmInt) + sizeof(Closure) + 2 * sizeof(void*),
      },
      &(ElmInt){
          .header = HEADER_INT,
          .value = 4,
      },
      &closure_spec_iter2,
      &(GcStackMap){
          .header = HEADER_GC_STACK_TC,
          .older = push,
          .replay = int4 + sizeof(ElmInt),
          .newer = int4 + sizeof(ElmInt) + sizeof(Closure) + 2 * sizeof(void*) +
                   sizeof(GcStackMap) + 2 * sizeof(ElmInt),
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
  mu_expect_equal("GC State stackmap",
      (void*)gc_state.stack_map,
      int4 + sizeof(ElmInt) + sizeof(Closure) + 2 * sizeof(void*));
  return NULL;
}
