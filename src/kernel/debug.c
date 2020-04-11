#include "./debug.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "./gc-internals.h"
#include "./types.h"
#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#else
#define emscripten_run_script(x)
#endif

extern GcState gc_state;

bool is_marked(void* p) {
  GcState* state = &gc_state;
  size_t* pword = (size_t*)p;
  if (pword < state->heap.start || pword > state->heap.end) return true;
  size_t slot = pword - state->heap.start;
  size_t word = slot / GC_WORD_BITS;
  size_t bit = slot % GC_WORD_BITS;
  size_t mask = (size_t)1 << bit;
  size_t masked = state->heap.bitmap[word] & mask;
  size_t downshift = masked >> bit;  // get 1 or 0, avoiding 64-bit compiler bugs
  return (bool)downshift;
}

void print_value(ElmValue* v) {
  printf("| " FORMAT_PTR " | " FORMAT_HEX " |  %c   |%5d | ",
      v,
      *((size_t*)v),
      is_marked(v) ? 'X' : ' ',
      v->header.size);
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
#if STRING_ENCODING == UTF16
      printf("String \"");
      size_t body_bytes = (v->header.size * SIZE_UNIT) - sizeof(Header);
      for (size_t i = 0; i < body_bytes; i = i + 2) {
        char c = v->elm_string.bytes[i];
        if (c) printf("%c", c);
      }
      printf("\"");
#else
      printf("String \"%s\"", v->elm_string.bytes);
#endif
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
    case Tag_Record: {
      printf("Record fieldgroup: %p values: ", v->record.fieldgroup);
      size_t header_kids = (size_t)(v->header.size) - (sizeof(Record) / sizeof(void*));
      for (size_t i = 0; i < header_kids; ++i) {
        printf("%p ", v->record.values[i]);
      }
      break;
    }
    case Tag_Closure: {
      printf("Closure (%p) n_values: %d max_values: %d values: ",
          v->closure.evaluator,
          v->closure.n_values,
          v->closure.max_values);
      size_t header_kids = (size_t)(v->header.size) - (sizeof(Closure) / sizeof(void*));
      for (size_t i = 0; i < header_kids; ++i) {
        printf("%p ", v->closure.values[i]);
      }
      break;
    }
    case Tag_GcException:
      printf("GcException");
      break;
    case Tag_GcStackPush:
      printf(
          "GcStackPush newer: %p older: %p", v->gc_stackmap.newer, v->gc_stackmap.older);
      break;
    case Tag_GcStackPop:
      printf("GcStackPop newer: %p older: %p replay: %p",
          v->gc_stackmap.newer,
          v->gc_stackmap.older,
          v->gc_stackmap.replay);
      break;
    case Tag_GcStackTailCall:
      printf("GcStackTailCall newer: %p older: %p replay: %p",
          v->gc_stackmap.newer,
          v->gc_stackmap.older,
          v->gc_stackmap.replay);
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
  printf("|    Address     |       Hex        | Mark | Size | Value\n");
  printf("| -------------- | ---------------- | ---- | ---- | -----\n");
#else
  printf("| Address  |   Hex    | Mark | Size | Value\n");
  printf("| -------- | -------- | ---- | ---- | -----\n");
#endif

  size_t* first_value = state->heap.start;
  size_t* next_value = first_value;
  for (size_t* p = first_value; p < state->next_alloc; p++) {
    if (p == next_value) {
      if (*p == 0) {
        // summarize big chunks of zeros
        while (*p == 0)
          p++;
        printf("| " FORMAT_PTR " | " FORMAT_HEX " |      |%5zd | (zeros)\n",
            next_value,
            (size_t)0,
            p - next_value);
        next_value = p;
        if (p >= state->next_alloc) break;
      }
      ElmValue* v = (ElmValue*)p;
      print_value(v);
      if (v->header.size > 0 && v->header.size < 128) {
        next_value += v->header.size;
      } else {
        next_value++;
      }
    } else {
      printf("| " FORMAT_PTR " | " FORMAT_HEX " |  %c   |      |\n",
          p,
          *p,
          is_marked(p) ? 'X' : ' ');
    }
  }
}

void print_bitmap() {
  GcState* state = &gc_state;

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

void print_state() {
  GcState* state = &gc_state;

  size_t start = (size_t)state->heap.start;
  size_t end = (size_t)state->heap.end;
  size_t system_end = (size_t)state->heap.system_end;
  size_t next_alloc = (size_t)state->next_alloc;
  size_t nursery = (size_t)state->nursery;
  size_t stack_map_empty = (size_t)state->stack_map_empty;

  size_t total = (system_end - start + 512) / 1024;
  size_t available = (end - start + 512) / 1024;
  size_t used = (next_alloc - start + 512) / 1024;
  size_t stack = (next_alloc - stack_map_empty + 512) / 1024;
  size_t since_gc = (next_alloc - nursery + 512) / 1024;

  printf("start %p\n", state->heap.start);
  printf("system_end %p      (%zd kB total heap)\n", state->heap.system_end, total);
  printf("end %p             (%zd kB app heap)\n", state->heap.end, available);
  printf("next_alloc %p      (%zd kB used)\n", state->next_alloc, used);
  printf("stack_map_empty %p (%zd kB current call)\n", state->stack_map_empty, stack);
  printf("nursery %p         (%zd kB since last GC)\n", state->nursery, since_gc);
  printf("\n");
  printf("offsets %p\n", state->heap.offsets);
  printf("bitmap %p\n", state->heap.bitmap);
  printf("roots %p\n", state->roots);
  printf("stack_map %p\n", state->stack_map);
  printf("stack_depth %zd\n", state->stack_depth);
  printf("replay_ptr %p\n", state->replay_ptr);

  // print_bitmap();
}

void log_error(char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  print_heap();
  print_state();
  printf("Unit = %p\n", &Unit);
  printf("True = %p\n", &True);
  printf("False = %p\n", &False);
  fprintf(stderr, fmt, args);
  va_end(args);
  emscripten_run_script("debugger;");
}
