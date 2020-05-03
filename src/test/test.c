#include "./test.h"

#include <getopt.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../kernel/kernel.h"
#include "./gc_test.h"

char* types_test();
char* utils_test();
char* basics_test();
char* string_test();
char* char_test();
char* list_test();

int verbose = false;
int tests_run = 0;
int tests_failed = 0;
int assertions_made = 0;


// ---------------------------------------------------------
// 
//  "Compiler-generated" values
// 
// ---------------------------------------------------------

enum {
  CTOR_Nothing,
  CTOR_Just,
};
void* eval_elm_core_Maybe_Just(void* args[]) {
  return ctorCustom(CTOR_Just, 1, args);
}
Closure g_elm_core_Maybe_Just = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_core_Maybe_Just,
};
Custom g_elm_core_Maybe_Nothing = {
    .header = HEADER_CUSTOM(0),
    .ctor = CTOR_Nothing,
};

FieldGroup* Wrapper_appFieldGroups[] = {NULL};
void** Wrapper_mainsArray[] = {NULL};

char Debug_evaluator_name_buf[1024];
char* Debug_evaluator_name(void* p) {
  sprintf(Debug_evaluator_name_buf, "%p", p);
  return Debug_evaluator_name_buf;
}
char* Debug_ctors[] = {};
char* Debug_fields[] = {};
char* Debug_jsValues[] = {};
int Debug_ctors_size = 0;
int Debug_fields_size = 0;
int Debug_jsValues_size = 0;


// ---------------------------------------------------------
//
//                TESTING UTILITIES
//
// Till now I've been using minunit
// But now I'm finding it too minimal so I'm switching to this
//
// ---------------------------------------------------------

char* test_description;
void* test_heap_ptr;

void describe(char* description, void* (*test)()) {
  tests_run++;
  test_description = description;
  test_heap_ptr = GC_malloc(0);
  if (verbose) {
    printf("\n%s\n", description);
  }
  if (test() == pGcFull) {
    fprintf(stderr, "Heap overflow in test \"%s\"\n", description);
    print_heap();
  }
}

void* expect_equal(char* expect_description, void* left, void* right) {
  bool ok = A2(&Utils_equal, left, right) == &True;
  if (!ok) {
    if (!verbose) {
      printf("\n%s\n", test_description);
    }
    printf("FAIL: %s\n", expect_description);
    printf("Left: %p\n", left);
    printf("Right: %p\n", right);
    print_heap_range(test_heap_ptr, GC_malloc(0));
    printf("\n");
    tests_failed++;
  } else if (verbose) {
    printf("PASS: %s\n", expect_description);
  }
  assertions_made++;
  return NULL;
}

// Debug function, with pre-allocated memory for strings
// Avoiding use of malloc in test code in case it screws up GC
// A single printf may require many separate hex strings
#define TEST_MAX_HEXES_PER_PRINTF 16
char hex_strings[TEST_MAX_HEXES_PER_PRINTF][9 * 1024 / 4];
char* hex(void* addr, int size) {
  static u32 rotate = 0;
  rotate = (rotate + 1) % TEST_MAX_HEXES_PER_PRINTF;

  size = (size < 1024) ? size : 1024;
  u32 i, c = 0;
  for (i = 0; i < size; c += 9, i += 4) {
    // Print in actual byte order (little endian)
    sprintf(hex_strings[rotate] + c,
        "%02x%02x%02x%02x|",
        *(u8*)(addr + i),
        *(u8*)(addr + i + 1),
        *(u8*)(addr + i + 2),
        *(u8*)(addr + i + 3));
  }
  hex_strings[rotate][c - 1] = 0;  // erase last "|" and terminate the string
  return hex_strings[rotate];
}

// Print a memory address held in a pointer
//   Addresses are printed in little-endian format
//   => Easier to see what pointers are pointing to it
//   => Harder to subtract in your head if you're interested in sizes & memory layout
char* hex_ptr(void* ptr) {
  return hex(&ptr, sizeof(void*));
}

char* test_all(
    bool types, bool utils, bool basics, bool string, bool chr, bool list, bool gc) {
  if (verbose) {
    printf("Selected tests: ");
    if (types) printf("types ");
    if (utils) printf("utils ");
    if (basics) printf("basics ");
    if (string) printf("string ");
    if (chr) printf("char ");
    if (list) printf("list ");
    if (gc) printf("gc ");
    printf("\n\n");
  }
  if (types) mu_run_test(types_test);
  if (utils) mu_run_test(utils_test);
  if (basics) mu_run_test(basics_test);
  if (string) mu_run_test(string_test);
  if (chr) mu_run_test(char_test);
  if (list) mu_run_test(list_test);
  if (gc) mu_run_test(gc_test);

  return NULL;
}

int main(int argc, char** argv) {
  GC_init();

  static struct option long_options[] = {
      {"verbose", no_argument, NULL, 'v'},
      {"all", no_argument, NULL, 'a'},
      {"types", no_argument, NULL, 't'},
      {"utils", no_argument, NULL, 'u'},
      {"basics", no_argument, NULL, 'b'},
      {"string", no_argument, NULL, 's'},
      {"char", no_argument, NULL, 'c'},
      {"list", no_argument, NULL, 'l'},
      {"gc", no_argument, NULL, 'g'},
      {NULL, 0, NULL, 0},
  };

  // By default in Bash shell, just do what's specified
  bool types = false;
  bool basics = false;
  bool string = false;
  bool chr = false;
  bool utils = false;
  bool list = false;
  bool gc = false;

// Running in a Windows CMD shell
// Prob via Codelite IDE. Dunno how to set up CLI args there yet!
// Set this up for whatever I'm working on
#if defined(_WIN32)
  gc = true;
  verbose = true;
#endif

  char options[] = "vatubsclg";

  int opt;
  while ((opt = getopt_long(argc, argv, options, long_options, NULL)) != -1) {
    switch (opt) {
      case 'v':
        verbose = true;
        break;
      case 'a':
        types = true;
        utils = true;
        basics = true;
        string = true;
        chr = true;
        gc = true;
        break;
      case 't':
        types = true;
        break;
      case 'u':
        utils = true;
        break;
      case 'b':
        basics = true;
        break;
      case 's':
        string = true;
        break;
      case 'c':
        chr = true;
        break;
      case 'l':
        list = true;
        break;
      case 'g':
        gc = true;
        break;
      default:
        fprintf(stderr, "Usage: %s [-%s]\n", argv[0], options);
        exit(EXIT_FAILURE);
    }
  }

  char* mu_error_message = test_all(types, utils, basics, string, chr, list, gc);
  int exit_code;

  if (tests_failed) {
    printf("FAILED %d new-style tests\n", tests_failed);
    exit_code = EXIT_FAILURE;
  }
  if (mu_error_message != NULL) {
    printf("FAILED min_unit test: %s\n", mu_error_message);
    exit_code = EXIT_FAILURE;
  }
  if (!tests_failed && !mu_error_message) {
    printf("\nALL TESTS PASSED\n");
    exit_code = EXIT_SUCCESS;
  }
  printf("Tests run: %d\n", tests_run);
  printf("Assertions made: %d\n", assertions_made);

  exit(exit_code);
}
