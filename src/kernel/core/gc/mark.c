#include "internals.h"
#include <stdio.h>

// Mark a value as live and return 'true' if previously marked
bool mark_words(GcHeap* heap, void* p_void, size_t size) {
  size_t* p = (size_t*)p_void;

  if (IS_OUTSIDE_HEAP(p)) return true;   // "already marked" (will not be GC'd, don't trace)
  if (size == 0) return true;
  assert(p < heap->end);
  assert(p + size < heap->end);

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
      bitmap[first_word] |= bitmask;
    }
  } else {
    size_t first_mask = make_bitmask(first_bit, GC_WORD_BITS - 1);
    already_marked = !!(bitmap[first_word] & first_mask);
    if (!already_marked) {
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

void mark_trace(GcHeap* heap, ElmValue* v, size_t* ignore_below) {
  size_t* first_word = (size_t*)v;
  if (first_word < ignore_below) return;
  size_t n_children = child_count(v);

  assert(sanity_check(v));

  bool already_marked = mark_words(heap, v, v->header.size);
  if (already_marked) return;

  if (v->header.tag == Tag_JsRef) {
    markJsRef(v->js_ref.index);
  }

  size_t* first_child_field = first_word + v->header.size - n_children;
  ElmValue** child_ptr_array = (ElmValue**)first_child_field;

  for (size_t i = 0; i < n_children; ++i) {
    ElmValue* child = child_ptr_array[i];
    if (IS_OUTSIDE_HEAP(child)) continue;

    assert((size_t*)child < heap->end);
    assert(child <= v); // Need the '=' because Closures can refer to themselves

    mark_trace(heap, child, ignore_below);
  }
}

void mark(GcState* state, size_t* ignore_below) {
  GcHeap* heap = &state->heap;

  // Clear all mark bits
  bitmap_reset(heap);

  // Mark values freshly allocated in still-running function calls
  for (size_t i = 0; i < state->stack_map.index; ++i) {
    if (stack_flags[i] == 'F') continue;
    ElmValue* v = stack_values[i];
    mark_trace(heap, v, ignore_below);
  }

  // Mark GC roots (mutable values in Elm effect managers, including the Model)

  for (Cons* root_cell = state->roots; root_cell != &Nil; root_cell = root_cell->tail) {
    // printf("marking root\n");
    mark_words(&state->heap, root_cell, sizeof(Cons) / SIZE_UNIT);

    // Each GC root is a mutable pointer in a fixed location outside the dynamic heap,
    // pointing to a value on the dynamic heap that should be preserved.
    // e.g. After `update`, the GC root pointer for `model` will be switched from the old
    // to the new value. The double pointer is the off-heap fixed address where we store
    // the address of the current value
    ElmValue** root_mutable_pointer = (ElmValue**)root_cell->head;
    ElmValue* live_heap_value = *root_mutable_pointer;
    mark_trace(&state->heap, live_heap_value, ignore_below);
  }
}
