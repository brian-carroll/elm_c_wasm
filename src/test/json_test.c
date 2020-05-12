#include "../kernel/kernel.h"
#include "./test.h"

void* parse_bool(u16** cursor, u16* end);
void* test_Json_parse_bool() {
  ElmString16* json;
  u16* cursor;
  void* result;

  json = create_string("true");
  cursor = json->words16;
  result = parse_bool(&cursor, cursor + code_units(json));
  mu_expect_equal("parse_bool(\"true\") == True", result, &True);
  mu_expect_equal(
      "should advance cursor to the end of 'true'", cursor - json->words16, 4);

  json = create_string("false");
  cursor = json->words16;
  result = parse_bool(&cursor, cursor + code_units(json));
  mu_expect_equal("parse_bool(\"false\") == False", result, &False);
  mu_expect_equal(
      "should advance cursor to the end of 'false'", cursor - json->words16, 5);

  json = create_string("truck");
  cursor = json->words16;
  result = parse_bool(&cursor, cursor + code_units(json));
  mu_expect_equal("parse_bool(\"truck\") == NULL", result, NULL);
  mu_expect_equal("should not move cursor for 'truck'", cursor - json->words16, 0);

  json = create_string("f");
  cursor = json->words16;
  result = parse_bool(&cursor, cursor + code_units(json));
  mu_expect_equal("parse_bool(\"f\") == NULL", result, NULL);
  mu_expect_equal("should not move cursor for 'f'", cursor - json->words16, 0);

  return NULL;
}

void* parse_null(u16** cursor, u16* end);
void* test_Json_parse_null() {
  ElmString16* json;
  u16* cursor;
  void* result;

  json = create_string("null");
  cursor = json->words16;
  result = parse_null(&cursor, cursor + code_units(json));
  mu_expect_equal("parse_null(\"null\") == JsNull", result, &JsNull);
  mu_expect_equal(
      "should advance cursor to the end of 'null'", cursor - json->words16, 4);

  json = create_string("nule");
  cursor = json->words16;
  result = parse_null(&cursor, cursor + code_units(json));
  mu_expect_equal("parse_null(\"nule\") == NULL", result, NULL);
  mu_expect_equal("should not move cursor for 'nule'", cursor - json->words16, 0);

  json = create_string("n");
  cursor = json->words16;
  result = parse_null(&cursor, cursor + code_units(json));
  mu_expect_equal("parse_null(\"n\") == NULL", result, NULL);
  mu_expect_equal("should not move cursor for 'n'", cursor - json->words16, 0);

  return NULL;
}

void* parse_int(u16** cursor, u16* end);
void* test_Json_parse_int() {
  ElmString16* json;
  u16* cursor;
  void* result;

  json = create_string("123");
  cursor = json->words16;
  result = parse_int(&cursor, cursor + code_units(json));
  expect_equal("parse_int(\"123\") == 123", result, NEW_ELM_INT(123));
  mu_expect_equal("should advance cursor to the end", cursor - json->words16, 3);

  json = create_string("-123");
  cursor = json->words16;
  result = parse_int(&cursor, cursor + code_units(json));
  expect_equal("parse_int(\"-123\") == -123", result, NEW_ELM_INT(-123));
  mu_expect_equal("should advance cursor to the end", cursor - json->words16, 4);

  json = create_string("");
  cursor = json->words16;
  result = parse_int(&cursor, cursor + code_units(json));
  mu_expect_equal("parse_int(\"\") == NULL", result, NULL);
  mu_expect_equal("should not move cursor", cursor - json->words16, 0);

  json = create_string("-");
  cursor = json->words16;
  result = parse_int(&cursor, cursor + code_units(json));
  mu_expect_equal("parse_int(\"-\") == NULL", result, NULL);
  mu_expect_equal("should not move cursor", cursor - json->words16, 0);

  json = create_string("abc");
  cursor = json->words16;
  result = parse_int(&cursor, cursor + code_units(json));
  mu_expect_equal("parse_int(\"abc\") == NULL", result, NULL);
  mu_expect_equal("should not move cursor", cursor - json->words16, 0);

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

  return NULL;
}