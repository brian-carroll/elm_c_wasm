#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "../kernel/kernel.h"
#include "./test.h"

// ---------------------------------------------------------
//          STRING TEST UTILITIES
// ---------------------------------------------------------

char* test_description;

void describe(char* description, void* (*test)()) {
  tests_run++;
  test_description = description;
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

void print_elm_string(FILE* file, ElmString16* str) {
  size_t len = code_units(str);
  for (size_t i = 0; i < len; i++) {
    u16 c = str->words16[i];
    fprintf(file, "%c", (char)c);
  }
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
    printf(" but got \"");
    print_elm_string(stdout, actual);
    printf("\"\n");
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

char* string_test() {
  if (verbose) {
    printf("\n\n\n");
    printf("####################################################\n");
    printf("\n");
    printf("String\n");
    printf("------\n");
  }

  mu_run_test(test_code_units);
  describe("test_String_append", &test_String_append);
  describe("test_String_fromNumber", &test_String_fromNumber);
  describe("test_String_join", &test_String_join);
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
