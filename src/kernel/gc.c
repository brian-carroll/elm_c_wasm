#include <stdio.h>
#include <string.h>
#include "types.h"
#include "gc.h"

enum {
    GcStackEmpty,
    GcStackPush,
    GcStackPop,
    GcStackTailCall,
} GcStackCtor;

Custom stack_empty; // ends up *above* the heap! Agh! How do I make sure heap is above constants?
GcState state;

GcState* GC_init() {
    stack_empty = (Custom){
        .header = HEADER_CUSTOM(0),
        .ctor = GcStackEmpty
    };
    u32* heap_data = state.pages[0].data;
    memset(heap_data, 0, GC_PAGE_BYTES);
    state.system_max_heap = &heap_data[GC_PAGE_BYTES/4];
    state.max_heap = state.system_max_heap;
    state.current_heap = &heap_data[0];
    state.roots = &Nil;
    state.stack_map = &stack_empty;
    state.stack_depth = 0;
    return &state;
}

static void* collect() {
    printf("collect: not implemented yet\n");
    return state.current_heap;
}


void* GC_allocate(size_t size) {
    u32 ints = (size+3)/4;
    u32* old_heap = state.current_heap;
    u32* new_heap = state.current_heap + ints;

    if (new_heap < (u32*)state.max_heap) {
        state.current_heap = new_heap;
        return old_heap;
    } else {
        return collect();
    }
}


static void mark_trace(ElmValue* v, ElmValue* ignore_below) {
    void** children; // array of pointers to child objects
    u32 n_children = 0;

    v->header.gc_mark = GcLive;

    switch (v->header.tag) {
        case Tag_Int:
        case Tag_Float:
        case Tag_Char:
        case Tag_String:
        case Tag_Nil:
            break;

        case Tag_Cons:
            children = &v->cons.head;
            n_children = 2;
            break;

        case Tag_Tuple2:
            children = &v->tuple2.a;
            n_children = 2;
            break;

        case Tag_Tuple3:
            children = &v->tuple3.a;
            n_children = 3;
            break;

        case Tag_Custom:
            children = &v->custom.values[0];
            n_children = custom_params(&v->custom);
            break;

        case Tag_Record:
            children = &v->record.values[0];
            n_children = v->record.fieldset->size;
            break;

        case Tag_Closure:
            children = &v->closure.values[0];
            n_children = v->closure.n_values;
            break;

        case Tag_GcFull:
            children = (void**)&v->gc_full.continuation;
            n_children = (v->gc_full.continuation != NULL) ? 1 : 0;
            break;
    }

    for (u32 i=0; i<n_children; ++i) {
        ElmValue* child = children[i];
        if (child->header.gc_mark == GcLive) continue;
        if (child < ignore_below) continue;
        mark_trace(child, ignore_below);
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
const u32 head = 0;
const u32 tail = 1;

void* GC_stack_push(Closure* c) {
    Header h = HEADER_CUSTOM(2);
    Custom* p = GC_allocate(sizeof(Custom) + 2*sizeof(void*));
    p->header = h;
    p->ctor = GcStackPush;
    p->values[head] = c;
    p->values[tail] = state.stack_map;

    state.stack_map = p;
    state.stack_depth++;
    return p;
}

void GC_stack_tailcall(Closure* c, void* push) {
    Custom* p = GC_allocate(sizeof(Custom) + 2*sizeof(void*));
    p->header = HEADER_CUSTOM(2);
    p->ctor = GcStackTailCall;
    p->values[head] = c;
    p->values[tail] = push;

    state.stack_map = p;
    // stack_depth stays the same
}

void GC_stack_pop(ElmValue* result, void* push) {
    Custom* p = GC_allocate(sizeof(Custom) + 2*sizeof(void*));
    p->header = HEADER_CUSTOM(2);
    p->ctor = GcStackPop;
    p->values[head] = result;
    p->values[tail] = push;

    state.stack_map = p;
    state.stack_depth--;
}


static void mark_linear(void* from, void* to) {
    Header* h_from = (Header*)from;
    Header* h_to = (Header*)to;
    for (Header* h = h_from ; h < h_to ; h += h->size) {
        h->gc_mark = GcLive;
    }
}

void mark_stack_map(ElmValue* ignore_below) {
    // Types are important for pointer arithmetic, be careful
    Custom* stack_item = state.stack_map;
    Custom* prev_item = (Custom*)state.current_heap;

    i32 min_depth = state.stack_depth;
    i32 current_depth = state.stack_depth;
    i32 new_depth = state.stack_depth;

    // Mark all values allocated by running functions, which we need to replay & resume
    // These values may be needed in local variables of those functions
    while (1) {
        switch (stack_item->ctor) {
            case GcStackPush:
                printf("GcStackPush @ %llx\n", (u64)stack_item);
                // going backwards => entering a section that's shallower than before
                new_depth = current_depth - 1;
                if (current_depth < min_depth) {
                    min_depth = current_depth;
                    mark_trace(stack_item->values[head], ignore_below);
                    mark_linear(stack_item + stack_item->header.size, prev_item);
                }
                break;

            case GcStackPop:
                printf("GcStackPop @ %llx\n", (u64)stack_item);
                // going backwards => entering a section that's deeper than before
                new_depth = current_depth + 1;
            case GcStackTailCall:
                if (stack_item->ctor == GcStackTailCall) printf("GcStackTailCall @ %llx\n", (u64)stack_item);
                if (current_depth == min_depth) {
                    printf("Entering mark_trace\n");
                    mark_trace(stack_item->values[head], ignore_below); // returned value from deeper function to active function
                    printf("Entering mark_linear\n");
                    mark_linear(stack_item + stack_item->header.size, prev_item);
                }
                break;

            case GcStackEmpty:
            default:
                return;
        }
        current_depth = new_depth;
        stack_item = stack_item->values[tail];
    }
}


// void* GC_next_replay() {

// }


/* Options
    - clear marks before GC (but this is when there's most garbage)
    - trace it after GC (less garbage)
    - iterate by address after GC (less garbage, better cache)
    - Mark dead *while* compacting (best)
*/
static void clear_marks(Header* h) {
    while (h < (Header*)state.current_heap) {
        h->gc_mark = GcDead;
        h += h->size;
    }
}

static void mark(void* ignore_below) {
    ElmValue* current_root = state.roots;
    while (current_root->header.tag == Tag_Cons) {
        mark_trace(current_root->cons.head, ignore_below);
        current_root = current_root->cons.tail;
    }
}
