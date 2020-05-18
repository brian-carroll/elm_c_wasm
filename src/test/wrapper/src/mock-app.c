#include <assert.h>
#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#else
#define EMSCRIPTEN_KEEPALIVE
#include <stdio.h>
#endif

#include "../../../kernel/gc-internals.h"
#include "../../../kernel/kernel.h"

/* ---------------------------------------------------------

          INT, FLOAT, CHAR

--------------------------------------------------------- */

static ElmInt test_int = {
    .header = HEADER_INT,
    .value = 1234567,
};
size_t EMSCRIPTEN_KEEPALIVE get_test_int() {
  return (size_t)&test_int;
}

static ElmFloat test_float = {
    .header = HEADER_FLOAT,
    .value = 3.14159265,
};
size_t EMSCRIPTEN_KEEPALIVE get_test_float() {
  return (size_t)&test_float;
}

ElmChar test_char16 = {
    .header = HEADER_CHAR,
    .words16[0] = 'A',
};
size_t EMSCRIPTEN_KEEPALIVE get_test_char16() {
  return (size_t)&test_char16;
}

ElmChar test_char32 = {
    .header = HEADER_CHAR,
    .words16[0] = 0xd83d,
    .words16[1] = 0xde4c,
};
size_t EMSCRIPTEN_KEEPALIVE get_test_char32() {
  return (size_t)&test_char32;
}

/* ---------------------------------------------------------

          STRINGS

--------------------------------------------------------- */

ElmString* address1;
ElmString* firstName1;
ElmString* lastName1;
ElmString* firstName2;
ElmString* lastName2;

void* init_strings() {
  //                                             1 23 45 67 8  90
  address1 = NEW_ELM_STRING(9, "a\0d\0d\0r\0001");
  //                                                1 23 45 67 89 01 23 45 67 8  90
  firstName1 = NEW_ELM_STRING(19, "f\0i\0r\0s\0t\0N\0a\0m\0e\0001");
  //                                               1 23 45 67 89 01 23 45 6  78
  lastName1 = NEW_ELM_STRING(17, "l\0a\0s\0t\0N\0a\0m\0e\0001");
  //                                                1 23 45 67 89 01 23 45 67 8  90
  firstName2 = NEW_ELM_STRING(19, "f\0i\0r\0s\0t\0N\0a\0m\0e\0002");
  //                                               1 23 45 67 89 01 23 45 6  78
  lastName2 = NEW_ELM_STRING(17, "l\0a\0s\0t\0N\0a\0m\0e\0002");
  return NULL;
}

size_t EMSCRIPTEN_KEEPALIVE get_test_string() {
  return (size_t)firstName1;
}

/* ---------------------------------------------------------

          LIST

--------------------------------------------------------- */
Cons* list0;
Cons* list1;

void* init_list() {
  list1 = NEW_CONS(lastName1, pNil);
  list0 = NEW_CONS(firstName1, list1);
  return NULL;
}

size_t EMSCRIPTEN_KEEPALIVE get_test_list() {
  return (size_t)list0;
}

/* ---------------------------------------------------------

          TUPLES

--------------------------------------------------------- */

static Tuple2 tuple2 = {
    .header = HEADER_TUPLE2,
    .a = &test_int,
    .b = &test_char16,
};

size_t EMSCRIPTEN_KEEPALIVE get_test_tuple2() {
  return (size_t)&tuple2;
}

static Tuple3 tuple3 = {
    .header = HEADER_TUPLE3,
    .a = &test_int,
    .b = &test_char16,
    .c = &test_float,
};

size_t EMSCRIPTEN_KEEPALIVE get_test_tuple3() {
  return (size_t)&tuple3;
}

/* ---------------------------------------------------------

          UNION/CUSTOM TYPES

--------------------------------------------------------- */

Custom nothing = {
    .header = HEADER_CUSTOM(0),
    .ctor = 0,
};

size_t EMSCRIPTEN_KEEPALIVE get_test_nothing() {
  return (size_t)&nothing;
}

Custom just_int = {
    .header = HEADER_CUSTOM(1),
    .ctor = 1,
    .values = {&test_int},
};

size_t EMSCRIPTEN_KEEPALIVE get_test_just_int() {
  return (size_t)&just_int;
}

/* ---------------------------------------------------------

          RECORDS

--------------------------------------------------------- */

FieldGroup address_firstName_lastName = {
    .size = 3,
    .fields = {0, 1, 2},
};
FieldGroup firstName_lastName = {
    .size = 2,
    .fields = {1, 2},
};
FieldGroup* fieldGroupsArray[] = {
    &address_firstName_lastName,
    &firstName_lastName,
    NULL,
};

void* ctor_address_firstName_lastName(void* address, void* firstName, void* lastName) {
  Record* r = GC_malloc(sizeof(Record) + 3 * sizeof(void*));
  r->header = HEADER_RECORD(3);
  r->fieldgroup = &address_firstName_lastName;
  r->values[0] = address;
  r->values[1] = firstName;
  r->values[2] = lastName;
  return r;
}

void* ctor_firstName_lastName(void* firstName, void* lastName) {
  Record* r = GC_malloc(sizeof(Record) + 2 * sizeof(void*));
  r->header = HEADER_RECORD(2);
  r->fieldgroup = &firstName_lastName;
  r->values[0] = firstName;
  r->values[1] = lastName;
  return r;
}

void* rec_address_firstName_lastName;
void* rec_firstName_lastName;
size_t EMSCRIPTEN_KEEPALIVE get_rec_address_firstName_lastName() {
  return (size_t)rec_address_firstName_lastName;
}
size_t EMSCRIPTEN_KEEPALIVE get_rec_firstName_lastName() {
  return (size_t)rec_firstName_lastName;
}
void* init_records() {
  rec_address_firstName_lastName =
      ctor_address_firstName_lastName(address1, firstName1, lastName1);
  rec_firstName_lastName = ctor_firstName_lastName(firstName2, lastName2);
  return NULL;
}

/* ---------------------------------------------------------

          EQUALITY

--------------------------------------------------------- */
void* test_equal_help(void* ptr1, void* ptr2) {
  return A2(&Utils_equal, ptr1, ptr2);
}
size_t EMSCRIPTEN_KEEPALIVE test_equal(size_t addr1, size_t addr2) {
  void* result = test_equal_help((void*)addr1, (void*)addr2);
  // For the test, better to just throw rather than catching and doing GC
  assert(result != pGcFull);
  return result == &True;
}

/* ---------------------------------------------------------

          CLOSURE

--------------------------------------------------------- */

static ElmInt one = {
    .header = HEADER_INT,
    .value = 1,
};

Closure* increment;
void* init_closure() {
  // partial application
  increment = A1(&Basics_add, &one);
  return NULL;
}
size_t EMSCRIPTEN_KEEPALIVE get_increment_callback() {
  return (size_t)increment;
}

/* ---------------------------------------------------------

          INITIALISE THE HEAP

--------------------------------------------------------- */

int EMSCRIPTEN_KEEPALIVE main(int argc, char** argv) {
  int exit_code = GC_init();
  if (exit_code) return exit_code;

  GC_register_root(&rec_address_firstName_lastName);
  GC_register_root(&rec_firstName_lastName);
  init_strings();
  init_list();
  init_records();
  init_closure();

  Wrapper_registerFieldGroups(fieldGroupsArray);

  return exit_code;
}
