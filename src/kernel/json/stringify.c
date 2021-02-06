#include <stdio.h>  // sprintf

#include "../core/core.h"
#include "json-internal.h"
#include "json.h"

#define GC_NOT_FULL NULL;

static void* grow(u16** end) {
  CAN_THROW(GC_malloc(false, stringify_alloc_chunk));
  *end = (u16*)GC_malloc(false, 0);
  if (stringify_alloc_chunk < 1024) stringify_alloc_chunk *= 2;
  return GC_NOT_FULL;
}

static void* copy_ascii(char* src, u16** dest, u16** end) {
  char* from = src;
  u16* to = *dest;

  for (; *from; to++, from++) {
    if (to >= *end) {
      CAN_THROW(grow(end));
    }
    *to = *from;
  }

  *dest = to;
  return GC_NOT_FULL;
}

static void* stringify_string(ElmString16* src, u16** dest, u16** to_end) {
  u16* from = src->words16;
  u16* to = *dest;

  size_t len = code_units(src);
  u16* from_end = from + len;

  *to++ = '"';

  for (; from < from_end; to++, from++) {
    if (to + 5 >= *to_end) {
      CAN_THROW(grow(to_end));
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

  *dest = to;
  return GC_NOT_FULL;
}

void* stringify(u32 indent, u32 indent_current, void* p, u16** cursor, u16** end);

static void* write_indent(u32 indent_current, u16** dest, u16** end) {
  if (indent_current) {
    u16* to = *dest;
    while (to + indent_current >= *end) {
      CAN_THROW(grow(end));
    }
    for (size_t i = 0; i < indent_current; i++) {
      *to++ = ' ';
    }
    *dest = to;
  }
  return GC_NOT_FULL;
}

static void* write_char(u16** to, u16** end, u16 c) {
  if (to >= end) {
    CAN_THROW(grow(end));
  }
  **to = c;
  (*to)++;
  return GC_NOT_FULL;
}

static void* stringify_array(
    u32 indent, u32 indent_current, Custom* array, u16** to, u16** end) {
  u32 indent_next = indent_current + indent;

  CAN_THROW(write_char(to, end, '['));
  if (indent) {
    CAN_THROW(write_char(to, end, '\n'));
  }

  u32 len = custom_params(array);
  for (size_t i = 0; i < len; i++) {
    CAN_THROW(write_indent(indent_next, to, end));
    CAN_THROW(stringify(indent, indent_next, array->values[i], to, end));
    if (i < len - 1) {
      CAN_THROW(write_char(to, end, ','));
    }
    if (indent) {
      CAN_THROW(write_char(to, end, '\n'));
    }
  }

  CAN_THROW(write_indent(indent_current, to, end));
  CAN_THROW(write_char(to, end, ']'));

  return GC_NOT_FULL;
}

static void* stringify_object(
    u32 indent, u32 indent_current, Custom* object, u16** to, u16** end) {
  u32 indent_next = indent_current + indent;

  CAN_THROW(write_char(to, end, '{'));
  if (indent) {
    CAN_THROW(write_char(to, end, '\n'));
  }

  u32 len = custom_params(object);
  for (size_t i = 0; i < len; i += 2) {
    ElmString16* field = object->values[i];
    void* value = object->values[i + 1];
    CAN_THROW(write_indent(indent_next, to, end));
    CAN_THROW(stringify(indent, indent_next, field, to, end));
    CAN_THROW(write_char(to, end, ':'));
    if (indent) {
      CAN_THROW(write_char(to, end, ' '));
    }
    CAN_THROW(stringify(indent, indent_next, value, to, end));
    if (i < len - 2) {
      CAN_THROW(write_char(to, end, ','));
    }
    if (indent) {
      CAN_THROW(write_char(to, end, '\n'));
    }
  }

  CAN_THROW(write_indent(indent_current, to, end));
  CAN_THROW(write_char(to, end, '}'));

  return GC_NOT_FULL;
}

void* stringify(u32 indent, u32 indent_current, void* p, u16** cursor, u16** end) {
  ElmValue* v = p;
  Tag tag = v->header.tag;

  if (p == &Json_encodeNull) {
    return CAN_THROW(copy_ascii("null", cursor, end));
  } else if (p == &True) {
    return CAN_THROW(copy_ascii("true", cursor, end));
  } else if (p == &False) {
    return CAN_THROW(copy_ascii("false", cursor, end));
  } else if (tag == Tag_Float) {
    char buf[32];
    sprintf(buf, "%g", v->elm_float.value);
    return CAN_THROW(copy_ascii(buf, cursor, end));
  } else if (tag == Tag_String) {
    return CAN_THROW(stringify_string(&v->elm_string16, cursor, end));
  } else if (tag == Tag_Custom) {
    u32 ctor = v->custom.ctor;
    if (ctor == JSON_VALUE_ARRAY) {
      return CAN_THROW(stringify_array(indent, indent_current, p, cursor, end));
    } else if (ctor == JSON_VALUE_OBJECT) {
      return CAN_THROW(stringify_object(indent, indent_current, p, cursor, end));
    }
  }
  return GC_NOT_FULL;
}
