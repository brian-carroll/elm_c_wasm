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
            the new locations.
        - On this second "replay" run, no registers can possibly hold pointers
            to the old locations.
        - Now, on "replay", every call our top-level function makes can be replaced
            with the return value we remember from the first time.
        - This means replay is fast, and doesn't regenerate the same garbage again.
            It executes very little code, produces minimal garbage, and runs fast.
        - This only works with pure functions, but in Elm that's most of the program.
        - The `apply` operator has some hooks into the GC. The GC puts some extra
            marker values into the heap to track pushes and pops of the call stack.


    References
        - "The Garbage Collection Handbook" by Jones, Hosking, and Moss (section 3.4)
        - Kermany and Petrank, 2006
        - Abuaiadh et al, 2004
*/

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include "types.h"
#include "gc.h"
#include "gc-internals.h"

GcState gc_state;

static void* stack_empty(); // pre-declaration


// Call exactly once on program startup
int GC_init() {

    // Get current max address of program data
    size_t* break_ptr = sbrk(0);

    // Align to next block boundary
    size_t break_aligned = (size_t)break_ptr;
    if (break_aligned % GC_BLOCK_BYTES) {
        break_aligned |= (GC_BLOCK_BYTES-1);
        break_aligned++;
    }
    size_t* heap_bottom = (size_t*)break_aligned;

    // Initialise state with zero heap size
    gc_state = (GcState){
        .heap = (GcHeap){
            .start = heap_bottom,
            .end = heap_bottom,
            .system_end = heap_bottom,
            .bitmap = heap_bottom,
            .offsets = heap_bottom,
        },
        .next_alloc = heap_bottom,
        .roots = &Nil,
        .stack_map = NULL,
        .stack_depth = 0,
    };

    // Ask the system for more memory
    size_t top_of_current_page =
        (size_t)heap_bottom | (size_t)(GC_WASM_PAGE_BYTES-1);

    size_t* top_of_next_page =
        (size_t*)(top_of_current_page + GC_WASM_PAGE_BYTES + 1);

    int err = set_heap_end(&gc_state.heap, top_of_next_page);

    if (!err) {
        stack_empty();
    }

    return err;
}

/*
  Register a mutable pointer as a GC root

  To be called by Kernel modules once on program startup
  Double-reference means Kernel modules never need to re-register.

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
void GC_register_root(ElmValue** ptr_to_mutable_ptr) {
    gc_state.roots = (ElmValue*)newCons(
        ptr_to_mutable_ptr,
        gc_state.roots
    );
}


/*
  Allocate memory on the heap
  Same interface as malloc in stdlib.h
*/
void* GC_malloc(size_t bytes) {
    size_t words = bytes / sizeof(size_t);
    size_t* old_heap = gc_state.next_alloc;
    size_t* new_heap = gc_state.next_alloc + words;

    if (new_heap < gc_state.heap.end) {
        gc_state.next_alloc = new_heap;
        return old_heap;
    } else {
        // TODO: maybe grow instead?
        // Use 'size' to make sure we get enough
        collect(&gc_state);
        return gc_state.next_alloc;
    }
}



/*          STACK MAP

    The Stack Map keeps track of which heap objects are currently
    pointed to from the stack or registers.

    As we execute the program, functions grab pointers to heap objects
    and manipulate them in local variables. But when we interrupt to
    do GC, we move objects around, making those local pointers corrupt.
    We need a way to fix this up after GC.

    Local heap pointers must either be:
        - on the C stack (in the top part of Wasm linear memory)
        - or in registers (represented as the 'stack machine' in Wasm)

    People usually scan both, but Wasm's registers are completely
    abstracted away and un-scannable!

    So we do this:
        - When out of memory, abandon all functions (sort of like an exception)
        - Keep all heap objects that were allocated by functions we interrupted
        - Keep any return values from functions that have finished running
        - Do a collection
        - Rerun the top-level function (e.g. `update` with same model and message)
        - **BUT** replace function calls with the values they returned last time
        - For all allocations, return same object as last time (at its new address)

    This fast-forwards back to where we interrupted execution, with all local
    variables referencing only the new moved pointers.

    The functions below are called from the `apply` operator
*/



// The stack map works like a list, but with 3 variants of Cons
// Use list terminology to make code more readable.
const size_t head = 1;
const size_t tail = 0; // StackPush only has a tail, no head

static void* stack_empty() {
    GcStackMap* p = GC_malloc(sizeof(GcStackMap));
    p->header = HEADER_GC_STACK_EMPTY;
    gc_state.stack_map = p;
    return p;
}

void* GC_stack_push() {
    GcStackMap* p = GC_malloc(sizeof(GcStackMap));
    p->header = HEADER_GC_STACK_PUSH;
    p->older = gc_state.stack_map;

    gc_state.stack_map = p;
    gc_state.stack_depth++;
    return p;
}

void GC_stack_tailcall(Closure* c, void* push) {
    GcStackMap* p = GC_malloc(sizeof(GcStackMap));
    p->header = HEADER_GC_STACK_TC;
    p->older = push;
    p->data = c;

    gc_state.stack_map = p;
    // stack_depth stays the same
}

void GC_stack_pop(ElmValue* result, void* push) {
    GcStackMap* p = GC_malloc(sizeof(GcStackMap));
    p->header = HEADER_GC_STACK_POP;
    p->older = push;
    p->data = result;

    gc_state.stack_map = p;
    gc_state.stack_depth--;
}

void* GC_next_replay() {
    /*
        if not replay mode, return NULL

        replay pointer should be pointing at a stack push
        find the previous stack item
            StackPop -> return its head
            StackTailcall -> return it and have 'apply' execute it
            StackPush -> NULL (resume actual execution)


    */
    return NULL;
}

/*
// Check compiled size of GC in Wasm
// Only 4kB!!! :)
// I don't have replay or controller yet, but wow
// And this includes some fixed overhead that emcc generates
//
int main(int argc, char** argv) {

    ElmValue** root = (ElmValue**)argv[0];
    size_t word = (size_t)argc;
    Closure* c = (Closure*)argv[1];
    ElmValue* v = (ElmValue*)argv[2];

    GC_init();
    GC_register_root(root);
    GC_malloc(word);
    void* push = GC_stack_push();
    GC_stack_tailcall(c, push);
    GC_stack_pop(v, push);

    GcHeap* heap = (GcHeap*)argv[3];
    size_t* pword = (size_t*)argv[4];
    GcState* state = (GcState*)argv[5];

    set_heap_end(heap, pword);
    mark(state, pword);
    compact(state, pword);
}
*/
