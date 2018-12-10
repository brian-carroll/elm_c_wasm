#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include "./types.h"
#include "./gc-internals.h"
#ifdef DEBUG
    #include <stdio.h>
#endif


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

        case Tag_GcException:
        case Tag_GcStackEmpty:
            return 0;

        case Tag_GcStackPush:
            return 1;

        case Tag_GcStackPop:
        case Tag_GcStackTailCall:
            return 2;
    }
    return 0;
}

/* ====================================================

                SYSTEM MEMORY

   ==================================================== */

int set_heap_end(GcHeap* heap, size_t* new_break_ptr) {

    int has_error = brk(new_break_ptr);
    if (has_error) {
        #ifdef DEBUG
            fprintf(stderr, "Failed to get heap memory. Error code %d\n", errno);
        #endif
        return errno;
    }

    size_t heap_words = new_break_ptr - heap->start;

    // This calculation is in bytes, not words, to prevent
    // truncation errors for smaller blocks (<1 word of bitmap)
    const size_t bytes_per_word = sizeof(void*);
    size_t heap_bytes = heap_words * bytes_per_word;

    const size_t bitmap_bytes_per_block = GC_BLOCK_BYTES / GC_WORD_BITS;
    const size_t offset_bytes_per_block = bytes_per_word;
    const size_t block_plus_overhead_bytes =
        GC_BLOCK_BYTES
        + bitmap_bytes_per_block
        + offset_bytes_per_block;

    // A fractional block needs the overhead of a full block
    size_t heap_blocks = GC_DIV_ROUND_UP(heap_bytes, block_plus_overhead_bytes);

    size_t bitmap_bytes = heap_blocks * bitmap_bytes_per_block;
    size_t offset_bytes = heap_blocks * offset_bytes_per_block;

    // Now convert to words, rounding up
    size_t bitmap_words = GC_DIV_ROUND_UP(bitmap_bytes, bytes_per_word);
    size_t offset_words = GC_DIV_ROUND_UP(offset_bytes, bytes_per_word);

    heap->bitmap = new_break_ptr - bitmap_words;
    heap->offsets = heap->bitmap - offset_words;
    heap->end = heap->offsets;
    heap->system_end = new_break_ptr;

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
        
        #ifdef DEBUG
            if ((size_t*)child > heap->end) {
                fprintf(stderr,
                    "mark_trace: %p out of bounds, reached via %p with header tag %d\n",
                    child, v, v->header.tag
                );
                return;
            }
        #endif

        mark_trace(heap, child, ignore_below);
    }
}


void mark_stack_map(GcState* state, size_t* ignore_below) {
    GcStackMap* stack_item = state->stack_map;
    GcStackMap* prev_stack_item = (GcStackMap*)state->next_alloc;

    size_t min_depth = state->stack_depth;
    size_t current_depth = state->stack_depth;
    size_t new_depth = state->stack_depth;

    // Mark all values allocated by running functions, which we need to replay & resume
    // These values may be needed in local variables of those functions
    while (1) {
        bool mark_allocated = false;
        switch (stack_item->header.tag) {
            case Tag_GcStackPush:
                // going backwards => entering a section that's shallower than before
                new_depth = current_depth - 1;
                if (new_depth < min_depth) {
                    min_depth = new_depth;
                    mark_allocated = (prev_stack_item->header.tag != Tag_GcStackTailCall);
                }
                break;

            case Tag_GcStackPop:
                // going backwards => entering a section that's deeper than before
                new_depth = current_depth + 1;
            case Tag_GcStackTailCall:
                if (current_depth == min_depth) {
                    mark_trace(&state->heap, stack_item->data, ignore_below); // returned value from deeper function to active function
                    mark_allocated = true;
                }
                break;

            case Tag_GcStackEmpty:
                mark_words(&state->heap, stack_item, stack_item->header.size);
                return;

            default:
                return;
        }

        mark_words(&state->heap, stack_item, stack_item->header.size);

        // Mark all values allocated in this section of the stack
        if (mark_allocated) {
            size_t* first_allocated = (size_t*)stack_item + stack_item->header.size;
            size_t words_allocated = (size_t*)prev_stack_item - first_allocated;
            mark_words(&state->heap, first_allocated, words_allocated);
        }

        current_depth = new_depth;
        prev_stack_item = stack_item;
        stack_item = stack_item->older;
    }
}


void bitmap_reset(GcHeap* heap) {
    for (size_t* bm_word = heap->bitmap;
        bm_word < heap->system_end;
        bm_word++
    ) {
        *bm_word = 0;
    }
}


void mark(GcState* state, size_t* ignore_below) {
    bitmap_reset(&state->heap);
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


inline void bitmap_next(size_t* word, size_t* mask) {
    *mask <<= 1;
    if (*mask == 0) {
        *word = *word + 1;
        *mask = (size_t)1;
    }
}

void bitmap_next_test_wrapper(size_t* word, size_t* mask) {
    bitmap_next(word, mask);
}


// Count live words between two heap pointers, using the bitmap
size_t bitmap_dead_between(GcHeap* heap, size_t* first, size_t* last) {
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
        if ((heap->bitmap[word] & mask) == 0) count++;
        bitmap_next(&word, &mask);
    }
    while (mask < last_mask) {
        if ((heap->bitmap[word] & mask) == 0) count++;
        mask <<= 1;
    }
    return count;
}


// The offset is the (positive) amount by which a value is moved *down* during compaction
// Slightly different from the book and papers but easier on my brain.
void calc_offsets(GcHeap* heap, size_t* compact_start, size_t* compact_end) {
    size_t ndead = 0;
    size_t offset_index = (compact_start - heap->start) / GC_BLOCK_WORDS;
    heap->offsets[offset_index] = ndead;

    size_t* prev_block = compact_start;
    size_t prev_block_start_addr = (size_t)prev_block & GC_BLOCK_MASK;
    size_t* current_block = (size_t*)(prev_block_start_addr + GC_BLOCK_BYTES);

    #ifdef DEBUG
        printf("\n");
        printf("calc_offsets\n");
        printf("compact_start %p\n", compact_start);
        printf("heap->start %p\n", heap->start);
        printf("current_block %p\n", current_block);
        printf("\n");
    #endif

    while (current_block < compact_end) {
        ndead += bitmap_dead_between(heap, prev_block, current_block);
        heap->offsets[++offset_index] = ndead;

        #ifdef DEBUG
            printf("offset %zd %p : %zd\n", offset_index, current_block, ndead);
        #endif

        prev_block = current_block;
        current_block += GC_BLOCK_WORDS;
    }
}


// Calculate where a value has moved to, return a pointer to the new location
size_t* forwarding_address(GcHeap* heap, size_t* old_pointer) {
    size_t block_index = (old_pointer - heap->start) / GC_BLOCK_WORDS;
    size_t block_offset = heap->offsets[block_index];

    size_t* old_block_start = heap->start + (block_index * GC_BLOCK_WORDS);
    size_t offset_in_block = bitmap_dead_between(heap, old_block_start, old_pointer);

    size_t* new_pointer = old_pointer - block_offset - offset_in_block;

    #ifdef DEBUG
        printf("\nforwarding_address:\n");
        printf("old_pointer %p\n", old_pointer);
        printf("heap->start %p\n", heap->start);
        printf("block_index %zd\n", block_index);
        printf("block_offset %zd\n", block_offset);
        printf("old_block_start %p\n", old_block_start);
        printf("offset_in_block %zd\n", offset_in_block);
        printf("new_pointer %p\n", new_pointer);
        printf("old_pointer - new_pointer %zd\n", old_pointer - new_pointer);
        printf("\n");

        if (new_pointer > heap->end || new_pointer < heap->start) {
            fprintf(stderr,
                "forwarding_address: %p to %p (-%zd)\n",
                old_pointer, new_pointer, old_pointer - new_pointer
            );
        }
    #endif

    return new_pointer;
}



void compact(GcState* state, size_t* compact_start) {
    GcHeap* heap = &state->heap;
    size_t* compact_end = state->next_alloc;

    // Find starting point in bitmap
    size_t heap_index = (size_t)(compact_start - heap->start);
    size_t bm_word = heap_index / GC_WORD_BITS;
    size_t bm_bit = heap_index % GC_WORD_BITS;
    size_t bm_mask = (size_t)1 << bm_bit;

    // Find first garbage patch
    size_t* first_move_to = compact_start;
    while ((heap->bitmap[bm_word] & bm_mask) && (first_move_to < compact_end)) {
        bitmap_next(&bm_word, &bm_mask);
        first_move_to++;
    }
    if (first_move_to >= compact_end) return;

    calc_offsets(heap, compact_start, compact_end);
    size_t* to = first_move_to;
    size_t garbage_so_far = 0;

    #ifdef DEBUG
        printf("compact: first available space at %p\n", first_move_to);
    #endif


    // Iterate over live patches of data
    size_t* from = to;
    while (from < compact_end) {

        // Next live patch (bitmap only)
        while (!(heap->bitmap[bm_word] & bm_mask) && (from < compact_end)) {
            bitmap_next(&bm_word, &bm_mask);
            garbage_so_far++;
            from++;
        }
        size_t* live_patch_start = from;

        // Next garbage patch (bitmap only)
        size_t* next_garbage = from;
        while ((heap->bitmap[bm_word] & bm_mask) && (next_garbage < compact_end)) {
            bitmap_next(&bm_word, &bm_mask);
            next_garbage++;
        }

        #ifdef DEBUG
            printf("Moving %zd words down by %zd from (%p - %p) to %p\n",
                next_garbage - live_patch_start,
                garbage_so_far,
                live_patch_start,
                next_garbage-1,
                to
            );
        #endif

        // Copy each value in the live patch
        while (from < next_garbage) {

            // Find how many children this value has
            ElmValue* v = (ElmValue*)from;
            size_t n_children = child_count(v);
            size_t* next_value = from + v->header.size;
            size_t* first_child_field = next_value - n_children;

            #ifdef DEBUG
                if (n_children > 10
                    || next_value > heap->end
                    || v->header.size > 100
                ) {
                    fprintf(stderr, "Possibly corrupted object at %p : tag 0x%x size %d children %zd\n",
                        v, v->header.tag, v->header.size, n_children
                    );
                }
            #endif

            // Copy all the non-pointer data that comes before child pointers
            while (from < first_child_field) {
                *to++ = *from++;
            }

            // Copy the child pointers, modifying to point to new addresses
            size_t** child_ptr_array = (size_t**)first_child_field;
            for (size_t c=0; c < n_children; c++) {
                size_t* child_old = child_ptr_array[c];
                size_t* child_new;
                if (child_old < first_move_to) {
                    // Child value has not been moved
                    child_new = child_old;
                } else if (child_old >= live_patch_start) {
                    // Optimisation for related values in the same patch
                    child_new = child_old - garbage_so_far;
                } else {
                    // Calculate where child was moved to (using bitmap and block offsets)
                    child_new = forwarding_address(heap, child_old);
                }
                *to++ = (size_t)child_new;
            }

            from = next_value;
        }
    }

    // Compaction is finished. Update the GC state and roots.

    state->next_alloc = to;

    size_t* stack_map = (size_t*)state->stack_map;
    if (stack_map > first_move_to) {
        stack_map = forwarding_address(heap, stack_map);

        #ifdef DEBUG
            printf("Changing stackmap pointer from %p to %p\n",
                state->stack_map, stack_map
            );
        #endif

        state->stack_map = (GcStackMap*)stack_map;
    }

    for (ElmValue* root_cell=state->roots; root_cell->header.tag==Tag_Cons; root_cell=root_cell->cons.tail) {
        size_t** root_mutable_pointer = root_cell->cons.head;
        size_t* live_heap_value = *root_mutable_pointer;
        if (live_heap_value > first_move_to) {
            live_heap_value = forwarding_address(heap, live_heap_value);

            #ifdef DEBUG
                printf("Changing root from %p to %p\n",
                    *root_mutable_pointer, live_heap_value
                );
            #endif

            *root_mutable_pointer = live_heap_value;
        }
    }
}



/* ====================================================

                REPLAY

   ==================================================== */

void reverse_stack_map(GcState* state) {
    GcStackMap* newer_item = (GcStackMap*)state->next_alloc;

    for (GcStackMap* stack_item = state->stack_map;
        stack_item->header.tag != Tag_GcStackEmpty;
        stack_item = stack_item->older
    ) {
        stack_item->newer = newer_item;
        newer_item = stack_item;
    }
}


/* ====================================================

                CONTROL

   ==================================================== */

void heap_overflow(GcState* state) {
}
