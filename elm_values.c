#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Primitive types

typedef char i8;
typedef short i16;
typedef int i32;
typedef long int i64;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long int u64;

typedef float f32;
typedef double f64;


// Constants
// using 'const' qualifier is pointless because we cast pointers to void*
u8 False = 0;
u8 True = 1;
u8 Unit = 0;


// Comparable constructor values
// Enum is 4 bytes, not just 1. But in a struct, C compiler will pad to 4 bytes anyway.
// Later we may use the extra bytes for a GC header
typedef enum {
    Ctor_Nil,
    Ctor_Cons,
    Ctor_Tuple2,
    Ctor_Tuple3,
    Ctor_Int,
    Ctor_Float,
    Ctor_Char,
    Ctor_String,
} Ctor_Comp;


// List

struct nil {
    Ctor_Comp ctor;
};
struct nil Nil = {
    .ctor = Ctor_Nil
};

typedef struct {
    Ctor_Comp ctor;
    void* head;
    void* tail;
} Cons;

#define CONS(h,t) { .ctor = Ctor_Cons, .head = h, .tail = t }

Cons* newList_Cons(void* head, void* tail) {
    Cons *p = malloc(sizeof(Cons));
    p->ctor = Ctor_Cons;
    p->head = head;
    p->tail = tail;
    return p;
};


// Tuples

typedef struct {
    Ctor_Comp ctor;
    void* a;
    void* b;
} Tuple2;

#define TUPLE2(a,b) { .ctor = Ctor_Tuple2, .a = a, .b = b }

Tuple2* newTuple2(void* a, void* b) {
    Tuple2 *p = malloc(sizeof(Tuple2));
    p->ctor = Ctor_Tuple2;
    p->a = a;
    p->b = b;
    return p;
};

typedef struct {
    Ctor_Comp ctor;
    void* a;
    void* b;
    void* c;
} Tuple3;

#define TUPLE3(a,b,c) { .ctor = Ctor_Tuple3, .a = a, .b = b, .c = c }

Tuple3* newTuple3(void* a, void* b, void* c) {
    Tuple3 *p = malloc(sizeof(Tuple3));
    p->ctor = Ctor_Tuple3;
    p->a = a;
    p->b = b;
    p->c = c;
    return p;
};


// Numbers

typedef struct {
    Ctor_Comp ctor;
    i32 value;
} ElmInt;

#define ELM_INT(x) { .ctor = Ctor_Int, .value = x }

ElmInt* newElmInt(i32 value) {
    ElmInt *p = malloc(sizeof(ElmInt));
    p->ctor = Ctor_Int;
    p->value = value;
    return p;
};


typedef struct {
    Ctor_Comp ctor;
    f64 value;
} ElmFloat;

#define ELM_FLOAT(x) { .ctor = Ctor_Float, .value = x }

ElmFloat* newElmFloat(f64 value) {
    ElmFloat *p = malloc(sizeof(ElmFloat));
    p->ctor = Ctor_Float;
    p->value = value;
    return p;
};


// Char

typedef struct {
    Ctor_Comp ctor;
    i32 value;
} ElmChar;

#define ELM_CHAR(x) { .ctor = Ctor_Char, .value = x }

ElmChar* newElmChar(u32 value) {
    ElmChar *p = malloc(sizeof(ElmChar));
    p->ctor = Ctor_Char;
    p->value = value;
    return p;
};

// String

typedef struct {
    Ctor_Comp ctor;
    u32 len_unicode;
    u32 len_bytes;
    u8 bytes[];
} ElmString;

// string constructor function? I'm not sure what we want it to look like!


// Enums (unions with no params)
/*
 Elm compiler generates constants in memory, structures can point to them
 Choose smallest required size.
*/


// Union types
/*
 Elm compiler generates C structs, enums, consts/functions
*/

// Record

typedef struct {
    u32 size;
    u32 fields[];
} FieldSet;

typedef struct {
    FieldSet* fields;
    void* values[];
} Record;

/*
 Elm compiler generates
    field sets (global constant structs)
    record constructor functions
*/


// Closure

typedef struct {
    u8 n_values; // current number of applied args
    u8 max_values;
    void* (*evaluator)(void**); // pointer to a function that takes an array of pointers and returns a pointer
    void* values[];
} Closure;

#define CLOSURE(f, n) { .evaluator = &f, .max_values = n, .n_values = 0, .values = {} }

void* apply(Closure* c_old, u8 n_applied, void* applied[]) {
    if (c_old->max_values == n_applied) {
        // avoid allocating a new closure if we don't need it
        return (*c_old->evaluator)(applied);
    } else {
        u8 n_old = c_old->n_values;
        u8 n_new = n_old + n_applied;

        size_t size_old = sizeof(Closure) + n_old * sizeof(void*);
        size_t size_applied = n_applied * sizeof(void*);
        size_t size_new = size_old + size_applied;

        Closure *c = malloc(size_new);
        memcpy(c, c_old, size_old);
        memcpy(&c->values[n_old], applied, size_applied);
        // void* c_applied = c + size_old;
        // memcpy(c_applied, applied, size_applied);
        c->n_values = n_new;

        if (n_new == c->max_values) {
            return (*c->evaluator)(c->values);
        } else {
            return c;
        }
    }
}


typedef union {
    ElmInt i;
    ElmFloat f;
} Number;


void* eval_add(void* args[2]) {
    Number *pa = args[0];
    Number *pb = args[1];
    if (pa->f.ctor == Ctor_Float) {
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

Closure add = CLOSURE(eval_add, 2);




// This is code I could generate from the Elm compiler. I could write better by hand.
// The intermediate ElmInt allocated on heap is hard to eliminate in generated code.
// Would be nice to evaluate entire expression on raw ints, and only then box it in ElmInt
// Would require Elm compiler optimizer to identify Int-only subexpressions (also Float-only)
// so that the code generator would know what to do.
void* eval_user_project_closure(void* args[]) {
    ElmInt *outerScopeValue = args[0];
    ElmInt *arg1 = args[1];
    ElmInt *arg2 = args[2];

    ElmInt *tmp = apply(&add, 2, (void*[]){arg2, outerScopeValue});
    return apply(&add, 2, (void*[]){arg1, tmp});
}

// Constants calculated at compile time and pre-loaded into memory from Wasm data segment
Closure user_project_closure = CLOSURE(eval_user_project_closure, 3);
ElmInt outerScopeValue = ELM_INT(1);


char* hex(void* addr, int size) {
    char *str;
    int i, c=0, n=(9*size)/4;
    str = malloc(n);
    for (i=0; i < size; c += 9, i += 4) {
        // Print in actual byte order (little endian)
        sprintf(str + c,
            "%02x%02x%02x%02x|",
            *(char*)(addr + i),
            *(char*)(addr + i + 1),
            *(char*)(addr + i + 2),
            *(char*)(addr + i + 3)
        );
    }
    str[c-1] = 0; // erase last "|" and terminate the string
    return str;
}

// Print a memory address held in a pointer
//   Addresses are printed in little-endian format
//   => Easier to see what pointers are pointing to it
//   => Harder to subtract in your head if you're interested in sizes & memory layout
char* hex_ptr(void* ptr) {
    return hex(&ptr, sizeof(void*));
}



int main(int argc, char ** argv) {
    printf("sizeof(int) = %d\n", (int)sizeof(int));
    printf("sizeof(f64) = %d\n", (int)sizeof(f64));
    printf("sizeof(Ctor_Comp) = %d\n", (int)sizeof(Ctor_Comp));
    printf("\n");

    printf("False size=%ld %s %d\n", sizeof(False), hex_ptr(&False), False);
    printf("True size=%ld %s %d\n", sizeof(True), hex_ptr(&True), True);
    printf("Unit size=%ld %s %d\n", sizeof(Unit), hex_ptr(&Unit), Unit);
    printf("\n");

    printf("Nil size=%ld addr=%s ctor=%d\n", sizeof(Nil), hex_ptr(&Nil), (int)Nil.ctor);

    Cons *c = newList_Cons(&Unit, &Nil); // [()]
    printf("Cons size=%ld addr=%s ctor=%d head=%d tail=%d\n",
        sizeof(Cons), hex_ptr(c), (int)c->ctor, (int)c->head, (int)c->tail
    );
    printf("\n");

    Tuple2 *t2 = newTuple2(&Unit, &Unit); // ((),())
    printf("Tuple2 size=%ld addr=%s ctor=%d a=%d b=%d\n",
        sizeof(Tuple2), hex_ptr(t2), (int)t2->ctor, (int)t2->a, (int)t2->b
    );

    Tuple3 *t3 = newTuple3(&Unit, &Unit, &Unit); // ((),(),())
    printf("Tuple3 size=%ld addr=%s ctor=%d a=%d b=%d c=%d\n",
        sizeof(Tuple3), hex_ptr(t3), (int)t3->ctor, (int)t3->a, (int)t3->b, (int)t3->c
    );
    printf("\n");

    ElmInt *i = newElmInt(123);
    printf("Int size=%ld addr=%s ctor=%d value=%d\n",
        sizeof(ElmInt), hex_ptr(i), (int)i->ctor, i->value
    );
    ElmFloat *f = newElmFloat(123.456);
    printf("Float size=%ld addr=%s ctor=%d value=%f\n",
        sizeof(ElmFloat), hex_ptr(f), (int)f->ctor, f->value
    );

    ElmChar *ch = newElmChar('A');
    printf("Char size=%ld addr=%s ctor=%d value=%c\n",
        sizeof(ElmChar), hex_ptr(ch), (int)ch->ctor, ch->value
    );
    printf("\n");


    /*
        outerScopeValue =
            1

        closure arg1 arg2 =
            outerScopeValue + arg1 + arg2

        curried = 
            closure 2

        answer =
            curried 3
    */
    // ElmInt* outerScopeValue = newElmInt(1);
    Closure* closure = apply(
        &user_project_closure,
        1,
        (void*[]){&outerScopeValue}
    );
    ElmInt two = ELM_INT(2);
    ElmInt three = ELM_INT(3);
    Closure* curried = apply(
        closure,
        1,
        (void*[]){&two}
    );
    ElmInt* answer = apply(
        curried,
        1,
        (void*[]){&three}
    );

    printf("outerScopeValue addr=%s ctor=%d value=%d, hex=%s\n",
        hex_ptr(&outerScopeValue), (int)outerScopeValue.ctor, outerScopeValue.value,
        hex(&outerScopeValue, sizeof(ElmInt))
    );

    printf("two addr=%s ctor=%d value=%d, hex=%s\n",
        hex_ptr(&two), (int)two.ctor, two.value,
        hex(&two, sizeof(ElmInt))
    );

    printf("three addr=%s ctor=%d value=%d, hex=%s\n",
        hex_ptr(&three), (int)three.ctor, three.value,
        hex(&three, sizeof(ElmInt))
    );

    printf("closure addr=%s n_values=%d max_values=%d, hex=%s\n",
        hex_ptr(closure), (int)closure->n_values, (int)closure->max_values,
        hex(closure, sizeof(Closure) + closure->n_values * sizeof(void*))
    );

    printf("curried addr=%s n_values=%d max_values=%d, hex=%s\n",
        hex_ptr(curried), (int)curried->n_values, (int)curried->max_values,
        hex(curried, sizeof(Closure) + curried->n_values * sizeof(void*))
    );

    printf("answer addr=%s ctor=%d value=%d, hex=%s\n",
        hex_ptr(answer), (int)answer->ctor, answer->value,
        hex(answer, sizeof(ElmInt))
    );
};
