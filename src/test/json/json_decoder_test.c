#include <stddef.h>

#include "../../kernel/kernel.h"
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
  expect_equal(test_decode_buf,
      A2(&Json_runOnString, decoder, json),
      NEW_CUSTOM(CTOR_Err,
          1,
          ((void*[]){
              NEW_CUSTOM(CTOR_Failure,
                  2,
                  ((void*[]){
                      create_string(msg_c_str),
                      parse_json(json),
                  })),
          })));
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
  ElmString16* field = create_string("myField");
  Custom* decoder = A2(&Json_decodeField, field, &Json_decodeInt);

  test_decode_ok(
      decoder, "{ \"a\": null, \"myField\": 123, \"b\": null }", NEW_ELM_INT(123));

  test_decode_errFailure(
      decoder, "null", "Expecting an OBJECT with a field named `myField`");

  test_decode_err(decoder,
      "{\"myField\":null}",
      err(errField("myField", errFailure("Expecting an INT", &Json_Value_null))));

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
}