#include "core.h"


void* eval_Bitwise_and(void* args[]) {
  ElmInt* a = args[0];
  ElmInt* b = args[1];
  return newElmInt((i32)a->value & (i32)b->value);
}
Closure Bitwise_and = {
  .header = HEADER_CLOSURE(0),
  .evaluator = eval_Bitwise_and,
  .max_values = 2,
};

void* eval_Bitwise_or(void* args[]) {
  ElmInt* a = args[0];
  ElmInt* b = args[1];
  return newElmInt((i32)a->value | (i32)b->value);
}
Closure Bitwise_or = {
  .header = HEADER_CLOSURE(0),
  .evaluator = eval_Bitwise_or,
  .max_values = 2,
};

void* eval_Bitwise_xor(void* args[]) {
  ElmInt* a = args[0];
  ElmInt* b = args[1];
  return newElmInt((i32)a->value ^ (i32)b->value);
}
Closure Bitwise_xor = {
  .header = HEADER_CLOSURE(0),
  .evaluator = eval_Bitwise_xor,
  .max_values = 2,
};

void* eval_Bitwise_complement(void* args[]) {
  ElmInt* a = args[0];
  return newElmInt(~(i32)a->value);
}
Closure Bitwise_complement = {
  .header = HEADER_CLOSURE(0),
  .evaluator = eval_Bitwise_complement,
  .max_values = 1,
};

void* eval_Bitwise_shiftLeftBy(void* args[]) {
  ElmInt* offset = args[0];
  ElmInt* a = args[1];
  return newElmInt((i32)a->value << (i32)offset->value);
}
Closure Bitwise_shiftLeftBy = {
  .header = HEADER_CLOSURE(0),
  .evaluator = eval_Bitwise_shiftLeftBy,
  .max_values = 2,
};

void* eval_Bitwise_shiftRightBy(void* args[]) {
  ElmInt* offset = args[0];
  ElmInt* a = args[1];
  return newElmInt((i32)a->value >> (i32)offset->value);
}
Closure Bitwise_shiftRightBy = {
  .header = HEADER_CLOSURE(0),
  .evaluator = eval_Bitwise_shiftRightBy,
  .max_values = 2,
};

void* eval_Bitwise_shiftRightZfBy(void* args[]) {
  ElmInt* offset = args[0];
  ElmInt* a = args[1];
  i64 a_signed = a->value;
  u64 a_unsigned = a_signed & (u64)0xffffffff;
  return newElmInt(a_unsigned >> (i32)offset->value);
}
Closure Bitwise_shiftRightZfBy = {
  .header = HEADER_CLOSURE(0),
  .evaluator = eval_Bitwise_shiftRightZfBy,
  .max_values = 2,
};
