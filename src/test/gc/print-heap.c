#include <stdbool.h>
#include <stdio.h>
#include "../../kernel/gc-internals.h"
#include "../../kernel/types.h"

extern GcState gc_state;

bool is_marked(void* p) {
  GcState* state = &gc_state;
  size_t* pword = (size_t*)p;
  size_t slot = pword - state->heap.start;
  if (slot >> (GC_WORD_BITS - 1)) return true;  // off heap => not garbage, stop tracing
  size_t word = slot / GC_WORD_BITS;
  size_t bit = slot % GC_WORD_BITS;
  size_t mask = (size_t)1 << bit;

  return (state->heap.bitmap[word] & mask) != 0;
}

void print_value(ElmValue* v) {
  printf("| %p |  %c   |%5d | ", v, is_marked(v) ? 'X' : ' ', v->header.size);
  switch (v->header.tag) {
    case Tag_Int:
      printf("Int %d", v->elm_int.value);
      break;
    case Tag_Float:
      printf("Float %f", v->elm_float.value);
      break;
    case Tag_Char:
      printf("Char 0x%8x", v->elm_char.value);
      break;
    case Tag_String:
      printf("String \"%s\"", v->elm_string.bytes);
      break;
    case Tag_List:
      printf("Cons head: %p tail: %p", v->cons.head, v->cons.tail);
      break;
    case Tag_Tuple2:
      printf("Tuple2 a: %p b: %p", v->tuple2.a, v->tuple2.b);
      break;
    case Tag_Tuple3:
      printf("Tuple3 a: %p b: %p c: %p", v->tuple3.a, v->tuple3.b, v->tuple3.c);
      break;
    case Tag_Custom:
      printf("Custom ctor: %d ", v->custom.ctor);
      for (size_t i = 0; i < custom_params(&v->custom); ++i) {
        printf("%p ", v->custom.values[i]);
      }
      break;
    case Tag_Record:
      printf("Record fieldgroup: %p values: ", v->record.fieldgroup);
      for (size_t i = 0; i < v->record.fieldgroup->size; ++i) {
        printf("%p ", v->record.values[i]);
      }
      break;
    case Tag_Closure:
      printf("Closure (%p) n_values: %d max_values: %d values: ", v->closure.evaluator,
             v->closure.n_values, v->closure.max_values);
      for (size_t i = 0; i < v->closure.n_values; ++i) {
        printf("%p ", v->closure.values[i]);
      }
      break;
    case Tag_GcException:
      printf("GcException");
      break;
    case Tag_GcStackPush:
      printf("GcStackPush newer: %p older: %p", v->gc_stackmap.newer,
             v->gc_stackmap.older);
      break;
    case Tag_GcStackPop:
      printf("GcStackPop newer: %p older: %p replay: %p", v->gc_stackmap.newer,
             v->gc_stackmap.older, v->gc_stackmap.replay);
      break;
    case Tag_GcStackTailCall:
      printf("GcStackTailCall newer: %p older: %p replay: %p", v->gc_stackmap.newer,
             v->gc_stackmap.older, v->gc_stackmap.replay);
      break;
    case Tag_GcStackEmpty:
      printf("GcStackEmpty newer: %p", v->gc_stackmap.newer);
      break;
    case Tag_Unused:
      printf("Unused tag!!");
  }
  printf("\n");
}

void print_heap() {
  GcState* state = &gc_state;

#ifdef TARGET_64BIT
  printf("|    Address     | Mark | Size | Value\n");
  printf("| -------------- | ---- | ---- | -----\n");

#else
  printf("| Address  | Mark | Size | Value\n");
  printf("| -------- | ---- | ---- | -----\n");
#endif

  size_t* first_value = state->heap.start;
  ElmValue* v = (ElmValue*)first_value;
  if (v->header.tag == Tag_GcStackEmpty) {
    print_value(v);
    first_value += v->header.size;
  }
  // skip junk zeros used to force GC in tests
  size_t* zeros = first_value;
  while (*first_value == 0)
    first_value++;

  if (first_value > zeros) {
    printf("| %p |      |%5zd | (zeros)\n", zeros, first_value - zeros);
  }

  size_t* next_value = first_value;
  for (size_t* p = first_value; p < state->next_alloc; p++) {
    if (p == next_value) {
      v = (ElmValue*)p;
      print_value(v);
      if (v->header.size > 0 && v->header.size < 128) {
        next_value += v->header.size;
      } else {
        next_value++;
      }
    } else {
      printf("| %p |  %c   |      |\n", p, is_marked(p) ? 'X' : ' ');
    }
  }
}

void print_state() {
  GcState* state = &gc_state;

  printf("start = %p\n", state->heap.start);
  printf("end = %p (%zd bytes)\n", state->heap.end, state->heap.end - state->heap.start);
  printf("offsets = %p\n", state->heap.offsets);
  printf("bitmap = %p\n", state->heap.bitmap);
  printf("system_end = %p\n", state->heap.system_end);
  printf("next_alloc = %p\n", state->next_alloc);
  printf("roots = %p\n", state->roots);
  printf("stack_map = %p\n", state->stack_map);
  printf("stack_depth = %zd\n", state->stack_depth);
  printf("stack_map_empty = %p\n", state->stack_map_empty);

  // find last non-zero word in the bitmap
  size_t bitmap_size = state->heap.system_end - state->heap.bitmap;
  size_t last_word = bitmap_size;

  printf("Bitmap (size %zd):\n", bitmap_size);
  for (size_t word = 0; word <= last_word && word < bitmap_size; word++) {
    size_t value = state->heap.bitmap[word];
    if (value) {
#ifdef TARGET_64BIT
      printf("%3zd | %016zx\n", word, value);
#else
      printf("%3zd | %08zx\n", word, value);
#endif
    }
  }

  printf("\n");
}
