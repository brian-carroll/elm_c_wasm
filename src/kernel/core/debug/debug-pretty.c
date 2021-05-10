// =======================================================================
//
//                PRETTY PRINTING - WITH MEMORY ADDRESSES
//
// =======================================================================

#include <stdio.h> // putchar

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
  if (indent > 10) {
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
  if (indent > 10) {
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

  switch (v->header.tag) {
    case Tag_Int:
      safe_printf("Int %d\n", v->elm_int.value);
      break;
    case Tag_Float:
      safe_printf("Float %f\n", v->elm_float.value);
      break;
    case Tag_Char:
      safe_printf("Char 0x%8x\n", v->elm_char.value);
      break;
    case Tag_String: {
      safe_printf("String \"");
      ElmString16* s = p;
      for (size_t i = 0; i < code_units(s); i++) {
        u16 chr = s->words16[i];
        if (chr) {
          if (chr < 128) {
            putchar(chr);
          } else {
            putchar('#');
          }
        }
      }
      safe_printf("\"\n");
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
        safe_printf("(JSON Object)");
      } else if (ctor == JSON_VALUE_ARRAY) {
        safe_printf("(JSON Array)");
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
      safe_printf("Closure %s\n", name);
      for (int i = 0; i < v->closure.n_values && i < 10; i++) {
        pretty_print_child(deeper, v->closure.values[i]);
      }
      break;
    }
    case Tag_JsRef:
      safe_printf("External JS object %d\n", v->js_ref.index);
      break;
    default:
      safe_printf("CORRUPTED!! tag %x size %d\n", v->header.tag, v->header.size);
      break;
  }
}

void Debug_pretty(const char* label, void* p) {
  safe_printf("\n");
  for (int i = 0; i < FORMAT_PTR_LEN; i++) {
    safe_printf("-");
  }
  safe_printf("  %s (%p)\n", label, p);
  pretty_print_child(2, p);
}
