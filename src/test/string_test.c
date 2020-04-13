#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "../kernel/kernel.h"
#include "./test.h"

// ---------------------------------------------------------
//          STRING TEST UTILITIES
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

void* expect_equal(void* left, void* right) {
  bool ok = A2(&Utils_equal, left, right) == &True;
  if (!ok) {
    if (!verbose) {
      printf("\n%s\n", test_description);
    }
    printf("\n");
    printf("Left: %p\n", left);
    printf("Right: %p\n", right);
    print_heap_range(test_heap_ptr, GC_malloc(0));
    printf("\n");
    tests_failed++;
  } else if (verbose) {
    printf("  OK\n");
  }
  assertions_made++;
  return NULL;
}

void* expect_string(char* expected_c_str, ElmString16* actual) {
  ElmString16* expected = create_string(expected_c_str);
  bool ok = A2(&Utils_equal, actual, expected) == &True;
  if (!ok) {
    if (!verbose) {
      printf("\n%s\n", test_description);
    }
    printf("\n");
    printf("Expected \"%s\"\n", expected_c_str);
    print_value_full(expected);
    printf("\n");
    printf(" but got ");
    print_elm_string(actual);
    printf("\n");
    print_value_full(actual);
    printf("\n");
    tests_failed++;
  } else if (verbose) {
    printf("  OK: \"%s\"\n", expected_c_str);
  }
  assertions_made++;
  return NULL;
}

// ---------------------------------------------------------
//          STRING TESTS
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

void* test_String_append() {
  ElmString16* hello = create_string("hello");
  ElmString16* world = create_string(" world");
  expect_string("hello world", A2(&String_append, hello, world));
  return NULL;
}

void* test_String_fromNumber() {
  expect_string("2147483647", A1(&String_fromNumber, NEW_ELM_INT(2147483647)));
  expect_string("-2147483648", A1(&String_fromNumber, NEW_ELM_INT(-2147483648)));
  expect_string(
      "-3.141592653589793", A1(&String_fromNumber, NEW_ELM_FLOAT(-3.141592653589793)));
  expect_string(
      "-3141592653589793", A1(&String_fromNumber, NEW_ELM_FLOAT(-3141592653589793)));
  return NULL;
}

void* test_String_join() {
  expect_string("home/steve/Desktop",
      A2(&String_join,
          create_string("/"),
          List_create(3,
              (void*[]){
                  create_string("home"),
                  create_string("steve"),
                  create_string("Desktop"),
              })));
  return NULL;
}

ptrdiff_t find_reverse(u16* sub, u16* str, size_t sub_len, ptrdiff_t str_idx);

void* test_find_reverse() {
  ElmString16* sep;
  ElmString16* str = create_string("home/jill/Desktop");
  size_t str_len = code_units(str);
  ptrdiff_t result;

  if (verbose) {
    printf("find_reverse\n");
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
  mu_expect_equal(
      "should find 2nd last char, given '' again", result, str_len - 2);

  sep = create_string("h");
  result = find_reverse(sep->words16, str->words16, code_units(sep), str_len - 1);
  mu_expect_equal("should find a single char at start", result, 0);

  sep = create_string("ji");
  result = find_reverse(sep->words16, str->words16, code_units(sep), str_len - 1);
  mu_expect_equal(
      "should find multiple chars in middle (no retry)", result, 5);

  sep = create_string("jil");
  result = find_reverse(sep->words16, str->words16, code_units(sep), str_len - 1);
  mu_expect_equal(
      "should find multiple chars in middle (immediate retry)", result, 5);

  sep = create_string("ho");
  result = find_reverse(sep->words16, str->words16, code_units(sep), str_len - 1);
  mu_expect_equal("should find multiple chars at start (retry)", result, 0);

  return NULL;
}

void* test_String_split() {
  /*
  > String.split "abc" "ab"
  ["ab"] : List String
  > String.split "ab" "ab"
  ["",""] : List String
  > String.split "" "ab"
  ["a","b"] : List String
  */
  // expect_equal(List_create(3,
  //                  (void*[]){
  //                      create_string("home"),
  //                      create_string("steve"),
  //                      create_string("Desktop"),
  //                  }),
  //     A2(&String_split, create_string("/"), create_string("home/steve/Desktop")));

  // ElmString16* abc = create_string("abc");
  // ElmString16* ab = create_string("ab");
  // ElmString16* empty = create_string("");
  // Cons* list_ab = NEW_CONS(ab, &Nil);

  // printf("\n## separator longer than search string\n\n");
  // expect_equal(list_ab, A2(&String_split, abc, ab));

  // printf("\n## separate on empty string\n\n");
  // expect_equal(List_create(2,
  //                  (void*[]){
  //                      create_string("a"),
  //                      create_string("b"),
  //                  }),
  //     A2(&String_split, empty, ab));

  // printf("\n## separator same as search string\n\n");
  // expect_equal(List_create(2,
  //                  (void*[]){
  //                      empty,
  //                      empty,
  //                  }),
  //     A2(&String_split, ab, ab));

  return NULL;
}

char* string_test() {
  if (verbose) {
    printf("\n\n\n");
    printf("####################################################\n");
    printf("\n");
    printf("String\n");
    printf("------\n");
  }

  // mu_run_test(test_code_units);
  // describe("test_String_append", &test_String_append);
  // describe("test_String_fromNumber", &test_String_fromNumber);
  // describe("test_String_join", &test_String_join);
  mu_run_test(test_find_reverse);
  // describe("test_String_split", &test_String_split);

  return NULL;
}

/*
- String_join
  - String.join "/" [ "home", "steve", "Desktop" ]
- String_split
  -[ "home", "steve", "Desktop", "" ] (String.split "/" "home/steve/Desktop/")
- String_all
  -
- String_length
  -
- String_slice
  -
- String_toInt
  -
- String_uncons
  -
- String_contains
  -
- String_indexes
  -
- String_startsWith
  -
- String_trim
  -
*/
