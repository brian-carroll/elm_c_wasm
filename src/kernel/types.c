#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"


u8 False = 0;
u8 True = 1;
u8 Unit = 0;

struct nil Nil = {
    .header = HEADER_NIL  // not on the dynamic heap, but has a header anyway
};

Cons* newCons(void* head, void* tail) {
    Cons *p = malloc(sizeof(Cons));
    p->header = HEADER_CONS;
    p->head = head;
    p->tail = tail;
    return p;
};

Tuple2* newTuple2(void* a, void* b) {
    Tuple2 *p = malloc(sizeof(Tuple2));
    p->header = HEADER_TUPLE2;
    p->a = a;
    p->b = b;
    return p;
};

Tuple3* newTuple3(void* a, void* b, void* c) {
    Tuple3 *p = malloc(sizeof(Tuple3));
    p->header = HEADER_TUPLE3;
    p->a = a;
    p->b = b;
    p->c = c;
    return p;
};

ElmInt* newElmInt(i32 value) {
    ElmInt *p = malloc(sizeof(ElmInt));
    p->header = HEADER_INT;
    p->value = value;
    return p;
};

ElmFloat* newElmFloat(f64 value) {
    ElmFloat *p = malloc(sizeof(ElmFloat));
    p->header = HEADER_FLOAT;
    p->value = value;
    return p;
};


ElmChar* newElmChar(u32 value) {
    ElmChar *p = malloc(sizeof(ElmChar));
    p->header = HEADER_CHAR;
    p->value = value;
    return p;
};

ElmString* newElmString(size_t n, char *str) {
    // Pad to next 32-bit boundary
    size_t n_ints = n/4 + 1;
    size_t n_bytes_padded = n_ints * 4;

    ElmString *p = malloc(sizeof(ElmString) + n_bytes_padded);
    p->header = HEADER_STRING(n_ints);

    // Last byte of the padding contains its size in bytes
    u32* ints = (u32*)p->bytes;
    u32 padding = (n_bytes_padded - n) << 24; // padding size, with 3 bytes of leading zeros (little-endian)
    ints[n_ints-1] = padding;

    // Copy string content (overwrites some of the padding and that's OK)
    memcpy(p->bytes, str, n);
    return p;
}

// // Sanity check
// void main(void) {
//     newCons(&Unit, &Nil);
//     newTuple2(&Unit, &Unit);
//     newTuple3(&Unit, &Unit, &Unit);
//     newElmInt(123);
//     newElmFloat(123.456);
//     newElmChar('A');
//     newElmString(11, "hello world");
//     printf("OK\n");
// }
