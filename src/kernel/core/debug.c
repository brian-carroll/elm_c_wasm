#include "core.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../json/json.h"
#include "./gc/internals.h"
#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#else
#define emscripten_run_script(x)
#endif

extern GcState gc_state;
extern char stack_flags[GC_STACK_MAP_SIZE];
char Debug_unknown_evaluator[] = "(?)";


// =======================================================================
//
//                PRETTY PRINTING - WITH MEMORY ADDRESSES
//
// =======================================================================

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
      } else if (ctor == JSON_VALUE_OBJECT) {
        printf("(JSON Object)");
      } else if (ctor == JSON_VALUE_ARRAY) {
        printf("(JSON Array)");
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
  if (!p) {
    printf("NULL");
    return;
  }
  if (!sanity_check(p)) {
    printf("(corrupt data?)");
    return;
  }
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
      if (p == pNil) {
        printf("Nil");
      } else {
        printf("Cons head: %p tail: %p", v->cons.head, v->cons.tail);
      }
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
      } else {
        u32 ctor = v->custom.ctor;
        if (ctor < Debug_ctors_size) {
          printf("Custom %s ", Debug_ctors[ctor] + 5);
        } else {
          printf("Custom ctor: %d ", ctor);
        }
        for (size_t i = 0; i < custom_params(&v->custom); ++i) {
          printf("%p ", v->custom.values[i]);
        }
      }
      break;
    case Tag_Record: {
      printf("Record (");
      print_value(v->record.fieldgroup);
      printf(") values: ");
      size_t header_kids = (size_t)(v->header.size) - (sizeof(Record) / sizeof(void*));
      for (size_t i = 0; i < header_kids; ++i) {
        printf("%p ", v->record.values[i]);
      }
      break;
    }
    case Tag_FieldGroup: {
      printf("Fieldgroup ");
      FieldGroup* fg = &v->fieldgroup;
      for (u32 i = 0; i < fg->size; ++i) {
        u32 fieldId = fg->fields[i];
        char* fieldName = Debug_fields[fieldId];
        printf("%s ", &fieldName[6]);
      }
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


// =======================================================================
//
//                      GARBAGE COLLECTOR DEBUG
//
// =======================================================================

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
#ifdef _WIN32
  printf("|     Address      |       Hex        | Mark | Size | Value\n");
  printf("| ---------------- | ---------------- | ---- | ---- | -----\n");
#elif TARGET_64BIT
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
      if (sanity_check(v)) {
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
  // mark(state, state->heap.start); // beware of infinite loop
  print_heap_range(state->heap.start, state->next_alloc);
}


void print_bitmap(const char* function, const char* filename, int line_no) {
  GcHeap* heap = &gc_state.heap;
  size_t* bitmap = gc_state.heap.bitmap;

  // find last non-zero word in the bitmap
  size_t bitmap_size = heap->system_end - heap->bitmap;
  size_t last_word = bitmap_size;
  while (bitmap[--last_word] == 0)
    ;

  printf("Bitmap at %s:%d (%s)\n", filename, line_no, function);
  size_t* p = heap->start;
  for (size_t word = 0; word <= last_word && word < bitmap_size;
       word++, p += GC_WORD_BITS) {
    size_t value = bitmap[word];
    char s[GC_WORD_BITS + 1];
    for (size_t bit = 0, mask = 1; bit < GC_WORD_BITS; bit++, mask <<= 1) {
      if (value & mask) {
        s[bit] = 'X';
      } else {
        size_t* addr = p + bit;
        s[bit] = (*addr) ? '|' : '-';
      }
    }
    s[GC_WORD_BITS] = 0;
    printf("%3zd | " FORMAT_PTR " %s\n", word, p, s);
  }
  printf("\n");
}

void print_stack_map() {
  GcStackMap* sm = &gc_state.stack_map;
  printf("\n");
  printf("\nStack map:\n");
  printf("\n");

  GcStackMapIndex top = sm->index;
  for (u32 i = 0; i < top; ++i) {
    void* value = stack_values[i];
    char flag = stack_flags[i];
    if (flag == 'F') {
      char* eval_name = value ? Debug_evaluator_name(value) : "NULL";
      printf("-----------------\n");
      printf("%2d | %c | " FORMAT_PTR " | %s\n", i, flag, value, eval_name);
    } else {
      printf("%2d | %c | " FORMAT_PTR " | ", i, flag, value);
      print_value(value);
      printf("\n");
    }
  }
}


void print_state() {
  GcState* state = &gc_state;

  size_t start = (size_t)state->heap.start;
  size_t end = (size_t)state->heap.end;
  size_t system_end = (size_t)state->heap.system_end;
  size_t next_alloc = (size_t)state->next_alloc;
  size_t end_of_old_gen = (size_t)state->end_of_old_gen;

  size_t total = (system_end - start + 512) / 1024;
  size_t available = (end - start + 512) / 1024;
  size_t used = (next_alloc - start + 512) / 1024;
  size_t since_gc = (next_alloc - end_of_old_gen + 512) / 1024;

  printf("\n");
  printf("%p start\n", state->heap.start);
  printf("%p system_end      (%zd kB total heap)\n", state->heap.system_end, total);
  printf("%p end             (%zd kB app heap)\n", state->heap.end, available);
  printf("%p next_alloc      (%zd kB used)\n", state->next_alloc, used);
  printf("%p end_of_old_gen  (%zd kB since last GC)\n", state->end_of_old_gen, since_gc);
  printf("\n");
  printf("%p offsets\n", state->heap.offsets);
  printf("%p bitmap\n", state->heap.bitmap);
  printf("%p roots\n", state->roots);
  printf("\n");
  printf("%d stack_index\n", state->stack_map.index);
  printf("\n");

  // PRINT_BITMAP();
}


void format_ptr_diff_size(char* buffer, size_t buf_size, void* start, void* end) {
  size_t bytes = end - start;
  size_t words = bytes / sizeof(void*);
  format_mem_size(buffer, buf_size, words);
}


void format_mem_size(char* buffer, size_t buf_size, size_t words) {
  size_t bytes = words * sizeof(void*);
  char* suffix;
  f32 amount;
  if (bytes < 1024) {
    suffix = "B";
    amount = (f32)bytes;
  } else if (bytes < 1024 * 1024) {
    suffix = "kB";
    amount = (f32)bytes / 1024;
  } else {
    suffix = "MB";
    amount = (f32)bytes / (1024 * 1024);
  }
  char* format = (amount < 10) ? "%.1f %s" : "%.0f %s";
  snprintf(buffer, buf_size, format, amount, suffix);
}


void print_ptr_diff_size(void* start, void* end) {
  char buf[100];
  format_ptr_diff_size(buf, sizeof(buf), start, end);
  printf("%s", buf);
}


void print_mem_size(size_t words) {
  char buf[100];
  format_mem_size(buf, sizeof(buf), words);
  printf("%s", buf);
}

#if PERF_TIMER_ENABLED
void print_gc_perf(void* untyped_perf_data, bool major) {
  struct gc_perf_data* perf_data = untyped_perf_data;
  char size_before[20];
  char size_after[20];
  format_mem_size(size_before, sizeof(size_before), perf_data->size);
  format_ptr_diff_size(size_after, sizeof(size_after), gc_state.heap.start, gc_state.next_alloc);

  bool dummy = true;
  if (dummy) printf("GC performance:\n");
  if (dummy) printf("  before:  %s\n", size_before);
  if (dummy) printf("  after:   %s\n", size_after);
  if (dummy) printf("  mark:    %5lld k cycles\n", (perf_data->marked - perf_data->start) / 1000);
  if (major) printf("  compact: %5lld k cycles\n", (perf_data->compacted - perf_data->marked) / 1000);
  if (dummy) printf("  sweep:   %5lld k cycles\n", (perf_data->swept - perf_data->marked) / 1000);
  if (major) printf("  jsRefs:  %5lld k cycles\n", (perf_data->jsRefs - perf_data->compacted) / 1000);
}
#endif

// =======================================================================
//
//                      C DEBUGGING
//
// =======================================================================


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


// ========================================================
//
//      Elm functions
//
// ========================================================

#define GC_NOT_FULL NULL;
size_t toString_alloc_chunk_words;

struct string_builder {
  ElmString16* s;
  u16* cursor;
  u16* end;
};
typedef struct string_builder StringBuilder;

// assumes nothing else gets allocated while stringifying
static void grow(StringBuilder* sb) {
  GC_allocate(false, toString_alloc_chunk_words);
  sb->end = GC_allocate(false, 0);
  sb->s->header.size += toString_alloc_chunk_words;
  if (toString_alloc_chunk_words < 256) toString_alloc_chunk_words *= 2;
  return;
}


static void ensure_space(size_t need, StringBuilder* sb) {
  while (sb->end - sb->cursor < need) {
    grow(sb);
  }
}


static void copy_ascii(char* src, StringBuilder* sb) {
  char* from = src;
  u16* to = sb->cursor;

  for (; *from; to++, from++) {
    if (to >= sb->end) {
      grow(sb);
    }
    *to = *from;
  }

  sb->cursor = to;
}


void Debug_toStringHelp(int depth, void* p, StringBuilder* sb) {
  ElmValue* v = p;
  char ascii_buf[25];

  if (!depth) {
    copy_ascii("...", sb);
    return;
  }

  switch (v->header.tag) {
    case Tag_Int: {
      snprintf(ascii_buf, sizeof(ascii_buf), "%d", v->elm_int.value);
      copy_ascii(ascii_buf, sb);
      return;
    }
    case Tag_Float: {
      snprintf(ascii_buf, sizeof(ascii_buf), "%.16g", v->elm_float.value);
      copy_ascii(ascii_buf, sb);
      return;
    }
    case Tag_Char: {
      u16 lower = v->elm_char.words16[0];
      u16 upper = v->elm_char.words16[1];
      ensure_space(upper ? 4 : 3, sb);
      u16* write = sb->cursor;
      *write++ = '\'';
      *write++ = lower;
      if (upper) *write++ = upper;
      *write++ = '\'';
      sb->cursor = write;
      return;
    }
    case Tag_String: {
      size_t len = code_units(&v->elm_string16);
      ensure_space(len + 2, sb);
      u16* write = sb->cursor;
      *write++ = '"';
      for (size_t i = 0; i < len; ++i) {
        *write++ = v->elm_string16.words16[i];
      }
      *write++ = '"';
      sb->cursor = write;
      return;
    }
    case Tag_List: {
      copy_ascii("[", sb);
      for (Cons* list = &v->cons; list != pNil; list = list->tail) {
        Debug_toStringHelp(depth - 1, list->head, sb);
        if (list->tail != pNil) copy_ascii(", ", sb);
      }
      copy_ascii("]", sb);
      return;
    }
    case Tag_Tuple2: {
      Tuple2* t = &v->tuple2;
      copy_ascii("(", sb);
      Debug_toStringHelp(depth - 1, t->a, sb);
      copy_ascii(", ", sb);
      Debug_toStringHelp(depth - 1, t->b, sb);
      copy_ascii(")", sb);
      return;
    }
    case Tag_Tuple3: {
      Tuple3* t = &v->tuple3;
      copy_ascii("(", sb);
      Debug_toStringHelp(depth - 1, t->a, sb);
      copy_ascii(", ", sb);
      Debug_toStringHelp(depth - 1, t->b, sb);
      copy_ascii(", ", sb);
      Debug_toStringHelp(depth - 1, t->c, sb);
      copy_ascii(")", sb);
      return;
    }
    case Tag_Custom: {
      Custom* c = &v->custom;
      if (c == &True) {
        copy_ascii("True", sb);
        return;
      } else if (c == &False) {
        copy_ascii("False", sb);
        return;
      } else if (c == &Unit) {
        copy_ascii("()", sb);
        return;
      }
      char* ctor;
      if (c->ctor < Debug_ctors_size) {
        ctor = Debug_ctors[c->ctor] + 5;
      } else if (c->ctor == JSON_VALUE_OBJECT) {
        ctor = "<JSON Object>";
      } else if (c->ctor == JSON_VALUE_ARRAY) {
        ctor = "<JSON Array>";
      } else {
        ctor = "<unknown ctor>";
      }
      copy_ascii(ctor, sb);
      copy_ascii(" ", sb);
      int len = custom_params(c);
      for (int i = 0; i < len; ++i) {
        Debug_toStringHelp(depth - 1, c->values[i], sb);
        if (i != len - 1) copy_ascii(" ", sb);
      }
      return;
    }
    case Tag_Record: {
      Record* r = &v->record;
      FieldGroup* fg = r->fieldgroup;
      u32 size = fg->size;
      copy_ascii("{", sb);
      for (int i = 0; i < size; ++i) {
        char* field = Debug_fields[fg->fields[i]];
        copy_ascii(field, sb);
        copy_ascii(": ", sb);
        Debug_toStringHelp(depth - 1, r->values[i], sb);
        if (i != size - 1) copy_ascii(",", sb);
      }
      return;
    }
    case Tag_FieldGroup: {
      copy_ascii("<fieldgroup>", sb);
      return;
    }
    case Tag_Closure: {
      copy_ascii("<function>", sb);
      return;
    }
    case Tag_JsRef: {
      copy_ascii("<JavaScript>", sb);
      return;
    }
    default: {
      copy_ascii("<unknown>", sb);
      return;
    }
  }
}

void* eval_Debug_toString(void* args[]) {
  void* value = args[0];
  toString_alloc_chunk_words = 16;
  size_t len = (toString_alloc_chunk_words - 1) * SIZE_UNIT / sizeof(u16);
  ElmString16* str = newElmString16(len);
  StringBuilder sb = {
      .s = str,
      .cursor = str->words16,
      .end = str->words16 + len,
  };

  Debug_toStringHelp(5, value, &sb);

  // Shrink the string
  ptrdiff_t cursor_addr = (ptrdiff_t)(sb.cursor);
  ptrdiff_t aligned_cursor_addr = (cursor_addr + SIZE_UNIT - 1) & (-SIZE_UNIT);
  ptrdiff_t size = (aligned_cursor_addr - (ptrdiff_t)str) / SIZE_UNIT;
  str->header.size = (u32)size;

  // Give back unused memory to the allocator
  ptrdiff_t end_addr = (ptrdiff_t)(sb.end);
  ptrdiff_t negative_alloc = aligned_cursor_addr - end_addr;
  GC_allocate(false, negative_alloc / SIZE_UNIT);

  return str;
}
Closure Debug_toString = {
    .header = HEADER_CLOSURE(0),
    .max_values = 1,
    .evaluator = eval_Debug_toString,
};


void* eval_Debug_log(void* args[]) {
  ElmString16* label = args[0];
  void* value = args[1];

  size_t label_len = code_units(label);
  for (size_t i = 0; i < label_len; ++i) {
    putchar(label->words16[i]);
  }

  putchar(':');
  putchar(' ');

  ElmString16* s = eval_Debug_toString(args + 1);
  size_t s_len = code_units(s);
  for (size_t i = 0; i < s_len; ++i) {
    putchar(s->words16[i]);
  }
  putchar('\n');

  return value;
}
Closure Debug_log = {
    .header = HEADER_CLOSURE(0),
    .max_values = 2,
    .evaluator = eval_Debug_log,
};


void* eval_Debug_todo(void* args[]) {
  ElmString16* message = args[0];
  size_t len = code_units(message);
  for (size_t i = 0; i < len; ++i) {
    putchar(message->words16[i]);
  }
  putchar('\n');
  exit(EXIT_FAILURE);
  return NULL;
}
Closure Debug_todo = {
    .header = HEADER_CLOSURE(0),
    .max_values = 1,
    .evaluator = eval_Debug_todo,
};
