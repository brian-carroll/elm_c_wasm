#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "./test.h"
#include "./types_test.h"
#include "./utils_test.h"
#include "./basics_test.h"
#include "./string_test.h"
#include "./gc_test.h"

// Init functions
#include "../kernel/types.h"
#include "../kernel/utils.h"
#include "../kernel/basics.h"
#include "../kernel/string.h"
#include "../kernel/gc.h"

#ifdef __EMSCRIPTEN__
    int verbose = true;
#else
    int verbose = false;
#endif

int tests_run = 0;


// Debug function, with pre-allocated memory for strings
// Avoiding use of malloc in test code in case it screws up GC
// A single printf may require many separate hex strings
#define TEST_MAX_HEXES_PER_PRINTF 16
char hex_strings[TEST_MAX_HEXES_PER_PRINTF][9*1024/4];
char* hex(void* addr, int size) {
    static u32 rotate = 0;
    rotate = (rotate+1) % TEST_MAX_HEXES_PER_PRINTF;

    size = (size < 1024) ? size : 1024;
    u32 i, c=0;
    for (i=0; i < size; c += 9, i += 4) {
        // Print in actual byte order (little endian)
        sprintf(hex_strings[rotate] + c,
            "%02x%02x%02x%02x|",
            *(u8*)(addr + i),
            *(u8*)(addr + i + 1),
            *(u8*)(addr + i + 2),
            *(u8*)(addr + i + 3)
        );
    }
    hex_strings[rotate][c-1] = 0; // erase last "|" and terminate the string
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
    bool types,
    bool utils,
    bool basics,
    bool string,
    bool gc
) {
    if (types)  mu_run_test(types_test);
    if (utils)  mu_run_test(utils_test);
    if (basics) mu_run_test(basics_test);
    if (string) mu_run_test(string_test);
    if (gc)     mu_run_test(gc_test);

    return NULL;
}



int main(int argc, char ** argv) {

    GC_init();
    Types_init();
    Basics_init();
    Utils_init();
    String_init();

    static struct option long_options[] =
    {
        {"verbose", no_argument, NULL, 'v'},
        {"all", no_argument, NULL, 'a'},
        {"types", no_argument, NULL, 't'},
        {"utils", no_argument, NULL, 'u'},
        {"basics", no_argument, NULL, 'b'},
        {"string", no_argument, NULL, 's'},
        {"gc", no_argument, NULL, 'g'},
        {NULL, 0, NULL, 0}
    };

    // By default in Bash shell, just do what's specified
    bool types = false;
    bool basics = false;
    bool string = false;
    bool utils = false;
    bool gc = false;

    // Running in a Windows CMD shell
    // Prob via Codelite IDE. Dunno how to set up CLI args there yet!
    // Set this up for whatever I'm working on
    #if defined(_WIN32)
    gc = true;
    verbose = true;
    #endif

    // In the browser. No CLI. Set reasonable defaults.
    #if defined(__EMSCRIPTEN__)
    types = true;
    utils = true;
    basics = true;
    string = true;
    verbose = true;
    #endif

    int opt;
    while ((opt = getopt_long(argc, argv, "vatubsg", long_options, NULL)) != -1) {
        switch (opt) {
            case 'v':
                verbose = true;
                break;
            case 'a':
                types = true;
                utils = true;
                basics = true;
                string = true;
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
            case 'g':
                gc = true;
                break;
            default:
                fprintf(stderr, "Usage: %s [-vatubsg]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    char* result = test_all(
        types,
        utils,
        basics,
        string,
        gc
    );
    bool passed = (result == NULL);

    if (!passed) {
        printf("Failed: %s\n", result);
    } else {
        printf("\nALL TESTS PASSED\n");
    }
    printf("Tests run: %d\n", tests_run);

    int exit_code = passed ? EXIT_SUCCESS : EXIT_FAILURE;
    exit(exit_code);
}
