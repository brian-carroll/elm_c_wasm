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

#include "../core.h"
#include "allocate.c"
#include "bitmap.c"
#include "compact.c"
#include "header.c"
#include "heap.c"
#include "internals.h"
#include "mark.c"
#include "stack.c"

#ifdef DEBUG

#include <stdio.h>
void (*gc_test_mark_callback)();
#define TEST_MARK_CALLBACK() \
  if (gc_test_mark_callback) gc_test_mark_callback()

#else
#define TEST_MARK_CALLBACK()
#endif

#if PERF_TIMER_ENABLED
struct gc_perf_data perf_data;
#endif


GcState gc_state;

/* ====================================================

                PROGRAM STARTUP

   ==================================================== */

void reset_state(GcState* state) {
  void* start = state->heap.start;
  state->end_of_old_gen = start;
  state->next_alloc = start;
  state->roots = &Nil;
  stack_clear();
}

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

#ifdef _WIN32
// #include <intrin.h>
// #define popcount(w) __popcnt64(w)
// TODO: figure out header files/options/whatever for MSVC
int popcount(u64 word) {
  u64 w = word;
  int count;
  for (count = 0; w; count++) {
    w &= w - 1;  // clear the least significant bit set
  }
  return count;
}
#else
#define popcount(w) __builtin_popcountll(w)
#endif

/**
 * Minor collection
 * Can be used during execution. Does not move any live pointers.
 */
void GC_collect_minor() {
  GcState* state = &gc_state;
  size_t* ignore_below = state->end_of_old_gen;

  PERF_START();

  mark(state, ignore_below);
  PERF_TIMER(marked);
  TEST_MARK_CALLBACK();

  i32 new_gen_size = state->heap.end - ignore_below;
  i32 used = (i32)state->n_marked_words;
  i32 percent_marked = (100 * used) / new_gen_size;
  printf("Minor GC marked %d%% (%zd kB / %zd kB)\n",
      percent_marked,
      used * SIZE_UNIT / 1024,
      new_gen_size * SIZE_UNIT / 1024);

  if (percent_marked > 75) {
    grow_heap_x2(&state->heap);
  }

  sweepJsRefs(false);
  PERF_TIMER(jsRefs);
  PERF_TIMER_PRINT();
}


/**
 * Major collection
 * Not to be used during execution. Moves pointers by compaction.
 */
void GC_collect_major() {
  GcState* state = &gc_state;
  size_t* ignore_below = state->heap.start;

  PERF_START();

  mark(state, ignore_below);
  PERF_TIMER(marked);
  TEST_MARK_CALLBACK();

  compact(state, ignore_below);
  PERF_TIMER(compacted);

  sweepJsRefs(true);
  PERF_TIMER(jsRefs);

  size_t used = state->next_alloc - state->heap.start;
  size_t available = state->heap.end - state->heap.start;
  printf("Major GC: %zd kB used, %zd kb available\n",
      used * SIZE_UNIT / 1024,
      available * SIZE_UNIT / 1024);

  if (used * 2 > available) {
    grow_heap_x2(&state->heap);
  }

  PERF_TIMER_PRINT();
}


/* ====================================================

                PROGRAM ENTRY POINT

    Execute a function in the context of the GC

   ==================================================== */

void* GC_execute(Closure* c) {
  stack_clear();
  stack_enter(c);
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

  *global_permanent_ptr = init_func();
}
