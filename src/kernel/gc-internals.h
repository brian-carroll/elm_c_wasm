/*
  GC internal structures and definitions, not part of its API
  Only for including into tests, not other kernel code
*/

#ifndef ELM_KERNEL_GC_INTERNALS
#define ELM_KERNEL_GC_INTERNALS

#include "types.h"
#include <stdbool.h>


#define GC_WASM_PAGE_SIZE 65536
#define GC_BLOCK_WORDS (512/sizeof(size_t))
#define GC_WORD_BITS (sizeof(size_t)*8)


typedef struct {
    size_t* start;
    size_t* end; // end of usable heap area
    size_t** offsets;
    size_t* bitmap;
    size_t* system_end; // end of heap including GC reserved area (bitmap & offsets)
} GcHeap;


typedef struct {
    GcHeap heap;
    size_t* next_alloc;
    ElmValue* roots;
    Custom* stack_map;
    size_t stack_depth;
} GcState;


enum {
    GcStackEmpty,
    GcStackPush,
    GcStackPop,
    GcStackTailCall,
} GcStackCtor;


size_t child_count(ElmValue* v);
int set_heap_end(GcHeap* heap, size_t* new_break_ptr);
size_t make_bitmask(size_t first_bit, size_t last_bit);
bool mark_words(GcHeap* heap, void* p_void, size_t size);
void mark_trace(GcHeap* heap, ElmValue* v, size_t* ignore_below);
void mark_stack_map(GcState* state, size_t* ignore_below);
void mark(GcState* state, size_t* ignore_below);
void compact(GcState* state, size_t* compact_start);

void collect(GcState*);

#endif
