/*
  GC internal structures and definitions, not part of public API
*/

#ifndef ELM_KERNEL_GC_INTERNALS
#define ELM_KERNEL_GC_INTERNALS

#include <stdbool.h>
#include <assert.h>
#include <errno.h>

#include "../types.h"
#include "../gc.h"
#include "../debug.h"
#include "../../wrapper/wrapper.h"

#define GC_INITIAL_HEAP_MB 8
#define GC_WASM_PAGE_BYTES 65536
#define GC_BLOCK_WORDS 64
#define GC_BLOCK_BYTES (GC_BLOCK_WORDS * sizeof(size_t))
#define GC_BLOCK_MASK (-GC_BLOCK_BYTES)
#define GC_WORD_BITS (sizeof(size_t) * 8)
#define GC_DIV_ROUND_UP(num, den) ((num + den - 1) / den)
#define GC_STACK_MAP_SIZE (1024 * 1024 / sizeof(void*))

#define ALL_ONES -1

typedef struct {
  size_t* start;
  size_t* end;  // end of usable heap area
  size_t* offsets;
  size_t* bitmap;
  size_t* system_end;  // end of heap including GC reserved area (bitmap & offsets)
} GcHeap;

typedef struct {
  GcHeap heap;
  size_t* next_alloc;
  size_t* nursery;
  Cons* roots;

  Closure* entry;
  void** stack_values;
  char* stack_flags;
  EvalFunction* stack_functions;
  EvalFunction* call_stack;
  GcStackMapIndex stack_index;
  GcStackMapIndex call_stack_index;
  GcStackMapIndex replay_until;
} GcState;

extern GcState gc_state;

void mark(GcState* state, size_t* ignore_below);
void compact(GcState* state, size_t* compact_start);

void reset_state(GcState* state);
int init_heap(GcHeap* heap);

void bitmap_reset(GcHeap*);
size_t bitmap_dead_between(GcHeap* heap, size_t* first, size_t* last);
size_t child_count(ElmValue* v);
bool sanity_check(void* v);
size_t make_bitmask(size_t first_bit, size_t last_bit);
void bitmap_next(size_t* word, size_t* mask);


void* malloc_replay(ptrdiff_t bytes);

#endif
