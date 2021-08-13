#ifndef __EMSCRIPTEN__

/*
  Emulate JavaScript functions in C, for non-Wasm platforms
*/

#include <stddef.h>
#include <stdio.h>  // sscanf

#include "../kernel/core/core.h"
#include "../kernel/json/json-internal.h"
#include "../kernel/json/json.h"
#include "test.h"

typedef enum js_shape {
  NOT_CIRCULAR,
  MAYBE_CIRCULAR,
} JsShape;

typedef enum js_heap_gen {
  GEN_CONST,
  GEN_OLD,
  GEN_NEW,
} JsHeapGen;

typedef struct js_heap_entry {
  void* value;
  bool isMarked;
  JsHeapGen generation;
} JsHeapEntry;

static int unusedJsHeapSlot;

#define JS_HEAP_MAX_LENGTH 100
u32 jsHeapLength = 0;
static JsHeapEntry jsHeap[JS_HEAP_MAX_LENGTH];

u32 allocateJsRef(void* value) {
  u32 id = 0;
  while (id < jsHeapLength && jsHeap[id].value != &unusedJsHeapSlot)
    id++;
  if (id == jsHeapLength) {
    jsHeapLength++;
    ASSERT(jsHeapLength < JS_HEAP_MAX_LENGTH, jsHeapLength);
  }
  jsHeap[id] = (JsHeapEntry){
      .value = value,
      .isMarked = false,
      .generation = GEN_NEW,
  };
  return id;
}

void markJsRef(u32 id) {
  jsHeap[id].isMarked = true;
}

void sweepJsRefs(bool isFullGc) {
  i32 lastUsedSlot = -1;
  for (u32 index = 0; index < jsHeapLength; index++) {
    JsHeapEntry* slot = &jsHeap[index];
    bool shouldKeep = slot->isMarked || (!isFullGc && slot->generation != GEN_NEW);
    if (!shouldKeep) {
      slot->value = &unusedJsHeapSlot;
    } else {
      if (isFullGc && slot->generation == GEN_NEW) {
        slot->generation = GEN_OLD;
      }
      lastUsedSlot = index;
    }
    slot->isMarked = false;
  }
  jsHeapLength = lastUsedSlot + 1;
}

static void* writeJsonValue(ElmValue* value, JsShape jsShape) {
  if (value->header.tag != Tag_Custom) {
    return Utils_clone(value);
  }
  Custom* c = &value->custom;
  if (c == &Json_null || c == &True || c == &False) {
    return c;
  }
  if (c->ctor == JSON_VALUE_WRAP) {
    void* unwrapped = (void*)writeJsonValue(c->values[0], jsShape);
    Custom* wrapped = GC_allocate(true, SIZE_CUSTOM(1));
    wrapped->header = (Header)HEADER_CUSTOM(1);
    wrapped->ctor = JSON_VALUE_WRAP;
    wrapped->values[0] = unwrapped;
    return wrapped;
  }
  if (jsShape == MAYBE_CIRCULAR) {
    JsRef* jsRef = GC_allocate(true, SIZE_JS_REF);
    jsRef->header = (Header)HEADER_JS_REF;
    jsRef->id = allocateJsRef(value);
    return jsRef;
  }
  return Utils_clone(value);
}

ptrdiff_t getJsRefArrayIndex(u32 jsRefId, u32 index) {
  Custom* array = jsHeap[jsRefId].value;
  if (array->header.tag != Tag_Custom || array->ctor != JSON_VALUE_ARRAY) return 0;
  u32 len = custom_params(array);
  if (index >= len) return -((ptrdiff_t)len + 1);
  ElmValue* value = array->values[index];
  return (ptrdiff_t)writeJsonValue(value, MAYBE_CIRCULAR);
}

void* getJsRefObjectField(u32 jsRefId, ElmString* fieldStringAddr) {
  Custom* obj = jsHeap[jsRefId].value;
  if (obj->header.tag != Tag_Custom || obj->ctor != JSON_VALUE_OBJECT) {
    return 0;
  }
  u32 len = custom_params(obj);
  void* value = &Json_null;
  u32 i;
  for (i = 0; i < len; i += 2) {
    ElmString* field = obj->values[i];
    if (Utils_apply(&Utils_equal, 2, (void*[]){field, fieldStringAddr}) == &True) {
      value = obj->values[i + 1];
      break;
    }
  }
  if (i == len) return 0;

  return writeJsonValue(value, MAYBE_CIRCULAR);
}

void* getJsRefValue(u32 jsRefId) {
  return writeJsonValue(jsHeap[jsRefId].value, NOT_CIRCULAR);
}

void* applyJsRef(u32 jsRefId, u32 nArgs, void* args[]) {
  Closure* c = jsHeap[jsRefId].value;
  return Utils_apply(c, nArgs, args);
}

Record* jsRefToWasmRecord(u32 jsRefId) {
  return jsHeap[jsRefId].value;
}

Tuple2* Wrapper_setupOutgoingPort() {
  return NULL;
}

u32 Wrapper_setupIncomingPort(u32 managerId) {
  return 0;
}

Task* Wrapper_sleep(f64 time) {
  return NULL;
}


// ---------------------------------------------------
// Test values
// Circular values must be outside the GC-managed heap
// ---------------------------------------------------

static ElmString str_a = {
    .header = HEADER_STRING(1),
    .words16 = {'a'},
};
static ElmString str_b = {
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

void* testCircularJsValue(bool isArray) {
  void* value = isArray ? &array_circular : &object_circular;
  void* json_wrapped = Utils_apply(&Json_wrap, 1, (void*[]){value});
  return writeJsonValue(json_wrapped, MAYBE_CIRCULAR);
};

void* testJsonValueRoundTrip(ElmString* jsonStringAddr) {
  void* value = parse_json((ElmString*)jsonStringAddr);
  void* json_wrapped = Utils_apply(&Json_wrap, 1, (void*[]){value});
  return writeJsonValue(json_wrapped, MAYBE_CIRCULAR);
}

// emulate the JS parseFloat function in C
f64 parseFloat(u16* chars16, size_t len16) {
  ASSERT(len16 <= 32, len16);
  char ascii[32];
  int i = 0;
  for (; i < len16; i++) {
    ascii[i] = chars16[i];
  }
  ascii[i] = '\0';
  f64 f;
  int successCount = sscanf(ascii, "%lg", &f);

#ifdef _MSC_VER
  f64 not_a_number = nan(NULL);
#else
  f64 not_a_number = 0.0 / 0.0;
#endif

  return successCount ? f : not_a_number;
}


void jsStepper(void* viewMetadata) {
  return;
}

#endif
