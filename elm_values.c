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


// Static values (not on dynamic heap)
// using 'const' qualifier is pointless because we cast pointers to void*
u8 False = 0;
u8 True = 1;
u8 Unit = 0;


// Type tags for heap values
typedef enum {
    Tag_Int,
    Tag_Float,
    Tag_Char,
    Tag_String,
    Tag_Nil,
    Tag_Cons,
    Tag_Tuple2,
    Tag_Tuple3,
    Tag_Custom,
    Tag_Record,
    Tag_Closure,
} Tag;


typedef enum {
    GcWhite,
    GcGray,
    GcBlack,
} GcColor;

#define DEFAULT_COLOR GcWhite

// Header for heap values
typedef struct {
    Tag tag :4;          // runtime type tag (4 bits)
    GcColor gc_color :2; // GC white/gray/black (2 bits)
    u32 size :26;        // payload size in integers (26 bits => <256MB)
} Header;

#define HEADER_INT        (Header){ .tag=Tag_Int,     .size=0, .gc_color=DEFAULT_COLOR }
#define HEADER_FLOAT      (Header){ .tag=Tag_Float,   .size=0, .gc_color=DEFAULT_COLOR }
#define HEADER_CHAR       (Header){ .tag=Tag_Char,    .size=0, .gc_color=DEFAULT_COLOR }
#define HEADER_STRING(x)  (Header){ .tag=Tag_String,  .size=(x+3)/4, .gc_color=DEFAULT_COLOR }
#define HEADER_NIL        (Header){ .tag=Tag_Nil,     .size=0, .gc_color=DEFAULT_COLOR }
#define HEADER_CONS       (Header){ .tag=Tag_Cons,    .size=2, .gc_color=DEFAULT_COLOR }
#define HEADER_TUPLE2     (Header){ .tag=Tag_Tuple2,  .size=2, .gc_color=DEFAULT_COLOR }
#define HEADER_TUPLE3     (Header){ .tag=Tag_Tuple3,  .size=3, .gc_color=DEFAULT_COLOR }
#define HEADER_CUSTOM(x)  (Header){ .tag=Tag_Custom,  .size=1+x, .gc_color=DEFAULT_COLOR }
#define HEADER_RECORD(x)  (Header){ .tag=Tag_Record,  .size=1+x, .gc_color=DEFAULT_COLOR }
#define HEADER_CLOSURE(x) (Header){ .tag=Tag_Closure, .size=2+x, .gc_color=DEFAULT_COLOR }

u32 eq_header(Header a, Header b) {
    return a.tag == b.tag && a.size == b.size;
}

int gc_first_pointer_offset(Header header) {
    switch (header.tag) {
        case Tag_Int:
        case Tag_Float:
        case Tag_Char:
        case Tag_String:
        case Tag_Nil:
            return -1; // no pointers to trace

        case Tag_Cons:
        case Tag_Tuple2:
        case Tag_Tuple3:
            return 0;  // first pointer is directly after header

        case Tag_Custom:
        case Tag_Record:
            return 1;  // skip 1 pointer-sized element (4 bytes)

        case Tag_Closure:
            return 2;  // skip 2 pointer-sized elements (8 bytes)
        
        default:
            return -1;
    }
}

// List

struct nil {
    Header header;
};
struct nil Nil = {
    .header = HEADER_NIL  // not on the dynamic heap, but has a header anyway
};

typedef struct {
    Header header;
    void* head;
    void* tail;
} Cons;

Cons* newList_Cons(void* head, void* tail) {
    Cons *p = malloc(sizeof(Cons));
    p->header = HEADER_CONS;
    p->head = head;
    p->tail = tail;
    return p;
};


// Tuples

typedef struct {
    Header header;
    void* a;
    void* b;
} Tuple2;

Tuple2* newTuple2(void* a, void* b) {
    Tuple2 *p = malloc(sizeof(Tuple2));
    p->header = HEADER_TUPLE2;
    p->a = a;
    p->b = b;
    return p;
};

typedef struct {
    Header header;
    void* a;
    void* b;
    void* c;
} Tuple3;

Tuple3* newTuple3(void* a, void* b, void* c) {
    Tuple3 *p = malloc(sizeof(Tuple3));
    p->header = HEADER_TUPLE3;
    p->a = a;
    p->b = b;
    p->c = c;
    return p;
};


// Numbers

typedef struct {
    Header header;
    i32 value;
} ElmInt;

ElmInt* newElmInt(i32 value) {
    ElmInt *p = malloc(sizeof(ElmInt));
    p->header = HEADER_INT;
    p->value = value;
    return p;
};


typedef struct {
    Header header;
    f64 value;
} ElmFloat;

ElmFloat* newElmFloat(f64 value) {
    ElmFloat *p = malloc(sizeof(ElmFloat));
    p->header = HEADER_FLOAT;
    p->value = value;
    return p;
};


// Char

typedef struct {
    Header header;
    i32 value;
} ElmChar;

ElmChar* newElmChar(u32 value) {
    ElmChar *p = malloc(sizeof(ElmChar));
    p->header = HEADER_CHAR;
    p->value = value;
    return p;
};

// String

typedef struct {
    Header header;
    u8 bytes[];
} ElmString;

// string constructor function? I'm not sure what we want it to look like!


// Enums (unions with no params)
/*
 Elm compiler generates constants in memory, structures can point to them
 Choose smallest required size.
*/


// Custom types

typedef struct {
    Header header;
    u32 ctor;
    void* values[];
} Custom;


// Record

typedef struct {
    u32 size;
    u32 fields[];
} FieldSet;

typedef struct {
    Header header;
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
    Header header;
    u16 n_values; // current number of applied args
    u16 max_values;
    void* (*evaluator)(void**); // pointer to a function that takes an array of pointers and returns a pointer
    void* values[];
} Closure;

#define CLOSURE(f, n) (Closure){ \
    .header = HEADER_CLOSURE(0), \
    .evaluator = &f, \
    .max_values = n, \
    .n_values = 0, \
    .values = {} \
}

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
        c->header = HEADER_CLOSURE(n_new);
        c->n_values = n_new;

        if (n_new == c->max_values) {
            return (*c->evaluator)(c->values);
        } else {
            return c;
        }
    }
}


// __________________________


typedef union {
    u8 unit_or_bool;
    ElmInt elm_int;
    ElmFloat elm_float;
    ElmChar elm_char;
    ElmString elm_string;
    struct nil nil;
    Cons cons;
    Tuple2 tuple2;
    Tuple3 tuple3;
    Custom custom;
    Record record;
    Closure closure;
} ElmValue;

// Equality with recursion (and possible stack overflow)
u8* eq(ElmValue *pa, ElmValue *pb) {

    // Reference equality shortcut
    if (pa == pb) {
        return &True;
    }

    // Bool and Unit don't have headers, so check them first
    u8 *pa_const = &pa->unit_or_bool;
    if (pa_const == &True || pa_const == &False || pa_const == &Unit) {
        // Wouldn't get here if they were equal. It would be reference equality.
        return &False;
    }

    Header ha = *(Header*)pa;
    Header hb = *(Header*)pb;
    if (ha.tag != hb.tag) {
        return &False;
    }
    if (ha.size != hb.size) {
        return &False;
    }

    switch (ha.tag) {
        case Tag_Int:
            return pa->elm_int.value == pb->elm_int.value
                ? &True : &False;

        case Tag_Float:
            return pa->elm_float.value == pb->elm_float.value
                ? &True : &False;

        case Tag_Char:
            return pa->elm_char.value == pb->elm_char.value
                ? &True : &False;

        case Tag_String: {
            // Cast array of bytes to array of ints, for speed
            u32* a32 = (u32*)pa->elm_string.bytes;
            u32* b32 = (u32*)pb->elm_string.bytes;
            for (int i=0; i < ha.size; i++) {
                if (a32[i] != b32[i]) {
                    return &False;
                }
            }
            return &True;
        }            

        case Tag_Nil:
            return &False; // we know pa!=pb, so can't both be Nil

        case Tag_Cons:
            return eq(pa->cons.head, pb->cons.head)
                && eq(pa->cons.tail, pb->cons.tail)
                ? &True : &False;
            

        case Tag_Tuple2:
            return eq(pa->tuple2.a, pb->tuple2.a)
                && eq(pa->tuple2.b, pb->tuple2.b)
                ? &True : &False;

        case Tag_Tuple3:
            return eq(pa->tuple3.a, pb->tuple3.a)
                && eq(pa->tuple3.b, pb->tuple3.b)
                && eq(pa->tuple3.c, pb->tuple3.c)
                ? &True : &False;

        case Tag_Custom:
            if (pa->custom.ctor != pb->custom.ctor) {
                return &False;
            }
            for (int i=0; i<ha.size; i++) {
                if (!eq(pa->custom.values[i], pb->custom.values[i])) {
                    return &False;                   
                }
            }
            return &True;

        case Tag_Record:
            if (pa->record.fields != pb->record.fields) {
                return &False;
            }
            for (int i=0; i<ha.size; i++) {
                if (!eq(pa->record.values[i], pb->record.values[i])) {
                    return &False;                    
                }
            }
            return &True;

        case Tag_Closure:
        default:
            // Debug.crash
            return &False;
    }
}


// __________________________


typedef union {
    ElmInt i;
    ElmFloat f;
} Number;


void* eval_add(void* args[2]) {
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
ElmInt outerScopeValue = (ElmInt){ .header = HEADER_INT, .value = 1 };


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
    printf("\n");

    printf("False size=%ld %s %d\n", sizeof(False), hex_ptr(&False), False);
    printf("True size=%ld %s %d\n", sizeof(True), hex_ptr(&True), True);
    printf("Unit size=%ld %s %d\n", sizeof(Unit), hex_ptr(&Unit), Unit);
    printf("\n");

    printf("Nil size=%ld addr=%s ctor=%d\n", sizeof(Nil), hex_ptr(&Nil), (int)Nil.header.tag);

    Cons *c = newList_Cons(&Unit, &Nil); // [()]
    printf("Cons size=%ld addr=%s ctor=%d head=%d tail=%d\n",
        sizeof(Cons), hex_ptr(c), (int)c->header.tag, (int)c->head, (int)c->tail
    );
    printf("\n");

    Tuple2 *t2 = newTuple2(&Unit, &Unit); // ((),())
    printf("Tuple2 size=%ld addr=%s ctor=%d a=%d b=%d\n",
        sizeof(Tuple2), hex_ptr(t2), (int)t2->header.tag, (int)t2->a, (int)t2->b
    );

    Tuple3 *t3 = newTuple3(&Unit, &Unit, &Unit); // ((),(),())
    printf("Tuple3 size=%ld addr=%s ctor=%d a=%d b=%d c=%d\n",
        sizeof(Tuple3), hex_ptr(t3), (int)t3->header.tag, (int)t3->a, (int)t3->b, (int)t3->c
    );
    printf("\n");

    ElmInt *i = newElmInt(123);
    printf("Int size=%ld addr=%s ctor=%d value=%d\n",
        sizeof(ElmInt), hex_ptr(i), (int)i->header.tag, i->value
    );
    ElmFloat *f = newElmFloat(123.456);
    printf("Float size=%ld addr=%s ctor=%d value=%f\n",
        sizeof(ElmFloat), hex_ptr(f), (int)f->header.tag, f->value
    );

    ElmChar *ch = newElmChar('A');
    printf("Char size=%ld addr=%s ctor=%d value=%c\n",
        sizeof(ElmChar), hex_ptr(ch), (int)ch->header.tag, ch->value
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
    ElmInt two = (ElmInt){ .header = HEADER_INT, .value = 2 };
    ElmInt three = (ElmInt){ .header = HEADER_INT, .value = 3 };
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
        hex_ptr(&outerScopeValue), (int)outerScopeValue.header.tag, outerScopeValue.value,
        hex(&outerScopeValue, sizeof(ElmInt))
    );

    printf("two addr=%s ctor=%d value=%d, hex=%s\n",
        hex_ptr(&two), (int)two.header.tag, two.value,
        hex(&two, sizeof(ElmInt))
    );

    printf("three addr=%s ctor=%d value=%d, hex=%s\n",
        hex_ptr(&three), (int)three.header.tag, three.value,
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
        hex_ptr(answer), (int)answer->header.tag, answer->value,
        hex(answer, sizeof(ElmInt))
    );
};
