#include <stddef.h>

#include "../kernel/kernel.h"
#include "./test.h"

#define PARSE_TEST_MSG_LEN 1024
char parse_test_msg[PARSE_TEST_MSG_LEN];

char* parse_test_helper(void* (*parse_func)(u16** cursor, u16* end),
    ElmString16* json,
    void* expect_val,
    size_t expect_chars_consumed) {
  // ---
  size_t len = code_units(json);
  u16* cursor = json->words16;
  void* result = (*parse_func)(&cursor, cursor + len);

  int msg_prefix_len =
      sprintf(parse_test_msg, "should %s '", expect_val ? "parse" : "reject");
  int i;
  for (i = 0; i < len && i < PARSE_TEST_MSG_LEN; i++) {
    u16 c = json->words16[i];
    parse_test_msg[msg_prefix_len + i] = (c > 255) ? '#' : c;
  }
  parse_test_msg[msg_prefix_len + i] = '\'';
  parse_test_msg[msg_prefix_len + i + 1] = 0;

  ptrdiff_t consumed = cursor - json->words16;

  if (expect_val == NULL) {
    mu_assert(parse_test_msg, result == NULL && consumed == 0);
  } else if (consumed != expect_chars_consumed) {
    printf("FAIL: %s\n", parse_test_msg);
    printf("Expected to consume %zd characters but consumed %zd\n",
        expect_chars_consumed,
        consumed);
    assertions_made++;
    tests_failed++;
  } else {
    expect_equal(parse_test_msg, result, expect_val);
  }

  return NULL;
}

void* parse_bool(u16** cursor, u16* end);
void* test_Json_parse_bool() {
  parse_test_helper(&parse_bool, create_string("true"), &True, 4);
  parse_test_helper(&parse_bool, create_string("false"), &False, 5);
  parse_test_helper(&parse_bool, create_string("truck"), NULL, 0);
  parse_test_helper(&parse_bool, create_string("f"), NULL, 0);
  parse_test_helper(&parse_bool, create_string(""), NULL, 0);
  return NULL;
}

void* parse_null(u16** cursor, u16* end);
void* test_Json_parse_null() {
  parse_test_helper(&parse_null, create_string("null"), &JsNull, 4);
  parse_test_helper(&parse_null, create_string("nule"), NULL, 0);
  parse_test_helper(&parse_null, create_string("n"), NULL, 0);
  parse_test_helper(&parse_null, create_string(""), NULL, 0);
  return NULL;
}

void* parse_int(u16** cursor, u16* end);
void* test_Json_parse_int() {
  parse_test_helper(&parse_int, create_string("123"), NEW_ELM_INT(123), 3);
  parse_test_helper(&parse_int, create_string("-123"), NEW_ELM_INT(-123), 4);
  parse_test_helper(&parse_int, create_string(""), NULL, 0);
  parse_test_helper(&parse_int, create_string("-"), NULL, 0);
  parse_test_helper(&parse_int, create_string("abc"), NULL, 0);
  return NULL;
}

void* parse_float(u16** cursor, u16* end);
void* test_Json_parse_float() {
  parse_test_helper(&parse_float, create_string("123.456"), NEW_ELM_FLOAT(123.456), 7);
  parse_test_helper(&parse_float, create_string("-123.456"), NEW_ELM_FLOAT(-123.456), 8);
  parse_test_helper(
      &parse_float, create_string("-123.456e-3"), NEW_ELM_FLOAT(-123.456e-3), 11);
  parse_test_helper(
      &parse_float, create_string("-123.456E+1"), NEW_ELM_FLOAT(-1234.56), 11);
  parse_test_helper(&parse_float, create_string(""), NULL, 0);
  parse_test_helper(&parse_float, create_string("-+e"), NULL, 0);
  parse_test_helper(&parse_float, create_string("abc"), NULL, 0);
  return NULL;
}

char* json_test() {
  if (verbose) {
    printf("\n\n\n");
    printf("####################################################\n");
    printf("\n");
    printf("Json\n");
    printf("------\n");
  }

  describe("test_Json_parse_bool", test_Json_parse_bool);
  describe("test_Json_parse_null", test_Json_parse_null);
  describe("test_Json_parse_int", test_Json_parse_int);
  describe("test_Json_parse_float", test_Json_parse_float);

  return NULL;
}