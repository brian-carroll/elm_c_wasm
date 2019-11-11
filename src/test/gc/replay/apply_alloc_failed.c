#include "../replay_test.h"

ElmInt spaceToLeave = {
    .header = HEADER_INT,
    .value = 0,
};

void* eval_generateGarbage(void* args[1]) {
  size_t bytes_to_leave = ((ElmInt*)args[0])->value;
  size_t words_to_leave = (bytes_to_leave + SIZE_UNIT - 1) / SIZE_UNIT;  // round up
  size_t words_left_now = gc_state.heap.end - gc_state.next_alloc;
  size_t words_pop = sizeof(GcStackMap) / SIZE_UNIT;
  size_t words_junk = words_left_now - words_to_leave - words_pop;

  ElmString* s = GC_malloc(words_junk * SIZE_UNIT);
  s->header = (Header){
      .tag = Tag_String,
      .size = (u32)words_junk,
  };

  return &Unit;  // don't return the string => becomes garbage
}

Closure generateGarbage = {
    .header = HEADER_CLOSURE(0),
    .max_values = 1,
    .evaluator = &eval_generateGarbage,
};

char* test_replay_apply_alloc_failed() {
  if (verbose) {
    printf(
        "\n"
        "## test_replay_apply_alloc_failed\n"
        "\n");
  }

  // SETUP
  gc_test_reset();
  memcpy(mock_func_ops,
      (Tag[]){
          Tag_GcStackPop,
      },
      sizeof(Tag[1]));

  void* h = gc_state.heap.start;

  spaceToLeave.value = 4 * sizeof(void*);
  void* unit = Utils_apply(&generateGarbage, 1, (void* []){&spaceToLeave});
  mu_assert("Almost fill up the heap, without overflowing", unit != pGcFull);
  mu_expect_equal("Space left is exactly right",
      (void*)gc_state.heap.end - (void*)gc_state.next_alloc,
      spaceToLeave.value);

  void* result = Utils_apply(&mock_func, 2, (void* []){NULL, NULL});
  mu_assert("Function call should throw", result == pGcFull);

  GC_collect_full();
  GC_start_replay();
  Utils_apply(&generateGarbage, 1, (void* []){&spaceToLeave});
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
          .newer = h + 2 * sizeof(GcStackMap),
      },
      &(GcStackMap){
          .header = HEADER_GC_STACK_POP,
          .replay = &Unit,
          .older = h + sizeof(GcStackMap),
          .newer = h + 3 * sizeof(GcStackMap),
      },
      &(GcStackMap){
          .header = HEADER_GC_STACK_PUSH,
          .older = h + 2 * sizeof(GcStackMap),
      },
      &(GcStackMap){
          .header = HEADER_GC_STACK_POP,
          .older = h + 3 * sizeof(GcStackMap),
      },
      NULL,
  };
  char* heap_err_after = assert_heap_values("heap after GC & replay", heap_after_spec);
  if (heap_err_after) return heap_err_after;

  mu_assert("GC State replay_ptr", gc_state.replay_ptr == NULL);
  mu_assert("GC State stack_depth", gc_state.stack_depth == 0);
  mu_expect_equal(
      "GC State stackmap", (void*)gc_state.stack_map, h + 4 * sizeof(GcStackMap));

  return NULL;
}
