#include <math.h>
#include "types.h"
#include "basics.h"

i32 ipow(i32 base, i32 exp) {
    if (exp < 0) {
        if (base == 1) {
            return 1;
        } else if (base == -1) {
            return (exp & 1) ? -1 : 1;
        } else {
            return 0;
        }
    }

    i32 result = 1;
    while (1) { // <32 iterations since exp>=0
        if (exp & 1) {
            result *= base;
        }
        exp >>= 1;
        if (!exp) {
            break;
        }
        base *= base;
    }
    return result;
}

#define BASICS_NUMBER_BINOP(funcname, f_op, i_op) \
static void* funcname(void* args[2]) {  \
    Number *pa = args[0];   \
    Number *pb = args[1];   \
    if (pa->f.header.tag == Tag_Float) {    \
        f64 fa = pa->f.value;   \
        f64 fb = pb->f.value;   \
        f64 f = f_op;    \
        return newElmFloat(f);  \
    } else {    \
        i32 ia = pa->i.value;   \
        i32 ib = pb->i.value;   \
        i32 i = i_op;    \
        return newElmInt(i);    \
    }   \
}

BASICS_NUMBER_BINOP(eval_add, fa + fb, ia + ib)
BASICS_NUMBER_BINOP(eval_sub, fa - fb, ia - ib)
BASICS_NUMBER_BINOP(eval_mul, fa * fb, ia * ib)
BASICS_NUMBER_BINOP(eval_pow, pow(fa, fb), ipow(ia,ib))

Closure Basics_add;
Closure Basics_sub;
Closure Basics_mul;
Closure Basics_pow;

void init_basics() {
    Basics_add = F2(eval_add);
    Basics_sub = F2(eval_sub);
    Basics_mul = F2(eval_mul);
    Basics_pow = F2(eval_pow);
}
