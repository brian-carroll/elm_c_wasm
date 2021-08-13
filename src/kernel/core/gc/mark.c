#include "../core.h"
#include "../types.h"
#include "internals.h"

// Mark a value as live and return 'true' if previously marked
bool mark_words(GcState* state, void* p_void, size_t size) {
  GcHeap* heap = &state->heap;
  size_t* p = (size_t*)p_void;

  if (IS_OUTSIDE_HEAP(p)) {
    return true;  // "already marked" (will not be GC'd, don't trace)
  }
  if (size == 0) return true;
  ASSERT(p < heap->end, p);
  if (p + size > heap->end) {
    safe_printf("Marking %p - %p, past heap end at %p\n", p, p + size, heap->end);
    print_state();
    print_stack_map();
    // fflush(0);
  }
  ASSERT(p + size <= heap->end, p + size);

  size_t first_heap_index = (size_t)(p - heap->start);
  size_t first_bmp_index = first_heap_index / 64;
  size_t first_bit = first_heap_index & 63;

  size_t last_heap_index = first_heap_index + (size - 1);  // requires size>0
  size_t last_bmp_index = last_heap_index / 64;
  size_t last_bit = last_heap_index & 63;

  u64* bitmap = heap->bitmap;
  bool already_marked;

  if (first_bmp_index == last_bmp_index) {
    u64 bitmask = make_bitmask(first_bit, last_bit);
    already_marked = !!(bitmap[first_bmp_index] & bitmask);
    if (!already_marked) {
      state->n_marked_words += size;
      bitmap[first_bmp_index] |= bitmask;
    }
  } else {
    u64 first_mask = make_bitmask(first_bit, 63);
    already_marked = !!(bitmap[first_bmp_index] & first_mask);
    if (!already_marked) {
      state->n_marked_words += size;
      bitmap[first_bmp_index] |= first_mask;

      // Loop for values spanning >2 words in bitmap. Rare. ElmString only.
      for (size_t i = first_bmp_index + 1; i < last_bmp_index; ++i) {
        bitmap[i] = ALL_ONES;
      }

      bitmap[last_bmp_index] |= make_bitmask(0, last_bit);
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
    ASSERT_SANITY(v);

    bool already_marked = mark_words(state, v, v->header.size);
    if (already_marked) continue;

    if (v->header.tag == Tag_JsRef) {
      markJsRef(v->js_ref.id);
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
    if (stack_flags[i] == 'F') continue;  // TODO: should we also skip the function?
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

  // Fix up the heap layout if we grew gc_temp_data while tracing
  // (if the app allocated huge data structures that were both deep and wide)
  if (heap->system_end != heap_copy.system_end) {
    size_t already_resized_bytes = (heap->system_end - heap->start) * sizeof(void*);
    set_heap_layout(heap, heap->start, already_resized_bytes);
    move_metadata_after_resize(&heap_copy, heap);
  }
}
