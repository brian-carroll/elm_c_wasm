#ifndef TEST_H
#define TEST_H

#define __USE_MINGW_ANSI_STDIO 1
#include <stdbool.h>

#include "../kernel/core/types.h"

#define TEST_CTOR(x) x,
enum ctor {
#include "./test-ctors.inc"
NUM_TEST_CTORS
};
#undef TEST_CTOR

// ---------------------------------------------------------
//
//             TEST JS IMPORTS & CIRCULAR VALUES
//
// ---------------------------------------------------------

enum {
  TEST_JS_OBJECT_NON_CIRCULAR,
  TEST_JS_OBJECT_CIRCULAR,
  TEST_JS_ARRAY_NON_CIRCULAR,
  TEST_JS_ARRAY_CIRCULAR,
};

extern void* testCircularJsValue(bool isArray);
extern void* testJsonValueRoundTrip(ElmString* jsonStringAddr);

#ifdef __EMSCRIPTEN__
extern void* testElmValueRoundTrip(void* addr);
extern void* testWriteJsValueToWasm(u32 index);
extern void* testCallWasmFuncWithJsArgs(Closure* closureAddr);
extern JsRef* testWriteJsCallbackToWasm();
#endif

// ---------------------------------------------------------
//
//             LOW-LEVEL TESTING (BYTES, NUMBERS)
//
// Based on minunit http://www.jera.com/techinfo/jtns/jtn002.html
// but no early return. Print all failures.
//
// ---------------------------------------------------------
#define mu_assert(message, test)     \
  do {                               \
    assertions_made++;               \
    if (!(test)) {                   \
      tests_failed++;                \
      safe_printf("FAIL: %s\n", message); \
    } else if (verbose) {            \
      safe_printf("PASS: %s\n", message); \
    }                                \
  } while (0)

#define mu_expect_equal(message, expr1, expr2)           \
  do {                                                   \
    assertions_made++;                                   \
    if (expr1 != expr2) {                                \
      tests_failed++;                                    \
      safe_printf("FAIL: %s\n  LHS : 0x%zx\n  RHS : 0x%zx\n", \
          message,                                       \
          (size_t)(expr1),                               \
          (size_t)(expr2));                              \
    } else if (verbose) {                                \
      safe_printf("PASS: %s\n", message);                     \
    }                                                    \
  } while (0)

#define mu_run_test(test) \
  do {                    \
    test();               \
    tests_run++;          \
  } while (0)

extern int tests_run, tests_failed, assertions_made, verbose;

char* hex(void* addr, int size);

extern char* current_describe_string;
extern void* test_heap_ptr;

void describe(char* description, void (*test)());
void describe_arg(char* description, void (*test)(void* arg), void* arg);
void expect_equal(char* expect_description, void* left, void* right);
ElmString* create_string(char* c_string);

#endif
