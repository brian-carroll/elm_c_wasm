#include <stdio.h>
#include "internals.h"

/* ====================================================

                ALLOCATE

   ==================================================== */

static size_t* find_space(GcState* state, size_t* start, size_t size) {
  GcHeap* heap = &state->heap;
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
    if (next_occupied - next_available >= size) {
      state->end_of_alloc_patch = next_occupied;
      return next_available;
    }
  }
}

/*
  Allocate memory on the heap
*/
void* GC_allocate(bool push_to_stack, ptrdiff_t words) {
  GcState* state = &gc_state;
  size_t* alloc = state->next_alloc;
  size_t* new_alloc = alloc + words;

  if (new_alloc >= state->end_of_alloc_patch) {
    // Out of space in current empty patch. Find the next patch.
    alloc = find_space(state, new_alloc, words);
    if (!alloc) {
      // No more empty patches left. Mark garbage and search again
      GC_collect_minor();
      alloc = find_space(state, state->heap.start, words);
    }
    new_alloc = alloc + words;
  }

  if (push_to_stack) {
    GC_stack_push_value(alloc);
  }
  state->next_alloc = new_alloc;
  return alloc;
}


/* ====================================================

                COPY

   ==================================================== */

void* GC_memcpy(void* vdest, void* vsrc, size_t words) {
  size_t bytes = words * sizeof(void*);

  u8* dest = vdest;
  u8* src = vsrc;
  u32* src32;
  u32* dest32;
  u64* src64;
  u64* dest64;

  // Get 64-bit alignment if we don't already have it
  if ((size_t)dest % sizeof(u64)) {
    src32 = (u32*)src;
    dest32 = (u32*)dest;
    *dest32 = *src32;
    bytes -= sizeof(u32);
    src += sizeof(u32);
    dest += sizeof(u32);
  }

  // Bulk copy in 64-bit chunks
  src64 = (u64*)src;
  dest64 = (u64*)dest;
  size_t i = 0;
  for (; i < bytes / sizeof(u64); i++) {
    dest64[i] = src64[i];
  }

  // last 32 bits if needed
  if (bytes % sizeof(u64)) {
    src32 = (u32*)(&src64[i]);
    dest32 = (u32*)(&dest64[i]);
    *dest32 = *src32;
  }

  return dest;  // C standard lib returns this. Normally ignored.
}
