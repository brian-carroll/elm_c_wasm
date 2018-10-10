#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../kernel/utils.h"
#include "../kernel/basics.h"
#include "./test.h"

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
    mu_assert("Expect: () == ()", apply(&eq, 2, (void*[]){&Unit, &Unit}) == &True);

    mu_assert("Expect: True == True", apply(&eq, 2, (void*[]){&True, &True}) == &True);
    mu_assert("Expect: False == False", apply(&eq, 2, (void*[]){&False, &False}) == &True);
    mu_assert("Expect: True /= False", apply(&eq, 2, (void*[]){&True, &False}) == &False);
    mu_assert("Expect: False /= True", apply(&eq, 2, (void*[]){&False, &True}) == &False);

    ElmInt two = (ElmInt){ .header = HEADER_INT, .value = 2 };
    ElmInt three = (ElmInt){ .header = HEADER_INT, .value = 3 };

    mu_assert("Expect: 2 == 2", apply(&eq, 2, (void*[]){&two, &two}) == &True);
    mu_assert("Expect: 2 /= 3", apply(&eq, 2, (void*[]){&two, &three}) == &False);

    mu_assert("Expect: True /= 3", apply(&eq, 2, (void*[]){&True, &three}) == &False);

    ElmFloat *f = newElmFloat(123.456);
    ElmFloat *f1 = newElmFloat(123.456);
    ElmFloat *f2 = newElmFloat(2.0);
    mu_assert("Expect: 123.456 == 123.456 (by reference)", apply(&eq, 2, (void*[]){f, f}) == &True);
    mu_assert("Expect: 123.456 == 123.456 (by value)", apply(&eq, 2, (void*[]){f, f1}) == &True);
    mu_assert("Expect: 123.456 == 2.0 (by value)", apply(&eq, 2, (void*[]){f, f2}) == &False);

    ElmChar a1 = (ElmChar){ .header = HEADER_CHAR, .value = 'A' };
    ElmChar a2 = (ElmChar){ .header = HEADER_CHAR, .value = 'A' };
    ElmChar b  = (ElmChar){ .header = HEADER_CHAR, .value = 'B' };
    mu_assert("Expect: 'A' == 'A', by reference", apply(&eq, 2, (void*[]){&a1, &a1}) == &True);
    mu_assert("Expect: 'A' == 'A', by value", apply(&eq, 2, (void*[]){&a1, &a2}) == &True);
    mu_assert("Expect: 'A' /= 'B'", apply(&eq, 2, (void*[]){&a1, &b}) == &False);

    ElmString* hello1 = newElmString(5, "hello");
    ElmString* hello2 = newElmString(5, "hello");
    ElmString* hello_ = newElmString(6, "hello_");
    ElmString* world = newElmString(5, "world");

    if (verbose) {
        printf("hello1 str=\"%s\" hex=%s\n", hello1->bytes, hex(hello1, (1+hello1->header.size)*4) );
        printf("hello2 str=\"%s\" hex=%s\n", hello2->bytes, hex(hello2, (1+hello2->header.size)*4) );
        printf("hello_ str=\"%s\" hex=%s\n", hello_->bytes, hex(hello_, (1+hello_->header.size)*4) );
        printf("world str=\"%s\" hex=%s\n", world->bytes, hex(world, (1+world->header.size)*4) );
    }

    mu_assert("Expect: \"hello\" == \"hello\" (by reference)", apply(&eq, 2, (void*[]){hello1, hello1}) == &True);
    mu_assert("Expect: \"hello\" == \"hello\" (by value)", apply(&eq, 2, (void*[]){hello1, hello2}) == &True);
    mu_assert("Expect: \"hello\" /= \"world\"", apply(&eq, 2, (void*[]){hello1, world}) == &False);
    mu_assert("Expect: \"hello\" /= \"hello_\"", apply(&eq, 2, (void*[]){hello1, hello_}) == &False);
    mu_assert("Expect: \"hello_\" /= \"hello\"", apply(&eq, 2, (void*[]){hello_, hello1}) == &False);


    // TODO: Lists and tuples and nested values


    free(f);
    free(f1);
    free(f2);
    free(hello1);
    free(hello2);
    free(hello_);
    free(world);
    return NULL;
}

char* utils_test() {
    mu_run_test(test_apply);
    mu_run_test(test_eq);

    return NULL;
}
