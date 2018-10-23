#ifndef ELM_KERNEL_UTILS
#define ELM_KERNEL_UTILS

#include "types.h"

Closure eq;
Closure record_access;

void* apply(Closure* c_old, u8 n_applied, void* applied[]);
Record* record_update(Record* r, u32 n_updates, u32 fields[], void* values[]);
void* clone(void* x);
void init_utils();

#endif
