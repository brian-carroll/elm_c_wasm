#include "./debug.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "./gc-internals.h"
#include "./string.h"
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

static void print_indent(int indent) {
  for (int i = 0; i < indent; i++) {
    printf(" ");
  }
}

static void Debug_prettyHelp(int indent, void* p);

void pretty_print_child(int indent, void* p) {
  printf(FORMAT_PTR, p);
  for (int i = 0; i < indent; i++) {
    printf(" ");
  }
  Debug_prettyHelp(indent, p);
}

static void Debug_prettyHelp(int indent, void* p) {
  ElmValue* v = p;

  int deeper = indent + 2;
  int deeper2 = indent + 4;

  if (p == &True) {
    printf("True\n");
    return;
  }
  if (p == &False) {
    printf("False\n");
    return;
  }
  if (p == &Unit) {
    printf("Unit\n");
    return;
  }

  switch (v->header.tag) {
    case Tag_Int:
      printf("Int %d\n", v->elm_int.value);
      break;
    case Tag_Float:
      printf("Float %f\n", v->elm_float.value);
      break;
    case Tag_Char:
      printf("Char 0x%8x\n", v->elm_char.value);
      break;
    case Tag_String: {
      printf("String \"");
      ElmString16* s = p;
      for (size_t i = 0; i < code_units(s); i++) {
        u16 unit = s->words16[i];
        if (unit) {
          if (unit < 128) {
            printf("%c", (char)unit);
          } else {
            printf("#");
          }
        }
      }
      printf("\"\n");
      break;
    }
    case Tag_List: {
      if (p == pNil) {
        printf("[]\n");
      } else {
        printf("[\n");
        for (Cons* cell = p; cell != &Nil; cell = cell->tail) {
          pretty_print_child(deeper, cell->head);
        }
        print_indent(FORMAT_PTR_LEN + indent);
        printf("]\n");
      }
      break;
    }
    case Tag_Tuple2: {
      printf("(\n");
      pretty_print_child(deeper, v->tuple2.a);
      pretty_print_child(deeper, v->tuple2.b);
      print_indent(FORMAT_PTR_LEN + indent);
      printf(")\n");
      break;
    }
    case Tag_Tuple3: {
      printf("(\n");
      pretty_print_child(deeper, v->tuple3.a);
      pretty_print_child(deeper, v->tuple3.b);
      pretty_print_child(deeper, v->tuple3.c);
      print_indent(FORMAT_PTR_LEN + indent);
      printf(")\n");
      break;
    }
    case Tag_Custom: {
      u32 ctor = v->custom.ctor;
      if (ctor < Debug_ctors_size) {
        size_t offset = sizeof("CTOR");
        printf("%s\n", &Debug_ctors[ctor][offset]);
      } else {
        printf("Custom (ctor %d)\n", ctor);
      }
      for (int i = 0; i < custom_params(p); i++) {
        pretty_print_child(deeper, v->custom.values[i]);
      }
      break;
    }
    case Tag_Record: {
      printf("{\n");
      FieldGroup* fg = v->record.fieldgroup;
      u32 n_fields = v->header.size - (sizeof(Record) / SIZE_UNIT);
      for (int i = 0; i < n_fields; i++) {
        void* child = v->custom.values[i];
        printf(FORMAT_PTR, child);
        print_indent(deeper);
        u32 field = fg->fields[i];
        if (field < Debug_fields_size) {
          size_t offset = sizeof("FIELD");
          printf("%s = ", &Debug_fields[field][offset]);
        } else {
          printf("%d = ", field);
        }
        Debug_prettyHelp(deeper2, child);
      }
      print_indent(FORMAT_PTR_LEN + indent);
      printf("}\n");
      break;
    }
    case Tag_FieldGroup: {
      printf("FieldGroup\n");
      u32 n_fields = v->fieldgroup.size;
      for (int i = 0; i < n_fields; i++) {
        print_indent(deeper);
        u32 field = v->fieldgroup.fields[i];
        if (field < Debug_fields_size) {
          size_t offset = sizeof("FIELD");
          printf("%s\n", &Debug_fields[field][offset]);
        } else {
          printf("%d\n", field);
        }
      }
      break;
    }
    case Tag_Closure: {
      char* name;
      if (v->closure.max_values != NEVER_EVALUATE) {
        name = Debug_evaluator_name(v->closure.evaluator);
      } else {
        size_t js_value_id = (size_t)v->closure.evaluator;
        if (js_value_id < Debug_jsValues_size) {
          name = Debug_jsValues[js_value_id];
        } else {
          name = "(unknown)";
        }
      }
      printf("Closure %s\n", name);
      for (int i = 0; i < v->closure.n_values; i++) {
        pretty_print_child(deeper, v->closure.values[i]);
      }
      break;
    }
    case Tag_GcException:
      printf("GC marker (Exception)\n");
      break;
    case Tag_GcStackEmpty:
      printf("GC marker (Stack Empty)\n");
      break;
    case Tag_GcStackPush:
      printf("GC marker (Stack Push)\n");
      break;
    case Tag_GcStackPop:
      printf("GC marker (Stack Pop)\n");
      break;
    case Tag_GcStackTailCall:
      printf("GC marker (Stack Tail Call)\n");
      break;
    default:
      printf("CORRUPTED!! tag %x size %d\n", v->header.tag, v->header.size);
      break;
  }
}

void Debug_pretty(const char* label, void* p) {
  printf("\n");
  for (int i = 0; i < FORMAT_PTR_LEN; i++) {
    printf("-");
  }
  printf("  %s\n", label);
  pretty_print_child(2, p);
}

void print_value(void* p) {
  ElmValue* v = p;
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
    case Tag_FieldGroup: {
      printf("Fieldgroup");
      break;
    }
    case Tag_Closure: {
      if (v->closure.max_values != NEVER_EVALUATE) {
        printf("Closure (%s) n_values: %d max_values: %d values: ",
            Debug_evaluator_name(v->closure.evaluator),
            v->closure.n_values,
            v->closure.max_values);
      } else {
        size_t js_value_id = (size_t)v->closure.evaluator;
        char* name =
            js_value_id < Debug_jsValues_size ? Debug_jsValues[js_value_id] : "unknown";
        printf("JS Closure (%s) n_values: %d values: ", name, v->closure.n_values);
      }
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
  }
  printf("\n");
}

void print_heap_range(size_t* start, size_t* end) {
#ifdef TARGET_64BIT
  printf("|    Address     |       Hex        | Mark | Size | Value\n");
  printf("| -------------- | ---------------- | ---- | ---- | -----\n");
#else
  printf("| Address  |   Hex    | Mark | Size | Value\n");
  printf("| -------- | -------- | ---- | ---- | -----\n");
#endif

  size_t* first_value = start;
  size_t* next_value = first_value;
  for (size_t* p = first_value; p < end; p++) {
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
        if (p >= end) break;
      }
      ElmValue* v = (ElmValue*)p;
      print_value(v);
      if (v->header.size > 0 && v->header.size < 102400) {
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

void print_value_full(void* p) {
  Header* h = p;
  size_t* words = p;
  print_heap_range(words, words + h->size);
}

void print_heap() {
  GcState* state = &gc_state;
  print_heap_range(state->heap.start, state->next_alloc);
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
  printf("\n");

  // print_bitmap();
}

// Execute the JS `debugger` statement (browser devtools)
void Debug_pause() {
  emscripten_run_script("debugger;");
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
