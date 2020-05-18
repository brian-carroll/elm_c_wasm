#include "../../gc_test.h"
#include "../../test.h"
#include "../replay_test.h"

char* test_replay_apply_alloc_failed() {
  if (verbose) {
    printf(
        "\n"
        "## test_replay_apply_alloc_failed\n"
        "\n");
  }

  // SETUP
  gc_test_reset();

  size_t words_to_leave = 2;
  size_t words_left_now = gc_state.heap.end - gc_state.next_alloc;
  size_t words_junk = words_left_now - words_to_leave;
  void* garbage = GC_malloc(words_junk * SIZE_UNIT);

  mu_assert("Almost fill up the heap, without overflowing", garbage != pGcFull);
  mu_expect_equal("Space left is exactly right",
      gc_state.heap.end - gc_state.next_alloc,
      words_to_leave);

  void* result = Utils_apply(&Utils_equal, 2, (void*[]){&Unit, &Unit});
  mu_expect_equal("Heap should overflow on making a function call", result, pGcFull);

  GC_collect_full();
  GC_prep_replay();

  result = Utils_apply(&Utils_equal, 2, (void*[]){&Unit, &Unit});

  void* h = gc_state.heap.start;

  // HEAP AFTER GC
  void* heap_after_spec[] = {
      &(GcStackMap){
          .header = HEADER_GC_STACK_EMPTY,
          .newer = h + sizeof(GcStackMap),
      },
      &(GcStackMap){
          .header = HEADER_GC_STACK_PUSH,
          .older = h,
      },
      &(GcStackMap){
          .header = HEADER_GC_STACK_POP,
          .replay = &True,
          .older = h + sizeof(GcStackMap),
      },
      NULL,
  };
  char* heap_err_after = assert_heap_values("heap after GC & replay", heap_after_spec);
  if (heap_err_after) return heap_err_after;

  mu_expect_equal("GC State replay_ptr", gc_state.replay_ptr, NULL);
  mu_expect_equal("GC State stack_depth", gc_state.stack_depth, 0);
  mu_expect_equal(
      "GC State stackmap", (void*)gc_state.stack_map, h + 2 * sizeof(GcStackMap));

  return NULL;
}
