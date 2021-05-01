// ========================================================
//
//      Elm core lib Debug module
//
// ========================================================


// helpers for string building & memory management


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


// ========================================================


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


// ========================================================


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


// ========================================================


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
