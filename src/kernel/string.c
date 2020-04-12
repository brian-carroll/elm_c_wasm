#include <stdio.h>  // sprintf
#include <stdlib.h>
#include <string.h>

#include "gc.h"
#include "maybe.h"
#include "types.h"
#include "utils.h"

#define IS_LOW_SURROGATE(word) (0xDC00 <= word && word <= 0xDFFF)
#define IS_HIGH_SURROGATE(word) (0xD800 <= word && word <= 0xDBFF)

size_t code_units(ElmString16* s) {
  u32 size = s->header.size;
  u32 size16 = size * SIZE_UNIT / 2;
  u16* words16 = (u16*)s;
  u16 last = words16[size16 - 1];
  size_t len16 = last ? (size16 - 2) : (size16 - 3);
  return len16;
}

/*
 * String.uncons
 */
static void* eval_String_uncons(void* args[]) {
  ElmString16* string = args[0];
  size_t len = code_units(string);
  if (len == 0) {
    return &g_elm_core_Maybe_Nothing;
  }
  u16 word = string->words16[0];
  u32 codepoint = (u32)word;
  size_t i = 0;
  if (IS_LOW_SURROGATE(word)) {
    i = 1;
    codepoint |= (string->words16[1] << 16);
  }
  ElmChar* c = NEW_ELM_CHAR(codepoint);

  char* remainder = (char*)(&string->words16[i + 1]);
  ElmString16* s = NEW_ELM_STRING(len - i, remainder);

  return A1(&g_elm_core_Maybe_Just, NEW_TUPLE2(c, s));
}
Closure String_uncons = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_String_uncons,
    .max_values = 1,
};

/*
 * String.length
 */
static void* eval_String_length(void* args[]) {
  size_t len = code_units(args[0]);
  return NEW_ELM_INT((u32)len);
}
Closure String_length = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_String_length,
    .max_values = 1,
};

/*
 * String.append
 */
void* String_append_eval(void* args[2]) {
  ElmString16* a = args[0];
  ElmString16* b = args[1];

  size_t len_a = code_units(a);
  size_t len_b = code_units(b);

  ElmString16* s = NEW_ELM_STRING16(len_a + len_b);

  memcpy(s->words16, a->words16, len_a * sizeof(u16));
  memcpy(&s->words16[len_a], b->words16, len_b * sizeof(u16));

  return s;
}
Closure String_append = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &String_append_eval,
    .max_values = 2,
};

/*
 * String.fromNumber
 */
static void* String_fromNumber_eval(void* args[1]) {
  Number* box = args[0];
  char buf[25];
  size_t n_chars;

  if (box->i.header.tag == Tag_Int) {
    n_chars = (size_t)snprintf(buf, sizeof(buf), "%d", box->i.value);
  } else {
    n_chars = (size_t)snprintf(buf, sizeof(buf), "%.16g", box->f.value);
  }

  ElmString16* s = NEW_ELM_STRING16(n_chars);
  for (size_t i = 0; i < n_chars; i++) {
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
static void* eval_String_toInt(void* args[]) {
  ElmString16* str = args[0];
  size_t len = code_units(str);
  u16 code0 = str->words16[0];
  i32 total = 0;
  size_t start = code0 == 0x2B /* + */ || code0 == 0x2D /* - */ ? 1 : 0;

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
             : A1(&g_elm_core_Maybe_Just, NEW_ELM_INT(code0 == 0x2D ? -total : total));
}
Closure String_toInt = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_String_toInt,
    .max_values = 1,
};

/*
 * String.join
 */
static void* eval_String_join(void* args[]) {
  ElmString16* sep = args[0];
  Cons* strs = args[1];

  ElmString16* s = strs->head;
  u32 result_len = code_units(s);
  Header h = HEADER_STRING(result_len);
  ElmString16* result = GC_malloc(h.size * SIZE_UNIT);
  if (result == pGcFull) {
    return pGcFull;
  }
  result->header = h;
  memcpy(result->words16, s->words16, result_len * 2);

  u32 sep_len = code_units(sep);

  for (strs = strs->tail; strs != &Nil; strs = strs->tail) {
    u16* to = &result->words16[result_len];
    s = strs->head;
    u32 len = code_units(s);

    result_len += sep_len + len;
    h = HEADER_STRING(result_len);
    if (GC_malloc((h.size - result->header.size) * SIZE_UNIT) == pGcFull) {
      return pGcFull;
    }
    result->header = h;

    memcpy(to, sep->words16, sep_len * 2);
    memcpy(to + sep_len, s->words16, len * 2);
  }
  return result;
}
Closure String_join = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_String_join,
    .max_values = 1,
};

/*
 * String.split
 */
static void* eval_String_split(void* args[]) {
  ElmString16* sep = args[0];
  ElmString16* str = args[1];

  size_t sep_len = code_units(sep);
  size_t str_len = code_units(str);

  Cons* result = &Nil;

  // iterate over substrings
  for (size_t str_idx = str_len - 1; str_idx > 0; --str_idx) {
    size_t substr_end_idx = str_idx;
    size_t substr_start_idx;

    // search for next substring
    while (1) {
      if (str_idx == 0) {
        substr_start_idx = 0;
      } else {
        size_t sep_idx = sep_len - 1;

        // match last codepoint of sep
        if (str->words16[str_idx] != sep->words16[sep_idx]) {
          str_idx--;
          continue;
        }
        size_t match_end_idx = str_idx;

        // match as many other codepoints as possible
        while (sep_idx && str_idx && str->words16[str_idx] == sep->words16[sep_idx]) {
          sep_idx--;
          str_idx--;
        };

        if (sep_idx != 0) {
          // not a full match. resume searching from next codepoint
          str_idx = match_end_idx - 1;
          continue;
        }
        substr_start_idx = match_end_idx + 1;
      }

      // Copy the substring and put it in the list
      size_t substr_len = 1 + substr_end_idx - substr_start_idx;
      ElmString* substr =
          NEW_ELM_STRING(substr_len * 2, (char*)(&str->words16[substr_start_idx]));
      result = NEW_CONS(substr, result);

      break;  // to outer loop (over substrings)
    };
  }

  return result;
}
Closure String_split = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_String_split,
    .max_values = 1,
};

/*
 * String.slice
 */
static void* eval_String_slice(void* args[]) {
  ElmInt* argStart = args[0];
  ElmInt* argEnd = args[1];
  ElmString16* str = args[2];
  size_t len = code_units(str);

  i32 start = argStart->value < 0 ? 0 : (argStart->value > len ? len : argStart->value);
  i32 end = argEnd->value < 0 ? 0 : (argEnd->value > len ? len : argEnd->value);
  if (start > end) {
    return NEW_ELM_STRING(0, NULL);
  }

  size_t n_words = (size_t)(end - start);
  size_t n_bytes = n_words * 2;
  u16* words_to_copy = &str->words16[start];

  return NEW_ELM_STRING(n_bytes, (char*)words_to_copy);
}
Closure String_slice = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_String_slice,
    .max_values = 3,
};

/*
 * String.all
 */
static void* eval_String_all(void* args[]) {
  Closure* isGood = args[0];
  ElmString16* s = args[1];
  size_t len = code_units(s);
  ElmChar* c = NEW_ELM_CHAR(0);

  for (size_t i = 0; i < len; i++) {
    u16 word = s->words16[i];
    u32 codepoint = (u32)word;
    if (IS_LOW_SURROGATE(word)) {
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
static void* eval_String_contains(void* args[]) {
  ElmString16* sub = args[0];
  ElmString16* str = args[1];

  size_t lsub = code_units(sub);
  size_t lstr = code_units(str);

  if (lsub == 0) return 0;

  u16 c0 = sub->words16[0];
  for (size_t istr = 0; istr < lstr; istr++) {
    if (str->words16[istr] == c0) {
      size_t isub = 1;
      while (isub < lsub && sub->words16[isub] == str->words16[istr]) {
        isub++;
      }
      if (isub == lsub) {
        return &True;
      }
    }
  }
  return &False;
}
Closure String_contains = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_String_contains,
    .max_values = 2,
};

/**
 * String.indexes
 */
static void* eval_String_indexes(void* args[]) {
  ElmString16* sub = args[0];
  ElmString16* str = args[1];

  size_t sub_len = code_units(sub);
  size_t str_len = code_units(str);

  Cons* result = &Nil;

  // iterate over substring occurrences
  for (size_t str_idx = str_len - 1; str_idx > 0; --str_idx) {
    // search for next substring
    while (1) {
      if (str_idx > 0) {
        size_t sub_idx = sub_len - 1;

        // match last codepoint of sub
        if (str->words16[str_idx] != sub->words16[sub_idx]) {
          str_idx--;
          continue;
        }

        // match as many other codepoints as possible
        size_t match_idx = str_idx;
        while (sub_idx && match_idx && str->words16[match_idx] == sub->words16[sub_idx]) {
          sub_idx--;
          match_idx--;
        };

        if (sub_idx != 0) {
          // not a full match. resume searching from next codepoint
          str_idx--;
          continue;
        }
      }

      result = NEW_CONS(NEW_ELM_INT((i32)str_idx), result);
      break;  // to outer loop (over substrings)
    };
  }

  return result;
}
Closure String_indexes = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_String_indexes,
    .max_values = 2,
};

// https://tc39.es/ecma262/#prod-LineTerminator
// https://tc39.es/ecma262/#prod-WhiteSpace
// https://www.compart.com/en/unicode/category/Zs
size_t is_whitespace(u16 c) {
  return (c == 0x000A || c == 0x000D || c == 0x0020 || c == 0x00A0 || c == 0x1680 ||
          c == 0x2000 || c == 0x2001 || c == 0x2002 || c == 0x2003 || c == 0x2004 ||
          c == 0x2005 || c == 0x2006 || c == 0x2007 || c == 0x2008 || c == 0x2009 ||
          c == 0x200A || c == 0x2028 || c == 0x2029 || c == 0x202F || c == 0x205F ||
          c == 0x3000);
}

/**
 * String.trim
 */
static void* eval_String_trim(void* args[]) {
  ElmString16* str = args[1];
  size_t len = code_units(str);
  size_t start = 0;
  size_t end = len - 1;
  while (is_whitespace(str->words16[start]) && start < len) {
    start++;
  }
  while (is_whitespace(str->words16[end]) && end) {
    end--;
  }
  size_t n_bytes = (end + 1 - start) * 2;
  return NEW_ELM_STRING(n_bytes, (char*)(&str->words16[start]));
}
Closure String_trim = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_String_trim,
    .max_values = 1,
};

/**
 * String.trimLeft
 */
static void* eval_String_trimLeft(void* args[]) {
  ElmString16* str = args[1];
  size_t len = code_units(str);
  size_t start = 0;
  while (is_whitespace(str->words16[start]) && start < len) {
    start++;
  }
  size_t n_bytes = (len - start) * 2;
  return NEW_ELM_STRING(n_bytes, (char*)(&str->words16[start]));
}
Closure String_trimLeft = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_String_trimLeft,
    .max_values = 1,
};

/**
 * String.trimRight
 */
static void* eval_String_trimRight(void* args[]) {
  ElmString16* str = args[1];
  size_t len = code_units(str);
  size_t end = len - 1;
  while (is_whitespace(str->words16[end])) {
    end--;
  }
  size_t n_bytes = (end + 1) * 2;
  return NEW_ELM_STRING(n_bytes, (char*)(&str->words16[0]));
}
Closure String_trimRight = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_String_trimRight,
    .max_values = 1,
};

/**
 * String.startsWith
 */
void* eval_String_startsWith(void* args[]) {
  ElmString16* sub = args[0];
  ElmString16* str = args[1];

  size_t lsub = code_units(sub);
  size_t lstr = code_units(str);

  if (lsub > lstr) {
    return &False;
  }

  for (size_t i = 0; i < lsub; i++) {
    if (str->words16[i] == sub->words16[i]) {
      return &False;
    }
  }
  return &True;
}
Closure String_startsWith = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_String_startsWith,
    .max_values = 1,
};

/**
 * String.endsWith
 */
void* eval_String_endsWith(void* args[]) {
  ElmString16* sub = args[0];
  ElmString16* str = args[1];

  size_t lsub = code_units(sub);
  size_t lstr = code_units(str);

  if (lsub > lstr) {
    return &False;
  }

  for (size_t i = 0; i < lsub; i++) {
    if (str->words16[lstr - 1 - i] == sub->words16[lsub - 1 - i]) {
      return &False;
    }
  }
  return &True;
}
Closure String_endsWith = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_String_endsWith,
    .max_values = 1,
};
