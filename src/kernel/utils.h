#ifndef ELM_KERNEL_UTILS
#define ELM_KERNEL_UTILS

#include "types.h"

Closure Utils_eq;
Closure Utils_access;
Closure Utils_append;

void* Utils_apply(Closure* c_old, u8 n_applied, void* applied[]);
Record* Utils_update(Record* r, u32 n_updates, u32 fields[], void* values[]);
void* Utils_clone(void* x);
void Utils_init();

#define A1(f,a) Utils_apply(f, 1, (void*[]){a})
#define A2(f,a,b) Utils_apply(f, 2, (void*[]){a,b})
#define A3(f,a,b,c) Utils_apply(f, 3, (void*[]){a,b,c})
#define A4(f,a,b,c,d) Utils_apply(f, 4, (void*[]){a,b,c,d})

#endif
