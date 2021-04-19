#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "../kernel/core/core.h"
#include "../kernel/core/gc/internals.h"
#include "./gc/bitmap_test.c"
#include "test.h"

bool mark_words(GcState* state, void* p_void, size_t size);

void gc_test_reset() {
  GcState* state = &gc_state;
  bitmap_reset(&state->heap);
  reset_state(state);
  for (size_t* p = state->heap.start; p < state->heap.end; p++) {
    *p = 0;
  }
}


#define LARGE_ALLOCATION_WORDS 1024

void gc_test_large_allocation() {
  GC_allocate(false, 1024);
}


// --------------------------------------------------------------------------------


void set_heap_layout(GcHeap* heap, size_t* start, size_t bytes);

char* test_heap_layout() {
  if (verbose) {
    printf(
        "\n"
        "## test_heap_layout\n"
        "\n");
  }

  gc_test_reset();

  GcHeap* heap = &gc_state.heap;
  size_t* original_start = heap->start;
  size_t original_bytes = (heap->system_end - heap->start) * SIZE_UNIT;

  for (size_t kb = 16; kb <= 1024; kb *= 2) {
    size_t bytes = kb * 1024;
    size_t words = bytes / sizeof(void*);

    set_heap_layout(heap, original_start, bytes);

    float percent_bitmap = 100.0 * (heap->system_end - heap->bitmap) / words;
    float percent_offsets = 100.0 * (heap->bitmap - heap->offsets) / words;

    bool bitmap_ok = (sizeof(void*) == sizeof(u64))
                         ? (percent_bitmap > 1.5 && percent_bitmap < 1.6)
                         : (percent_bitmap > 2.95 && percent_bitmap < 3.05);
    bool offsets_ok = (percent_offsets > 1.45 && percent_offsets < 1.6);

    assertions_made++;
    if (!bitmap_ok || !offsets_ok) {
      tests_failed++;
      printf(
          "FAIL: GC overhead should be the right fraction of the heap at %zu kB\n", kb);
      printf("bitmap %f %%\n", percent_bitmap);
      printf("offsets %f %%\n", percent_offsets);
    } else if (verbose) {
      printf(
          "PASS: GC overhead should be the right fraction of the heap at %zu kB\n", kb);
    }
  }

  set_heap_layout(heap, original_start, original_bytes);
  gc_test_reset();

  return NULL;
}

// --------------------------------------------------------------------------------

#define TEST_MEMCPY_BUF_SIZE 10

void test_memcpy_reset(size_t* from, size_t* to) {
  for (int i = 0; i < TEST_MEMCPY_BUF_SIZE; ++i) {
    to[i] = 0;
    from[i] = i + 1;
  }
}

void* test_memcpy() {
  if (verbose) {
    printf(
        "\n"
        "## test_memcpy\n");
  }
  gc_test_reset();

  // Ensure buffers are 64-bit aligned by declaring them as u64
  u64 from64[TEST_MEMCPY_BUF_SIZE / 2];
  u64 to64[TEST_MEMCPY_BUF_SIZE / 2];

  // Now cast to word-sized values
  size_t* from = (size_t*)from64;
  size_t* to = (size_t*)to64;

  size_t* src;
  size_t* dest;
  size_t size;
  char description[100];

#ifndef TARGET_64BIT
  if (verbose) {
    printf("\n32-bit aligned\n");
  }
  src = from + 1;
  dest = to + 1;
  assert((size_t)dest % sizeof(u64) == sizeof(size_t));

  for (size = 1; size <= 6; ++size) {
    test_memcpy_reset(from, to);
    GC_memcpy(dest, src, size);
    int mismatches = 0;
    for (int i = 0; i < size; ++i) {
      if (dest[i] != src[i]) mismatches++;
    }
    snprintf(description,
        sizeof(description),
        "should correctly copy %zd 32-bit words, 32-bit aligned",
        size);
    mu_assert(description, mismatches == 0);
  }
#endif

  if (verbose) {
    printf("\n64-bit aligned\n");
  }
  src = from;
  dest = to;
  assert((size_t)dest % sizeof(u64) == 0);

  for (size = 1; size <= 6; ++size) {
    test_memcpy_reset(from, to);
    GC_memcpy(dest, src, size);
    int mismatches = 0;
    for (int i = 0; i < size; ++i) {
      if (dest[i] != src[i]) mismatches++;
    }
    snprintf(description,
        sizeof(description),
        "should correctly copy %zd 32-bit words, 64-bit aligned",
        size);
    mu_assert(description, mismatches == 0);
  }

  return NULL;
}


// --------------------------------------------------------------------------------

struct stackmap_mark_scenario {
  char* name;
  char* expected_marks;
  i32 argument;
  i32 gc_trigger_arg;
};

#ifdef TARGET_64BIT
#define IS_64BIT 1
#else
#define IS_64BIT 0
#endif

int stackmap_mark_scenario_index;
struct stackmap_mark_scenario stackmap_mark_scenarios[] = {
    {
        .name = "COMPLETE",
        .argument = 6,
        .gc_trigger_arg = 8,
        .expected_marks = "",
    },
    {
        .name = "RECURSE",
        .argument = 6,
        .gc_trigger_arg = 2,
        .expected_marks = IS_64BIT ? "XXXXXXX" : "XXXXXXXXXXXX",
    },
};

ElmInt int_0 = {.header = HEADER_INT, .value = 0};
ElmInt int_1 = {.header = HEADER_INT, .value = 1};
ElmInt int_2 = {.header = HEADER_INT, .value = 2};
#define g_elm_core_Basics_eq Utils_equal
#define g_elm_core_Basics_le Utils_le
#define g_elm_core_Basics_add Basics_add
#define g_elm_core_Basics_sub Basics_sub


void* eval_fibGlobalTailRecHelp(void* args[]) {
  ElmInt* x_n = args[0];
  ElmInt* x_i = args[1];
  ElmInt* x_a = args[2];
  ElmInt* x_b = args[3];
  // u32 gc_stack_frame = GC_get_stack_frame();
tce_loop:;
  struct stackmap_mark_scenario scenario =
      stackmap_mark_scenarios[stackmap_mark_scenario_index];
  if (x_i->value == scenario.gc_trigger_arg) {
    gc_test_large_allocation();
  }

  void* if0;
  if (A2(&g_elm_core_Basics_eq, x_i, x_n) == &True) {
    if0 = x_a;
  } else {
    ElmInt* tmp1 = x_n;
    ElmInt* tmp2 = A2(&g_elm_core_Basics_add, x_i, &int_1);
    ElmInt* tmp3 = x_b;
    ElmInt* tmp4 = A2(&g_elm_core_Basics_add, x_a, x_b);
    x_n = tmp1;
    x_i = tmp2;
    x_a = tmp3;
    x_b = tmp4;
    GC_stack_tailcall(4, x_n, x_i, x_a, x_b);
    goto tce_loop;
    if0 = NULL;
  };
  return if0;
}
Closure fibGlobalTailRecHelp = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x4,
    .evaluator = eval_fibGlobalTailRecHelp,
};


void* eval_fibGlobalTailRec(void* args[]) {
  void* x_n = args[0];
  return A4(&fibGlobalTailRecHelp, x_n, &int_0, &int_0, &int_1);
}
Closure fibGlobalTailRec = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_fibGlobalTailRec,
};


Closure fibGlobalRec;
void* eval_fibGlobalRec(void* args[]) {
  ElmInt* x_i = args[0];
  ElmInt* if0;

  struct stackmap_mark_scenario scenario =
      stackmap_mark_scenarios[stackmap_mark_scenario_index];
  fflush(0);
  if (x_i->value == scenario.gc_trigger_arg) {
    gc_test_large_allocation();
  }

  if (A2(&g_elm_core_Basics_le, x_i, &int_1) == &True) {
    if0 = &int_1;
  } else {
    ElmInt* tmp1 = A2(&g_elm_core_Basics_sub, x_i, &int_1);
    ElmInt* tmp2 = A1(&fibGlobalRec, tmp1);
    ElmInt* tmp3 = A2(&g_elm_core_Basics_sub, x_i, &int_2);
    ElmInt* tmp4 = A1(&fibGlobalRec, tmp3);
    if0 = A2(&g_elm_core_Basics_add, tmp2, tmp4);
  };
  return if0;
}
Closure fibGlobalRec = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_fibGlobalRec,
};

/*
  TODO
  Improve this test

  - Use Fibonacci so that we can check against an actual known result!
    - Do a tail recursive version and a normal recursive version
  - Throw in some extra garbage allocations
  - At a particular count, trigger a big allocation that forces GC
  - Have a GC test callback just before allocation fails
    - In the callback, check what's marked and do some assertions
  - After GC, let it run to completion and check Fibonacci result.

Scenario parameters:
  - available space to start with
  - expected mark bits
  - Fibonacci result


*/


static int fibonacci(int n) {
  if (n <= 1) return n;

  int t1 = 0;
  int t2 = 1;
  int next;

  for (int i = 1; i <= n; ++i) {
    next = t1 + t2;
    t1 = t2;
    t2 = next;
  }
  return next;
}


bool callback_called;

void stackmap_mark_test_callback() {
  char message[100];
  callback_called = true;
  GcState* state = &gc_state;
  size_t* start = stack_values[1];
  struct stackmap_mark_scenario scenario =
      stackmap_mark_scenarios[stackmap_mark_scenario_index];

  // print_heap();
  // print_state();
  // print_stack_map();

  size_t expected_size = strlen(scenario.expected_marks);
  size_t actual_size = state->next_alloc - start;
  snprintf(message,
      sizeof(message),
      "should have %zd allocated words before GC",
      expected_size);
  mu_expect_equal(message, expected_size, actual_size);

  int expected_marks_len = strlen(scenario.expected_marks);
  int n_correct = 0;
  for (size_t* p = stack_values[1]; p < state->next_alloc; p++) {
    int i = p - start;
    if (i >= expected_marks_len) break;
    bool expected = scenario.expected_marks[i] == 'X';
    bool actual = is_marked(p);
    if (expected == actual) {
      n_correct++;
    } else if (verbose) {
      printf("%p %s be marked\n", p, expected ? "should" : "should not");
    }
  }

  mu_expect_equal("stack map should correctly mark all words", n_correct, expected_size);
}


char* stackmap_mark_test() {
  if (verbose) {
    printf("\n## stackmap_mark_test\n");
  }
  gc_test_mark_callback = stackmap_mark_test_callback;
  const int n_scenarios =
      sizeof(stackmap_mark_scenarios) / sizeof(stackmap_mark_scenarios[0]);

  for (int i = 0; i < n_scenarios; ++i) {
    stackmap_mark_scenario_index = i;
    struct stackmap_mark_scenario scenario = stackmap_mark_scenarios[i];

    if (verbose) {
      printf("\n- %s\n", scenario.name);
    }

    callback_called = false;
    gc_test_reset();
    size_t filler_size = gc_state.heap.end - gc_state.heap.start - LARGE_ALLOCATION_WORDS;
    GC_allocate(false, filler_size);
    ElmInt* arg = newElmInt(scenario.argument);
    Closure* c = newClosure(1, 1, eval_fibGlobalRec, (void*[]){arg});

    ElmInt* result = GC_execute(c);

    mu_expect_equal(
        "should return the correct result", result->value, fibonacci(scenario.argument));

    if (scenario.gc_trigger_arg < scenario.argument) {
      mu_assert("should call the mark callback", callback_called);
    } else {
      mu_assert("should not call the mark callback", !callback_called);
    }
  }

  return NULL;
}


// --------------------------------------------------------------------------------

void* eval_trashyFold(void* args[]) {
  ElmInt* free_var = args[0];
  ElmInt* a = args[1];
  Cons* acc = args[2];
  ElmInt* b = acc->head;

  assert(sanity_check(free_var));
  assert(sanity_check(a));
  assert(sanity_check(acc));
  assert(sanity_check(b));

  ElmInt* c = A2(&Basics_add, a, free_var);
  assert(sanity_check(c));

  ElmInt* d = A2(&Basics_sub, c, b);
  assert(sanity_check(d));

  Cons* result = newCons(d, newCons(c, newCons(b, pNil)));
  assert(sanity_check(result));

  return result;
}
Closure trashyFold = {
    .header = HEADER_CLOSURE(0),
    .max_values = 3,
    .evaluator = &eval_trashyFold,
};

void* eval_listNonsense(void* args[]) {
  Cons* list = args[0];
  assert(sanity_check(list));

  Cons* acc = newCons(newElmInt(64), &Nil);
  assert(sanity_check(acc));

  ElmInt* free_var = newElmInt(123);
  assert(sanity_check(free_var));

  Closure* partial = A1(&trashyFold, free_var);
  assert(sanity_check(partial));

  Cons* folded = A3(&g_elm_core_List_foldl, partial, acc, list);
  assert(sanity_check(folded));

  Cons* reversed = A1(&g_elm_core_List_reverse, folded);
  assert(sanity_check(reversed));

  return reversed;
}
Closure listNonsense = {
    .header = HEADER_CLOSURE(0),
    .evaluator = eval_listNonsense,
    .max_values = 1,
};


// --------------------------------------------------------------------------------

int count_gc_cycles;
void assertions_test_callback() {
  count_gc_cycles++;
}


void* eval_infinite_loop(void* args[]) {
  Cons* list = args[0];
  ElmInt* max_gc_cycles = args[1];
  // u32 gc_stack_frame = GC_get_stack_frame();

  assert(sanity_check(list));

  while (count_gc_cycles < max_gc_cycles->value) {
    list = A1(&listNonsense, list);
    assert(sanity_check(list));
    GC_stack_tailcall(2, list, max_gc_cycles);
  }

  return list;
}


void* test_execute(Closure* c) {
  gc_test_mark_callback = assertions_test_callback;
  stack_clear();
  stack_enter(c);
  return Utils_apply(stack_values[1], 0, NULL);
}


char* assertions_test() {
  if (verbose) {
    printf(
        "\n"
        "## assertions_test\n"
        "(run for long enough to do a few GCs, try to trigger assertions)\n"
        "\n");
  }
  gc_test_reset();

  void* list_elems[3] = {
      newElmInt(123),
      newElmInt(999),
      newElmInt(-7),
  };
  Cons* list = List_create(3, list_elems);
  ElmInt* max_gc_cycles = newElmInt(10);
  Closure* c = newClosure(2, 2, eval_infinite_loop, ((void*[]){list, max_gc_cycles}));

  test_execute(c);

  mu_assert("should complete without triggering any assertions", true);
  return NULL;
}

// --------------------------------------------------------------------------------

void* eval_generateHeapPattern(void* args[]) {
  ElmInt* garbageChunkSize = args[0];
  ElmInt* liveChunkSize = args[1];
  ElmInt* iterations = args[2];

  Cons* liveList = pNil;
  i32 nKidsGarbage = SIZE_CUSTOM(garbageChunkSize->value) - SIZE_CUSTOM(0);
  i32 nKidsLive =
      SIZE_CUSTOM(liveChunkSize->value) - SIZE_CUSTOM(0) - SIZE_INT - SIZE_LIST;
  assert(nKidsGarbage >= 0);
  assert(nKidsLive >= 1);

  printf("nKidsGarbage %d\n", nKidsGarbage);
  printf("nKidsLive %d\n", nKidsLive);

tce_loop:;
  do {
    printf("tce_loop: iterations=%d, liveList=%p, liveList->head=%p, liveList->tail=%p\n",
        iterations->value,
        liveList,
        liveList->head,
        liveList->tail);
    if (iterations->value == 0) {
      printf("Heap pattern generated. Calculating result\n");
      PRINT_BITMAP();

      i32 nErrors = 0;
      i32 expected = 1;
      for (; liveList->tail != pNil; liveList = liveList->tail) {
        Custom* live = liveList->head;
        ElmInt* iter = live->values[0];
        fflush(0);
        if (iter->value != expected) {
          printf("Wrong value at %p: expected %d, got %d\n", iter, expected, iter->value);
          nErrors++;
        }
        expected++;
      }
      return newElmInt(nErrors);
    } else {
      Custom* garbage = newCustom(CTOR_Err, nKidsGarbage, NULL);
      for (int i = 0; i < nKidsGarbage; i++) {
        garbage->values[i] = pUnit;
      }

      Custom* live = newCustom(CTOR_Ok, nKidsLive, NULL);
      live->values[0] = iterations;
      for (int i = 1; i < nKidsLive; i++) {
        live->values[i] = pUnit;
      }
      liveList = newCons(live, liveList);
      iterations = newElmInt(iterations->value - 1);
      GC_stack_tailcall(4, garbageChunkSize, liveChunkSize, iterations, liveList);
      goto tce_loop;
    };
  } while (0);
}

void minor_gc_test_callback() {
  printf("\n\n minor_gc_test_callback \n\n");
  // print_heap();
  // print_state();
}

char* minor_gc_test() {
  if (verbose) {
    printf(
        "\n"
        "## minor_gc_test\n"
        "(fill the heap with a pattern, mark, sweep, allocate in swept areas)\n"
        "\n");
  }
  gc_test_reset();
  printf("gc_test_reset\n");
  gc_test_mark_callback = minor_gc_test_callback;

  GcState* state = &gc_state;
  GcHeap* heap = &state->heap;

  size_t heap_size = heap->end - heap->start;

  i32 garbageChunkSize = 256;
  i32 liveChunkSize = 256;
  i32 iterations_to_fill_heap = heap_size / (garbageChunkSize + liveChunkSize);
  i32 iterations = iterations_to_fill_heap + (iterations_to_fill_heap / 4);

  if (verbose) {
    printf("garbageChunkSize = %d\n", garbageChunkSize);
    printf("liveChunkSize = %d\n", liveChunkSize);
    printf("iterations_to_fill_heap = %d\n", iterations_to_fill_heap);
    printf("iterations = %d\n", iterations);
  }

  Closure* run = newClosure(3,
      3,
      eval_generateHeapPattern,
      ((void*[]){
          newElmInt(garbageChunkSize),
          newElmInt(liveChunkSize),
          newElmInt(iterations),
      }));

  stack_clear();
  stack_enter(run);
  ElmInt* nErrors = Utils_apply(run, 0, NULL);
  mu_expect_equal("should complete with zero errors", nErrors->value, 0);

  print_heap();
  print_state();

  return NULL;
}


// --------------------------------------------------------------------------------


char unknown_function_address[FORMAT_PTR_LEN];
char* Debug_evaluator_name(void* p) {
  if (p == Utils_le.evaluator) {
    return "Utils_le     ";
  } else if (p == Basics_sub.evaluator) {
    return "Basics_sub   ";
  } else if (p == Basics_add.evaluator) {
    return "Basics_add   ";
  } else if (p == g_elm_core_List_foldl.evaluator) {
    return "List.foldl   ";
  } else if (p == g_elm_core_List_reverse.evaluator) {
    return "List.reverse ";
  } else if (p == eval_trashyFold) {
    return "trashyFold   ";
  } else if (p == eval_listNonsense) {
    return "listNonsense ";
  } else if (p == eval_infinite_loop) {
    return "infinite_loop";
  } else if (p == eval_generateHeapPattern) {
    return "generateHeapPattern";
  } else {
    snprintf(unknown_function_address, FORMAT_PTR_LEN, FORMAT_PTR, p);
    return unknown_function_address;
  }
}


char* gc_test() {
  if (verbose)
    printf(
        "##############################################################################\n"
        "\n"
        "                              Garbage Collector tests\n"
        "\n"
        "##############################################################################"
        "\n");

  mu_run_test(gc_bitmap_test);
  mu_run_test(test_heap_layout);
  mu_run_test(test_memcpy);
  mu_run_test(stackmap_mark_test);
  mu_run_test(minor_gc_test);
  // mu_run_test(assertions_test); // FIXME

  return NULL;
}
