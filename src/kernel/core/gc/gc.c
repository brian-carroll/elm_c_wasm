/*
    Garbage collector for Elm
    -------------------------
    A mark-compact collector, based on the Compressor algorithm.

    Notable features:
    ================
    - Compression uses only a single pass over the heap

    - Uses a bitmap of live/dead bits instead of marking the values themselves.
        => Many operations involve only a few localised memory accesses

    - Calculates 'forwarding addresses' of moved pointers on the fly
        - New location has the same number of live bytes below it as the old one did
        - Speeds up the calculation by storing a table of 'offsets' to the first
            live value in each 512-byte block of memory

    - Designed for both 32 and 64-bit platforms.
        Uses the size_t data type everywhere (32 or 64 bits as per platform)

    - Takes advantage of immutable-only data
        - Can do partial compression, starting at any point and only looking
        at values _above_ it, ignoring everything below.
        - Enabled by the fact that immutable values can only reference older
        values (lower addresses).
        - Offers flexibility to do quick partial collections.

    - Takes advantage of pure functions
        - A novel stack map idea: "replay" for pure functions
        - Wasm does not allow access to registers (which it models as a stack machine)
        - Therefore we can't find or modify pointers stored in registers. So how can
            we move things around safely?
        - Elm functions are pure. So we can abort execution, move values around,
            then "replay" the same function call with the same values, but at
            the new locations, and resume execution where we left off.
        - On this second "replay" run, no registers can possibly hold pointers
            to the old locations.
        - Now, on "replay", every call our top-level function makes can be replaced
            with the return value we remember from the first time.
        - This means replay is fast, and doesn't regenerate the same garbage again.
            It quickly gets us back to the same execution state we were in before GC.
        - This only works with pure functions, but in Elm that's most of the program.
        - The `apply` operator has some hooks into the GC. The GC puts some extra
            marker values into the heap to track pushes and pops of the call stack.


    References
        - "The Garbage Collection Handbook" by Jones, Hosking, and Moss (section 3.4)
        - Kermany and Petrank, 2006
        - Abuaiadh et al, 2004
*/

#include "internals.h"

/* ====================================================

                PROGRAM STARTUP

   ==================================================== */

// Call exactly once on program startup
int GC_init() {
  GcState* state = &gc_state;  // local reference for debugger to see

  int err = init_heap(&state->heap);

  reset_state(state);
  if (!err) {
    GC_stack_empty();
  }

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
void* GC_register_root(void** ptr_to_mutable_ptr) {
  GcState* state = &gc_state;
  state->roots = NEW_CONS(ptr_to_mutable_ptr, state->roots);
  return ptr_to_mutable_ptr;  // return anything but pGcFull to show success
}


/* ====================================================

                COLLECT

   ==================================================== */

static void collect(GcState* state, size_t* ignore_below) {
#ifdef DEBUG
  printf("collecting garbage from %p\n", ignore_below);
#endif
  mark(state, ignore_below);
  compact(state, ignore_below);
  bool is_full_gc = ignore_below <= gc_state.heap.start;
  sweepJsRefs(is_full_gc);

  bool stack_empty_was_deleted = (size_t*)state->stack_map_empty >= state->next_alloc;
  if (stack_empty_was_deleted) {
    GC_stack_empty();
  }
}

void GC_collect_full() {
  collect(&gc_state, gc_state.heap.start);
}

void GC_collect_nursery() {
  collect(&gc_state, gc_state.nursery);
}
