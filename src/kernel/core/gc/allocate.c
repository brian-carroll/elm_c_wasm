#include "internals.h"

/* ====================================================

                ALLOCATE

   ==================================================== */

/*
  Allocate memory on the heap
    push_to_stack: should be `true` in 99.99% of cases. Only false for fancy perf tricks
    alloc_words: Number of words to allocate. A word is the size of a pointer, 32 or 64 bits.
*/
void* GC_allocate(bool push_to_stack, ptrdiff_t alloc_words) {
  GcState* state = &gc_state;
  GcHeap* heap = &state->heap;
  size_t* alloc = state->next_alloc;
  size_t* end_of_alloc_patch = state->end_of_alloc_patch;

  size_t* new_alloc = alloc + alloc_words;

  if (new_alloc >= end_of_alloc_patch) {
    alloc = bitmap_find_space(heap, end_of_alloc_patch, alloc_words, &end_of_alloc_patch);
    if (!alloc) {
      GC_collect_minor();

      size_t new_gen_size = state->heap.end - state->end_of_old_gen;
      size_t used_size = state->n_marked_words;
      size_t free_size = new_gen_size - used_size;
      size_t fullness_threshold = new_gen_size / 2;
      bool too_full = used_size > fullness_threshold;
      bool large_alloc = alloc_words > free_size;
      bool should_grow = too_full || large_alloc;

      if (!should_grow) {
        alloc = bitmap_find_space(heap, state->end_of_old_gen, alloc_words, &end_of_alloc_patch);
        should_grow = !alloc;  // handle failure due to fragmentation
      }
      if (should_grow) {
        size_t* old_end = heap->end;
        grow_heap(heap, alloc_words);
        alloc = old_end;
        end_of_alloc_patch = heap->end;
        safe_printf("grew heap by ");
        print_ptr_diff_size(old_end, heap->end);
        safe_printf("\n");
      }
    }
    new_alloc = alloc + alloc_words;
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

#if !TARGET_64BIT
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

#if !TARGET_64BIT
  // last 32 bits if needed
  if (bytes % sizeof(u64)) {
    src32 = (u32*)(&src64[i]);
    dest32 = (u32*)(&dest64[i]);
    *dest32 = *src32;
  }
#endif

  return dest;  // C standard lib returns this. Normally ignored.
}
