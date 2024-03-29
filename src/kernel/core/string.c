#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "./gc/internals.h"
#include "core.h"

#define IS_LEADING_SURROGATE(word) (0xD800 <= word && word <= 0xDBFF)
#define IS_TRAILING_SURROGATE(word) (0xDC00 <= word && word <= 0xDFFF)
#define MAX_FLOAT_LEN 25

size_t code_units(ElmString* s) {
  u32 size = s->header.size;
  size_t* words = (size_t*)s;
  size_t* after = words + size;
  u16* last_code_unit = ((u16*)after) - 1;
  while ((*last_code_unit == 0) && (last_code_unit >= s->words16)) {
    last_code_unit--;
  }
  size_t units = last_code_unit + 1 - s->words16;
  return units;
}


#if 0
void debug_print_chars(char* label, void* start, void* after) {
  char buf[1024];
  char* c = buf;
  for (u16* p = (u16*)start; p < (u16*)after; p++, c++) {
    *c = *p;
  }
  *c = 0;
  safe_printf("%s: '%s'\n", label, buf);
}
#endif


#define not_aligned_64(ptr) ((size_t)ptr & (sizeof(u64) - 1))

static u16* copy_chars(u16* to16, u16* from16, u16* after16) {
  while (not_aligned_64(to16) && (from16 < after16) && *from16) {
    *to16++ = *from16++;
  }

  u64* to64 = (u64*)to16;
  u64* from64 = (u64*)from16;
  ptrdiff_t len64 = (after16 - from16) / 4;
  u64* after64 = from64 + len64;
  while (from64 < after64 && *from64) { // zero check prevents a bug in optimised build
    *to64++ = *from64++;
  }

  to16 = (u16*)to64;
  from16 = (u16*)from64;
  while ((from16 < after16) && *from16) {
    *to16++ = *from16++;
  }

  return to16;
}

#undef not_aligned_64


u16* String_copy(u16* to, ElmString* s) {
  return copy_chars(to, s->words16, s->words16 + code_units(s));
}


ptrdiff_t find_reverse(u16* sub, u16* str, size_t sub_len, ptrdiff_t str_idx) {
  for (;;) {
    // Match last char of substring
    ptrdiff_t sub_idx = sub_len - 1;
    while (str_idx >= 0 && sub_idx >= 0 && str[str_idx] != sub[sub_idx]) {
      str_idx--;
    }
    if (str_idx < 0) return -1;  // not found

    // Partial match. If the rest fails, retry from next.
    ptrdiff_t retry_idx = str_idx - 1;

    // Match remaining chars of substring
    do {
      str_idx--;
      sub_idx--;
    } while (str_idx >= 0 && sub_idx >= 0 && str[str_idx] == sub[sub_idx]);

    if (sub_idx < 0) return str_idx + 1;  // Found it!
    if (str_idx < 0) return -1;  // Matched some more chars then ran out => not found.
    str_idx = retry_idx;         // Only a partial match. Backtrack and try again.
  }
}

ptrdiff_t find_forward(u16* sub, u16* str, size_t sub_len, size_t str_len) {
  ptrdiff_t str_idx = 0;
  for (;;) {
    // Match first char of substring
    ptrdiff_t sub_idx = 0;
    while (str_idx < str_len && sub_idx < sub_len && str[str_idx] != sub[0]) {
      str_idx++;
    }

    if (str_idx >= str_len) return -1;  // not found

    // Partial match. If the rest fails, retry from next.
    ptrdiff_t retry_idx = str_idx + 1;

    // Match remaining chars of substring
    while (str_idx < str_len && sub_idx < sub_len && str[str_idx] == sub[sub_idx]) {
      str_idx++;
      sub_idx++;
    }

    if (sub_idx >= sub_len) return str_idx - sub_len;  // Found it!
    if (str_idx >= str_len) return -1;                 // Reached end without full match
    str_idx = retry_idx;  // Only a partial match. Backtrack and try again.
  }
}

/*
 * String.uncons
 */
void* eval_String_uncons(void* args[]) {
  ElmString* string = args[0];
  size_t len = code_units(string);
  if (len == 0) {
    return &g_elm_core_Maybe_Nothing;
  }
  u16 word = string->words16[0];
  u32 codepoint = (u32)word;
  size_t char_units;
  if (IS_LEADING_SURROGATE(word)) {
    char_units = 2;
    codepoint |= (string->words16[1] << 16);
  } else {
    char_units = 1;
  }
  ElmChar* c = newElmChar(codepoint);

  ElmString* s = newElmString(len - char_units);
  copy_chars(s->words16, string->words16 + char_units, string->words16 + len);
  return A1(&g_elm_core_Maybe_Just, newTuple2(c, s));
}
Closure String_uncons = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_String_uncons,
    .max_values = 1,
};

/*
 * String.append
 */
void* eval_String_append(void* args[]) {
  ElmString* a = args[0];
  ElmString* b = args[1];

  size_t len_a = code_units(a);
  size_t len_b = code_units(b);

  ElmString* s = newElmString(len_a + len_b);

  copy_chars(s->words16, a->words16, a->words16 + len_a);
  copy_chars(s->words16 + len_a, b->words16, b->words16 + len_b);

  return s;
}
Closure String_append = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_String_append,
    .max_values = 2,
};

/*
 * String.length
 */
void* eval_String_length(void* args[]) {
  size_t len = code_units(args[0]);
  return newElmInt((i32)len);
}
Closure String_length = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_String_length,
    .max_values = 1,
};

/*
 * String.foldr
 */
void* eval_String_foldr(void* args[]) {
  Closure* func = args[0];
  void* state = args[1];
  ElmString* string = args[2];

  size_t i = code_units(string);
  while (i--) {
    u32 word = string->words16[i];
    if (0xDC00 <= word && word <= 0xDFFF) {
      i--;
      word = (word << 16) | string->words16[i];
    }
    ElmChar* c = newElmChar(word);
    state = A2(func, c, state);
  }
  return state;
}
Closure String_foldr = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_String_foldr,
    .max_values = 3,
};

/*
 * String.split
 */
void* eval_String_split(void* args[]) {
  ElmString* sep = args[0];
  ElmString* str = args[1];

  size_t sep_len = code_units(sep);
  size_t str_len = code_units(str);
  Cons* result = &Nil;

  ptrdiff_t str_idx = str_len - 1;
  ptrdiff_t match_idx;
  ptrdiff_t substr_idx;
  ptrdiff_t substr_len;
  ElmString* substr;

  do {
    match_idx = find_reverse(sep->words16, str->words16, sep_len, str_idx);
    substr_idx = (match_idx < 0) ? 0 : (match_idx + sep_len);
    substr_len = 1 + str_idx - substr_idx;

    substr = newElmString(substr_len);
    copy_chars(substr->words16,
        str->words16 + substr_idx,
        str->words16 + substr_idx + substr_len);
    result = newCons(substr, result);

    str_idx = match_idx - 1;
  } while (substr_idx > 0);

  return result;
}
Closure String_split = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_String_split,
    .max_values = 2,
};

/*
 * String.join
 */
void* eval_String_join(void* args[]) {
  ElmString* sep = args[0];
  Cons* strs = args[1];

  if (strs == &Nil) {
    return newElmString(0);
  }
  size_t sep_len = code_units(sep);
  size_t len16 = 0;
  for (Cons* cell = strs; cell != &Nil; cell = cell->tail) {
    ElmString* s = cell->head;
    len16 += code_units(s);
    if (cell->tail != pNil) {
      len16 += sep_len;
    }
  }

  ElmString* result = newElmString(len16);

  u16* cursor = result->words16;
  for (Cons* cell = strs; cell != &Nil; cell = cell->tail) {
    ElmString* s = cell->head;
    cursor = String_copy(cursor, s);
    if (cell->tail != &Nil) {
      cursor = String_copy(cursor, sep);
    }
  }
  ASSERT_EQUAL(cursor, result->words16 + len16);

  return result;
}
Closure String_join = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_String_join,
    .max_values = 2,
};

/*
 * String.slice
 */
static size_t slice_wrap_index(i32 n, i32 len) {
  if (n <= -len) {
    return 0;
  } else if (n < 0) {
    return n + len;
  } else if (n > len) {
    return len;
  } else {
    return n;
  }
}
void* eval_String_slice(void* args[]) {
  ElmInt* argStart = args[0];
  ElmInt* argEnd = args[1];
  ElmString* str = args[2];
  size_t len = code_units(str);

  i32 start = slice_wrap_index(argStart->value, (i32)len);
  i32 end = slice_wrap_index(argEnd->value, (i32)len);
  if (start > end) {
    return newElmString(0);
  }

  ElmString* result = newElmString(end - start);
  copy_chars(result->words16, str->words16 + start, str->words16 + end);
  return result;
}
Closure String_slice = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_String_slice,
    .max_values = 3,
};

bool is_whitespace(u16 c) {
  // https://tc39.es/ecma262/#sec-string.prototype.trim
  // https://tc39.es/ecma262/#prod-WhiteSpace
  // https://www.compart.com/en/unicode/category/Zs
  // https://tc39.es/ecma262/#prod-LineTerminator

  // Naive implementation would do 25 comparisons for non-whitespace
  u8 upper = c >> 8;
  if (upper == 0x00) {
    if (c > 0x20) {
      // 223 chars: 3 comparisons
      return c == 0xA0;
    } else {
      // Check in order of (guessed) frequency
      return c == ' ' || c == '\n' || c == '\r' || c == '\t' || c == '\v' || c == '\f';
    }
  } else if (upper == 0x20) {
    if (c > 0x205F) {
      // 160 non-whitespace: 3 comparisons
      return false;
    } else if (c > 0x2029) {
      // 52 non-whitespace: 6 comparisons
      // 2 whitespace: 4-6 comparisons
      return c == 0x202F || c == 0x205F;
    } else {
      // 35 non-whitespace: 7 comparisons
      // 13 whitespace: 5-7 comparisons
      return c <= 0x200A || c == 0x2028 || c == 0x2029;
    }
  } else {
    // 65021 non-whitespace: 5 comparisons
    // 3 whitespace: 3-5 comparisons
    return c == 0x1680 || c == 0x3000 || c == 0xFEFF;
  }
}

/**
 * String.trim
 */
void* eval_String_trim(void* args[]) {
  ElmString* str = args[0];
  size_t len = code_units(str);
  u16* start = str->words16;
  u16* end = start + len - 1;
  while (is_whitespace(*start) && start <= end) {
    start++;
  }
  while (is_whitespace(*end) && end > start) {
    end--;
  }
  ptrdiff_t n_units = end + 1 - start;
  ElmString* result = newElmString(n_units);
  if (n_units > 0) {
    copy_chars(result->words16, start, end + 1);
  }
  return result;
}
Closure String_trim = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_String_trim,
    .max_values = 1,
};

/**
 * String.trimLeft
 */
void* eval_String_trimLeft(void* args[]) {
  ElmString* str = args[0];
  size_t len = code_units(str);
  u16* start = str->words16;
  u16* end = start + len - 1;
  while (is_whitespace(*start) && start <= end) {
    start++;
  }
  ptrdiff_t n_units = end + 1 - start;
  ElmString* result = newElmString(n_units);
  if (n_units > 0) {
    copy_chars(result->words16, start, end + 1);
  }
  return result;
}
Closure String_trimLeft = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_String_trimLeft,
    .max_values = 1,
};

/**
 * String.trimRight
 */
void* eval_String_trimRight(void* args[]) {
  ElmString* str = args[0];
  size_t len = code_units(str);
  u16* start = str->words16;
  u16* end = start + len - 1;
  while (is_whitespace(*end) && end >= start) {
    end--;
  }
  ptrdiff_t n_units = end + 1 - start;
  ElmString* result = newElmString(n_units);
  if (n_units > 0) {
    copy_chars(result->words16, start, end + 1);
  }
  return result;
}
Closure String_trimRight = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_String_trimRight,
    .max_values = 1,
};

/*
 * String.all
 */
void* eval_String_all(void* args[]) {
  Closure* isGood = args[0];
  ElmString* s = args[1];
  size_t len = code_units(s);
  ElmChar* c = newElmChar(0);

  for (size_t i = 0; i < len; i++) {
    u16 word = s->words16[i];
    u32 codepoint = (u32)word;
    if (IS_LEADING_SURROGATE(word)) {
      i++;
      codepoint |= (s->words16[i] << 16);
    }
    c->value = codepoint;
    if (A1(isGood, c) == &False) {
      return &False;
    }
  }
  return &True;
}
Closure String_all = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_String_all,
    .max_values = 2,
};

/**
 * String.contains
 */
void* eval_String_contains(void* args[]) {
  ElmString* sub = args[0];
  ElmString* str = args[1];

  size_t lsub = code_units(sub);
  if (lsub == 0) return &True;
  size_t lstr = code_units(str);
  if (lsub > lstr) return &False;

  ptrdiff_t idx = find_forward(sub->words16, str->words16, lsub, lstr);
  return (idx == -1) ? &False : &True;
}
Closure String_contains = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_String_contains,
    .max_values = 2,
};

/**
 * String.startsWith
 */
void* eval_String_startsWith(void* args[]) {
  ElmString* sub = args[0];
  ElmString* str = args[1];

  size_t lsub = code_units(sub);
  if (lsub == 0) return &True;
  size_t lstr = code_units(str);
  if (lsub > lstr) return &False;

  for (size_t i = 0; i < lsub; i++) {
    if (str->words16[i] != sub->words16[i]) {
      return &False;
    }
  }
  return &True;
}
Closure String_startsWith = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_String_startsWith,
    .max_values = 2,
};

/**
 * String.endsWith
 */
void* eval_String_endsWith(void* args[]) {
  ElmString* sub = args[0];
  ElmString* str = args[1];

  size_t lsub = code_units(sub);
  if (lsub == 0) return &True;
  size_t lstr = code_units(str);
  if (lsub > lstr) return &False;

  for (size_t i = 0; i < lsub; i++) {
    if (str->words16[lstr - 1 - i] != sub->words16[lsub - 1 - i]) {
      return &False;
    }
  }
  return &True;
}
Closure String_endsWith = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_String_endsWith,
    .max_values = 2,
};

/**
 * String.indexes
 */
void* eval_String_indexes(void* args[]) {
  ElmString* sub = args[0];
  ElmString* str = args[1];

  size_t sub_len = code_units(sub);
  if (sub_len == 0) {
    return &Nil;
  }
  size_t str_len = code_units(str);
  Cons* result = &Nil;

  ptrdiff_t str_idx = str_len - 1;
  ptrdiff_t match_idx;

  for (;;) {
    match_idx = find_reverse(sub->words16, str->words16, sub_len, str_idx);
    if (match_idx < 0) {
      return result;
    }
    result = newCons(newElmInt((i32)match_idx), result);
    str_idx = match_idx - 1;
  }
}
Closure String_indexes = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_String_indexes,
    .max_values = 2,
};

/*
 * String.fromNumber
 */
void* String_fromNumber_eval(void* args[]) {
  Number* box = args[0];
  char buf[MAX_FLOAT_LEN];
  int n_chars;

  if (box->i.header.tag == Tag_Int) {
    n_chars = stbsp_snprintf(buf, sizeof(buf), "%ld", (i64)box->i.value);
  } else {
    n_chars = stbsp_snprintf(buf, sizeof(buf), "%.16g", box->f.value);
  }

  ElmString* s = newElmString(n_chars);
  for (int i = 0; i < n_chars; i++) {
    s->words16[i] = (u16)buf[i];
  }
  return s;
}
Closure String_fromNumber = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &String_fromNumber_eval,
    .max_values = 1,
};

/*
 * String.toInt
 */
void* eval_String_toInt(void* args[]) {
  ElmString* str = args[0];
  size_t len = code_units(str);
  u16 code0 = str->words16[0];
  i32 total = 0;
  size_t start = (code0 == '+' || code0 == '-');

  size_t i;
  for (i = start; i < len; ++i) {
    u16 code = str->words16[i];
    if (code < 0x30 || 0x39 < code) {
      return &g_elm_core_Maybe_Nothing;
    }
    total = 10 * total + code - 0x30;
  }

  return i == start
             ? &g_elm_core_Maybe_Nothing
             : A1(&g_elm_core_Maybe_Just, newElmInt(code0 == 0x2D ? -total : total));
}
Closure String_toInt = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_String_toInt,
    .max_values = 1,
};

/*
 * String.toFloat
 */
void* eval_String_toFloat(void* args[]) {
  ElmString* s = args[0];
  size_t len = code_units(s);
  if (len >= MAX_FLOAT_LEN) {
    len = MAX_FLOAT_LEN - 1;
  }
  f64 value = parseFloat(s->words16, len);
  return isnan(value) ? &g_elm_core_Maybe_Nothing
                      : A1(&g_elm_core_Maybe_Just, newElmFloat(value));
}
Closure String_toFloat = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_String_toFloat,
    .max_values = 1,
};
