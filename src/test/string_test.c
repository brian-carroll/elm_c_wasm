#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "../kernel/kernel.h"
#include "./test.h"

size_t code_units(ElmString16* s);
ptrdiff_t find_reverse(u16* sub, u16* str, size_t sub_len, ptrdiff_t str_idx);

// ---------------------------------------------------------
//
//          STRING TESTING UTILITIES
//
// ---------------------------------------------------------

char* test_description;
void* test_heap_ptr;

void describe(char* description, void* (*test)()) {
  tests_run++;
  test_description = description;
  test_heap_ptr = GC_malloc(0);
  if (verbose) {
    printf("\n%s\n", description);
  }
  if (test() == pGcFull) {
    fprintf(stderr, "Heap overflow in test \"%s\"\n", description);
    print_heap();
  }
}

ElmString16* create_string(char* c_string) {
  size_t c_len = (size_t)strlen(c_string);
  size_t bytes_utf16 = c_len * 2;
  ElmString16* s = NEW_ELM_STRING(bytes_utf16, NULL);
  for (size_t i = 0; i < c_len; i++) {
    s->words16[i] = (u16)c_string[i];
  }
  return s;
}

void print_elm_string(ElmString16* str) {
  size_t len = code_units(str);
  printf("\"");
  for (size_t i = 0; i < len; i++) {
    u16 c = str->words16[i];
    printf("%c", (char)c);
  }
  printf("\"");
}

void* expect_equal(char* expect_description, void* left, void* right) {
  bool ok = A2(&Utils_equal, left, right) == &True;
  if (!ok) {
    if (!verbose) {
      printf("\n%s\n", test_description);
    }
    printf("FAIL: %s\n", expect_description);
    printf("Left: %p\n", left);
    printf("Right: %p\n", right);
    print_heap_range(test_heap_ptr, GC_malloc(0));
    printf("\n");
    tests_failed++;
  } else if (verbose) {
    printf("PASS: %s\n", expect_description);
  }
  assertions_made++;
  return NULL;
}

void* expect_string(char* call_expr, char* expected_c_str, ElmString16* actual) {
  ElmString16* expected = create_string(expected_c_str);
  bool ok = A2(&Utils_equal, actual, expected) == &True;
  if (!ok) {
    if (!verbose) {
      printf("\n%s\n", test_description);
    }
    printf("\n");
    printf("%s\n", call_expr);
    printf("Expected %s\n", expected_c_str);
    print_value_full(expected);
    printf("\n");
    printf("Actual ");
    print_elm_string(actual);
    printf("\n");
    print_value_full(actual);
    printf("\n");
    tests_failed++;
  } else if (verbose) {
    printf("PASS: %s == \"%s\"\n", call_expr, expected_c_str);
  }
  assertions_made++;
  return NULL;
}

// ---------------------------------------------------------
//
//          STRING HELPER TESTS
//
// ---------------------------------------------------------

char mu_message[1024];

void* test_code_units() {
  mu_assert("Expect code_units=0 for \"\"", code_units(NEW_ELM_STRING16(0)) == 0);
  mu_assert("Expect code_units=1 for \"1\"", code_units(create_string("1")) == 1);

  char buf[21];
  for (size_t i = 2; i <= 20; i++) {
    size_t j = 0;
    for (; j < i; j++) {
      buf[j] = '.';
    }
    buf[j] = 0;
    sprintf(mu_message, "Expect code_units=%zu for \"%s\"", i, buf);
    mu_assert(mu_message, code_units(create_string(buf)) == i);
  }
  return NULL;
}

void* test_find_reverse() {
  ElmString16* sep;
  ElmString16* str = create_string("home/jill/Desktop");
  size_t str_len = code_units(str);
  ptrdiff_t result;

  if (verbose) {
    printf("\ntest_find_reverse\n");
  }

  sep = create_string("z");
  result = find_reverse(sep->words16, str->words16, code_units(sep), str_len - 1);
  mu_expect_equal("should return -1 for no match", result, -1);

  sep = create_string("go");
  result = find_reverse(sep->words16, str->words16, code_units(sep), str_len - 1);
  mu_expect_equal("should return -1 for partial match", result, -1);

  sep = create_string("/");
  result = find_reverse(sep->words16, str->words16, code_units(sep), str_len - 1);
  mu_expect_equal("should find last match", result, 9);

  sep = create_string("");
  result = find_reverse(sep->words16, str->words16, code_units(sep), str_len - 1);
  mu_expect_equal("should find last char, given ''", result, str_len - 1);

  sep = create_string("");
  result = find_reverse(sep->words16, str->words16, code_units(sep), str_len - 2);
  mu_expect_equal("should find 2nd last char, given '' again", result, str_len - 2);

  sep = create_string("h");
  result = find_reverse(sep->words16, str->words16, code_units(sep), str_len - 1);
  mu_expect_equal("should find a single char at start", result, 0);

  sep = create_string("ji");
  result = find_reverse(sep->words16, str->words16, code_units(sep), str_len - 1);
  mu_expect_equal("should find multiple chars in middle (no retry)", result, 5);

  sep = create_string("jil");
  result = find_reverse(sep->words16, str->words16, code_units(sep), str_len - 1);
  mu_expect_equal("should find multiple chars in middle (immediate retry)", result, 5);

  sep = create_string("ho");
  result = find_reverse(sep->words16, str->words16, code_units(sep), str_len - 1);
  mu_expect_equal("should find multiple chars at start (retry)", result, 0);

  return NULL;
}

// ---------------------------------------------------------
//
//          STRING TESTS
//
// ---------------------------------------------------------

void* test_String_uncons() {
  ElmString16* abc = create_string("abc");
  ElmString16* bc = create_string("bc");
  ElmString16* a_str = create_string("a");
  ElmString16* empty = create_string("");
  ElmChar* a_char = NEW_ELM_CHAR((u32)'a');

  expect_equal("uncons \"abc\" == Just ('a',\"bc\")",
      A1(&String_uncons, abc),
      A1(&g_elm_core_Maybe_Just, NEW_TUPLE2(a_char, bc)));

  expect_equal(
      "uncons \"\" == Nothing", A1(&String_uncons, empty), &g_elm_core_Maybe_Nothing);

  expect_equal("uncons \"a\" == Just ('a',\"\")",
      A1(&String_uncons, a_str),
      A1(&g_elm_core_Maybe_Just, NEW_TUPLE2(a_char, empty)));

  return NULL;
}

void* test_String_append() {
  ElmString16* hello = create_string("hello");
  ElmString16* world = create_string(" world");
  ElmString16* empty = create_string("");
  expect_string(
      "append \"hello\" \" world\"", "hello world", A2(&String_append, hello, world));
  expect_string("append \"hello\" \"\"", "hello", A2(&String_append, hello, empty));
  expect_string("append \"\" \"hello\"", "hello", A2(&String_append, empty, hello));
  expect_string("append \"\" \"\"", "", A2(&String_append, empty, empty));
  return NULL;
}

void* test_String_split() {
  expect_equal("split \"/\" \"home/steve/Desktop\" == [\"home\",\"steve\",\"Desktop\"]",
      A2(&String_split, create_string("/"), create_string("home/steve/Desktop")),
      List_create(3,
          (void*[]){
              create_string("home"),
              create_string("steve"),
              create_string("Desktop"),
          }));

  ElmString16* abc = create_string("abc");
  ElmString16* ab = create_string("ab");
  ElmString16* empty = create_string("");
  Cons* list_ab = NEW_CONS(ab, &Nil);

  expect_equal("split \"abc\" \"ab\" == [\"ab\"]  # separator longer than string",
      A2(&String_split, abc, ab),
      list_ab);

  expect_equal("split \"\" \"ab\" == [\"a\",\"b\"]  # split on empty string",
      A2(&String_split, empty, ab),
      List_create(2,
          (void*[]){
              create_string("a"),
              create_string("b"),
          }));

  expect_equal("split \"ab\" \"ab\" == [\"\",\"\"]",
      A2(&String_split, ab, ab),
      List_create(2,
          (void*[]){
              empty,
              empty,
          }));

  expect_equal(
      "split \"ab\" \"\" == [\"\"]", A2(&String_split, ab, empty), NEW_CONS(empty, &Nil));

  return NULL;
}

void* test_String_join() {
  ElmString16* slash = create_string("/");
  ElmString16* empty = create_string("");
  expect_string("join \"/\" [\"home\",\"steve\",\"Desktop\"]",
      "home/steve/Desktop",
      A2(&String_join,
          slash,
          List_create(3,
              (void*[]){
                  create_string("home"),
                  create_string("steve"),
                  create_string("Desktop"),
              })));
  expect_string("join \"/\" []", "", A2(&String_join, slash, &Nil));
  expect_string("join \"\" [\"/\"]", "/", A2(&String_join, empty, NEW_CONS(slash, &Nil)));
  expect_string("join \"/\" [\"\"]", "", A2(&String_join, slash, NEW_CONS(empty, &Nil)));
  expect_string("join \"\" [\"\"]", "", A2(&String_join, empty, NEW_CONS(empty, &Nil)));
  expect_string("join \"\" []", "", A2(&String_join, empty, &Nil));
  return NULL;
}

void* test_String_slice() {
  ElmString16* abc = create_string("abc");

  expect_string(
      "slice 1 2 \"abc\"", "b", A3(&String_slice, NEW_ELM_INT(1), NEW_ELM_INT(2), abc));
  expect_string(
      "slice 1 -1 \"abc\"", "b", A3(&String_slice, NEW_ELM_INT(1), NEW_ELM_INT(-1), abc));
  expect_string(
      "slice -2 2 \"abc\"", "b", A3(&String_slice, NEW_ELM_INT(-2), NEW_ELM_INT(2), abc));
  expect_string("slice -2 -1 \"abc\"",
      "b",
      A3(&String_slice, NEW_ELM_INT(-2), NEW_ELM_INT(-1), abc));

  expect_string(
      "slice 2 1 \"abc\"", "", A3(&String_slice, NEW_ELM_INT(2), NEW_ELM_INT(1), abc));

  expect_string(
      "slice 0 3 \"abc\"", "abc", A3(&String_slice, NEW_ELM_INT(0), NEW_ELM_INT(3), abc));
  expect_string(
      "slice -3 0 \"abc\"", "", A3(&String_slice, NEW_ELM_INT(-3), NEW_ELM_INT(0), abc));
  expect_string(
      "slice  0 -3 \"abc\"", "", A3(&String_slice, NEW_ELM_INT(0), NEW_ELM_INT(-3), abc));

  expect_string("slice -100 100 \"abc\"",
      "abc",
      A3(&String_slice, NEW_ELM_INT(-100), NEW_ELM_INT(100), abc));
  expect_string("slice -100 100 \"\"",
      "",
      A3(&String_slice, NEW_ELM_INT(-100), NEW_ELM_INT(100), create_string("")));

  return NULL;
}

void* test_String_indexes() {
  expect_equal("indexes \"/\" \"home/steve/Desktop\" == [4,10]",
      A2(&String_indexes, create_string("/"), create_string("home/steve/Desktop")),
      List_create(2,
          (void*[]){
              NEW_ELM_INT(4),
              NEW_ELM_INT(10),
          }));

  ElmString16* abc = create_string("abc");
  ElmString16* ab = create_string("ab");
  ElmString16* empty = create_string("");

  expect_equal("indexes \"abc\" \"ab\" == []  # substring longer than string",
      A2(&String_indexes, abc, ab),
      &Nil);

  expect_equal("indexes \"\" \"ab\" == []  # empty substring",
      A2(&String_indexes, empty, ab),
      &Nil);

  expect_equal("indexes \"ab\" \"ab\" == [0]",
      A2(&String_indexes, ab, ab),
      NEW_CONS(NEW_ELM_INT(0), &Nil));

  expect_equal("indexes \"ab\" \"\" == []", A2(&String_indexes, ab, empty), &Nil);

  return NULL;
}

void* test_String_trim() {
  // TODO
  // "hello" == trim " \n hello \t \r\n"
  // "" == trim " \n \t \r\n"
  // "" == trim ""
  return NULL;
}

void* test_String_trimLeft() {
  // TODO
  // "hello \t \r\n" == trimLeft " \n hello \t \r\n"
  // "" == trimLeft " \n \t \r\n"
  // "" == trimLeft ""
  return NULL;
}

void* test_String_trimRight() {
  // TODO
  // " \n hello" == trimRight " \n hello \t \r\n"
  // "" == trimRight " \n \t \r\n"
  // "" == trimRight ""
  return NULL;
}

void* test_String_all() {
  // TODO
  // isX = (==) 'X'
  // all isX "XXXXX" == True
  // all isX "XXXX " == False
  // all isX "" == True
  return NULL;
}

void* test_String_contains() {
  // TODO
  // contains "ab" "abc" == True
  // contains "abc" "ab" == False
  // contains "abc" "" == False
  // contains "" "abc" == True
  return NULL;
}

void* test_String_startsWith() {
  // TODO
  // startsWith "ab" "abc" == True
  // startsWith "bc" "abc" == False
  // startsWith "abc" "ab" == False
  // startsWith "abc" "" == False
  // startsWith "" "abc" == True
  return NULL;
}

void* test_String_endsWith() {
  // TODO
  // endsWith "ab" "abc" == False
  // endsWith "bc" "abc" == True
  // endsWith "abc" "ab" == False
  // endsWith "abc" "" == False
  // endsWith "" "abc" == True
  return NULL;
}

void* test_String_fromNumber() {
  expect_string("fromNumber 2147483647",
      "2147483647",
      A1(&String_fromNumber, NEW_ELM_INT(2147483647)));
  expect_string("fromNumber -2147483648",
      "-2147483648",
      A1(&String_fromNumber, NEW_ELM_INT(-2147483648)));
  expect_string("fromNumber -3.141592653589793",
      "-3.141592653589793",
      A1(&String_fromNumber, NEW_ELM_FLOAT(-3.141592653589793)));
  expect_string("fromNumber -3141592653589793",
      "-3141592653589793",
      A1(&String_fromNumber, NEW_ELM_FLOAT(-3141592653589793)));
  return NULL;
}

void* test_String_toInt() {
  expect_equal("toInt \"2147483647\" == Just 2147483647",
      A1(&String_toInt, create_string("2147483647")),
      A1(&g_elm_core_Maybe_Just, NEW_ELM_INT(2147483647)));

  expect_equal("toInt \"-2147483648\" == Just -2147483648",
      A1(&String_toInt, create_string("-2147483648")),
      A1(&g_elm_core_Maybe_Just, NEW_ELM_INT(-2147483648)));

  expect_equal("toInt \"+10\" == Just 10",
      A1(&String_toInt, create_string("+10")),
      A1(&g_elm_core_Maybe_Just, NEW_ELM_INT(10)));

  expect_equal("toInt \"0\" == Just 0",
      A1(&String_toInt, create_string("0")),
      A1(&g_elm_core_Maybe_Just, NEW_ELM_INT(0)));

  expect_equal("toInt \"+0\" == Just 0",
      A1(&String_toInt, create_string("+0")),
      A1(&g_elm_core_Maybe_Just, NEW_ELM_INT(0)));

  expect_equal("toInt \"-0\" == Just 0",
      A1(&String_toInt, create_string("-0")),
      A1(&g_elm_core_Maybe_Just, NEW_ELM_INT(0)));

  expect_equal("toInt \"hello\" == Nothing",
      A1(&String_toInt, create_string("hello")),
      &g_elm_core_Maybe_Nothing);

  expect_equal("toInt \"\" == Nothing",
      A1(&String_toInt, create_string("")),
      &g_elm_core_Maybe_Nothing);

  return NULL;
}

// ---------------------------------------------------------
//
//          RUN ALL STRING TESTS
//
// ---------------------------------------------------------

char* string_test() {
  if (verbose) {
    printf("\n\n\n");
    printf("####################################################\n");
    printf("\n");
    printf("String\n");
    printf("------\n");
  }

  describe("test_String_uncons", &test_String_uncons);
  describe("test_String_append", &test_String_append);
  mu_run_test(test_code_units);
  mu_run_test(test_find_reverse);
  describe("test_String_split", &test_String_split);
  describe("test_String_join", &test_String_join);
  describe("test_String_slice", &test_String_slice);
  // TODO: describe("test_String_trim", &test_String_trim);
  // TODO: describe("test_String_trimLeft", &test_String_trimLeft);
  // TODO: describe("test_String_trimRight", &test_String_trimRight);
  // TODO: describe("test_String_all", &test_String_all);
  // TODO: describe("test_String_contains", &test_String_contains);
  // TODO: describe("test_String_startsWith", &test_String_startsWith);
  // TODO: describe("test_String_endsWith", &test_String_endsWith);
  describe("test_String_indexes", &test_String_indexes);
  describe("test_String_fromNumber", &test_String_fromNumber);
  describe("test_String_toInt", &test_String_toInt);

  return NULL;
}
