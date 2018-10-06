#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../src/utils.h"
#include "../src/basics.h"
#include "./test.c" // including .c file, naughty!

bool verbose = false;
int tests_run = 0;

// This is code I could generate from the Elm compiler. I could write better by hand.
// The intermediate ElmInt allocated on heap is hard to eliminate in generated code.
// Would be nice to evaluate entire expression on raw ints, and only then box it in ElmInt
// Would require Elm compiler optimizer to identify Int-only subexpressions (also Float-only)
// so that the code generator would know what to do.
void* eval_user_project_closure(void* args[]) {
    ElmInt *outerScopeValue = args[0];
    ElmInt *arg1 = args[1];
    ElmInt *arg2 = args[2];

    ElmInt *tmp = apply(&add, 2, (void*[]){arg2, outerScopeValue});
    return apply(&add, 2, (void*[]){arg1, tmp});
}


/*
    outerScopeValue =
        1

    closure arg1 arg2 =
        outerScopeValue + arg1 + arg2

    curried = 
        closure 2

    answer =
        curried 3
*/

char* test_apply(void) {
    Closure user_project_closure = (Closure) {
        .header = HEADER_CLOSURE(0),
        .evaluator = &eval_user_project_closure,
        .max_values = 3,
        .n_values = 0
    };

    ElmInt outerScopeValue = (ElmInt){ .header = HEADER_INT, .value = 1 };
    ElmInt two = (ElmInt){ .header = HEADER_INT, .value = 2 };
    ElmInt three = (ElmInt){ .header = HEADER_INT, .value = 3 };

    Closure* closure = apply(
        &user_project_closure,
        1,
        (void*[]){&outerScopeValue}
    );

    Closure* curried = apply(
        closure,
        1,
        (void*[]){&two}
    );
    ElmInt* answer = apply(
        curried,
        1,
        (void*[]){&three}
    );

    if (verbose) {
        printf("outerScopeValue addr=%s ctor=%d value=%d, hex=%s\n",
            hex_ptr(&outerScopeValue), (int)outerScopeValue.header.tag, outerScopeValue.value,
            hex(&outerScopeValue, sizeof(ElmInt))
        );

        printf("two addr=%s ctor=%d value=%d, hex=%s\n",
            hex_ptr(&two), (int)two.header.tag, two.value,
            hex(&two, sizeof(ElmInt))
        );

        printf("three addr=%s ctor=%d value=%d, hex=%s\n",
            hex_ptr(&three), (int)three.header.tag, three.value,
            hex(&three, sizeof(ElmInt))
        );

        printf("closure addr=%s n_values=%d max_values=%d, hex=%s\n",
            hex_ptr(closure), (int)closure->n_values, (int)closure->max_values,
            hex(closure, sizeof(Closure) + closure->n_values * sizeof(void*))
        );

        printf("curried addr=%s n_values=%d max_values=%d, hex=%s\n",
            hex_ptr(curried), (int)curried->n_values, (int)curried->max_values,
            hex(curried, sizeof(Closure) + curried->n_values * sizeof(void*))
        );

        printf("answer addr=%s ctor=%d value=%d, hex=%s\n",
            hex_ptr(answer), (int)answer->header.tag, answer->value,
            hex(answer, sizeof(ElmInt))
        );
        printf("\n");
    }

    ElmInt expected_answer = (ElmInt){ .header = HEADER_INT, .value = 6 };
    mu_assert("answer should be ElmInt 6",
        memcmp(answer, &expected_answer, sizeof(ElmInt)) == 0
    );

    free(curried);
    free(closure);
    free(answer);
    // The intermediate ElmInt created in eval_user_project_closure is unreachable, can't free it
    // Oh well... tests won't crash anyway, I'm being too careful. I guess we need GC
    return NULL;
}


char* test_eq(void) {
    mu_assert("Expect: () == ()", eq(&Unit, &Unit) == &True);

    mu_assert("Expect: True == True", eq(&True, &True) == &True);
    mu_assert("Expect: False == False", eq(&False, &False) == &True);
    mu_assert("Expect: True /= False", eq(&True, &False) == &False);
    mu_assert("Expect: False /= True", eq(&False, &True) == &False);

    ElmInt two = (ElmInt){ .header = HEADER_INT, .value = 2 };
    ElmInt three = (ElmInt){ .header = HEADER_INT, .value = 3 };

    mu_assert("Expect: 2 == 2", eq(&two, &two) == &True);
    mu_assert("Expect: 2 /= 3", eq(&two, &three) == &False);

    mu_assert("Expect: True /= 3", eq(&True, &three) == &False);

    ElmFloat *f = newElmFloat(123.456);
    ElmFloat *f1 = newElmFloat(123.456);
    mu_assert("Expect: 123.456 == 123.456 (by reference)", eq(f, f) == &True);
    mu_assert("Expect: 123.456 == 123.456 (by value)", eq(f, f1) == &True);
    mu_assert("Expect: 123.456 /= 1.0 : %s\n", eq(f, newElmFloat(1.0)) == &False);

    mu_assert("Expect: 'A' == 'A'", eq(newElmChar('A'), newElmChar('A')) == &True);
    mu_assert("Expect: 'A' /= 'B'", eq(newElmChar('A'), newElmChar('B')) == &False);

    ElmString* hello1 = newElmString(5, "hello");
    ElmString* hello2 = newElmString(5, "hello");
    ElmString* hello_ = newElmString(6, "hello_");
    ElmString* world = newElmString(5, "world");

    mu_assert("Expect: \"hello\" == \"hello\" (by reference)", eq(hello1, hello1) == &True);
    mu_assert("Expect: \"hello\" == \"hello\" (by value)", eq(hello1, hello2) == &True);
    mu_assert("Expect: \"hello\" /= \"hello_\"", eq(hello1, hello_) == &False);
    mu_assert("Expect: \"hello_\" /= \"hello\"", eq(hello_, hello1) == &False);
    mu_assert("Expect: \"hello_\" /= \"world\"", eq(hello_, world) == &False);

    free(f);
    free(f1);
    free(hello1);
    free(hello2);
    free(hello_);
    free(world);
    return NULL;
}


char* test_all() {
    mu_run_test(test_apply);
    mu_run_test(test_eq);

    return NULL;
}


int main(int argc, char ** argv) {
    int opt;

    basics_init();

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
        printf("ALL TESTS PASSED\n");
    }
    printf("Tests run: %d\n", tests_run);

    return !passed;
}
