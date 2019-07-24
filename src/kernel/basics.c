#include "basics.h"
#include <math.h>
#include "types.h"

i32 ipow(i32 base, i32 ex) {
  if (ex < 0) {
    if (base == 1) {
      return 1;
    } else if (base == -1) {
      return (ex & 1) ? -1 : 1;
    } else {
      return 0;
    }
  }

  i32 result = 1;
  while (1) {  // < 32 iterations since ex >= 0
    if (ex & 1) {
      result *= base;
    }
    ex >>= 1;
    if (!ex) {
      break;
    }
    base *= base;
  }
  return result;
}

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
F2(Basics_add, add_eval);

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
F2(Basics_sub, sub_eval);

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
F2(Basics_mul, mul_eval);

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
F2(Basics_pow, pow_eval);
