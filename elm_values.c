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

char False = 0;
char True = 1;

char Unit = 0;


// Comparable constructor values
// Enum is 4 bytes, not just 1. But in a struct, C compiler will pad to 4 bytes anyway.
// Later we may use the extra bytes for a GC header
typedef enum ctor_comp {
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

typedef struct cons {
    Ctor_Comp ctor;
    void* head;
    void* tail;
} Cons;

Cons* __List_Cons(void* head, void* tail) {
    Cons *p = malloc(sizeof(Cons));
    p->ctor = Ctor_Cons;
    p->head = head;
    p->tail = tail;
    return p;
};


// Tuples

typedef struct tuple2 {
    Ctor_Comp ctor;
    void* a;
    void* b;
} Tuple2;

Tuple2* __Tuple2(void* a, void* b) {
    Tuple2 *p = malloc(sizeof(Tuple2));
    p->ctor = Ctor_Tuple2;
    p->a = a;
    p->b = b;
    return p;
};

typedef struct tuple3 {
    Ctor_Comp ctor;
    void* a;
    void* b;
    void* c;
} Tuple3;

Tuple3* __Tuple3(void* a, void* b, void* c) {
    Tuple3 *p = malloc(sizeof(Tuple3));
    p->ctor = Ctor_Tuple3;
    p->a = a;
    p->b = b;
    p->c = c;
    return p;
};


// Numbers

typedef struct elm_int {
    Ctor_Comp ctor;
    i32 value;
} ElmInt;

ElmInt* __ElmInt(i32 value) {
    ElmInt *p = malloc(sizeof(ElmInt));
    p->ctor = Ctor_Int;
    p->value = value;
    return p;
};

typedef struct elm_float {
    Ctor_Comp ctor;
    f64 value;
} ElmFloat;

ElmFloat* __ElmFloat(f64 value) {
    ElmFloat *p = malloc(sizeof(ElmFloat));
    p->ctor = Ctor_Float;
    p->value = value;
    return p;
};


// Char

typedef ElmInt ElmChar;
ElmChar* __ElmChar(u32 value) {
    ElmChar *p = malloc(sizeof(ElmChar));
    p->ctor = Ctor_Char;
    p->value = value;
    return p;
};

// String

typedef struct elm_string {
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

typedef struct field_set {
    u32 size;
    u32 fields[];
} FieldSet;

typedef struct record {
    FieldSet* fields;
    void* values[];
} Record;

/*
 Elm compiler generates
    field sets (global constant structs)
    record constructor functions
*/


// Closure

typedef struct closure {
    u8 n_values; // current number of applied args
    u8 max_values;
    void* (*evaluator)(void**); // pointer to a function that takes an array of pointers and returns a pointer
    void* values[];
} Closure;


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
        c->n_values = n_new;

        if (n_new == c->max_values) {
            return (*c->evaluator)(c->values);
        } else {
            return c;
        }
    }
}


typedef union number {
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
        return __ElmFloat(f);
    } else {
        i32 ia = pa->i.value;
        i32 ib = pb->i.value;
        i32 i = ia + ib;
        return __ElmInt(i);
    }
}

Closure add = {
    .n_values = 0,
    .max_values = 2,
    .evaluator = &eval_add,
    .values = {}
};




// What I could generate from Elm compiler, not what I could write by hand
// Obvious optimizations missing:
//   Intermediate ElmInt allocated on heap. No way to work on unboxed ints and only
//     create the final result of the function on the heap.
//   No way to skip the convoluted 'apply' in this common case of having all args ready
//     because `add` needs to be first-class *always* and I don't have a way to
//     specialize it for when it's not being passed around.
void* eval_user_project_closure(void* args[]) {
    ElmInt *outerScopeValue = args[0];
    ElmInt *arg1 = args[1];
    ElmInt *arg2 = args[2];

    ElmInt *tmp = apply(&add, 2, (void*[]){arg2, outerScopeValue});
    return apply(&add, 2, (void*[]){arg1, tmp});
}

Closure user_project_closure = {
    .n_values = 0,
    .max_values = 3,
    .evaluator = &eval_user_project_closure,
    .values = {}
};


int main(int argc, char ** argv) {
    printf("sizeof(int) = %d\n", (int)sizeof(int));
    printf("sizeof(f64) = %d\n", (int)sizeof(f64));
    printf("sizeof(Ctor_Comp) = %d\n", (int)sizeof(Ctor_Comp));
    printf("\n");

    printf("False size=%ld %d %d\n", sizeof(False), (int)(&False), False);
    printf("True size=%ld %d %d\n", sizeof(True), (int)(&True), True);
    printf("Unit size=%ld %d %d\n", sizeof(Unit), (int)(&Unit), Unit);
    printf("\n");

    printf("Nil size=%ld addr=%d ctor=%d\n", sizeof(Nil), (int)(&Nil), (int)Nil.ctor);

    Cons *c = __List_Cons(&Unit, &Nil); // [()]
    printf("Cons size=%ld addr=%d ctor=%d head=%d tail=%d\n",
        sizeof(Cons), (int)c, (int)c->ctor, (int)c->head, (int)c->tail
    );
    printf("\n");

    Tuple2 *t2 = __Tuple2(&Unit, &Unit); // ((),())
    printf("Tuple2 size=%ld addr=%d ctor=%d a=%d b=%d\n",
        sizeof(Tuple2), (int)t2, (int)t2->ctor, (int)t2->a, (int)t2->b
    );

    Tuple3 *t3 = __Tuple3(&Unit, &Unit, &Unit); // ((),(),())
    printf("Tuple3 size=%ld addr=%d ctor=%d a=%d b=%d c=%d\n",
        sizeof(Tuple3), (int)t3, (int)t3->ctor, (int)t3->a, (int)t3->b, (int)t3->c
    );
    printf("\n");

    ElmInt *i = __ElmInt(123);
    printf("Int size=%ld addr=%d ctor=%d value=%d\n",
        sizeof(ElmInt), (int)i, (int)i->ctor, i->value
    );
    ElmFloat *f = __ElmFloat(123.456);
    printf("Float size=%ld addr=%d ctor=%d value=%f\n",
        sizeof(ElmFloat), (int)f, (int)f->ctor, f->value
    );

    ElmChar *ch = __ElmChar('A');
    printf("Char size=%ld addr=%d ctor=%d value=%c\n",
        sizeof(ElmChar), (int)ch, (int)ch->ctor, ch->value
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
    ElmInt* outerScopeValue = __ElmInt(1);
    Closure* closure = apply(
        &user_project_closure,
        1,
        (void*[]){outerScopeValue}
    );
    Closure* curried = apply(
        closure,
        1,
        (void*[]){__ElmInt(2)}
    );
    ElmInt* answer = apply(
        curried,
        1,
        (void*[]){__ElmInt(3)}
    );

    printf("outerScopeValue addr=%d ctor=%d value=%d\n",
        (int)outerScopeValue, (int)outerScopeValue->ctor, outerScopeValue->value
    );

    printf("closure addr=%d n_values=%d max_values=%d\n",
        (int)closure, (int)closure->n_values, (int)closure->max_values
    );

    printf("curried addr=%d n_values=%d max_values=%d\n",
        (int)curried, (int)curried->n_values, (int)curried->max_values
    );

    printf("answer addr=%d ctor=%d value=%d\n",
        (int)answer, (int)answer->ctor, answer->value
    );
};
