#include "types.h"

#include <stdlib.h>
#include <string.h>

#include "gc.h"

// see also NEW_CONS in header file
Cons* ctorCons(void* head, void* tail) {
  Cons* p = CAN_THROW(GC_malloc(sizeof(Cons)));
  p->header = HEADER_LIST;
  p->head = head;
  p->tail = tail;
  return p;
};

// see also NEW_TUPLE2 in header file
Tuple2* ctorTuple2(void* a, void* b) {
  Tuple2* p = CAN_THROW(GC_malloc(sizeof(Tuple2)));
  p->header = HEADER_TUPLE2;
  p->a = a;
  p->b = b;
  return p;
};

// see also NEW_TUPLE3 in header file
Tuple3* ctorTuple3(void* a, void* b, void* c) {
  Tuple3* p = CAN_THROW(GC_malloc(sizeof(Tuple3)));
  p->header = HEADER_TUPLE3;
  p->a = a;
  p->b = b;
  p->c = c;
  return p;
};

// see also NEW_ELM_INT in header file
ElmInt* ctorElmInt(i32 value) {
  ElmInt* p = CAN_THROW(GC_malloc(sizeof(ElmInt)));
  p->header = HEADER_INT;
  p->value = value;
  return p;
};

// see also NEW_ELM_FLOAT in header file
ElmFloat* ctorElmFloat(f64 value) {
  ElmFloat* p = CAN_THROW(GC_malloc(sizeof(ElmFloat)));
  p->header = HEADER_FLOAT;
  p->value = value;
  return p;
};

// see also NEW_ELM_CHAR in header file
ElmChar* ctorElmChar(u32 value) {
  ElmChar* p = CAN_THROW(GC_malloc(sizeof(ElmChar)));
  p->header = HEADER_CHAR;
  p->value = value;
  return p;
};

// Strings are padded to the next 32/64-bit boundary.

// see also NEW_ELM_STRING in header file
ElmString* ctorElmString(size_t payload_bytes, char* str) {
  size_t used_bytes = sizeof(Header) + payload_bytes +
                      (STRING_ENCODING == UTF8);  // 1 byte for padding size
  size_t aligned_words = (used_bytes + SIZE_UNIT - 1) / SIZE_UNIT;  // ceil
  size_t aligned_bytes = aligned_words * SIZE_UNIT;

  ElmString* p = CAN_THROW(GC_malloc(aligned_bytes));
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

// see also NEW_ELM_STRING16 in header file
ElmString16* ctorElmString16(size_t len16) {
  size_t used_bytes = sizeof(Header) + len16 * sizeof(u16);
  size_t aligned_words = (used_bytes + SIZE_UNIT - 1) / SIZE_UNIT;  // ceil
  size_t aligned_bytes = aligned_words * SIZE_UNIT;

  ElmString16* p = CAN_THROW(GC_malloc(aligned_bytes));

  if (aligned_bytes != used_bytes) {
    size_t* words = (size_t*)p;
    words[aligned_words - 1] = 0; // set padding to zero
  }

  // Write header _after_ padding in case there's only one word!
  p->header = (Header){
      .tag = Tag_String,
      .size = (u32)aligned_words,
  };

  return p;
}

Custom* ctorCustom(u32 ctor, u32 n_children, void* children[]) {
  Custom* c = CAN_THROW(GC_malloc(sizeof(Custom) + n_children * sizeof(void*)));
  c->header = HEADER_CUSTOM(n_children);
  c->ctor = ctor;
  for (size_t i = 0; i < n_children; ++i) {
    c->values[i] = children[i];
  }
  return c;
}

Record* ctorRecord(FieldGroup* fg, u32 n_children, void* children[]) {
  Record* r = CAN_THROW(GC_malloc(sizeof(Record) + n_children * sizeof(void*)));
  r->header = HEADER_RECORD(n_children);
  r->fieldgroup = fg;
  for (size_t i = 0; i < n_children; ++i) {
    r->values[i] = children[i];
  }
  return r;
}
Closure* ctorClosure(
    u16 n_values, u16 max_values, void* (*evaluator)(void*[]), void* values[]) {
  Closure* c = CAN_THROW(GC_malloc(sizeof(Closure) + n_values * sizeof(void*)));
  c->header = HEADER_CLOSURE(n_values);
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
  u32 total_size = c->header.size * SIZE_UNIT;
  u32 before_pointers = sizeof(Header) + sizeof(u32);
  return (total_size - before_pointers) / sizeof(void*);
}

Cons Nil = {.header = HEADER_LIST, .head = NULL, .tail = NULL};
Custom Unit = {.header = HEADER_CUSTOM(0), .ctor = 0};
Custom True = {.header = HEADER_CUSTOM(0), .ctor = 1};
Custom False = {.header = HEADER_CUSTOM(0), .ctor = 0};
GcException GcFull = {.header = HEADER_GC_EXCEPTION};

void* pNil = &Nil;
void* pUnit = &Unit;
void* pTrue = &True;
void* pFalse = &False;
void* pGcFull = &GcFull;
