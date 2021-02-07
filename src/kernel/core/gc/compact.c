#include "internals.h"

// The offset is the (positive) amount by which a value is moved *down* during compaction
// Slightly different from the book and papers but easier on my brain.
void calc_offsets(GcHeap* heap, size_t* compact_start, size_t* compact_end) {
  size_t ndead = 0;
  size_t offset_index = (compact_start - heap->start) / GC_BLOCK_WORDS;
  heap->offsets[offset_index] = ndead;

  size_t* prev_block = compact_start;
  size_t prev_block_start_addr = (size_t)prev_block & GC_BLOCK_MASK;
  size_t* current_block = (size_t*)(prev_block_start_addr + GC_BLOCK_BYTES);

  log_debug("\n");
  log_debug("calc_offsets\n");
  log_debug("compact_start %p\n", compact_start);
  log_debug("heap->start %p\n", heap->start);
  log_debug("current_block %p\n", current_block);
  log_debug("\n");

  while (current_block < compact_end) {
    ndead += bitmap_dead_between(heap, prev_block, current_block);
    heap->offsets[++offset_index] = ndead;

    prev_block = current_block;
    current_block += GC_BLOCK_WORDS;
  }
}


// Calculate where a value has moved to, return a pointer to the new location
void* forwarding_address(GcHeap* heap, size_t* old_pointer) {
  if (old_pointer < heap->start) return old_pointer;

  size_t block_index = (old_pointer - heap->start) / GC_BLOCK_WORDS;
  size_t block_offset = heap->offsets[block_index];

  size_t* old_block_start = heap->start + (block_index * GC_BLOCK_WORDS);
  size_t offset_in_block = bitmap_dead_between(heap, old_block_start, old_pointer);

  size_t* new_pointer = old_pointer - block_offset - offset_in_block;

  log_debug("\nforwarding_address:\n");
  log_debug("old_pointer %p\n", old_pointer);
  log_debug("heap->start %p\n", heap->start);
  log_debug("block_index %zd\n", block_index);
  log_debug("block_offset %zd\n", block_offset);
  log_debug("old_block_start %p\n", old_block_start);
  log_debug("offset_in_block %zd\n", offset_in_block);
  log_debug("new_pointer %p\n", new_pointer);
  log_debug("old_pointer - new_pointer %zd\n", old_pointer - new_pointer);
  log_debug("\n");

  assert(new_pointer >= heap->start && new_pointer < heap->end);

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

  log_debug("compact: first available space at %p\n", first_move_to);

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

    log_debug("Moving %zd words down by %zd from (%p - %p) to %p\n",
        next_garbage - live_patch_start,
        garbage_so_far,
        live_patch_start,
        next_garbage - 1,
        to);

    // Copy each value in the live patch
    while (from < next_garbage) {
      // Find how many children this value has
      ElmValue* v = (ElmValue*)from;
      size_t n_children = child_count(v);
      size_t* next_value = from + v->header.size;
      size_t* first_child_field = next_value - n_children;

      // sanity check for corrupted data
      assert(n_children <= 10 && next_value < heap->end && v->header.size < 100);

      // Copy all the non-pointer data that comes before child pointers
      while (from < first_child_field) {
        *to++ = *from++;
      }

      // Copy the child pointers, modifying to point to new addresses
      size_t** child_ptr_array = (size_t**)first_child_field;
      for (size_t c = 0; c < n_children; c++) {
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
  state->nursery = to;

  for (size_t i = 0; i < state->stack_map.index; ++i) {
    stack_values[i] = forwarding_address(heap, stack_values[i]);
  }

  size_t* roots = (size_t*)state->roots;
  if (roots > first_move_to) {
    state->roots = forwarding_address(heap, roots);
  }

  for (Cons* root_cell = state->roots; root_cell != &Nil; root_cell = root_cell->tail) {
    size_t** root_mutable_pointer = root_cell->head;
    size_t* live_heap_value = *root_mutable_pointer;
    if (live_heap_value > first_move_to) {
      live_heap_value = forwarding_address(heap, live_heap_value);

      log_debug("Changing root from %p to %p\n", *root_mutable_pointer, live_heap_value);

      *root_mutable_pointer = live_heap_value;
    }
  }
}
