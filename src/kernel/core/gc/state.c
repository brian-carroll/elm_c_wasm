#include "internals.h"

#ifdef _WIN32
#include <heapapi.h>
#else
#include <sys/mman.h>
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

HANDLE hHeap;

void* get_memory_from_system(size_t bytes) {
  // https://docs.microsoft.com/en-us/windows/win32/api/heapapi/
  assert(bytes % GC_SYSTEM_MEM_CHUNK == 0);
  return HeapAlloc(hHeap, 0, bytes);
}

#else

void* get_memory_from_system(size_t bytes) {
  // mmap can map files into memory but apparently everyone
  // uses it just for plain old memory allocation too.
  assert(bytes % GC_SYSTEM_MEM_CHUNK == 0);
  void *addr = NULL; // requested starting address
  size_t length = bytes;
  int prot = PROT_READ | PROT_WRITE;
  int flags = MAP_PRIVATE | MAP_ANONYMOUS; // MAP_ANONYMOUS = memory not backed by a file
  int fd = -1; // file descriptor for file-backed memory, or -1
  off_t offset = 0; // file offset for the memory block to start at

  void* allocated = mmap(addr, length, prot, flags, fd, offset);
  return allocated;
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

#ifdef _WIN32
  hHeap = HeapCreate(0, GC_INITIAL_HEAP_MB * MB, GC_MAX_HEAP_MB * MB);
#endif

  size_t bytes = GC_INITIAL_HEAP_MB * MB;
  void* start = get_memory_from_system(bytes);

  set_heap_layout(heap, start, bytes);

  return 0;
}

/* ====================================================

          BITMAP

    This collector doesn't put mark bits in the
    value headers, but rather "off to the side",
    in their own block.
    They are tightly packed for good L1 cache performance,
    so scanning for live or garbage regions is fast.
   ==================================================== */

void bitmap_reset(GcHeap* heap) {
  for (size_t* bm_word = heap->bitmap; bm_word < heap->system_end; bm_word++) {
    *bm_word = 0;
  }
}


// Count garbage words between two heap pointers, using the bitmap
size_t bitmap_dead_between(GcHeap* heap, size_t* first, size_t* last) {
  size_t first_index = (size_t)(first - heap->start);
  size_t first_word = first_index / GC_WORD_BITS;
  size_t first_mask = (size_t)1 << (first_index % GC_WORD_BITS);

  size_t last_index = (size_t)(last - heap->start);
  size_t last_word = last_index / GC_WORD_BITS;
  size_t last_mask = (size_t)1 << (last_index % GC_WORD_BITS);

  size_t count = 0;
  size_t word = first_word;
  size_t mask = first_mask;

  while (word < last_word) {
    if ((heap->bitmap[word] & mask) == 0) count++;
    bitmap_next(&word, &mask);
  }
  while (mask < last_mask) {
    if ((heap->bitmap[word] & mask) == 0) count++;
    mask <<= 1;
  }
  return count;
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
void bitmap_next(size_t* word, size_t* mask) {
  *mask <<= 1;
  if (*mask == 0) {
    *word = *word + 1;
    *mask = 1;
  }
}
