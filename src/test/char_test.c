#include "../kernel/kernel.h"
#include "./test.h"

/*
https://en.wikipedia.org/wiki/UTF-16#Examples

            Unicode     low      high
dollar  $	  0x0024      0x0024
euro    €	  0x20AC      0x20AC
deseret 𐐷   0x10437     0xD801  0xDC37
han     𤭢  0x24B62     0xD852  0xDF62
*/

char* test_Char_toCode() {
  // ElmChar dollar = (ElmChar){
  //     .header = HEADER_CHAR,
  //     .value = 0x0024,  // $
  // };
  // ElmInt* dollar_code = A1(&Char_toCode, &dollar);
  // mu_expect_equal("toCode \"$\" == 24", dollar_code->value, 0x0024);

  // ElmChar euro = (ElmChar){
  //     .header = HEADER_CHAR,
  //     .value = 0x20AC,  // €
  // };
  // ElmInt* euro_code = A1(&Char_toCode, &euro);
  // mu_expect_equal("toCode euro == 0x20AC", euro_code->value, 0x20AC);

  // ElmChar deseret = (ElmChar){
  //     .header = HEADER_CHAR,
  //     .value = 0x10437,  // 𐐷
  // };
  // ElmInt* deseret_code = A1(&Char_toCode, &deseret);
  // mu_expect_equal("toCode deseret == 0xDC37D801", deseret_code->value, 0xDC37D801);

  // ElmChar han = (ElmChar){
  //     .header = HEADER_CHAR,
  //     .value = 0x24B62,  // 𤭢
  // };
  // ElmInt* han_code = A1(&Char_toCode, &han);
  // mu_expect_equal("toCode han == 0xDF62D852", han_code->value, 0xDF62D852);

  return NULL;
}

char* char_test() {
  if (verbose) {
    printf("\n\n\n");
    printf("####################################################\n");
    printf("\n");
    printf("Char\n");
    printf("------\n");
  }

  mu_run_test(test_Char_toCode);

  return NULL;
}
