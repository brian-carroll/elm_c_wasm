#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "../../kernel/gc-internals.h"
#include "../../kernel/gc.h"
#include "../../kernel/types.h"
#include "../../kernel/utils.h"
#include "../gc_test.h"
#include "../test.h"
#include "./print-heap.h"

extern GcState gc_state;

Tag mock_func_tags[10];
i32 mock_func_count;

void* eval_mock_func(void* args[2]) {
  void* last_alloc = NULL;
  for (mock_func_count = 0;; mock_func_count++) {
    Tag current_tag = mock_func_tags[mock_func_count];
    switch (current_tag) {
      case Tag_Int:
        last_alloc = NEW_ELM_INT(mock_func_count);
        printf("allocated at %p, count=%d\n", last_alloc, mock_func_count);
        break;
      case Tag_GcException:
        printf("throwing exception, count=%d\n", mock_func_count);
        return pGcFull;
      case Tag_GcStackPop:
        printf("returning pointer %p, count=%d\n", last_alloc, mock_func_count);
        return last_alloc;
      default:
        fprintf(stderr, "Unhandled tag in eval_mock_func %x\n", current_tag);
    }
  }
}

const Closure mock_func = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_mock_func,
    .max_values = 2,
};

int next_value_matches(size_t** p, const void* v) {
  Header* h = (Header*)v;
  bool areEqual = memcmp(*p, v, h->size * SIZE_UNIT) == 0;
  *p += h->size;
  return areEqual;
}

char* test_gc_replay_finished() {
  // SETUP
  gc_test_reset();
  memcpy(mock_func_tags,
      (Tag[]){
          Tag_Int,
          Tag_Int,
          Tag_Int,
          Tag_GcStackPop,
      },
      sizeof(Tag[4]));

  // RUN
  void* result1 = Utils_apply(&mock_func, 2, (void* []){NULL, NULL});
  print_heap();
  print_state();
  ElmInt expected_result = {
      .header = HEADER_INT,
      .value = 3,
  };
  mu_assert("Return value",
      memcmp(result1, &expected_result, expected_result.header.size) == 0);

  size_t* heap_value;

  // HEAP BEFORE GC
  heap_value = gc_state.heap.start;
  printf("%p ", heap_value);
  mu_assert("Empty",
      next_value_matches(&heap_value, &(GcStackMap){.header = HEADER_GC_STACK_EMPTY}));

  size_t* push = heap_value;
  printf("%p ", heap_value);
  mu_assert("Push",
      next_value_matches(&heap_value,
          &(GcStackMap){.header = HEADER_GC_STACK_PUSH, .older = gc_state.heap.start}));

  printf("%p ", heap_value);
  mu_assert("Int 0",
      next_value_matches(&heap_value, &(ElmInt){.header = HEADER_INT, .value = 0}));

  printf("%p ", heap_value);
  mu_assert("Int 1",
      next_value_matches(&heap_value, &(ElmInt){.header = HEADER_INT, .value = 1}));

  size_t* ret = heap_value;
  printf("%p ", heap_value);
  mu_assert("Int 2",
      next_value_matches(&heap_value, &(ElmInt){.header = HEADER_INT, .value = 2}));

  printf("%p ", heap_value);
  mu_assert("Pop",
      next_value_matches(&heap_value,
          &(GcStackMap){.header = HEADER_GC_STACK_POP, .older = push, .replay = ret}));

  // GC + REPLAY
  GC_collect_full();
  GC_start_replay();
  void* result2 = Utils_apply(&mock_func, 2, (void* []){NULL, NULL});
  mu_assert("Replay return value",
      memcmp(result2, &expected_result, expected_result.header.size) == 0);

  print_heap();

  // HEAP AFTER GC
  heap_value = gc_state.heap.start;
  printf("%p ", heap_value);
  mu_assert("Empty",
      next_value_matches(&heap_value,
          &(GcStackMap){
              .header = HEADER_GC_STACK_EMPTY,
              .newer = heap_value + sizeof(GcStackMap) / SIZE_UNIT,
          }));

  size_t* push2 = heap_value;
  printf("%p ", heap_value);
  mu_assert("Push",
      next_value_matches(&heap_value,
          &(GcStackMap){
              .header = HEADER_GC_STACK_PUSH,
              .older = gc_state.heap.start,
              .newer = heap_value + (sizeof(GcStackMap) + sizeof(ElmInt)) / SIZE_UNIT,
          }));

  size_t* ret2 = heap_value;
  printf("%p ", heap_value);
  mu_assert("Int 2",
      next_value_matches(&heap_value, &(ElmInt){.header = HEADER_INT, .value = 2}));

  size_t* pop2 = heap_value;
  printf("%p ", heap_value);
  mu_assert("Pop",
      next_value_matches(&heap_value,
          &(GcStackMap){
              .header = HEADER_GC_STACK_POP,
              .older = push2,
              .replay = ret2,
              .newer = gc_state.next_alloc,
          }));

  mu_assert("GC State replay_ptr", gc_state.replay_ptr == NULL);
  mu_assert("GC State stack_depth", gc_state.stack_depth == 0);
  mu_assert("GC State stackmap", gc_state.stack_map == (GcStackMap*)pop2);

  return NULL;
}

char* replay_scenario_tests() {
  mu_run_test(test_gc_replay_finished);
  return NULL;
}
