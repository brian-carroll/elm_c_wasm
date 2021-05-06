#ifndef __EMSCRIPTEN__

/*
  Non-Wasm target => JS imports don't exist
  Need equivalent C implementations instead
  C-only version makes it easier to use C debug tools
*/

#include <assert.h>
#include <stddef.h>

#include "../kernel/core/core.h"
#include "../kernel/json/json-internal.h"
#include "../kernel/json/json.h"
#include "test.h"

enum JsShape {
  NOT_CIRCULAR,
  MAYBE_CIRCULAR,
};

struct jsHeapEntry {
  void* value;
  bool isMarked;
  bool isOldGen;
};

static int unusedJsHeapSlot;

#define JS_HEAP_MAX_LENGTH 100
static u32 jsHeapLength = 0;
static struct jsHeapEntry jsHeap[JS_HEAP_MAX_LENGTH];

static u32 allocateJsRef(void* value) {
  u32 id = 0;
  while (id < jsHeapLength && jsHeap[id].value != &unusedJsHeapSlot)
    id++;
  if (id == jsHeapLength) {
    jsHeapLength++;
    assert(jsHeapLength < JS_HEAP_MAX_LENGTH);
  }
  jsHeap[id] = (struct jsHeapEntry){
      .value = value,
      .isMarked = false,
      .isOldGen = false,
  };
  return id;
}

void markJsRef(u32 id) {
  jsHeap[id].isMarked = true;
}

void sweepJsRefs(bool isFullGc) {
  u32 lastMarked = 0;
  for (u32 index = 0; index < jsHeapLength; index++) {
    struct jsHeapEntry* slot = &jsHeap[index];
    if (slot->isMarked || (!isFullGc && slot->isOldGen)) {
      lastMarked = index;
      if (isFullGc) {
        slot->isOldGen = true;
      }
    } else {
      slot->value = &unusedJsHeapSlot;
      slot->isOldGen = false;
    }
    slot->isMarked = false;
  }
  jsHeapLength = lastMarked + 1;
}

static size_t writeJsonValue(ElmValue* value, enum JsShape jsShape) {
  if (value->header.tag != Tag_Custom) {
    return (size_t)Utils_clone(value);
  }
  Custom* c = &value->custom;
  if (c == &Json_encodeNull || c == &True || c == &False) {
    return (size_t)c;
  }
  if (c->ctor == JSON_VALUE_WRAP) {
    void* unwrapped = (void*)writeJsonValue(c->values[0], jsShape);
    Custom* wrapped = GC_allocate(true, SIZE_CUSTOM(1));
    wrapped->header = (Header)HEADER_CUSTOM(1);
    wrapped->ctor = JSON_VALUE_WRAP;
    wrapped->values[0] = unwrapped;
    return (size_t)wrapped;
  }
  if (jsShape == MAYBE_CIRCULAR) {
    JsRef* jsRef = GC_allocate(true, SIZE_JS_REF);
    jsRef->header = (Header)HEADER_JS_REF;
    jsRef->index = allocateJsRef(value);
    return (size_t)jsRef;
  }
  return (size_t)Utils_clone(value);
}

ptrdiff_t getJsRefArrayIndex(u32 jsRefId, u32 index) {
  Custom* array = jsHeap[jsRefId].value;
  if (array->header.tag != Tag_Custom || array->ctor != JSON_VALUE_ARRAY) return 0;
  u32 len = custom_params(array);
  if (index >= len) return -((ptrdiff_t)len + 1);
  ElmValue* value = array->values[index];
  return writeJsonValue(value, MAYBE_CIRCULAR);
}

ptrdiff_t getJsRefObjectField(u32 jsRefId, size_t fieldStringAddr) {
  Custom* obj = jsHeap[jsRefId].value;
  if (obj->header.tag != Tag_Custom || obj->ctor != JSON_VALUE_OBJECT) {
    return 0;
  }
  u32 len = custom_params(obj);
  void* value = &Json_encodeNull;
  u32 i;
  for (i = 0; i < len; i += 2) {
    ElmString16* field = obj->values[i];
    if (Utils_apply(&Utils_equal, 2, (void*[]){field, (void*)fieldStringAddr}) == &True) {
      value = obj->values[i + 1];
      break;
    }
  }
  if (i == len) return 0;

  return writeJsonValue(value, MAYBE_CIRCULAR);
}

ptrdiff_t getJsRefValue(u32 jsRefId) {
  return writeJsonValue(jsHeap[jsRefId].value, NOT_CIRCULAR);
}

// ---------------------------------------------------
// Test values
// Circular values must be outside the GC-managed heap
// ---------------------------------------------------

static ElmString16 str_a = {
    .header = HEADER_STRING(1),
    .words16 = {'a'},
};
static ElmString16 str_b = {
    .header = HEADER_STRING(1),
    .words16 = {'b'},
};
static ElmInt num = {.header = HEADER_INT, .value = 2};
static Custom object_circular = {
    .header = HEADER_CUSTOM(4),
    .ctor = JSON_VALUE_OBJECT,
    .values =
        {
            &str_a,
            &object_circular,
            &str_b,
            &num,
        },
};
static Custom array_circular = {
    .header = HEADER_CUSTOM(2),
    .ctor = JSON_VALUE_ARRAY,
    .values =
        {
            &array_circular,
            &num,
        },
};

size_t testCircularJsValue(bool isArray) {
  void* value = isArray ? &array_circular : &object_circular;
  void* json_wrapped = Utils_apply(&Json_wrap, 1, (void*[]){value});
  return writeJsonValue(json_wrapped, MAYBE_CIRCULAR);
};

size_t testJsonValueRoundTrip(size_t jsonStringAddr) {
  void* value = parse_json((ElmString16*)jsonStringAddr);
  void* json_wrapped = Utils_apply(&Json_wrap, 1, (void*[]){value});
  return writeJsonValue(json_wrapped, MAYBE_CIRCULAR);
}

#endif
