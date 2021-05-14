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


char gc_bitmap_next_test_str[100];

char* gc_bitmap_next_test() {
  if (verbose) {
    safe_printf("\n");
    safe_printf("## gc_bitmap_next_test\n");
    safe_printf("\n");
  }
  gc_test_reset();

  GcBitmapIter iter;

  int assertion = 1;

  iter.index = 0;
  iter.mask = 1;
  stbsp_sprintf(gc_bitmap_next_test_str,
      "bitmap_next assertion %d from word %zd mask %016x",
      assertion++,
      iter.index,
      iter.mask);
  bitmap_next(&iter);
  mu_assert(gc_bitmap_next_test_str, iter.index == 0 && iter.mask == 2);

  iter.index = 0;
  iter.mask = 2;
  stbsp_sprintf(gc_bitmap_next_test_str,
      "bitmap_next assertion %d from word %zd mask %016x",
      assertion++,
      iter.index,
      iter.mask);
  bitmap_next(&iter);
  mu_assert(gc_bitmap_next_test_str, iter.index == 0 && iter.mask == 4);

  iter.index = 1;
  iter.mask = 1;
  stbsp_sprintf(gc_bitmap_next_test_str,
      "bitmap_next assertion %d from word %zd mask %016x",
      assertion++,
      iter.index,
      iter.mask);
  bitmap_next(&iter);
  mu_assert(gc_bitmap_next_test_str, iter.index == 1 && iter.mask == 2);

  iter.index = 1;
  iter.mask = 2;
  stbsp_sprintf(gc_bitmap_next_test_str,
      "bitmap_next assertion %d from word %zd mask %016x",
      assertion++,
      iter.index,
      iter.mask);
  bitmap_next(&iter);
  mu_assert(gc_bitmap_next_test_str, iter.index == 1 && iter.mask == 4);

  iter.index = 0;
  char* format_str;
  iter.mask = 0x8000000000000000;
  format_str = "bitmap_next assertion %d from word %zd mask %016zx";

  mu_expect_equal("bitmap_next: highest bit is correctly set in test", iter.mask << 1, 0);
  assertion++;

  stbsp_sprintf(gc_bitmap_next_test_str, format_str, assertion++, iter.index, iter.mask);
  bitmap_next(&iter);
  mu_assert(gc_bitmap_next_test_str, iter.index == 1 && iter.mask == 1);

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

  return NULL;
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


void test_bitmap_find_space() {
  if (verbose) {
    safe_printf("\n");
    safe_printf("## test_bitmap_find_space\n");
    safe_printf("\n");
  }
  GcHeap* heap = &gc_state.heap;
  size_t* end_of_space;
  size_t* alloc;
  size_t* end_of_bitmap = heap->end;

  bitmap_reset(heap);
  alloc = bitmap_find_space(heap, heap->start, 1, &end_of_space);
  mu_assert("should find whole heap when heap is empty",
      (alloc == heap->start) && (end_of_space == end_of_bitmap));


  bitmap_reset(heap);
  mark_words(&gc_state, heap->start + 1, 1);
  alloc = bitmap_find_space(heap, heap->start, 1, &end_of_space);
  mu_assert("should find a one-word gap at start of heap",
      (alloc == heap->start) && (end_of_space == heap->start + 1));


  bitmap_reset(heap);
  mark_words(&gc_state, heap->start, 63);
  alloc = bitmap_find_space(heap, heap->start, 1, &end_of_space);
  mu_assert("should find space just before a bitmap boundary",
      (alloc == heap->start + 63) && (end_of_space == end_of_bitmap));


  bitmap_reset(heap);
  mark_words(&gc_state, heap->start, 64);
  alloc = bitmap_find_space(heap, heap->start, 1, &end_of_space);
  mu_assert("should find space just after a bitmap boundary",
      (alloc == heap->start + 64) && (end_of_space == end_of_bitmap));


  bitmap_reset(heap);
  mark_words(&gc_state, heap->start + 63, 1);
  alloc = bitmap_find_space(heap, heap->start, 1, &end_of_space);
  mu_assert("should find space from start to before a bitmap boundary",
      (alloc == heap->start) && (end_of_space == heap->start + 63));


  bitmap_reset(heap);
  mark_words(&gc_state, heap->start + 64, 1);
  alloc = bitmap_find_space(heap, heap->start, 1, &end_of_space);
  mu_assert("should find space from start to after a bitmap boundary",
      (alloc == heap->start) && (end_of_space == heap->start + 64));
}


// --------------------------------------------------------------------------------


void gc_bitmap_test() {
  if (verbose) {
    safe_printf("\n");
    safe_printf("gc_bitmap_test\n");
    safe_printf("-----------\n");
  }

  mu_run_test(gc_bitmap_next_test);
  mu_run_test(gc_dead_between_test);
  mu_run_test(test_bitmap_iter_conversions);
  mu_run_test(test_bitmap_find);
  mu_run_test(test_bitmap_find_space);
}
