#include <stdio.h>  // sprintf
#include <stdlib.h>
#include <string.h>

#include "gc.h"
#include "types.h"

// local utility function
#if STRING_ENCODING == UTF16
static size_t String_codepoints(ElmString16* s) {
  u32 size = s->header.size;
  u32 size16 = size * SIZE_UNIT / 2;
  u16* words16 = (u16*)s;
  u16 last = words16[size16 - 1];
  size_t len16 = last ? (size16 - 2) : (size16 - 3);
  return len16;
}

static size_t indexOf(ElmString16* sub, ElmString16* str) {
  size_t lstr = String_codepoints(str);
  size_t lsub = String_codepoints(sub);

  if (lsub == 0) return 0;

  u16 c0 = sub->words16[0];
  for (size_t istr = 0; istr < lstr; istr++) {
    if (str->words16[istr] == c0) {
      size_t isub = 1;
      while (isub < lsub && sub->words16[isub] == str->words16[istr]) {
        isub++;
      }
      if (isub == lsub) {
        return istr;
      }
    }
  }
  return -1;
}

#define IS_LOW_SURROGATE(word) (0xDC00 <= word && word <= 0xDFFF)
#define IS_HIGH_SURROGATE(word) (0xD800 <= word && word <= 0xDBFF)

#else
static size_t String_bytes(ElmString* s) {
  size_t total_bytes = (size_t)(s->header.size * SIZE_UNIT);
  u8* struct_bytes = (u8*)s;
  u8 last_byte = struct_bytes[total_bytes - 1];
  size_t len = (total_bytes - sizeof(Header)) - (size_t)(last_byte + 1);
  return len;
}
#endif

/*
 * String.uncons
 */
static void* eval_String_uncons(void* args[]) {
  ElmString16* string = args[0];
  size_t len = String_codepoints(string);
  if (len == 0) {
    return elm_core_Maybe_Nothing;
  }
  u16 word = string->words16[0];
  u32 codepoint = (u32)word;
  size_t i = 0;
  if (IS_LOW_SURROGATE(word)) {
    i = 1;
    codepoint |= (s->words16[1] << 16);
  }
  ElmChar* c = NEW_ELM_CHAR(codepoint);

  char* remainder = (char*)(&string->words16[i + 1]);
  ElmString16* s = NEW_ELM_STRING(len - i, remainder);

  return elm_core_Maybe_Just(NEW_TUPLE2(c, s));
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
  size_t len = String_codepoints(args[0]);
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
  ElmString* a = args[0];
  ElmString* b = args[1];

  u32 na = String_bytes(a);
  u32 nb = String_bytes(b);

  ElmString* s = NEW_ELM_STRING(na + nb, NULL);

  // May not be aligned on word boundaries
  // TODO: see if this can be optimised more
  memcpy(s->bytes, a->bytes, na);
  memcpy(&s->bytes[na], b->bytes, nb);

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
static void* fromInt(ElmInt* box) {
  i32 unboxed = box->value;
  char buf[12];  // enough for -2147483648
  size_t n_chars = (size_t)snprintf(buf, sizeof(buf), "%d", unboxed);
  ElmString* s = NEW_ELM_STRING(n_chars, NULL);
  u16* utf16 = (u16*)s->bytes;
  for (size_t i = 0; i < n_chars; i++) {
    utf16[i] = (u16)buf[i];
  }
  return s;
}

static void* fromFloat(ElmFloat* box) {
  f64 unboxed = box->value;
  char dummy[1];
  size_t n_chars = (size_t)snprintf(dummy, sizeof(dummy), "%g", unboxed);
  ElmString* tmp = NEW_ELM_STRING((n_chars + 1) / 2, NULL);
  ElmString* s = NEW_ELM_STRING(n_chars, NULL);
  snprintf(tmp->bytes, n_chars, "%g", unboxed);
  u16* utf16 = (u16*)s->bytes;
  for (size_t i = 0; i < n_chars; i++) {
    utf16[i] = (u16)tmp->bytes[i];
  }
  return s;
}

static void* String_fromNumber_eval(void* args[1]) {
  Number* box = args[0];
  return (box->i.header.tag == Tag_Int) ? fromInt(box) : fromFloat(box);
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
  size_t len = String_codepoints(str);
  u16 code0 = str->words16[0];
  u32 total = 0;
  size_t start = code0 == 0x2B /* + */ || code0 == 0x2D /* - */ ? 1 : 0;

  for (size_t i = start; i < str.length; ++i) {
    u16 code = str->words16[i];
    if (code < 0x30 || 0x39 < code) {
      return elm_core_Maybe_Nothing;
    }
    total = 10 * total + code - 0x30;
  }

  return i == start ? elm_core_Maybe_Nothing
                    : elm_core_Maybe_Just(code0 == 0x2D ? -total : total);
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
  u32 result_len = String_codepoints(s);
  Header h = HEADER_STRING(result_len);
  ElmString16* result = GC_malloc(h.size * SIZE_UNIT);
  if (result == pGcFull) {
    return pGcFull;
  }
  result->header = h;
  memcpy(result->values, s->words16, result_len * 2);

  u32 sep_len = String_codepoints(sep);

  for (strs = strs->tail; strs != &Nil; strs = strs->tail) {
    u16* to = &result->values[result_len];
    s = strs->head;
    u32 len = String_codepoints(s);

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

  size_t sep_len = String_codepoints(sep);
  size_t str_len = String_codepoints(str);

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
  ElmInt* start = args[0];
  ElmInt* end = args[1];
  ElmString16* str = args[2];
  size_t len = String_codepoints(str);

  size_t n_words = (size_t)(end - start);
  size_t n_bytes = payload_words * 2;
  u16* words_to_copy = str->words16[start->value];

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
  size_t len = String_codepoints(s);
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
  return indexOf(sub, str) == -1 ? &False : &True;
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

  size_t sub_len = String_codepoints(sub);
  size_t str_len = String_codepoints(str);

  Cons* result = &Nil;

  // iterate over substring occurrences
  for (size_t str_idx = str_len - 1; str_idx > 0; --str_idx) {
    size_t substr_end_idx = str_idx;
    size_t substr_start_idx;

    // search for next substring
    while (1) {
      if (str_idx == 0) {
        substr_start_idx = 0;
      } else {
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

/**
 * String.startsWith
 */
static void* eval_String_startsWith(void* args[]) {
  ElmString16* str = args[1];
}
Closure String_startsWith = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_String_startsWith,
    .max_values = 2,
};

/**
 * String.trim
 */
static void* eval_String_trim(void* args[]) {
  ElmString16* str = args[1];
}
Closure String_trim = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_String_trim,
    .max_values = 1,
};
/*

var _String_startsWith = F2(function(sub, str)
{
        return str.indexOf(sub) === 0;
});

var _String_endsWith = F2(function(sub, str)
{
        return str.length >= sub.length &&
                str.lastIndexOf(sub) === str.length - sub.length;
});


*/