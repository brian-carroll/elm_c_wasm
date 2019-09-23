#define __USE_MINGW_ANSI_STDIO 1
#include <stdio.h>

#define mu_assert(message, test)     \
  do {                               \
    assertions_made++;               \
    if (!(test)) {                   \
      return message;                \
    } else if (verbose) {            \
      printf("PASS: %s\n", message); \
    }                                \
  } while (0)

#define mu_expect_equal(message, expr1, expr2)         \
  do {                                                 \
    assertions_made++;                                 \
    if (expr1 != expr2) {                              \
      if (verbose) {                                   \
        printf("FAIL: %s\n  LHS : %zx\n  RHS : %zx\n", \
            message,                                   \
            (size_t)(expr1),                           \
            (size_t)(expr2));                          \
      }                                                \
      return message;                                  \
    } else if (verbose) {                              \
      printf("PASS: %s\n", message);                   \
    }                                                  \
  } while (0)

#define mu_run_test(test)                \
  do {                                   \
    char* message = test();              \
    tests_run++;                         \
    if (message != NULL) return message; \
  } while (0)

extern int tests_run, assertions_made, verbose;

char* hex(void* addr, int size);
char* hex_ptr(void* ptr);
