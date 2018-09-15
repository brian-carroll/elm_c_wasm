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


// Char

typedef ElmInt ElmChar;
ElmChar* __ElmChar(unsigned int value) {
    ElmChar *p = malloc(sizeof(ElmInt));
    p->ctor = Ctor_Char;
    p->value = value;
    return p;
};

// String

typedef struct elm_string {
    Ctor_Comp ctor;
    unsigned int len_unicode;
    unsigned int len_bytes;
    char bytes[];
} ElmString;

// string constructor function?



int main(int argc, char ** argv) {
    printf("sizeof(int) = %d\n", (int)sizeof(int));
    printf("sizeof(double) = %d\n", (int)sizeof(double));
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

