#include <stdio.h>  // sprintf
#include <stdlib.h>
#include <string.h>
#include "gc.h"
#include "types.h"

// local utility function
#if STRING_ENCODING == UTF16
static size_t String_codepoints(ElmString* s) {
  u32 size = s->header.size;
  u32 size16 = size * SIZE_UNIT / 2;
  u16* words16 = (u16*)s;
  u16 last = words16[size16 - 1];
  size_t len16 = last ? (size16 - 2) : (size16 - 3);
  return len16;
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
      return Maybe_Nothing;
    }
    total = 10 * total + code - 0x30;
  }

  return i == start ? __Maybe_Nothing : Maybe_Just(code0 == 0x2D ? -total : total);
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
