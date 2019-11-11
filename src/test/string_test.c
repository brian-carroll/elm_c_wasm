#include "../kernel/string.h"
#include <stdlib.h>
#include <string.h>
#include "../kernel/types.h"
#include "../kernel/utils.h"
#include "./test.h"

size_t String_bytes(ElmString* s);

char* string_bytes_test() {
  if (verbose) {
    printf("\n");
    printf("## String_bytes\n");
    printf("\n");
  }
  ElmString* str0 = NEW_ELM_STRING(0, "");
  size_t len0 = String_bytes(str0);
  mu_assert("Expect correct byte length for string length 0", len0 == 0);
  mu_assert("Expect correct byte length for string length 1",
      String_bytes(NEW_ELM_STRING(1, "1")) == 1);
  mu_assert("Expect correct byte length for string length 3",
      String_bytes(NEW_ELM_STRING(3, "123")) == 3);
  mu_assert("Expect correct byte length for string length 4",
      String_bytes(NEW_ELM_STRING(4, "1234")) == 4);
  mu_assert("Expect correct byte length for string length 5",
      String_bytes(NEW_ELM_STRING(5, "12345")) == 5);
  mu_assert("Expect correct byte length for string length 7",
      String_bytes(NEW_ELM_STRING(7, "123457")) == 7);
  mu_assert("Expect correct byte length for string length 8",
      String_bytes(NEW_ELM_STRING(8, "1234578")) == 8);
  mu_assert("Expect correct byte length for string length 9",
      String_bytes(NEW_ELM_STRING(9, "12345789")) == 9);
  mu_assert("Expect correct byte length for string length 10",
      String_bytes(NEW_ELM_STRING(10, "12345789a")) == 10);
  mu_assert("Expect correct byte length for string length 11",
      String_bytes(NEW_ELM_STRING(11, "12345789ab")) == 11);
  mu_assert("Expect correct byte length for string length 12",
      String_bytes(NEW_ELM_STRING(12, "12345789abc")) == 12);

  return NULL;
}

char* string_append_test() {
  ElmString* h = NEW_ELM_STRING(5, "hello");
  ElmString* w = NEW_ELM_STRING(6, " world");
  ElmString* hw = A2(&String_append, h, w);

  if (verbose) {
    printf("\n");
    printf("## String_append\n");
    printf("\n");
    printf("\"%s\" ++ \"%s\" == \"%s\"\n", h->bytes, w->bytes, hw->bytes);
    printf("hello: %s\n", hex(h, sizeof(ElmString) + 8));
    printf("world: %s\n", hex(w, sizeof(ElmString) + 8));
    printf("hello world: %s\n", hex(hw, sizeof(ElmString) + 12));
  }

  mu_assert("Expect: \"hello\" ++ \" world\" == \"hello world\"",
      strcmp((char*)hw->bytes, "hello world") == 0);

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

  mu_run_test(string_bytes_test);
  mu_run_test(string_append_test);
  return NULL;
}
