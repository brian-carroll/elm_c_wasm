#include <stddef.h>

#include "../../kernel/core/core.h"
#include "../../kernel/json/json.h"
#include "../test.h"

//-----------------------------------------
// Test helpers

#define PARSE_TEST_MSG_LEN 1024
char parse_test_msg[PARSE_TEST_MSG_LEN];

void parse_test(void* (*parse_func)(u16** cursor, u16* end),
    ElmString* json,
    void* expect_val,
    size_t expect_chars_consumed) {
  // ---
  size_t len = code_units(json);
  u16* cursor = json->words16;
  void* result = (*parse_func)(&cursor, cursor + len);

  int msg_prefix_len;
  if (expect_val) {
    msg_prefix_len = stbsp_sprintf(
        parse_test_msg, "should correctly parse %zd chars from '", expect_chars_consumed);
  } else {
    msg_prefix_len = stbsp_sprintf(parse_test_msg, "should reject '");
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
    safe_printf("FAIL: %s\n", parse_test_msg);
    safe_printf("Expected to consume %zd characters but consumed %zd\n",
        expect_chars_consumed,
        consumed);
    assertions_made++;
    tests_failed++;
  } else {
    expect_equal(parse_test_msg, result, expect_val);
  }
}

//-----------------------------------------

void* parse_bool(u16** cursor, u16* end);
void test_Json_parse_bool() {
  parse_test(&parse_bool, create_string("true"), &True, 4);
  parse_test(&parse_bool, create_string("false"), &False, 5);
  parse_test(&parse_bool, create_string("truh"), NULL, 0);
  parse_test(&parse_bool, create_string("falsy"), NULL, 0);
  parse_test(&parse_bool, create_string("f"), NULL, 0);
  parse_test(&parse_bool, create_string(""), NULL, 0);
}

void* parse_null(u16** cursor, u16* end);
void test_Json_parse_null() {
  parse_test(&parse_null, create_string("null"), &Json_null, 4);
  parse_test(&parse_null, create_string("nule"), NULL, 0);
  parse_test(&parse_null, create_string("n"), NULL, 0);
  parse_test(&parse_null, create_string(""), NULL, 0);
}

void* parse_number(u16** cursor, u16* end);
void test_Json_parse_number() {
  parse_test(&parse_number, create_string("123.456"), newElmFloat(123.456), 7);
  parse_test(&parse_number, create_string("-123.456"), newElmFloat(-123.456), 8);
  parse_test(&parse_number, create_string("-123.456e-3"), newElmFloat(-123.456e-3), 11);
  parse_test(&parse_number, create_string("-123.456E+1"), newElmFloat(-1234.56), 11);
  parse_test(&parse_number, create_string(""), NULL, 0);
  parse_test(&parse_number, create_string("-+e"), NULL, 0);
  parse_test(&parse_number, create_string("abc"), NULL, 0);
}

#define DQUOTE "\""
void* parse_string(u16** cursor, u16* end);
ElmString unicode_test_string = {
    .header = HEADER_STRING(2), .words16 = {0xD852, 0xDF62},  // 𤭢
};

void test_Json_parse_string() {
  // valid strings
  parse_test(
      &parse_string, create_string(DQUOTE "hello" DQUOTE), create_string("hello"), 7);

  parse_test(&parse_string,
      create_string(DQUOTE "hello" DQUOTE ": 123"),
      create_string("hello"),
      7);

  parse_test(&parse_string, create_string(DQUOTE DQUOTE), create_string(""), 2);

  parse_test(&parse_string,
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

  parse_test(&parse_string,
      create_string(DQUOTE "\\uD852\\uDF62" DQUOTE),
      &unicode_test_string,
      14);

  // invalid JSON strings
  parse_test(&parse_string, create_string("hello" DQUOTE), NULL, 0);  // no opening quotes
  parse_test(&parse_string,
      create_string(DQUOTE "hello"),  // no closing quotes
      NULL,
      0);
  parse_test(&parse_string,
      create_string(DQUOTE "\\g" DQUOTE),  // unknown escaped character
      NULL,
      0);
  parse_test(&parse_string,
      create_string(DQUOTE "\\u123" DQUOTE),  // unicode sequence too short
      NULL,
      0);
  parse_test(&parse_string,
      create_string(DQUOTE "\\u123X" DQUOTE),  // unicode sequence non-hex
      NULL,
      0);
  parse_test(&parse_string,
      create_string(DQUOTE "\\" DQUOTE),  // escaped closing quote
      NULL,
      0);
  parse_test(&parse_string,
      create_string(DQUOTE "hello\\"),  // escape char at end of input
      NULL,
      0);
  parse_test(&parse_string, create_string(""), NULL, 0);
  parse_test(&parse_string, create_string(DQUOTE "\t" DQUOTE), NULL, 0);  // unescaped tab
}

void skip_whitespace(u16** cursor, u16* end);
void test_Json_skip_whitespace() {
  ElmString* json;
  u16* cursor;

  json = create_string("\t\r\n true \t\r\n");
  cursor = json->words16;
  skip_whitespace(&cursor, cursor + code_units(json));
  mu_expect_equal(
      "should move cursor to first non-whitespace", cursor, &json->words16[4]);

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
}

void* parse_array(u16** cursor, u16* end);
void test_Json_parse_array() {
  // valid arrays

  parse_test(&parse_array,
      create_string("[123]"),
      newCustom(JSON_VALUE_ARRAY, 1, (void*[]){newElmFloat(123)}),
      5);

  parse_test(&parse_array,
      create_string("[\r\n\tnull\r\n ] "),
      newCustom(JSON_VALUE_ARRAY, 1, (void*[]){&Json_null}),
      12);

  parse_test(&parse_array,
      create_string("[ 123,\"hi\" ]"),
      newCustom(JSON_VALUE_ARRAY,
          2,
          ((void*[]){
              newElmFloat(123),
              create_string("hi"),
          })),
      12);

  parse_test(&parse_array,
      create_string("[123 , \"hi\"]"),
      newCustom(JSON_VALUE_ARRAY,
          2,
          ((void*[]){
              newElmFloat(123),
              create_string("hi"),
          })),
      12);

  parse_test(&parse_array, create_string("[]"), newCustom(JSON_VALUE_ARRAY, 0, NULL), 2);

  parse_test(
      &parse_array, create_string("[ \r\n\t]"), newCustom(JSON_VALUE_ARRAY, 0, NULL), 6);

  // failure cases
  parse_test(&parse_array, create_string(""), NULL, 0);
  parse_test(&parse_array, create_string("[invalid]"), NULL, 0);
  parse_test(&parse_array, create_string("null ]"), NULL, 0);
  parse_test(&parse_array, create_string("["), NULL, 0);
  parse_test(&parse_array, create_string("[ "), NULL, 0);
  parse_test(&parse_array, create_string("[ null"), NULL, 0);
  parse_test(&parse_array, create_string("[ null "), NULL, 0);
  parse_test(&parse_array, create_string("[ null ,"), NULL, 0);
  parse_test(&parse_array, create_string("[ null , "), NULL, 0);
  parse_test(&parse_array, create_string("[ null , ]"), NULL, 0);
}

void* parse_object(u16** cursor, u16* end);
void test_Json_parse_object() {
  // valid objects

  parse_test(&parse_object,
      create_string("{\"a\":123}"),
      newCustom(JSON_VALUE_OBJECT,
          2,
          ((void*[]){
              create_string("a"),
              newElmFloat(123),
          })),
      9);

  parse_test(&parse_object,
      create_string("{ \"a\" : 123 } "),
      newCustom(JSON_VALUE_OBJECT,
          2,
          ((void*[]){
              create_string("a"),
              newElmFloat(123),
          })),
      13);

  parse_test(&parse_object,
      create_string("{ \"a\":123,\"stuff\":false }"),
      newCustom(JSON_VALUE_OBJECT,
          4,
          ((void*[]){
              create_string("a"),
              newElmFloat(123),
              create_string("stuff"),
              &False,
          })),
      25);

  parse_test(&parse_object,
      create_string("{\"a\":123 , \"stuff\":false}"),
      newCustom(JSON_VALUE_OBJECT,
          4,
          ((void*[]){
              create_string("a"),
              newElmFloat(123),
              create_string("stuff"),
              &False,
          })),
      25);

  parse_test(
      &parse_object, create_string("{}"), newCustom(JSON_VALUE_OBJECT, 0, NULL), 2);

  parse_test(&parse_object,
      create_string("{ \r\n\t}"),
      newCustom(JSON_VALUE_OBJECT, 0, NULL),
      6);

  // // failure cases
  parse_test(&parse_object, create_string("{null:123}"), NULL, 0);
  parse_test(&parse_object, create_string("{0:123}"), NULL, 0);
  parse_test(&parse_object, create_string("123"), NULL, 0);

  parse_test(&parse_object, create_string("{ \"a\" : 123 , }"), NULL, 0);
  parse_test(&parse_object, create_string("{ \"a\" : 123 , "), NULL, 0);
  parse_test(&parse_object, create_string("{ \"a\" : 123 ,"), NULL, 0);
  parse_test(&parse_object, create_string("{ \"a\" : 123 "), NULL, 0);
  parse_test(&parse_object, create_string("{ \"a\" : 123"), NULL, 0);
  parse_test(&parse_object, create_string("{ \"a\" : "), NULL, 0);
  parse_test(&parse_object, create_string("{ \"a\" :"), NULL, 0);
  parse_test(&parse_object, create_string("{ \"a\" "), NULL, 0);
  parse_test(&parse_object, create_string("{ \"a\""), NULL, 0);
  parse_test(&parse_object, create_string("{ "), NULL, 0);
  parse_test(&parse_object, create_string("{"), NULL, 0);
  parse_test(&parse_object, create_string(""), NULL, 0);
}

void json_parser_test() {
  if (verbose) {
    safe_printf("\n");
    safe_printf("Json parser\n");
    safe_printf("------------\n");
  }

  describe("test_Json_parse_bool", test_Json_parse_bool);
  describe("test_Json_parse_null", test_Json_parse_null);
  describe("test_Json_parse_number", test_Json_parse_number);
  describe("test_Json_parse_string", test_Json_parse_string);
  describe("test_Json_skip_whitespace", test_Json_skip_whitespace);
  describe("test_Json_parse_array", test_Json_parse_array);
  describe("test_Json_parse_object", test_Json_parse_object);
}