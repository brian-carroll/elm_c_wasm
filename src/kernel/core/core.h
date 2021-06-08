#ifndef CORE_H
#define CORE_H

#include "../wrapper/wrapper.h"
#include "types.h"
#include "./debug/debug.h"

#ifdef _WIN32
// Ensure windows.h is included very early in compilation
// Otherwise we get vague errors about undefined architecture
#include <windows.h>
#endif

#define ARRAY_LEN(a) (sizeof(a)/sizeof(a[0]))

/*
  , (+), (-), (*), (/), (//), (^)
  , toFloat, round, floor, ceiling, truncate
  , (==), (/=)
  , (<), (>), (<=), (>=), max, min, compare, Order(..)
  , Bool(..), not, (&&), (||), xor
  , (++)
  , modBy, remainderBy, negate, abs, clamp, sqrt, logBase, e
  , pi, cos, sin, tan, acos, asin, atan, atan2
  , degrees, radians, turns
  , toPolar, fromPolar
  , isNaN, isInfinite
  , identity, always, (<|), (|>), (<<), (>>), Never, never
*/

extern Closure Basics_negate;  // is not a kernel function in Elm-JS

extern Closure Basics_add;
extern Closure Basics_sub;
extern Closure Basics_mul;
extern Closure Basics_fdiv;
extern Closure Basics_idiv;
extern Closure Basics_pow;

extern Closure Basics_toFloat;
extern Closure Basics_round;
extern Closure Basics_floor;
extern Closure Basics_ceiling;
// extern Closure Basics_truncate;

extern Closure Basics_not;
extern Closure Basics_and;
extern Closure Basics_or;
extern Closure Basics_xor;

extern Closure Basics_modBy;
extern Closure Basics_remainderBy;
extern Closure Basics_log;

extern Closure Basics_identity;


// =========================================

extern Closure Bitwise_and;
extern Closure Bitwise_or;
extern Closure Bitwise_xor;
extern Closure Bitwise_complement;
extern Closure Bitwise_shiftLeftBy;
extern Closure Bitwise_shiftRightBy;
extern Closure Bitwise_shiftRightZfBy;

// =========================================

extern Closure Char_toCode;

// =========================================

extern Custom g_elm_core_Basics_LT;
extern Custom g_elm_core_Basics_EQ;
extern Custom g_elm_core_Basics_GT;

extern Custom g_elm_core_Maybe_Nothing;
extern Closure g_elm_core_Maybe_Just;

extern Closure g_elm_core_Array_initialize;
extern Closure g_elm_core_List_reverse;
extern Closure g_elm_core_List_foldl;

void* eval_elm_core_Result_Ok(void* args[]);
void* eval_elm_core_Result_Err(void* args[]);
void* eval_elm_core_Result_isOk(void* args[]);

// =========================================

// manage
int GC_init();
void GC_register_root(void** root);
void GC_init_root(void** global_permanent_ptr, void* (*init_func)());
void GC_collect_major();
void GC_collect_minor();
void* GC_execute(Closure* c);

// allocate
void* GC_allocate(bool push_to_stack, ptrdiff_t words);
void* GC_memcpy(void* dest, void* src, size_t words);

typedef u16 GcStackMapIndex;

void GC_stack_push_value(void* value);
void GC_stack_pop_frame(EvalFunction evaluator, void* result, GcStackMapIndex push);
GcStackMapIndex GC_get_stack_frame();
void GC_stack_tailcall(int count, ...);
void GC_stack_push_frame(EvalFunction evaluator);

// =========================================

extern Custom JsArray_empty;
extern Closure JsArray_singleton;
extern Closure JsArray_length;
extern Closure JsArray_initialize;
extern Closure JsArray_initializeFromList;
extern Closure JsArray_unsafeGet;
extern Closure JsArray_unsafeSet;
extern Closure JsArray_push;
extern Closure JsArray_foldl;
extern Closure JsArray_foldr;
extern Closure JsArray_map;
extern Closure JsArray_indexedMap;
extern Closure JsArray_slice;
extern Closure JsArray_appendN;

// =========================================

void* List_create(size_t len, void* values[]);
void* eval_List_append(void* args[]);

extern Closure List_cons;
extern Closure List_append;
extern Closure List_map2;
extern Closure List_sortBy;

// toArray & fromArray
// Needed because core Elm code leaks details of the JS implementation
// e.g. String.join and String.split use JS built-ins that need arrays
// In C, we might as well just operate on Elm data.
#define List_toArray Basics_identity
#define List_fromArray Basics_identity

// =========================================

void Platform_initOnIntercept(Closure* update, Closure* subscriptions);
Cons* Platform_initializeEffects();
void* eval_Platform_leaf(void*[]);

extern u32 Platform_managers_size;  // compiler-generated constant
extern Custom* Platform_effectManagers;
extern Closure sendToApp;

extern Closure Platform_batch;
extern Closure Platform_leaf;
extern Closure Platform_map;


// =========================================

void Queue_push(Queue* q, void* value);
void* Queue_shift(Queue* q);

// =========================================

void* eval_Scheduler_rawSpawn(void* args[]);
void* eval_Scheduler_spawn(void* args[]);
void* eval_Scheduler_succeed(void* args[]);
void* eval_Scheduler_fail(void* args[]);
void* eval_Scheduler_binding(void* args[]);
void* eval_Scheduler_andThen(void* args[]);
void* eval_Scheduler_onError(void* args[]);
void* eval_Scheduler_receive(void* args[]);
void* eval_Scheduler_rawSend(void* args[]);
void* eval_Scheduler_send(void* args[]);
void* eval_Scheduler_kill(void* args[]);

extern Closure Scheduler_succeed;
extern Closure Scheduler_fail;
extern Closure Scheduler_binding;
extern Closure Scheduler_andThen;
extern Closure Scheduler_onError;
extern Closure Scheduler_receive;
extern Closure Scheduler_rawSpawn;
extern Closure Scheduler_spawn;
extern Closure Scheduler_rawSend;
extern Closure Scheduler_send;
extern Closure Scheduler_kill;

// =========================================

void* eval_String_append(void* args[]);  // exposed for `++` (Utils_append)
size_t code_units(ElmString* s);

// extern Closure String_cons;
extern Closure String_uncons;
extern Closure String_append;
extern Closure String_length;
// extern Closure String_map;
// extern Closure String_filter;
// extern Closure String_reverse;
// extern Closure String_foldl;
extern Closure String_foldr;
extern Closure String_split;
extern Closure String_join;
extern Closure String_slice;
extern Closure String_trim;
extern Closure String_trimLeft;
extern Closure String_trimRight;
// extern Closure String_words;
// extern Closure String_lines;
// extern Closure String_toUpper;
// extern Closure String_toLower;
// extern Closure String_any;
extern Closure String_all;
extern Closure String_contains;
extern Closure String_startsWith;
extern Closure String_endsWith;
extern Closure String_indexes;
extern Closure String_fromNumber;
extern Closure String_toInt;
extern Closure String_toFloat;
// extern Closure String_fromList;

// =========================================

struct str_builder {
  Cons* first_section;
  Cons* last_section;
  u16* cursor;
  u16* end;
  u32 finished_sections_length;
};
typedef struct str_builder StringBuilder;

void StringBuilder_init(StringBuilder* sb);
ElmString* StringBuilder_toString(StringBuilder* sb);
void StringBuilder_ensureSpace(StringBuilder* sb, size_t need);
void StringBuilder_copyAscii(StringBuilder* sb, char* src);
void StringBuilder_writeChar(StringBuilder* sb, char c);
void StringBuilder_writeIndent(StringBuilder* sb, u32 indent_current);
void StringBuilder_startSection(StringBuilder* sb, size_t min_code_units);
void StringBuilder_finishSection(StringBuilder* sb);

// =========================================

extern Closure Utils_equal;
extern Closure Utils_notEqual;
extern Closure Utils_append;
extern Closure Utils_compare;

extern Closure Utils_lt;
extern Closure Utils_le;
extern Closure Utils_gt;
extern Closure Utils_ge;

void* Utils_apply(Closure* c_old, u16 n_applied, void* applied[]);
Record* Utils_update(Record* r, u32 n_updates, u32 fields[], void* values[]);
void* Utils_destruct_index(ElmValue* v, size_t index);
void* Utils_clone(void* x);
void* Utils_access_eval(void* args[2]);
void Utils_initGlobal(void** global, void* (*init_func)());
#define A1(fn, a) Utils_apply(fn, 1, (void*[]){a})
#define A2(fn, a, b) Utils_apply(fn, 2, (void*[]){a, b})
#define A3(fn, a, b, c) Utils_apply(fn, 3, (void*[]){a, b, c})
#define A4(fn, a, b, c, d) Utils_apply(fn, 4, (void*[]){a, b, c, d})
#define A5(fn, a, b, c, d, e) Utils_apply(fn, 5, (void*[]){a, b, c, d, e})
#define A6(fn, a, b, c, d, e, f) Utils_apply(fn, 6, (void*[]){a, b, c, d, e, f})
#define A7(fn, a, b, c, d, e, f, g) Utils_apply(fn, 7, (void*[]){a, b, c, d, e, f, g})
#define A8(fn, a, b, c, d, e, f, g, h) \
  Utils_apply(fn, 8, (void*[]){a, b, c, d, e, f, g, h})
#define A9(fn, a, b, c, d, e, f, g, h, i) \
  Utils_apply(fn, 9, (void*[]){a, b, c, d, e, f, g, h, i})

#endif
