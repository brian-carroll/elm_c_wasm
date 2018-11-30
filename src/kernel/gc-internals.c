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

size_t child_count(ElmValue* v) {
    switch (v->header.tag) {
        case Tag_Int:
        case Tag_Float:
        case Tag_Char:
        case Tag_String:
        case Tag_Nil:
            return 0;

        case Tag_Cons:
        case Tag_Tuple2:
            return 2;

        case Tag_Tuple3:
            return 3;

        case Tag_Custom:
            return custom_params(&v->custom);

        case Tag_Record:
            return v->record.fieldset->size;

        case Tag_Closure:
            return v->closure.n_values;

        case Tag_GcFull:
            return 1;
    }
    return 0;
}

/* ====================================================

                SYSTEM MEMORY

   ==================================================== */

int set_heap_end(GcHeap* heap, size_t* new_break_ptr) {

    int has_error = brk(new_break_ptr);
    if (has_error) {
        // fprintf(stderr, "Failed to get heap memory. Error code %d\n", errno);
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


/* ====================================================

                MARK

   ==================================================== */

size_t make_bitmask(size_t first_bit, size_t last_bit) {
    size_t mask = ALL_ONES;
    mask <<= GC_WORD_BITS -1 - last_bit;
    mask >>= GC_WORD_BITS -1 - (last_bit - first_bit);
    mask <<= first_bit;
    return mask;
}


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
        size_t first_mask = make_bitmask(first_bit, GC_WORD_BITS-1);
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
    size_t* first_word = (size_t*)v;
    if (first_word < ignore_below) return;

    bool already_marked = mark_words(heap, v, v->header.size);
    if (already_marked) return;

    size_t n_children = child_count(v);
    size_t* first_child_field = first_word + v->header.size - n_children;
    ElmValue** child_ptr_array = (ElmValue**)first_child_field;

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
    for (size_t* bm_word = state->heap.bitmap; bm_word < state->heap.system_end; bm_word++) {
        *bm_word = 0;
    }
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

const size_t max_mask = (size_t)1 << (GC_WORD_BITS-1);
const size_t BLOCK_MASK = -GC_BLOCK_WORDS;


// Count live words between two heap pointers, using the bitmap
size_t bitmap_live_between(GcHeap* heap, size_t* first, size_t* last) {
    size_t first_index = (size_t)(first - heap->start);
    size_t first_word = first_index / GC_WORD_BITS;
    size_t first_mask = (size_t)1 << (first_index % GC_WORD_BITS);

    size_t last_index = (size_t)(last - heap->start);
    size_t last_word = last_index / GC_WORD_BITS;
    size_t last_mask = (size_t)1 << (last_index % GC_WORD_BITS);

    size_t count = 0;
    size_t word = first_word;
    size_t mask = first_mask;

    while (word < last_word) {
        while (mask < max_mask) {
            if (heap->bitmap[word] & mask) {
                count++;
            }
            mask <<= 1;
        }
        mask = 1;
        word++;
    }
    while (mask < last_mask) {
        if (heap->bitmap[word] & mask) {
            count++;
        }
        mask <<= 1;
    }
    return count;
}


// Calculate where a value has moved to, return a pointer to the new location
size_t* forwarding_address(GcHeap* heap, size_t* old_pointer) {
    size_t* old_block_start = (size_t*)((size_t)old_pointer & BLOCK_MASK);
    size_t offset_in_block = bitmap_live_between(heap, old_block_start, old_pointer);
    size_t offset_index = (old_block_start - heap->start) / GC_BLOCK_WORDS;
    size_t* new_block_start = heap->offsets[offset_index];
    size_t* new_pointer = new_block_start + offset_in_block;
    return new_pointer;
}


void compact(GcState* state, size_t* compact_start) {

    GcHeap* heap = &state->heap;
    size_t* compact_end = state->next_alloc;

    size_t* next_block_ptr = compact_start;

    // Find starting point in bitmap
    size_t heap_index = (size_t)(compact_start - heap->start);
    size_t bm_word = heap_index / GC_WORD_BITS;
    size_t bm_bit = heap_index % GC_WORD_BITS;
    size_t bm_mask = (size_t)1 << bm_bit;

    // Find first garbage patch
    size_t* next_garbage = compact_start;
    while ((heap->bitmap[bm_word] & bm_mask) && (next_garbage < compact_end)) {
        if (bm_mask < max_mask) {
            bm_mask <<= 1;
        } else {
            bm_mask = 1;
            bm_word++;
        }
        next_garbage++;
    }
    if (next_garbage >= compact_end) return;

    size_t* to = next_garbage;
    size_t* from = to;

    // Iterate over live patches of data
    while (from < compact_end) {

        // Move 'from' to start of a live patch (bitmap only, no heap operations)
        while (!(heap->bitmap[bm_word] & bm_mask) && (from < compact_end)) {
            if (bm_mask < max_mask) {
                bm_mask <<= 1;
            } else {
                bm_mask = 1;
                bm_word++;
            }
            from++;
        }
        if (from >= compact_end) return;

        // Move 'next_garbage' to after the live patch (bitmap only, no heap operations)
        next_garbage = from;
        while ((heap->bitmap[bm_word] & bm_mask) && (next_garbage < compact_end)) {
            if (bm_mask < max_mask) {
                bm_mask <<= 1;
            } else {
                bm_mask = 1;
                bm_word++;
            }
            next_garbage++;
        }

        // Copy each value in the live patch (actual heap operations)
        while (from < next_garbage) {

            // Update offsets array if this is the first live value in a block
            // (Offsets are used to calculate forwarding addresses later)
            if (from >= next_block_ptr) {
                size_t current_block_addr = (size_t)from & BLOCK_MASK; // may higher than next_block_ptr!
                size_t heap_start_addr = (size_t)heap->start;
                size_t current_block_idx =
                    (current_block_addr - heap_start_addr) / GC_BLOCK_WORDS;
                heap->offsets[current_block_idx] = to;
                next_block_ptr = 
                    heap->start + ((current_block_idx+1) * GC_BLOCK_WORDS);
            }

            // Find the children of this value, if any
            ElmValue* v = (ElmValue*)from;
            size_t n_children = child_count(v);
            size_t* next_value = from + v->header.size;
            size_t* first_child_field = next_value - n_children;

            // Copy all the non-pointer data that comes before child pointers
            while (from < first_child_field) {
                *to++ = *from++;
            }

            // Copy the child pointers, modifying to point to new addresses
            size_t** child_ptr_array = (size_t**)first_child_field;
            for (size_t c=0; c < n_children; c++) {
                size_t* child_old = child_ptr_array[c];
                size_t* child_new =
                    (child_old < compact_start)
                        ? child_old
                        : forwarding_address(heap, child_old);
                *to++ = (size_t)child_new;
            }

            from = next_value;
        }
    }

    state->next_alloc = to;

    // TODO: forward addresses for roots and stack map
}



/* ====================================================

                CONTROL

   ==================================================== */

void collect(GcState* state) {
}
