#include "../gc/internals.h"

// =======================================================================
//
//                      GARBAGE COLLECTOR DEBUG
//
// =======================================================================

extern GcState gc_state;
extern char stack_flags[GC_STACK_MAP_SIZE];

#ifdef DEBUG

static size_t target_offsets[] = {
    // 0x9b820,
    // 0x40bd0,
};

void Debug_print_offset(const char* label, void* p) {
  size_t offset = (u8*)p - (u8*)gc_state.heap.start;
  safe_printf("%s @ offset 0x%zx\n", label, offset);
}

bool Debug_is_target_addr(void* p) {
  if (!ARRAY_LEN(target_offsets)) {
    return false;
  }
  size_t offset = (u8*)p - (u8*)gc_state.heap.start;
  for (int i = 0; i < ARRAY_LEN(target_offsets); i++) {
    if (offset == target_offsets[i]) {
      return true;
    }
  }
  return false;
}

bool Debug_is_target_in_range(void* from, void* to) {
  if (!ARRAY_LEN(target_offsets)) {
    return false;
  }
  for (void* p = from; p < to; p += sizeof(void*)) {
    if (Debug_is_target_addr(p)) {
      return true;
    }
  }
  return false;
}
#endif


bool is_marked(void* p) {
  GcHeap* heap = &gc_state.heap;
  GcBitmapIter iter = ptr_to_bitmap_iter(heap, p);
  return (heap->bitmap[iter.index] & iter.mask) != 0;
}


void print_value(void* p) {
  if (!p) {
    safe_printf("NULL");
    return;
  }
  if (!sanity_check(p)) {
    safe_printf("(corrupt data? " FORMAT_HEX ")", *(size_t*)p);
    return;
  }
  ElmValue* v = p;
  switch (v->header.tag) {
    case Tag_Int:
      safe_printf("Int %d", (i32)v->elm_int.int_f64);
      break;
    case Tag_Float:
      safe_printf("Float %f", v->elm_float.value);
      break;
    case Tag_Char:
      safe_printf("Char 0x%x", v->elm_char.value);
      if (v->elm_char.value < 128) {
        safe_printf(" '%c'", v->elm_char.value);
      }
      break;
    case Tag_String: {
      char buf[128];
      ElmString* s = &v->elm_string;
      size_t len = code_units(s);
      size_t i = 0;
      for (; i < len && i < sizeof(buf); i++) {
        buf[i] = s->words16[i];
      }
      buf[i] = 0;
      safe_printf("String \"%s\"", buf);
      break;
    }
    case Tag_List:
      if (p == pNil) {
        safe_printf("Nil");
      } else {
        safe_printf("Cons head: %p tail: %p", v->cons.head, v->cons.tail);
      }
      break;
    case Tag_Tuple2:
      safe_printf("Tuple2 a: %p b: %p", v->tuple2.a, v->tuple2.b);
      break;
    case Tag_Tuple3:
      safe_printf("Tuple3 a: %p b: %p c: %p", v->tuple3.a, v->tuple3.b, v->tuple3.c);
      break;
    case Tag_Custom:
      if (p == pTrue) {
        safe_printf("True");
      } else if (p == pFalse) {
        safe_printf("False");
      } else if (p == pUnit) {
        safe_printf("Unit");
      } else {
        u32 ctor = v->custom.ctor;
        if (ctor < Debug_ctors_size) {
          safe_printf("Custom %s ", Debug_ctors[ctor] + 5);
        } else {
          safe_printf("Custom ctor: %d ", ctor);
        }
        for (size_t i = 0; i < custom_params(&v->custom); ++i) {
          safe_printf("%p ", v->custom.values[i]);
        }
      }
      break;
    case Tag_Record: {
      safe_printf("Record (");
      print_value(v->record.fieldgroup);
      safe_printf(") values: ");
      size_t header_kids = (size_t)(v->header.size) - (sizeof(Record) / sizeof(void*));
      for (size_t i = 0; i < header_kids; ++i) {
        safe_printf("%p ", v->record.values[i]);
      }
      break;
    }
    case Tag_FieldGroup: {
      safe_printf("Fieldgroup ");
      FieldGroup* fg = &v->fieldgroup;
      for (u32 i = 0; i < fg->size; ++i) {
        u32 fieldId = fg->fields[i];
        char* fieldName = Debug_fields[fieldId];
        safe_printf("%s ", &fieldName[6]);
      }
      break;
    }
    case Tag_Closure: {
      safe_printf("Closure (%s) n_values: %d max_values: %d values: ",
          Debug_evaluator_name(v->closure.evaluator),
          v->closure.n_values,
          v->closure.max_values);
      size_t header_kids = (size_t)(v->header.size) - (sizeof(Closure) / sizeof(void*));
      for (size_t i = 0; i < header_kids; ++i) {
        safe_printf("%p ", v->closure.values[i]);
      }
      break;
    }
    case Tag_JsRef:
      safe_printf("JsRef %d", v->js_ref.id);
      break;
    case Tag_Process: {
      Process* p = (Process*)v;
      u32 stack_size = 0;
      for (ProcessStack* stack = p->stack; stack; stack = stack->rest) {
        stack_size++;
      }
      u32 mailbox_size = 0;
      for (Cons* cell = p->mailbox.front; cell != &Nil; cell = cell->tail) {
        mailbox_size++;
      }
      safe_printf("Process id %d, stack size %d, mailbox size %d",
          p->id,
          stack_size,
          mailbox_size);
      break;
    }
    case Tag_Task: {
      safe_printf("Task");
      break;
    }
    default:
      safe_printf("<Corrupt data, tag=0x%x>", v->header.tag);
  }
}


void print_value_line(void* p) {
  ElmValue* v = p;
  safe_printf("| " FORMAT_PTR " | " FORMAT_HEX " |  %c   |%5d | ",
      v,
      *((size_t*)v),
      is_marked(v) ? 'X' : ' ',
      v->header.size);
  print_value(v);
  safe_printf("\n");
}


void print_heap_range(size_t* start, size_t* end) {
#if TARGET_64BIT
  safe_printf("|     Address      |       Hex        | Mark | Size | Value\n");
  safe_printf("| ---------------- | ---------------- | ---- | ---- | -----\n");
#else
  safe_printf("| Address  |   Hex    | Mark | Size | Value\n");
  safe_printf("| -------- | -------- | ---- | ---- | -----\n");
#endif

  size_t* first_value = start;
  size_t* next_value = first_value;
  for (size_t* p = first_value; p < end; p++) {
    if (p == next_value) {
      if (*p == 0) {
        // summarize big chunks of zeros
        while (*p == 0)
          p++;
        safe_printf("| " FORMAT_PTR " | " FORMAT_HEX " |      |%5zd | (zeros)\n",
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
      safe_printf("| " FORMAT_PTR " | " FORMAT_HEX " |  %c   |      |\n",
          p,
          *p,
          is_marked(p) ? 'X' : ' ');
    }
  }
}


void print_value_full(void* p) {
  Header* h = p;
  size_t* words = p;
  size_t size = sanity_check(p) ? h->size : 4;
  print_heap_range(words, words + size);
}


void print_heap() {
  GcState* state = &gc_state;
  // mark(state, state->heap.start); // beware of infinite loop
  print_heap_range(state->heap.start, state->next_alloc);
}


void print_bitmap(const char* function, const char* filename, int line_no) {
  GcHeap* heap = &gc_state.heap;
  u64* bitmap = gc_state.heap.bitmap;

  // find last non-zero word in the bitmap
  size_t last_word = heap->bitmap_size;
  while (bitmap[--last_word] == 0 && last_word > 0)
    ;

  safe_printf("Bitmap at %s:%d (%s)\n", filename, line_no, function);
  for (size_t word = 0; word <= last_word; word++) {
    size_t* p = heap->start + (word * 64);
    u64 value = bitmap[word];
    char s[65];

    u64 mask = 1;
    for (size_t bit = 0; bit < 64; bit++, mask <<= 1) {
      size_t* addr = p + bit;
      if (addr == heap->end) {
        s[bit] = '\0';
        break;
      };
      if (value & mask) {
        s[bit] = 'X';
      } else {
        s[bit] = (*addr) ? '|' : '-';
      }
    }
    s[64] = '\0';
    safe_printf("%4zd | " FORMAT_PTR " %s\n", word, p, s);
  }
  safe_printf("\n");
}


void print_stack_map() {
  GcStackMap* sm = &gc_state.stack_map;
  safe_printf("\n");
  safe_printf("\nStack map:\n");
  safe_printf("\n");

  GcStackMapIndex top = sm->index;
  for (u32 i = 0; i < top; ++i) {
    ElmValue* value = stack_values[i];
    char flag = stack_flags[i];
    if (flag == 'F') {
      safe_printf("-----------------\n");
      safe_printf("%2d | %c | " FORMAT_PTR " |\n", i, flag, value);
      i++;
      value = stack_values[i];
      flag = stack_flags[i];
      char* eval_name = value ? Debug_evaluator_name(value) : "NULL";
      safe_printf("%2d | %c | " FORMAT_PTR " | ", i, flag, value);
      switch (flag) {
        case 'J':
          safe_printf("JsRef %d\n", value->js_ref.id);
          break;
        case 'C':
          safe_printf("Closure %s\n", eval_name);
          break;
        case 'I':
          safe_printf("Global initializer\n"); // TODO compiler could generate init names
          break;
        case 'W':
          safe_printf("%s Wrapper args\n", eval_name);
          break;
        default:
          assert(false);
      }
    } else {
      safe_printf("%2d | %c | " FORMAT_PTR " | ", i, flag, value);
      print_value(value);
      safe_printf("\n");
    }
  }
}


void print_state() {
  GcState* state = &gc_state;
  GcHeap* heap = &gc_state.heap;

  size_t total = ((u8*)heap->system_end - (u8*)heap->start + 512) / 1024;
  size_t available = ((u8*)heap->end - (u8*)heap->start + 512) / 1024;
  size_t used = ((u8*)state->next_alloc - (u8*)heap->start + 512) / 1024;
  size_t old_gen = ((u8*)state->end_of_old_gen - (u8*)heap->start + 512) / 1024;

  size_t temp_bits = sizeof(heap->gc_temp[0]) * 8;
  size_t bitmap_bits = sizeof(heap->bitmap[0]) * 8;

  int n_roots = 0;
  for (Cons* r = state->roots; r != pNil; r = r->tail) {
    n_roots++;
  }

  safe_printf("\n");
  safe_printf("%p start\n", heap->start);
  safe_printf("%p system_end      (%zd kB total heap)\n", heap->system_end, total);
  safe_printf("%p end             (%zd kB app heap)\n", heap->end, available);
  safe_printf("%p next_alloc      (%zd kB used)\n", state->next_alloc, used);
  safe_printf("%p end_of_old_gen  (%zd kB)\n", state->end_of_old_gen, old_gen);
  safe_printf("\n");
  safe_printf("%p gc_temp (%zd x %db)\n", heap->gc_temp, heap->gc_temp_size, temp_bits);
  safe_printf("%p bitmap (%zd x %db)\n", heap->bitmap, heap->bitmap_size, bitmap_bits);
  safe_printf("%p roots (%d)\n", state->roots, n_roots);
  safe_printf("%8d stack_index\n", state->stack_map.index);
  safe_printf("\n");
}


void format_ptr_diff_size(char* buffer, size_t buf_size, void* start, void* end) {
  size_t bytes = (char*)end - (char*)start;
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
  char* format = (amount < 10) ? "%.1f %s" : "%'.0f %s";
  stbsp_snprintf(buffer, buf_size, format, amount, suffix);
}


void print_ptr_diff_size(void* start, void* end) {
  char buf[100];
  format_ptr_diff_size(buf, sizeof(buf), start, end);
  safe_printf("%s", buf);
}


void print_mem_size(size_t words) {
  char buf[100];
  format_mem_size(buf, sizeof(buf), words);
  safe_printf("%s", buf);
}


// =======================================================================
//
//                PRETTY PRINTING - WITH MEMORY ADDRESSES
//
// =======================================================================

static void print_indent(int indent) {
  for (int i = 0; i < indent; i++) {
    safe_printf(" ");
  }
}

static void Debug_prettyHelp(int indent, void* p);

#if !defined(__EMSCRIPTEN__) && defined(__linux__)
extern char etext, edata, end;  // memory regions, defined by linker
#endif

void pretty_print_child(int indent, void* p) {
  if (indent > 20) {
    safe_printf("...etc...\n");
    return;
  }
  safe_printf(FORMAT_PTR, p);
  for (int i = 0; i < indent; i++) {
    safe_printf(" ");
  }
  Debug_prettyHelp(indent, p);
}

static void Debug_prettyHelp(int indent, void* p) {
  if (p == NULL) {
    safe_printf("NULL\n");
    return;
  }
  if (indent > 20) {
    safe_printf("...etc...\n");
    return;
  }

  ElmValue* v = p;

  int deeper = indent + 2;
  int deeper2 = indent + 4;

  // Avoid dereferencing addresses that are too high to be valid
  if ((size_t)p > (size_t)gc_state.heap.system_end) {
    safe_printf("(out of bounds) %p\n", p);
    return;
  }
#if !defined(__EMSCRIPTEN__) && defined(__linux__)
  // Avoid dereferencing addresses that are too low to be valid
  // (e.g. unboxed integers used in some kernel code)
  // Not needed in Wasm since addresses down to zero don't segfault
  // It's OS-specific. Implement for Mac or Windows someday if needed.
  if ((size_t)p < (size_t)&etext) {
    safe_printf("(unboxed integer) %zd\n", (size_t)p);
    return;
  }
#endif

  if (p == &True) {
    safe_printf("True\n");
    return;
  }
  if (p == &False) {
    safe_printf("False\n");
    return;
  }
  if (p == &Unit) {
    safe_printf("()\n");
    return;
  }
  if (p == &Json_encodeNull) {
    safe_printf("Json.null\n");
    return;
  }

  switch (v->header.tag) {
    case Tag_Int:
      safe_printf("Int %d\n", v->elm_int.int_f64);
      break;
    case Tag_Float:
      safe_printf("Float %f\n", v->elm_float.value);
      break;
    case Tag_Char:
      safe_printf("Char 0x%8x\n", v->elm_char.value);
      break;
    case Tag_String: {
      ElmString* s = p;
      char buf[100];
      size_t i = 0;
      for (; i < code_units(s) && i < sizeof(buf) - 1; i++) {
        u16 chr = s->words16[i];
        if (chr) {
          if (chr < 128) {
            buf[i] = chr;
          } else {
            buf[i] = '#';
          }
        }
      }
      buf[i] = '\0';
      safe_printf("String \"%s\"\n", buf);
      break;
    }
    case Tag_List: {
      if (p == pNil) {
        safe_printf("[]\n");
      } else {
        safe_printf("[\n");
        size_t count = 0;
        for (Cons* cell = p; cell != &Nil; cell = cell->tail) {
          pretty_print_child(deeper, cell->head);
          if (count++ > 10) {
            safe_printf("...\n");
            break;
          }
        }
        print_indent(FORMAT_PTR_LEN + indent);
        safe_printf("]\n");
      }
      break;
    }
    case Tag_Tuple2: {
      safe_printf("(\n");
      pretty_print_child(deeper, v->tuple2.a);
      pretty_print_child(deeper, v->tuple2.b);
      print_indent(FORMAT_PTR_LEN + indent);
      safe_printf(")\n");
      break;
    }
    case Tag_Tuple3: {
      safe_printf("(\n");
      pretty_print_child(deeper, v->tuple3.a);
      pretty_print_child(deeper, v->tuple3.b);
      pretty_print_child(deeper, v->tuple3.c);
      print_indent(FORMAT_PTR_LEN + indent);
      safe_printf(")\n");
      break;
    }
    case Tag_Custom: {
      u32 ctor = v->custom.ctor;
      if (ctor < Debug_ctors_size) {
        size_t offset = sizeof("CTOR");
        safe_printf("%s\n", &Debug_ctors[ctor][offset]);
      } else if (ctor == JSON_VALUE_OBJECT) {
        safe_printf("(JSON Object)\n");
      } else if (ctor == JSON_VALUE_ARRAY) {
        safe_printf("(JSON Array)\n");
      } else {
        safe_printf("Custom (ctor %d)\n", ctor);
      }
      for (int i = 0; i < custom_params(p) && i < 10; i++) {
        pretty_print_child(deeper, v->custom.values[i]);
      }
      break;
    }
    case Tag_Record: {
      safe_printf("{\n");
      FieldGroup* fg = v->record.fieldgroup;
      u32 n_fields = v->header.size - (sizeof(Record) / SIZE_UNIT);
      for (int i = 0; i < n_fields && i < 10; i++) {
        void* child = v->custom.values[i];
        safe_printf(FORMAT_PTR, child);
        print_indent(deeper);
        u32 field = fg->fields[i];
        if (field < Debug_fields_size) {
          size_t offset = sizeof("FIELD");
          safe_printf("%s = ", &Debug_fields[field][offset]);
        } else {
          safe_printf("%d = ", field);
        }
        Debug_prettyHelp(deeper2, child);
      }
      print_indent(FORMAT_PTR_LEN + indent);
      safe_printf("}\n");
      break;
    }
    case Tag_FieldGroup: {
      safe_printf("FieldGroup\n");
      u32 n_fields = v->fieldgroup.size;
      for (int i = 0; i < n_fields && i < 10; i++) {
        print_indent(deeper);
        u32 field = v->fieldgroup.fields[i];
        if (field < Debug_fields_size) {
          size_t offset = sizeof("FIELD");
          safe_printf("%s\n", &Debug_fields[field][offset]);
        } else {
          safe_printf("%d\n", field);
        }
      }
      break;
    }
    case Tag_Closure: {
      safe_printf("Closure %s\n", Debug_evaluator_name(v->closure.evaluator));
      for (int i = 0; i < v->closure.n_values && i < 10; i++) {
        pretty_print_child(deeper, v->closure.values[i]);
      }
      break;
    }
    case Tag_JsRef:
      safe_printf("External JS object #%d\n", v->js_ref.id);
      break;

    case Tag_Process: {
      Process* p = (Process*)v;
      u32 stack_size = 0;
      for (ProcessStack* stack = p->stack; stack; stack = stack->rest) {
        stack_size++;
      }
      u32 mailbox_size = 0;
      for (Cons* cell = p->mailbox.front; cell != &Nil; cell = cell->tail) {
        mailbox_size++;
      }
      safe_printf("Process id %d, stack size %d, mailbox size %d\n",
          p->id,
          stack_size,
          mailbox_size);
      for (int i = 0; i < custom_params(&v->custom) && i < 10; i++) {
        pretty_print_child(deeper, v->custom.values[i]);
      }
      break;
    }

    case Tag_Task: {
      Task* t = (Task*)v;
      static const char* task_ctors[] = {
          "SUCCEED",
          "FAIL",
          "BINDING",
          "AND_THEN",
          "ON_ERROR",
          "RECEIVE",
      };
      static const char* task_props[] = {
          "value",
          "callback",
          "kill",
          "task",
      };
      safe_printf("Task %s\n", task_ctors[t->ctor]);
      void** children = (void**)(&t->value);
      for (int i = 0; i < ARRAY_LEN(task_props); i++) {
        void* child = children[i];
        safe_printf(FORMAT_PTR, child);
        print_indent(deeper);
        safe_printf("%s = ", task_props[i]);
        Debug_prettyHelp(deeper2, children[i]);
      }
      break;
    }

    default:
      safe_printf("CORRUPTED!! tag %x size %d\n", v->header.tag, v->header.size);
      break;
  }
}

void Debug_pretty(const char* label, void* p) {
  char dashes[20];
  for (int i = 0; i < FORMAT_PTR_LEN; i++) {
    dashes[i] = '-';
  }
  dashes[FORMAT_PTR_LEN] = '\0';
  safe_printf("%s ", dashes);
  safe_printf("  %s (%p)\n", label, p);
  pretty_print_child(2, p);
}

void Debug_pretty_with_location(
    const char* function, int line, const char* label, void* p) {
  char dashes[20];
  for (int i = 0; i < FORMAT_PTR_LEN; i++) {
    dashes[i] = '-';
  }
  dashes[FORMAT_PTR_LEN] = '\0';
  safe_printf("%s ", dashes);
  safe_printf("%s @ %s:%d\n", label, function, line);
  pretty_print_child(2, p);
}
