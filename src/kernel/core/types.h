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
  Tag_Int,              // 0
  Tag_Float,            // 1
  Tag_Char,             // 2
  Tag_String,           // 3
  Tag_List,             // 4
  Tag_Tuple2,           // 5
  Tag_Tuple3,           // 6
  Tag_Custom,           // 7
  Tag_Record,           // 8
  Tag_FieldGroup,       // 9
  Tag_Closure,          // a
  Tag_GcException,      // b
  Tag_GcStackEmpty,     // c
  Tag_GcStackPush,      // d
  Tag_GcStackPop,       // e
  Tag_GcStackTailCall,  // f
} Tag;

typedef struct {
  u32 size : 28;  // payload size in integers (28 bits => <1GB)
  Tag tag : 4;    // runtime type tag (4 bits)
} Header;

// It's nice if we can target 64-bit platforms
// C compile times are dramatically faster for native than for WebAssembly => faster dev
// cycle
#if defined(_LP64) || defined(_WIN64)
#define TARGET_64BIT
#endif

#ifdef TARGET_64BIT
// Padding would be inserted by compiler anyway
// This makes it more explicit (but we have to get it right!)
// This also gives automatic zeroing of the padding in struct literals,
// since if you leave out a member the compiler zeros it
#define PADDING64 u32 padding;
#else
#define PADDING64
#endif

// Header size field has units corresponding to this many bytes:
#define SIZE_UNIT sizeof(void*)

#define HEADER_INT \
  (Header) { .tag = Tag_Int, .size = sizeof(ElmInt) / SIZE_UNIT }
#define HEADER_FLOAT \
  (Header) { .tag = Tag_Float, .size = sizeof(ElmFloat) / SIZE_UNIT }
#define HEADER_CHAR \
  (Header) { .tag = Tag_Char, .size = sizeof(ElmChar) / SIZE_UNIT }
#define HEADER_STRING(utf16len)                                                     \
  (Header) {                                                                        \
    .tag = Tag_String,                                                              \
    .size = (sizeof(Header) + (sizeof(u16) * utf16len) + SIZE_UNIT - 1) / SIZE_UNIT \
  }
#define HEADER_LIST \
  (Header) { .tag = Tag_List, .size = sizeof(Cons) / SIZE_UNIT }
#define HEADER_TUPLE2 \
  (Header) { .tag = Tag_Tuple2, .size = sizeof(Tuple2) / SIZE_UNIT }
#define HEADER_TUPLE3 \
  (Header) { .tag = Tag_Tuple3, .size = sizeof(Tuple3) / SIZE_UNIT }
#define HEADER_CUSTOM(p) \
  (Header) { .tag = Tag_Custom, .size = (sizeof(Custom) + p * sizeof(void*)) / SIZE_UNIT }
#define HEADER_RECORD(p) \
  (Header) { .tag = Tag_Record, .size = (sizeof(Record) + p * sizeof(void*)) / SIZE_UNIT }
#define HEADER_FIELDGROUP(p)                                                          \
  (Header) {                                                                          \
    .tag = Tag_FieldGroup, .size = (sizeof(FieldGroup) + p * sizeof(u32)) / SIZE_UNIT \
  }
#define HEADER_CLOSURE(p)                                                         \
  (Header) {                                                                      \
    .tag = Tag_Closure, .size = (sizeof(Closure) + p * sizeof(void*)) / SIZE_UNIT \
  }

#define HEADER_GC_EXCEPTION \
  (Header) { .tag = Tag_GcException, .size = sizeof(GcException) / SIZE_UNIT }
#define HEADER_GC_STACK_EMPTY \
  (Header) { .tag = Tag_GcStackEmpty, .size = sizeof(GcStackMap) / SIZE_UNIT }
#define HEADER_GC_STACK_PUSH \
  (Header) { .tag = Tag_GcStackPush, .size = sizeof(GcStackMap) / SIZE_UNIT }
#define HEADER_GC_STACK_POP \
  (Header) { .tag = Tag_GcStackPop, .size = sizeof(GcStackMap) / SIZE_UNIT }
#define HEADER_GC_STACK_TC \
  (Header) { .tag = Tag_GcStackTailCall, .size = sizeof(GcStackMap) / SIZE_UNIT }

#define CAN_THROW(expr)                                 \
  ({                                                    \
    void* tmp_CAN_THROW = expr;                         \
    if (tmp_CAN_THROW == pGcFull) return tmp_CAN_THROW; \
    tmp_CAN_THROW;                                      \
  })

// LIST

typedef struct {
  Header header;
  PADDING64
  void* head;
  void* tail;
} Cons;
Cons* ctorCons(void* head, void* tail);
#define NEW_CONS(head, tail) CAN_THROW(ctorCons(head, tail))

// TUPLES

typedef struct {
  Header header;
  PADDING64
  void* a;
  void* b;
} Tuple2;
Tuple2* ctorTuple2(void* a, void* b);
#define NEW_TUPLE2(a, b) CAN_THROW(ctorTuple2(a, b))

typedef struct {
  Header header;
  PADDING64
  void* a;
  void* b;
  void* c;
} Tuple3;
Tuple3* ctorTuple3(void* a, void* b, void* c);
#define NEW_TUPLE3(a, b, c) CAN_THROW(ctorTuple3(a, b, c))

// NUMBERS

typedef struct {
  Header header;
  i32 value;
} ElmInt;
ElmInt* ctorElmInt(i32 value);
#define NEW_ELM_INT(value) CAN_THROW(ctorElmInt(value))

typedef struct {
  Header header;
  u32 padding;  // on both 32 and 64-bit platforms
  f64 value;
} ElmFloat;
ElmFloat* ctorElmFloat(f64 value);
#define NEW_ELM_FLOAT(value) CAN_THROW(ctorElmFloat(value))

typedef union {
  ElmInt i;
  ElmFloat f;
} Number;

// CHAR

typedef struct {
  Header header;
  union {
    u32 value;
    u16 words16[2];
    u8 bytes[4];
  };
} ElmChar;
ElmChar* ctorElmChar(u32 value);
#define NEW_ELM_CHAR(value) CAN_THROW(ctorElmChar(value))

// STRING

// Make sure constant strings are properly aligned on 64-bit target.
// Otherwise C compiler can truncate the zero padding at the end,
// putting other data there. Messes up length calculation, etc.
typedef struct __attribute__((aligned)) {
  Header header;
  u8 bytes[];
} ElmString;
ElmString* ctorElmString(size_t n, char* str);
#define NEW_ELM_STRING(n, str) CAN_THROW(ctorElmString(n, str))

typedef struct __attribute__((aligned)) {
  Header header;
  u16 words16[];
} ElmString16;
ElmString16* ctorElmString16(size_t n);
#define NEW_ELM_STRING16(len16) CAN_THROW(ctorElmString16(len16))

enum {
  UTF8,
  UTF16,
};
#define STRING_ENCODING UTF16

// CUSTOM

typedef struct {
  Header header;
  u32 ctor;
  void* values[];
} Custom;

u32 custom_params(Custom* c);  // number of parameters in a Custom struct
Custom* ctorCustom(u32 ctor, u32 n_children, void* children[]);
#define NEW_CUSTOM(ctor, n, c) CAN_THROW(ctorCustom(ctor, n, c))

// RECORD

typedef struct {
  Header header;
  u32 size;
  u32 fields[];
} FieldGroup;

typedef struct {
  Header header;
  PADDING64
  FieldGroup* fieldgroup;
  void* values[];
} Record;
Record* ctorRecord(FieldGroup* fg, u32 n_children, void* children[]);
#define NEW_RECORD(fg, n, c) CAN_THROW(ctorRecord(fg, n, c))

/*
 Elm compiler generates
    field sets (global constant structs)
    record constructor functions
*/

// CLOSURE

typedef struct {
  Header header;
  u16 n_values;  // current number of applied args
  u16 max_values;
  void* (*evaluator)(void*[]);  // pointer to a function that takes an array of pointers
                                 // and returns a pointer
  void* values[];
} Closure;
// Use effectively "infinite" arity for JS functions, so we don't try to evaluate in Wasm
#define NEVER_EVALUATE 0xffff

Closure* ctorClosure(
    u16 n_values, u16 max_values, void* (*evaluator)(void*[]), void* values[]);
#define NEW_CLOSURE(n, m, e, v) CAN_THROW(ctorClosure(n, m, e, v))

// GARBAGE COLLECTOR TYPES

// A value used to implement an exception when GC is full
typedef struct {
  Header header;
} GcException;

// Doubly-linked list for tracking stack pointers
// GC makes assumptions about order of these fields (child_count and its usages)
typedef struct {
  Header header;
  PADDING64
  void* newer;  // cheat! mutable field pointing at newer stuff
  void* replay;
  void* older;
} GcStackMap;

// ANY ELM VALUE (for pointers in collections)
typedef union {
  Header header;
  u32 header_as_u32;
  ElmInt elm_int;
  ElmFloat elm_float;
  ElmChar elm_char;
  ElmString elm_string;
  ElmString16 elm_string16;
  Cons cons;
  Tuple2 tuple2;
  Tuple3 tuple3;
  Custom custom;
  Record record;
  FieldGroup fieldgroup;
  Closure closure;
  GcException gc_exception;
  GcStackMap gc_stackmap;
} ElmValue;

// STATIC CONSTANTS

GcException GcFull;
void* pGcFull;

Cons Nil;
void* pNil;

Custom Unit;
void* pUnit;

Custom False;
void* pFalse;

Custom True;
void* pTrue;

#endif  // #ifndef ELM_KERNEL_TYPES
