#include "test.h"
#include "json/json_decoder_test.c"
#include "json/json_encoder_test.c"
#include "json/json_parser_test.c"

void json_parser_test();
void json_decoder_test();
void json_encoder_test();

void json_test() {
  if (verbose) {
    safe_printf("\n\n\n");
    safe_printf("####################################################\n");
    safe_printf("\n");
    safe_printf("Json\n");
    safe_printf("====\n");
  }

  json_parser_test();
  json_decoder_test();
  json_encoder_test();
}