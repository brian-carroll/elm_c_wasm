#include "../kernel/kernel.h"
#include "./test.h"

void* parse_bool(u16** cursor);

void* test_Json_parse_bool() {
  ElmString16* json;
  u16* cursor;

  json = create_string("true");
  cursor = json->words16;
  mu_expect_equal("parse_bool(\"true\") == True", parse_bool(&cursor), &True);
  mu_expect_equal(
      "should advance cursor to the end of 'true'", cursor - json->words16, 4);

  json = create_string("false");
  cursor = json->words16;
  mu_expect_equal("parse_bool(\"false\") == False", parse_bool(&cursor), &False);
  mu_expect_equal(
      "should advance cursor to the end of 'false'", cursor - json->words16, 5);

  json = create_string("truck");
  cursor = json->words16;
  mu_expect_equal("parse_bool(\"truck\") == NULL", parse_bool(&cursor), NULL);
  mu_expect_equal("should not move cursor for 'truck'", cursor - json->words16, 0);

  return NULL;
}


void* parse_null(u16** cursor);

void* test_Json_parse_null() {
  ElmString16* json;
  u16* cursor;

  json = create_string("null");
  cursor = json->words16;
  mu_expect_equal("parse_null(\"null\") == JsNull", parse_null(&cursor), &JsNull);
  mu_expect_equal(
      "should advance cursor to the end of 'null'", cursor - json->words16, 4);

  json = create_string("nule");
  cursor = json->words16;
  mu_expect_equal("parse_null(\"nule\") == NULL", parse_null(&cursor), NULL);
  mu_expect_equal("should not move cursor for 'nule'", cursor - json->words16, 0);

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
  return NULL;
}