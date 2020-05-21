#include "test.h"

void json_parser_test();
void json_decoder_test();
void json_encoder_test();

char* json_test() {
  if (verbose) {
    printf("\n\n\n");
    printf("####################################################\n");
    printf("\n");
    printf("Json\n");
    printf("====\n");
  }

  // json_parser_test();
  // json_decoder_test();
  json_encoder_test();

  return NULL;
}