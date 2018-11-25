#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include "./types.h"
#include "./gc-internals.h"


static const size_t ALL_ONES = -1; // 0xfffff...


/* ====================================================

                ELM VALUES

   ==================================================== */

// Find children of an Elm value
// returns a pointer to first child, and writes to *n_children.
// Would be nicer to treat both outputs the same way, but
// that would make child_ptr_array a triple pointer. Ugh. Just no.
void* find_children(ElmValue* v, size_t* n_children) {
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

/* ====================================================

            BITMAP & LOW-LEVEL FUNCTIONS

   ==================================================== */

int set_heap_end(GcHeap* heap, size_t* new_break_ptr) {

    int has_error = brk(new_break_ptr);
    if (has_error) {
        fprintf(stderr, "Failed to get heap memory. Error code %d\n", errno);
        return errno;
    }

    size_t heap_words = new_break_ptr - heap->start;

    size_t bitmap_words_per_block = GC_BLOCK_WORDS / GC_WORD_BITS;
    size_t offset_words_per_block = 1;
    size_t block_plus_overhead_words =
        GC_BLOCK_WORDS
        + bitmap_words_per_block
        + offset_words_per_block;

    size_t heap_blocks = heap_words / block_plus_overhead_words;

    size_t bitmap_words = heap_blocks * bitmap_words_per_block;
    size_t offsets_words = heap_blocks * offset_words_per_block;

    size_t* bitmap = new_break_ptr - bitmap_words;
    size_t* offsets = bitmap - offsets_words;

    heap->end = offsets;
    heap->system_end = new_break_ptr;
    heap->bitmap = bitmap;
    heap->offsets = (size_t**)offsets;

    return 0;
}


size_t make_bitmask(size_t first_bit, size_t last_bit) {
    size_t mask = ALL_ONES;
    mask <<= GC_WORD_BITS -1 - last_bit;
    mask >>= GC_WORD_BITS -1 - (last_bit - first_bit);
    mask <<= first_bit;
    return mask;
}


/* ====================================================

                MARK

   ==================================================== */


// Mark a value as live and return 'true' if previously marked
bool mark_words(GcHeap* heap, void* p_void, size_t size) {
    size_t* p = (size_t*)p_void;

    // If value is outside the heap (constant), then consider it
    // "already marked" (=> children won't be traced)
    if (p < heap->start) return true;

    size_t first_index = (size_t)(p - heap->start);
    size_t first_word = first_index / GC_WORD_BITS;
    size_t first_bit = first_index % GC_WORD_BITS;

    size_t last_index = first_index + size - 1;
    size_t last_word = last_index / GC_WORD_BITS;
    size_t last_bit = last_index % GC_WORD_BITS;

    size_t* bitmap = heap->bitmap;
    bool already_marked;

    if (first_word == last_word) {
        size_t bitmask = make_bitmask(first_bit, last_bit);
        already_marked = !!(bitmap[first_word] & bitmask);
        if (!already_marked) {
            bitmap[first_word] |= bitmask;
        }
    } else {
        size_t first_mask = make_bitmask(first_bit, 63);
        already_marked = !!(bitmap[first_word] & first_mask);
        if (!already_marked) {
            bitmap[first_word] |= first_mask;
            bitmap[last_word] |= make_bitmask(0, last_bit);

            // Long string
            for (size_t word = first_word+1; word<last_word; ++word) {
                bitmap[word] = ALL_ONES;
            }
        }
    }
    return already_marked;
}


void mark_trace(GcHeap* heap, ElmValue* v, size_t* ignore_below) {
    if ((size_t*)v < ignore_below) return;

    bool already_marked = mark_words(heap, v, v->header.size);
    if (already_marked) return;

    size_t n_children;
    void** child_ptr_array = find_children(v, &n_children);

    if (child_ptr_array == NULL) return;
    for (size_t i=0; i < n_children; ++i) {
        ElmValue* child = child_ptr_array[i];
        mark_trace(heap, child, ignore_below);
    }
}


// The stack map works like a list, but with 3 variants of Cons
// Use list terminology to make code more readable.
extern const size_t head;
extern const size_t tail;

void mark_stack_map(GcState* state, size_t* ignore_below) {
    // Types are important for pointer arithmetic, be careful
    Custom* stack_item = state->stack_map;
    Custom* prev_stack_item = (Custom*)state->next_alloc;

    size_t min_depth = state->stack_depth;
    size_t current_depth = state->stack_depth;
    size_t new_depth = state->stack_depth;

    // Mark all values allocated by running functions, which we need to replay & resume
    // These values may be needed in local variables of those functions
    while (1) {
        mark_words(&state->heap, stack_item, stack_item->header.size);
        bool mark_allocated = false;
        switch (stack_item->ctor) {
            case GcStackPush:
                // going backwards => entering a section that's shallower than before
                new_depth = current_depth - 1;
                if (new_depth < min_depth) {
                    min_depth = new_depth;
                    mark_allocated = (prev_stack_item->ctor != GcStackTailCall);
                }
                break;

            case GcStackPop:
                // going backwards => entering a section that's deeper than before
                new_depth = current_depth + 1;
            case GcStackTailCall:
                if (current_depth == min_depth) {
                    mark_trace(&state->heap, stack_item->values[head], ignore_below); // returned value from deeper function to active function
                    mark_allocated = true;
                }
                break;

            case GcStackEmpty:
            default:
                return;
        }

        // Mark all values allocated in this section of the stack
        if (mark_allocated) {
            size_t* first_allocated = (size_t*)stack_item + stack_item->header.size;
            size_t words_allocated = (size_t*)prev_stack_item - first_allocated;
            mark_words(&state->heap, first_allocated, words_allocated);
        }

        current_depth = new_depth;
        prev_stack_item = stack_item;
        stack_item = stack_item->values[tail];
    }
}


void mark(GcState* state, size_t* ignore_below) {
    for (ElmValue* root_cell=state->roots; root_cell->header.tag==Tag_Cons; root_cell=root_cell->cons.tail) {
        mark_words(&state->heap, root_cell, root_cell->header.size);

        // Each GC root is a mutable pointer in a fixed location outside the dynamic heap,
        // pointing to a value on the dynamic heap that should be preserved.
        // e.g. After `update`, the GC root pointer for `model` will be switched from the old to the new value.
        // The double pointer is the off-heap fixed address where we store the address of the current value
        ElmValue** root_mutable_pointer = (ElmValue**)root_cell->cons.head;
        ElmValue* live_heap_value = *root_mutable_pointer;
        mark_trace(&state->heap, live_heap_value, ignore_below);
    }
    mark_stack_map(state, ignore_below);
}



/* ====================================================

                COMPACT

   ==================================================== */

// void* forwarding_address(void* old_address) {
//     return NULL;
// }


// static void copy_value(u32* from, u32* to) {
//     void** child_ptr_array;
//     u32 n_children;
//     ElmValue* v = (ElmValue*)from;
//     child_ptr_array = find_children(v, &n_children);

//     if (!child_ptr_array || !n_children) {
//         // Int, Float, Char, String
//         for (u32 i=0; i < v->header.size; ++i) {
//             *to++ = *from++;
//         }
//     } else {
//         // container types
//         while ((void**)from < child_ptr_array) {
//             *to++ = *from++;
//         }
//         void** child_to = (void**)to;
//         void** child_from = (void**)from;
//         for (u32 i=0; i < n_children; ++i) {
//             *child_to++ = forwarding_address(*child_from++);
//         }
//     }
// }

// u32* compact() {
//     u32* current = state.pages[0].data;

//     // skip initial live data, find first garbage patch
//     while (is_marked(current)) {
//         current = next_heap_object(current);
//     }
//     u32* to = current;

//     while (current < state.current_heap) {
//         while (!is_marked(current)) {
//             current = next_heap_object(current);
//         }

//         u32* live_start = current;
//         u32* next_garbage = live_start + size_marked(live_start);

//         for (u32* from=live_start; from<next_garbage; ++from) {
//             copy_value(from, to);
//             to++;
//         }

//         current = to;
//     }

//     return state.current_heap;
// }






/* ====================================================

                STACK MAP

   ==================================================== */




/* ====================================================

                CONTROL

   ==================================================== */

void collect(GcState* state) {
}
