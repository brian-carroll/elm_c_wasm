#ifndef TEST_H
#define TEST_H

#define __USE_MINGW_ANSI_STDIO 1
#include <stdio.h>

#include "../kernel/types.h"

enum {
  CTOR_Nothing,
  CTOR_Just,
  CTOR_Ok,
  CTOR_Err,
  CTOR_Failure,
  CTOR_Field,
  CTOR_Index,
  CTOR_OneOf,
  CTOR_MockElmArray,
};

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
      printf("FAIL: %s\n", message); \
    } else if (verbose) {            \
      printf("PASS: %s\n", message); \
    }                                \
  } while (0)

#define mu_expect_equal(message, expr1, expr2)           \
  do {                                                   \
    assertions_made++;                                   \
    if (expr1 != expr2) {                                \
      tests_failed++;                                    \
      printf("FAIL: %s\n  LHS : 0x%zx\n  RHS : 0x%zx\n", \
          message,                                       \
          (size_t)(expr1),                               \
          (size_t)(expr2));                              \
    } else if (verbose) {                                \
      printf("PASS: %s\n", message);                     \
    }                                                    \
  } while (0)

#define mu_run_test(test) \
  do {                    \
    test();               \
    tests_run++;          \
  } while (0)

extern int tests_run, tests_failed, assertions_made, verbose;

char* hex(void* addr, int size);
char* hex_ptr(void* ptr);

extern char* test_description;
extern void* test_heap_ptr;

void describe(char* description, void* (*test)());
void* expect_equal(char* expect_description, void* left, void* right);
ElmString16* create_string(char* c_string);

#endif
