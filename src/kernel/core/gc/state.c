#include "internals.h"

#ifdef _WIN32
#include <heapapi.h>
#include <windows.h>
#else
#include <unistd.h>
#endif


/* ====================================================

        GC STATE

   ==================================================== */

// globals
GcState gc_state;

void reset_state(GcState* state) {
  void* start = state->heap.start;
  state->nursery = start;
  state->next_alloc = start;
  state->roots = &Nil;
  stack_clear();
}

/* ====================================================

        HEAP

   ==================================================== */

#ifdef _WIN32

static HANDLE hHeap;
static const DWORD dwFlags = HEAP_GENERATE_EXCEPTIONS | HEAP_ZERO_MEMORY;
static void* system_block;

void* get_initial_memory(size_t bytes) {
  // https://docs.microsoft.com/en-us/windows/win32/api/heapapi/
  assert(bytes % GC_SYSTEM_MEM_CHUNK == 0);
  hHeap = GetProcessHeap();
  system_block = HeapAlloc(hHeap, dwFlags, bytes + GC_BLOCK_BYTES);
  size_t unaligned_addr = (size_t)system_block;
  size_t aligned_addr = (unaligned_addr + GC_BLOCK_BYTES - 1) & GC_BLOCK_MASK;
  return (void*)aligned_addr;
}

void grow_memory(void* start, size_t new_total_bytes) {
  assert(new_total_bytes % GC_SYSTEM_MEM_CHUNK == 0);
  size_t alignment_bytes = (size_t)start - (size_t)system_block;
  size_t new_system_bytes = new_total_bytes + alignment_bytes;
  void* result = HeapReAlloc(hHeap, dwFlags, system_block, new_system_bytes);
  assert(result == system_block);
}

#else

void* get_initial_memory(size_t bytes) {
  size_t initial_break = (size_t)sbrk(0);
  size_t aligned_break = (initial_break + GC_BLOCK_BYTES - 1) & GC_BLOCK_MASK;
  size_t new_break = aligned_break + bytes;
  int result = brk((void*)new_break);
  assert(result == 0);
  return (void*)aligned_break;
}

void grow_memory(void* start, size_t new_total_bytes) {
  void* new_break = start + new_total_bytes;
  int result = brk(new_break);
  assert(result == 0);
}

#endif


void set_heap_layout(GcHeap* heap, size_t* start, size_t bytes) {
  size_t heap_words = bytes / SIZE_UNIT;
  size_t* system_end = start + heap_words;

  // This calculation is in bytes, not words, to prevent
  // truncation errors for smaller blocks (<1 word of bitmap)
  size_t bytes_per_word = sizeof(void*);
  size_t heap_bytes = heap_words * bytes_per_word;

  size_t bitmap_bytes_per_block = GC_BLOCK_BYTES / GC_WORD_BITS;
  size_t offset_bytes_per_block = bytes_per_word;
  size_t block_plus_overhead_bytes =
      GC_BLOCK_BYTES + bitmap_bytes_per_block + offset_bytes_per_block;

  // A fractional block needs the overhead of a full block
  size_t heap_blocks = GC_DIV_ROUND_UP(heap_bytes, block_plus_overhead_bytes);

  size_t bitmap_bytes = heap_blocks * bitmap_bytes_per_block;
  size_t offset_bytes = heap_blocks * offset_bytes_per_block;

  // Now convert to words, rounding up
  size_t bitmap_words = GC_DIV_ROUND_UP(bitmap_bytes, bytes_per_word);
  size_t offset_words = GC_DIV_ROUND_UP(offset_bytes, bytes_per_word);

  heap->start = start;
  heap->bitmap = system_end - bitmap_words;
  heap->offsets = heap->bitmap - offset_words;
  heap->end = heap->offsets;
  heap->system_end = system_end;
}


// Call exactly once on program startup
int init_heap(GcHeap* heap) {
  size_t bytes = GC_INITIAL_HEAP_MB * MB;
  void* start = get_initial_memory(bytes);

  set_heap_layout(heap, start, bytes);

  return 0;
}
