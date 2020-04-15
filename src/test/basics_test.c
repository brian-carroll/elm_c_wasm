#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "../kernel/kernel.h"
#include "./test.h"

int ftest(ElmFloat* actual, f64 expected) {
  return fabs(actual->value - expected) / expected < 0.001;
}

int itest(ElmInt* actual, i32 expected) {
  return actual->value == expected;
}

char* test_number_binops() {
  if (verbose) {
    printf("\n");
    printf("## Numerical binary operators\n");
    printf("\n");
  }

  ElmInt* i1 = NEW_ELM_INT(1);
  ElmInt* i2 = NEW_ELM_INT(2);
  ElmInt* im = NEW_ELM_INT(-1);
  ElmInt* ibig1 = NEW_ELM_INT(123456);
  ElmInt* ibig2 = NEW_ELM_INT(654321);
  ElmInt* imbig1 = NEW_ELM_INT(-123456);
  ElmInt* imbig2 = NEW_ELM_INT(-654321);

  ElmFloat* f1 = NEW_ELM_FLOAT(1.0);
  ElmFloat* f2 = NEW_ELM_FLOAT(3.14);
  ElmFloat* fm = NEW_ELM_FLOAT(-123.456);

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
  mu_assert("Expect: 2^0=1", itest(A2(&Basics_pow, i2, NEW_ELM_INT(0)), 1));
  mu_assert("Expect: 1^123456=1", itest(A2(&Basics_pow, i1, ibig1), 1));
  mu_assert("Expect: 1^-123456=1", itest(A2(&Basics_pow, i1, imbig1), 1));
  mu_assert("Expect: 1^-654321=1", itest(A2(&Basics_pow, i1, imbig2), 1));
  mu_assert("Expect: -1^-123456=1", itest(A2(&Basics_pow, im, imbig1), 1));
  mu_assert("Expect: -1^-654321=-1", itest(A2(&Basics_pow, im, imbig2), -1));

  mu_assert("Expect: 1.0 ^ 3.14 = 1.0", ftest(A2(&Basics_pow, f1, f2), 1.0));
  mu_assert("Expect: 3.14 ^ 1.0 = 3.14", ftest(A2(&Basics_pow, f2, f1), 3.14));

  return NULL;
}

char* test_number_unops() {
  if (verbose) {
    printf("\n");
    printf("## Numerical unary operators\n");
    printf("\n");
  }
  // Basics.negate
  mu_assert("-(3.14) == -3.14", ftest(A1(&Basics_negate, NEW_ELM_FLOAT(3.14)), -3.14));
  mu_assert("-(-3.14) == 3.14", ftest(A1(&Basics_negate, NEW_ELM_FLOAT(-3.14)), 3.14));
  mu_assert("-(123) == -123", itest(A1(&Basics_negate, NEW_ELM_INT(-123)), 123));
  mu_assert("-(-123) == 123", itest(A1(&Basics_negate, NEW_ELM_INT(123)), -123));

  // Basics.toFloat
  mu_assert("toFloat -123 == -123.0", ftest(A1(&Basics_toFloat, NEW_ELM_INT(-123)), -123.0));

  // Basics.floor
  mu_assert("floor 3.9 == 3", itest(A1(&Basics_floor, NEW_ELM_FLOAT(3.9)), 3));
  mu_assert("floor -3.14 == -4", itest(A1(&Basics_floor, NEW_ELM_FLOAT(-3.14)), -4));

  // Basics.ceiling
  mu_assert("ceiling 3.14 == 4", itest(A1(&Basics_ceiling, NEW_ELM_FLOAT(3.14)), 4));
  mu_assert("ceiling -4.9 == -4", itest(A1(&Basics_ceiling, NEW_ELM_FLOAT(-4.9)), -4));

  // Basics_log
  mu_assert("log 2.71828 == 1.0", ftest(A1(&Basics_log, NEW_ELM_FLOAT(2.71828)), 1.0));

  return NULL;
}

char* basics_test() {
  if (verbose) {
    printf("\n\n\n");
    printf("####################################################\n");
    printf("\n");
    printf("Basics\n");
    printf("------\n");
  }

  mu_run_test(test_number_binops);
  mu_run_test(test_number_unops);
  return NULL;
}
