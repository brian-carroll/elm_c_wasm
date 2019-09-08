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
          Tag_Int,
          Tag_Int,
          Tag_Int,
          Tag_GcStackPop,
      },
      sizeof(Tag[4]));

  // RUN
  void* result1 = Utils_apply(&mock_func, 2, (void* []){NULL, NULL});
  ElmInt expected_result = {
      .header = HEADER_INT,
      .value = INT_OFFSET + 2,
  };
  bool return_val_ok =
      memcmp(result1, &expected_result, expected_result.header.size * SIZE_UNIT) == 0;
  if (!return_val_ok) {
    print_heap();
    print_state();
  }
  mu_assert("Return value", return_val_ok);

  // HEAP BEFORE GC
  const void* heap_before_spec[] = {
      &(GcStackMap){
          .header = HEADER_GC_STACK_EMPTY,
      },
      &(GcStackMap){
          .header = HEADER_GC_STACK_PUSH,
          .older = (void*)(-sizeof(GcStackMap)),
      },
      &(ElmInt){
          .header = HEADER_INT,
          .value = INT_OFFSET,
      },
      &(ElmInt){
          .header = HEADER_INT,
          .value = INT_OFFSET + 1,
      },
      &(ElmInt){
          .header = HEADER_INT,
          .value = INT_OFFSET + 2,
      },
      &(GcStackMap){
          .header = HEADER_GC_STACK_POP,
          .older = (void*)(-(3 * sizeof(ElmInt) + sizeof(GcStackMap))),
          .replay = (void*)(-sizeof(ElmInt)),
      },
      NULL,
  };
  char* heap_err_before_gc = assert_heap_values("heap before GC", heap_before_spec);
  if (heap_err_before_gc) return heap_err_before_gc;

  // GC + REPLAY
  GC_collect_full();
  GC_start_replay();
  void* result2 = Utils_apply(&mock_func, 2, (void* []){NULL, NULL});
  mu_assert("Replay return value",
      memcmp(result2, &expected_result, expected_result.header.size * SIZE_UNIT) == 0);

  // HEAP AFTER GC
  const void* heap_after_spec[] = {
      &(GcStackMap){
          .header = HEADER_GC_STACK_EMPTY,
          .newer = (void*)(sizeof(GcStackMap)),
      },
      &(GcStackMap){
          .header = HEADER_GC_STACK_PUSH,
          .older = (void*)(-sizeof(GcStackMap)),
          .newer = (void*)(sizeof(GcStackMap) + sizeof(ElmInt)),
      },
      &(ElmInt){
          .header = HEADER_INT,
          .value = INT_OFFSET + 2,
      },
      &(GcStackMap){
          .header = HEADER_GC_STACK_POP,
          .older = (void*)(-sizeof(GcStackMap) - sizeof(ElmInt)),
          .replay = (void*)(-sizeof(ElmInt)),
          .newer = (void*)(sizeof(GcStackMap)),
      },
      NULL,
  };
  char* heap_err_after = assert_heap_values("heap after GC & replay", heap_after_spec);
  if (heap_err_after) return heap_err_after;

  mu_assert("GC State replay_ptr", gc_state.replay_ptr == NULL);
  mu_assert("GC State stack_depth", gc_state.stack_depth == 0);
  mu_assert("GC State stackmap",
      (void*)gc_state.stack_map - (void*)gc_state.heap.start ==
          2 * sizeof(GcStackMap) + sizeof(ElmInt));

  return NULL;
}
