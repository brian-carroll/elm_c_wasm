#ifndef ELM_KERNEL_JS_ARRAY
#define ELM_KERNEL_JS_ARRAY

#include "types.h"


extern Custom JsArray_empty;
extern Closure JsArray_singleton;
extern Closure JsArray_length;
extern Closure JsArray_initialize;
extern Closure JsArray_initializeFromList;
extern Closure JsArray_unsafeGet;
extern Closure JsArray_unsafeSet;
extern Closure JsArray_push;
extern Closure JsArray_foldl;
extern Closure JsArray_foldr;
extern Closure JsArray_map;
extern Closure JsArray_indexedMap;
extern Closure JsArray_slice;
extern Closure JsArray_appendN;

#endif
