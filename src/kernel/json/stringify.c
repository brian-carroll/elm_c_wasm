#include "../core/core.h"
#include "../../lib/stb/stb_sprintf.h"
#include "json-internal.h"
#include "json.h"


static void stringify_string(ElmString16* src, StrBuilder* sb) {
  u16* from = src->words16;
  u16* to = sb->cursor;

  size_t len = code_units(src);
  u16* from_end = from + len;
  u16* section_end = sb->end - 5;

  *to++ = '"';

  for (; from < from_end; to++, from++) {
    if (to > section_end) {
      StrBuilder_finishSection(sb);
      StrBuilder_startSection(sb, 0);
      section_end = sb->end - 5;
      to = sb->cursor;
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


void stringify(u32 indent, u32 indent_current, void* p, StrBuilder* sb);

static void stringify_array(
    u32 indent, u32 indent_current, Custom* array, StrBuilder* sb) {
  u32 indent_next = indent_current + indent;

  StrBuilder_writeChar(sb, '[');
  if (indent) {
    StrBuilder_writeChar(sb, '\n');
  }

  u32 len = custom_params(array);
  for (size_t i = 0; i < len; i++) {
    StrBuilder_writeIndent(sb, indent_next);
    stringify(indent, indent_next, array->values[i], sb);
    if (i < len - 1) {
      StrBuilder_writeChar(sb, ',');
    }
    if (indent) {
      StrBuilder_writeChar(sb, '\n');
    }
  }

  StrBuilder_writeIndent(sb, indent_current);
  StrBuilder_writeChar(sb, ']');
}


static void stringify_object(
    u32 indent, u32 indent_current, Custom* object, StrBuilder* sb) {
  u32 indent_next = indent_current + indent;

  StrBuilder_writeChar(sb, '{');
  if (indent) {
    StrBuilder_writeChar(sb, '\n');
  }

  u32 len = custom_params(object);
  for (size_t i = 0; i < len; i += 2) {
    ElmString16* field = object->values[i];
    void* value = object->values[i + 1];
    StrBuilder_writeIndent(sb, indent_next);
    stringify(indent, indent_next, field, sb);
    StrBuilder_writeChar(sb, ':');
    if (indent) {
      StrBuilder_writeChar(sb, ' ');
    }
    stringify(indent, indent_next, value, sb);
    if (i < len - 2) {
      StrBuilder_writeChar(sb, ',');
    }
    if (indent) {
      StrBuilder_writeChar(sb, '\n');
    }
  }

  StrBuilder_writeIndent(sb, indent_current);
  StrBuilder_writeChar(sb, '}');
}


void stringify(u32 indent, u32 indent_current, void* p, StrBuilder* sb) {
  ElmValue* v = p;
  Tag tag = v->header.tag;

  if (p == &Json_encodeNull) {
    StrBuilder_copyAscii(sb, "null");
  } else if (p == &True) {
    StrBuilder_copyAscii(sb, "true");
  } else if (p == &False) {
    StrBuilder_copyAscii(sb, "false");
  } else if (tag == Tag_Float) {
    char buf[32];
    stbsp_snprintf(buf, sizeof(buf), "%g", v->elm_float.value);
    StrBuilder_copyAscii(sb, buf);
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
