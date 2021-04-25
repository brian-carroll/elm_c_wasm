// Declarations of values defined in Elm code
// that are referenced from C code
#ifndef CORE_H
#define CORE_H

#include "../wrapper/wrapper.h"
#include "types.h"

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

#ifdef _WIN32
#define FORMAT_HEX "%016zx"
#define FORMAT_PTR "%16p"
#define FORMAT_PTR_LEN 16
#elif TARGET_64BIT
#define FORMAT_HEX "%016zx"
#define FORMAT_PTR "%14p"
#define FORMAT_PTR_LEN 14
#else
#define FORMAT_HEX "%08zx"
#define FORMAT_PTR "%8p"
#define FORMAT_PTR_LEN 8
#endif

#define IS_OUTSIDE_HEAP(p) (heap->start > (size_t*)p || heap->end <= (size_t*)p)

void Debug_pretty(const char* label, void* p);
extern char* Debug_ctors[];
extern char* Debug_fields[];
extern char* Debug_jsValues[];
extern int Debug_ctors_size;
extern int Debug_fields_size;
extern int Debug_jsValues_size;
char* Debug_evaluator_name(void*);
extern char Debug_unknown_evaluator[];
extern Closure Debug_toString;
extern Closure Debug_log;
extern Closure Debug_todo;

void Debug_pause();

bool is_marked(void* p);
void print_value(void* p);
void print_value_full(void* p);
void print_heap_range(size_t* start, size_t* end);
void print_heap();
void print_state();
void print_stack_map();
void print_bitmap(const char* function, const char* filename, int line_no);
#define PRINT_BITMAP() print_bitmap(__FUNCTION__, __FILE__, __LINE__)
void format_ptr_diff_size(char* buffer, size_t buf_size, void* start, void* end);
void format_mem_size(char* buffer, size_t buf_size, size_t words);
void print_ptr_diff_size(void* start, void* end);
void print_mem_size(size_t words);
void print_gc_perf(void* perf_data, bool major);

void log_error(char* fmt, ...);
void log_debug(char* fmt, ...);

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
bool GC_collect_minor();
void* GC_execute(Closure* c);

// allocate
void* GC_allocate(bool push_to_stack, ptrdiff_t words);
void* GC_memcpy(void* dest, void* src, size_t words);


typedef u32 GcStackMapIndex;

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

void* eval_String_append(void* args[]);  // exposed for `++` (Utils_append)
size_t code_units(ElmString16* s);

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
