#ifndef ELM_KERNEL_UTILS
#define ELM_KERNEL_UTILS

#include "types.h"

Closure eq;
Closure Utils_access;

void* apply(Closure* c_old, u8 n_applied, void* applied[]);
Record* Utils_update(Record* r, u32 n_updates, u32 fields[], void* values[]);
void* clone(void* x);
void init_utils();

#define A2(f,a,b) apply(f, 2, (void*[]){a,b})
#define A3(f,a,b,c) apply(f, 3, (void*[]){a,b,c})
#define A4(f,a,b,c,d) apply(f, 4, (void*[]){a,b,c,d})

#endif
