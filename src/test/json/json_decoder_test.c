#include <stddef.h>

#include "../../kernel/kernel.h"
#include "./../test.h"

ElmString16 str_invalid_json;

void* test_Json_decode_invalidJson() {
  Custom* decoder = &Json_decodeBool;
  ElmString16* json = create_string("");
  Custom* result = A2(&Json_runOnString, decoder, json);
  Custom* failure = result->values[0];
  ElmString16* msg = failure->values[0];

  mu_expect_equal(
      "should return an `Err _`, given an empty string", result->ctor, CTOR_Err);

  mu_expect_equal("should return an `Err (Failure _ _)`, given an empty string",
      failure->ctor,
      CTOR_Failure);

  expect_equal("should return the expected error message, given an empty JSON string",
      msg,
      create_string("This is not valid JSON!"));

  return NULL;
}

// void* test_Json_decode_bool() {
//   Custom* decoder = &Json_decodeBool;

//   A2(&Json_runOnString, decoder, create_string(""));

//   return NULL;
// }

void json_decoder_test() {
  if (verbose) {
    printf("\n");
    printf("Json.decodeString\n");
    printf("-----------------\n");
  }

  describe("test_Json_decode_invalidJson", test_Json_decode_invalidJson);
  // describe("test_Json_decode_bool", test_Json_decode_bool);
}