#include "test.h"

#include <getopt.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../kernel/core/core.h"
#include "gc_test.h"

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
//  "Compiler-generated" values
//
// ---------------------------------------------------------

void* eval_List_reverse(void* args[]) {
  Cons* list = args[0];
  Cons* result = &Nil;
  for (; list != &Nil; list = list->tail) {
    result = NEW_CONS(list->head, result);
  }
  return result;
}
Closure g_elm_core_List_reverse = {
    .header = HEADER_CLOSURE(0),
    .max_values = 1,
    .evaluator = &eval_List_reverse,
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

void* eval_elm_core_Result_Ok(void* args[]) {
  return ctorCustom(CTOR_Ok, 1, args);
}
void* eval_elm_core_Result_Err(void* args[]) {
  return ctorCustom(CTOR_Err, 1, args);
}
void* eval_elm_core_Result_isOk(void* args[]) {
  void* x_result = args[0];
  void* tmp0;
  u32 tmp1 = ((Custom*)x_result)->ctor;
  do {
    if (tmp1 == CTOR_Ok) {
      tmp0 = &True;
      break;
    } else {
      tmp0 = &False;
      break;
    };
  } while (0);
  return tmp0;
}
Closure g_elm_core_Result_isOk = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_core_Result_isOk,
};

void* eval_elm_json_Json_Decode_Failure(void* args[]) {
  return ctorCustom(CTOR_Failure, 2, args);
}
void* eval_elm_json_Json_Decode_Field(void* args[]) {
  return ctorCustom(CTOR_Field, 2, args);
}
void* eval_elm_json_Json_Decode_Index(void* args[]) {
  return ctorCustom(CTOR_Index, 2, args);
}
void* eval_elm_json_Json_Decode_OneOf(void* args[]) {
  return ctorCustom(CTOR_OneOf, 1, args);
}
Closure g_elm_json_Json_Decode_Failure = {
    .header = HEADER_CLOSURE(0),
    .max_values = 2,
    .evaluator = &eval_elm_json_Json_Decode_Failure,
};
Closure g_elm_json_Json_Decode_Field = {
    .header = HEADER_CLOSURE(0),
    .max_values = 2,
    .evaluator = &eval_elm_json_Json_Decode_Field,
};
Closure g_elm_json_Json_Decode_Index = {
    .header = HEADER_CLOSURE(0),
    .max_values = 2,
    .evaluator = &eval_elm_json_Json_Decode_Index,
};
Closure g_elm_json_Json_Decode_OneOf = {
    .header = HEADER_CLOSURE(0),
    .max_values = 1,
    .evaluator = &eval_elm_json_Json_Decode_OneOf,
};

void* eval_elm_core_Array_initialize(void* args[]) {
  ElmInt* len = args[0];
  Closure* fn = args[1];
  Custom* mock_array = NEW_CUSTOM(CTOR_MockElmArray, len->value, NULL);
  for (size_t i = 0; i < len->value; i++) {
    mock_array->values[i] = A1(fn, NEW_ELM_INT(i));
  }
  return mock_array;
}
Closure g_elm_core_Array_initialize = {
    .header = HEADER_CLOSURE(0),
    .max_values = 2,
    .evaluator = &eval_elm_core_Array_initialize,
};

FieldGroup* Wrapper_appFieldGroups[] = {NULL};
void** Wrapper_mainsArray[] = {NULL};

char Debug_evaluator_name_buf[1024];
char* Debug_evaluator_name(void* p) {
  sprintf(Debug_evaluator_name_buf, "%p", p);
  return Debug_evaluator_name_buf;
}

int Debug_ctors_size = 8;
char* Debug_ctors[] = {
    "CTOR_Nothing",
    "CTOR_Just",
    "CTOR_Ok",
    "CTOR_Err",
    "CTOR_Failure",
    "CTOR_Field",
    "CTOR_Index",
    "CTOR_OneOf",
};
char* Debug_fields[] = {};
char* Debug_jsValues[] = {};
int Debug_fields_size = 0;
int Debug_jsValues_size = 0;

size_t evalWasmThunkInJs(size_t addr) {
  void* result = Utils_apply((Closure*)addr, 0, NULL);
  return (size_t)result;
}

// ---------------------------------------------------------
//
//          HIGHER LEVEL TESTING (ELM VALUES)
//
// Test kernel functions using the Elm equality operator etc.
// Not for lowest-level stuff like GC, or the equality op itself!
// But good for most kernel modules.
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
    print_heap_range(test_heap_ptr, GC_malloc(0));
    printf("FAIL: %s\n", expect_description);
    Debug_pretty("Left", left);
    Debug_pretty("Right", right);
    printf("\n");
    tests_failed++;
  } else if (verbose) {
    printf("PASS: %s\n", expect_description);
  }
  assertions_made++;
  return NULL;
}

ElmString16* create_string(char* c_string) {
  size_t c_len = (size_t)strlen(c_string);
  size_t bytes_utf16 = c_len * 2;
  ElmString16* s = NEW_ELM_STRING(bytes_utf16, NULL);
  for (size_t i = 0; i < c_len; i++) {
    s->words16[i] = (u16)c_string[i];
  }
  return s;
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

char* test_all(bool types,
    bool utils,
    bool basics,
    bool string,
    bool chr,
    bool list,
    bool json,
    bool gc) {
  if (verbose) {
    printf("Selected tests: ");
    if (types) printf("types ");
    if (utils) printf("utils ");
    if (basics) printf("basics ");
    if (string) printf("string ");
    if (chr) printf("char ");
    if (list) printf("list ");
    if (json) printf("json ");
    if (gc) printf("gc ");
    printf("\n\n");
  }
  if (types) mu_run_test(types_test);
  if (utils) mu_run_test(utils_test);
  if (basics) mu_run_test(basics_test);
  if (string) mu_run_test(string_test);
  if (chr) mu_run_test(char_test);
  if (list) mu_run_test(list_test);
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
  bool json = false;
  bool gc = false;

// Running in a Windows CMD shell
// Prob via Codelite IDE. Dunno how to set up CLI args there yet!
// Set this up for whatever I'm working on
#if defined(_WIN32)
  gc = true;
  verbose = true;
#endif

  char options[] = "vatubscljg";

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
      case 'j':
        json = !optarg;
        break;
      case 'g':
        gc = !optarg;
        break;
      default:
        fprintf(stderr, "Usage: %s [-%s]\n", argv[0], options);
        exit(EXIT_FAILURE);
    }
  }

#ifdef __EMSCRIPTEN__
  printf("\n");
  printf("WebAssembly Tests\n");
  printf("=================\n");
#else
  printf("\n");
  printf("Native Binary Tests\n");
  printf("===================\n");
#endif

  test_all(types, utils, basics, string, chr, list, json, gc);
  int exit_code;

  if (tests_failed) {
    printf("FAILED %d tests\n", tests_failed);
    exit_code = EXIT_FAILURE;
  } else {
    printf("\nALL TESTS PASSED\n");
    exit_code = EXIT_SUCCESS;
  }
  printf("Tests run: %d\n", tests_run);
  printf("Assertions made: %d\n", assertions_made);
  printf("\n");

  exit(exit_code);
}
