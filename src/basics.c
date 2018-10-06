#include "types.h"

static void* eval_add(void* args[2]) {
    Number *pa = args[0];
    Number *pb = args[1];
    if (pa->f.header.tag == Tag_Float) {
        f64 fa = pa->f.value;
        f64 fb = pb->f.value;
        f64 f = fa + fb;
        return newElmFloat(f);
    } else {
        i32 ia = pa->i.value;
        i32 ib = pb->i.value;
        i32 i = ia + ib;
        return newElmInt(i);
    }
}

Closure add;

void basics_init() {
    add = CLOSURE(eval_add, 2);
}
