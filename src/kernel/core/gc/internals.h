/*
  GC internal structures and definitions, not part of public API
*/

#ifndef ELM_KERNEL_GC_INTERNALS
#define ELM_KERNEL_GC_INTERNALS

#include <stdbool.h>
#include <assert.h>
#include <errno.h>

#include "../core.h"

#define GC_INITIAL_HEAP_MB 8
#define GC_WASM_PAGE_BYTES 65536
#define GC_BLOCK_WORDS 64
#define GC_BLOCK_BYTES (GC_BLOCK_WORDS * sizeof(size_t))
#define GC_BLOCK_MASK (-GC_BLOCK_BYTES)
#define GC_WORD_BITS (sizeof(size_t) * 8)
#define GC_DIV_ROUND_UP(num, den) ((num + den - 1) / den)
#define GC_STACK_MAP_SIZE (10 * 1024)

#define ALL_ONES -1

typedef struct {
  size_t* start;
  size_t* end;  // end of usable heap area
  size_t* offsets;
  size_t* bitmap;
  size_t* system_end;  // end of heap including GC reserved area (bitmap & offsets)
} GcHeap;

typedef struct {
  GcStackMapIndex frame;
  GcStackMapIndex index;
  GcStackMapIndex replay_until;  
} GcStackMap;

typedef struct {
  GcHeap heap;
  size_t* next_alloc;
  size_t* nursery;
  Cons* roots;
  GcStackMap stack_map;
} GcState;

extern GcState gc_state;
extern void* stack_values[GC_STACK_MAP_SIZE];
extern char stack_flags[GC_STACK_MAP_SIZE];

void mark(GcState* state, size_t* ignore_below);
void compact(GcState* state, size_t* compact_start);

void reset_state(GcState* state);
int init_heap(GcHeap* heap);

void stack_clear();
void stack_enter(Closure* c);
void stack_prepare_for_replay();

void bitmap_reset(GcHeap*);
void bitmap_next(size_t* word, size_t* mask);
size_t bitmap_dead_between(GcHeap* heap, size_t* first, size_t* last);
size_t make_bitmask(size_t first_bit, size_t last_bit);

size_t child_count(ElmValue* v);
bool sanity_check(void* v);

void* malloc_replay(ptrdiff_t bytes);

#endif
