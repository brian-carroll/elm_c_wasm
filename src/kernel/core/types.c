#include "core.h"

#include <stdlib.h>
#include <string.h>


Cons* newCons(void* head, void* tail) {
  Cons* p = GC_allocate(true, SIZE_LIST);
  p->header = (Header)HEADER_LIST;
  p->head = head;
  p->tail = tail;
  if (Debug_is_target_in_range(p, (size_t*)p + p->header.size)) {
    Debug_pretty("allocation at target address", p);
  }
  return p;
}


Tuple2* newTuple2(void* a, void* b) {
  Tuple2* p = GC_allocate(true, SIZE_TUPLE2);
  p->header = (Header)HEADER_TUPLE2;
  p->a = a;
  p->b = b;
  if (Debug_is_target_in_range(p, (size_t*)p + p->header.size)) {
    Debug_pretty("allocation at target address", p);
  }
  return p;
}


Tuple3* newTuple3(void* a, void* b, void* c) {
  Tuple3* p = GC_allocate(true, SIZE_TUPLE3);
  p->header = (Header)HEADER_TUPLE3;
  p->a = a;
  p->b = b;
  p->c = c;
  if (Debug_is_target_in_range(p, (size_t*)p + p->header.size)) {
    Debug_pretty("allocation at target address", p);
  }
  return p;
}


ElmInt* newElmInt(i32 value) {
  ElmInt* p = GC_allocate(true, SIZE_INT);
  p->header = (Header)HEADER_INT;
  p->value = value;
  if (Debug_is_target_in_range(p, (size_t*)p + p->header.size)) {
    Debug_pretty("allocation at target address", p);
  }
  return p;
}

ElmFloat* newElmFloat(f64 value) {
  ElmFloat* p = GC_allocate(true, SIZE_FLOAT);
  p->header = (Header)HEADER_FLOAT;
  p->value = value;
  if (Debug_is_target_in_range(p, (size_t*)p + p->header.size)) {
    Debug_pretty("allocation at target address", p);
  }
  return p;
}


ElmChar* newElmChar(u32 value) {
  ElmChar* p = GC_allocate(true, SIZE_CHAR);
  p->header = (Header)HEADER_CHAR;
  p->value = value;
  if (Debug_is_target_in_range(p, (size_t*)p + p->header.size)) {
    Debug_pretty("allocation at target address", p);
  }
  return p;
}

// Strings are padded to the next 32/64-bit boundary.
ElmString* newElmString(size_t len16) {
  size_t used_bytes = sizeof(Header) + len16 * sizeof(u16);
  size_t aligned_words = (used_bytes + SIZE_UNIT - 1) / SIZE_UNIT;  // ceil
  size_t aligned_bytes = aligned_words * SIZE_UNIT;

  ElmString* p = GC_allocate(true, aligned_words);

  if (aligned_bytes != used_bytes) {
    size_t* words = (size_t*)p;
    words[aligned_words - 1] = 0;  // set padding to zero
  }

  // Write header _after_ padding in case there's only one word!
  p->header = (Header){
      .tag = Tag_String,
      .size = (u32)aligned_words,
  };

  if (Debug_is_target_in_range(p, (size_t*)p + p->header.size)) {
    Debug_pretty("allocation at target address", p);
  }
  return p;
}


Custom* newCustom(u32 ctor, u32 n_children, void* children[]) {
  Custom* c = GC_allocate(true, SIZE_CUSTOM(n_children));

  c->header = (Header)HEADER_CUSTOM(n_children);
  c->ctor = ctor;
  if (children != NULL) {
    for (size_t i = 0; i < n_children; ++i) {
      c->values[i] = children[i];
    }
  }
  if (Debug_is_target_in_range(c, (size_t*)c + c->header.size)) {
    Debug_pretty("allocation at target address", c);
  }
  return c;
}


Record* newRecord(FieldGroup* fg, u32 n_children, void* children[]) {
  Record* r = GC_allocate(true, SIZE_RECORD(n_children));
  r->header = (Header)HEADER_RECORD(n_children);
  r->fieldgroup = fg;
  for (size_t i = 0; i < n_children; ++i) {
    r->values[i] = children[i];
  }
  if (Debug_is_target_in_range(r, (size_t*)r + r->header.size)) {
    Debug_pretty("allocation at target address", r);
  }
  return r;
}


Closure* newClosure(
    u16 n_values, u16 max_values, void* (*evaluator)(void*[]), void* values[]) {
  Closure* c = GC_allocate(true, SIZE_CLOSURE(n_values));
  c->header = (Header)HEADER_CLOSURE(n_values);
  c->n_values = n_values;
  c->max_values = max_values;
  c->evaluator = evaluator;
  if (values != NULL) {
    for (size_t i = 0; i < n_values; ++i) {
      c->values[i] = values[i];
    }
  }
  if (Debug_is_target_in_range(c, (size_t*)c + c->header.size)) {
    Debug_pretty("allocation at target address", c);
  }
  return c;
}

JsRef* newJsRef(u32 id) {
  JsRef* jsRef = GC_allocate(true, SIZE_JS_REF);
  jsRef->header = (Header)HEADER_JS_REF;
  jsRef->id = id;
  return jsRef;
}

u32 custom_params(Custom* c) {
  return c->header.size - (sizeof(Custom) / SIZE_UNIT);
}

Cons Nil = {.header = HEADER_LIST, .head = NULL, .tail = NULL};
Custom Unit = {.header = HEADER_CUSTOM(0), .ctor = 0};
Custom True = {.header = HEADER_CUSTOM(0), .ctor = 1};
Custom False = {.header = HEADER_CUSTOM(0), .ctor = 0};

void* pNil = &Nil;
void* pUnit = &Unit;
void* pTrue = &True;
void* pFalse = &False;
