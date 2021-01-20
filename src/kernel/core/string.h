#ifndef ELM_KERNEL_STRING
#define ELM_KERNEL_STRING

#include "types.h"

void* eval_String_append(void* args[]);  // exposed for `++` (Utils_append)
size_t code_units(ElmString16* s);

// extern Closure String_cons;
extern Closure String_uncons;
extern Closure String_append;
extern Closure String_length;
// extern Closure String_map;
// extern Closure String_filter;
// extern Closure String_reverse;
// extern Closure String_foldl;
extern Closure String_foldr;
extern Closure String_split;
extern Closure String_join;
extern Closure String_slice;
extern Closure String_trim;
extern Closure String_trimLeft;
extern Closure String_trimRight;
// extern Closure String_words;
// extern Closure String_lines;
// extern Closure String_toUpper;
// extern Closure String_toLower;
// extern Closure String_any;
extern Closure String_all;
extern Closure String_contains;
extern Closure String_startsWith;
extern Closure String_endsWith;
extern Closure String_indexes;
extern Closure String_fromNumber;
extern Closure String_toInt;
extern Closure String_toFloat;
// extern Closure String_fromList;

#endif