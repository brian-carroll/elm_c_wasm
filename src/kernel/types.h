#ifndef ELM_KERNEL_TYPES
#define ELM_KERNEL_TYPES

#include <stdlib.h>

// WEBASSEMBLY PRIMITIVE TYPES

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


typedef enum {
    Tag_Int,     // 0
    Tag_Float,   // 1
    Tag_Char,    // 2
    Tag_String,  // 3
    Tag_Nil,     // 4
    Tag_Cons,    // 5
    Tag_Tuple2,  // 6
    Tag_Tuple3,  // 7
    Tag_Custom,  // 8
    Tag_Record,  // 9
    Tag_Closure, // a
    Tag_GcFull,  // b
} Tag;


typedef struct {
    u32 size :28;       // payload size in integers (28 bits => <1GB)
    Tag tag :4;         // runtime type tag (4 bits)
} Header;

// It's nice if we can target 64-bit platforms
// C compile times are dramatically faster for native than for WebAssembly => faster dev cycle
#if defined(_LP64) || defined(_WIN64)
    #define TARGET_64BIT
#endif

// Header size field has units corresponding to this many bytes:
#define SIZE_UNIT sizeof(size_t)

#define HEADER_INT         (Header){ .tag=Tag_Int,     .size=sizeof(ElmInt)/SIZE_UNIT }
#define HEADER_FLOAT       (Header){ .tag=Tag_Float,   .size=sizeof(ElmFloat)/SIZE_UNIT }
#define HEADER_CHAR        (Header){ .tag=Tag_Char,    .size=sizeof(ElmChar)/SIZE_UNIT }
#define HEADER_STRING(s)   (Header){ .tag=Tag_String,  .size=s }
#define HEADER_NIL         (Header){ .tag=Tag_Nil,     .size=1 }
#define HEADER_CONS        (Header){ .tag=Tag_Cons,    .size=sizeof(Cons)/SIZE_UNIT }
#define HEADER_TUPLE2      (Header){ .tag=Tag_Tuple2,  .size=sizeof(Tuple2)/SIZE_UNIT }
#define HEADER_TUPLE3      (Header){ .tag=Tag_Tuple3,  .size=sizeof(Tuple3)/SIZE_UNIT }
#define HEADER_CUSTOM(p)   (Header){ .tag=Tag_Custom,  .size=(sizeof(Custom) + p*sizeof(void*))/SIZE_UNIT }
#define HEADER_RECORD(p)   (Header){ .tag=Tag_Record,  .size=(sizeof(Record) + p*sizeof(void*))/SIZE_UNIT }
#define HEADER_CLOSURE(p)  (Header){ .tag=Tag_Closure, .size=(sizeof(Closure) + p*sizeof(void*))/SIZE_UNIT }
#define HEADER_GCFULL      (Header){ .tag=Tag_GcFull,  .size=sizeof(GcContinuation)/SIZE_UNIT }


// LIST

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

u32 custom_params(Custom* c); // number of parameters in a Custom struct

// RECORD

typedef struct {
    u32 size;
    u32 fields[];
} FieldSet;

typedef struct {
    Header header;
    FieldSet* fieldset;
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
    void* (*evaluator)(void*[]); // pointer to a function that takes an array of pointers and returns a pointer
    void* values[];
} Closure;

#define F2(f) (Closure) { \
    .header = HEADER_CLOSURE(0), \
    .evaluator = &f, \
    .max_values = 2, \
}


// GARBAGE COLLECTOR TYPES

// A value used to implement an 'exception' when GC is full
typedef struct {
    Header header;
    Closure* continuation; // a partially complete tail recursion
} GcContinuation;


// ANY ELM VALUE (for pointers in collections)
typedef union {
    Header header;
    ElmInt elm_int;
    ElmFloat elm_float;
    ElmChar elm_char;
    ElmString elm_string;
    Cons cons;
    Tuple2 tuple2;
    Tuple3 tuple3;
    Custom custom;
    Record record;
    Closure closure;
    GcContinuation gc_full;
} ElmValue;


// STATIC CONSTANTS

GcContinuation GcFull; // constant where continuation=NULL
void* pGcFull;

ElmValue Nil;
ElmValue Unit;
ElmValue False;
ElmValue True;
void Types_init();


#endif // #ifndef ELM_KERNEL_TYPES
