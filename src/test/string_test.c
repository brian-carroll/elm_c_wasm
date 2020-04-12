#include "../kernel/string.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "../kernel/debug.h"
#include "../kernel/types.h"
#include "../kernel/utils.h"
#include "../kernel/gc.h"
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
    FILE* f = stderr;
    fprintf(f, "%s: Expected \"%s\" but got \"", test_description, expected_c_str);
    print_elm_string(f, actual);
    fprintf(f, "\"\n");
  } else if (verbose) {
    printf("OK: \"%s\"\n", expected_c_str);
  }
  assertions_made++;
  return NULL;
}

// ---------------------------------------------------------
//          STRING TESTS
// ---------------------------------------------------------

void* string_append_test() {
  ElmString16* hello = create_string("hello");
  ElmString16* world = create_string(" world");
  expect_string("hello world", A2(&String_append, hello, world));
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

  describe("string_append_test", &string_append_test);
  return NULL;
}

/*
- String_append
  - "hello " ++ "world" == "hello world"
- String_fromNumber
  - Int 2147483647 == "2147483647"
  - Int -2147483648 == "-2147483648"
  - Float -3.141592653589793
  - Float -3141592653589793
- String_join
  -
- String_split
  -
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
