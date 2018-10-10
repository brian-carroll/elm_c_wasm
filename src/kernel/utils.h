#ifndef ELM_KERNEL_UTILS
#define ELM_KERNEL_UTILS

#include "types.h"

void* apply(Closure* c_old, u8 n_applied, void* applied[]);

Closure eq;
void utils_init();

#endif
