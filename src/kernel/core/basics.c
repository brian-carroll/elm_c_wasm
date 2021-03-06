#include "core.h"

#include <assert.h>
#include <math.h>

/**
 * negate
 * Elm version of Basics_negate ends up being self-referential!
 * Have to break the cycle using a kernel version
 */
static void* eval_negate(void* args[]) {
  Number* x = args[0];
  if (x->f.header.tag == Tag_Float) {
    f64 val = x->f.value;
    return newElmFloat(-val);
  } else {
    i32 val = x->i.value;
    return newElmInt(-val);
  }
}
Closure Basics_negate = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_negate,
    .max_values = 1,
};

/**
 * add
 */
static void* eval_add(void* args[2]) {
  Number* pa = args[0];
  Number* pb = args[1];
  Tag ta = pa->f.header.tag;
  Tag tb = pb->f.header.tag;
  // Compiler can generate Number literals so we can't rely on both being the same type!
  if (ta == Tag_Float) {
    if (tb == Tag_Float)
      return newElmFloat(pa->f.value + pb->f.value);
    else
      return newElmFloat(pa->f.value + (f64)pb->i.value);
  } else {
    if (tb == Tag_Float)
      return newElmFloat((f64)pa->i.value + pb->f.value);
    else
      return newElmInt(pa->i.value + pb->i.value);
  }
}
Closure Basics_add = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_add,
    .max_values = 2,
};

/**
 * sub
 */
static void* eval_sub(void* args[2]) {
  Number* pa = args[0];
  Number* pb = args[1];
  Tag ta = pa->f.header.tag;
  Tag tb = pb->f.header.tag;
  // Compiler can generate Number literals so we can't rely on both being the same type!
  if (ta == Tag_Float) {
    if (tb == Tag_Float)
      return newElmFloat(pa->f.value - pb->f.value);
    else
      return newElmFloat(pa->f.value - (f64)pb->i.value);
  } else {
    if (tb == Tag_Float)
      return newElmFloat((f64)pa->i.value - pb->f.value);
    else
      return newElmInt(pa->i.value - pb->i.value);
  }
}
Closure Basics_sub = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_sub,
    .max_values = 2,
};

/**
 * mul
 */
static void* eval_mul(void* args[2]) {
  Number* pa = args[0];
  Number* pb = args[1];
  Tag ta = pa->f.header.tag;
  Tag tb = pb->f.header.tag;
  if (ta == Tag_Float) {
    if (tb == Tag_Float)
      return newElmFloat(pa->f.value * pb->f.value);
    else
      return newElmFloat(pa->f.value * (f64)pb->i.value);
  } else {
    if (tb == Tag_Float)
      return newElmFloat((f64)pa->i.value * pb->f.value);
    else
      return newElmInt(pa->i.value * pb->i.value);
  }
}
Closure Basics_mul = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_mul,
    .max_values = 2,
};

/**
 * fdiv
 */
static void* eval_fdiv(void* args[2]) {
  Number* pa = args[0];
  Number* pb = args[1];
  Tag ta = pa->f.header.tag;
  Tag tb = pb->f.header.tag;
  // Compiler generates round Number literals as Int, so we may need to implicitly convert like JS does
  // Core library tests fail without this
  if (ta == Tag_Float) {
    if (tb == Tag_Float)
      return newElmFloat(pa->f.value / pb->f.value);
    else
      return newElmFloat(pa->f.value / (f64)pb->i.value);
  } else {
    if (tb == Tag_Float)
      return newElmFloat((f64)pa->i.value / pb->f.value);
    else
      return newElmFloat((f64)pa->i.value / (f64)pb->i.value);
  }
}
Closure Basics_fdiv = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_fdiv,
    .max_values = 2,
};

/**
 * idiv
 */
static void* eval_idiv(void* args[2]) {
  ElmInt* ia = args[0];
  ElmInt* ib = args[1];
  i32 result = ia->value / ib->value;
  return newElmInt(result);
}
Closure Basics_idiv = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_idiv,
    .max_values = 2,
};

/**
 * pow
 */
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
static void* eval_pow(void* args[2]) {
  Number* pa = args[0];
  Number* pb = args[1];
  if (pa->f.header.tag == Tag_Float) {
    f64 fa = pa->f.value;
    f64 fb = pb->f.value;
    f64 f = pow(fa, fb);
    return newElmFloat(f);
  } else {
    i32 ia = pa->i.value;
    i32 ib = pb->i.value;
    i32 i = ipow(ia, ib);
    return newElmInt(i);
  }
}
Closure Basics_pow = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_pow,
    .max_values = 2,
};

/*
 * toFloat
 */
static void* eval_toFloat(void* args[]) {
  ElmInt* i = args[0];
  if (i->header.tag == Tag_Float) return i;
  return newElmFloat((f64)i->value);
}
Closure Basics_toFloat = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_toFloat,
    .max_values = 1,
};

/*
 * round
 */
static void* eval_round(void* args[]) {
  ElmFloat* f = args[0];
  if (f->header.tag == Tag_Int) return f;
  f64 result = round(f->value);
  return newElmInt((i32)result);
}
Closure Basics_round = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_round,
    .max_values = 1,
};

/*
 * floor
 */
static void* eval_floor(void* args[]) {
  ElmFloat* f = args[0];
  if (f->header.tag == Tag_Int) return f;
  f64 result = floor(f->value);
  return newElmInt((i32)result);
}
Closure Basics_floor = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_floor,
    .max_values = 1,
};

/*
 * ceiling
 */
static void* eval_ceiling(void* args[]) {
  ElmFloat* f = args[0];
  if (f->header.tag == Tag_Int) return f;
  f64 result = ceil(f->value);
  return newElmInt((i32)result);
}
Closure Basics_ceiling = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_ceiling,
    .max_values = 1,
};

/**
 * not
 */
static void* eval_not(void* args[2]) {
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
static void* eval_and(void* args[2]) {
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
static void* eval_or(void* args[2]) {
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
static void* eval_xor(void* args[2]) {
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
static void* eval_remainderBy(void* args[]) {
  ElmInt* den = args[0];
  ElmInt* num = args[1];
  return newElmInt(num->value % den->value);
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
static void* eval_modBy(void* args[]) {
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
static void* eval_log(void* args[]) {
  Number* x = args[0];
  // Type uncertainty due to Number literals generated as Int
  f64 value = x->f.header.tag == Tag_Float ? x->f.value : (f64)x->i.value;
  return newElmFloat(log(value));
}
Closure Basics_log = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_log,
    .max_values = 1,
};

/**
 * identity
 */
static void* eval_identity(void* args[]) {
  return args[0];
}
Closure Basics_identity = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_identity,
    .max_values = 1,
};
