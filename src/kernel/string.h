#ifndef ELM_KERNEL_STRING
#define ELM_KERNEL_STRING

#include "types.h"

void* String_append_eval(void* args[]);  // exposed for `++`
Closure String_append;
Closure String_fromNumber;
Closure String_join;
Closure String_split;
Closure String_all;
Closure String_length;
Closure String_slice;
Closure String_toInt;
Closure String_uncons;
Closure String_contains;
Closure String_indexes;
Closure String_startsWith;
Closure String_trim;

#endif
