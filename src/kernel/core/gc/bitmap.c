#include "../types.h"
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
  u64* bitmap = heap->bitmap;
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
#else
#define popcount(w) __builtin_popcountll(w)
#endif


// Count garbage words between two heap pointers, using the bitmap
size_t bitmap_dead_between(GcHeap* heap, size_t* first, size_t* last) {
  u64* bitmap = heap->bitmap;

  size_t first_heap_index = first - heap->start;
  size_t first_bmp_index = first_heap_index / 64;
  size_t first_bit = first_heap_index % 64;
  size_t last_heap_index = last - heap->start;
  size_t last_bmp_index = last_heap_index / 64;
  size_t last_bit = last_heap_index % 64;

  if (first_bmp_index == last_bmp_index) {
    u64 bitmask = make_bitmask(first_bit, last_bit);
    bitmask &= (bitmask >> 1);  // clear highest bit (if any are set)
    u64 masked = bitmap[first_bmp_index] & bitmask;
    size_t live_count = popcount(masked);
    size_t dead_count = last_bit - first_bit - live_count;
    return dead_count;
  } else {
    u64 first_mask = make_bitmask(first_bit, 64 - 1);
    size_t live_count = popcount(bitmap[first_bmp_index] & first_mask);

    for (size_t i = first_bmp_index + 1; i < last_bmp_index; ++i) {
      live_count += popcount(bitmap[i]);
    }

    u64 last_mask = make_bitmask(0, last_bit);
    last_mask &= (last_mask >> 1);  // clear highest bit (if any are set)
    live_count += popcount(bitmap[last_bmp_index] & last_mask);
    size_t dead_count = last - first - live_count;

    return dead_count;
  }
}


// Make a mask to test selected bits in a bitmap word
u64 make_bitmask(size_t first_bit, size_t last_bit) {
  u64 mask = ALL_ONES;
  mask <<= 63 - last_bit;
  mask >>= 63 - (last_bit - first_bit);
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
      .index = heap_index / 64,
      .mask = (u64)1 << (heap_index % 64),
  };
  return iter;
}


size_t* bitmap_iter_to_ptr(GcHeap* heap, GcBitmapIter iter) {
  size_t* ptr = heap->start + (iter.index * 64);
  u64 mask = iter.mask;

  size_t bit = 63;
  if (mask & 0x00000000FFFFFFFF) bit -= 32;
  if (mask & 0x0000FFFF0000FFFF) bit -= 16;
  if (mask & 0x00FF00FF00FF00FF) bit -= 8;
  if (mask & 0x0F0F0F0F0F0F0F0F) bit -= 4;
  if (mask & 0x3333333333333333) bit -= 2;
  if (mask & 0x5555555555555555) bit -= 1;

  ptr += bit;

  if (ptr > heap->end) {
    return heap->end;
  } else {
    return ptr;
  }
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
  size_t start_index = (start - heap->start) / 64;
  u64* bitmap = heap->bitmap;
  size_t i = start_index + 1;

  for (;;) {
    for (; i < heap->bitmap_size; i++) {
      if (!bitmap[i]) break;
    }
    if (i >= heap->bitmap_size) {
      return NULL;
    }
    size_t* free_ptr = heap->start + i * 64;

    for (i++; i < heap->bitmap_size; i++) {
      if (bitmap[i]) break;
    }

    size_t* live_ptr;
    if (i >= heap->bitmap_size) {
      live_ptr = heap->end;
      if (live_ptr - free_ptr < min_size) {
        return NULL;
      }
    } else {
      live_ptr = heap->start + i * 64;
      if (live_ptr - free_ptr < min_size) {
        continue;
      }
    }

    *end_of_space = live_ptr;
    return free_ptr;
  }
}
