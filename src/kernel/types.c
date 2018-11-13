#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"

Header Nil = HEADER_NIL;

Cons* newCons(void* head, void* tail) {
    Cons *p = malloc(sizeof(Cons));
    if (p == NULL) return pGcFull;
    p->header = HEADER_CONS;
    p->head = head;
    p->tail = tail;
    return p;
};

Tuple2* newTuple2(void* a, void* b) {
    Tuple2 *p = malloc(sizeof(Tuple2));
    if (p == NULL) return pGcFull;
    p->header = HEADER_TUPLE2;
    p->a = a;
    p->b = b;
    return p;
};

Tuple3* newTuple3(void* a, void* b, void* c) {
    Tuple3 *p = malloc(sizeof(Tuple3));
    if (p == NULL) return pGcFull;
    p->header = HEADER_TUPLE3;
    p->a = a;
    p->b = b;
    p->c = c;
    return p;
};

ElmInt* newElmInt(i32 value) {
    ElmInt *p = malloc(sizeof(ElmInt));
    if (p == NULL) return pGcFull;
    p->header = HEADER_INT;
    p->value = value;
    return p;
};

ElmFloat* newElmFloat(f64 value) {
    ElmFloat *p = malloc(sizeof(ElmFloat));
    if (p == NULL) return pGcFull;
    p->header = HEADER_FLOAT;
    p->value = value;
    return p;
};

ElmChar* newElmChar(u32 value) {
    ElmChar *p = malloc(sizeof(ElmChar));
    if (p == NULL) return pGcFull;
    p->header = HEADER_CHAR;
    p->value = value;
    return p;
};

// Strings are padded to the next 32-bit boundary.
// Last byte of padding is a number 0-3 telling the size of the padding itself,
// so that we can find the exact byte length of the String.
// Like OCaml, https://v1.realworldocaml.org/v1/en/html/memory-representation-of-values.html#string-values
ElmString* newElmString(size_t n, char *str) {
    size_t n_ints = n/4 + 1;
    size_t n_bytes_padded = n_ints * 4;

    ElmString *p = malloc(sizeof(ElmString) + n_bytes_padded);
    if (p == NULL) return pGcFull;
    p->header = HEADER_STRING(n_ints);

    u32* ints = (u32*)p->bytes;
    u32 padding = (n_bytes_padded - n -1) << 24; // little-endian => high byte goes last
    ints[n_ints-1] = padding;

    // Some String functions don't want to provide an initial value yet
    if (str != NULL) {
        memcpy(p->bytes, str, n);
    }
    return p;
}


void Types_init() {
    Unit = (Custom){ .header = HEADER_CUSTOM(0), .ctor = 0 };

    True = (Custom){ .header = HEADER_CUSTOM(0), .ctor = 1 };
    False = (Custom){ .header = HEADER_CUSTOM(0), .ctor = 0 };

    GcFull = (GcContinuation){
        .header = HEADER_GCFULL,
        .continuation = NULL
    };
    pGcFull = &GcFull;
}
