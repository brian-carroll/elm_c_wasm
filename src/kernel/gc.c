#include <stdio.h>
#include <stdbool.h>
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
    memset(heap_data, 0, GC_PAGE_SLOTS*4);
    memset(state.pages[0].bitmap, 0, sizeof(state.pages[0].bitmap));
    state.system_max_heap = &heap_data[GC_PAGE_SLOTS-1];
    state.max_heap = state.system_max_heap;
    state.current_heap = &heap_data[0];
    state.roots = &Nil;
    state.stack_map = &stack_empty;
    state.stack_depth = 0;
    return &state;
}


void* next_heap_object(void* p) {
    Header* h = (Header*)p;
    return (void*)(h + h->size);
}

const u64 ALL_ONES = -1;

u64 make_bitmask(u32 first_bit, u32 last_bit) {
    u64 mask = ALL_ONES;
    mask <<= 63 - last_bit;
    mask >>= 63 - (last_bit - first_bit);
    mask <<= first_bit;
    return mask;
}


bool mark_value(void* p) {
    u32* p32 = (u32*)p;
    u32 first_slot = p32 - state.pages[0].data;

    if (first_slot >= GC_PAGE_SLOTS) return false; 
    u32 first_word = first_slot / GC_BITMAP_WORDSIZE;
    u32 first_bit = first_slot % GC_BITMAP_WORDSIZE;

    Header* h = (Header*) p;
    u32 last_slot = first_slot + h->size - 1;
    u32 last_word = last_slot / GC_BITMAP_WORDSIZE;
    u32 last_bit = last_slot % GC_BITMAP_WORDSIZE;

    u64* bitmap = state.pages[0].bitmap;
    bool already_marked;
    if (first_word == last_word) {
        u64 bitmask = make_bitmask(first_bit, last_bit);
        already_marked = !!(bitmap[first_word] & bitmask);
        bitmap[first_word] |= bitmask;
    } else {
        u64 first_mask = make_bitmask(first_bit, 63);
        already_marked = !!(bitmap[first_word] & first_mask);
        bitmap[first_word] |= first_mask;
        bitmap[last_word] |= make_bitmask(0, last_bit);
        for (u32 word = first_word+1; word<last_word; ++word) {
            bitmap[word] = ALL_ONES;
        }
    }
    return already_marked;
}


bool is_marked(void* p) {
    u32* p32 = (u32*)p;
    u32 slot = p32 - state.pages[0].data;
    if (slot >= GC_PAGE_SLOTS) return true; // off heap => not garbage, stop tracing

    u32 word = slot / GC_BITMAP_WORDSIZE;
    u32 bit = slot % GC_BITMAP_WORDSIZE;
    u64 mask = (u64)1 << bit;

    return (state.pages[0].bitmap[word] & mask) != 0;
}


u32 size_marked(void* p) {
    u32* p32 = (u32*)p;
    u32 slot = p32 - state.pages[0].data;
    if (slot >= GC_PAGE_SLOTS) return 0;

    u32 word = slot / GC_BITMAP_WORDSIZE;
    u32 bit = slot % GC_BITMAP_WORDSIZE;
    u64 mask = (u64)1 << bit;
    u32 size = 0;

    while (word < GC_PAGE_SLOTS/GC_BITMAP_WORDSIZE) {
        while (mask) {
            if ((state.pages[0].bitmap[word] & mask) == 0) {
                return size;
            }
            size++;
            mask <<= 1;
        }
        word++;
        mask = 1;
    }
    return size;
}



void* find_children(ElmValue* v, u32* n_children) {
    void** child_ptr_array = NULL;
    switch (v->header.tag) {
        case Tag_Int:
        case Tag_Float:
        case Tag_Char:
        case Tag_String:
        case Tag_Nil:
            child_ptr_array = NULL;
            *n_children = 0;
            break;

        case Tag_Cons:
            child_ptr_array = &v->cons.head;
            *n_children = 2;
            break;

        case Tag_Tuple2:
            child_ptr_array = &v->tuple2.a;
            *n_children = 2;
            break;

        case Tag_Tuple3:
            child_ptr_array = &v->tuple3.a;
            *n_children = 3;
            break;

        case Tag_Custom:
            child_ptr_array = &v->custom.values[0];
            *n_children = custom_params(&v->custom);
            break;

        case Tag_Record:
            child_ptr_array = &v->record.values[0];
            *n_children = v->record.fieldset->size;
            break;

        case Tag_Closure:
            child_ptr_array = &v->closure.values[0];
            *n_children = v->closure.n_values;
            break;

        case Tag_GcFull:
            child_ptr_array = (void**)&v->gc_full.continuation;
            *n_children = (v->gc_full.continuation != NULL) ? 1 : 0;
            break;
    }
    return child_ptr_array;
}


void* forwarding_address(void* old_address) {
    return NULL;
}


static void copy_value(u32* from, u32* to) {
    void** child_ptr_array;
    u32 n_children;
    ElmValue* v = (ElmValue*)from;
    child_ptr_array = find_children(v, &n_children);

    if (!child_ptr_array || !n_children) {
        // Int, Float, Char, String
        for (u32 i=0; i < v->header.size; ++i) {
            *to++ = *from++;
        }
    } else {
        // container types
        while ((void**)from < child_ptr_array) {
            *to++ = *from++;
        }
        void** child_to = (void**)to;
        void** child_from = (void**)from;
        for (u32 i=0; i < n_children; ++i) {
            *child_to++ = forwarding_address(*child_from++);
        }
    }
}


static u32* compact() {
    u32* current = state.pages[0].data;

    // skip initial live data, find first garbage patch
    while (is_marked(current)) {
        current = next_heap_object(current);
    }
    u32* to = current;

    while (current < state.current_heap) {
        while (!is_marked(current)) {
            current = next_heap_object(current);
        }

        u32* live_start = current;
        u32* next_garbage = live_start + size_marked(live_start);

        for (u32* from=live_start; from<next_garbage; ++from) {
            copy_value(from, to);
            to++;
        }

        current = to;
    }

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
        return compact();
    }
}


static void mark_trace(ElmValue* v, ElmValue* ignore_below) {
    void** child_ptr_array;
    u32 n_children;

    if (v < ignore_below) return;

    bool already_marked = mark_value(v);
    if (already_marked) return;

    child_ptr_array = find_children(v, &n_children);

    if (child_ptr_array == NULL) return;
    for (u32 i=0; i < n_children; ++i) {
        ElmValue* child = child_ptr_array[i];
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
const u32 head = 1;
const u32 tail = 0; // StackPush only has a tail, no head

void* GC_stack_push() {
    Header h = HEADER_CUSTOM(1);
    Custom* p = GC_allocate(sizeof(Custom) + sizeof(void*));
    p->header = h;
    p->ctor = GcStackPush;
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
        mark_value(h);
    }
}

static void mark_stack_map(ElmValue* ignore_below) {
    // Types are important for pointer arithmetic, be careful
    Custom* stack_item = state.stack_map;
    Custom* prev_stack_item = (Custom*)state.current_heap;

    i32 min_depth = state.stack_depth;
    i32 current_depth = state.stack_depth;
    i32 new_depth = state.stack_depth;

    // Mark all values allocated by running functions, which we need to replay & resume
    // These values may be needed in local variables of those functions
    while (1) {
        mark_value(stack_item);
        switch (stack_item->ctor) {
            case GcStackPush:
                // going backwards => entering a section that's shallower than before
                new_depth = current_depth - 1;
                if (new_depth < min_depth) {
                    min_depth = new_depth;
                    if (prev_stack_item->ctor != GcStackTailCall) {
                        mark_linear(next_heap_object(stack_item), prev_stack_item);
                    }
                }
                break;

            case GcStackPop:
                // going backwards => entering a section that's deeper than before
                new_depth = current_depth + 1;
            case GcStackTailCall:
                if (current_depth == min_depth) {
                    mark_trace(stack_item->values[head], ignore_below); // returned value from deeper function to active function
                    mark_linear(next_heap_object(stack_item), prev_stack_item);
                }
                break;

            case GcStackEmpty:
            default:
                return;
        }
        current_depth = new_depth;
        prev_stack_item = stack_item;
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
// static void clear_marks(Header* h) {
//     while (h < (Header*)state.current_heap) {
//         h->gc_mark = GcDead;
//         h += h->size;
//     }
// }


void GC_register_root(ElmValue** root_mutable_pointer) {
    state.roots = (ElmValue*)newCons(root_mutable_pointer, state.roots);
}


void mark(ElmValue* ignore_below) {
    for (ElmValue* root_cell=state.roots; root_cell->header.tag==Tag_Cons; root_cell=root_cell->cons.tail) {
        mark_value(root_cell);

        // Each GC root is a mutable pointer in a fixed location outside the dynamic heap,
        // pointing to a value on the dynamic heap that should be preserved.
        // e.g. After `update`, the GC root pointer for `model` will be switched from the old to the new value.
        // The double pointer is the off-heap fixed address where we store the address of the current value
        ElmValue** root_mutable_pointer = (ElmValue**)root_cell->cons.head;
        ElmValue* live_heap_value = *root_mutable_pointer;
        mark_trace(live_heap_value, ignore_below);
    }
    mark_stack_map(ignore_below);
}
