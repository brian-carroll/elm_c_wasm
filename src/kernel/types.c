#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"

// Global constants that don't need headers
u8 False = 0;
u8 True = 1;
u8 Unit = 0;

struct nil Nil = {
    .header = HEADER_NIL
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

// Strings are padded to the next 32-bit boundary.
// Last byte of padding is a number 1-4 telling the size of the padding itself,
// so that we can find the exact byte length of the String.
// Roughly like OCaml, https://v1.realworldocaml.org/v1/en/html/memory-representation-of-values.html#string-values
ElmString* newElmString(size_t n, char *str) {
    size_t n_ints = n/4 + 1;
    size_t n_bytes_padded = n_ints * 4;

    ElmString *p = malloc(sizeof(ElmString) + n_bytes_padded);
    p->header = HEADER_STRING(n_ints);

    u32* ints = (u32*)p->bytes;
    u32 padding = (n_bytes_padded - n) << 24; // little-endian => high byte goes last
    ints[n_ints-1] = padding;

    memcpy(p->bytes, str, n);
    return p;
}
