#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "../kernel/core/core.h"
#include "test.h"

size_t code_units(ElmString* s);
ptrdiff_t find_reverse(u16* sub, u16* str, size_t sub_len, ptrdiff_t str_idx);
ptrdiff_t find_forward(u16* sub, u16* str, size_t sub_len, size_t str_len);
bool is_whitespace(u16 c);

// ---------------------------------------------------------
//
//          STRING TESTING UTILITIES
//
// ---------------------------------------------------------

void print_elm_string(ElmString* str) {
  size_t len = code_units(str);
  safe_printf("\"");
  for (size_t i = 0; i < len; i++) {
    u16 c = str->words16[i];
    safe_printf("%c", (char)c);
  }
  safe_printf("\"");
}

void expect_string(char* unit_description, char* expected_c_str, ElmString* actual) {
  ElmString* expected = create_string(expected_c_str);
  bool ok = A2(&Utils_equal, actual, expected) == &True;
  if (!ok) {
    if (!verbose) {
      safe_printf("\n%s\n", current_describe_string);
    }
    safe_printf("\n");
    safe_printf("%s\n", unit_description);
    safe_printf("Expected %s\n", expected_c_str);
    print_value_full(expected);
    safe_printf("\n");
    safe_printf("Actual ");
    print_elm_string(actual);
    safe_printf("\n");
    print_value_full(actual);
    safe_printf("\n");
    tests_failed++;
  } else if (verbose) {
    safe_printf("PASS: %s == \"%s\"\n", unit_description, expected_c_str);
  }
  assertions_made++;
}

// ---------------------------------------------------------
//
//          STRING HELPER TESTS
//
// ---------------------------------------------------------

char mu_message[1024];

void test_code_units() {
  mu_assert("Expect code_units=0 for \"\"", code_units(newElmString(0)) == 0);
  mu_assert("Expect code_units=1 for \"1\"", code_units(create_string("1")) == 1);

  char buf[21];
  for (size_t i = 2; i <= 20; i++) {
    size_t j = 0;
    for (; j < i; j++) {
      buf[j] = '.';
    }
    buf[j] = 0;
    stbsp_sprintf(mu_message, "Expect code_units=%zu for \"%s\"", i, buf);
    mu_assert(mu_message, code_units(create_string(buf)) == i);
  }
}

void test_find_reverse() {
  //                                01234567890123456
  ElmString* str = create_string("home/jill/Desktop");
  ElmString* sub;
  size_t str_len = code_units(str);
  ptrdiff_t result;

  if (verbose) {
    safe_printf("\ntest_find_reverse\n");
  }

  sub = create_string("z");
  result = find_reverse(sub->words16, str->words16, code_units(sub), str_len - 1);
  mu_expect_equal("should return -1 for no match", result, -1);

  sub = create_string("go");
  result = find_reverse(sub->words16, str->words16, code_units(sub), str_len - 1);
  mu_expect_equal("should return -1 for partial match", result, -1);

  sub = create_string("/");
  result = find_reverse(sub->words16, str->words16, code_units(sub), str_len - 1);
  mu_expect_equal("should find last match", result, 9);

  sub = create_string("");
  result = find_reverse(sub->words16, str->words16, code_units(sub), str_len - 1);
  mu_expect_equal("should find last char, given ''", result, str_len - 1);

  sub = create_string("");
  result = find_reverse(sub->words16, str->words16, code_units(sub), str_len - 2);
  mu_expect_equal("should find 2nd last char, given '' again", result, str_len - 2);

  sub = create_string("h");
  result = find_reverse(sub->words16, str->words16, code_units(sub), str_len - 1);
  mu_expect_equal("should find a single char at start", result, 0);

  sub = create_string("ji");
  result = find_reverse(sub->words16, str->words16, code_units(sub), str_len - 1);
  mu_expect_equal("should find multiple chars in middle (no retry)", result, 5);

  sub = create_string("jil");
  result = find_reverse(sub->words16, str->words16, code_units(sub), str_len - 1);
  mu_expect_equal("should find multiple chars in middle (immediate retry)", result, 5);

  sub = create_string("ho");
  result = find_reverse(sub->words16, str->words16, code_units(sub), str_len - 1);
  mu_expect_equal("should find multiple chars at start (retry)", result, 0);
}

void test_find_forward() {
  //                                01234567890123456
  ElmString* str = create_string("home/jill/Desktop");
  ElmString* sub;
  size_t str_len = code_units(str);
  ptrdiff_t result;

  if (verbose) {
    safe_printf("\ntest_find_forward\n");
  }

  sub = create_string("z");
  result = find_forward(sub->words16, str->words16, code_units(sub), str_len);
  mu_expect_equal("should return -1 for no match", result, -1);

  sub = create_string("my");
  result = find_forward(sub->words16, str->words16, code_units(sub), str_len);
  mu_expect_equal("should return -1 for partial match", result, -1);

  sub = create_string("/");
  result = find_forward(sub->words16, str->words16, code_units(sub), str_len);
  mu_expect_equal("should find first match", result, 4);

  sub = create_string("");
  result = find_forward(sub->words16, str->words16, code_units(sub), str_len);
  mu_expect_equal("should find first char, given ''", result, 0);

  sub = create_string("");
  result = find_forward(&sub->words16[1], str->words16, code_units(sub), str_len - 1);
  mu_expect_equal("should find 2nd char, given '' again", result, 0);

  sub = create_string("p");
  result = find_forward(sub->words16, str->words16, code_units(sub), str_len);
  mu_expect_equal("should find a single char at end", result, str_len - 1);

  sub = create_string("De");
  result = find_forward(sub->words16, str->words16, code_units(sub), str_len);
  mu_expect_equal("should find multiple chars in middle (no retry)", result, 10);

  sub = create_string("l/");
  result = find_forward(sub->words16, str->words16, code_units(sub), str_len);
  mu_expect_equal("should find multiple chars in middle (immediate retry)", result, 8);

  sub = create_string("op");
  result = find_forward(sub->words16, str->words16, code_units(sub), str_len);
  mu_expect_equal("should find multiple chars at end (retry)", result, 15);
}

u16 all_whitespace_chars[] = {
    // https://www.compart.com/en/unicode/category/Zs
    0x0020,
    0x00A0,
    0x1680,
    0x2000,
    0x2001,
    0x2002,
    0x2003,
    0x2004,
    0x2005,
    0x2006,
    0x2007,
    0x2008,
    0x2009,
    0x200A,
    0x202F,
    0x205F,
    0x3000,

    // https://tc39.es/ecma262/#prod-WhiteSpace
    0x0009,
    0x000B,
    0x000C,
    0x0020,
    0x00A0,
    0xFEFF,

    // https://tc39.es/ecma262/#prod-LineTerminator
    0x000A,
    0x000D,
    0x2028,
    0x2029,
};
const size_t n_whitespace_chars = sizeof(all_whitespace_chars) / sizeof(u16);
void test_is_whitespace() {
  u32 incorrect_results = 0;
  for (u32 code = 0; code <= 0xffff; code++) {
    u16 c = (u16)code;
    bool actual = is_whitespace(c);
    bool expected = false;
    for (size_t i = 0; i < n_whitespace_chars; i++) {
      if (all_whitespace_chars[i] == c) {
        expected = true;
        break;
      }
    }
    if (actual != expected) {
      incorrect_results++;
      safe_printf("FAIL: is_whitespace(0x%x) should be %x\n", c, expected);
    }
  }
  mu_assert("is_whitespace should be correct for all code units", incorrect_results == 0);
}

// ---------------------------------------------------------
//
//          STRING TESTS
//
// ---------------------------------------------------------

void test_String_uncons() {
  ElmString* abc = create_string("abc");
  ElmString* bc = create_string("bc");
  ElmString* a_str = create_string("a");
  ElmString* empty = create_string("");
  ElmChar* a_char = newElmChar((u32)'a');

  expect_equal("uncons \"abc\" == Just ('a',\"bc\")",
      A1(&String_uncons, abc),
      A1(&g_elm_core_Maybe_Just, newTuple2(a_char, bc)));

  expect_equal(
      "uncons \"\" == Nothing", A1(&String_uncons, empty), &g_elm_core_Maybe_Nothing);

  expect_equal("uncons \"a\" == Just ('a',\"\")",
      A1(&String_uncons, a_str),
      A1(&g_elm_core_Maybe_Just, newTuple2(a_char, empty)));
}

void test_String_append() {
  ElmString* hello = create_string("hello");
  ElmString* world = create_string(" world");
  ElmString* empty = create_string("");
  expect_string(
      "append \"hello\" \" world\"", "hello world", A2(&String_append, hello, world));
  expect_string("append \"hello\" \"\"", "hello", A2(&String_append, hello, empty));
  expect_string("append \"\" \"hello\"", "hello", A2(&String_append, empty, hello));
  expect_string("append \"\" \"\"", "", A2(&String_append, empty, empty));
}

void test_String_split() {
  expect_equal("split \"/\" \"home/steve/Desktop\" == [\"home\",\"steve\",\"Desktop\"]",
      A2(&String_split, create_string("/"), create_string("home/steve/Desktop")),
      List_create(3,
          (void*[]){
              create_string("home"),
              create_string("steve"),
              create_string("Desktop"),
          }));

  ElmString* abc = create_string("abc");
  ElmString* ab = create_string("ab");
  ElmString* empty = create_string("");
  Cons* list_ab = newCons(ab, &Nil);

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
      "split \"ab\" \"\" == [\"\"]", A2(&String_split, ab, empty), newCons(empty, &Nil));
}

void test_String_join() {
  ElmString* slash = create_string("/");
  ElmString* empty = create_string("");
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
  expect_string("join \"\" [\"/\"]", "/", A2(&String_join, empty, newCons(slash, &Nil)));
  expect_string("join \"/\" [\"\"]", "", A2(&String_join, slash, newCons(empty, &Nil)));
  expect_string("join \"\" [\"\"]", "", A2(&String_join, empty, newCons(empty, &Nil)));
  expect_string("join \"\" []", "", A2(&String_join, empty, &Nil));
}

void test_String_slice() {
  ElmString* abc = create_string("abc");

  expect_string(
      "slice 1 2 \"abc\"", "b", A3(&String_slice, newElmInt(1), newElmInt(2), abc));
  expect_string(
      "slice 1 -1 \"abc\"", "b", A3(&String_slice, newElmInt(1), newElmInt(-1), abc));
  expect_string(
      "slice -2 2 \"abc\"", "b", A3(&String_slice, newElmInt(-2), newElmInt(2), abc));
  expect_string("slice -2 -1 \"abc\"",
      "b",
      A3(&String_slice, newElmInt(-2), newElmInt(-1), abc));

  expect_string(
      "slice 2 1 \"abc\"", "", A3(&String_slice, newElmInt(2), newElmInt(1), abc));

  expect_string(
      "slice 0 3 \"abc\"", "abc", A3(&String_slice, newElmInt(0), newElmInt(3), abc));
  expect_string(
      "slice -3 0 \"abc\"", "", A3(&String_slice, newElmInt(-3), newElmInt(0), abc));
  expect_string(
      "slice  0 -3 \"abc\"", "", A3(&String_slice, newElmInt(0), newElmInt(-3), abc));

  expect_string("slice -100 100 \"abc\"",
      "abc",
      A3(&String_slice, newElmInt(-100), newElmInt(100), abc));
  expect_string("slice -100 100 \"\"",
      "",
      A3(&String_slice, newElmInt(-100), newElmInt(100), create_string("")));
}

void test_String_indexes() {
  expect_equal("indexes \"/\" \"home/steve/Desktop\" == [4,10]",
      A2(&String_indexes, create_string("/"), create_string("home/steve/Desktop")),
      List_create(2,
          (void*[]){
              newElmInt(4),
              newElmInt(10),
          }));

  ElmString* abc = create_string("abc");
  ElmString* ab = create_string("ab");
  ElmString* empty = create_string("");

  expect_equal("indexes \"abc\" \"ab\" == []  # substring longer than string",
      A2(&String_indexes, abc, ab),
      &Nil);

  expect_equal("indexes \"\" \"ab\" == []  # empty substring",
      A2(&String_indexes, empty, ab),
      &Nil);

  expect_equal("indexes \"ab\" \"ab\" == [0]",
      A2(&String_indexes, ab, ab),
      newCons(newElmInt(0), &Nil));

  expect_equal("indexes \"ab\" \"\" == []", A2(&String_indexes, ab, empty), &Nil);
}

void test_String_trim() {
  expect_string("trim \" \\n a \\t \\r\\n\"",
      "a",
      A1(&String_trim, create_string(" \n a \t \r\n")));

  expect_string("trim \"\"", "", A1(&String_trim, create_string("")));

  ElmString* allWhitespaceChars = newElmString(n_whitespace_chars);
  for (size_t i = 0; i < n_whitespace_chars; i++) {
    allWhitespaceChars->words16[i] = all_whitespace_chars[i];
  }
  expect_string("trim allWhitespaceChars", "", A1(&String_trim, allWhitespaceChars));
}

void test_String_trimLeft() {
  expect_string("trimLeft \" \\n a \\t \\r\\n\"",
      "a \t \r\n",
      A1(&String_trimLeft, create_string(" \n a \t \r\n")));

  expect_string("trimLeft \"a \\t \\r\\n\"",
      "a \t \r\n",
      A1(&String_trimLeft, create_string("a \t \r\n")));

  expect_string("trimLeft \" \\n a\"",
      "a",
      A1(&String_trimLeft, create_string(" \n a")));

  expect_string(
      "trimLeft \" \\n \\t \\r\\n\"", "", A1(&String_trimLeft, create_string(" \n \t \r\n")));

  expect_string("trimLeft \"\"", "", A1(&String_trimLeft, create_string("")));
}

void test_String_trimRight() {
  expect_string("trimRight \" \\n a \\t \\r\\n\"",
      " \n a",
      A1(&String_trimRight, create_string(" \n a \t \r\n")));

  expect_string("trimRight \" \\n a\"",
      " \n a",
      A1(&String_trimRight, create_string(" \n a")));

  expect_string("trimRight \"a \\t \\r\\n\"",
      "a",
      A1(&String_trimRight, create_string("a \t \r\n")));

  expect_string("trimRight \" \\n \\t \\r\\n\"",
      "",
      A1(&String_trimRight, create_string(" \n \t \r\n")));

  expect_string("trimRight \"\"", "", A1(&String_trimRight, create_string("")));
}

void* eval_isX(void* args[]) {
  ElmChar* c = args[0];
  return c->value == (u32)'X' ? &True : &False;
}
Closure isX = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_isX,
    .max_values = 1,
};

void test_String_all() {
  expect_equal("all ((==) 'X') \"XXXXX\" == True",
      A2(&String_all, &isX, create_string("XXXXX")),
      &True);

  expect_equal("all ((==) 'X') \"XXXX-\" == False",
      A2(&String_all, &isX, create_string("XXXX-")),
      &False);

  expect_equal(
      "all ((==) 'X') \"\" == True", A2(&String_all, &isX, create_string("")), &True);
}

void test_String_contains() {
  ElmString* abc = create_string("abc");
  ElmString* ab = create_string("ab");
  ElmString* empty = create_string("");

  expect_equal("contains \"ab\" \"abc\" == True", A2(&String_contains, ab, abc), &True);

  expect_equal("contains \"abc\" \"ab\" == False", A2(&String_contains, abc, ab), &False);

  expect_equal(
      "contains \"abc\" \"\" == False", A2(&String_contains, abc, empty), &False);

  expect_equal("contains \"\" \"abc\" == True", A2(&String_contains, empty, abc), &True);

  expect_equal("contains \"\" \"\" == True", A2(&String_contains, empty, empty), &True);
}

void test_String_startsWith() {
  ElmString* abc = create_string("abc");
  ElmString* ab = create_string("ab");
  ElmString* bc = create_string("bc");
  ElmString* empty = create_string("");

  expect_equal(
      "startsWith \"ab\" \"abc\" == True", A2(&String_startsWith, ab, abc), &True);

  expect_equal(
      "startsWith \"bc\" \"abc\" == False", A2(&String_startsWith, bc, abc), &False);

  expect_equal(
      "startsWith \"abc\" \"ab\" == False", A2(&String_startsWith, abc, ab), &False);

  expect_equal(
      "startsWith \"abc\" \"\" == False", A2(&String_startsWith, abc, empty), &False);

  expect_equal(
      "startsWith \"\" \"abc\" == True", A2(&String_startsWith, empty, abc), &True);

  expect_equal(
      "startsWith \"\" \"\" == True", A2(&String_startsWith, empty, empty), &True);
}

void test_String_endsWith() {
  ElmString* abc = create_string("abc");
  ElmString* ab = create_string("ab");
  ElmString* bc = create_string("bc");
  ElmString* empty = create_string("");

  expect_equal("endsWith \"ab\" \"abc\" == False", A2(&String_endsWith, ab, abc), &False);

  expect_equal("endsWith \"bc\" \"abc\" == True", A2(&String_endsWith, bc, abc), &True);

  expect_equal("endsWith \"abc\" \"ab\" == False", A2(&String_endsWith, abc, ab), &False);

  expect_equal(
      "endsWith \"abc\" \"\" == False", A2(&String_endsWith, abc, empty), &False);

  expect_equal("endsWith \"\" \"abc\" == True", A2(&String_endsWith, empty, abc), &True);

  expect_equal("endsWith \"\" \"\" == True", A2(&String_endsWith, empty, empty), &True);
}

void test_String_fromNumber() {
  expect_string("fromNumber 2147483647",
      "2147483647",
      A1(&String_fromNumber, newElmInt(2147483647)));
  expect_string("fromNumber -2147483648",
      "-2147483648",
      A1(&String_fromNumber, newElmInt(-2147483648)));
  expect_string("fromNumber -2147483647",
      "-2147483647",
      A1(&String_fromNumber, newElmInt(-2147483647)));
  expect_string("fromNumber -1",
      "-1",
      A1(&String_fromNumber, newElmInt(-1)));
  expect_string("fromNumber -3.141592653589793",
      "-3.141592653589793",
      A1(&String_fromNumber, newElmFloat(-3.141592653589793)));
  expect_string("fromNumber -3141592653589793",
      "-3141592653589793",
      A1(&String_fromNumber, newElmFloat(-3141592653589793)));
}

void test_String_toInt() {
  expect_equal("toInt \"2147483647\" == Just 2147483647",
      A1(&String_toInt, create_string("2147483647")),
      A1(&g_elm_core_Maybe_Just, newElmInt(2147483647)));

  expect_equal("toInt \"-2147483648\" == Just -2147483648",
      A1(&String_toInt, create_string("-2147483648")),
      A1(&g_elm_core_Maybe_Just, newElmInt(-2147483648)));

  expect_equal("toInt \"+10\" == Just 10",
      A1(&String_toInt, create_string("+10")),
      A1(&g_elm_core_Maybe_Just, newElmInt(10)));

  expect_equal("toInt \"0\" == Just 0",
      A1(&String_toInt, create_string("0")),
      A1(&g_elm_core_Maybe_Just, newElmInt(0)));

  expect_equal("toInt \"+0\" == Just 0",
      A1(&String_toInt, create_string("+0")),
      A1(&g_elm_core_Maybe_Just, newElmInt(0)));

  expect_equal("toInt \"-0\" == Just 0",
      A1(&String_toInt, create_string("-0")),
      A1(&g_elm_core_Maybe_Just, newElmInt(0)));

  expect_equal("toInt \"hello\" == Nothing",
      A1(&String_toInt, create_string("hello")),
      &g_elm_core_Maybe_Nothing);

  expect_equal("toInt \"\" == Nothing",
      A1(&String_toInt, create_string("")),
      &g_elm_core_Maybe_Nothing);
}

// ---------------------------------------------------------
//
//          RUN ALL STRING TESTS
//
// ---------------------------------------------------------

void string_test() {
  if (verbose) {
    safe_printf("\n\n\n");
    safe_printf("####################################################\n");
    safe_printf("\n");
    safe_printf("String\n");
    safe_printf("------\n");
  }

  mu_run_test(test_code_units);
  mu_run_test(test_find_reverse);
  mu_run_test(test_find_forward);
  mu_run_test(test_is_whitespace);

  describe("test_String_uncons", &test_String_uncons);
  describe("test_String_append", &test_String_append);
  describe("test_String_split", &test_String_split);
  describe("test_String_join", &test_String_join);
  describe("test_String_slice", &test_String_slice);
  describe("test_String_trim", &test_String_trim);
  describe("test_String_trimLeft", &test_String_trimLeft);
  describe("test_String_trimRight", &test_String_trimRight);
  describe("test_String_all", &test_String_all);
  describe("test_String_contains", &test_String_contains);
  describe("test_String_startsWith", &test_String_startsWith);
  describe("test_String_endsWith", &test_String_endsWith);
  describe("test_String_indexes", &test_String_indexes);
  describe("test_String_fromNumber", &test_String_fromNumber);
  describe("test_String_toInt", &test_String_toInt);
}
