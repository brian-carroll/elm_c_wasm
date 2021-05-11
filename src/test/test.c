#include "test.h"

#ifdef _WIN32
#include <windows.h>
#include "../lib/wingetopt/wingetopt.c"
#else
#include <getopt.h>
#include <unistd.h>
#endif

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "../lib/stb/stb_sprintf.h"
#include "../kernel/core/core.h"

#include "basics_test.c"
#include "char_test.c"
#include "debug_test.c"
#include "gc_test.c"
#include "json_test.c"
#include "list_test.c"
#include "string_test.c"
#include "test-compiled.c"
#include "test-imports.c"
#include "types_test.c"
#include "utils_test.c"

char* types_test();
char* utils_test();
char* basics_test();
char* string_test();
char* char_test();
char* list_test();
char* json_test();

int verbose = false;
int tests_run = 0;
int tests_failed = 0;
int assertions_made = 0;

// ---------------------------------------------------------
//
//          HIGHER LEVEL TESTING (ELM VALUES)
//
// Test kernel functions using the Elm equality operator etc.
// Not for lowest-level stuff like GC, or the equality op itself!
// But good for most kernel modules.
//
// ---------------------------------------------------------

char* current_describe_string;
void* test_heap_ptr;

void describe(char* description, void* (*test)()) {
  tests_run++;
  current_describe_string = description;
  test_heap_ptr = GC_allocate(false, 0);
  if (verbose) {
    safe_printf("\n%s\n", description);
  }
  test();
}

void describe_arg(char* description, void* (*test)(void* arg), void* arg) {
  tests_run++;
  current_describe_string = description;
  test_heap_ptr = GC_allocate(false, 0);
  if (verbose) {
    safe_printf("\n%s\n", description);
  }
  test(arg);
}

void* expect_equal(char* expect_description, void* left, void* right) {
  bool ok = A2(&Utils_equal, left, right) == &True;
  if (!ok) {
    if (!verbose) {
      safe_printf("\n%s\n", current_describe_string);
    }
    print_heap_range(test_heap_ptr, GC_allocate(false, 0));
    safe_printf("FAIL: %s\n", expect_description);
    Debug_pretty("Left", left);
    Debug_pretty("Right", right);
    safe_printf("\n");
    tests_failed++;
  } else if (verbose) {
    safe_printf("PASS: %s\n", expect_description);
  }
  assertions_made++;
  return NULL;
}

ElmString16* create_string(char* c_string) {
  size_t len = (size_t)strlen(c_string);
  ElmString16* s = newElmString16(len);
  for (size_t i = 0; i < len; i++) {
    s->words16[i] = (u16)c_string[i];
  }
  return s;
}

// Debug function, with pre-allocated memory for strings
// Avoiding use of malloc in test code in case it screws up GC
// A single safe_printf may require many separate hex strings
#define TEST_MAX_HEXES_PER_PRINTF 16
char hex_strings[TEST_MAX_HEXES_PER_PRINTF][9 * 1024 / 4];
char* hex(void* addr, int size) {
  static u32 rotate = 0;
  rotate = (rotate + 1) % TEST_MAX_HEXES_PER_PRINTF;

  size = (size < 1024) ? size : 1024;
  u32 i, c = 0;
  for (i = 0; i < size; c += 9, i += 4) {
    // Print in actual byte order (little endian)
    stbsp_sprintf(hex_strings[rotate] + c,
        "%02x%02x%02x%02x|",
        *((u8*)addr + i),
        *((u8*)addr + i + 1),
        *((u8*)addr + i + 2),
        *((u8*)addr + i + 3));
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

char* test_all(bool types,
    bool utils,
    bool basics,
    bool string,
    bool chr,
    bool list,
    bool debug,
    bool json,
    bool gc) {
  if (verbose) {
    safe_printf("Selected tests: ");
    if (types) safe_printf("types ");
    if (utils) safe_printf("utils ");
    if (basics) safe_printf("basics ");
    if (string) safe_printf("string ");
    if (chr) safe_printf("char ");
    if (list) safe_printf("list ");
    if (debug) safe_printf("debug ");
    if (json) safe_printf("json ");
    if (gc) safe_printf("gc ");
    safe_printf("\n\n");
  }
  if (types) mu_run_test(types_test);
  if (utils) mu_run_test(utils_test);
  if (basics) mu_run_test(basics_test);
  if (string) mu_run_test(string_test);
  if (chr) mu_run_test(char_test);
  if (list) mu_run_test(list_test);
  if (debug) mu_run_test(debug_test);
  if (json) mu_run_test(json_test);
  if (gc) mu_run_test(gc_test);

  return NULL;
}

int main(int argc, char** argv) {
  GC_init();

  static struct option long_options[] = {
      {"verbose", no_argument, NULL, 'v'},
      {"all", no_argument, NULL, 'a'},
      {"types", optional_argument, NULL, 't'},
      {"utils", optional_argument, NULL, 'u'},
      {"basics", optional_argument, NULL, 'b'},
      {"string", optional_argument, NULL, 's'},
      {"char", optional_argument, NULL, 'c'},
      {"list", optional_argument, NULL, 'l'},
      {"debug", optional_argument, NULL, 'd'},
      {"json", optional_argument, NULL, 'j'},
      {"gc", optional_argument, NULL, 'g'},
      {NULL, 0, NULL, 0},
  };

  // By default in Bash shell, just do what's specified
  bool types = false;
  bool basics = false;
  bool string = false;
  bool chr = false;
  bool utils = false;
  bool list = false;
  bool debug = false;
  bool json = false;
  bool gc = false;

  char options[] = "vatubscldjg";

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
        list = true;
        json = true;
        gc = true;
        break;
      case 't':
        types = !optarg;
        break;
      case 'u':
        utils = !optarg;
        break;
      case 'b':
        basics = !optarg;
        break;
      case 's':
        string = !optarg;
        break;
      case 'c':
        chr = !optarg;
        break;
      case 'l':
        list = !optarg;
        break;
      case 'd':
        debug = !optarg;
        break;
      case 'j':
        json = !optarg;
        break;
      case 'g':
        gc = !optarg;
        break;
      default: {
        safe_printf("Usage: %s [-%s]\n", argv[0], options);
        exit(EXIT_FAILURE);
      }
    }
  }

#ifdef __EMSCRIPTEN__
  safe_printf("\n");
  safe_printf("WebAssembly Tests\n");
  safe_printf("=================\n");
#else
  safe_printf("\n");
  safe_printf("Native Binary Tests\n");
  safe_printf("===================\n");
#endif
  safe_printf("argument list: ");
  for (int i = 1; i < argc; i++) {
    safe_printf("'%s' ", argv[i]);
  }
  safe_printf("\n");

  test_all(types, utils, basics, string, chr, list, debug, json, gc);
  int exit_code;

  if (tests_failed) {
    safe_printf("FAILED %d tests\n", tests_failed);
    exit_code = EXIT_FAILURE;
  } else {
    safe_printf("\nALL TESTS PASSED\n");
    exit_code = EXIT_SUCCESS;
  }
  safe_printf("Tests run: %d\n", tests_run);
  safe_printf("Assertions made: %d\n", assertions_made);
  safe_printf("\n");

  exit(exit_code);
}
