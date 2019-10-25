#ifndef ELM_KERNEL_STRING
#define ELM_KERNEL_STRING

#include "types.h"

void* String_append_eval(void* args[]);  // exposed for `++`
const Closure String_append;

const Closure String_fromInt;

#endif
