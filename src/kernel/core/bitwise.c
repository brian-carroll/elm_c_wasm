#include "core.h"


static void* eval_Bitwise_and(void* args[]) {
  ElmInt* a = args[0];
  ElmInt* b = args[1];
  return ctorElmInt(a->value & b->value);
}
Closure Bitwise_and = {
  .header = HEADER_CLOSURE(0),
  .evaluator = eval_Bitwise_and,
  .max_values = 2,
};

static void* eval_Bitwise_or(void* args[]) {
  ElmInt* a = args[0];
  ElmInt* b = args[1];
  return ctorElmInt(a->value | b->value);
}
Closure Bitwise_or = {
  .header = HEADER_CLOSURE(0),
  .evaluator = eval_Bitwise_or,
  .max_values = 2,
};

static void* eval_Bitwise_xor(void* args[]) {
  ElmInt* a = args[0];
  ElmInt* b = args[1];
  return ctorElmInt(a->value ^ b->value);
}
Closure Bitwise_xor = {
  .header = HEADER_CLOSURE(0),
  .evaluator = eval_Bitwise_xor,
  .max_values = 2,
};

static void* eval_Bitwise_complement(void* args[]) {
  ElmInt* a = args[0];
  return ctorElmInt(~a->value);
}
Closure Bitwise_complement = {
  .header = HEADER_CLOSURE(0),
  .evaluator = eval_Bitwise_complement,
  .max_values = 1,
};

static void* eval_Bitwise_shiftLeftBy(void* args[]) {
  ElmInt* offset = args[0];
  ElmInt* a = args[1];
  return ctorElmInt(a->value << offset->value);
}
Closure Bitwise_shiftLeftBy = {
  .header = HEADER_CLOSURE(0),
  .evaluator = eval_Bitwise_shiftLeftBy,
  .max_values = 2,
};

static void* eval_Bitwise_shiftRightBy(void* args[]) {
  ElmInt* offset = args[0];
  ElmInt* a = args[1];
  return ctorElmInt(a->value >> offset->value);
}
Closure Bitwise_shiftRightBy = {
  .header = HEADER_CLOSURE(0),
  .evaluator = eval_Bitwise_shiftRightBy,
  .max_values = 2,
};

static void* eval_Bitwise_shiftRightZfBy(void* args[]) {
  ElmInt* offset = args[0];
  ElmInt* a = args[1];
  u32 a_unsigned = a->value;
  return ctorElmInt(a_unsigned >> offset->value);
}
Closure Bitwise_shiftRightZfBy = {
  .header = HEADER_CLOSURE(0),
  .evaluator = eval_Bitwise_shiftRightZfBy,
  .max_values = 2,
};
