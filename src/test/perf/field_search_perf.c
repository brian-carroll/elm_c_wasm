#include <stdlib.h>

#include "../../kernel/core/types.h"

FieldGroup fg2 = {.header = HEADER_FIELDGROUP(2), .size = 2, .fields = {485, 870}};
FieldGroup fg5 = {
    .header = HEADER_FIELDGROUP(5), .size = 5, .fields = {263, 343, 575, 691, 763}};
FieldGroup fg10 = {.header = HEADER_FIELDGROUP(10),
    .size = 10,
    .fields = {44, 123, 180, 443, 522, 532, 583, 645, 752, 816}};
FieldGroup fg20 = {.header = HEADER_FIELDGROUP(20),
    .size = 20,
    .fields = {
        54,
        129,
        137,
        313,
        367,
        406,
        414,
        423,
        443,
        610,
        622,
        652,
        705,
        710,
        722,
        741,
        799,
        871,
        937,
        984,
    }};
FieldGroup fg50 = {.header = HEADER_FIELDGROUP(50),
    .size = 50,
    .fields = {
        2,
        22,
        45,
        66,
        87,
        122,
        134,
        138,
        167,
        177,
        218,
        319,
        320,
        348,
        368,
        377,
        383,
        393,
        428,
        459,
        507,
        516,
        536,
        538,
        557,
        568,
        577,
        578,
        587,
        600,
        622,
        631,
        635,
        695,
        730,
        743,
        763,
        798,
        804,
        834,
        857,
        881,
        895,
        914,
        921,
        922,
        934,
        941,
        951,
        992,
    }};

FieldGroup fg100 = {.header = HEADER_FIELDGROUP(100),
    .size = 100,
    .fields = {
        1,
        28,
        33,
        34,
        43,
        50,
        63,
        64,
        65,
        73,
        93,
        99,
        126,
        155,
        168,
        170,
        175,
        196,
        204,
        208,
        216,
        217,
        228,
        236,
        241,
        248,
        262,
        289,
        297,
        302,
        320,
        331,
        333,
        339,
        360,
        362,
        362,
        367,
        379,
        380,
        382,
        394,
        400,
        401,
        410,
        412,
        430,
        431,
        439,
        448,
        453,
        479,
        480,
        489,
        491,
        522,
        534,
        544,
        546,
        558,
        558,
        559,
        560,
        574,
        579,
        585,
        589,
        590,
        612,
        629,
        640,
        659,
        670,
        672,
        699,
        716,
        740,
        744,
        747,
        747,
        756,
        757,
        763,
        779,
        787,
        802,
        816,
        828,
        829,
        836,
        876,
        883,
        888,
        935,
        947,
        964,
        965,
        982,
        983,
        994,
    }};

static u32 fieldgroup_search(FieldGroup* fieldgroup, u32 search) {
  u32 first = 0;
  u32 last = fieldgroup->size - 1;
  u32* array = fieldgroup->fields;

  while (first <= last) {
    u32 middle = (first + last) / 2;
    if (array[middle] == search) {
      return middle;
    } else if (array[middle] < search) {
      first = middle + 1;
    } else {
      last = middle - 1;
    }
  }

  return 0;
}

static u32 fieldgroup_seek(FieldGroup* fieldgroup, u32 search) {
  u32* array = fieldgroup->fields;

  for (size_t i = 0; i < fieldgroup->size; i++) {
    if (array[i] == search) return i;
  }

  return 0;
}

/**
 * Performance testing for record field search
 *
 * Question: Is binary search really the way to go?
 * Most record fieldgroups are small, so maybe hopping around
 * is actually bad for branch prediction and it's better to
 * do a simple linear search?
 *
 * Answer: No! Binary search wins!
 *   $ gcc -O3 ./field_search_perf.c
 *   $ time ./a.out 100000000 5 0 575
 *   real    0m0.120s
 *   user    0m0.109s
 *   sys     0m0.000s
 *   $ time ./a.out 100000000 5 1 575
 *   real    0m0.329s
 *   user    0m0.328s
 *   sys     0m0.000s
 */
int perf_main(int argc, char** argv) {
  int count;
  int size;
  int algo;
  int field;

  if (argc != 5) return 1;

  sscanf(argv[1], "%d", &count);
  sscanf(argv[2], "%d", &size);
  sscanf(argv[3], "%d", &algo);
  sscanf(argv[4], "%d", &field);

  FieldGroup* fg;
  switch (size) {
    case 2:
      fg = &fg2;
      break;
    case 5:
      fg = &fg5;
      break;
    case 10:
      fg = &fg10;
      break;
    case 20:
      fg = &fg20;
      break;
    case 50:
      fg = &fg50;
      break;
    case 100:
      fg = &fg100;
      break;
    default:
      return 2;
  }

  u32 field_pos;
  if (algo == 0) {
    for (int i = 0; i < count; i++) {
      field_pos = fieldgroup_search(fg, field);
    }
  } else {
    for (int i = 0; i < count; i++) {
      field_pos = fieldgroup_seek(fg, field);
    }
  }

  safe_printf("count %d size %d algo %d field %d: found in position %u\n",
      count,
      size,
      algo,
      field,
      field_pos);

  return 0;
}
