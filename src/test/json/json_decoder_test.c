#include <stddef.h>

#include "../../kernel/kernel.h"
#include "./../test.h"

void* test_Json_decode_bool() {
  return NULL;
}


void json_decoder_test() {
  if (verbose) {
    printf("\n");
    printf("------------------------------------\n");
    printf("\n");
    printf("Json decoders\n");
    printf("------------\n");
  }

  describe("test_Json_decode_bool", test_Json_decode_bool);
}