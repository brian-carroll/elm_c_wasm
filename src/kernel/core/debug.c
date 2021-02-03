#include "debug.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "./gc/internals.h"
#include "string.h"
#include "types.h"
#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#else
#define emscripten_run_script(x)
#endif

extern GcState gc_state;

char Debug_unknown_evaluator[] = "(?)";

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

#if !defined(__EMSCRIPTEN__) && defined(__linux__)
extern char etext, edata, end;  // memory regions, defined by linker
#endif

void pretty_print_child(int indent, void* p) {
  if (indent > 10) {
    printf("...etc...\n");
    return;
  }
  printf(FORMAT_PTR, p);
  for (int i = 0; i < indent; i++) {
    printf(" ");
  }
  Debug_prettyHelp(indent, p);
}

static void Debug_prettyHelp(int indent, void* p) {
  if (p == NULL) {
    printf("NULL\n");
    return;
  }
  if (indent > 10) {
    printf("...etc...\n");
    return;
  }

  ElmValue* v = p;

  int deeper = indent + 2;
  int deeper2 = indent + 4;

  // Avoid dereferencing addresses that are too high to be valid
  if ((size_t)p > (size_t)gc_state.heap.system_end) {
    printf("(out of bounds) %p\n", p);
    return;
  }
#if !defined(__EMSCRIPTEN__) && defined(__linux__)
  // Avoid dereferencing addresses that are too low to be valid
  // (e.g. unboxed integers used in some kernel code)
  // Not needed in Wasm since addresses down to zero don't segfault
  // It's OS-specific. Implement for Mac or Windows someday if needed.
  if ((size_t)p < (size_t)&etext) {
    printf("(unboxed integer) %zd\n", (size_t)p);
    return;
  }
#endif

  if (p == &True) {
    printf("True\n");
    return;
  }
  if (p == &False) {
    printf("False\n");
    return;
  }
  if (p == &Unit) {
    printf("()\n");
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
        size_t count = 0;
        for (Cons* cell = p; cell != &Nil; cell = cell->tail) {
          pretty_print_child(deeper, cell->head);
          if (count++ > 10) {
            printf("...\n");
            break;
          }
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
      for (int i = 0; i < custom_params(p) && i < 10; i++) {
        pretty_print_child(deeper, v->custom.values[i]);
      }
      break;
    }
    case Tag_Record: {
      printf("{\n");
      FieldGroup* fg = v->record.fieldgroup;
      u32 n_fields = v->header.size - (sizeof(Record) / SIZE_UNIT);
      for (int i = 0; i < n_fields && i < 10; i++) {
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
      for (int i = 0; i < n_fields && i < 10; i++) {
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
      for (int i = 0; i < v->closure.n_values && i < 10; i++) {
        pretty_print_child(deeper, v->closure.values[i]);
      }
      break;
    }
    case Tag_JsRef:
      printf("External JS object %d\n", v->js_ref.index);
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
  printf("  %s (%p)\n", label, p);
  pretty_print_child(2, p);
}

void print_value(void* p) {
  ElmValue* v = p;
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
      if (p == pTrue) {
        printf("True");
      } else if (p == pFalse) {
        printf("False");
      } else if (p == pUnit) {
        printf("Unit");
      } else if (p == pNil) {
        printf("Nil");
      } else {
        printf("Custom ctor: %d ", v->custom.ctor);
        for (size_t i = 0; i < custom_params(&v->custom); ++i) {
          printf("%p ", v->custom.values[i]);
        }
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
    case Tag_JsRef:
      printf("JsRef %d", v->js_ref.index);
      break;
    default:
      printf("<Corrupt data, tag=0x%x>", v->header.tag);
  }
}

void print_value_line(void* p) {
  ElmValue* v = p;
  printf("| " FORMAT_PTR " | " FORMAT_HEX " |  %c   |%5d | ",
      v,
      *((size_t*)v),
      is_marked(v) ? 'X' : ' ',
      v->header.size);
  print_value(v);
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
      print_value_line(v);
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
  mark(state, state->heap.start);
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

void print_live_sections() {
  GcState* state = &gc_state;
  printf("\n");
  printf("\nLive Sections:\n");
  printf("\n");

  int i=0;
  for (GcLiveSection* section = state->first_live_section;
       section <= state->current_live_section;
       section++, i++) {    

    if (section->start == section->end) {
      printf("%2d | " FORMAT_PTR " |    | %s | ", i, section->start, Debug_evaluator_name(section->evaluator));
      print_value(section->start);
      printf("\n");
    } else {
      char size[3];
      if (section->start < state->heap.end) {
        snprintf(size, 3, "%2zd", section->end - section->start);
      } else {
        snprintf(size, 3, "  ");
      }

      printf("%2d | %p | %s | %s |\n",
        i,
        section->start,
        size,
        Debug_evaluator_name(section->evaluator));
    }
  }
}

void print_state() {
  GcState* state = &gc_state;

  size_t start = (size_t)state->heap.start;
  size_t end = (size_t)state->heap.end;
  size_t system_end = (size_t)state->heap.system_end;
  size_t next_alloc = (size_t)state->next_alloc;
  size_t nursery = (size_t)state->nursery;

  size_t total = (system_end - start + 512) / 1024;
  size_t available = (end - start + 512) / 1024;
  size_t used = (next_alloc - start + 512) / 1024;
  size_t since_gc = (next_alloc - nursery + 512) / 1024;

  printf("\n");
  printf("%p start\n", state->heap.start);
  printf("%p system_end      (%zd kB total heap)\n", state->heap.system_end, total);
  printf("%p end             (%zd kB app heap)\n", state->heap.end, available);
  printf("%p next_alloc      (%zd kB used)\n", state->next_alloc, used);
  printf("%p nursery         (%zd kB since last GC)\n", state->nursery, since_gc);
  printf("\n");
  printf("%p offsets\n", state->heap.offsets);
  printf("%p bitmap\n", state->heap.bitmap);
  printf("%p roots\n", state->roots);
  printf("\n");
  printf("%p entry\n", state->entry);
  printf("%p current_live_section\n", state->current_live_section);
  printf("%p replay_live_section\n", state->replay_live_section);
  printf("%p first_live_section\n", state->first_live_section);
  printf("%p end_live_section\n", state->end_live_section);
  printf("%p replay\n", state->replay);
  printf("\n");

  // print_bitmap();
}

// Execute the JS `debugger` statement (browser devtools)
void Debug_pause() {
  emscripten_run_script("debugger;");
}

#ifdef __EMSCRIPTEN__
void log_error(char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  // print_heap();
  print_state();
  // fprintf(stderr, fmt, args);
  printf(fmt, args);
  va_end(args);
  // emscripten_run_script("debugger;");
  exit(EXIT_FAILURE);
}
#else
void log_error(char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  printf(fmt, args);
  va_end(args);
  exit(EXIT_FAILURE);
}
#endif

#if defined(DEBUG) && defined(DEBUG_LOG)
void log_debug(char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  printf(fmt, args);
  va_end(args);
}
#else
void log_debug(char* fmt, ...) {}
#endif
