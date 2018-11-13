#include <stdio.h>
#include "types.h"

void* system_max_heap; // The point at which we need new memory from the OS/browser
void* max_heap; // The point at which allocation fails (keeping some space for GC internal use)

Header* current_heap; // Next allocated address
ElmValue* roots;  // All "GC roots" in a linked list
Custom* stack_map; // Special "GC root". Track which pointers are on the call stack.
Custom stack_empty;
i32 stack_depth = 0;


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

enum {
    GcStackEmpty,
    GcStackPush,
    GcStackPop,
    GcStackTailCall,
} GcStackCtor;

// The stack map is like a list with 3 variants of Cons
// Define constants to make code more readable.
const u32 head = 0;
const u32 tail = 1;

void GC_init() {
    roots = &Nil;
    stack_empty = (Custom){
        .header = HEADER_CUSTOM(0),
        .ctor = GcStackEmpty
    };
    stack_map = &stack_empty;
}

void* GC_stack_push(Closure* c) {
    Custom* p = malloc(sizeof(Custom) + sizeof(void*));
    p->header = HEADER_CUSTOM(1);
    p->ctor = GcStackPush;
    p->values[head] = c;
    p->values[tail] = stack_map;

    stack_map = p;
    stack_depth++;
    return p;
}

void GC_stack_tailcall(Closure* c, void* push) {
    Custom* p = malloc(sizeof(Custom) + 2*sizeof(void*));
    p->header = HEADER_CUSTOM(2);
    p->ctor = GcStackTailCall;
    p->values[head] = c;
    p->values[tail] = push;

    stack_map = p;
    // stack_depth stays the same
}

void GC_stack_pop(ElmValue* result, void* push) {
    Custom* p = malloc(sizeof(Custom) + 2*sizeof(void*));
    p->header = HEADER_CUSTOM(2);
    p->ctor = GcStackPop;
    p->values[head] = result;
    p->values[tail] = push;

    stack_map = p;
    stack_depth--;
}


void mark_linear(Header* from, Header* to) {
    for (Header* h = from ; h <= to ; h += h->size) {
        h->gc_mark = GcLive;
    }
}

static void mark_stack_map(ElmValue* ignore_below) {
    // Types are important for pointer arithmetic, be careful
    Header* stack_item = (Header*)stack_map;
    Header* prev_item = current_heap;

    i32 min_depth = stack_depth;
    i32 current_depth = stack_depth;
    i32 new_depth;

    // Mark all values allocated by running functions, which we need to replay & resume
    // These values may be needed in local variables of those functions
    while (1) {
        switch (stack_item->ctor) {
            case GcStackPush:
                // going backwards => entering a section that's shallower than before
                new_depth = current_depth - 1;
                if (current_depth < min_depth) {
                    min_depth = current_depth;
                    mark_trace(stack_item->values[head], ignore_below);
                    mark_linear(stack_item + stack_item->size, prev_item - 1);
                }
                break;

            case GcStackPop:
                // going backwards => entering a section that's deeper than before
                new_depth = current_depth + 1;
            case GcStackTailCall:
                if (current_depth == min_depth) {
                    mark_trace(stack_item->values[head], ignore_below); // returned value from deeper function to active function
                    mark_linear(stack_item + stack_item->size, prev_item - 1);
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



static void mark_trace(ElmValue* v, ElmValue* ignore_below) {
    ElmValue* first = v;
    ElmValue* last = NULL;
    u32 nparams;

    v->header.gc_mark = GcLive;

    switch (v->header.tag) {
        case Tag_Int:
        case Tag_Float:
        case Tag_Char:
        case Tag_String:
        case Tag_Nil:
            break;

        case Tag_Cons:
            first = v->cons.head;
            last = v->cons.tail;
            break;

        case Tag_Tuple2:
            first = v->tuple2.a;
            last = v->tuple2.b;
            break;

        case Tag_Tuple3:
            first = v->tuple3.a;
            last = v->tuple3.c;
            break;

        case Tag_Custom:
            nparams = ((v->header.size * SIZE_UNIT) - sizeof(u32)) / sizeof(void*);
            if (nparams > 0) {
                first = v->custom.values[0];
                last = v->custom.values[nparams-1];
            }
            break;

        case Tag_Record:
            nparams = v->record.fieldset->size;
            if (nparams > 0) {
                first = v->record.values[0];
                last = v->record.values[nparams-1];
            }
            break;

        case Tag_Closure:
            nparams = v->closure.n_values;
            if (nparams > 0) {
                first = v->closure.values[0];
                last = v->closure.values[nparams-1];
            }
            break;

        case Tag_GcFull:
            if (v->gc_full.continuation != NULL) {
                first = (ElmValue*)v->gc_full.continuation;
                last = first;
            }
            break;
    }

    for (ElmValue* child = first; child <= last; child += sizeof(void*)) {
        if (child->header.gc_mark == GcLive) continue;
        if (child < ignore_below) continue;
        mark_trace(child, ignore_below);
    }
}

/* Options
    - clear marks before GC (but this is when there's most garbage)
    - trace it after GC (less garbage)
    - iterate by address after GC (less garbage, better cache)
    - Mark dead *while* compacting (best)
*/
static void clear_marks(Header* h) {
    while (h < current_heap) {
        h->gc_mark = GcDead;
        h += h->size * SIZE_UNIT;
    }
}

static void mark(void* ignore_below) {
    ElmValue* current_root = roots;
    while (current_root->header.tag == Tag_Cons) {
        mark_trace(current_root->cons.head, ignore_below);
        current_root = current_root->cons.tail;
    }
}
