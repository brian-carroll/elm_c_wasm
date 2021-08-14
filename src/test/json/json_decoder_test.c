
#include "../../kernel/core/core.h"
#include "../../kernel/json/json.h"
#include "../test.h"

void* parse_json(ElmString* json);
#define WRAP(ptr) A1(&Json_wrap, ptr)

//=================================================================================================
// Test helpers

char test_decode_buf[1024];

i32 latest_jsref_index = -1;
void* createJsValue(ElmString* json) {
  Custom* wrapped = testJsonValueRoundTrip(json);
  JsRef* jsRef = wrapped->values[0];
  if (jsRef->header.tag == Tag_JsRef) {
    latest_jsref_index = jsRef->id;
  }
  return wrapped;
}

void test_decode_ok(Closure* runner, Custom* decoder, char* json_c_str, void* expected) {
  stbsp_sprintf(test_decode_buf, "should correctly decode '%s'", json_c_str);
  ElmString* json = create_string(json_c_str);

  void* actual;
  if (runner == &Json_runOnString) {
    actual = A2(&Json_runOnString, decoder, json);
  } else {
    void* wrappedJsRef = createJsValue(json);
    actual = A2(&Json_run, decoder, wrappedJsRef);
  }

  expect_equal(test_decode_buf, actual, newCustom(CTOR_Ok, 1, (void*[]){expected}));
}

void test_decode_err(
    Closure* runner, Custom* decoder, char* json_c_str, Custom* expected_err) {
  stbsp_sprintf(test_decode_buf, "should return expected error for '%s'", json_c_str);
  ElmString* json = create_string(json_c_str);

  void* actual_err;
  if (runner == &Json_runOnString) {
    actual_err = A2(&Json_runOnString, decoder, json);
  } else {
    actual_err = A2(&Json_run, decoder, createJsValue(json));
  }

  expect_equal(test_decode_buf, actual_err, expected_err);
}

void test_decode_errFailure(
    Closure* runner, Custom* decoder, char* json_c_str, char* msg_c_str) {
  ElmString* json = create_string(json_c_str);
  stbsp_sprintf(test_decode_buf, "should return expected error for '%s'", json_c_str);

  void* actual_err = runner == &Json_runOnString
                         ? A2(&Json_runOnString, decoder, json)
                         : A2(&Json_run, decoder, createJsValue(json));

  void* expected_err = newCustom(CTOR_Err,
      1,
      ((void*[]){
          newCustom(CTOR_Failure,
              2,
              ((void*[]){
                  create_string(msg_c_str),
                  WRAP(parse_json(json)),
              })),
      }));
  expect_equal(test_decode_buf, actual_err, expected_err);
}

Custom* errField(char* field, Custom* error) {
  return newCustom(CTOR_Field, 2, ((void*[]){create_string(field), error}));
}
Custom* errIndex(u32 idx, Custom* error) {
  return newCustom(CTOR_Index, 2, ((void*[]){newElmInt(idx), error}));
}
Custom* errOneOf(Cons* errors) {
  return newCustom(CTOR_OneOf, 1, (void*[]){errors});
}
Custom* errFailure(char* msg_c_str, void* json_value) {
  return newCustom(CTOR_Failure,
      2,
      ((void*[]){
          create_string(msg_c_str),
          WRAP(json_value),
      }));
}
Custom* err(Custom* error) {
  return newCustom(CTOR_Err, 1, (void*[]){error});
}


//=================================================================================================

void test_Json_decode_invalidJson() {
  Closure* runner = &Json_runOnString;
  char* json = "invalid JSON !";

  Custom* expectedError = err(newCustom(CTOR_Failure,
      2,
      ((void*[]){
          create_string("This is not valid JSON!"),
          WRAP(create_string(json)),
      })));

  test_decode_err(runner, &Json_decodeBool, json, expectedError);
}

void test_Json_decodeBool(void* runner) {
  test_decode_ok(runner, &Json_decodeBool, "true", &True);
  test_decode_ok(runner, &Json_decodeBool, "false", &False);

  test_decode_errFailure(runner, &Json_decodeBool, "null", "Expecting a BOOL");
}

void test_Json_decodeInt(void* runner) {
  test_decode_ok(runner, &Json_decodeInt, "123", newElmInt(123));
  test_decode_ok(runner, &Json_decodeInt, "-123", newElmInt(-123));

  test_decode_errFailure(runner, &Json_decodeInt, "null", "Expecting an INT");
  test_decode_errFailure(runner, &Json_decodeInt, "9876543210", "Expecting an INT");
  test_decode_errFailure(runner, &Json_decodeInt, "3.14", "Expecting an INT");
}

void test_Json_decodeFloat(void* runner) {
  test_decode_ok(runner, &Json_decodeFloat, "3.14", newElmFloat(3.14));
  test_decode_ok(runner, &Json_decodeFloat, "-3.14", newElmFloat(-3.14));
  test_decode_ok(runner, &Json_decodeFloat, "-3.14e-1", newElmFloat(-0.314));

  test_decode_errFailure(runner, &Json_decodeFloat, "null", "Expecting a FLOAT");
}

void test_Json_decodeString(void* runner) {
  test_decode_ok(runner, &Json_decodeString, "\"hello\"", create_string("hello"));
  test_decode_errFailure(runner, &Json_decodeString, "null", "Expecting a STRING");
}

void test_Json_decodeNull(void* runner) {
  ElmInt* num = newElmInt(42);
  Custom* decoder = A1(&Json_decodeNull, num);
  test_decode_ok(runner, decoder, "null", num);
  test_decode_errFailure(runner, decoder, "123", "Expecting null");
}

void test_Json_decodeValue(void* runner) {
  void* expected = runner == &Json_runOnString ? (void*)newCustom(JSON_VALUE_ARRAY,
                                                     3,
                                                     ((void*[]){
                                                         newElmFloat(1),
                                                         newElmFloat(2),
                                                         newElmFloat(3),
                                                     }))
                                               : (void*)newJsRef(latest_jsref_index + 1);
  test_decode_ok(runner, &Json_decodeValue, "[1,2,3]", WRAP(expected));
}

void test_Json_decodeList(void* runner) {
  Custom* decoder = A1(&Json_decodeList, &Json_decodeInt);

  test_decode_ok(runner,
      decoder,
      "[1,2,3]",
      List_create(3,
          ((void*[]){
              newElmInt(1),
              newElmInt(2),
              newElmInt(3),
          })));

  test_decode_errFailure(runner, decoder, "null", "Expecting a LIST");

  test_decode_err(runner,
      decoder,
      "[null]",
      err(errIndex(0, errFailure("Expecting an INT", &Json_null))));
}

void test_Json_decodeArray(void* runner) {
  Custom* decoder = A1(&Json_decodeArray, &Json_decodeInt);

  test_decode_ok(runner,
      decoder,
      "[0,1,2]",
      A2(&g_elm_core_Array_initialize, newElmInt(3), &Basics_identity));

  test_decode_errFailure(runner, decoder, "null", "Expecting an ARRAY");

  test_decode_err(runner,
      decoder,
      "[null]",
      err(errIndex(0, errFailure("Expecting an INT", &Json_null))));
}

void test_Json_decodeField(void* runner) {
  char* fld = "myField";
  if (verbose) {
    safe_printf("field = \"%s\"\n", fld);
  }
  ElmString* field = create_string(fld);
  Custom* decoder = A2(&Json_decodeField, field, &Json_decodeInt);

  test_decode_ok(
      runner, decoder, "{ \"a\": null, \"myField\": 123, \"b\": null }", newElmInt(123));

  test_decode_errFailure(
      runner, decoder, "null", "Expecting an OBJECT with a field named `myField`");

  test_decode_err(runner,
      decoder,
      "{\"myField\":null}",
      err(errField(fld, errFailure("Expecting an INT", &Json_null))));
}

void test_Json_decodeIndex(void* runner) {
  const i32 idx = 2;
  if (verbose) {
    safe_printf("index = %d\n", idx);
  }
  ElmInt* index = newElmInt(idx);
  Custom* decoder = A2(&Json_decodeIndex, index, &Json_decodeInt);

  test_decode_ok(runner, decoder, "[null,\"hello\",123,{}]", newElmInt(123));

  test_decode_errFailure(runner, decoder, "null", "Expecting an ARRAY");

  void* too_short;
  if (runner == &Json_runOnString) {
    too_short = parse_json(create_string("[123,456]"));
  } else {
    too_short = newJsRef(latest_jsref_index + 1);
  }
  test_decode_err(runner,
      decoder,
      "[123,456]",
      err(errFailure(
          "Expecting a LONGER array. Need index 2 but only see 2 entries", too_short)));

  test_decode_err(runner,
      decoder,
      "[{},\"hello\",null,123]",
      err(errIndex(idx, errFailure("Expecting an INT", &Json_null))));
}

void test_Json_decodeKeyValuePairs(void* runner) {
  Custom* decoder = A1(&Json_decodeKeyValuePairs, &Json_decodeInt);

  test_decode_ok(runner,
      decoder,
      "{ \"a\": 1, \"b\": 2, \"c\": 3 }",
      List_create(3,
          ((void*[]){
              newTuple2(create_string("a"), newElmInt(1)),
              newTuple2(create_string("b"), newElmInt(2)),
              newTuple2(create_string("c"), newElmInt(3)),
          })));

  test_decode_errFailure(runner, decoder, "null", "Expecting an OBJECT");

  test_decode_err(runner,
      decoder,
      "{ \"a\": 1, \"b\": null, \"c\": 3 }",
      err(errField("b", errFailure("Expecting an INT", &Json_null))));
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
  return newElmInt(total);
}
Closure testMapper = {
    .header = HEADER_CLOSURE(0),
    .max_values = 1,  // need to mutate this!
    .n_values = 1,
    .evaluator = &eval_testMapper,
    .values = {&mapArgs},
};

void test_Json_map(void* runner) {
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

    json_index += stbsp_sprintf(&json[json_index], "\"%c\":%d", 'a' + i, i + 1);
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
      args[j + 1] = A2(&Json_decodeField, create_string(field), &Json_decodeInt);
    }
    void* decoder = Utils_apply(mapFn, n_args + 1, args);

    expect += n_args;
    test_decode_ok(runner, decoder, json, newElmInt(expect));

    json[json_index] = ',';
    json_index++;
  }
}

void* eval_selectField(void* args[]) {
  ElmString* name = args[0];
  return A2(&Json_decodeField, name, &Json_decodeInt);
}
Closure selectField = {
    .header = HEADER_CLOSURE(0),
    .max_values = 1,
    .evaluator = &eval_selectField,
};

void test_Json_andThen(void* runner) {
  /*
    decoder : Decoder Int
    decoder =
      field "dataField" string
        |> andThen selectField

    selectField : String -> Decoder Int
    selectField name =
        field name int

    decodeString decoder """{ "dataField": "foo", "foo": 123 }""" == Ok 123
    decodeString decoder """{ "dataField": "bar", "bar": 456 }""" == Ok 456
  */
  Custom* decoder = A2(&Json_andThen,
      &selectField,
      A2(&Json_decodeField, create_string("dataField"), &Json_decodeString));

  test_decode_ok(
      runner, decoder, "{ \"dataField\": \"foo\", \"foo\": 123 }", newElmInt(123));

  test_decode_ok(
      runner, decoder, "{ \"dataField\": \"bar\", \"bar\": 456 }", newElmInt(456));

  test_decode_errFailure(
      runner, decoder, "null", "Expecting an OBJECT with a field named `dataField`");

  test_decode_err(runner,
      decoder,
      "{ \"dataField\": null }",
      err(errField("dataField", errFailure("Expecting a STRING", &Json_null))));

  test_decode_err(runner,
      decoder,
      "{ \"dataField\": \"foo\", \"foo\": null }",
      err(errField("foo", errFailure("Expecting an INT", &Json_null))));
}

void test_Json_oneOf(void* runner) {
  /*
    decoder : Decoder Int
    decoder =
      oneOf [ int, null 0 ]
  */
  if (verbose) {
    safe_printf("decoder = oneOf [ int, null 0 ]\n");
  }

  Custom* decoder = A1(&Json_oneOf,
      List_create(2,
          ((void*[]){
              &Json_decodeInt,
              A1(&Json_decodeNull, newElmInt(0)),
          })));

  test_decode_ok(runner, decoder, "123", newElmInt(123));
  test_decode_ok(runner, decoder, "null", newElmInt(0));

  void* empty_array = runner == &Json_runOnString
                          ? (void*)newCustom(JSON_VALUE_ARRAY, 0, NULL)
                          : (void*)newJsRef(latest_jsref_index + 1);

  test_decode_err(runner,
      decoder,
      "[]",
      err(errOneOf(List_create(2,
          ((void*[]){
              errFailure("Expecting an INT", empty_array),
              errFailure("Expecting null", empty_array),
          })))));
}

void test_Json_fail(void* runner) {
  char* msg = "it failed";
  Custom* decoder = A1(&Json_fail, create_string(msg));
  test_decode_errFailure(runner, decoder, "42", msg);

  if (runner == &Json_runOnString) {
    test_decode_errFailure(runner, decoder, "[1,2,3]", msg);
  } else {
    test_decode_err(runner,
        decoder,
        "[1,2,3]",
        err(errFailure(msg, newJsRef(latest_jsref_index + 1))));
  }
}

void test_Json_succeed(void* runner) {
  ElmInt* msg = newElmInt(42);
  Custom* decoder = A1(&Json_succeed, msg);

  test_decode_ok(runner, decoder, "true", msg);
  test_decode_ok(runner, decoder, "[1,2,3]", msg);
}

void json_runner_test(void* runner) {
  describe_arg("test_Json_decodeBool", test_Json_decodeBool, runner);
  describe_arg("test_Json_decodeInt", test_Json_decodeInt, runner);
  describe_arg("test_Json_decodeFloat", test_Json_decodeFloat, runner);
  describe_arg("test_Json_decodeString", test_Json_decodeString, runner);
  describe_arg("test_Json_decodeNull", test_Json_decodeNull, runner);
  describe_arg("test_Json_decodeValue", test_Json_decodeValue, runner);
  describe_arg("test_Json_decodeList", test_Json_decodeList, runner);
  describe_arg("test_Json_decodeArray", test_Json_decodeArray, runner);
  describe_arg("test_Json_decodeField", test_Json_decodeField, runner);
  describe_arg("test_Json_decodeIndex", test_Json_decodeIndex, runner);
  describe_arg("test_Json_decodeKeyValuePairs", test_Json_decodeKeyValuePairs, runner);
  describe_arg("test_Json_map", test_Json_map, runner);
  describe_arg("test_Json_andThen", test_Json_andThen, runner);
  describe_arg("test_Json_oneOf", test_Json_oneOf, runner);
  describe_arg("test_Json_fail", test_Json_fail, runner);
  describe_arg("test_Json_succeed", test_Json_succeed, runner);
}

void json_decoder_test() {
  if (verbose) {
    safe_printf("\n");
    safe_printf("Json.Decode.decodeString\n");
    safe_printf("------------------------\n");
  }
  describe("test_Json_decode_invalidJson", test_Json_decode_invalidJson);
  json_runner_test(&Json_runOnString);

  if (verbose) {
    safe_printf("\n");
    safe_printf("Json.Decode.decodeValue\n");
    safe_printf("------------------------\n");
  }
  json_runner_test(&Json_run);
}