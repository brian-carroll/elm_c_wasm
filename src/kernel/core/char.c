#include "core.h"

void* eval_toCode(void* args[]) {
  ElmChar* c = args[0];
  u32 high = (u32)(c->words16[0]);
  u32 low = (u32)(c->words16[1]);
  u32 codepoint;
  if (low) {
    codepoint = (((high - 0xD800) << 10) | (low - 0xDC00)) + 0x10000;
  } else {
    codepoint = high;
  }

  return newElmInt(codepoint);
}
Closure Char_toCode = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_toCode,
    .max_values = 1,
};
