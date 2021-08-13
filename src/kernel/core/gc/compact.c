#include "../types.h"
#include "internals.h"

// The offset is the (positive) amount by which a value is moved *down* during compaction
// Slightly different from the book and papers but easier on my brain.
void calc_offsets(GcHeap* heap, size_t* compact_start, size_t* compact_end) {
  size_t ndead = 0;
  size_t offset_index = (compact_start - heap->start) / GC_BLOCK_WORDS;
  heap->gc_temp[offset_index] = ndead;

  size_t* prev_block = compact_start;
  size_t prev_block_start_addr = (size_t)prev_block & GC_BLOCK_MASK;
  size_t* current_block = (size_t*)(prev_block_start_addr + GC_BLOCK_BYTES);

  while (current_block < compact_end) {
    ndead += bitmap_dead_between(heap, prev_block, current_block);
    heap->gc_temp[++offset_index] = ndead;

    prev_block = current_block;
    current_block += GC_BLOCK_WORDS;
  }
}


// Calculate where a value has moved to, return a pointer to the new location
void* forwarding_address(GcHeap* heap, size_t* old_pointer) {
  if (IS_OUTSIDE_HEAP(old_pointer)) return old_pointer;

  size_t block_index = (old_pointer - heap->start) / GC_BLOCK_WORDS;
  size_t block_offset = heap->gc_temp[block_index];

  size_t* old_block_start = heap->start + (block_index * GC_BLOCK_WORDS);
  size_t offset_in_block = bitmap_dead_between(heap, old_block_start, old_pointer);

  size_t* new_pointer = old_pointer - block_offset - offset_in_block;

  return new_pointer;
}


void compact(GcState* state, size_t* compact_start) {
  GcHeap* heap = &state->heap;
  size_t* compact_end = heap->end;

  // Find starting point in bitmap
  // bm_iter acts as a target for the `from` pointer, working ahead of it
  GcBitmapIter bm_iter = ptr_to_bitmap_iter(heap, compact_start);

  // Find first garbage patch
  bitmap_find(heap, false, &bm_iter);
  size_t* first_move_to = bitmap_iter_to_ptr(heap, bm_iter);
  if (first_move_to >= compact_end) return;

  calc_offsets(heap, compact_start, compact_end);


  // Adjust any upward pointers in the first live patch
  for (size_t* p = compact_start; p < first_move_to;) {
    ElmValue* v = (ElmValue*)p;
    size_t n_children = child_count(v);
    size_t* next_value = p + v->header.size;
    size_t* first_child_field = next_value - n_children;
    size_t** child_ptr_array = (size_t**)first_child_field;
    for (size_t c = 0; c < n_children; c++) {
      size_t* child_old = child_ptr_array[c];
      if (child_old > first_move_to) {
        child_ptr_array[c] = forwarding_address(heap, child_old);
      }
    }
    p = next_value;
  }


  // The `to` pointer is where we're copying to.
  // It just moves uniformly forward. Doesn't need to care about the mark bits.
  size_t* to = first_move_to;
  size_t garbage_so_far = 0;

  // Iterate over live patches of data
  size_t* from = to;
  while (from < compact_end) {
    // Next live patch (bitmap only)
    bitmap_find(heap, true, &bm_iter);
    size_t* live_patch_start = bitmap_iter_to_ptr(heap, bm_iter);
    garbage_so_far += live_patch_start - from;
    from = live_patch_start;

    // Next garbage patch (bitmap only)
    bitmap_find(heap, false, &bm_iter);
    size_t* next_garbage = bitmap_iter_to_ptr(heap, bm_iter);

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
      ASSERT_SANITY(v);
      ASSERT(next_value < heap->end, next_value);

      // Copy all the non-pointer data that comes before child pointers
      while (from < first_child_field) {
        *to++ = *from++;
      }

      // Copy the child pointers, modifying to point to new addresses
      size_t** child_ptr_array = (size_t**)first_child_field;
      for (size_t c = 0; c < n_children; c++) {
        size_t* child_old = child_ptr_array[c];
        size_t* child_new;
        if (child_old < first_move_to || child_old > heap->end) {
          // Child value has not been moved
          child_new = child_old;
        } else if (child_old >= live_patch_start && child_old < from) {
          // Optimisation for parent and child both in the current live patch
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
  state->end_of_old_gen = to;
  state->end_of_alloc_patch = heap->end;

  size_t* roots = (size_t*)state->roots;
  if (roots > first_move_to) {
    state->roots = forwarding_address(heap, roots);
  }

  for (Cons* root_cell = state->roots; root_cell != &Nil; root_cell = root_cell->tail) {
    size_t** root_mutable_pointer = root_cell->head;
    size_t* current_root_value = *root_mutable_pointer;
    if (current_root_value > first_move_to && current_root_value < heap->end) {
      current_root_value = forwarding_address(heap, current_root_value);
      *root_mutable_pointer = current_root_value;
    }
  }
}
