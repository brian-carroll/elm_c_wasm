#include <stdio.h>

#include "../core/core.h"
#include "json.h"

void* eval_Json_wrap(void* args[]) {
  ElmValue* v = args[0];
  if (v->header.tag == Tag_Int) {
    v = NEW_ELM_FLOAT(v->elm_int.value);
  }
  return v;
}
Closure Json_wrap = {
    .header = HEADER_CLOSURE(0),
    .max_values = 1,
    .evaluator = &eval_Json_wrap,
};

void* eval_Json_unwrap(void* args[]) {
  return args[0];
}
Closure Json_unwrap = {
    .header = HEADER_CLOSURE(0),
    .max_values = 1,
    .evaluator = &eval_Json_unwrap,
};

Custom emptyArray = {
    .header = HEADER_CUSTOM(0),
    .ctor = JSON_VALUE_ARRAY,
};
void* eval_Json_emptyArray(void* args[]) {
  return &emptyArray;
}
Closure Json_emptyArray = {
    .header = HEADER_CLOSURE(0),
    .max_values = 1,
    .evaluator = &eval_Json_emptyArray,
};

Custom emptyObject = {
    .header = HEADER_CUSTOM(0),
    .ctor = JSON_VALUE_OBJECT,
};
void* eval_Json_emptyObject(void* args[]) {
  return &emptyObject;
}
Closure Json_emptyObject = {
    .header = HEADER_CLOSURE(0),
    .max_values = 1,
    .evaluator = &eval_Json_emptyObject,
};

void* eval_Json_addField(void* args[]) {
  ElmString16* key = args[0];
  void* value = args[1];
  Custom* old_object = args[2];

  u32 old_params = custom_params(old_object);
  u32 old_size = old_object->header.size;
  u32 new_size = old_size + 2;

  Custom* new_object = CAN_THROW(GC_malloc(new_size * SIZE_UNIT));
  GC_memcpy(new_object, old_object, old_size * SIZE_UNIT);
  new_object->values[old_params] = key;
  new_object->values[old_params + 1] = value;

  return new_object;
}
Closure Json_addField = {
    .header = HEADER_CLOSURE(0),
    .max_values = 3,
    .evaluator = &eval_Json_addField,
};

void* eval_Json_addEntry(void* args[]) {
  Closure* func = args[0];
  void* entry = args[1];
  Custom* old_array = args[2];

  u32 old_params = custom_params(old_array);
  u32 old_size = old_array->header.size;
  u32 new_size = old_size + 1;

  Custom* new_array = CAN_THROW(GC_malloc(new_size * SIZE_UNIT));
  GC_memcpy(new_array, old_array, old_size * SIZE_UNIT);
  new_array->values[old_params] = A1(&Json_unwrap, A1(func, entry));

  return new_array;
}
Closure Json_addEntry = {
    .header = HEADER_CLOSURE(0),
    .max_values = 3,
    .evaluator = &eval_Json_addEntry,
};

size_t alloc_chunk_bytes;
#define GC_NOT_FULL NULL;

void write_indent(u32 n, u16** cursor, u16** end) {}

void* copy_ascii(char* src, u16** dest, u16** end) {
  char* from = src;
  u16* to = *dest;

  for (; *from; to++, from++) {
    if (to >= *end) {
      if (GC_malloc(alloc_chunk_bytes) == pGcFull) {
        return pGcFull;
      } else {
        *end += alloc_chunk_bytes / 2;
      }
    }
    *to = *from;
  }

  *dest = to;
  return GC_NOT_FULL;
}

void* encode_string(ElmString16* src, u16** dest, u16** to_end) {
  u16* from = src->words16;
  u16* to = *dest;

  size_t len = code_units(src);
  u16* from_end = from + len;

  *to++ = '"';

  for (; from < from_end; to++, from++) {
    if (to + 5 >= *to_end) {
      if (GC_malloc(alloc_chunk_bytes) == pGcFull) {
        return pGcFull;
      } else {
        *to_end += alloc_chunk_bytes / 2;
      }
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

void* encode(u32 indent, u32 indent_current, void* p, u16** cursor, u16** end);

void* encode_array(u32 indent, u32 indent_current, Custom* array, u16** to, u16** end) {
  /*
    <indent>[\n
    <indent_next><encode>,\n
    <indent>]

    be careful of trailing comma
  */
  return GC_NOT_FULL;
}

void* encode_object(u32 indent, u32 indent_current, Custom* object, u16** to, u16** end) {
  //
  return GC_NOT_FULL;
}

void* encode(u32 indent, u32 indent_current, void* p, u16** cursor, u16** end) {
  ElmValue* v = p;
  Tag tag = v->header.tag;
  if (p == &Json_Value_null) {
    return copy_ascii("null", cursor, end);
  } else if (p == &True) {
    return copy_ascii("true", cursor, end);
  } else if (p == &False) {
    return copy_ascii("false", cursor, end);
  } else if (tag == Tag_Float) {
    char buf[32];
    sprintf(buf, "%g", v->elm_float.value);
    return copy_ascii(buf, cursor, end);
  } else if (tag == Tag_String) {
    return encode_string(&v->elm_string16, cursor, end);
  } else if (tag == Tag_Custom) {
    u32 ctor = v->custom.ctor;
    if (ctor == JSON_VALUE_ARRAY) {
      return encode_array(indent, indent_current, p, cursor, end);
    } else if (ctor == JSON_VALUE_OBJECT) {
      return encode_object(indent, indent_current, p, cursor, end);
    }
  }
  return GC_NOT_FULL;
}

void* eval_Json_encode(void* args[]) {
  ElmInt* indentLevel = args[0];
  void* value = args[1];

  alloc_chunk_bytes = 64;
  size_t len = alloc_chunk_bytes / 2;
  ElmString16* str = NEW_ELM_STRING16(len);
  u16* cursor = str->words16;
  u16* end = cursor + len;

  void* gc_full = encode(indentLevel->value, 0, value, &cursor, &end);

  // normalise the string length, chopping off any over-allocated space
  size_t truncated_str_end_addr = ((size_t)cursor + SIZE_UNIT - 1) & (-SIZE_UNIT);
  size_t final_size = (truncated_str_end_addr - (size_t)str) / SIZE_UNIT;
  str->header.size = (u32)final_size;
  for (; cursor < (u16*)truncated_str_end_addr; cursor++) {
    *cursor = 0;
  }

  return gc_full ? pGcFull : str;
}
Closure Json_encode = {
    .header = HEADER_CLOSURE(0),
    .max_values = 2,
    .evaluator = &eval_Json_encode,
};