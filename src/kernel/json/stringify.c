#include "../core/core.h"
#include "../../lib/stb/stb_sprintf.h"
#include "json-internal.h"
#include "json.h"

// assumes nothing else gets allocated while stringifying
static void grow(StringBuilder* sb) {
  GC_allocate(false, stringify_alloc_chunk_words);
  sb->end = GC_allocate(false, 0);
  sb->s->header.size += stringify_alloc_chunk_words;
  if (stringify_alloc_chunk_words < 256) stringify_alloc_chunk_words *= 2;
  return;
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


static void stringify_string(ElmString16* src, StringBuilder* sb) {
  u16* from = src->words16;
  u16* to = sb->cursor;

  size_t len = code_units(src);
  u16* from_end = from + len;

  *to++ = '"';

  for (; from < from_end; to++, from++) {
    if (to + 5 >= sb->end) {
      grow(sb);
    }
    u16 c = *from;
    if (c == '"' || c == '\\') {
      *to++ = '\\';
      *to = c;
    } else if (c > 0x1f) {
      *to = c;
    } else {
      *to++ = '\\';
      switch (c) {
        case '\b':
          *to = 'b';
          break;
        case '\f':
          *to = 'f';
          break;
        case '\n':
          *to = 'n';
          break;
        case '\r':
          *to = 'r';
          break;
        case '\t':
          *to = 't';
          break;
        default:
          *to++ = 'u';
          *to++ = '0';
          *to++ = '0';
          *to++ = (c & 0x10) ? '1' : '0';
          c = c & 0xf;
          if (c >= 10) {
            *to = 'a' + c - 10;
          } else {
            *to = '0' + c;
          }
          break;
      }
    }
  }

  *to++ = '"';

  sb->cursor = to;
}


void stringify(u32 indent, u32 indent_current, void* p, StringBuilder* sb);


static void write_indent(u32 indent_current, StringBuilder* sb) {
  if (indent_current) {
    u16* to = sb->cursor;
    while (to + indent_current >= sb->end) {
      grow(sb);
    }
    for (size_t i = 0; i < indent_current; i++) {
      *to++ = ' ';
    }
    sb->cursor = to;
  }
}


static void write_char(StringBuilder* sb, u16 c) {
  if (sb->cursor >= sb->end) {
    grow(sb);
  }
  *(sb->cursor) = c;
  sb->cursor++;
}


static void stringify_array(
    u32 indent, u32 indent_current, Custom* array, StringBuilder* sb) {
  u32 indent_next = indent_current + indent;

  write_char(sb, '[');
  if (indent) {
    write_char(sb, '\n');
  }

  u32 len = custom_params(array);
  for (size_t i = 0; i < len; i++) {
    write_indent(indent_next, sb);
    stringify(indent, indent_next, array->values[i], sb);
    if (i < len - 1) {
      write_char(sb, ',');
    }
    if (indent) {
      write_char(sb, '\n');
    }
  }

  write_indent(indent_current, sb);
  write_char(sb, ']');
}


static void stringify_object(
    u32 indent, u32 indent_current, Custom* object, StringBuilder* sb) {
  u32 indent_next = indent_current + indent;

  write_char(sb, '{');
  if (indent) {
    write_char(sb, '\n');
  }

  u32 len = custom_params(object);
  for (size_t i = 0; i < len; i += 2) {
    ElmString16* field = object->values[i];
    void* value = object->values[i + 1];
    write_indent(indent_next, sb);
    stringify(indent, indent_next, field, sb);
    write_char(sb, ':');
    if (indent) {
      write_char(sb, ' ');
    }
    stringify(indent, indent_next, value, sb);
    if (i < len - 2) {
      write_char(sb, ',');
    }
    if (indent) {
      write_char(sb, '\n');
    }
  }

  write_indent(indent_current, sb);
  write_char(sb, '}');
}


void stringify(u32 indent, u32 indent_current, void* p, StringBuilder* sb) {
  ElmValue* v = p;
  Tag tag = v->header.tag;

  if (p == &Json_encodeNull) {
    copy_ascii("null", sb);
  } else if (p == &True) {
    copy_ascii("true", sb);
  } else if (p == &False) {
    copy_ascii("false", sb);
  } else if (tag == Tag_Float) {
    char buf[32];
    stbsp_snprintf(buf, sizeof(buf), "%g", v->elm_float.value);
    copy_ascii(buf, sb);
  } else if (tag == Tag_String) {
    stringify_string(&v->elm_string16, sb);
  } else if (tag == Tag_Custom) {
    u32 ctor = v->custom.ctor;
    if (ctor == JSON_VALUE_ARRAY) {
      stringify_array(indent, indent_current, p, sb);
    } else if (ctor == JSON_VALUE_OBJECT) {
      stringify_object(indent, indent_current, p, sb);
    }
  }
}
