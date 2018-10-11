#ifndef ELM_KERNEL_UTILS
#define ELM_KERNEL_UTILS

#include "types.h"

void* apply(Closure* c_old, u8 n_applied, void* applied[]);

Closure eq;
Closure record_access;
void utils_init();

#endif
