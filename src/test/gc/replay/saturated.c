#include "../replay_test.h"

char* test_replay_saturated() {
  if (verbose) {
    printf(
        "\n"
        "## test_replay_saturated\n"
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
  void* result1 = Utils_apply(&mock_func, 2, (void* []){NULL, NULL});
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
  GC_prep_replay();
  Utils_apply(&mock_func, 2, (void* []){NULL, NULL});

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

  mu_assert("GC State replay_ptr", gc_state.replay_ptr == NULL);
  mu_assert("GC State stack_depth", gc_state.stack_depth == 1);
  mu_assert("GC State stackmap",
      (void*)gc_state.stack_map - (void*)gc_state.heap.start == sizeof(GcStackMap));

  return NULL;
}
