#include "internals.h"

/* ====================================================

        HEAP

  For simplicity & code size we have one region that we grow and shrink.
  That's perfect for the Wasm platform, but not ideal for OS heap APIs.
  There may be cases where the native executable would be slow or crash.

   ==================================================== */

static void* get_initial_system_memory(size_t bytes);
static void resize_system_memory(GcHeap* heap, size_t new_total_bytes);

#ifdef _WIN32

#include <windows.h>
#include <memoryapi.h>


#define GC_SYSTEM_RESERVED_BYTES (128 * MB)


void ErrorExit(const char* calling_func, int line_no) {
  // Retrieve the system error message for the last-error code
  DWORD dw = GetLastError();
  DWORD dwFlags = FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS;
  DWORD dwLanguageId = MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT);
  char buf[1024];
  DWORD count = FormatMessage(dwFlags, NULL, dw, dwLanguageId, buf, sizeof(buf), NULL);
  safe_printf("%s:%d %s\n", calling_func, line_no, buf);
  ExitProcess(dw);
}


static void* get_initial_system_memory(size_t bytes) {
  assert(bytes % GC_SYSTEM_MEM_CHUNK == 0);
  u8* reserved = VirtualAlloc(NULL, GC_SYSTEM_RESERVED_BYTES, MEM_RESERVE, PAGE_READWRITE);
  if (!reserved) {
    ErrorExit(__FUNCTION__, __LINE__);
  }

  // We need to commit one chunk at a time
  // VirtualFree can only be called with an address we explicitly committed
  for (size_t offset = 0; offset < GC_INITIAL_HEAP_MB * MB; offset += GC_SYSTEM_MEM_CHUNK) {
    if (!VirtualAlloc(reserved + offset, GC_SYSTEM_MEM_CHUNK, MEM_COMMIT, PAGE_READWRITE)) {
      ErrorExit(__FUNCTION__, __LINE__);
    }
  }
  // safe_printf("committed initial memory at %p\n", reserved);

  return reserved;
}

static void resize_system_memory(GcHeap* heap, size_t new_total_bytes) {
  assert(new_total_bytes % GC_SYSTEM_MEM_CHUNK == 0);
  u8* start = (u8*)heap->start;
  u8* end = (u8*)heap->system_end;
  size_t old_total_bytes = end - start;

  if (new_total_bytes > old_total_bytes) {
    // safe_printf("resize_system_memory: grow from %zx to %zx\n", old_total_bytes, new_total_bytes);
    for (size_t offset = old_total_bytes; offset < new_total_bytes; offset += GC_SYSTEM_MEM_CHUNK) {
      if (!VirtualAlloc(start + offset, GC_SYSTEM_MEM_CHUNK, MEM_COMMIT, PAGE_READWRITE)) {
        ErrorExit(__FUNCTION__, __LINE__);
      }
    }
  } else if (new_total_bytes < old_total_bytes) {
    // safe_printf("resize_system_memory: shrink from %zx to %zx\n", old_total_bytes, new_total_bytes);
    for (size_t offset = new_total_bytes; offset < old_total_bytes; offset += GC_SYSTEM_MEM_CHUNK) {
      if (!VirtualFree(start + offset, GC_SYSTEM_MEM_CHUNK, MEM_DECOMMIT)) {
        ErrorExit(__FUNCTION__, __LINE__);
      }
    }
  } else {
    safe_printf("resize_system_memory: nothing to do\n");
  }
}

#else

#include <unistd.h>

static void* get_initial_system_memory(size_t bytes) {
  size_t initial_break = (size_t)sbrk(0);
  size_t aligned_addr = (initial_break + GC_BLOCK_BYTES - 1) & GC_BLOCK_MASK;
  void* aligned_break = (void*)aligned_addr;
  void* new_break = aligned_break + bytes;
  brk(new_break);
  assert(sbrk(0) == new_break);
  return aligned_break;
}

static void resize_system_memory(GcHeap* heap, size_t new_total_bytes) {
  void* new_break = ((void*)heap->start) + new_total_bytes;
  if (new_break == (void*)heap->system_end) {
    return;
  }
  brk(new_break);
  assert(sbrk(0) == new_break);
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

  // fill out metadata from the top down
  heap->system_end = system_end;

  heap->gc_temp_size = offset_words;
  heap->gc_temp = system_end - offset_words;

  heap->bitmap_size = bitmap_words;
  heap->bitmap = heap->gc_temp - bitmap_words;

  heap->end = heap->bitmap;
  heap->start = start;
}


int init_heap(GcHeap* heap) {
  size_t bytes = GC_INITIAL_HEAP_MB * MB;
  void* start = heap->start;
  if (start) {
    resize_system_memory(heap, bytes);
  } else {
    start = get_initial_system_memory(bytes);
  }

  set_heap_layout(heap, start, bytes);

  return 0;
}


// Grow to 2x, or enough to fit the current allocation, whichever is larger
void grow_heap(GcHeap* heap, size_t current_alloc_words) {
  GcHeap old_heap = *heap;
  size_t old_total_words = heap->system_end - heap->start;

  size_t alloc_with_overhead = current_alloc_words + (current_alloc_words >> 4);
  size_t extra_words =
      (alloc_with_overhead > old_total_words) ? alloc_with_overhead : old_total_words;
  size_t new_total_words = old_total_words + extra_words;
  size_t new_total_bytes =
      GC_ROUND_UP(new_total_words * sizeof(void*), GC_SYSTEM_MEM_CHUNK);

  resize_system_memory(heap, new_total_bytes);
  set_heap_layout(heap, heap->start, new_total_bytes);

  // GC bookkeeping data
  GC_memcpy(heap->bitmap, old_heap.bitmap, old_heap.bitmap_size);
  for (size_t i = 0; i < old_heap.gc_temp_size; ++i) {
    old_heap.gc_temp[i] = 0;
  }
}
