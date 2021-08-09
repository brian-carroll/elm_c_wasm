#include "core.h"
#include "../json/json.h"

/**
 * empty : JsArray a
 */
Custom JsArray_empty = {
    .header = HEADER_CUSTOM(0),
    .ctor = JSON_VALUE_ARRAY,
};

/**
 * singleton : a -> JsArray a
 */
void* eval_JsArray_singleton(void* args[]) {
  return newCustom(JSON_VALUE_ARRAY, 1, args);
}
Closure JsArray_singleton = {
    .header = HEADER_CLOSURE(0),
    .evaluator = eval_JsArray_singleton,
    .max_values = 1,
};

/**
 * length : JsArray a -> Int
 */
void* eval_JsArray_length(void* args[]) {
  i32 len = custom_params(args[0]);
  return newElmInt(len);
}
Closure JsArray_length = {
    .header = HEADER_CLOSURE(0),
    .evaluator = eval_JsArray_length,
    .max_values = 1,
};

/**
 * initialize : Int -> Int -> (Int -> a) -> JsArray a
 */
void* eval_JsArray_initialize(void* args[]) {
  ElmInt* size = args[0];
  ElmInt* offset = args[1];
  Closure* func = args[2];

  u32 len = size->value;
  i32 o = offset->value;
  Custom* result = newCustom(JSON_VALUE_ARRAY, len, NULL);

  for (size_t i = 0; i < len; i++) {
    ElmInt* num = newElmInt(o + i);
    result->values[i] = A1(func, num);
  }

  return result;
}
Closure JsArray_initialize = {
    .header = HEADER_CLOSURE(0),
    .evaluator = eval_JsArray_initialize,
    .max_values = 3,
};

/**
 * initializeFromList : Int -> List a -> ( JsArray a, List a )
 */
void* eval_JsArray_initializeFromList(void* args[]) {
  ElmInt* max = args[0];
  Cons* ls = args[1];

  ptrdiff_t max_len = max->value;
  Custom* result = newCustom(JSON_VALUE_ARRAY, max_len, NULL);

  ptrdiff_t i = 0;
  for (; i < max_len && ls != pNil; i++) {
    result->values[i] = ls->head;
    ls = ls->tail;
  }
  result->header.size = SIZE_CUSTOM(i); // Array logic depends on this
  ptrdiff_t reclaim = i - max_len;
  GC_allocate(false, reclaim); // return unused memory to the GC

  return newTuple2(result, ls);
}
Closure JsArray_initializeFromList = {
    .header = HEADER_CLOSURE(0),
    .evaluator = eval_JsArray_initializeFromList,
    .max_values = 2,
};

/**
 * unsafeGet : Int -> JsArray a -> a
 */
void* eval_JsArray_unsafeGet(void* args[]) {
  ElmInt* index = args[0];
  Custom* array = args[1];
  i32 i = index->value;
  return array->values[i];
}
Closure JsArray_unsafeGet = {
    .header = HEADER_CLOSURE(0),
    .evaluator = eval_JsArray_unsafeGet,
    .max_values = 2,
};

/**
 * unsafeSet : Int -> a -> JsArray a -> JsArray a
 */
void* eval_JsArray_unsafeSet(void* args[]) {
  ElmInt* index = args[0];
  void* value = args[1];
  Custom* array = args[2];

  Custom* result = Utils_clone(array);
  i32 i = index->value;
  result->values[i] = value;
  return result;
}
Closure JsArray_unsafeSet = {
    .header = HEADER_CLOSURE(0),
    .evaluator = eval_JsArray_unsafeSet,
    .max_values = 3,
};

/**
 * push : a -> JsArray a -> JsArray a
 */
void* eval_JsArray_push(void* args[]) {
  void* value = args[0];
  Custom* array = args[1];

  u32 old_size = array->header.size;
  u32 new_size = old_size + 1;
  Custom* result = GC_allocate(true, new_size);
  GC_memcpy(result, array, old_size);

  result->header.size = new_size;
  void** result_ptrs = (void**)result;
  result_ptrs[new_size - 1] = value;
  return result;
}
Closure JsArray_push = {
    .header = HEADER_CLOSURE(0),
    .evaluator = eval_JsArray_push,
    .max_values = 2,
};

/**
 * foldl : (a -> b -> b) -> b -> JsArray a -> b
 */
void* eval_JsArray_foldl(void* args[]) {
  Closure* func = args[0];
  void* acc = args[1];
  Custom* array = args[2];

  size_t len = custom_params(array);
  for (size_t i = 0; i < len; i++) {
    acc = A2(func, array->values[i], acc);
  }

  return acc;
}
Closure JsArray_foldl = {
    .header = HEADER_CLOSURE(0),
    .evaluator = eval_JsArray_foldl,
    .max_values = 3,
};

/**
 * foldr : (a -> b -> b) -> b -> JsArray a -> b
 */
void* eval_JsArray_foldr(void* args[]) {
  Closure* func = args[0];
  void* acc = args[1];
  Custom* array = args[2];

  i32 len = custom_params(array);
  for (i32 i = len - 1; i >= 0; i--) {
    void* child = array->values[i];
    acc = A2(func, child, acc);
  }

  return acc;
}
Closure JsArray_foldr = {
    .header = HEADER_CLOSURE(0),
    .evaluator = eval_JsArray_foldr,
    .max_values = 3,
};

/**
 * map : (a -> b) -> JsArray a -> JsArray b
 */
void* eval_JsArray_map(void* args[]) {
  Closure* func = args[0];
  Custom* array = args[1];

  size_t len = custom_params(array);
  Custom* result = newCustom(JSON_VALUE_ARRAY, len, NULL);
  for (size_t i = 0; i < len; i++) {
    result->values[i] = A1(func, array->values[i]);
  }

  return result;
}
Closure JsArray_map = {
    .header = HEADER_CLOSURE(0),
    .evaluator = eval_JsArray_map,
    .max_values = 3,
};

/**
 * indexedMap : (Int -> a -> b) -> Int -> JsArray a -> JsArray b
 */
void* eval_JsArray_indexedMap(void* args[]) {
  Closure* func = args[0];
  ElmInt* offset = args[1];
  Custom* array = args[2];

  i32 o = offset->value;
  i32 len = custom_params(array);
  Custom* result = newCustom(JSON_VALUE_ARRAY, len, NULL);
  for (i32 i = 0; i < len; i++) {
    ElmInt* num =  newElmInt(i + o);
    result->values[i] = A2(func, num, array->values[i]);
  }

  return result;
}
Closure JsArray_indexedMap = {
    .header = HEADER_CLOSURE(0),
    .evaluator = eval_JsArray_indexedMap,
    .max_values = 3,
};

/**
 * slice : Int -> Int -> JsArray a -> JsArray a
 */
void* eval_JsArray_slice(void* args[]) {
  ElmInt* from = args[0];
  ElmInt* to = args[1];
  Custom* array = args[2];

  i32 len = (i32)to->value - (i32)from->value;
  Custom* result = newCustom(JSON_VALUE_ARRAY, len, NULL);
  i32 j = 0;
  for (i32 i = from->value; i < to->value; i++) {
    result->values[j] = array->values[i];
    j++;
  }

  return result;
}
Closure JsArray_slice = {
    .header = HEADER_CLOSURE(0),
    .evaluator = eval_JsArray_slice,
    .max_values = 3,
};

/**
 * appendN : Int -> JsArray a -> JsArray a -> JsArray a
 */
void* eval_JsArray_appendN(void* args[]) {
  ElmInt* n = args[0];
  Custom* dest = args[1];
  Custom* source = args[2];

  i32 destLen = custom_params(dest);
  i32 sourceLen = custom_params(source);
  i32 itemsToCopy = n->value - destLen;

  if (itemsToCopy > sourceLen) {
    itemsToCopy = sourceLen;
  }

  i32 size = destLen + itemsToCopy;
  Custom* result = newCustom(JSON_VALUE_ARRAY, size, NULL);

  for (i32 i = 0; i < destLen; i++) {
    result->values[i] = dest->values[i];
  }

  for (i32 i = 0; i < itemsToCopy; i++) {
    result->values[i + destLen] = source->values[i];
  }

  return result;
}
Closure JsArray_appendN = {
    .header = HEADER_CLOSURE(0),
    .evaluator = eval_JsArray_appendN,
    .max_values = 3,
};
