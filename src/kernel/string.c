#include <stdio.h>  // sprintf
#include <stdlib.h>
#include <string.h>
#include "gc.h"
#include "types.h"

// local utility function
size_t String_bytes(ElmString* s) {
  size_t total_bytes = (size_t)(s->header.size * SIZE_UNIT);
  u8* struct_bytes = (u8*)s;
  u8 last_byte = struct_bytes[total_bytes - 1];
  size_t len = (total_bytes - sizeof(Header)) - (size_t)(last_byte + 1);
  return len;
}

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
const Closure String_append = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &String_append_eval,
    .max_values = 2,
};

static void* String_fromInt_eval(void* args[1]) {
  ElmInt* box = args[0];
  i32 unboxed = box->value;
  char buf[12];  // enough for -2147483648
  size_t ascii_len = (size_t)sprintf(buf, "%d", unboxed);
  size_t utf16_len = ascii_len << 1;
  ElmString* s = (NEW_ELM_STRING(utf16_len, NULL));
  u16* utf16 = (u16*)s->bytes;
  for (size_t i = 0; i < ascii_len; i++) {
    utf16[i] = (u16)buf[i];
  }
  return s;
}
const Closure String_fromInt = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &String_fromInt_eval,
    .max_values = 1,
};
