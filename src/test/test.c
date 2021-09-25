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

#include "../kernel/core/core.h"
#include "../lib/stb/stb_sprintf.h"

#include "basics_test.c"
#include "char_test.c"
#include "debug_test.c"
#include "gc_test.c"
#include "json_test.c"
#include "list_test.c"
#include "platform_test.c"
#include "string_test.c"
#include "test-compiled.c"
#include "test-imports.c"
#include "types_test.c"
#include "utils_test.c"

#ifdef __EMSCRIPTEN__
#include "wrapper_test.c"
#else
void wrapper_test() {
  tests_run--;
}
void* eval_createTuple3(void* args[]) {
  return NULL;
}
#endif

struct test_suite {
  char* name;
  char shortName;
  bool enable;
  void (*run)();
};

#define NUM_SUITES 11
#define NUM_OPTIONS (NUM_SUITES + 2)

struct test_suite suites[] = {
    {.name = "types", .shortName = 't', .run = types_test},
    {.name = "utils", .shortName = 'u', .run = utils_test},
    {.name = "basics", .shortName = 'b', .run = basics_test},
    {.name = "string", .shortName = 's', .run = string_test},
    {.name = "char", .shortName = 'c', .run = char_test},
    {.name = "list", .shortName = 'l', .run = list_test},
    {.name = "debug", .shortName = 'd', .run = debug_test},
    {.name = "json", .shortName = 'j', .run = json_test},
    {.name = "wrapper", .shortName = 'w', .run = wrapper_test},
    {.name = "platform", .shortName = 'p', .run = platform_test},
    {.name = "gc", .shortName = 'g', .run = gc_test},
};
struct option long_options[NUM_OPTIONS + 1];

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

void describe(char* description, void (*test)()) {
  tests_run++;
  current_describe_string = description;
  test_heap_ptr = GC_allocate(false, 0);
  if (verbose) {
    safe_printf("\n%s\n", description);
  }
  test();
}

void describe_arg(char* description, void (*test)(void* arg), void* arg) {
  tests_run++;
  current_describe_string = description;
  test_heap_ptr = GC_allocate(false, 0);
  if (verbose) {
    safe_printf("\n%s\n", description);
  }
  test(arg);
}

void expect_equal(char* expect_description, void* left, void* right) {
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
}

ElmString* create_string(char* c_string) {
  size_t len = strlen(c_string);
  ElmString* s = newElmString(len);
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

void test_all() {
  if (verbose) {
    safe_printf("Selected tests: ");
    for (int i = 0; i < ARRAY_LEN(suites); i++) {
      struct test_suite* suite = suites + i;
      if (suite->enable) {
        safe_printf("%s ", suite->name);
      }
    }
    safe_printf("\n\n");
  }
  for (int i = 0; i < ARRAY_LEN(suites); i++) {
    struct test_suite* suite = suites + i;
    if (suite->enable) {
      suite->run();
    }
  }
}

int main(int argc, char** argv) {
  GC_init();

  ASSERT_EQUAL(NUM_SUITES, ARRAY_LEN(suites));

  char options[NUM_OPTIONS + 1] = "va";
  long_options[0] = (struct option){"verbose", no_argument, NULL, 'v'};
  long_options[1] = (struct option){"all", no_argument, NULL, 'a'};

  for (int i = 0; i < ARRAY_LEN(suites); i++) {
    struct test_suite* suite = suites + i;
    options[2 + i] = suite->shortName;
    long_options[2 + i] =
        (struct option){suite->name, no_argument, NULL, suite->shortName};
  }

  options[NUM_OPTIONS] = '\0';
  long_options[NUM_OPTIONS] = (struct option){NULL, 0, NULL, 0};

  int opt;
  int option_index = 0;
  while ((opt = getopt_long(argc, argv, options, long_options, &option_index)) != -1) {
    if (opt == 'v') {
      verbose = true;
    } else if (opt == 'a') {
      for (int i = 0; i < ARRAY_LEN(suites); i++) {
        struct test_suite* suite = suites + i;
        suite->enable = true;
      }
    } else if (opt == '?') {
        safe_printf("Cannot parse command line\n");
        for (int a = 0; a < argc; a++) {
          safe_printf("%s ", argv[a]);
        }
        safe_printf("\nUsage: %s [-%s]\n", argv[0], options);
        exit(EXIT_FAILURE);
    } else {
      for (int i = 0; i < ARRAY_LEN(suites); i++) {
        struct test_suite* suite = suites + i;
        if (suite->shortName == opt) {
          suite->enable = true;
          break;
        }
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

#ifndef TEST
  safe_printf("TEST macro not defined\n");
  exit(EXIT_FAILURE);
#endif

  test_all();
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
