#ifndef ELM_KERNEL_TYPES
#define ELM_KERNEL_TYPES

#include <stdlib.h>

// WEBASSEMBLY PRIMITIVE VALUES

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


// ELM STATIC CONSTANTS

u8 False;
u8 True;
u8 Unit;


// HEADER FOR HEAP VALUES

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

typedef struct {
    Tag tag :4;          // runtime type tag (4 bits)
    GcColor gc_color :2; // GC white/gray/black (2 bits)
    u32 size :26;        // payload size in integers (26 bits => <256MB)
} Header;

#define HEADER_INT        (Header){ .tag=Tag_Int,     .size=0, .gc_color=DEFAULT_COLOR }
#define HEADER_FLOAT      (Header){ .tag=Tag_Float,   .size=0, .gc_color=DEFAULT_COLOR }
#define HEADER_CHAR       (Header){ .tag=Tag_Char,    .size=0, .gc_color=DEFAULT_COLOR }
#define HEADER_STRING(x)  (Header){ .tag=Tag_String,  .size=x, .gc_color=DEFAULT_COLOR }
#define HEADER_NIL        (Header){ .tag=Tag_Nil,     .size=0, .gc_color=DEFAULT_COLOR }
#define HEADER_CONS       (Header){ .tag=Tag_Cons,    .size=2, .gc_color=DEFAULT_COLOR }
#define HEADER_TUPLE2     (Header){ .tag=Tag_Tuple2,  .size=2, .gc_color=DEFAULT_COLOR }
#define HEADER_TUPLE3     (Header){ .tag=Tag_Tuple3,  .size=3, .gc_color=DEFAULT_COLOR }
#define HEADER_CUSTOM(x)  (Header){ .tag=Tag_Custom,  .size=1+x, .gc_color=DEFAULT_COLOR }
#define HEADER_RECORD(x)  (Header){ .tag=Tag_Record,  .size=1+x, .gc_color=DEFAULT_COLOR }
#define HEADER_CLOSURE(x) (Header){ .tag=Tag_Closure, .size=2+x, .gc_color=DEFAULT_COLOR }


// LIST

struct nil {
    Header header;
};
struct nil Nil;

typedef struct {
    Header header;
    void* head;
    void* tail;
} Cons;
Cons* newCons(void* head, void* tail);


// TUPLES

typedef struct {
    Header header;
    void* a;
    void* b;
} Tuple2;
Tuple2* newTuple2(void* a, void* b);

typedef struct {
    Header header;
    void* a;
    void* b;
    void* c;
} Tuple3;
Tuple3* newTuple3(void* a, void* b, void* c);


// NUMBERS

typedef struct {
    Header header;
    i32 value;
} ElmInt;
ElmInt* newElmInt(i32 value);

typedef struct {
    Header header;
    f64 value;
} ElmFloat;
ElmFloat* newElmFloat(f64 value);

typedef union {
    ElmInt i;
    ElmFloat f;
} Number;


// CHAR

typedef struct {
    Header header;
    i32 value;
} ElmChar;
ElmChar* newElmChar(u32 value);


// STRING

typedef struct {
    Header header;
    u8 bytes[];
} ElmString;
ElmString* newElmString(size_t n, char *str);


// Enums (unions with no params)
/*
 Elm compiler generates constants in memory, structures can point to them
 Choose smallest required size.
*/


// CUSTOM

typedef struct {
    Header header;
    u32 ctor;
    void* values[];
} Custom;


// RECORD

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


// CLOSURE

typedef struct {
    Header header;
    u16 n_values; // current number of applied args
    u16 max_values;
    void* (*evaluator)(void**); // pointer to a function that takes an array of pointers and returns a pointer
    void* values[];
} Closure;

#define CLOSURE(f, n) (Closure) { \
    .header = HEADER_CLOSURE(0), \
    .evaluator = &f, \
    .max_values = n, \
    .n_values = 0, \
    .values = {} \
}


// ANY ELM VALUE (for pointers in collections)
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


#endif // #ifndef ELM_KERNEL_TYPES

