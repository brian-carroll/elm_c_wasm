#include <stddef.h>

#include "../../kernel/kernel.h"
#include "./../test.h"

ElmValue* parse_json(ElmString16* json);
void* Json_wrap(void*);

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

void* test_decode_err(Custom* decoder, char* json_c_str, Custom* decode_error) {
  sprintf(test_decode_buf, "should return expected error for '%s'", json_c_str);
  expect_equal(test_decode_buf,
      A2(&Json_runOnString, decoder, create_string(json_c_str)),
      NEW_CUSTOM(CTOR_Err, 1, ((void*[]){decode_error})));
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
Custom* errFailure(char* msg_c_str, char* json_c_str) {
  return NEW_CUSTOM(CTOR_Failure,
      2,
      ((void*[]){
          create_string(msg_c_str),
          parse_json(create_string(json_c_str)),
      }));
}
Custom* err(Custom* error) {
  return NEW_CUSTOM(CTOR_Err, 1, (void*){error});
}

//=================================================================================================

void* test_Json_decode_invalidJson() {
  char* json = "invalid JSON !";
  test_decode_err(&Json_decodeBool,
      json,
      NEW_CUSTOM(CTOR_Failure,
          2,
          ((void*[]){
              create_string("This is not valid JSON!"),
              Json_wrap(create_string(json)),
          })));

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

void json_decoder_test() {
  if (verbose) {
    printf("\n");
    printf("Json.decodeString\n");
    printf("-----------------\n");
  }

  describe("test_Json_decode_invalidJson", test_Json_decode_invalidJson);
  describe("test_Json_decodeBool", test_Json_decodeBool);
  describe("test_Json_decodeInt", test_Json_decodeInt);
}