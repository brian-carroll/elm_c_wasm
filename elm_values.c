#include <stdio.h>
#include <stdlib.h>


// Constants

char False = 0;
char True = 1;

char Unit = 0;


// Comparables

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
struct nil Nil;

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
    int value;
} ElmInt;

ElmInt* __ElmInt(int value) {
    ElmInt *p = malloc(sizeof(ElmInt));
    p->ctor = Ctor_Int;
    p->value = value;
    return p;
};

typedef struct elm_float {
    Ctor_Comp ctor;
    double value;
} ElmFloat;

ElmFloat* __ElmFloat(double value) {
    ElmFloat *p = malloc(sizeof(ElmFloat));
    p->ctor = Ctor_Float;
    p->value = value;
    return p;
};



int main(int argc, char ** argv) {
    printf("sizeof(int) = %d\n", (int)sizeof(int));
    printf("sizeof(Ctor_Comp) = %d\n", (int)sizeof(Ctor_Comp));
    printf("\n");

    printf("False %d %d\n", (int)(&False), False);
    printf("True %d %d\n", (int)(&True), True);
    printf("Unit %d %d\n", (int)(&Unit), Unit);
    printf("\n");

    printf("Nil addr=%d ctor=%d\n", (int)(&Nil), (int)Nil.ctor);

    Cons *c = __List_Cons(&Unit, &Nil); // [()]
    printf("Cons addr=%d ctor=%d head=%d tail=%d\n",
        (int)c, (int)c->ctor, (int)c->head, (int)c->tail
    );
    printf("\n");

    Tuple2 *t2 = __Tuple2(&Unit, &Unit); // ((),())
    printf("Tuple2 addr=%d ctor=%d a=%d b=%d\n",
        (int)t2, (int)t2->ctor, (int)t2->a, (int)t2->b
    );

    Tuple3 *t3 = __Tuple3(&Unit, &Unit, &Unit); // ((),(),())
    printf("Tuple3 addr=%d ctor=%d a=%d b=%d c=%d\n",
        (int)t3, (int)t3->ctor, (int)t3->a, (int)t3->b, (int)t3->c
    );
    printf("\n");

    ElmInt *i = __ElmInt(123);
    printf("Int addr=%d ctor=%d value=%d\n", (int)i, (int)i->ctor, i->value);
    ElmFloat *f = __ElmFloat(123.456);
    printf("Float addr=%d ctor=%d value=%f\n", (int)f, (int)f->ctor, f->value);

};


/*

    False,
    True,
    Unit,
    Int(i32),
    Float(f64),
    Char(char),
    Str(Box<str>),
    Nil,
    Cons {
        head: ElmRef,
        tail: ElmRef,
    },
    Tuple2(ElmRef, ElmRef),
    Tuple3(ElmRef, ElmRef, ElmRef),
    Union {
        ctor: Ctor,
        params: Box<[ElmRef]>,
    },
    Enum(Ctor),
    Record {
        field_set: &'static [Field],
        values: Box<[ElmRef]>,
    },
    Closure {
        arity: u8,
        values: Box<[ElmRef]>,
        evaluator: &'static Fn(Box<[ElmRef]>) -> ElmValue,
    },


*/

