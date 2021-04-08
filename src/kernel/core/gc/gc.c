/*
    Garbage collector for Elm
    -------------------------
    A mark-sweep collector with compaction based on the Compressor algorithm [1]

    Notable features:
    ================
    - Uses a bitmap of live/dead bits instead of marking the values themselves.
        => Many operations involve only a few localised memory accesses

    - Compression uses only a single pass over the heap.

    - Calculates 'forwarding addresses' of moved pointers on the fly
        - New location has the same number of live bytes below it as the old one did
        - Speeds up the calculation by storing a table of 'offsets' to the first
            live value in each 512-byte block of memory

    - Designed for both 32 and 64-bit platforms.
        Uses the size_t data type everywhere (32 or 64 bits as per platform)

    References
        [1] "The Garbage Collection Handbook" by Jones, Hosking, and Moss (section 3.4)
        [2] Kermany and Petrank, 2006
        [3] Abuaiadh et al, 2004
*/
#ifdef DEBUG
#include <stdio.h>
#endif
#include "internals.h"
#include "../core.h"

#include "allocate.c"
#include "bitmap.c"
#include "compact.c"
#include "header.c"
#include "mark.c"
#include "stack.c"
#include "state.c"

jmp_buf gcLongJumpBuf;

/* ====================================================

                PROGRAM STARTUP

   ==================================================== */

// Call exactly once on program startup
int GC_init() {
  GcState* state = &gc_state;  // local reference for debugger to see
  int err = init_heap(&state->heap);
  reset_state(state);
  return err;
}


/* ====================================================

        GC ROOTS

==================================================== */

/*
  Roots should be registered by Kernel modules once on program startup.

  Usage:
    ElmValue* my_mutable_heap_pointer;

    void My_init() {
        GC_register_root(&my_mutable_heap_pointer);
    }

  Once the mutable pointer is registered, the Kernel module
  can point it at any heap value it wants to keep alive.
  It can later update the pointer to point at a different
  ElmValue on the heap, as the program executes.
  Whenever the GC does a collection, it checks this pointer
  and preserves whatever heap value it is currently
  pointing to. If it moves the value, it will update the
  pointer to reference the new location.
*/
void GC_register_root(void** ptr_to_mutable_ptr) {
  GcState* state = &gc_state;
  state->roots = newCons(ptr_to_mutable_ptr, state->roots);
}


/* ====================================================

                COLLECT

   ==================================================== */

static size_t percent_full(GcState* state) {
  GcHeap* heap = &state->heap;
  size_t available_words = heap->end - heap->start;
  size_t used_words = state->next_alloc - heap->start;
  size_t percent = (100 * used_words) / available_words;
  return percent;
}


static void collect(GcState* state, size_t* ignore_below) {
  mark(state, ignore_below);
  compact(state, ignore_below);
  bool is_full_gc = ignore_below <= gc_state.heap.start;
  sweepJsRefs(is_full_gc);
  printf("After GC, heap is %zd%% full\n", percent_full(state));
}

void GC_collect_full() {
  collect(&gc_state, gc_state.heap.start);
}

void GC_collect_nursery() {
  collect(&gc_state, gc_state.nursery);
}


/* ====================================================

                PROGRAM ENTRY POINT
  
    Execute a function in the context of the GC

   ==================================================== */

void* GC_execute(Closure* c) {
  stack_clear();
  stack_enter(c);

  // long jump creates an implicit loop
  int out_of_memory = setjmp(gcLongJumpBuf);
  if (out_of_memory) {
    GC_collect_full();
  }
  return Utils_apply(stack_values[1], 0, NULL);
}


/* ====================================================

  Initialise a global value by evaluating an Elm expression

  global_permanent_ptr:  location of a pointer outside the heap
            that always points to the current heap location.
            GC will update this as it moves the value around.
  init_func:   a function that evaluates the Elm expression
            to initialise the global value
  ==================================================== */

void GC_init_root(void** global_permanent_ptr, void* (*init_func)()) {
  GC_register_root(global_permanent_ptr);

  stack_clear();
  stack_enter(NULL);

  // long jump creates an implicit loop
  int out_of_memory = setjmp(gcLongJumpBuf);
  if (out_of_memory) {
    GC_collect_full();
  }
  *global_permanent_ptr = init_func();
}
