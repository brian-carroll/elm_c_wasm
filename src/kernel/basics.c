#include "basics.h"
#include <math.h>

static void* and_eval(void* args[2]) {
  return (args[0] == &True && args[1] == &True) ? &True : &False;
}
Closure Basics_and = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &and_eval,
    .max_values = 2,
};

static void* or_eval(void* args[2]) {
  return (args[0] == &True || args[1] == &True) ? &True : &False;
}
Closure Basics_or = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &or_eval,
    .max_values = 2,
};

static void* add_eval(void* args[2]) {
  Number* pa = args[0];
  Number* pb = args[1];
  if (pa->f.header.tag == Tag_Float) {
    f64 fa = pa->f.value;
    f64 fb = pb->f.value;
    f64 f = fa + fb;
    return NEW_ELM_FLOAT(f);
  } else {
    i32 ia = pa->i.value;
    i32 ib = pb->i.value;
    i32 i = ia + ib;
    return NEW_ELM_INT(i);
  }
}
Closure Basics_add = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &add_eval,
    .max_values = 2,
};

static void* sub_eval(void* args[2]) {
  Number* pa = args[0];
  Number* pb = args[1];
  if (pa->f.header.tag == Tag_Float) {
    f64 fa = pa->f.value;
    f64 fb = pb->f.value;
    f64 f = fa - fb;
    return NEW_ELM_FLOAT(f);
  } else {
    i32 ia = pa->i.value;
    i32 ib = pb->i.value;
    i32 i = ia - ib;
    return NEW_ELM_INT(i);
  }
}
Closure Basics_sub = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &sub_eval,
    .max_values = 2,
};

static void* mul_eval(void* args[2]) {
  Number* pa = args[0];
  Number* pb = args[1];
  if (pa->f.header.tag == Tag_Float) {
    f64 fa = pa->f.value;
    f64 fb = pb->f.value;
    f64 f = fa * fb;
    return NEW_ELM_FLOAT(f);
  } else {
    i32 ia = pa->i.value;
    i32 ib = pb->i.value;
    i32 i = ia * ib;
    return NEW_ELM_INT(i);
  }
}
Closure Basics_mul = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &mul_eval,
    .max_values = 2,
};

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
static void* pow_eval(void* args[2]) {
  Number* pa = args[0];
  Number* pb = args[1];
  if (pa->f.header.tag == Tag_Float) {
    f64 fa = pa->f.value;
    f64 fb = pb->f.value;
    f64 f = pow(fa, fb);
    return NEW_ELM_FLOAT(f);
  } else {
    i32 ia = pa->i.value;
    i32 ib = pb->i.value;
    i32 i = ipow(ia, ib);
    return NEW_ELM_INT(i);
  }
}
Closure Basics_pow = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &pow_eval,
    .max_values = 2,
};
