#ifndef ELM_KERNEL_UTILS
#define ELM_KERNEL_UTILS

#include "types.h"

const Closure Utils_eq;
const Closure Utils_access;
const Closure Utils_append;
const Closure Utils_compare;

const Closure Utils_lt;
const Closure Utils_le;
const Closure Utils_gt;
const Closure Utils_ge;

const Custom Utils_LT;
const Custom Utils_EQ;
const Custom Utils_GT;

void* Utils_apply(Closure* c_old, u8 n_applied, void* applied[]);
Record* Utils_update(Record* r, u32 n_updates, u32 fields[], void* values[]);
void* Utils_clone(void* x);

#define A1(f, a) CAN_THROW(Utils_apply(f, 1, (void* []){a}))
#define A2(f, a, b) CAN_THROW(Utils_apply(f, 2, (void* []){a, b}))
#define A3(f, a, b, c) CAN_THROW(Utils_apply(f, 3, (void* []){a, b, c}))
#define A4(f, a, b, c, d) CAN_THROW(Utils_apply(f, 4, (void* []){a, b, c, d}))

#endif
