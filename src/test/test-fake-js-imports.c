#ifndef DISABLE_FAKE_JS_IMPORTS

#include <assert.h>
#include <stddef.h>

#include "../kernel/core/core.h"
#include "../kernel/json/json.h"
#include "test.h"

enum JsShape {
  NOT_CYCLIC,
  MAYBE_CYCLIC,
};

struct jsHeapEntry {
  void* value;
  bool isMarked;
  bool isOldGen;
};

int unusedJsHeapSlot;

#define JS_HEAP_MAX_LENGTH 100
u32 jsHeapLength = 0;
struct jsHeapEntry jsHeap[JS_HEAP_MAX_LENGTH];

static u32 storeJsRef(void* value) {
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

// entrypoint for JS heap C tests
ptrdiff_t writeJsonValue(ElmValue* value, enum JsShape jsShape) {
  if (jsShape == MAYBE_CYCLIC && value->header.tag == Tag_Custom) {
    JsRef* jsRef = GC_malloc(sizeof(JsRef));
    jsRef->header = HEADER_JS_REF;
    jsRef->index = storeJsRef(value);
    return (ptrdiff_t)jsRef;
  } else {
    return (ptrdiff_t)Utils_clone(value);
  }
}

ptrdiff_t getJsRefArrayIndex(u32 jsRefId, u32 index) {
  Custom* array = jsHeap[jsRefId].value;
  u32 len = custom_params(array);
  if (array->header.tag != Tag_Custom || array->ctor != JSON_VALUE_ARRAY ||
      index >= len) {
    return 0;
  }
  ElmValue* value = array->values[index];
  return writeJsonValue(value, MAYBE_CYCLIC);
}

ptrdiff_t getJsRefObjectField(u32 jsRefId, size_t fieldStringAddr) {
  Custom* obj = jsHeap[jsRefId].value;
  if (obj->header.tag != Tag_Custom || obj->ctor != JSON_VALUE_OBJECT) {
    return 0;
  }
  u32 len = custom_params(obj);
  void* value;
  u32 i;
  for (i = 0; i < len; i += 2) {
    ElmString16* field = obj->values[i];
    if (A2(&Utils_equal, field, (ElmString16*)fieldStringAddr) == &True) {
      value = obj->values[i + 1];
      break;
    }
  }
  if (i == len) return 0;

  return writeJsonValue(value, MAYBE_CYCLIC);
}

ptrdiff_t getJsRefValue(u32 jsRefId) {
  return writeJsonValue(jsHeap[jsRefId].value, NOT_CYCLIC);
}

#endif
