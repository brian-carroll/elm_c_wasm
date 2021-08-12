#include "../core/core.h"
#include "json-internal.h"
#include "json.h"


Custom Json_null = {
    .header = HEADER_CUSTOM(0),
    .ctor = JSON_VALUE_NULL,
};

Custom Json_encodeNull = {
    .header = HEADER_CUSTOM(1),
    .ctor = JSON_VALUE_WRAP,
    .values = {&Json_null},
};


void* eval_Json_wrap(void* args[]) {
  ElmValue* v = args[0];
  if (v->header.tag == Tag_Int) {
    v = (ElmValue*)newElmFloat(v->elm_int.value);
  }
  Custom* wrapped = newCustom(JSON_VALUE_WRAP, 1, (void*[]){v});
  return wrapped;
}
Closure Json_wrap = {
    .header = HEADER_CLOSURE(0),
    .max_values = 1,
    .evaluator = &eval_Json_wrap,
};


void* eval_Json_unwrap(void* args[]) {
  Custom* x = args[0];
  return x->values[0];
}
Closure Json_unwrap = {
    .header = HEADER_CLOSURE(0),
    .max_values = 1,
    .evaluator = &eval_Json_unwrap,
};


Custom emptyArray = {
    .header = HEADER_CUSTOM(0),
    .ctor = JSON_VALUE_ARRAY,
};
void* eval_Json_emptyArray(void* args[]) {
  return &emptyArray;
}
Closure Json_emptyArray = {
    .header = HEADER_CLOSURE(0),
    .max_values = 1,
    .evaluator = &eval_Json_emptyArray,
};


Custom emptyObject = {
    .header = HEADER_CUSTOM(0),
    .ctor = JSON_VALUE_OBJECT,
};
void* eval_Json_emptyObject(void* args[]) {
  return &emptyObject;
}
Closure Json_emptyObject = {
    .header = HEADER_CLOSURE(0),
    .max_values = 1,
    .evaluator = &eval_Json_emptyObject,
};


void* eval_Json_addField(void* args[]) {
  ElmString* key = args[0];
  Custom* wrappedValue = args[1];
  Custom* old_object = args[2];

  u32 old_params = custom_params(old_object);
  u32 old_size = old_object->header.size;
  u32 new_size = old_size + 2;

  Custom* new_object = GC_allocate(true, new_size);
  GC_memcpy(new_object, old_object, old_size);
  new_object->header.size = new_size;
  new_object->values[old_params] = key;
  new_object->values[old_params + 1] = wrappedValue->values[0];

  return new_object;
}
Closure Json_addField = {
    .header = HEADER_CLOSURE(0),
    .max_values = 3,
    .evaluator = &eval_Json_addField,
};


void* eval_Json_addEntry(void* args[]) {
  Closure* func = args[0];
  void* entry = args[1];
  Custom* old_array = args[2];

  u32 old_params = custom_params(old_array);
  u32 old_size = old_array->header.size;
  u32 new_size = old_size + 1;

  Custom* new_array = GC_allocate(true, new_size);
  GC_memcpy(new_array, old_array, old_size);
  new_array->header.size = new_size;

  Custom* wrappedEntry = A1(func, entry);
  new_array->values[old_params] = wrappedEntry->values[0];

  return new_array;
}
Closure Json_addEntry = {
    .header = HEADER_CLOSURE(0),
    .max_values = 3,
    .evaluator = &eval_Json_addEntry,
};


size_t stringify_alloc_chunk_words;


void* eval_Json_encode(void* args[]) {
  ElmInt* indentLevel = args[0];
  Custom* wrapped = args[1];
  void* value = wrapped->values[0];

  StringBuilder sb;
  StringBuilder_init(&sb);
  u32 indent = indentLevel->value;
  stringify(indent, 0, value, &sb);
  ElmString* str = StringBuilder_toString(&sb);

  return str;
}
Closure Json_encode = {
    .header = HEADER_CLOSURE(0),
    .max_values = 2,
    .evaluator = &eval_Json_encode,
};
