#include "../kernel/core/core.h"
#include "test.h"

void* expect_string(char* unit_description, char* expected_c_str, ElmString16* actual);


void* test_Debug_toString() {
  ElmInt* int1 = NEW_ELM_INT(1);
  ElmInt* int2 = NEW_ELM_INT(2);
  ElmInt* int3 = NEW_ELM_INT(3);

  void* list_items[3] = { int1, int2, int3 };
  Cons* list = List_create(3, list_items);

  // FieldGroup fg = {
  //   .header = HEADER_FIELDGROUP(1),
  //   .size = 1,
  //   .fields = {},
  // };

  expect_string("Int", "123", A1(&Debug_toString, NEW_ELM_INT(123)));
  expect_string("Float", "3.14", A1(&Debug_toString, NEW_ELM_FLOAT(3.14)));
  expect_string("Char", "'X'", A1(&Debug_toString, NEW_ELM_CHAR('X')));
  expect_string("String", "\"hello\"", A1(&Debug_toString, create_string("hello")));
  expect_string("List", "[1,2,3]", A1(&Debug_toString, list));
  expect_string("Tuple2", "(1,2)", A1(&Debug_toString, NEW_TUPLE2(int1, int2)));
  expect_string("Tuple3", "(1,2,3)", A1(&Debug_toString, NEW_TUPLE3(int1, int2, int3)));
  expect_string("Custom", "Just 3", A1(&Debug_toString, A1(&g_elm_core_Maybe_Just, int3)));
  // mu_expect_equal("Record", "123", A1(&Debug_toString, NULL));

  return NULL;
}

char* debug_test() {
  if (verbose) {
    printf("\n\n\n");
    printf("####################################################\n");
    printf("\n");
    printf("Debug\n");
    printf("------\n");
  }

  mu_run_test(test_Debug_toString);

  return NULL;
}
