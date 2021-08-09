#include "../kernel/core/core.h"
#include "test.h"

void expect_string(char* unit_description, char* expected_c_str, ElmString* actual);


void test_Debug_toString() {
  ElmInt* int1 = newElmInt(1);
  ElmInt* int2 = newElmInt(2);
  ElmInt* int3 = newElmInt(3);

  void* list_items[3] = {int1, int2, int3};
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
}


void* eval_createFragmentation(void* args[]) {
  Cons* list = args[0];
  ElmInt* gap = args[1];
  for (i32 i = 0;; i++) {
    GC_allocate(false, gap->value);
    Cons* newList = newCons(newElmInt(i), list);
    if (newList < list) {
      return newList;
    }
    list = newList;
    GC_stack_tailcall(1, list);
  }
}

char test_sequence[] = "abcdefghijklmnopqrstuvwxyz";

void* eval_buildString(void* args[]) {
  ElmInt* iter = args[0];

  StringBuilder sb;
  StringBuilder_init(&sb);

  i32 iterations = iter->value;
  for (int i = 0; i < iterations; i++) {
    int idx = i % (sizeof(test_sequence) - 1);
    StringBuilder_writeChar(&sb, test_sequence[idx]);
  }

  ElmString* str = StringBuilder_toString(&sb);
  return str;
}


void test_StringBuilder() {
  GC_init();

  Closure* createFragmentation =
      newClosure(2, 2, eval_createFragmentation, (void*[]){&Nil, newElmInt(162)});
  void* liveList = GC_execute(createFragmentation);
  GC_register_root(&liveList);

  Closure* buildString =
      newClosure(1, 1, eval_buildString, (void*[]){newElmInt(6 * 164 * sizeof(void*))});
  ElmString* longString = GC_execute(buildString);

  int errors = 0;
  for (int i = 0; i < code_units(longString); i++) {
    int idx = i % (sizeof(test_sequence) - 1);
    if (longString->words16[i] != test_sequence[idx]) {
      errors++;
      if (verbose) {
        safe_printf("wrong character at %p: expected %x, got %x\n",
            &longString->words16[i],
            test_sequence[idx],
            longString->words16[i]);
      }
    }
  }
  mu_expect_equal("should build a long string, in a fragmented heap, with no errors", errors, 0);
}


void debug_test() {
  if (verbose) {
    safe_printf("\n\n\n");
    safe_printf("####################################################\n");
    safe_printf("\n");
    safe_printf("Debug\n");
    safe_printf("------\n");
  }

  mu_run_test(test_Debug_toString);
  mu_run_test(test_StringBuilder);
}
