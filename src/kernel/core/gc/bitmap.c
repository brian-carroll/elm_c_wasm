#include "internals.h"

/* ====================================================

          BITMAP

    This collector doesn't put mark bits in the
    value headers, but rather "off to the side",
    in their own block.
    They are tightly packed for good cache performance,
    so scanning for live or garbage regions is fast.
   ==================================================== */

void bitmap_reset(GcHeap* heap) {
  size_t* bitmap = heap->bitmap;
  for (size_t i = 0; i < heap->bitmap_size; i++) {
    bitmap[i] = 0;
  }
}


#ifdef _WIN32
// #include <intrin.h>
// #define popcount(w) __popcnt64(w)
// TODO: figure out header files/options/whatever for MSVC
int popcount(u64 word) {
  u64 w = word;
  int count;
  for (count = 0; w; count++) {
    w &= w - 1;  // clear the least significant bit set
  }
  return count;
}
#elif TARGET_64BIT
#define popcount(w) __builtin_popcountll(w)
#else
#define popcount(w) __builtin_popcountl(w)
#endif


// Count garbage words between two heap pointers, using the bitmap
size_t bitmap_dead_between(GcHeap* heap, size_t* first, size_t* last) {
  size_t* bitmap = heap->bitmap;

  size_t first_index = first - heap->start;
  size_t first_word = first_index / GC_WORD_BITS;
  size_t first_bit = first_index % GC_WORD_BITS;
  size_t last_index = last - heap->start;
  size_t last_word = last_index / GC_WORD_BITS;
  size_t last_bit = last_index % GC_WORD_BITS;

  if (first_word == last_word) {
    size_t bitmask = make_bitmask(first_bit, last_bit);
    bitmask &= (bitmask >> 1); // clear highest bit (if any are set)
    size_t masked = bitmap[first_word] & bitmask;
    size_t live_count = popcount(masked);
    size_t dead_count = last_bit - first_bit - live_count;
    return dead_count;
  } else {
    size_t first_mask = make_bitmask(first_bit, GC_WORD_BITS - 1);
    size_t live_count = popcount(bitmap[first_word] & first_mask);

    for (size_t word = first_word + 1; word < last_word; ++word) {
      live_count += popcount(bitmap[word]);
    }

    size_t last_mask = make_bitmask(0, last_bit);
    last_mask &= (last_mask >> 1); // clear highest bit (if any are set)
    live_count += popcount(bitmap[last_word] & last_mask);
    size_t dead_count = last - first - live_count;

    return dead_count;
  }
}


// Make a mask to test selected bits in a bitmap word
size_t make_bitmask(size_t first_bit, size_t last_bit) {
  size_t mask = ALL_ONES;
  mask <<= GC_WORD_BITS - 1 - last_bit;
  mask >>= GC_WORD_BITS - 1 - (last_bit - first_bit);
  mask <<= first_bit;
  return mask;
}


// advance to the next bit in the bitmap (for loops)
void bitmap_next(GcBitmapIter* iter) {
  iter->mask <<= 1;
  if (iter->mask == 0) {
    iter->index++;
    iter->mask = 1;
  }
}


GcBitmapIter ptr_to_bitmap_iter(GcHeap* heap, size_t* ptr) {
  size_t heap_index = ptr - heap->start;
  GcBitmapIter iter = {
      .index = heap_index / GC_WORD_BITS,
      .mask = (size_t)1 << (heap_index % GC_WORD_BITS),
  };
  return iter;
}


size_t* bitmap_iter_to_ptr(GcHeap* heap, GcBitmapIter iter) {
  size_t* ptr = heap->start + (iter.index * GC_WORD_BITS);
  size_t mask = iter.mask;

#if TARGET_64BIT
  size_t bit = 63;
  if (mask & 0x00000000FFFFFFFF) bit -= 32;
  if (mask & 0x0000FFFF0000FFFF) bit -= 16;
  if (mask & 0x00FF00FF00FF00FF) bit -= 8;
  if (mask & 0x0F0F0F0F0F0F0F0F) bit -= 4;
  if (mask & 0x3333333333333333) bit -= 2;
  if (mask & 0x5555555555555555) bit -= 1;
#else
  size_t bit = 31;
  if (mask & 0x0000FFFF) bit -= 16;
  if (mask & 0x00FF00FF) bit -= 8;
  if (mask & 0x0F0F0F0F) bit -= 4;
  if (mask & 0x33333333) bit -= 2;
  if (mask & 0x55555555) bit -= 1;
#endif

  ptr += bit;

  if (ptr > heap->end) {
    return heap->end;
  } else {
    return ptr;
  }
}


size_t bitmap_is_live_at(GcHeap* heap, GcBitmapIter iter) {
  return heap->bitmap[iter.index] & iter.mask;
}


void bitmap_find(GcHeap* heap, bool target_value, GcBitmapIter* iter) {
  size_t size = heap->bitmap_size;
  for (; iter->index < size; bitmap_next(iter)) {
    bool value = !!(heap->bitmap[iter->index] & iter->mask);
    if (value == target_value) break;
  }
}


size_t* bitmap_find_space(
    GcHeap* heap, size_t* start, size_t min_size, size_t** end_of_space) {
  if (start >= heap->end) {
    return NULL;
  }
  GcBitmapIter iter = ptr_to_bitmap_iter(heap, start);

  for (;;) {
    bitmap_find(heap, false, &iter);
    size_t* next_available = bitmap_iter_to_ptr(heap, iter);
    if (next_available >= heap->end) {
      return NULL;
    }

    bitmap_find(heap, true, &iter);
    size_t* next_occupied = bitmap_iter_to_ptr(heap, iter);
    if (next_occupied - next_available >= min_size) {
      *end_of_space = next_occupied;
      return next_available;
    }
  }
}
