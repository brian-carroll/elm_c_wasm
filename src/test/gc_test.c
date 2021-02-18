#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "../kernel/core/core.h"
#include "test.h"
#include "gc_test.h"

void reset_state(GcState*);
bool mark_words(GcHeap* heap, void* p_void, size_t size);
void compact(GcState* state, size_t* compact_start);
bool sanity_check(void* v);


void gc_test_reset() {
  GcState* state = &gc_state;
  bitmap_reset(&state->heap);
  reset_state(state);
  for (size_t* p = state->heap.start; p < state->heap.end; p++) {
    *p = 0;
  }
}

// --------------------------------------------------------------------------------


char bitmap_msg[100];

char* gc_bitmap_test() {
  GcState* state = &gc_state;
  char str[] = "This is a test string that's an odd number of ints.....";
  gc_test_reset();

  for (size_t i = 0; i < 10; i++) {
    ElmInt* p1 = ctorElmInt(0x101);
    ElmInt* p2 = ctorElmInt(0x102);
    ElmString16* p3 = ctorElmString16(sizeof(str) - 1);
    ElmInt* p4 = ctorElmInt(0x103);

    for (int c=0; c < sizeof(str); c++) {
      p3->words16[c] = str[c];
    }

    mark_words(&state->heap, p1, p1->header.size);
    mark_words(&state->heap, p3, p3->header.size);

    mu_assert("p1 should be marked", !!is_marked(p1));
    mu_assert("p2 should NOT be marked", !is_marked(p2));
    mu_assert("p3 should be marked", !!is_marked(p3));
    mu_assert("p4 should NOT be marked", !is_marked(p4));
  }

  if (verbose) {
    printf(
        "\n"
        "# gc_bitmap_test\n"
        "\n");
    // print_heap();
    // print_state();
    printf("\n");
  }

  size_t* bottom_of_heap = state->heap.start;
  size_t* top_of_heap = state->next_alloc;

  size_t* ptr = bottom_of_heap;
  size_t* bitmap = state->heap.bitmap;

  size_t w = 0;
  while (ptr <= top_of_heap) {
    size_t word = bitmap[w];
    for (size_t b = 0; b < GC_WORD_BITS; b++) {
      bool bitmap_bit = (word & ((size_t)1 << b)) != 0;
      sprintf(bitmap_msg,
          "is_marked (%d) should match the bitmap (%d) addr = %p  word = %zd  bit = %zd",
          is_marked(ptr),
          bitmap_bit,
          ptr,
          w,
          b);
      mu_assert(bitmap_msg, is_marked(ptr) == bitmap_bit);
      ptr++;
    }
    w++;
  }

  return NULL;
}

// --------------------------------------------------------------------------------


char gc_bitmap_next_test_str[100];

char* gc_bitmap_next_test() {
  if (verbose) {
    printf("\n");
    printf("## gc_bitmap_next_test\n");
    printf("\n");
  }
  gc_test_reset();

  size_t word;
  size_t mask;

  int assertion = 1;

  word = 0;
  mask = 1;
  sprintf(gc_bitmap_next_test_str,
      "bitmap_next assertion %d from word %zd mask %0zx",
      assertion++,
      word,
      mask);
  bitmap_next(&word, &mask);
  mu_assert(gc_bitmap_next_test_str, word == 0 && mask == 2);

  word = 0;
  mask = 2;
  sprintf(gc_bitmap_next_test_str,
      "bitmap_next assertion %d from word %zd mask %0zx",
      assertion++,
      word,
      mask);
  bitmap_next(&word, &mask);
  mu_assert(gc_bitmap_next_test_str, word == 0 && mask == 4);

  word = 1;
  mask = 1;
  sprintf(gc_bitmap_next_test_str,
      "bitmap_next assertion %d from word %zd mask %0zx",
      assertion++,
      word,
      mask);
  bitmap_next(&word, &mask);
  mu_assert(gc_bitmap_next_test_str, word == 1 && mask == 2);

  word = 1;
  mask = 2;
  sprintf(gc_bitmap_next_test_str,
      "bitmap_next assertion %d from word %zd mask %0zx",
      assertion++,
      word,
      mask);
  bitmap_next(&word, &mask);
  mu_assert(gc_bitmap_next_test_str, word == 1 && mask == 4);

  word = 0;
  char* format_str;
#ifdef TARGET_64BIT
  mask = 0x8000000000000000;
  format_str = "bitmap_next assertion %d from word %zd mask %016zx";
#else
  mask = 0x80000000;
  format_str = "bitmap_next assertion %d from word %zd mask %08zx";
#endif

  mu_assert("bitmap_next: highest bit is correctly set in test", (mask << 1) == 0);
  assertion++;

  sprintf(gc_bitmap_next_test_str, format_str, assertion++, word, mask);
  bitmap_next(&word, &mask);
  mu_assert(gc_bitmap_next_test_str, word == 1 && mask == 1);

  return NULL;
}


// --------------------------------------------------------------------------------


char* gc_dead_between_test() {
  gc_test_reset();
  GcState* state = &gc_state;
  GcHeap* heap = &state->heap;

  size_t* first;
  size_t* last;

  heap->bitmap[0] = 0xf0f;
  first = heap->start + 4;
  last = heap->start + 8;
  mu_assert("bitmap_dead_between with 4 words dead",
      bitmap_dead_between(heap, first, last) == 4);

  first--;
  last++;

  mu_assert("bitmap_dead_between with 4 dead and 2 live",
      bitmap_dead_between(heap, first, last) == 4);

  heap->bitmap[0] = 0xf0;
  heap->bitmap[1] = 0x00;
  heap->bitmap[2] = 0xf0;
  first = heap->start + 2;
  last = heap->start + (2 * GC_WORD_BITS) + 10;

  mu_assert("bitmap_dead_between across 3 bitmap words",
      bitmap_dead_between(heap, first, last) ==
          ((GC_WORD_BITS - 2 - 4) + GC_WORD_BITS + 10 - 4));

  return NULL;
}

// --------------------------------------------------------------------------------

void set_heap_layout(GcHeap* heap, size_t* new_break_ptr);

char* test_heap_layout() {
  if (verbose) {
    printf(
        "\n"
        "## test_heap_layout\n"
        "\n");
  }

  gc_test_reset();

  GcHeap* heap = &gc_state.heap;
  size_t* original_break_ptr = heap->system_end;

  for (size_t kb = 16; kb <= 1024; kb *= 2) {
    size_t bytes = kb * 1024;
    size_t words = bytes / sizeof(void*);
    size_t* new_break_ptr = heap->start + words;

    set_heap_layout(heap, new_break_ptr);

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

  set_heap_layout(heap, original_break_ptr);
  gc_test_reset();

  return NULL;
}

// --------------------------------------------------------------------------------

#define TEST_MEMCPY_BUF_SIZE 10

void test_memcpy_reset(u32* from, u32* to) {
  for (int i = 0; i < TEST_MEMCPY_BUF_SIZE; ++i) {
    to[i] = 0;
    from[i] = i + 1;
  }
}

void* test_memcpy() {
  if (verbose) {
    printf(
        "\n"
        "## test_memcpy\n"
        "\n");
  }
  gc_test_reset();

  // Ensure buffers are 64-bit aligned by declaring them as u64
  u64 from64[TEST_MEMCPY_BUF_SIZE / 2];
  u64 to64[TEST_MEMCPY_BUF_SIZE / 2];

  // Now cast to 32-bit values
  u32* from = (u32*)from64;
  u32* to = (u32*)to64;

  u32* src;
  u32* dest;
  u32 size;
  char description[100];

  if (verbose) {
    printf("\n32-bit aligned\n");
  }
  src = from + 1;
  dest = to + 1;
  assert((size_t)dest % sizeof(u64) == sizeof(u32));

  for (size = 1; size <= 6; ++size) {
    test_memcpy_reset(from, to);
    GC_memcpy(dest, src, size * sizeof(u32));
    int mismatches = 0;
    for (int i = 0; i < size; ++i) {
      if (dest[i] != src[i]) mismatches++;
    }
    snprintf(description, sizeof(description), "should correctly copy %d 32-bit words, 32-bit aligned", size);
    mu_assert(description, mismatches == 0);
  }


  if (verbose) {
    printf("\n64-bit aligned\n");
  }
  src = from;
  dest = to;
  assert((size_t)dest % sizeof(u64) == 0);

  for (size = 1; size <= 6; ++size) {
    test_memcpy_reset(from, to);
    GC_memcpy(dest, src, size * sizeof(u32));
    int mismatches = 0;
    for (int i = 0; i < size; ++i) {
      if (dest[i] != src[i]) mismatches++;
    }
    snprintf(description, sizeof(description), "should correctly copy %d 32-bit words, 64-bit aligned", size);
    mu_assert(description, mismatches == 0);
  }

 return NULL;
}


// --------------------------------------------------------------------------------


/*
fib : Int -> Int
fib i =
    if i <= 1 then
        1
    else
        fib (i-1) + fib (i-2)
*/
#define g_elm_core_Basics_le Utils_le
#define g_elm_core_Basics_sub Basics_sub
#define g_elm_core_Basics_add Basics_add

ElmInt int_2 = { .header = HEADER_INT, .value = 2 };
ElmInt int_1 = { .header = HEADER_INT, .value = 1 };
ElmInt int_n = { .header = HEADER_INT, .value = 6 };

Closure fib;
void * eval_fib(void * args[]) {
    void * x_i = args[0];
    void * if0;
    if (A2(&g_elm_core_Basics_le, x_i, &int_1) == &True) {
        if0 = &int_1;
    } else {
        void * tmp1 = A2(&g_elm_core_Basics_sub, x_i, &int_2);
        void * tmp2 = A1(&fib, tmp1);
        void * tmp3 = A2(&g_elm_core_Basics_sub, x_i, &int_1);
        void * tmp4 = A1(&fib, tmp3);
        if0 = A2(&g_elm_core_Basics_add, tmp4, tmp2);
    };
    return if0;
}
Closure fib = { .header = HEADER_CLOSURE(0), .n_values = 0x0, .max_values = 0x1, .evaluator = &eval_fib };


char* gc_replay_test() {
  GcState* state = &gc_state;
  if (verbose) {
    printf(
        "\n"
        "## gc_replay_test\n"
        "\n");
  }
  gc_test_reset();

#ifdef TARGET_64BIT
  size_t not_quite_enough_space = 150/sizeof(void*);
#else
  size_t not_quite_enough_space = 150/sizeof(void*);
#endif
  size_t* ignore_below = state->heap.end - not_quite_enough_space;
  state->next_alloc = ignore_below;

  if (verbose) {
    printf("Set allocation pointer to leave only %zu (%zu-bit) words of heap space\n",
        not_quite_enough_space,
        (sizeof(void*)) * 8);
  }

  // Create a thunk as if entering Wasm from JS
  void* args[1];
  args[0] = &int_n;
  Closure* c = ctorClosure(1, 1, eval_fib, args);
  stack_clear();
  stack_enter(c);
  void* result = Utils_apply(c, 0, NULL);
  mu_expect_equal("Expect GC exception when test function called with insufficient heap space", result, pGcFull);
  GC_collect_full();
  ElmInt* result_replay = Utils_apply(stack_values[1], 0, NULL);


  if (verbose) {
    printf("Answer after replay = ");
    print_value(result_replay);
    printf("\n");
  }

  i32 answers[28] = { 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610, 987, 1597, 2584, 4181, 6765, 10946, 17711, 28657, 46368, 75025, 121393, 196418, 317811 };
  i32 answer = answers[int_n.value];

  bool pass = result_replay->value == answer;

  if (verbose && !pass) {
    print_heap();
    print_state();
  }

  mu_expect_equal("should return the correct result after resuming", result_replay->value, answer);

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

  Cons* result = ctorCons(d, ctorCons(c, ctorCons(b, pNil)));
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

  Cons* acc = ctorCons(ctorElmInt(64), &Nil);
  assert(sanity_check(acc));

  ElmInt* free_var = ctorElmInt(123);
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

char* stackmap_mark_eyeball_test() {
  if (verbose) {
    printf(
        "\n"
        "## stackmap_mark_eyeball_test\n"
        "\n");
  }

  gc_test_reset();

  void* list_elems[3] = {
    ctorElmInt(123),
    ctorElmInt(999),
    ctorElmInt(-7),
  };
  Cons* list = List_create(3, list_elems);
  Closure* c = ctorClosure(1, 1, eval_listNonsense, ((void*[]){list}));

  stack_clear();
  stack_enter(c);

  Utils_apply(c, 0, NULL);

  GcState* state = &gc_state;
  mark(state, state->heap.start);
  // print_heap();
  // print_state();
  // print_stack_map();

  return NULL;
}


// --------------------------------------------------------------------------------


void* eval_infinite_loop(void* args[]) {
  u32 gc_stack_frame = GC_get_stack_frame();
  Closure* gc_resume = ctorClosure(1, 1, eval_infinite_loop, args);
  assert(sanity_check(gc_resume));

  Cons* list = gc_resume->values[0];
  assert(sanity_check(list));

  tce_loop:
  list = A1(&listNonsense, list);
  assert(sanity_check(list));
  gc_resume = GC_stack_tailcall(
    gc_stack_frame, gc_resume, 1, ((void * []){ list })
  );
  goto tce_loop;
}


void* test_execute(Closure* c, int gc_cycles) {
  stack_clear();
  stack_enter(c);

  // printf("test_execute after setup\n");
  // print_stack_map();

  for (int i = 0; i < gc_cycles; i++) {
    if (verbose) {
      printf("executing...\n");
    }
    assert(sanity_check(stack_values[1]));
    void* result = Utils_apply(stack_values[1], 0, NULL);
    if (result != pGcFull) {
      stack_clear();
      return result;
    }
    if (verbose) {
      printf("\n\ncalling GC (%d)...\n", i);
      print_stack_map();
    }

    GC_collect_full();
    if (verbose) {
      printf("\n\ncompleted GC (%d), starting replay...\n", i);
      print_heap();
      print_stack_map();
    }
  }
  return NULL;
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
    ctorElmInt(123),
    ctorElmInt(999),
    ctorElmInt(-7),
  };
  Cons* list = List_create(3, list_elems);
  Closure* c = ctorClosure(1, 1, eval_infinite_loop, ((void*[]){list}));

  test_execute(c, 10);

  mu_assert("should complete without triggering any assertions", true);
  return NULL;
}

// --------------------------------------------------------------------------------


char unknown_function_address[FORMAT_PTR_LEN];
char * Debug_evaluator_name(void * p) {
  if (p == eval_fib) {
    return "fib          ";
  } else if (p == Utils_le.evaluator) {
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
        "##############################################################################\n");

  mu_run_test(gc_bitmap_test);
  mu_run_test(gc_bitmap_next_test);
  mu_run_test(gc_dead_between_test);
  mu_run_test(test_heap_layout);
  mu_run_test(test_memcpy);
  mu_run_test(gc_replay_test);
  mu_run_test(stackmap_mark_eyeball_test);
  mu_run_test(assertions_test);

  return NULL;
}
