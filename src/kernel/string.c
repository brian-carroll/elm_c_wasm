#include <stdio.h>  // sprintf
#include <stdlib.h>
#include <string.h>
#include "gc.h"
#include "types.h"

// local utility function
#if STRING_ENCODING == UTF16
size_t String_codepoints(ElmString* s) {
  u32 size = s->header.size;
  u32 size16 = size * SIZE_UNIT / 2;
  u16* words16 = (u16*)s;
  u16 last = words16[size16 - 1];
  size_t len16 = last ? (size16 - 2) : (size16 - 3);
  return len16;
}
#else
size_t String_bytes(ElmString* s) {
  size_t total_bytes = (size_t)(s->header.size * SIZE_UNIT);
  u8* struct_bytes = (u8*)s;
  u8 last_byte = struct_bytes[total_bytes - 1];
  size_t len = (total_bytes - sizeof(Header)) - (size_t)(last_byte + 1);
  return len;
}
#endif

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
  if (box->i.header.tag == Tag_Int) {
    return fromInt(box);
  }
  return fromFloat(box);
}
Closure String_fromNumber = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &String_fromNumber_eval,
    .max_values = 1,
};

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

static void* eval_String_split(void* args[]) {
  ElmString* sep = args[0];
  ElmString* str = args[1];

  /*
    outer vars
      sep_len
      result

    loop vars:
      str_idx position in str
      sep_idx position in sep
      copy_end end of range to copy

    search backwards through str
    check for a match
      check current sep_idx against current str_idx
      if no match, reset sep_idx = sep_len-1 and keep going
      if match, decrement sep_idx and try again
      if sep_idx already zero, we have a proper match
        do a copy NEW_ELM_STRING
        create a Cons cell, point `result` at it
        update copy_end to str_idx-1
  */
}
Closure String_split = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_String_split,
    .max_values = 1,
};
