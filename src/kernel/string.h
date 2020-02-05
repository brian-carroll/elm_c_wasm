#ifndef ELM_KERNEL_STRING
#define ELM_KERNEL_STRING

#include "types.h"

void* String_append_eval(void* args[]);  // exposed for `++`
Closure String_append;

Closure String_fromNumber;
Closure String_join;
Closure String_split;

#endif
