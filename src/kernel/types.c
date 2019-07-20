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
// Last byte of padding is a number telling the size of the padding itself,
// so that we can find the exact byte length of the String.
// Like OCaml,
// https://v1.realworldocaml.org/v1/en/html/memory-representation-of-values.html#string-values

// see also NEW_ELM_STRING in header file
ElmString* ctorElmString(size_t payload_bytes, char* str) {
  size_t used_bytes = sizeof(Header) + payload_bytes + 1;  // 1 byte for padding size
  size_t aligned_words = (used_bytes + SIZE_UNIT - 1) / SIZE_UNIT;  // ceil
  size_t aligned_bytes = aligned_words * SIZE_UNIT;

  ElmString* p = CAN_THROW(GC_malloc(aligned_bytes));

  // Insert zero padding, and last byte indicating size of padding
  // Store padding size minus 1, so that if there's only 1 byte of padding,
  // it's a zero. (Zero-terminated strings are handy for debug etc.)
  size_t* words = (size_t*)p;  // the ElmString as an array of words
  size_t padding_bytes = aligned_bytes - (used_bytes - 1);
  size_t last_byte_value = padding_bytes - 1;
  words[aligned_words - 1] = last_byte_value << (SIZE_UNIT - 1) * 8;

  // Write header _after_ padding to avoid overwriting if there's only one word
  p->header = (Header){.tag = Tag_String, .size = (u32)aligned_words};

  // Copy the string body if provided
  if (str != NULL) {
    // Header is 32 bits => ElmString body is 32-bit aligned
    size_t payload_ints = payload_bytes / 4;
    u32* str32 = (u32*)str;
    u32* i_from = str32;
    u32* i_to = (u32*)p->bytes;
    while (i_from < str32 + payload_ints) {
      *i_to++ = *i_from++;
    }

    // Last few bytes (<4)
    char* c_from = (char*)i_from;
    char* c_to = (char*)i_to;
    while (c_from < str + payload_bytes) {
      *c_to++ = *c_from++;
    }
  }

  return p;
}

u32 custom_params(Custom* c) {
  u32 total_size = c->header.size * SIZE_UNIT;
  u32 before_pointers = sizeof(Header) + sizeof(u32);
  return (total_size - before_pointers) / sizeof(void*);
}

void Types_init() {
  Nil = (Cons){.header = HEADER_LIST, .head = NULL, .tail = NULL};
  pNil = &Nil;

  Unit = (Custom){.header = HEADER_CUSTOM(0), .ctor = 0};
  pUnit = &Unit;

  True = (Custom){.header = HEADER_CUSTOM(0), .ctor = 1};
  pTrue = &True;

  False = (Custom){.header = HEADER_CUSTOM(0), .ctor = 0};
  pFalse = &False;

  GcFull = (GcException){.header = HEADER_GC_EXCEPTION};
  pGcFull = &GcFull;
}
