#include "../../kernel/core/core.h"
#include "../../kernel/json/json.h"
#include "../test.h"

#define WRAP(ptr) A1(&Json_wrap, ptr)

void* test_Json_encodeBool() {
  expect_equal("should encode True to 'true'",
      A2(&Json_encode, NEW_ELM_INT(0), &True),
      create_string("true"));
  expect_equal("should encode False to 'false'",
      A2(&Json_encode, NEW_ELM_INT(0), &False),
      create_string("false"));
  return NULL;
}

void* test_Json_encodeNull() {
  expect_equal("should encode null to 'null'",
      A2(&Json_encode, NEW_ELM_INT(0), WRAP(&Json_Value_null)),
      create_string("null"));
  return NULL;
}

void* test_Json_encodeNumber() {
  expect_equal("should encode 3.14 to '3.14'",
      A2(&Json_encode, NEW_ELM_INT(0), WRAP(NEW_ELM_FLOAT(3.14))),
      create_string("3.14"));
  expect_equal("should encode `123 : Int` to '123'",
      A2(&Json_encode, NEW_ELM_INT(0), WRAP(NEW_ELM_INT(123))),
      create_string("123"));
  return NULL;
}

void json_encoder_test() {
  if (verbose) {
    printf("\n");
    printf("Json.Encode\n");
    printf("-----------\n");
  }
  describe("test_Json_encodeBool", test_Json_encodeBool);
  describe("test_Json_encodeNull", test_Json_encodeNull);
  describe("test_Json_encodeNumber", test_Json_encodeNumber);
}