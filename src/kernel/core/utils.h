#ifndef ELM_KERNEL_UTILS
#define ELM_KERNEL_UTILS

#include "types.h"

extern Closure Utils_equal;
extern Closure Utils_notEqual;
extern Closure Utils_append;
extern Closure Utils_compare;

extern Closure Utils_lt;
extern Closure Utils_le;
extern Closure Utils_gt;
extern Closure Utils_ge;

extern Custom Utils_LT;
extern Custom Utils_EQ;
extern Custom Utils_GT;

void* Utils_apply(Closure* c_old, u16 n_applied, void* applied[]);
Record* Utils_update(Record* r, u32 n_updates, u32 fields[], void* values[]);
void* Utils_destruct_index(ElmValue* v, size_t index);
void* Utils_clone(void* x);
void* Utils_access_eval(void* args[2]);
void Utils_initGlobal(void** global, void* (*init_func)());

#define A1(fn, a) CAN_THROW(Utils_apply(fn, 1, (void*[]){a}))
#define A2(fn, a, b) CAN_THROW(Utils_apply(fn, 2, (void*[]){a, b}))
#define A3(fn, a, b, c) CAN_THROW(Utils_apply(fn, 3, (void*[]){a, b, c}))
#define A4(fn, a, b, c, d) CAN_THROW(Utils_apply(fn, 4, (void*[]){a, b, c, d}))
#define A5(fn, a, b, c, d, e) CAN_THROW(Utils_apply(fn, 5, (void*[]){a, b, c, d, e}))
#define A6(fn, a, b, c, d, e, f) \
  CAN_THROW(Utils_apply(fn, 6, (void*[]){a, b, c, d, e, f}))
#define A7(fn, a, b, c, d, e, f, g) \
  CAN_THROW(Utils_apply(fn, 7, (void*[]){a, b, c, d, e, f, g}))
#define A8(fn, a, b, c, d, e, f, g, h) \
  CAN_THROW(Utils_apply(fn, 8, (void*[]){a, b, c, d, e, f, g, h}))
#define A9(fn, a, b, c, d, e, f, g, h, i) \
  CAN_THROW(Utils_apply(fn, 9, (void*[]){a, b, c, d, e, f, g, h, i}))

#endif
