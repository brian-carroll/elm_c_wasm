#ifdef __EMSCRIPTEN__
/*
  Not much to do here. All the functions are implemented in JS.
  Just declare any test-only imports
*/
#include "../kernel/core/types.h"
extern size_t writeJsTestValue(u32 id);

#else

/*
  Non-Wasm target => JS imports don't exist
  Need equivalent C implementations instead
  C-only version makes it easier to use C debug tools
*/

#include <assert.h>
#include <stddef.h>
#include <stdio.h>

#include "../kernel/core/core.h"
#include "../kernel/json/json-internal.h"
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

static int unusedJsHeapSlot;

#define JS_HEAP_MAX_LENGTH 100
static u32 jsHeapLength = 0;
static struct jsHeapEntry jsHeap[JS_HEAP_MAX_LENGTH];

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

static ptrdiff_t writeJsonValue(ElmValue* value, enum JsShape jsShape) {
  if (value == (void*)&Json_encodeNull || value == pTrue || value == pFalse) {
    return (ptrdiff_t)value;
  } else if (value->header.tag == Tag_Custom && jsShape == MAYBE_CYCLIC) {
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
    if (Utils_apply(&Utils_equal, 2, (void*[]){field, (void*)fieldStringAddr}) == &True) {
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

// ---------------------------------------------------
// Test values
// Cyclic values must be outside the GC-managed heap
// ---------------------------------------------------

static ElmString16 str_a = {
    .header = HEADER_STRING(1),
    .words16 = {'a'},
};
static ElmString16 str_c = {
    .header = HEADER_STRING(1),
    .words16 = {'c'},
};
static ElmInt two = {
    .header = HEADER_INT,
    .value = 2,
};
static Custom object_cyclic = {
    .header = HEADER_CUSTOM(4),
    .ctor = JSON_VALUE_OBJECT,
    .values =
        {
            &str_a,
            &object_cyclic,
            &str_c,
            &two,
        },
};
static Custom array_cyclic = {
    .header = HEADER_CUSTOM(2),
    .ctor = JSON_VALUE_ARRAY,
    .values =
        {
            &array_cyclic,
            &two,
        },
};

size_t writeJsTestValue(u32 id) {
  void* value;
  switch (id) {
    case TEST_JS_OBJECT_NON_CYCLIC:
      value = parse_json(create_string("{ a: { b: 1 }, c: 2 }"));
      break;
    case TEST_JS_OBJECT_CYCLIC:
      value = &object_cyclic;
      break;
    case TEST_JS_ARRAY_NON_CYCLIC:
      value = parse_json(create_string("[[1], 2]"));
      break;
    case TEST_JS_ARRAY_CYCLIC:
      value = &array_cyclic;
      break;
    default:
      printf("Unknown JS test value ID %d\n", id);
      value = &Json_encodeNull;
      break;
  }
  return writeJsonValue(value, MAYBE_CYCLIC);
};

#endif
