#include "core.h"

#include <assert.h>
#include <math.h>

/**
 * negate
 * Elm version of Basics_negate ends up being self-referential!
 * Have to break the cycle using a kernel version
 */
void* eval_negate(void* args[]) {
  ElmFloat* x = args[0];
  ElmFloat* result = GC_allocate(true, SIZE_FLOAT);
  result->header = x->header;
  result->value = -x->value;
  return result;
}
Closure Basics_negate = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_negate,
    .max_values = 1,
};

/**
 * add
 */
void* eval_add(void* args[]) {
  ElmFloat* x = args[0];
  ElmFloat* y = args[1];
  ElmFloat* result = GC_allocate(true, SIZE_FLOAT);
  result->header = x->header;
  result->value = x->value + y->value;
  return result;
}
Closure Basics_add = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_add,
    .max_values = 2,
};

/**
 * sub
 */
void* eval_sub(void* args[]) {
  ElmFloat* x = args[0];
  ElmFloat* y = args[1];
  ElmFloat* result = GC_allocate(true, SIZE_FLOAT);
  result->header = x->header;
  result->value = x->value - y->value;
  return result;
}
Closure Basics_sub = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_sub,
    .max_values = 2,
};

/**
 * mul
 */
void* eval_mul(void* args[]) {
  ElmFloat* x = args[0];
  ElmFloat* y = args[1];
  ElmFloat* result = GC_allocate(true, SIZE_FLOAT);
  result->header = x->header;
  result->value = x->value * y->value;
  return result;
}
Closure Basics_mul = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_mul,
    .max_values = 2,
};

/**
 * fdiv
 */
void* eval_fdiv(void* args[]) {
  ElmFloat* x = args[0];
  ElmFloat* y = args[1];
  ElmFloat* result = GC_allocate(true, SIZE_FLOAT);
  result->header = x->header;
  result->value = x->value / y->value;
  return result;
}
Closure Basics_fdiv = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_fdiv,
    .max_values = 2,
};

/**
 * idiv
 */
void* eval_idiv(void* args[]) {
  ElmInt* x = args[0];
  ElmInt* y = args[1];
  i64 result = (i64)x->value / (i64)y->value;
  return newElmInt((f64)result);
}
Closure Basics_idiv = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_idiv,
    .max_values = 2,
};

#if 0
// I was all proud of this, and now it's unused! Maybe some day...
static i32 ipow(i32 base, i32 ex) {
  if (ex < 0) {
    if (base == 1) {
      return 1;
    } else if (base == -1) {
      return (ex & 1) ? -1 : 1;
    } else {
      return 0;
    }
  }
  // Iterate over bits (<32 iterations)
  // Gradually reduce ex to 0, maintaining invariant `result*base^ex`
  // Eventually `base^0=1`, so `result` must be original value of `base^ex`
  // https://stackoverflow.com/a/101613
  i32 result = 1;
  while (1) {
    // (result*base)*base^(ex-1) = result*base^ex   [ex-1 is implicit in right shift]
    if (ex & 1) {
      result *= base;
    }
    // result*(base^2)^(ex/2) = result*base^ex
    ex >>= 1;
    if (!ex) {  // always terminates if ex started positive or zero
      break;
    }
    base *= base;
  }
  return result;
}
#endif

/**
 * pow
 */
void* eval_pow(void* args[]) {
  ElmFloat* x = args[0];
  ElmFloat* y = args[1];
  ElmFloat* result = GC_allocate(true, SIZE_FLOAT);
  result->header = x->header;
  result->value = pow(x->value, y->value);
  return result;
}
Closure Basics_pow = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_pow,
    .max_values = 2,
};

/*
 * toFloat
 */
void* eval_toFloat(void* args[]) {
  ElmInt* i = args[0];
  return newElmFloat(i->value);
}
Closure Basics_toFloat = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_toFloat,
    .max_values = 1,
};

/*
 * round
 */
void* eval_round(void* args[]) {
  ElmFloat* f = args[0];
  f64 result = round(f->value);
  return newElmInt(result);
}
Closure Basics_round = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_round,
    .max_values = 1,
};

/*
 * floor
 */
void* eval_floor(void* args[]) {
  ElmFloat* f = args[0];
  f64 result = floor(f->value);
  return newElmInt(result);
}
Closure Basics_floor = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_floor,
    .max_values = 1,
};

/*
 * ceiling
 */
void* eval_ceiling(void* args[]) {
  ElmFloat* f = args[0];
  f64 result = ceil(f->value);
  return newElmInt(result);
}
Closure Basics_ceiling = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_ceiling,
    .max_values = 1,
};

/**
 * not
 */
void* eval_not(void* args[]) {
  return (args[0] == &False) ? &True : &False;
}
Closure Basics_not = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_not,
    .max_values = 1,
};

/**
 * and
 */
void* eval_and(void* args[]) {
  return (args[0] == &True && args[1] == &True) ? &True : &False;
}
Closure Basics_and = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_and,
    .max_values = 2,
};

/**
 * or
 */
void* eval_or(void* args[]) {
  return (args[0] == &True || args[1] == &True) ? &True : &False;
}
Closure Basics_or = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_or,
    .max_values = 2,
};

/**
 * xor
 */
void* eval_xor(void* args[]) {
  return (args[0] != args[1]) ? &True : &False;
}
Closure Basics_xor = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_xor,
    .max_values = 2,
};

/**
 * modBy
 */
void* eval_remainderBy(void* args[]) {
  ElmInt* den = args[0];
  ElmInt* num = args[1];
  return newElmInt((i64)num->value % (i64)den->value);
}
Closure Basics_remainderBy = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_remainderBy,
    .max_values = 2,
};

/**
 * modBy
 * https://www.microsoft.com/en-us/research/wp-content/uploads/2016/02/divmodnote-letter.pdf
 */
void* eval_modBy(void* args[]) {
  ElmInt* a0 = args[0];
  ElmInt* a1 = args[1];
  i32 modulus = a0->value;
  i32 x = a1->value;

  assert(modulus != 0);
  i32 answer = x % modulus;

  return newElmInt(((answer > 0 && modulus < 0) || (answer < 0 && modulus > 0))
                       ? answer + modulus
                       : answer);
}
Closure Basics_modBy = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_modBy,
    .max_values = 2,
};

/**
 * log
 */
void* eval_log(void* args[]) {
  ElmFloat* x = args[0];
  // Type uncertainty due to Number literals generated as Int. Resolved by int-as-float
  return newElmFloat(log(x->value));
}
Closure Basics_log = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_log,
    .max_values = 1,
};

/**
 * identity
 */
void* eval_identity(void* args[]) {
  return args[0];
}
Closure Basics_identity = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_identity,
    .max_values = 1,
};
