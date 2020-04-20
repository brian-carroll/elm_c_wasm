#ifndef ELM_KERNEL_STRING
#define ELM_KERNEL_STRING

#include "types.h"

void* eval_String_append(void* args[]);  // exposed for `++` (Utils_append)
size_t code_units(ElmString16* s);

// Closure String_cons;
Closure String_uncons;
Closure String_append;
Closure String_length;
// Closure String_map;
// Closure String_filter;
// Closure String_reverse;
// Closure String_foldl;
// Closure String_foldr;
Closure String_split;
Closure String_join;
Closure String_slice;
Closure String_trim;
Closure String_trimLeft;
Closure String_trimRight;
// Closure String_words;
// Closure String_lines;
// Closure String_toUpper;
// Closure String_toLower;
// Closure String_any;
Closure String_all;
Closure String_contains;
Closure String_startsWith;
Closure String_endsWith;
Closure String_indexes;
Closure String_fromNumber;
Closure String_toInt;
// Closure String_toFloat;
// Closure String_fromList;

#endif