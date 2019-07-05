#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../kernel/types.h"
#include "../kernel/basics.h"
#include "../kernel/utils.h"
#include "./test.h"

int test_float_val(ElmFloat *actual, f64 expected)
{
    return fabs(actual->value - expected) / expected < 0.001;
}

int test_int_val(ElmInt *actual, i32 expected)
{
    return actual->value == expected;
}

char *test_number_binops()
{

    ElmInt *i1 = NEW_ELM_INT(1);
    ElmInt *i2 = NEW_ELM_INT(2);
    ElmInt *im = NEW_ELM_INT(-1);
    ElmInt *ibig1 = NEW_ELM_INT(123456);
    ElmInt *ibig2 = NEW_ELM_INT(654321);
    ElmInt *imbig1 = NEW_ELM_INT(-123456);
    ElmInt *imbig2 = NEW_ELM_INT(-654321);

    ElmFloat *f1 = NEW_ELM_FLOAT(1.0);
    ElmFloat *f2 = NEW_ELM_FLOAT(3.14);
    ElmFloat *fm = NEW_ELM_FLOAT(-123.456);

    // ADD

    mu_assert("Expect: 1+1=2", test_int_val(A2(&Basics_add, i1, i1), 2));
    mu_assert("Expect: 1+(-1)=0", test_int_val(A2(&Basics_add, i1, im), 0));
    mu_assert("Expect: 2+1=2", test_int_val(A2(&Basics_add, i2, i1), 3));
    mu_assert("Expect: 1+2=3", test_int_val(A2(&Basics_add, i1, i2), 3));
    mu_assert("Expect: 123456+654321=777777",
              test_int_val(A2(&Basics_add, ibig1, ibig2), 777777));

    mu_assert("Expect: 1.0 + 3.14 = 4.14", test_float_val(A2(&Basics_add, f1, f2), 4.14));
    mu_assert("Expect: 1.0 + (-123.456) = -122.456", test_float_val(A2(&Basics_add, f1, fm), -122.456));

    // SUB

    mu_assert("Expect: 1-1=0", test_int_val(A2(&Basics_sub, i1, i1), 0));
    mu_assert("Expect: 2-1=1", test_int_val(A2(&Basics_sub, i2, i1), 1));
    mu_assert("Expect: 1-2=-1", test_int_val(A2(&Basics_sub, i1, i2), -1));
    mu_assert("Expect: 123456-654321=-530865",
              test_int_val(A2(&Basics_sub, ibig1, ibig2), -530865));

    mu_assert("Expect: 1.0 - 3.14 = -2.14", test_float_val(A2(&Basics_sub, f1, f2), -2.14));
    mu_assert("Expect: 1.0 - (-123.456) = 124.456", test_float_val(A2(&Basics_sub, f1, fm), 124.456));

    // MUL

    mu_assert("Expect: 1*1=1", test_int_val(A2(&Basics_mul, i1, i1), 1));
    mu_assert("Expect: 2*1=2", test_int_val(A2(&Basics_mul, i2, i1), 2));
    mu_assert("Expect: -1*2=-2", test_int_val(A2(&Basics_mul, im, i2), -2));
    mu_assert("Expect: 2*654321=1308642",
              test_int_val(A2(&Basics_mul, i2, ibig2), 1308642));

    mu_assert("Expect: 1.0 * 3.14 = 3.14", test_float_val(A2(&Basics_mul, f1, f2), 3.14));
    mu_assert("Expect: 3.14 * (-123.456) = -387.65184", test_float_val(A2(&Basics_sub, f1, fm), -387.65184));

    // POW

    mu_assert("Expect: 1^2=1", test_int_val(A2(&Basics_pow, i1, i2), 1));
    mu_assert("Expect: 2^1=2", test_int_val(A2(&Basics_pow, i2, i1), 2));
    mu_assert("Expect: -1^2=1", test_int_val(A2(&Basics_pow, im, i2), 1));
    mu_assert("Expect: 2^-1=0", test_int_val(A2(&Basics_pow, i2, im), 0));
    mu_assert("Expect: 2^0=1", test_int_val(A2(&Basics_pow, i2, NEW_ELM_INT(0)), 1));
    mu_assert("Expect: 1^123456=1", test_int_val(A2(&Basics_pow, i1, ibig1), 1));
    mu_assert("Expect: 1^-123456=1", test_int_val(A2(&Basics_pow, i1, imbig1), 1));
    mu_assert("Expect: 1^-654321=1", test_int_val(A2(&Basics_pow, i1, imbig2), 1));
    mu_assert("Expect: -1^-123456=1", test_int_val(A2(&Basics_pow, im, imbig1), 1));
    mu_assert("Expect: -1^-654321=-1", test_int_val(A2(&Basics_pow, im, imbig2), -1));

    mu_assert("Expect: 1.0 ^ 3.14 = 1.0", test_float_val(A2(&Basics_pow, f1, f2), 1.0));
    mu_assert("Expect: 3.14 ^ 1.0 = 3.14", test_float_val(A2(&Basics_pow, f2, f1), 3.14));

    return NULL;
}

char *basics_test()
{
    mu_run_test(test_number_binops);
    return NULL;
}
