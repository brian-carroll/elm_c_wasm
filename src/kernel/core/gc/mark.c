#include "internals.h"
#include "../core.h"
#include "../types.h"

// Mark a value as live and return 'true' if previously marked
bool mark_words(GcState* state, void* p_void, size_t size) {
  GcHeap* heap = &state->heap;
  size_t* p = (size_t*)p_void;

  if (IS_OUTSIDE_HEAP(p)) {
    return true;  // "already marked" (will not be GC'd, don't trace)
  }
  if (size == 0) return true;
  assert(p < heap->end);
  if (p + size > heap->end) {
    safe_printf("Marking %p - %p, past heap end at %p\n", p, p+size, heap->end);
    print_state();
    print_stack_map();
    // fflush(0);
  }
  assert(p + size <= heap->end);

  size_t first_index = (size_t)(p - heap->start);
  size_t first_word = first_index / GC_WORD_BITS;
  size_t first_bit = first_index % GC_WORD_BITS;

  size_t last_index = first_index + (size - 1);  // requires size>0
  size_t last_word = last_index / GC_WORD_BITS;
  size_t last_bit = last_index % GC_WORD_BITS;

  size_t* bitmap = heap->bitmap;
  bool already_marked;

  if (first_word == last_word) {
    size_t bitmask = make_bitmask(first_bit, last_bit);
    already_marked = !!(bitmap[first_word] & bitmask);
    if (!already_marked) {
      state->n_marked_words += size;
      bitmap[first_word] |= bitmask;
    }
  } else {
    size_t first_mask = make_bitmask(first_bit, GC_WORD_BITS - 1);
    already_marked = !!(bitmap[first_word] & first_mask);
    if (!already_marked) {
      state->n_marked_words += size;
      bitmap[first_word] |= first_mask;

      // Loop for values spanning >2 words in bitmap. Rare. ElmString only.
      for (size_t word = first_word + 1; word < last_word; ++word) {
        bitmap[word] = ALL_ONES;
      }

      bitmap[last_word] |= make_bitmask(0, last_bit);
    }
  }
  return already_marked;
}

static void mark_trace(GcState* state, ElmValue* root, size_t* ignore_below) {
  GcHeap* heap = &state->heap;
  if ((size_t*)root < ignore_below || (size_t*)root > heap->end) {
    return;
  }

  // Use GC temporary memory area as a stack of pointers to trace
  ElmValue** mark_stack = (ElmValue**)heap->gc_temp;
  mark_stack[0] = root;
  size_t mark_stack_size = heap->gc_temp_size;
  size_t next_index = 1;

  do {
    ElmValue* v = mark_stack[--next_index];  // pop
    size_t* words = (size_t*)v;
    // assert(sanity_check(v)); // !!
    if (!sanity_check(v)) {
      print_stack_map();
      print_heap_range(words - 64, words + 16);
      Debug_print_offset("corrupt", v);
      exit(1);
    }

    bool already_marked = mark_words(state, v, v->header.size);
    if (already_marked) continue;

    if (v->header.tag == Tag_JsRef) {
      markJsRef(v->js_ref.index);
      continue;
    }

    size_t n_children = child_count(v);
    ElmValue** children = (ElmValue**)(words + v->header.size - n_children);

    for (size_t i = 0; i < n_children; ++i) {
      ElmValue* child = children[i];
      size_t* child_words = (size_t*)child;

      if (Debug_is_target_addr(child)) {
        Debug_print_offset("parent", v);
        print_value_full(v);
        Debug_print_offset("child", child);
        print_value_full(child);
        Debug_pretty("child", child);
      }

      if (child_words < ignore_below || child_words > heap->end) {
        continue;
      }
      mark_stack[next_index] = child;
      next_index++;

      if (next_index >= mark_stack_size) {
        safe_printf("mark stack overflowed! Get more memory to grow it\n");
        // mark stack overflowed! Get more memory to grow it
        size_t new_total_bytes = next_heap_size_bytes(heap, 0);
        resize_system_memory(heap, new_total_bytes);
        // For now, allocate all of the new space to the mark stack.
        // Params related to the bitmap and app data must stay where they are.
        heap->system_end = heap->start + (new_total_bytes / sizeof(void*));
        heap->gc_temp_size = heap->system_end - heap->gc_temp;
        mark_stack_size = heap->gc_temp_size;
      }
    }
  } while (next_index);
}

/**
 * Mark live words
 * state: The GC state
 * ignore_below: Lowest address to consider for marking
 *      (used to ignore the old generation in a minor GC)
 */
void mark(GcState* state, size_t* ignore_below) {
  GcHeap* heap = &state->heap;
  GcHeap heap_copy = *heap;

  // Clear all mark bits
  bitmap_reset(heap);
  state->n_marked_words = 0;

  // Mark values being used in the current call stack
  for (size_t i = 0; i < state->stack_map.index; ++i) {
    if (stack_flags[i] == 'F') continue;
    ElmValue* v = stack_values[i];
    mark_trace(state, v, ignore_below);
  }

  // Mark GC roots
  for (Cons* root_cell = state->roots; root_cell != &Nil; root_cell = root_cell->tail) {
    mark_words(state, root_cell, SIZE_LIST);

    // Each GC root is a mutable pointer in a fixed location outside the heap,
    // pointing to a value on the heap that should be preserved.
    // e.g. After `update`, the root pointer for `model` will be updated to point to
    // to the new value.
    // The double pointer is the off-heap address where we store the current heap address.
    ElmValue** root_mutable_pointer = (ElmValue**)root_cell->head;
    ElmValue* live_heap_value = *root_mutable_pointer;
    mark_trace(state, live_heap_value, ignore_below);
  }

  if (heap->system_end != heap_copy.system_end) {
    // We grew the heap to prevent overflowing the mark stack
    // The gc_temp area got all the extra space, but now we redistribute normally
    size_t already_resized_bytes = (heap->system_end - heap->start) * sizeof(void*);
    set_heap_layout(heap, heap->start, already_resized_bytes);
    move_metadata_after_resize(&heap_copy, heap);
  }
}
