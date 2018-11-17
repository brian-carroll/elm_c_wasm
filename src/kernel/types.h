#ifndef ELM_KERNEL_TYPES
#define ELM_KERNEL_TYPES

#include <stdlib.h>

// WEBASSEMBLY PRIMITIVE TYPES

typedef char i8;
typedef short i16;
typedef int i32;
typedef long long int i64;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long int u64;

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


typedef enum {
    GcDead,
    GcLive
} GcMark;

typedef struct {
    u32 size :27;       // payload size in integers (27 bits => <512MB)
    GcMark gc_mark :1;  // GC "alive or dead" mark bit
    Tag tag :4;         // runtime type tag (4 bits)
} Header;

// It's nice if we can target 64-bit platforms
// C compile times are dramatically faster for native than for WebAssembly => faster dev cycle
#if defined(_LP64) || defined(_WIN64)
    #define TARGET_64BIT
#endif

// Header size field has units corresponding to this many bytes:
#define SIZE_UNIT 4

#define HEADER_INT         (Header){ .tag=Tag_Int,     .gc_mark=GcDead, .size=sizeof(ElmInt)/SIZE_UNIT }
#define HEADER_FLOAT       (Header){ .tag=Tag_Float,   .gc_mark=GcDead, .size=sizeof(ElmFloat)/SIZE_UNIT }
#define HEADER_CHAR        (Header){ .tag=Tag_Char,    .gc_mark=GcDead, .size=sizeof(ElmChar)/SIZE_UNIT }
#define HEADER_STRING(n32) (Header){ .tag=Tag_String,  .gc_mark=GcDead, .size=sizeof(ElmString)/SIZE_UNIT + n32 }
#define HEADER_NIL         (Header){ .tag=Tag_Nil,     .gc_mark=GcDead, .size=sizeof(Header)/SIZE_UNIT }
#define HEADER_CONS        (Header){ .tag=Tag_Cons,    .gc_mark=GcDead, .size=sizeof(Cons)/SIZE_UNIT }
#define HEADER_TUPLE2      (Header){ .tag=Tag_Tuple2,  .gc_mark=GcDead, .size=sizeof(Tuple2)/SIZE_UNIT }
#define HEADER_TUPLE3      (Header){ .tag=Tag_Tuple3,  .gc_mark=GcDead, .size=sizeof(Tuple3)/SIZE_UNIT }
#define HEADER_CUSTOM(p)   (Header){ .tag=Tag_Custom,  .gc_mark=GcDead, .size=(sizeof(Custom) + p*sizeof(void*))/SIZE_UNIT }
#define HEADER_RECORD(p)   (Header){ .tag=Tag_Record,  .gc_mark=GcDead, .size=(sizeof(Record) + p*sizeof(void*))/SIZE_UNIT }
#define HEADER_CLOSURE(p)  (Header){ .tag=Tag_Closure, .gc_mark=GcDead, .size=(sizeof(Closure) + p*sizeof(void*))/SIZE_UNIT }
#define HEADER_GCFULL      (Header){ .tag=Tag_GcFull,  .gc_mark=GcDead, .size=sizeof(GcContinuation)/SIZE_UNIT }


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
