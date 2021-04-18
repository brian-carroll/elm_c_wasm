#include <stdio.h>
#include "internals.h"

/* ====================================================

                ALLOCATE

   ==================================================== */

/*
  Allocate memory on the heap
    push_to_stack: should be `true` in 99.99% of cases. Only false for fancy perf tricks
    words: Number of words to allocate. A word is the size of a pointer, 32 or 64 bits.
*/
void* GC_allocate(bool push_to_stack, ptrdiff_t words) {
  GcState* state = &gc_state;
  GcHeap* heap = &state->heap;
  size_t* alloc = state->next_alloc;
  size_t* end_of_alloc_patch = state->end_of_alloc_patch;

  size_t* new_alloc = alloc + words;

  if (new_alloc >= end_of_alloc_patch) {
    // Out of space in current empty patch. Find the next patch.
    alloc = bitmap_find_space(heap, end_of_alloc_patch, words, &end_of_alloc_patch);
    if (!alloc) {
      // No more empty patches left. Mark the heap and search again
      size_t* old_end = heap->end;
      bool grew = GC_collect_minor(); // only affects heap, not the rest of state
      if (grew) {
        // Use the new space. Allocation will be faster than if we found a gap.
        alloc = old_end;
        end_of_alloc_patch = heap->end;
      } else {
        alloc = bitmap_find_space(heap, heap->start, words, &end_of_alloc_patch);
      }
    }
    new_alloc = alloc + words;
    state->end_of_alloc_patch = end_of_alloc_patch;
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
  u64* src64;
  u64* dest64;

#ifndef TARGET_64BIT
  u32* src32;
  u32* dest32;

  // Get 64-bit alignment if we don't already have it
  if ((size_t)dest % sizeof(u64)) {
    src32 = (u32*)src;
    dest32 = (u32*)dest;
    *dest32 = *src32;
    bytes -= sizeof(u32);
    src += sizeof(u32);
    dest += sizeof(u32);
  }
#endif

  // Bulk copy in 64-bit chunks
  src64 = (u64*)src;
  dest64 = (u64*)dest;
  size_t i = 0;
  for (; i < bytes / sizeof(u64); i++) {
    dest64[i] = src64[i];
  }

#ifndef TARGET_64BIT
  // last 32 bits if needed
  if (bytes % sizeof(u64)) {
    src32 = (u32*)(&src64[i]);
    dest32 = (u32*)(&dest64[i]);
    *dest32 = *src32;
  }
#endif

  return dest;  // C standard lib returns this. Normally ignored.
}
