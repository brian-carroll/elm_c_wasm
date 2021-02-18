#include "core.h"

#include <stdlib.h>
#include <string.h>

Cons* newCons(void* head, void* tail) {
  Cons* p = GC_malloc(true, sizeof(Cons));
  p->header = (Header)HEADER_LIST;
  p->head = head;
  p->tail = tail;
  return p;
};

Tuple2* newTuple2(void* a, void* b) {
  Tuple2* p = GC_malloc(true, sizeof(Tuple2));
  p->header = (Header)HEADER_TUPLE2;
  p->a = a;
  p->b = b;
  return p;
};

Tuple3* newTuple3(void* a, void* b, void* c) {
  Tuple3* p = GC_malloc(true, sizeof(Tuple3));
  p->header = (Header)HEADER_TUPLE3;
  p->a = a;
  p->b = b;
  p->c = c;
  return p;
};

ElmInt* newElmInt(i32 value) {
  ElmInt* p = GC_malloc(true, sizeof(ElmInt));
  p->header = (Header)HEADER_INT;
  p->value = value;
  return p;
};

ElmFloat* newElmFloat(f64 value) {
  ElmFloat* p = GC_malloc(true, sizeof(ElmFloat));
  p->header = (Header)HEADER_FLOAT;
  p->value = value;
  return p;
};

ElmChar* newElmChar(u32 value) {
  ElmChar* p = GC_malloc(true, sizeof(ElmChar));
  p->header = (Header)HEADER_CHAR;
  p->value = value;
  return p;
};

// Strings are padded to the next 32/64-bit boundary.

ElmString* newElmString(size_t payload_bytes, char* str) {
  size_t used_bytes = sizeof(Header) + payload_bytes +
                      (STRING_ENCODING == UTF8);  // 1 byte for padding size
  size_t aligned_words = (used_bytes + SIZE_UNIT - 1) / SIZE_UNIT;  // ceil
  size_t aligned_bytes = aligned_words * SIZE_UNIT;

  ElmString* p = GC_malloc(true, aligned_bytes);
  size_t* words = (size_t*)p;  // the ElmString as an array of words

#if STRING_ENCODING == UTF16
  if (payload_bytes & 2) {
    words[aligned_words - 1] = 0;  // odd number of UTF-16 words => zero out the last word
  }
#elif STRING_ENCODING == UTF8
  // Insert zero padding, and last byte indicating size of padding
  // Store padding size minus 1, so that if there's only 1 byte of padding,
  // it's a zero. (Zero-terminated strings are handy for debug etc.)
  size_t padding_bytes = aligned_bytes - (used_bytes - 1);
  size_t last_byte_value = padding_bytes - 1;
  words[aligned_words - 1] = last_byte_value << (SIZE_UNIT - 1) * 8;
#else
  static_assert(0, "Unknown STRING_ENCODING, don't know how to add padding.");
#endif

  // Write header _after_ padding to avoid it getting overwritten for empty string
  p->header = (Header){
      .tag = Tag_String,
      .size = (u32)aligned_words,
  };

  // Copy the string body if provided
  if (str != NULL) {
    // Header is 32 bits => ElmString body is 32-bit aligned
    u32* i_from = (u32*)str;
    u32* i_end = i_from + (payload_bytes / 4);
    u32* i_to = (u32*)p->bytes;
    while (i_from < i_end) {
      *i_to++ = *i_from++;
    }

    // Last few bytes (<4)
    char* c_from = (char*)i_from;
    char* c_end = str + payload_bytes;
    char* c_to = (char*)i_to;
    while (c_from < c_end) {
      *c_to++ = *c_from++;
    }
  }

  return p;
}

ElmString16* newElmString16(size_t len16) {
  size_t used_bytes = sizeof(Header) + len16 * sizeof(u16);
  size_t aligned_words = (used_bytes + SIZE_UNIT - 1) / SIZE_UNIT;  // ceil
  size_t aligned_bytes = aligned_words * SIZE_UNIT;

  ElmString16* p = GC_malloc(true, aligned_bytes);

  if (aligned_bytes != used_bytes) {
    size_t* words = (size_t*)p;
    words[aligned_words - 1] = 0;  // set padding to zero
  }

  // Write header _after_ padding in case there's only one word!
  p->header = (Header){
      .tag = Tag_String,
      .size = (u32)aligned_words,
  };

  return p;
}

Custom* newCustom(u32 ctor, u32 n_children, void* children[]) {
  Custom* c = GC_malloc(true, sizeof(Custom) + n_children * sizeof(void*));

  c->header = (Header)HEADER_CUSTOM(n_children);
  c->ctor = ctor;
  if (children != NULL) {
    for (size_t i = 0; i < n_children; ++i) {
      c->values[i] = children[i];
    }
  }
  return c;
}

Record* newRecord(FieldGroup* fg, u32 n_children, void* children[]) {
  Record* r = GC_malloc(true, sizeof(Record) + n_children * sizeof(void*));
  r->header = (Header)HEADER_RECORD(n_children);
  r->fieldgroup = fg;
  for (size_t i = 0; i < n_children; ++i) {
    r->values[i] = children[i];
  }
  return r;
}
Closure* newClosure(
    u16 n_values, u16 max_values, void* (*evaluator)(void*[]), void* values[]) {
  Closure* c = GC_malloc(true, sizeof(Closure) + n_values * sizeof(void*));
  c->header = (Header)HEADER_CLOSURE(n_values);
  c->n_values = n_values;
  c->max_values = max_values;
  c->evaluator = evaluator;
  if (values != NULL) {
    for (size_t i = 0; i < n_values; ++i) {
      c->values[i] = values[i];
    }
  }
  return c;
}

u32 custom_params(Custom* c) {
  return c->header.size - (sizeof(Custom) / SIZE_UNIT);
}

Cons Nil = {.header = HEADER_LIST, .head = NULL, .tail = NULL};
Custom Unit = {.header = HEADER_CUSTOM(0), .ctor = 0};
Custom True = {.header = HEADER_CUSTOM(0), .ctor = 1};
Custom False = {.header = HEADER_CUSTOM(0), .ctor = 0};
Custom GcFull = {.header = HEADER_CUSTOM(0), .ctor = 0};

void* pNil = &Nil;
void* pUnit = &Unit;
void* pTrue = &True;
void* pFalse = &False;
void* pGcFull = &GcFull;
