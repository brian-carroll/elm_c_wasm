#include <stddef.h>

#include "../../kernel/core/core.h"
#include "../../kernel/json/json.h"
#include "../test.h"

ElmValue* parse_json(ElmString16* json);
void* wrap(void*);

//=================================================================================================
// Test helpers

char test_decode_buf[1024];

void* test_decode_ok(Custom* decoder, char* json_c_str, void* expected) {
  sprintf(test_decode_buf, "should correctly decode '%s'", json_c_str);
  expect_equal(test_decode_buf,
      A2(&Json_runOnString, decoder, create_string(json_c_str)),
      NEW_CUSTOM(CTOR_Ok, 1, (void*[]){expected}));
  return NULL;
}

void* test_decode_err(Custom* decoder, char* json_c_str, Custom* expected_err) {
  sprintf(test_decode_buf, "should return expected error for '%s'", json_c_str);
  void* actual_err = A2(&Json_runOnString, decoder, create_string(json_c_str));
  // print_heap();
  // Debug_pretty("actual", actual_err);
  // Debug_pretty("expected", expected_err);
  expect_equal(test_decode_buf, actual_err, expected_err);
  return NULL;
}

void* test_decode_errFailure(Custom* decoder, char* json_c_str, char* msg_c_str) {
  ElmString16* json = create_string(json_c_str);
  sprintf(test_decode_buf, "should return expected error for '%s'", json_c_str);
  void* actual_err = A2(&Json_runOnString, decoder, json);
  void* expected_err = NEW_CUSTOM(CTOR_Err,
      1,
      ((void*[]){
          NEW_CUSTOM(CTOR_Failure,
              2,
              ((void*[]){
                  create_string(msg_c_str),
                  parse_json(json),
              })),
      }));
  // print_heap();
  // Debug_pretty("actual", actual_err);
  // Debug_pretty("expected", expected_err);
  expect_equal(test_decode_buf, actual_err, expected_err);
  return NULL;
}

Custom* errField(char* field, Custom* error) {
  return NEW_CUSTOM(CTOR_Field, 2, ((void*[]){create_string(field), error}));
}
Custom* errIndex(u32 idx, Custom* error) {
  return NEW_CUSTOM(CTOR_Index, 2, ((void*[]){NEW_ELM_INT(idx), error}));
}
Custom* errOneOf(Cons* errors) {
  return NEW_CUSTOM(CTOR_OneOf, 1, (void*){errors});
}
Custom* errFailure(char* msg_c_str, void* json_value) {
  return NEW_CUSTOM(CTOR_Failure,
      2,
      ((void*[]){
          create_string(msg_c_str),
          json_value,
      }));
}
Custom* err(Custom* error) {
  return NEW_CUSTOM(CTOR_Err, 1, (void*[]){error});
}

//=================================================================================================

void* test_Json_decode_invalidJson() {
  char* json = "invalid JSON !";
  test_decode_err(&Json_decodeBool,
      json,
      err(NEW_CUSTOM(CTOR_Failure,
          2,
          ((void*[]){
              create_string("This is not valid JSON!"),
              wrap(create_string(json)),
          }))));

  return NULL;
}

void* test_Json_decodeBool() {
  test_decode_ok(&Json_decodeBool, "true", &True);
  test_decode_ok(&Json_decodeBool, "false", &False);

  test_decode_errFailure(&Json_decodeBool, "null", "Expecting a BOOL");

  return NULL;
}

void* test_Json_decodeInt() {
  test_decode_ok(&Json_decodeInt, "123", NEW_ELM_INT(123));
  test_decode_ok(&Json_decodeInt, "-123", NEW_ELM_INT(-123));

  test_decode_errFailure(&Json_decodeInt, "null", "Expecting an INT");
  test_decode_errFailure(&Json_decodeInt, "9876543210", "Expecting an INT");
  test_decode_errFailure(&Json_decodeInt, "3.14", "Expecting an INT");

  return NULL;
}

void* test_Json_decodeFloat() {
  test_decode_ok(&Json_decodeFloat, "3.14", NEW_ELM_FLOAT(3.14));
  test_decode_ok(&Json_decodeFloat, "-3.14", NEW_ELM_FLOAT(-3.14));
  test_decode_ok(&Json_decodeFloat, "-3.14e-1", NEW_ELM_FLOAT(-0.314));

  test_decode_errFailure(&Json_decodeFloat, "null", "Expecting a FLOAT");

  return NULL;
}

void* test_Json_decodeString() {
  test_decode_ok(&Json_decodeString, "\"hello\"", create_string("hello"));
  test_decode_errFailure(&Json_decodeString, "null", "Expecting a STRING");
  return NULL;
}

void* test_Json_decodeValue() {
  test_decode_ok(&Json_decodeValue,
      "[1,2,3]",
      wrap(NEW_CUSTOM(JSON_VALUE_ARRAY,
          3,
          ((void*[]){
              NEW_ELM_FLOAT(1),
              NEW_ELM_FLOAT(2),
              NEW_ELM_FLOAT(3),
          }))));
  return NULL;
}

void* test_Json_decodeList() {
  Custom* decoder = A1(&Json_decodeList, &Json_decodeInt);

  test_decode_ok(decoder,
      "[1,2,3]",
      List_create(3,
          ((void*[]){
              NEW_ELM_INT(1),
              NEW_ELM_INT(2),
              NEW_ELM_INT(3),
          })));

  test_decode_errFailure(decoder, "null", "Expecting a LIST");

  test_decode_err(decoder,
      "[null]",
      err(errIndex(0, errFailure("Expecting an INT", &Json_Value_null))));

  return NULL;
}

void* test_Json_decodeArray() {
  Custom* decoder = A1(&Json_decodeArray, &Json_decodeInt);

  test_decode_ok(decoder,
      "[0,1,2]",
      A2(&g_elm_core_Array_initialize, NEW_ELM_INT(3), &Basics_identity));

  test_decode_errFailure(decoder, "null", "Expecting an ARRAY");

  test_decode_err(decoder,
      "[null]",
      err(errIndex(0, errFailure("Expecting an INT", &Json_Value_null))));

  return NULL;
}

void* test_Json_decodeField() {
  char* fld = "myField";
  if (verbose) {
    printf("field = \"%s\"\n", fld);
  }
  ElmString16* field = create_string(fld);
  Custom* decoder = A2(&Json_decodeField, field, &Json_decodeInt);

  test_decode_ok(
      decoder, "{ \"a\": null, \"myField\": 123, \"b\": null }", NEW_ELM_INT(123));

  test_decode_errFailure(
      decoder, "null", "Expecting an OBJECT with a field named `myField`");

  test_decode_err(decoder,
      "{\"myField\":null}",
      err(errField(fld, errFailure("Expecting an INT", &Json_Value_null))));

  return NULL;
}

void* test_Json_decodeIndex() {
  const i32 idx = 2;
  if (verbose) {
    printf("index = %d\n", idx);
  }
  ElmInt* index = NEW_ELM_INT(idx);
  Custom* decoder = A2(&Json_decodeIndex, index, &Json_decodeInt);

  test_decode_ok(decoder, "[null,\"hello\",123,{}]", NEW_ELM_INT(123));

  test_decode_errFailure(decoder, "null", "Expecting an ARRAY");

  test_decode_errFailure(decoder,
      "[123,456]",
      "Expecting a LONGER array. Need index 2 but only see 2 entries");

  test_decode_err(decoder,
      "[{},\"hello\",null,123]",
      err(errIndex(idx, errFailure("Expecting an INT", &Json_Value_null))));

  return NULL;
}

void* test_Json_decodeKeyValuePairs() {
  Custom* decoder = A1(&Json_decodeKeyValuePairs, &Json_decodeInt);

  test_decode_ok(decoder,
      "{ \"a\": 1, \"b\": 2, \"c\": 3 }",
      List_create(3,
          ((void*[]){
              NEW_TUPLE2(create_string("a"), NEW_ELM_INT(1)),
              NEW_TUPLE2(create_string("b"), NEW_ELM_INT(2)),
              NEW_TUPLE2(create_string("c"), NEW_ELM_INT(3)),
          })));

  test_decode_errFailure(decoder, "null", "Expecting an OBJECT");

  test_decode_err(decoder,
      "{ \"a\": 1, \"b\": null, \"c\": 3 }",
      err(errField("b", errFailure("Expecting an INT", &Json_Value_null))));

  return NULL;
}

ElmInt mapArgs = {
    .header = HEADER_INT,
    .value = 0,
};
void* eval_testMapper(void* args[]) {
  ElmInt* nArgs = args[0];
  i32 total = 0;
  for (i32 i = 0; i < nArgs->value; i++) {
    ElmInt* arg = args[i + 1];
    total += arg->value;
  }
  return NEW_ELM_INT(total);
}
Closure testMapper = {
    .header = HEADER_CLOSURE(0),
    .max_values = 1,  // need to mutate this!
    .n_values = 1,
    .evaluator = &eval_testMapper,
    .values = {&mapArgs},
};

void* test_Json_map() {
  Closure* mapFns[] = {
      &Json_map1,
      &Json_map2,
      &Json_map3,
      &Json_map4,
      &Json_map5,
      &Json_map6,
      &Json_map7,
      &Json_map8,
  };

  i32 expect = 0;
  char json[1024];
  json[0] = '{';
  size_t json_index = 1;

  for (i32 i = 0; i <= 7; i++) {
    Closure* mapFn = mapFns[i];
    u32 n_args = i + 1;

    json_index += sprintf(&json[json_index], "\"%c\":%d", 'a' + i, i + 1);
    json[json_index] = '}';
    json[json_index + 1] = 0;

    testMapper.max_values = 1 + n_args;
    mapArgs.value = n_args;

    void* args[9];
    args[0] = &testMapper;
    for (size_t j = 0; j < n_args; j++) {
      char field[2];
      field[0] = 'a' + j;
      field[1] = 0;
      args[j + 1] = A2(&Json_decodeField, create_string(field) , &Json_decodeInt);
    }
    void* decoder = Utils_apply(mapFn, n_args + 1, args);

    expect += n_args;
    test_decode_ok(decoder, json, NEW_ELM_INT(expect));

    json[json_index] = ',';
    json_index++;
  }

  return NULL;
}

void* test_Json_andThen() {
  // Custom* decoder = A1(&Json_andThen, );
  mu_assert("TODO", 0);
  return NULL;
}

void* test_Json_oneOf() {
  // Custom* decoder = A1(&Json_andThen, );
  mu_assert("TODO", 0);
  return NULL;
}

void* test_Json_fail() {
  char* msg = "it failed";
  Custom* decoder = A1(&Json_fail, create_string(msg));
  test_decode_errFailure(decoder, "42", msg);
  test_decode_errFailure(decoder, "[1,2,3]", msg);

  return NULL;
}

void* test_Json_succeed() {
  ElmInt* msg = NEW_ELM_INT(42);
  Custom* decoder = A1(&Json_succeed, msg);

  test_decode_ok(decoder, "true", msg);
  test_decode_ok(decoder, "[1,2,3]", msg);

  return NULL;
}

void json_decoder_test() {
  if (verbose) {
    printf("\n");
    printf("Json.decodeString\n");
    printf("-----------------\n");
  }

  describe("test_Json_decode_invalidJson", test_Json_decode_invalidJson);
  describe("test_Json_decodeBool", test_Json_decodeBool);
  describe("test_Json_decodeInt", test_Json_decodeInt);
  describe("test_Json_decodeFloat", test_Json_decodeFloat);
  describe("test_Json_decodeString", test_Json_decodeString);
  describe("test_Json_decodeValue", test_Json_decodeValue);
  describe("test_Json_decodeList", test_Json_decodeList);
  describe("test_Json_decodeArray", test_Json_decodeArray);
  describe("test_Json_decodeField", test_Json_decodeField);
  describe("test_Json_decodeIndex", test_Json_decodeIndex);
  describe("test_Json_decodeKeyValuePairs", test_Json_decodeKeyValuePairs);
  describe("test_Json_map", test_Json_map);
  describe("test_Json_andThen", test_Json_andThen);
  describe("test_Json_oneOf", test_Json_oneOf);
  describe("test_Json_fail", test_Json_fail);
  describe("test_Json_succeed", test_Json_succeed);
}