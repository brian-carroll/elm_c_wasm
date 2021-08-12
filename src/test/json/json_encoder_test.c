#include "../../kernel/core/core.h"
#include "../../kernel/json/json-elm.h"
#include "../../kernel/json/json-internal.h"
#include "../../kernel/json/json.h"
#include "../test.h"

ElmInt int0 = {
    .header = HEADER_INT,
    .value = 0,
};

void test_Json_encode_boolean() {
  expect_equal("should encode True to 'true'",
      A2(&Json_encode, &int0, WRAP(&True)),
      create_string("true"));
  expect_equal("should encode False to 'false'",
      A2(&Json_encode, &int0, WRAP(&False)),
      create_string("false"));
}

void test_Json_encode_null() {
  expect_equal("should encode null to 'null'",
      A2(&Json_encode, &int0, WRAP(&Json_null)),
      create_string("null"));
}

void test_Json_encode_number() {
  expect_equal("should encode 3.14 to '3.14'",
      A2(&Json_encode, &int0, WRAP(newElmFloat(3.14))),
      create_string("3.14"));

  expect_equal("should encode 1e22 to '1e+22'",
      A2(&Json_encode, &int0, WRAP(newElmFloat(1e22))),
      create_string("1e+22"));

  expect_equal("should encode `123 : Int` to '123'",
      A2(&Json_encode, &int0, WRAP(newElmInt(123))),
      create_string("123"));
}

void test_Json_encode_string() {
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
}

void test_Json_stringify_array() {
  char* expected;
  char buf[512];

  Custom* array123 = newCustom(JSON_VALUE_ARRAY,
      3,
      ((void*[]){
          newElmFloat(1),
          newElmFloat(2),
          newElmFloat(3),
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
  stbsp_sprintf(buf, "encode 2 [1,2,3] = \n%s", expected);
  expect_equal(
      buf, A2(&Json_encode, newElmInt(2), WRAP(array123)), create_string(expected));

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
  stbsp_sprintf(buf, "encode 2 [[1,2,3]] = \n%s", expected);
  expect_equal(buf,
      A2(&Json_encode,
          newElmInt(2),
          WRAP(newCustom(JSON_VALUE_ARRAY,
              2,
              ((void*[]){
                  array123,
                  array123,
              })))),
      create_string(expected));
}

void test_Json_stringify_object() {
  char* expected;
  char buf[512];

  Custom* exampleObj = newCustom(JSON_VALUE_OBJECT,
      6,
      ((void*[]){
          create_string("a"),
          newElmFloat(1),
          create_string("b"),
          newElmFloat(2),
          create_string("c"),
          newElmFloat(3),
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
  stbsp_sprintf(buf, "encode 2 {a:1,b:2,c:3} = \n%s", expected);
  expect_equal(
      buf, A2(&Json_encode, newElmInt(2), WRAP(exampleObj)), create_string(expected));

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
  stbsp_sprintf(buf, "encode 2 {x:{a:1,b:2,c:3},y:{a:1,b:2,c:3}} = \n%s", expected);
  expect_equal(buf,
      A2(&Json_encode,
          newElmInt(2),
          WRAP(newCustom(JSON_VALUE_OBJECT,
              4,
              ((void*[]){
                  create_string("x"),
                  exampleObj,
                  create_string("y"),
                  exampleObj,
              })))),
      create_string(expected));
}

/*
 * Encode.list
 */
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

void test_Json_Encode_list() {
  void* c_array[] = {
      &True,
      &False,
  };
  Cons* list_bool = List_create(2, c_array);
  Custom* json_array = newCustom(JSON_VALUE_ARRAY, 2, c_array);

  Custom* encoded =
      A2(&g_elm_json_Json_Encode_list, &g_elm_json_Json_Encode_bool, list_bool);

  expect_equal("Encode.list Encode.bool [True,False] == JsonValueArray True False",
      encoded,
      WRAP(json_array));

  expect_equal("encode 0 (list bool [True,False]) == \"[true,false]\"",
      A2(&Json_encode, &int0, encoded),
      create_string("[true,false]"));
}

/*
 * Encode.object
 */
void* eval_elm_json_Json_Encode_object_lambda1(void* args[]) {
  void* x__v0 = args[0];
  void* x_obj = args[1];
  void* x_k = Utils_destruct_index(x__v0, 0);
  void* x_v = Utils_destruct_index(x__v0, 1);
  return A3(&Json_addField, x_k, x_v, x_obj);
}
void* eval_elm_json_Json_Encode_object(void* args[]) {
  void* x_pairs = args[0];
  Closure* tmp0 = newClosure(0, 2, &eval_elm_json_Json_Encode_object_lambda1, NULL);
  return A1(&Json_wrap,
      A3(&g_elm_core_List_foldl, tmp0, A1(&Json_emptyObject, &Unit), x_pairs));
}
Closure g_elm_json_Json_Encode_object = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_json_Json_Encode_object,
};

void test_Json_encode_object() {
  ElmString* name = create_string("name");
  ElmString* brian = create_string("Brian");
  ElmString* age = create_string("age");
  ElmInt int42 = {
      .header = HEADER_INT,
      .value = 42,
  };
  ElmFloat float42 = {
      .header = HEADER_FLOAT,
      .value = 42.0,
  };

  Cons* elm_pairs = List_create(2,
      ((void*[]){
          newTuple2(name, A1(&g_elm_json_Json_Encode_string, brian)),
          newTuple2(age, A1(&g_elm_json_Json_Encode_int, &int42)),
      }));

  Custom* json_object =
      WRAP(newCustom(JSON_VALUE_OBJECT, 4, ((void*[]){name, brian, age, &float42})));

  void* encodedObject = A1(&g_elm_json_Json_Encode_object, elm_pairs);

  expect_equal(
      "object [(\"name\", string \"Brian\"), (\"age\", int 42)]"
      " == JsonValueObject \"name\" \"Brian\" \"age\" 42.0",
      encodedObject,
      json_object);

  expect_equal(
      "encode 0 (object [(\"name\", string \"Brian\"), (\"age\", int 42)])"
      " == \"\"\"{\"name\":\"Brian\",\"age\":42}\"\"\"",
      A2(&Json_encode, &int0, encodedObject),
      create_string("{\"name\":\"Brian\",\"age\":42}"));
}

void json_encoder_test() {
  if (verbose) {
    safe_printf("\n");
    safe_printf("Json.Encode\n");
    safe_printf("-----------\n");
  }
  describe("test_Json_encode_boolean", test_Json_encode_boolean);
  describe("test_Json_encode_null", test_Json_encode_null);
  describe("test_Json_encode_number", test_Json_encode_number);
  describe("test_Json_encode_string", test_Json_encode_string);
  describe("test_Json_stringify_array", test_Json_stringify_array);
  describe("test_Json_stringify_object", test_Json_stringify_object);
  describe("test_Json_Encode_list", test_Json_Encode_list);
  describe("test_Json_encode_object", test_Json_encode_object);
}
