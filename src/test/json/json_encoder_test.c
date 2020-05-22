#include "../../kernel/core/core.h"
#include "../../kernel/json/json.h"
#include "../../kernel/json/json-internal.h"
#include "../test.h"

ElmInt int0 = {
    .header = HEADER_INT,
    .value = 0,
};

void* test_Json_encode_boolean() {
  expect_equal("should encode True to 'true'",
      A2(&Json_encode, &int0, &True),
      create_string("true"));
  expect_equal("should encode False to 'false'",
      A2(&Json_encode, &int0, &False),
      create_string("false"));
  return NULL;
}

void* test_Json_encode_null() {
  expect_equal("should encode null to 'null'",
      A2(&Json_encode, &int0, WRAP(&Json_Value_null)),
      create_string("null"));
  return NULL;
}

void* test_Json_encode_number() {
  expect_equal("should encode 3.14 to '3.14'",
      A2(&Json_encode, &int0, WRAP(NEW_ELM_FLOAT(3.14))),
      create_string("3.14"));

  expect_equal("should encode 1e22 to '1e+22'",
      A2(&Json_encode, &int0, WRAP(NEW_ELM_FLOAT(1e22))),
      create_string("1e+22"));

  expect_equal("should encode `123 : Int` to '123'",
      A2(&Json_encode, &int0, WRAP(NEW_ELM_INT(123))),
      create_string("123"));
  return NULL;
}

void* test_Json_encode_string() {
  expect_equal("should encode 'hello' to '\"hello\"'",
      A2(&Json_encode, &int0, WRAP(create_string("hello"))),
      create_string("\"hello\""));

  expect_equal(
      "should encode "
      "' \\ \" \\r \\n \\t \\b \\f '"
      " to "
      "'\" \\\\ \\\" \\r \\n \\t \\b \\f \"'",
      A2(&Json_encode, &int0, WRAP(create_string(" \\ \" \r \n \t \b \f "))),
      create_string("\" \\\\ \\\" \\r \\n \\t \\b \\f \""));

  expect_equal("should encode ASCII control chars 0x7 and 0xb to '\"\\u0007\\u000b\"'",
      A2(&Json_encode, &int0, WRAP(create_string("\a\v"))),
      create_string("\"\\u0007\\u000b\""));

  return NULL;
}

void* test_Json_stringify_array() {
  char* expected;
  char buf[512];

  Custom* array123 = NEW_CUSTOM(JSON_VALUE_ARRAY,
      3,
      ((void*[]){
          NEW_ELM_FLOAT(1),
          NEW_ELM_FLOAT(2),
          NEW_ELM_FLOAT(3),
      }));

  expect_equal("encode 0 [1,2,3] == \"[1,2,3]\"",
      A2(&Json_encode, &int0, WRAP(array123)),
      create_string("[1,2,3]"));

  expected =
      "[\n"
      "  1,\n"
      "  2,\n"
      "  3\n"
      "]";
  sprintf(buf, "encode 2 [1,2,3] = \n%s", expected);
  expect_equal(
      buf, A2(&Json_encode, NEW_ELM_INT(2), WRAP(array123)), create_string(expected));

  expected =
      "[\n"
      "  [\n"
      "    1,\n"
      "    2,\n"
      "    3\n"
      "  ],\n"
      "  [\n"
      "    1,\n"
      "    2,\n"
      "    3\n"
      "  ]\n"
      "]";
  sprintf(buf, "encode 2 [[1,2,3]] = \n%s", expected);
  expect_equal(buf,
      A2(&Json_encode,
          NEW_ELM_INT(2),
          WRAP(NEW_CUSTOM(JSON_VALUE_ARRAY,
              2,
              ((void*[]){
                  array123,
                  array123,
              })))),
      create_string(expected));

  return NULL;
}

void* test_Json_stringify_object() {
  char* expected;
  char buf[512];

  Custom* exampleObj = NEW_CUSTOM(JSON_VALUE_OBJECT,
      6,
      ((void*[]){
          create_string("a"),
          NEW_ELM_FLOAT(1),
          create_string("b"),
          NEW_ELM_FLOAT(2),
          create_string("c"),
          NEW_ELM_FLOAT(3),
      }));

  expect_equal("encode 0 {a:1,b:2,c:3} == \"{\"a\":1,\"b\":2,\"c\":3}\"",
      A2(&Json_encode, &int0, WRAP(exampleObj)),
      create_string("{\"a\":1,\"b\":2,\"c\":3}"));

  expected =
      "{\n"
      "  \"a\": 1,\n"
      "  \"b\": 2,\n"
      "  \"c\": 3\n"
      "}";
  sprintf(buf, "encode 2 {a:1,b:2,c:3} = \n%s", expected);
  expect_equal(
      buf, A2(&Json_encode, NEW_ELM_INT(2), WRAP(exampleObj)), create_string(expected));

  expected =
      "{\n"
      "  \"x\": {\n"
      "    \"a\": 1,\n"
      "    \"b\": 2,\n"
      "    \"c\": 3\n"
      "  },\n"
      "  \"y\": {\n"
      "    \"a\": 1,\n"
      "    \"b\": 2,\n"
      "    \"c\": 3\n"
      "  }\n"
      "}";
  sprintf(buf, "encode 2 {x:{a:1,b:2,c:3},y:{a:1,b:2,c:3}} = \n%s", expected);
  expect_equal(buf,
      A2(&Json_encode,
          NEW_ELM_INT(2),
          WRAP(NEW_CUSTOM(JSON_VALUE_OBJECT,
              4,
              ((void*[]){
                  create_string("x"),
                  exampleObj,
                  create_string("y"),
                  exampleObj,
              })))),
      create_string(expected));

  return NULL;
}

void* eval_elm_json_Json_Encode_list(void* args[]) {
  void* x_func = args[0];
  void* x_entries = args[1];
  return A1(&Json_wrap,
      A3(&g_elm_core_List_foldl,
          A1(&Json_addEntry, x_func),
          A1(&Json_emptyArray, &Unit),
          x_entries));
}
Closure g_elm_json_Json_Encode_list = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_elm_json_Json_Encode_list,
};

void json_encoder_test() {
  if (verbose) {
    printf("\n");
    printf("Json.Encode\n");
    printf("-----------\n");
  }
  describe("test_Json_encode_boolean", test_Json_encode_boolean);
  describe("test_Json_encode_null", test_Json_encode_null);
  describe("test_Json_encode_number", test_Json_encode_number);
  describe("test_Json_encode_string", test_Json_encode_string);
  describe("test_Json_stringify_array", test_Json_stringify_array);
  describe("test_Json_stringify_object", test_Json_stringify_object);
  // describe("test_Json_Encode_list", test_Json_Encode_list);
  // describe("test_Json_encode_object", test_Json_encode_object);
}