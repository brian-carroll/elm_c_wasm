#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "../kernel/core/core.h"
#include "test.h"

int ftest(ElmFloat* actual, f64 expected) {
  return fabs(actual->value - expected) / expected < 0.001;
}

int itest(ElmInt* actual, i32 expected) {
  return (i32)actual->value == expected;
}

void test_number_binops() {
  if (verbose) {
    safe_printf("\n");
    safe_printf("## Numerical binary operators\n");
    safe_printf("\n");
  }

  ElmInt* i1 = newElmInt(1);
  ElmInt* i2 = newElmInt(2);
  ElmInt* im = newElmInt(-1);

  ElmInt* ibig1 = newElmInt(123456);
  ElmInt* ibig2 = newElmInt(654321);
  ElmInt* imbig1 = newElmInt(-123456);
  ElmInt* imbig2 = newElmInt(-654321);

  ElmFloat* f1 = newElmFloat(1.0);
  ElmFloat* f2 = newElmFloat(3.14);
  ElmFloat* fm = newElmFloat(-123.456);

  // ADD

  mu_assert("Expect: 1+1=2", itest(A2(&Basics_add, i1, i1), 2));
  mu_assert("Expect: 1+(-1)=0", itest(A2(&Basics_add, i1, im), 0));
  mu_assert("Expect: 2+1=2", itest(A2(&Basics_add, i2, i1), 3));
  mu_assert("Expect: 1+2=3", itest(A2(&Basics_add, i1, i2), 3));
  mu_assert("Expect: 123456+654321=777777", itest(A2(&Basics_add, ibig1, ibig2), 777777));

  mu_assert("Expect: 1.0 + 3.14 = 4.14", ftest(A2(&Basics_add, f1, f2), 4.14));
  mu_assert(
      "Expect: 1.0 + (-123.456) = -122.456", ftest(A2(&Basics_add, f1, fm), -122.456));

  // SUB

  mu_assert("Expect: 1-1=0", itest(A2(&Basics_sub, i1, i1), 0));
  mu_assert("Expect: 2-1=1", itest(A2(&Basics_sub, i2, i1), 1));
  mu_assert("Expect: 1-2=-1", itest(A2(&Basics_sub, i1, i2), -1));
  mu_assert(
      "Expect: 123456-654321=-530865", itest(A2(&Basics_sub, ibig1, ibig2), -530865));

  mu_assert("Expect: 1.0 - 3.14 = -2.14", ftest(A2(&Basics_sub, f1, f2), -2.14));
  mu_assert(
      "Expect: 1.0 - (-123.456) = 124.456", ftest(A2(&Basics_sub, f1, fm), 124.456));

  // MUL

  mu_assert("Expect: 1*1=1", itest(A2(&Basics_mul, i1, i1), 1));
  mu_assert("Expect: 2*1=2", itest(A2(&Basics_mul, i2, i1), 2));
  mu_assert("Expect: -1*2=-2", itest(A2(&Basics_mul, im, i2), -2));
  mu_assert("Expect: 2*654321=1308642", itest(A2(&Basics_mul, i2, ibig2), 1308642));

  mu_assert("Expect: 1.0 * 3.14 = 3.14", ftest(A2(&Basics_mul, f1, f2), 3.14));
  mu_assert("Expect: 3.14 * (-123.456) = -387.65184",
      ftest(A2(&Basics_sub, f1, fm), -387.65184));

  // FDIV
  mu_assert("Expect: 1.0/1.0=1.0", ftest(A2(&Basics_fdiv, f1, f1), 1.0));
  mu_assert("Expect: -123.456/3.14=-39.317", ftest(A2(&Basics_fdiv, fm, f2), -39.317));

  // IDIV
  mu_assert("Expect: 1 // 1 = 1", itest(A2(&Basics_idiv, i1, i1), 1));
  mu_assert("Expect: 123456 // 2 = 61728", itest(A2(&Basics_idiv, ibig1, i2), 61728));
  mu_assert(
      "Expect: -654321 // -1 = 654321", itest(A2(&Basics_idiv, imbig2, im), 654321));

  // POW

  mu_assert("Expect: 1^2=1", itest(A2(&Basics_pow, i1, i2), 1));
  mu_assert("Expect: 2^1=2", itest(A2(&Basics_pow, i2, i1), 2));
  mu_assert("Expect: -1^2=1", itest(A2(&Basics_pow, im, i2), 1));
  mu_assert("Expect: 2^-1=0", itest(A2(&Basics_pow, i2, im), 0));
  mu_assert("Expect: 2^0=1", itest(A2(&Basics_pow, i2, newElmInt(0)), 1));
  mu_assert("Expect: 1^123456=1", itest(A2(&Basics_pow, i1, ibig1), 1));
  mu_assert("Expect: 1^-123456=1", itest(A2(&Basics_pow, i1, imbig1), 1));
  mu_assert("Expect: 1^-654321=1", itest(A2(&Basics_pow, i1, imbig2), 1));
  mu_assert("Expect: -1^-123456=1", itest(A2(&Basics_pow, im, imbig1), 1));
  mu_assert("Expect: -1^-654321=-1", itest(A2(&Basics_pow, im, imbig2), -1));

  mu_assert("Expect: 1.0 ^ 3.14 = 1.0", ftest(A2(&Basics_pow, f1, f2), 1.0));
  mu_assert("Expect: 3.14 ^ 1.0 = 3.14", ftest(A2(&Basics_pow, f2, f1), 3.14));

  ElmInt* i3 = newElmInt(3);
  ElmInt* i8 = newElmInt(8);
  ElmInt* im3 = newElmInt(-3);
  ElmInt* im8 = newElmInt(-8);

  mu_assert("modBy 3 8 == 2", itest(A2(&Basics_modBy, i3, i8), 2));
  mu_assert("modBy 3 -8 == 1", itest(A2(&Basics_modBy, i3, im8), 1));
  mu_assert("modBy -3 -8 == -2", itest(A2(&Basics_modBy, im3, im8), -2));
  mu_assert("modBy -3 8 == -1", itest(A2(&Basics_modBy, im3, i8), -1));

  mu_assert("remainderBy 3 8 == 2", itest(A2(&Basics_remainderBy, i3, i8), 2));
  mu_assert("remainderBy 3 -8 == -2", itest(A2(&Basics_remainderBy, i3, im8), -2));
  mu_assert("remainderBy -3 -8 == -2", itest(A2(&Basics_remainderBy, im3, im8), -2));
  mu_assert("remainderBy -3 8 == 2", itest(A2(&Basics_remainderBy, im3, i8), 2));
}

void test_number_unops() {
  if (verbose) {
    safe_printf("\n");
    safe_printf("## Numerical unary operators\n");
    safe_printf("\n");
  }
  // Basics.negate
  mu_assert("-(3.14) == -3.14", ftest(A1(&Basics_negate, newElmFloat(3.14)), -3.14));
  mu_assert("-(-3.14) == 3.14", ftest(A1(&Basics_negate, newElmFloat(-3.14)), 3.14));
  mu_assert("-(123) == -123", itest(A1(&Basics_negate, newElmInt(-123)), 123));
  mu_assert("-(-123) == 123", itest(A1(&Basics_negate, newElmInt(123)), -123));

  // Basics.toFloat
  mu_assert(
      "toFloat -123 == -123.0", ftest(A1(&Basics_toFloat, newElmInt(-123)), -123.0));

  // Basics.floor
  mu_assert("floor 3.9 == 3", itest(A1(&Basics_floor, newElmFloat(3.9)), 3));
  mu_assert("floor -3.14 == -4", itest(A1(&Basics_floor, newElmFloat(-3.14)), -4));

  // Basics.ceiling
  mu_assert("ceiling 3.14 == 4", itest(A1(&Basics_ceiling, newElmFloat(3.14)), 4));
  mu_assert("ceiling -4.9 == -4", itest(A1(&Basics_ceiling, newElmFloat(-4.9)), -4));

  // Basics_log
  mu_assert("log 2.71828 == 1.0", ftest(A1(&Basics_log, newElmFloat(2.71828)), 1.0));
}

void test_logical_ops() {
  if (verbose) {
    safe_printf("\n");
    safe_printf("## Logical operators\n");
    safe_printf("\n");
  }

  // not
  mu_assert("not True == False", A1(&Basics_not, &True) == &False);
  mu_assert("not False == True", A1(&Basics_not, &False) == &True);

  // and
  mu_assert("False && False == False", A2(&Basics_and, &False, &False) == &False);
  mu_assert("False && True == False", A2(&Basics_and, &False, &True) == &False);
  mu_assert("True && False == False", A2(&Basics_and, &True, &False) == &False);
  mu_assert("True && True == True", A2(&Basics_and, &True, &True) == &True);

  // or
  mu_assert("False || False == False", A2(&Basics_or, &False, &False) == &False);
  mu_assert("False || True == True", A2(&Basics_or, &False, &True) == &True);
  mu_assert("True || False == True", A2(&Basics_or, &True, &False) == &True);
  mu_assert("True || True == True", A2(&Basics_or, &True, &True) == &True);
}

void basics_test() {
  if (verbose) {
    safe_printf("\n\n\n");
    safe_printf("####################################################\n");
    safe_printf("\n");
    safe_printf("Basics\n");
    safe_printf("------\n");
  }

  mu_run_test(test_number_binops);
  mu_run_test(test_number_unops);
  mu_run_test(test_logical_ops);
}
