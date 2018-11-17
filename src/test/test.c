#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../kernel/types.h"
#include "../kernel/utils.h"
#include "../kernel/basics.h"
#include "../kernel/string.h"
#include "./test.h"
#include "./types_test.h"
#include "./utils_test.h"
#include "./basics_test.h"
#include "./string_test.h"
#include "./gc_test.h"

#ifdef __EMSCRIPTEN__
    int verbose = true;
#else
    int verbose = false;
#endif

int tests_run = 0;


// Debug function
char* hex(void* addr, int size) {
    char *str;
    u32 i, c=0, n=(9*size)/4;
    str = malloc(n);
    for (i=0; i < size; c += 9, i += 4) {
        // Print in actual byte order (little endian)
        sprintf(str + c,
            "%02x%02x%02x%02x|",
            *(u8*)(addr + i),
            *(u8*)(addr + i + 1),
            *(u8*)(addr + i + 2),
            *(u8*)(addr + i + 3)
        );
    }
    str[c-1] = 0; // erase last "|" and terminate the string
    return str;
}

// Print a memory address held in a pointer
//   Addresses are printed in little-endian format
//   => Easier to see what pointers are pointing to it
//   => Harder to subtract in your head if you're interested in sizes & memory layout
char* hex_ptr(void* ptr) {
    return hex(&ptr, sizeof(void*));
}



char* test_all() {
    // mu_run_test(types_test);
    // mu_run_test(utils_test);
    // mu_run_test(basics_test);
    // mu_run_test(string_test);
    mu_run_test(gc_test);

    return NULL;
}



int main(int argc, char ** argv) {
    int opt;

    Types_init();
    Basics_init();
    Utils_init();
    String_init();

    while ((opt = getopt(argc, argv, "v")) != -1) {
        switch (opt) {
            case 'v':
                verbose = true;
                break;
            default:
                fprintf(stderr, "Usage: %s [-v]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    char* result = test_all();
    bool passed = result == NULL;

    if (!passed) {
        printf("%s\n", result);
    } else {
        printf("\nALL TESTS PASSED\n");
    }
    printf("Tests run: %d\n", tests_run);

    return !passed;
}
