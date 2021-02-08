#include "internals.h"
#include <stdio.h>

/* ====================================================

                ALLOCATE & COPY

   ==================================================== */

/*
  Allocate memory on the heap
  Same interface as malloc in stdlib.h
*/
void* GC_malloc(bool push_to_stack, ptrdiff_t bytes) {
  GcState* state = &gc_state;
  if (state->stack_map.replay_until) {
    return malloc_replay(bytes);
  }
  ptrdiff_t words = bytes / sizeof(void*);
  assert(bytes % sizeof(void*) == 0);
  size_t* old_heap = state->next_alloc;
  size_t* new_heap = old_heap + words;

  if (new_heap >= state->heap.end) {
    // printf("OOM at %p with call stack at %s\n", old_heap,
      // Debug_evaluator_name(stack_values[state->stack_map.frame]));
    return pGcFull;
  }
  if (push_to_stack) {
    GC_stack_push_value(old_heap);
  }
  state->next_alloc = new_heap;
  return old_heap;
}


void* GC_memcpy(void* dest, void* src, size_t bytes) {
  assert(bytes % sizeof(u16) == 0);

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
