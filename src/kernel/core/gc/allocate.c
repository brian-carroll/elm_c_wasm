#include "internals.h"

/* ====================================================

                ALLOCATE & COPY

   ==================================================== */

/*
  Allocate memory on the heap
  Same interface as malloc in stdlib.h
*/
void* GC_malloc(ptrdiff_t bytes) {
  GcState* state = &gc_state;
  ptrdiff_t words = bytes / sizeof(void*);

  assert(bytes % sizeof(void*) == 0);

  size_t* replay = state->replay_ptr;
  if (replay != NULL) {  // replay mode

    assert(((Header*)replay)->size == words);
    size_t* next_replay = replay + words;
    if (next_replay >= state->next_alloc) {
      next_replay = NULL;  // exit replay mode
    }
    state->replay_ptr = next_replay;
    return (void*)replay;

  } else {  // normal (non-replay) mode

    size_t* old_heap = state->next_alloc;
    size_t* new_heap = old_heap + words;

    if (new_heap < state->heap.end) {
      state->next_alloc = new_heap;
      return old_heap;
    } else {
      return pGcFull;
    }
  }
}

void* GC_memcpy(void* dest, void* src, size_t bytes) {
#ifdef DEBUG
  if (bytes % sizeof(u16)) {  // string bodies can be 16-bit aligned
    log_error("GC_memcpy: Copy %zd bytes is misaligned\n", bytes);
  }
#endif
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
