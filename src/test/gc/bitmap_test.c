#include <stdlib.h>
#include <string.h>
#include "../../kernel/core/core.h"
#include "../../kernel/core/gc/internals.h"
#include "../test.h"

void gc_test_reset();
bool mark_words(GcState* state, void* p_void, size_t size);

GcBitmapIter start_iter = {
    .index = 0,
    .mask = 1,
};


// --------------------------------------------------------------------------------


void test_bitmap_dead_between() {
  gc_test_reset();
  GcState* state = &gc_state;
  GcHeap* heap = &state->heap;

  size_t* first;
  size_t* last;

  heap->bitmap[0] = 0xf0f;
  first = heap->start + 4;
  last = heap->start + 8;
  size_t result = bitmap_dead_between(heap, first, last);
  mu_expect_equal("bitmap_dead_between with 4 words dead", result, 4);

  first--;
  last++;

  result = bitmap_dead_between(heap, first, last);
  mu_expect_equal("bitmap_dead_between with 4 dead and 2 live", result, 4);

  heap->bitmap[0] = -16;  // 4 dead
  heap->bitmap[1] = -16;
  heap->bitmap[2] = -16;
  first = heap->start + 2;            // skip 2 dead slots
  last = heap->start + (2 * 64) + 2;  // skip 2 dead slots

  result = bitmap_dead_between(heap, first, last);
  mu_expect_equal("bitmap_dead_between across 3 bitmap words", result, 8);
}


// --------------------------------------------------------------------------------


void test_bitmap_iter_example(char* description, size_t* p) {
  GcHeap* heap = &gc_state.heap;
  GcBitmapIter iter = ptr_to_bitmap_iter(heap, p);
  size_t* result = bitmap_iter_to_ptr(heap, iter);
  mu_expect_equal(description, result, p);
}

void test_bitmap_iter_conversions() {
  if (verbose) {
    safe_printf("\n");
    safe_printf("## test_bitmap_iter_conversions\n");
    safe_printf("\n");
  }
  GcHeap* heap = &gc_state.heap;
  test_bitmap_iter_example("heap start", heap->start);
  test_bitmap_iter_example("heap start + 1", heap->start + 1);
  test_bitmap_iter_example("just below a bitmap word boundary", heap->start + 63);
  test_bitmap_iter_example("just above a bitmap word boundary", heap->start + 64);
  test_bitmap_iter_example("heap end - 1", heap->end - 1);
}


// --------------------------------------------------------------------------------


void test_bitmap_find() {
  if (verbose) {
    safe_printf("\n");
    safe_printf("## test_bitmap_find\n");
    safe_printf("\n");
  }
  GcHeap* heap = &gc_state.heap;
  GcBitmapIter iter;

  size_t* pointers[] = {
      heap->start,
      heap->start + 1,
      heap->end - 1,
  };
  char* descriptions[] = {
      "should find a marked value at heap->start",
      "should find a marked value at heap->start + 1",
      "should find a marked value at heap->end - 1",
  };

  for (int i = 0; i < sizeof(pointers) / sizeof(size_t*); ++i) {
    size_t* p = pointers[i];
    bitmap_reset(heap);
    iter = start_iter;
    mark_words(&gc_state, p, 1);
    bitmap_find(heap, true, &iter);
    size_t* found = bitmap_iter_to_ptr(heap, iter);
    mu_expect_equal(descriptions[i], found, p);
  }

  // Nothing to find
  bitmap_reset(heap);
  iter = start_iter;
  bitmap_find(heap, true, &iter);
  size_t* actual = bitmap_iter_to_ptr(heap, iter);
  size_t* expected = heap->end;
  mu_expect_equal("should return heap.end when nothing is marked", actual, expected);
}


// --------------------------------------------------------------------------------

const size_t ALLOC_CHUNK = 8;

void expect_alloc(char* message,
    size_t* expected_start,
    size_t* expected_end,
    size_t* actual_start,
    size_t* actual_end) {
  assertions_made++;
  if (actual_start != expected_start || actual_end != expected_end) {
    tests_failed++;
    safe_printf("FAIL: %s\n  expected: 0x%zx -> 0x%zx\n    actual: 0x%zx -> 0x%zx\n",
        message,
        expected_start,
        expected_end,
        actual_start,
        actual_end);
  } else if (verbose) {
    safe_printf("PASS: %s\n", message);
  }
}


void* align_up(size_t align_bytes, void* p) {
  size_t addr = (size_t)p;
  addr |= align_bytes - 1;
  addr++;
  return (void*)addr;
}


void test_bitmap_find_space() {
  if (verbose) {
    safe_printf("\n");
    safe_printf("## test_bitmap_find_space\n");
    safe_printf("\n");
  }
  GcHeap* heap = &gc_state.heap;
  size_t* end_of_space;
  size_t* alloc;


  bitmap_reset(heap);
  end_of_space = NULL;
  alloc = bitmap_find_space(heap, heap->start, 1, &end_of_space);
  expect_alloc("should find entire heap when it is empty",
      heap->start,
      heap->end,
      alloc,
      end_of_space);


  bitmap_reset(heap); 
  end_of_space = NULL;
  mark_words(&gc_state, heap->start + 64, 1);
  alloc = bitmap_find_space(heap, heap->start, 1, &end_of_space);
  expect_alloc("should find a 64-word space at the start",
      heap->start,
      heap->start + 64,
      alloc,
      end_of_space);


  bitmap_reset(heap); 
  end_of_space = NULL;
  mark_words(&gc_state, heap->start, 64);
  mark_words(&gc_state, heap->start + 128, 1);
  alloc = bitmap_find_space(heap, heap->start, 1, &end_of_space);
  expect_alloc("should find a 64-word space after a 64-word live section",
      heap->start + 64,
      heap->start + 128,
      alloc,
      end_of_space);


  bitmap_reset(heap); 
  end_of_space = NULL;
  mark_words(&gc_state, heap->start, 68);
  mark_words(&gc_state, heap->start + 128, 64);
  alloc = bitmap_find_space(heap, heap->start, 1, &end_of_space);
  expect_alloc("should skip over a 60-word space",
      heap->start + 192,
      heap->end,
      alloc,
      end_of_space);


  bitmap_reset(heap); 
  end_of_space = NULL;
  mark_words(&gc_state, heap->start + 1, 1);
  alloc = bitmap_find_space(heap, heap->start, 1, &end_of_space);
  expect_alloc("should skip over one space and one allocation",
      heap->start + ALLOC_CHUNK,
      heap->end,
      alloc,
      end_of_space);


  size_t* last_word = align_up(64 * sizeof(size_t), heap->end - 64);
  size_t* before_last_word = align_up(64 * sizeof(size_t), heap->end - 128);

  bitmap_reset(heap); 
  end_of_space = NULL;
  mark_words(&gc_state, heap->start, last_word - heap->start);
  alloc = bitmap_find_space(heap, heap->start, 1, &end_of_space);
  expect_alloc("should find space in the last word of the bitmap",
      last_word,
      heap->end,
      alloc,
      end_of_space);


  bitmap_reset(heap);
  end_of_space = NULL;
  mark_words(&gc_state, heap->start, before_last_word - heap->start);
  mark_words(&gc_state, last_word, heap->end - last_word);
  alloc = bitmap_find_space(heap, heap->start, 1, &end_of_space);

  expect_alloc("should find a 64-word space just before the end of the heap",
      before_last_word,
      last_word,
      alloc,
      end_of_space);


  bitmap_reset(heap); 
  end_of_space = NULL;
  mark_words(&gc_state, heap->start, 4);
  mark_words(&gc_state, heap->end - 4, 4);
  alloc = bitmap_find_space(heap, heap->start, 1, &end_of_space);
  size_t last_chunk = (size_t)heap->end & (-ALLOC_CHUNK * sizeof(size_t));
  expect_alloc(
      "should be able to expand into free space in the first and last bitmap words",
      heap->start + ALLOC_CHUNK,
      (size_t*)last_chunk,
      alloc,
      end_of_space);
}


// --------------------------------------------------------------------------------


#define bitmap_find_space_old bitmap_find_space

void perf_bitmap_find_space() {
  if (verbose) {
    safe_printf("\n");
    safe_printf("perf_bitmap_find_space\n");
    safe_printf("----------------------\n");
  }
  gc_test_reset();

  GcState* state = &gc_state;
  GcHeap* heap = &gc_state.heap;
  size_t heap_size = heap->end - heap->start;

  // fill up the bitmap with a random pattern
  while (state->n_marked_words < heap_size / 2) {
    size_t* p = heap->start + (rand() % heap_size);
    size_t size = rand() % 100;
    if (p + size > heap->end) {
      size = heap->end - p;
    }
    mark_words(&gc_state, p, size);
  }
  // PRINT_BITMAP();

  PerfTime time_new = 0;
  PerfTime time_old = 0;
  size_t space_new;
  size_t space_old;

  for (int i = 0; i < 10; i++) {
    PerfTime time_start = PERF_GET_TIME();

    space_new = 0;
    for (size_t *end_of_alloc_patch = heap->start, *alloc = heap->start; alloc;
         alloc = bitmap_find_space(heap, end_of_alloc_patch, 0, &end_of_alloc_patch)) {
      space_new += end_of_alloc_patch - alloc;
    }

    PerfTime time_middle = PERF_GET_TIME();

    space_old = 0;
    for (size_t *end_of_alloc_patch = heap->start, *alloc = heap->start; alloc;
         alloc =
             bitmap_find_space_old(heap, end_of_alloc_patch, 0, &end_of_alloc_patch)) {
      space_old += end_of_alloc_patch - alloc;
    }

    PerfTime time_end = PERF_GET_TIME();

    time_new += time_middle - time_start;
    time_old += time_end - time_middle;
  }

  // Can easily get ~10% "speedup" with the same function! :(
  float speedup = 100.0 * (((float)time_old / (float)time_new) - 1.0);
  float extra_space = 100.0 * (((float)space_new / (float)space_old) - 1.0);

  safe_printf("space_new   %zd\n", space_new);
  safe_printf("space_old   %zd\n", space_old);
  safe_printf("extra_space %.2f%%\n", extra_space);
  safe_printf("\n");
  safe_printf("time_new " PERF_FORMAT "\n", time_new);
  safe_printf("time_old " PERF_FORMAT "\n", time_old);
  safe_printf("speedup %.2f%%\n", speedup);
}


// --------------------------------------------------------------------------------


void gc_bitmap_test() {
  if (verbose) {
    safe_printf("\n");
    safe_printf("gc_bitmap_test\n");
    safe_printf("-----------\n");
  }

  mu_run_test(test_bitmap_dead_between);
  mu_run_test(test_bitmap_iter_conversions);
  mu_run_test(test_bitmap_find);
  mu_run_test(test_bitmap_find_space);

  // mu_run_test(perf_bitmap_find_space);
}
