#include <stdio.h>  // sprintf
#include <stdlib.h>
#include <string.h>
#include "gc.h"
#include "types.h"

// local utility function
#if STRING_ENCODING == UTF16
size_t String_bytes(ElmString* s) {
  u32 size = s->header.size;
  u32 size16 = size * SIZE_UNIT / 2;
  u16* words16 = (u16*)s;
  u16 last = words16[size16 - 1];
  size_t len16 = last ? (size16 - 2) : (size16 - 3);
  size_t len8 = len16 < 1;
  return len8;
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
