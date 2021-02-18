#include "internals.h"

#ifdef _MSC_VER
// Totally broken nonsense, just to see if we can compile without errors
#include <stdint.h>
void* sbrk(intptr_t x) {
  return (void*)x;
}
int brk(void* addr) {
  return (int)(size_t)addr;
}
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

void set_heap_layout(GcHeap* heap, size_t* new_break_ptr) {
  size_t heap_words = new_break_ptr - heap->start;

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

  heap->bitmap = new_break_ptr - bitmap_words;
  heap->offsets = heap->bitmap - offset_words;
  heap->end = heap->offsets;
  heap->system_end = new_break_ptr;
}


// currently only called on init but need later for growing the heap
int set_heap_end(GcHeap* heap, size_t* new_break_ptr) {
  int has_error = brk(new_break_ptr); // TODO
  if (has_error) {
    log_error("Failed to get heap memory. Error code %d\n", errno);
    return errno;
  }
  set_heap_layout(heap, new_break_ptr);
  return 0;
}


// Call exactly once on program startup
int init_heap(GcHeap* heap) {
  // Get current max address of program data
  size_t* break_ptr = sbrk(0);  // TODO
  // https://docs.microsoft.com/en-us/windows/win32/api/heapapi/nf-heapapi-heapcreate
  // HeapCreate 

  // Align to next page boundary
  size_t break_aligned = (size_t)break_ptr;
  if (break_aligned % GC_WASM_PAGE_BYTES) {
    break_aligned |= (GC_WASM_PAGE_BYTES - 1);
    break_aligned++;
  }
  size_t* heap_bottom = (size_t*)break_aligned;

  // Initialise heap with zero size
  *heap = (GcHeap){
      .start = heap_bottom,
      .end = heap_bottom,
      .system_end = heap_bottom,
      .bitmap = heap_bottom,
      .offsets = heap_bottom,
  };

  // Ask the system for more memory
  size_t top_of_current_page = (size_t)heap_bottom | (size_t)(GC_WASM_PAGE_BYTES - 1);
  size_t pages = (GC_INITIAL_HEAP_MB * 1024 * 1024) / GC_WASM_PAGE_BYTES;
  size_t* top_of_nth_page =
      (size_t*)(top_of_current_page + (pages * GC_WASM_PAGE_BYTES) + 1);

  return set_heap_end(heap, top_of_nth_page);
}

/* ====================================================

          BITMAP

    This collector doesn't put mark bits in the
    value headers, but rather "off to the side",
    in their own block.
    Scanning for live/garbage regions is fast.
    The mark bits for 2-4kB of heap data can fit
    into a single L1 cache line.
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
