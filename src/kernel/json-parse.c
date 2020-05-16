#include <stdio.h>

#include "./gc.h"
#include "./json.h"

Custom Json_Value_null = {.header = HEADER_CUSTOM(0), .ctor = JSON_VALUE_NULL};

void skip_whitespace(u16** cursor, u16* end) {
  u16* ptr = *cursor;
  for (; ptr < end; ptr++) {
    u16 c = *ptr;
    if (c != ' ' && c != '\t' && c != '\r' && c != '\n') {
      break;
    }
  }
  *cursor = ptr;
}

void* parse_bool(u16** cursor, u16* end) {
  size_t len = end - *cursor;
  u16* chars = *cursor;
  u64* bytes64 = (u64*)chars;

  const u64 BYTES_TRUE = 0x0065007500720074;
  const u64 BYTES_FALS = 0x0073006c00610066;

  if (len >= 4 && bytes64[0] == BYTES_TRUE) {
    *cursor += 4;
    return &True;
  } else if (len >= 5 && bytes64[0] == BYTES_FALS && chars[4] == 'e') {
    *cursor += 5;
    return &False;
  } else {
    return NULL;
  }
}

void* parse_null(u16** cursor, u16* end) {
  size_t len = end - *cursor;
  u16* chars = *cursor;
  u64* bytes64 = (u64*)chars;

  const u64 BYTES_NULL = 0x006c006c0075006e;

  if (len >= 4 && bytes64[0] == BYTES_NULL) {
    *cursor += 4;
    return &Json_Value_null;
  } else {
    return NULL;
  }
}

void* parse_number(u16** cursor, u16* end) {
  u16* chars = *cursor;

  size_t max = end - chars;
  if (max > 31) max = 31;

  char digits[32];
  size_t d = 0;
  for (size_t i = 0; i < max; i++) {
    char c = (char)chars[i];
    if ((c >= '0' && c <= '9') || c == '-' || c == '+' || c == 'e' || c == 'E' ||
        c == '.') {
      digits[d++] = c;
    } else {
      break;
    }
  }
  if (!d) return NULL;
  digits[d] = '\0';

  f64 f;
  int success = sscanf(digits, "%lg", &f);
  if (!success) return NULL;

  *cursor += d;
  return NEW_ELM_FLOAT(f);
}

void* parse_string(u16** cursor, u16* end) {
  u16* from = *cursor;
  u16* to;

  if (from >= end) return NULL;  // make sure it's safe to deref
  if (*from != '"') return NULL;
  from++;

  size_t alloc_chunk_bytes = 4 * SIZE_UNIT;  // Grow the output string in chunks this big
  ElmString16* str = NEW_ELM_STRING16(alloc_chunk_bytes / 2);
  u16* str_end = &str->words16[alloc_chunk_bytes / 2];

  for (to = str->words16;; to++, from++) {
    if (from >= end) return NULL;
    if (*from == '"') break;

    if (to >= str_end) {
      // Grow output string as needed, taking advantage of GC 'bump allocation'
      void* more_space = GC_malloc(alloc_chunk_bytes);
      if (more_space == pGcFull) return pGcFull;
      str->header.size += alloc_chunk_bytes / SIZE_UNIT;
      str_end += alloc_chunk_bytes / 2;
      if (alloc_chunk_bytes < 1024) alloc_chunk_bytes *= 2;
    }

    if (*from == '\\') {
      from++;
      if (from >= end) return NULL;
      switch (*from) {
        case '"':
        case '\\':
        case '/':
          *to = *from;
          break;
        case 'b':
          *to = '\b';
          break;
        case 'f':
          *to = '\f';
          break;
        case 'n':
          *to = '\n';
          break;
        case 'r':
          *to = '\r';
          break;
        case 't':
          *to = '\t';
          break;
        case 'u': {
          from++;
          if (from + 4 >= end) return NULL;
          u16 c = 0;
          for (size_t i = 0; i < 4; i++) {
            u16 h = from[i];
            if (h >= '0' && h <= '9') {
              c = (c * 16) + h - '0';
            } else if (h >= 'a' && h <= 'f') {
              c = (c * 16) + 10 + h - 'a';
            } else if (h >= 'A' && h <= 'F') {
              c = (c * 16) + 10 + h - 'A';
            } else {
              return NULL;
            }
          }
          from += 3;  // we'll get another +1 in the for loop
          *to = c;
          break;
        }
        default:
          return NULL;
      }
    } else if (*from > 0x1f) {
      *to = *from;
    } else {
      return NULL;
    }
  }

  // normalise the string length, chopping off any over-allocated space
  size_t truncated_str_end_addr = ((size_t)to + SIZE_UNIT - 1) & (-SIZE_UNIT);
  size_t final_size = (truncated_str_end_addr - (size_t)str) / SIZE_UNIT;
  str->header.size = (u32)final_size;
  for (; to < (u16*)truncated_str_end_addr; to++) {
    *to = 0;
  }

  *cursor = ++from;

  return str;
}

void* parse_recurse(u16** cursor, u16* end);

void* parse_array(u16** cursor, u16* end) {
  u16* c = *cursor;

  if (c >= end) return NULL;
  if (*c != '[') return NULL;
  c++;

  skip_whitespace(&c, end);
  if (c >= end) return NULL;

  if (*c == ']') {
    *cursor = ++c;
    return NEW_CUSTOM(JSON_VALUE_ARRAY, 0, NULL);
  }

  // gather the values into a List
  Cons* rev_values = &Nil;
  size_t len = 0;
  for (;;) {
    // parse value
    void* value = parse_recurse(&c, end);
    if (value == pGcFull) return pGcFull;
    if (c >= end || value == NULL) return NULL;

    // store value
    rev_values = NEW_CONS(value, rev_values);
    len++;

    // separator/end
    skip_whitespace(&c, end);
    if (c >= end) return NULL;
    if (*c == ']') {
      c++;
      break;
    }
    if (*c != ',') return NULL;
    c++;

    // next value
    skip_whitespace(&c, end);
    if (c >= end) return NULL;
  }

  // reverse the list into an array
  Custom* array = NEW_CUSTOM(JSON_VALUE_ARRAY, len, NULL);
  for (size_t i = len - 1; rev_values != &Nil; rev_values = rev_values->tail, i--) {
    array->values[i] = rev_values->head;
  }

  *cursor = c;
  return array;
}

void* parse_object(u16** cursor, u16* end) {
  u16* c = *cursor;

  if (c >= end) return NULL;
  if (*c != '{') return NULL;
  c++;

  skip_whitespace(&c, end);
  if (c >= end) return NULL;

  if (*c == '}') {
    *cursor = ++c;
    return NEW_CUSTOM(JSON_VALUE_OBJECT, 0, NULL);
  }

  // temporary list-like structure for key-value pairs [field, value, last_pair]
  Tuple3* rev_pairs = NULL;
  size_t n_pairs = 0;
  for (;;) {
    // field
    ElmString16* field = parse_string(&c, end);
    if (field == pGcFull) return pGcFull;
    if (c >= end || field == NULL) return NULL;

    // colon
    skip_whitespace(&c, end);
    if (c >= end) return NULL;
    if (*c != ':') return NULL;
    c++;
    skip_whitespace(&c, end);
    if (c >= end) return NULL;

    // value
    void* value = parse_recurse(&c, end);
    if (value == pGcFull) return pGcFull;
    if (c >= end || value == NULL) return NULL;

    // store pair
    rev_pairs = NEW_TUPLE3(field, value, rev_pairs);
    n_pairs++;

    // end of pair
    skip_whitespace(&c, end);
    if (c >= end) return NULL;
    if (*c == '}') {
      c++;
      break;
    }
    if (*c != ',') return NULL;
    c++;

    // next pair
    skip_whitespace(&c, end);
    if (c >= end) return NULL;
  }

  // reverse the list into an object
  Custom* object = NEW_CUSTOM(JSON_VALUE_OBJECT, 2 * n_pairs, NULL);
  for (size_t i = n_pairs - 1; rev_pairs != NULL; rev_pairs = rev_pairs->c, i--) {
    object->values[2 * i] = rev_pairs->a;
    object->values[2 * i + 1] = rev_pairs->b;
  }

  *cursor = c;
  return object;
}

void* parse_recurse(u16** cursor, u16* end) {
  switch (**cursor) {
    case 'n':
      return parse_null(cursor, end);

    case 't':
    case 'f':
      return parse_bool(cursor, end);

    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    case '-':
      return parse_number(cursor, end);

    case '"':
      return parse_string(cursor, end);

    case '[':
      return parse_array(cursor, end);

    case '{':
      return parse_object(cursor, end);

    default:
      return NULL;
  }
}

void* parse_value(u16** cursor, u16* end) {
  skip_whitespace(cursor, end);
  return parse_recurse(cursor, end);
}
