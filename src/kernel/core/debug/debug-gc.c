// =======================================================================
//
//                      GARBAGE COLLECTOR DEBUG
//
// =======================================================================

extern GcState gc_state;
extern char stack_flags[GC_STACK_MAP_SIZE];


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
  format_ptr_diff_size(
      size_after, sizeof(size_after), gc_state.heap.start, gc_state.next_alloc);

  bool dummy = true;
  if (dummy) printf("GC performance:\n");
  if (dummy) printf("  before:  %s\n", size_before);
  if (dummy) printf("  after:   %s\n", size_after);
  if (dummy)
    printf("  mark:    %5lld k cycles\n", (perf_data->marked - perf_data->start) / 1000);
  if (major)
    printf(
        "  compact: %5lld k cycles\n", (perf_data->compacted - perf_data->marked) / 1000);
  if (dummy)
    printf("  sweep:   %5lld k cycles\n",
        (perf_data->swept - (major ? perf_data->compacted : perf_data->marked)) / 1000);
  if (major)
    printf("  jsRefs:  %5lld k cycles\n", (perf_data->jsRefs - perf_data->swept) / 1000);
}
#endif
