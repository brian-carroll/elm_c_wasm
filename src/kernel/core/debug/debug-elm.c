// ========================================================
//
//      Elm core lib Debug module
//
// ========================================================

#include <stdio.h> // putchar


void Debug_toStringHelp(int depth, void* p, StringBuilder* sb) {
  ElmValue* v = p;
  char ascii_buf[25];

  if (!depth) {
    StringBuilder_copyAscii(sb, "...");
    return;
  }

  switch (v->header.tag) {
    case Tag_Int: {
      stbsp_snprintf(ascii_buf, sizeof(ascii_buf), "%lld", (i64)v->elm_int.int_f64);
      StringBuilder_copyAscii(sb, ascii_buf);
      return;
    }
    case Tag_Float: {
      stbsp_snprintf(ascii_buf, sizeof(ascii_buf), "%.16g", v->elm_float.value);
      StringBuilder_copyAscii(sb, ascii_buf);
      return;
    }
    case Tag_Char: {
      u16 lower = v->elm_char.words16[0];
      u16 upper = v->elm_char.words16[1];
      StringBuilder_ensureSpace(sb, upper ? 4 : 3);
      u16* write = sb->cursor;
      *write++ = '\'';
      *write++ = lower;
      if (upper) *write++ = upper;
      *write++ = '\'';
      sb->cursor = write;
      return;
    }
    case Tag_String: {
      size_t len = code_units(&v->elm_string);
      StringBuilder_ensureSpace(sb, len + 2);
      u16* write = sb->cursor;
      *write++ = '"';
      for (size_t i = 0; i < len; ++i) {
        *write++ = v->elm_string.words16[i];
      }
      *write++ = '"';
      sb->cursor = write;
      return;
    }
    case Tag_List: {
      StringBuilder_writeChar(sb, '[');
      for (Cons* list = &v->cons; list != pNil; list = list->tail) {
        Debug_toStringHelp(depth - 1, list->head, sb);
        if (list->tail != pNil) StringBuilder_writeChar(sb, ',');
      }
      StringBuilder_writeChar(sb, ']');
      return;
    }
    case Tag_Tuple2: {
      Tuple2* t = &v->tuple2;
      StringBuilder_writeChar(sb, '(');
      Debug_toStringHelp(depth - 1, t->a, sb);
      StringBuilder_writeChar(sb, ',');
      Debug_toStringHelp(depth - 1, t->b, sb);
      StringBuilder_writeChar(sb, ')');
      return;
    }
    case Tag_Tuple3: {
      Tuple3* t = &v->tuple3;
      StringBuilder_writeChar(sb, '(');
      Debug_toStringHelp(depth - 1, t->a, sb);
      StringBuilder_writeChar(sb, ',');
      Debug_toStringHelp(depth - 1, t->b, sb);
      StringBuilder_writeChar(sb, ',');
      Debug_toStringHelp(depth - 1, t->c, sb);
      StringBuilder_writeChar(sb, ')');
      return;
    }
    case Tag_Custom: {
      Custom* c = &v->custom;
      if (c == &True) {
        StringBuilder_copyAscii(sb, "True");
        return;
      } else if (c == &False) {
        StringBuilder_copyAscii(sb, "False");
        return;
      } else if (c == &Unit) {
        StringBuilder_copyAscii(sb, "()");
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
      StringBuilder_copyAscii(sb, ctor);
      StringBuilder_writeChar(sb, ' ');
      int len = custom_params(c);
      for (int i = 0; i < len; ++i) {
        Debug_toStringHelp(depth - 1, c->values[i], sb);
        if (i != len - 1) StringBuilder_writeChar(sb, ' ');
      }
      return;
    }
    case Tag_Record: {
      Record* r = &v->record;
      FieldGroup* fg = r->fieldgroup;
      u32 size = fg->size;
      StringBuilder_writeChar(sb, '{');
      for (int i = 0; i < size; ++i) {
        char* field = Debug_fields[fg->fields[i]];
        StringBuilder_copyAscii(sb, field);
        StringBuilder_copyAscii(sb, ": ");
        Debug_toStringHelp(depth - 1, r->values[i], sb);
        if (i != size - 1) StringBuilder_writeChar(sb, ',');
      }
      return;
    }
    case Tag_FieldGroup: {
      StringBuilder_copyAscii(sb, "<fieldgroup>");
      return;
    }
    case Tag_Closure: {
      StringBuilder_copyAscii(sb, "<function>");
      return;
    }
    case Tag_JsRef: {
      StringBuilder_copyAscii(sb, "<JavaScript>");
      return;
    }
    case Tag_Process: {
      StringBuilder_copyAscii(sb, "<Process>");
      return;
    }
    case Tag_Task: {
      StringBuilder_copyAscii(sb, "<Task>");
      return;
    }
    default: {
      StringBuilder_copyAscii(sb, "<unknown>");
      return;
    }
  }
}

void* eval_Debug_toString(void* args[]) {
  void* value = args[0];
  StringBuilder sb;
  StringBuilder_init(&sb);
  Debug_toStringHelp(5, value, &sb);
  ElmString* str = StringBuilder_toString(&sb);

  return str;
}
Closure Debug_toString = {
    .header = HEADER_CLOSURE(0),
    .max_values = 1,
    .evaluator = eval_Debug_toString,
};


// ========================================================


void* eval_Debug_log(void* args[]) {
  ElmString* label = args[0];
  void* value = args[1];

  size_t label_len = code_units(label);
  for (size_t i = 0; i < label_len; ++i) {
    putchar(label->words16[i]);
  }

  putchar(':');
  putchar(' ');

  ElmString* s = eval_Debug_toString(args + 1);
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
  ElmString* message = args[0];
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
