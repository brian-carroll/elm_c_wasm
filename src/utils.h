#ifndef ELM_KERNEL_UTILS
#define ELM_KERNEL_UTILS

#include "types.h"

void* apply(Closure* c_old, u8 n_applied, void* applied[]);

ElmInt* String_length_bytes(ElmString *s);
u8* eq(void* ptr_a, void* ptr_b);

#endif
