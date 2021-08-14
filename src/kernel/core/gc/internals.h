/*
  GC internal structures and definitions, not part of public API
*/

#ifndef ELM_KERNEL_GC_INTERNALS
#define ELM_KERNEL_GC_INTERNALS

#include <errno.h>
#include <stdbool.h>

#include "../core.h"

#define MB (1024 * 1024)
#define GC_INITIAL_HEAP_MB 1
#define GC_WASM_PAGE_BYTES 65536
#define GC_SYSTEM_ALLOC_BYTES 65536
#define GC_SYSTEM_MEM_CHUNK 65536  // allocation granularity for Wasm, and for Windows
#define GC_BLOCK_WORDS 64
#define GC_BLOCK_BYTES (GC_BLOCK_WORDS * sizeof(size_t))
#define GC_BLOCK_MASK (-GC_BLOCK_BYTES)
#define GC_DIV_ROUND_UP(num, den) ((num + den - 1) / den)
#define GC_ROUND_UP(num, pow2) ((num & (-(size_t)pow2)) + pow2)
#define GC_STACK_MAP_SIZE (10 * 1024)
#define GC_DO_SWEEP 0

#define ALL_ONES ((u64)(-1))

typedef struct {
  size_t* start;
  size_t* end;  // end of application heap area
  u64* bitmap;  // GC mark bits
  size_t bitmap_size;
  size_t* gc_temp;  // GC internal working memory area
  size_t gc_temp_size;
  size_t* system_end;  // end of entire heap including GC reserved areas
} GcHeap;

typedef struct {
  GcStackMapIndex frame;
  GcStackMapIndex index;
} GcStackMap;

typedef struct {
  u64 mask;
  size_t index;
} GcBitmapIter;

typedef struct {
  GcHeap heap;
  size_t* next_alloc;
  size_t* end_of_alloc_patch;
  size_t* end_of_old_gen;
  size_t n_marked_words;
  Cons* roots;
  GcStackMap stack_map;
} GcState;

extern GcState gc_state;
extern void* stack_values[GC_STACK_MAP_SIZE];
extern char stack_flags[GC_STACK_MAP_SIZE];
extern void (*gc_test_mark_callback)();

void mark(GcState* state, size_t* ignore_below);
void compact(GcState* state, size_t* compact_start);

void reset_state(GcState* state);
int init_heap(GcHeap* heap);
void* GC_get_memory_from_system(size_t bytes);
void grow_heap(GcHeap* heap, size_t current_alloc_words);
void resize_system_memory(GcHeap* heap, size_t new_total_bytes);
void move_metadata_after_resize(GcHeap* old_heap, GcHeap* new_heap);
void set_heap_layout(GcHeap* heap, size_t* start, size_t bytes);
size_t next_heap_size_bytes(GcHeap* heap, size_t current_alloc_words);

void stack_reset();

void bitmap_reset(GcHeap*);
size_t bitmap_dead_between(GcHeap* heap, size_t* first, size_t* last);
u64 make_bitmask(size_t first_bit, size_t last_bit);
GcBitmapIter ptr_to_bitmap_iter(GcHeap* heap, size_t* ptr);
size_t* bitmap_iter_to_ptr(GcHeap* heap, GcBitmapIter iter);
void bitmap_find(GcHeap* heap, bool target_value, GcBitmapIter* iter);
size_t* bitmap_find_space(GcHeap* heap, size_t* start, size_t min_size, size_t** end_of_space);

size_t child_count(ElmValue* v);

#define IS_OUTSIDE_HEAP(p) (heap->start > (size_t*)p || heap->end <= (size_t*)p)

#endif
