#ifndef ELM_KERNEL_UTILS
#define ELM_KERNEL_UTILS

#include "types.h"

void* apply(Closure* c_old, u8 n_applied, void* applied[]);

Closure eq;
Closure record_access;

Record* record_update(Record* r, u32 n_updates, u32 fields[], void* values[]);

void utils_init();

#endif
