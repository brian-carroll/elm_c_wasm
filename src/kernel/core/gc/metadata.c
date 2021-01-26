#include "internals.h"

void bitmap_reset(GcHeap* heap) {
  for (size_t* bm_word = heap->bitmap; bm_word < heap->system_end; bm_word++) {
    *bm_word = 0;
  }
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



/* ====================================================

                ELM VALUES

   ==================================================== */

size_t child_count(ElmValue* v) {
  switch (v->header.tag) {
    case Tag_Int:
    case Tag_Float:
    case Tag_Char:
    case Tag_String:
      return 0;

    case Tag_List:
      return v == pNil ? 0 : 2;

    case Tag_Tuple2:
      return 2;

    case Tag_Tuple3:
      return 3;

    case Tag_Custom:
      return custom_params(&v->custom);

    case Tag_Record:
      return v->header.size - (sizeof(Record) / SIZE_UNIT);

    case Tag_FieldGroup:
      return 0;

    case Tag_Closure:
      return v->closure.n_values;

    case Tag_JsRef:
    case Tag_GcStackEmpty:
      return 0;

    case Tag_GcStackPush:
      return 1;

    case Tag_GcStackPop:
    case Tag_GcStackTailCall:
      return 2;
  }
}


size_t make_bitmask(size_t first_bit, size_t last_bit) {
  size_t mask = ALL_ONES;
  mask <<= GC_WORD_BITS - 1 - last_bit;
  mask >>= GC_WORD_BITS - 1 - (last_bit - first_bit);
  mask <<= first_bit;
  return mask;
}

void bitmap_next(size_t* word, size_t* mask) {
  *mask <<= 1;
  if (*mask == 0) {
    *word = *word + 1;
    *mask = 1;
  }
}

void bitmap_next_test_wrapper(size_t* word, size_t* mask) {
  bitmap_next(word, mask);
}
