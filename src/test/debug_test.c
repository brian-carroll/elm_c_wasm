#include "../kernel/core/core.h"
#include "test.h"

void* expect_string(char* unit_description, char* expected_c_str, ElmString16* actual);


void* test_Debug_toString() {
  ElmInt* int1 = newElmInt(1);
  ElmInt* int2 = newElmInt(2);
  ElmInt* int3 = newElmInt(3);

  void* list_items[3] = { int1, int2, int3 };
  Cons* list = List_create(3, list_items);

  // FieldGroup fg = {
  //   .header = HEADER_FIELDGROUP(1),
  //   .size = 1,
  //   .fields = {},
  // };

  expect_string("Int", "123", A1(&Debug_toString, newElmInt(123)));
  expect_string("Float", "3.14", A1(&Debug_toString, newElmFloat(3.14)));
  expect_string("Char", "'X'", A1(&Debug_toString, newElmChar('X')));
  expect_string("String", "\"hello\"", A1(&Debug_toString, create_string("hello")));
  expect_string("List", "[1,2,3]", A1(&Debug_toString, list));
  expect_string("Tuple2", "(1,2)", A1(&Debug_toString, newTuple2(int1, int2)));
  expect_string("Tuple3", "(1,2,3)", A1(&Debug_toString, newTuple3(int1, int2, int3)));
  expect_string("Custom", "Just 3", A1(&Debug_toString, A1(&g_elm_core_Maybe_Just, int3)));
  // mu_expect_equal("Record", "123", A1(&Debug_toString, NULL));

  return NULL;
}

char* debug_test() {
  if (verbose) {
    safe_printf("\n\n\n");
    safe_printf("####################################################\n");
    safe_printf("\n");
    safe_printf("Debug\n");
    safe_printf("------\n");
  }

  mu_run_test(test_Debug_toString);

  return NULL;
}
