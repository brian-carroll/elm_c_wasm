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

  int msg_prefix_len;
  if (expect_val) {
    msg_prefix_len = sprintf(
        parse_test_msg, "should correctly parse %zd chars from '", expect_chars_consumed);
  } else {
    msg_prefix_len = sprintf(parse_test_msg, "should reject '");
  }
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
  parse_test_helper(&parse_bool, create_string("true"), &Json_Value_true, 4);
  parse_test_helper(&parse_bool, create_string("false"), &Json_Value_false, 5);
  parse_test_helper(&parse_bool, create_string("truh"), NULL, 0);
  parse_test_helper(&parse_bool, create_string("falsy"), NULL, 0);
  parse_test_helper(&parse_bool, create_string("f"), NULL, 0);
  parse_test_helper(&parse_bool, create_string(""), NULL, 0);
  return NULL;
}

void* parse_null(u16** cursor, u16* end);
void* test_Json_parse_null() {
  parse_test_helper(&parse_null, create_string("null"), &Json_Value_null, 4);
  parse_test_helper(&parse_null, create_string("nule"), NULL, 0);
  parse_test_helper(&parse_null, create_string("n"), NULL, 0);
  parse_test_helper(&parse_null, create_string(""), NULL, 0);
  return NULL;
}

void* parse_number(u16** cursor, u16* end);
void* test_Json_parse_number() {
  parse_test_helper(&parse_number, create_string("123.456"), NEW_ELM_FLOAT(123.456), 7);
  parse_test_helper(&parse_number, create_string("-123.456"), NEW_ELM_FLOAT(-123.456), 8);
  parse_test_helper(
      &parse_number, create_string("-123.456e-3"), NEW_ELM_FLOAT(-123.456e-3), 11);
  parse_test_helper(
      &parse_number, create_string("-123.456E+1"), NEW_ELM_FLOAT(-1234.56), 11);
  parse_test_helper(&parse_number, create_string(""), NULL, 0);
  parse_test_helper(&parse_number, create_string("-+e"), NULL, 0);
  parse_test_helper(&parse_number, create_string("abc"), NULL, 0);
  return NULL;
}

#define DQUOTE "\""
void* parse_string(u16** cursor, u16* end);
ElmString16 unicode_test_string = {
    .header = HEADER_STRING(2), .words16 = {0xD852, 0xDF62},  // 𤭢
};

void* test_Json_parse_string() {
  // valid strings
  parse_test_helper(
      &parse_string, create_string(DQUOTE "hello" DQUOTE), create_string("hello"), 7);

  parse_test_helper(&parse_string,
      create_string(DQUOTE "hello" DQUOTE ": 123"),
      create_string("hello"),
      7);

  parse_test_helper(&parse_string, create_string(DQUOTE DQUOTE), create_string(""), 2);

  parse_test_helper(&parse_string,
      create_string(DQUOTE "\\\""
                           "\\\\"
                           "\\/"
                           "\\b"
                           "\\f"
                           "\\n"
                           "\\r"
                           "\\t" DQUOTE),
      create_string("\""
                    "\\"
                    "/"
                    "\b"
                    "\f"
                    "\n"
                    "\r"
                    "\t"),
      18);

  parse_test_helper(&parse_string,
      create_string(DQUOTE "\\uD852\\uDF62" DQUOTE),
      &unicode_test_string,
      14);

  // invalid JSON strings
  parse_test_helper(
      &parse_string, create_string("hello" DQUOTE), NULL, 0);  // no opening quotes
  parse_test_helper(&parse_string,
      create_string(DQUOTE "hello"),  // no closing quotes
      NULL,
      0);
  parse_test_helper(&parse_string,
      create_string(DQUOTE "\\g" DQUOTE),  // unknown escaped character
      NULL,
      0);
  parse_test_helper(&parse_string,
      create_string(DQUOTE "\\u123" DQUOTE),  // unicode sequence too short
      NULL,
      0);
  parse_test_helper(&parse_string,
      create_string(DQUOTE "\\u123X" DQUOTE),  // unicode sequence non-hex
      NULL,
      0);
  parse_test_helper(&parse_string,
      create_string(DQUOTE "\\" DQUOTE),  // escaped closing quote
      NULL,
      0);
  parse_test_helper(&parse_string,
      create_string(DQUOTE "hello\\"),  // escape char at end of input
      NULL,
      0);
  parse_test_helper(&parse_string, create_string(""), NULL, 0);
  parse_test_helper(
      &parse_string, create_string(DQUOTE "\t" DQUOTE), NULL, 0);  // unescaped tab

  return NULL;
}

void skip_whitespace(u16** cursor, u16* end);
void* test_Json_skip_whitespace() {
  ElmString16* json;
  u16* cursor;

  json = create_string("\t\r\n true \t\r\n");
  cursor = json->words16;
  skip_whitespace(&cursor, cursor + code_units(json));
  mu_expect_equal("should move cursor to first non-whitespace", cursor, &json->words16[4]);

  json = create_string("true \t\r\n");
  cursor = json->words16;
  skip_whitespace(&cursor, cursor + code_units(json));
  mu_expect_equal("should do nothing if it's not on whitespace", cursor, json->words16);

  json = create_string(" \t\r\n");
  cursor = json->words16;
  skip_whitespace(&cursor, cursor + code_units(json));
  mu_expect_equal("should not run past end of string", cursor, &json->words16[4]);

  json = create_string("");
  cursor = json->words16;
  skip_whitespace(&cursor, cursor + code_units(json));
  mu_expect_equal("should handle empty string", cursor, json->words16);

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
  describe("test_Json_parse_number", test_Json_parse_number);
  describe("test_Json_parse_string", test_Json_parse_string);
  describe("test_Json_skip_whitespace", test_Json_skip_whitespace);  

  return NULL;
}