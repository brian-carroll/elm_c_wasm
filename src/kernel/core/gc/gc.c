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

#include "../core.h"  // debug
#include "allocate.c"
#include "bitmap.c"
#include "compact.c"
#include "header.c"
#include "heap.c"
#include "internals.h"
#include "mark.c"
#include "stack.c"

#ifdef TEST

void (*gc_test_mark_callback)();
#define TEST_MARK_CALLBACK() \
  if (gc_test_mark_callback) gc_test_mark_callback()

#else
#define TEST_MARK_CALLBACK()
#endif

GcState gc_state;

/* ====================================================

                PROGRAM STARTUP

   ==================================================== */

extern Queue Scheduler_queue;
Cons coreRoots = {
    .header = HEADER_LIST,
    .head = &Scheduler_queue.front,
    .tail = &Nil,
};

void reset_state(GcState* state) {
  void* start = state->heap.start;
  state->next_alloc = start;
  state->end_of_alloc_patch = state->heap.end;
  state->end_of_old_gen = start;
  state->n_marked_words = 0;
  state->roots = &coreRoots;
  stack_reset();
}

#if PERF_TIMER_ENABLED
void perf_get_baseline() {
  for (int i = 0; i < 3000000; i++) {
  }
}
#endif

// Call exactly once on program startup
int GC_init() {
  GcState* state = &gc_state;  // local reference for debugger to see
  int err = init_heap(&state->heap);
  reset_state(state);

#if PERF_TIMER_ENABLED
  PERF_TIMED_STATEMENT(perf_get_baseline());
#endif

  return err;
}


/* ====================================================

        GC ROOTS

==================================================== */

/*
  Register a GC root. We use the same code for two kinds of root:
  - Global vars that require Elm code to run to initialise them
    - Could be registered at compile time, but we do it at run time
  - Mutable state in Kernel code that points to heap values
    - Easier to do at run time. Hard for the compiler to know about.
    - We're not actually using this yet since most kernel code is in JS
    - Using a list instead of a C array supports this case.
*/
void GC_register_root(void** ptr_to_mutable_ptr) {
  GcState* state = &gc_state;
  state->roots = newCons(ptr_to_mutable_ptr, state->roots);
  GC_stack_pop_value();
}


/* ====================================================

                SWEEP

   ==================================================== */

#if GC_DO_SWEEP

void sweep_space(size_t* start_of_space, size_t* end_of_space) {
  size_t* w = start_of_space;
  if (!TARGET_64BIT && ((size_t)w & 7) && (w < end_of_space)) {
    *w++ = 0;
  }
  u64* p = (u64*)w;
  for (; p < (u64*)(end_of_space - 1); p++) {
    *p = 0;
  }
  for (w = (size_t*)p; w < end_of_space; w++) {
    *w = 0;
  }
}

// Sweep up the garbage by writing zeros to all the unused spaces in the heap
// This is not actually necessary, it's just nice for debug
void sweep(GcHeap* heap, size_t* start) {
  size_t* end_of_space = start;
  size_t* start_of_space = bitmap_find_space(heap, end_of_space, 1, &end_of_space);
  for (;;) {
    size_t* start_of_space = bitmap_find_space(heap, end_of_space, 1, &end_of_space);
    if (!start_of_space) break;
    sweep_space(start_of_space, end_of_space);
  }
}

#endif

/* ====================================================

                COLLECT

   ==================================================== */

/**
 * Minor collection
 * Can be used during execution. Does not move any live pointers.
 */
void GC_collect_minor() {
  GcState* state = &gc_state;
  size_t* ignore_below = state->end_of_old_gen;

  // safe_printf("\nStarting minor GC from %p\n", ignore_below);

  PERF_TIMED_STATEMENT(mark(state, ignore_below));
  TEST_MARK_CALLBACK();

#if GC_DO_SWEEP
  PERF_TIMED_STATEMENT(sweep(&state->heap, ignore_below));
#endif

#if 0
    size_t new_gen_size = state->heap.end - ignore_below;
    size_t used = state->n_marked_words;
    f32 percent_marked = (100.0 * used) / (f32)new_gen_size;
    char marked[20];
    char available[20];
    format_mem_size(marked, sizeof(marked), used);
    format_mem_size(available, sizeof(available), new_gen_size);
    safe_printf("Minor GC marked %f%% (%s / %s)\n", percent_marked, marked, available);
#endif

  PERF_TIMED_STATEMENT(sweepJsRefs(false));

#if PERF_TIMER_ENABLED
  perf_print();
#endif
}


/**
 * Major collection
 * Not to be used during execution. Moves pointers by compaction.
 */
void GC_collect_major() {
  GcState* state = &gc_state;
  size_t* ignore_below = state->heap.start;

#ifdef DEBUG
  if (state->stack_map.index) {
    print_stack_map();
    ASSERT_EQUAL(state->stack_map.index, 0);
  }
#endif
  PERF_TIMED_STATEMENT(mark(state, ignore_below));
  TEST_MARK_CALLBACK();

  PERF_TIMED_STATEMENT(compact(state, ignore_below));

#if GC_DO_SWEEP
  PERF_TIMED_STATEMENT(sweep_space(state->end_of_old_gen, state->heap.end));
#endif

  PERF_TIMED_STATEMENT(bitmap_reset(&state->heap));

  PERF_TIMED_STATEMENT(sweepJsRefs(true));

  size_t used = state->next_alloc - state->heap.start;
  size_t available = state->heap.end - state->heap.start;
  // safe_printf("Major GC: %zd kB used, %zd kb available\n",
  // used * SIZE_UNIT / 1024,
  // available * SIZE_UNIT / 1024);

  if (used * 2 > available) {
    PERF_TIMED_STATEMENT(grow_heap(&state->heap, 0));
  }

#if PERF_TIMER_ENABLED
  perf_print();
#endif
}


/* ====================================================

                PROGRAM ENTRY POINT

    Execute a function in the context of the GC

   ==================================================== */

void* GC_execute(Closure* c) {
  // TODO: remove this redundant stack frame! JS wrapper has already created one.
  GcStackMapIndex frame = GC_stack_push_frame('C', c->evaluator);
  GC_stack_push_value(c);

  void* result = Utils_apply(c, 0, NULL);

  GC_stack_pop_frame(c->evaluator, result, frame);

#if 0
  if (is_major_gc_needed()) {
    GC_collect_major();
    result = forwarding_address(&gc_state.heap, result);
  }
#endif
  return result;
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
#ifdef DEBUG
  GcState* state = &gc_state;
  if (state->stack_map.index) {
    print_stack_map();
    ASSERT(state->stack_map.index, 0);
  }
#endif

  GC_register_root(global_permanent_ptr);

  GcStackMapIndex frame = GC_stack_push_frame('I', init_func);
  *global_permanent_ptr = init_func();
  GC_stack_pop_frame(init_func, NULL, frame);
  GC_stack_pop_value();
}
