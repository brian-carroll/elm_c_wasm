#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include "types.h"
#include "gc.h"
#include "gc-internals.h"

GcState gc_state;

void* stack_empty();


int GC_init() {
    // Get current max address of program data
    void* break_ptr = sbrk(0);

    // Align to next 32 or 64-bit boundary
    size_t break_aligned = ((size_t)break_ptr | (sizeof(size_t) -1)) + 1;
    size_t* heap_bottom = (size_t*)break_aligned;

    // Initialise state with zero heap size
    gc_state = (GcState){
        .heap = (GcHeap){
            .start = heap_bottom,
            .end = heap_bottom,
            .system_end = heap_bottom,
            .bitmap = heap_bottom,
            .offsets = (size_t**)heap_bottom,
        },
        .next_alloc = heap_bottom,
        .roots = &Nil,
        .stack_map = NULL,
        .stack_depth = 0,
    };

    // Ask the system for more memory
    size_t top_of_current_page =
        (size_t)heap_bottom | (size_t)(GC_WASM_PAGE_SIZE-1);

    size_t* top_of_next_page =
        (size_t*)(top_of_current_page + GC_WASM_PAGE_SIZE);

    int err = set_heap_end(&gc_state.heap, top_of_next_page);

    if (!err) {
        gc_state.stack_map = stack_empty();
    }

    return err;
}


// Kernel modules can use this to register a GC root
void GC_register_root(ElmValue** root_mutable_pointer) {
    gc_state.roots = (ElmValue*)newCons(
        root_mutable_pointer,
        gc_state.roots
    );
}


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

    The StackMap is an untyped structure but it would look something like this:

    type StackMap
        = StackPush StackMap
        | StackPop ElmValue StackMap
        | StackTailCall Closure StackMap
        | StackEmpty
*/



// The stack map works like a list, but with 3 variants of Cons
// Use list terminology to make code more readable.
const size_t head = 1;
const size_t tail = 0; // StackPush only has a tail, no head

void* stack_empty() {
    Custom* p = GC_malloc(sizeof(Custom));
    p->header = HEADER_CUSTOM(0);
    p->ctor = GcStackEmpty;

    gc_state.stack_map = p;
    return p;
}

void* GC_stack_push() {
    Custom* p = GC_malloc(sizeof(Custom) + sizeof(void*));
    p->header = HEADER_CUSTOM(1);
    p->ctor = GcStackPush;
    p->values[tail] = gc_state.stack_map;

    gc_state.stack_map = p;
    gc_state.stack_depth++;
    return p;
}

void GC_stack_tailcall(Closure* c, void* push) {
    Custom* p = GC_malloc(sizeof(Custom) + 2*sizeof(void*));
    p->header = HEADER_CUSTOM(2);
    p->ctor = GcStackTailCall;
    p->values[tail] = push;
    p->values[head] = c;

    gc_state.stack_map = p;
    // stack_depth stays the same
}

void GC_stack_pop(ElmValue* result, void* push) {
    Custom* p = GC_malloc(sizeof(Custom) + 2*sizeof(void*));
    p->header = HEADER_CUSTOM(2);
    p->ctor = GcStackPop;
    p->values[tail] = push;
    p->values[head] = result;

    gc_state.stack_map = p;
    gc_state.stack_depth--;
}
