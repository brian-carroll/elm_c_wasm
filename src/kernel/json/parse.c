// Parse JSON values from a UTF-16 string
// https://ecma-international.org/publications/files/ECMA-ST/ECMA-404.pdf

#include <math.h>  // isnan
#include "../core/core.h"
#include "json.h"

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

  const u16 chars_true[4] = {'t', 'r', 'u', 'e'};
  const u16 chars_fals[4] = {'f', 'a', 'l', 's'};
  const u64* fast_compare_true = (u64*)chars_true;
  const u64* fast_compare_false = (u64*)chars_fals;

  if (len >= 4 && bytes64[0] == *fast_compare_true) {
    *cursor += 4;
    return &True;
  } else if (len >= 5 && bytes64[0] == *fast_compare_false && chars[4] == 'e') {
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

  const u16 chars_null[4] = {'n', 'u', 'l', 'l'};
  const u64* fast_compare_null = (u64*)chars_null;

  if (len >= 4 && bytes64[0] == *fast_compare_null) {
    *cursor += 4;
    return &Json_null;
  } else {
    return NULL;
  }
}

void* parse_number(u16** cursor, u16* end) {
  u16* chars = *cursor;

  size_t max = end - chars;
  if (max > 31) {
    max = 31;
  }
  u32 len16 = 0;
  for (; len16 < max; len16++) {
    u16 c = chars[len16];
    bool isValid = (c >= '0' && c <= '9') || c == '-' || c == '+' || c == 'e' ||
                   c == 'E' || c == '.';
    if (!isValid) break;
  }
  if (len16 == 0) return NULL;

  f64 f = parseFloat(chars, len16);
  if (isnan(f)) return NULL;

  *cursor += len16;
  return newElmFloat(f);
}

void* parse_string(u16** cursor, u16* end) {
  u16* from = *cursor;
  u16* to;

  if (from >= end) return NULL;  // make sure it's safe to deref
  if (*from != '"') return NULL;
  from++;

  u16* endquote = from;
  for (;;) {
    if (endquote >= end) {
      return NULL;
    } else if (*endquote == '"') {
      break;
    } else if (*endquote == '\\') {
      endquote += 2;
    } else {
      endquote++;
    }
  }

  size_t max_len = endquote - from;
  ElmString* str = newElmString(max_len);

  for (to = str->words16;; to++, from++) {
    if (from >= end) return NULL;
    if (*from == '"') break;
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
  size_t reclaim = str->header.size - final_size;
  str->header.size = (u32)final_size;
  GC_allocate(false, reclaim);
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
    return newCustom(JSON_VALUE_ARRAY, 0, NULL);
  }

  // gather the values into a List
  Cons* rev_values = &Nil;
  size_t len = 0;
  for (;;) {
    // parse value
    void* value = parse_recurse(&c, end);
    if (c >= end || value == NULL) return NULL;

    // store value
    rev_values = newCons(value, rev_values);
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
  Custom* array = newCustom(JSON_VALUE_ARRAY, len, NULL);
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
    return newCustom(JSON_VALUE_OBJECT, 0, NULL);
  }

  // temporary list-like structure for key-value pairs [field, value, last_pair]
  Tuple3* rev_pairs = NULL;
  size_t n_pairs = 0;
  for (;;) {
    // field
    ElmString* field = parse_string(&c, end);
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
    if (c >= end || value == NULL) return NULL;

    // store pair
    rev_pairs = newTuple3(field, value, rev_pairs);
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
  Custom* object = newCustom(JSON_VALUE_OBJECT, 2 * n_pairs, NULL);
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

    case '"':
      return parse_string(cursor, end);

    case '[':
      return parse_array(cursor, end);

    case '{':
      return parse_object(cursor, end);

    default:
      return parse_number(cursor, end);
  }
}

void* parse_json(ElmString* json) {
  u16* cursor = json->words16;
  u16* end = json->words16 + code_units(json);
  skip_whitespace(&cursor, end);
  return parse_recurse(&cursor, end);
}
