#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../kernel/types.h"
#include "../kernel/utils.h"
#include "../kernel/string.h"
#include "./test.h"

char* string_append_test() {

    ElmString* h = newElmString(5, "hello");
    ElmString* w = newElmString(6, " world");
    ElmString* hw = apply(&String_append, 2, (void*[]){h,w});

    if (verbose) {
        printf("## String_append\n");
        printf("\"%s\" ++ \"%s\" == \"%s\"\n", h->bytes, w->bytes, hw->bytes);
        printf("hello: %s\n", hex(h, sizeof(ElmString) + 8));
        printf("world: %s\n", hex(w, sizeof(ElmString) + 8));
        printf("hello world: %s\n", hex(hw, sizeof(ElmString) + 12));
    }

    mu_assert("Expect: \"hello\" ++ \" world\" == \"hello world\"",
        strcmp((const char*)hw->bytes, "hello world") == 0
    );

    return NULL;
}


char* string_test() {
    if (verbose) {
        printf("\n");
        printf("String\n");
        printf("------\n");
    }

    mu_run_test(string_append_test);
    return NULL;
}
