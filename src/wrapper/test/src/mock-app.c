#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#else
#define EMSCRIPTEN_KEEPALIVE
#include <stdio.h>
#endif
#include "../../../kernel/basics.h"
#include "../../../kernel/gc-internals.h"
#include "../../../kernel/gc.h"
#include "../../../kernel/types.h"
#include "../../../kernel/utils.h"

const FieldGroup address_firstName_lastName = {
    .size = 3,
    .fields = {123, 456, 789},
};
const FieldGroup firstName_lastName = {
    .size = 2,
    .fields = {456, 789},
};
const FieldGroup* appFieldGroups[] = {
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

ElmValue* rec_address_firstName_lastName;
ElmValue* rec_firstName_lastName;
size_t EMSCRIPTEN_KEEPALIVE get_rec_address_firstName_lastName() {
  return (size_t)rec_address_firstName_lastName;
}
size_t EMSCRIPTEN_KEEPALIVE get_rec_firstName_lastName() {
  return (size_t)rec_firstName_lastName;
}
void* init_records() {
  //                                             1 23 45 67 8  90
  const ElmString* address1 = NEW_ELM_STRING(9, "a\0d\0d\0r\0001");
  //                                                1 23 45 67 89 01 23 45 67 8  90
  const ElmString* firstName1 = NEW_ELM_STRING(19, "f\0i\0r\0s\0t\0N\0a\0m\0e\0001");
  //                                               1 23 45 67 89 01 23 45 6  78
  const ElmString* lastName1 = NEW_ELM_STRING(17, "l\0a\0s\0t\0N\0a\0m\0e\0001");
  //                                                1 23 45 67 89 01 23 45 67 8  90
  const ElmString* firstName2 = NEW_ELM_STRING(19, "f\0i\0r\0s\0t\0N\0a\0m\0e\0002");
  //                                               1 23 45 67 89 01 23 45 6  78
  const ElmString* lastName2 = NEW_ELM_STRING(17, "l\0a\0s\0t\0N\0a\0m\0e\0002");
  rec_address_firstName_lastName =
      ctor_address_firstName_lastName(address1, firstName1, lastName1);
  rec_firstName_lastName = ctor_firstName_lastName(firstName2, lastName2);
  return NULL;
}

int EMSCRIPTEN_KEEPALIVE main(int argc, char** argv) {
  const int exit_code = GC_init();

  GC_register_root(&rec_address_firstName_lastName);
  GC_register_root(&rec_firstName_lastName);
  init_records();

  return exit_code;
}
