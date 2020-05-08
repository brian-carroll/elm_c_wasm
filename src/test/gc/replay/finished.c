#include "../replay_test.h"

char* test_replay_finished() {
  if (verbose) {
    printf(
        "\n"
        "## test_gc_replay_finished\n"
        "\n");
  }

  // SETUP
  gc_test_reset();
  memcpy(mock_func_ops,
      (Tag[]){
          Tag_GcStackPush,
          Tag_Int,
          Tag_Int,
          Tag_Int,
          Tag_GcStackPop,
          Tag_GcException,
      },
      sizeof(Tag[6]));

  // RUN
  Utils_apply(&mock_func, 2, (void*[]){NULL, NULL});

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
          .value = INT_OFFSET + 1,
      },
      &(ElmInt){
          .header = HEADER_INT,
          .value = INT_OFFSET + 2,
      },
      &(ElmInt){
          .header = HEADER_INT,
          .value = INT_OFFSET + 3,
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

  // GC + REPLAY
  GC_collect_full();
  GC_prep_replay();

  Utils_apply(&mock_func, 2, (void*[]){NULL, NULL});

  // HEAP AFTER GC
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
      &(ElmInt){
          .header = HEADER_INT,
          .value = INT_OFFSET + 3,
      },
      &(GcStackMap){
          .header = HEADER_GC_STACK_POP,
          .older = h + 2 * sizeof(GcStackMap),
          .replay = h + 3 * sizeof(GcStackMap),
          .newer = h + 4 * sizeof(GcStackMap) + sizeof(ElmInt),
      },
      NULL,
  };
  char* heap_err_after = assert_heap_values("heap after GC & replay", heap_after_spec);
  if (heap_err_after) return heap_err_after;

  mu_expect_equal("GC State replay_ptr", gc_state.replay_ptr, NULL);
  mu_expect_equal("GC State stack_depth", gc_state.stack_depth, 1);
  mu_expect_equal("GC State stackmap",
      (void*)gc_state.stack_map - (void*)gc_state.heap.start,
      3 * sizeof(GcStackMap) + sizeof(ElmInt));

  return NULL;
}
