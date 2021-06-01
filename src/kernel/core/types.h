#ifndef ELM_KERNEL_TYPES
#define ELM_KERNEL_TYPES

#include <stdbool.h>
#include <stdint.h>
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
  // Tag_Zero, // invalid Elm value  TODO: fails Json tests, is that a real bug?
  Tag_Int,         // 0
  Tag_Float,       // 1
  Tag_Char,        // 2
  Tag_String,      // 3
  Tag_List,        // 4
  Tag_Tuple2,      // 5
  Tag_Tuple3,      // 6
  Tag_Custom,      // 7
  Tag_Record,      // 8
  Tag_FieldGroup,  // 9
  Tag_Closure,     // a
  Tag_JsRef,       // b
} Tag;

#ifdef _WIN32
typedef struct {
  u32 size : 27;
  Tag tag : 5;  // Windows treats this as signed, so comparisons can go wrong
} Header;
#else
typedef struct {
  u32 size : 28;  // payload size in integers (28 bits => <1GB)
  Tag tag : 4;    // runtime type tag (4 bits)
} Header;
#endif

// Support 64-bit native executables as well as 32-bit Wasm
// Compile times and tooling are much better for native
#define TARGET_64BIT (UINTPTR_MAX > 0xffffffff)

#if TARGET_64BIT
// Making the padding an explicit member means it gets zeroed rather than ignored
#define PADDING64 u32 padding;
#else
#define PADDING64
#endif

// Header size field has units corresponding to this many bytes:
#define SIZE_UNIT sizeof(void*)

#define SIZE_INT sizeof(ElmInt) / SIZE_UNIT
#define SIZE_FLOAT sizeof(ElmFloat) / SIZE_UNIT
#define SIZE_CHAR sizeof(ElmChar) / SIZE_UNIT
#define SIZE_STRING(utf16len) \
  (sizeof(Header) + (sizeof(u16) * utf16len) + SIZE_UNIT - 1) / SIZE_UNIT
#define SIZE_LIST sizeof(Cons) / SIZE_UNIT
#define SIZE_TUPLE2 sizeof(Tuple2) / SIZE_UNIT
#define SIZE_TUPLE3 sizeof(Tuple3) / SIZE_UNIT
#define SIZE_CUSTOM(p) (sizeof(Custom) + p * sizeof(void*)) / SIZE_UNIT
#define SIZE_RECORD(p) (sizeof(Record) + p * sizeof(void*)) / SIZE_UNIT
#define SIZE_FIELDGROUP(p) (sizeof(FieldGroup) + p * sizeof(u32)) / SIZE_UNIT
#define SIZE_CLOSURE(p) (sizeof(Closure) + p * sizeof(void*)) / SIZE_UNIT
#define SIZE_JS_REF sizeof(JsRef) / SIZE_UNIT

// clang-format off
#define HEADER_INT { .tag = Tag_Int, .size = SIZE_INT }
#define HEADER_FLOAT { .tag = Tag_Float, .size = SIZE_FLOAT }
#define HEADER_CHAR { .tag = Tag_Char, .size = SIZE_CHAR }
#define HEADER_STRING(utf16len) { .tag = Tag_String, .size = SIZE_STRING(utf16len) }
#define HEADER_LIST { .tag = Tag_List, .size = SIZE_LIST }
#define HEADER_TUPLE2 { .tag = Tag_Tuple2, .size = SIZE_TUPLE2 }
#define HEADER_TUPLE3 { .tag = Tag_Tuple3, .size = SIZE_TUPLE3 }
#define HEADER_CUSTOM(p) { .tag = Tag_Custom, .size = SIZE_CUSTOM(p) }
#define HEADER_RECORD(p) { .tag = Tag_Record, .size = SIZE_RECORD(p) }
#define HEADER_FIELDGROUP(p) { .tag = Tag_FieldGroup, .size = SIZE_FIELDGROUP(p) }
#define HEADER_CLOSURE(p) { .tag = Tag_Closure, .size = SIZE_CLOSURE(p) }
#define HEADER_JS_REF { .tag = Tag_JsRef, .size = SIZE_JS_REF }
// clang-format on


// LIST

typedef struct {
  Header header;
  PADDING64
  void* head;
  void* tail;
} Cons;
Cons* newCons(void* head, void* tail);

// TUPLES

typedef struct {
  Header header;
  PADDING64
  void* a;
  void* b;
} Tuple2;
Tuple2* newTuple2(void* a, void* b);

typedef struct {
  Header header;
  PADDING64
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
  u32 padding;  // on both 32 and 64-bit platforms
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
  union {
    u32 value;
    u16 words16[2];
    u8 bytes[4];
  };
} ElmChar;
ElmChar* newElmChar(u32 value);

// STRING

// Make sure constant strings are properly aligned on 64-bit target.
// Otherwise C compiler can truncate the zero padding at the end,
// putting other data there. Messes up length calculation, etc.
#if defined(_WIN32)
#define ALIGN(X) __declspec(align(X))
#else
#define ALIGN(X) __attribute__((aligned(X)))
#endif

struct ALIGN(8) elm_string {
  Header header;
  u16 words16[];
};
typedef struct elm_string ElmString;
ElmString* newElmString(size_t len16);


// CUSTOM

typedef struct {
  Header header;
  u32 ctor;
  void* values[];
} Custom;

u32 custom_params(Custom* c);  // number of parameters in a Custom struct
Custom* newCustom(u32 ctor, u32 n_children, void* children[]);

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
Record* newRecord(FieldGroup* fg, u32 n_children, void* children[]);

/*
 Elm compiler generates
    field sets (global constant structs)
    record constructor functions
*/

// CLOSURE

// pointer to a function that takes an array of pointers and returns a pointer
typedef void* (*EvalFunction)(void*[]);

typedef struct {
  Header header;
  u16 n_values;  // current number of applied args
  u16 max_values;
  EvalFunction evaluator;
  void* values[];
} Closure;
// Use effectively "infinite" arity for JS functions, so we don't try to evaluate in Wasm
#define NEVER_EVALUATE 0xffff

Closure* newClosure(u16 n_values, u16 max_values, EvalFunction evaluator, void* values[]);

// Reference to a JS object
typedef struct {
  Header header;
  u32 index;
} JsRef;


// ANY ELM VALUE (for pointers in collections)
typedef union {
  Header header;
  u32 header_as_u32;
  ElmInt elm_int;
  ElmFloat elm_float;
  ElmChar elm_char;
  ElmString elm_string;
  Cons cons;
  Tuple2 tuple2;
  Tuple3 tuple3;
  Custom custom;
  Record record;
  FieldGroup fieldgroup;
  Closure closure;
  JsRef js_ref;
} ElmValue;

// STATIC CONSTANTS

extern Cons Nil;
extern void* pNil;

extern Custom Unit;
extern void* pUnit;

extern Custom False;
extern void* pFalse;

extern Custom True;
extern void* pTrue;


// PLATFORM & SCHEDULER


#define KERNEL_CTOR_OFFSET 0x400 * 1000


typedef enum task_ctor {
  TASK_SUCCEED = KERNEL_CTOR_OFFSET,
  TASK_FAIL,
  TASK_BINDING,
  TASK_AND_THEN,
  TASK_ON_ERROR,
  TASK_RECEIVE,
} TaskCtor;

typedef struct task {
  Header header;
  u32 ctor;
  void* value;
  Closure* callback;
  Closure* kill;
  struct task* task;
} Task;


#define PROC_CTOR KERNEL_CTOR_OFFSET

typedef struct process_stack {
  Header header;
  u32 ctor;
  Closure* a;
  struct process_stack* b;
} ProcessStack;

typedef struct process {
  Header header;
  u32 ctor;
  u32 id;
  Task* root;
  ProcessStack* stack;
  Cons* mailbox;
} Process;


// Manager Config

// Manager


#endif  // #ifndef ELM_KERNEL_TYPES
