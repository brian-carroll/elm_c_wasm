#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../kernel/utils.h"
#include "../kernel/basics.h"
#include "./test.h"


char* test_records() {

    /*
        type alias Record1 =
            { someField: Int
            , otherField: String
            }

        type alias Record2 =
            { things: Int
            , someField: Int
            , stuff: Float
            }

        r1 = Record1 123 "hello"
        r2 = Record2 456 321 1.0

        int1 = .someField r1  -- 123
        int2 = .someField r2  -- 321

        r3 = { r2
               | things = 111
               , stuff = 3.14
             }

    */

    // Elm compiler transforms field names to numbers
    u32 things = 12;
    u32 someField = 23;
    u32 otherField = 42;
    u32 stuff = 71;

    FieldSet* fsRecord1 = malloc(sizeof(FieldSet) + 2*sizeof(u32));
    FieldSet* fsRecord2 = malloc(sizeof(FieldSet) + 3*sizeof(u32));

    fsRecord1->size = 2;
    fsRecord1->fields[0] = someField;
    fsRecord1->fields[1] = otherField;

    fsRecord2->size = 3;
    fsRecord2->fields[0] = things;
    fsRecord2->fields[1] = someField;
    fsRecord2->fields[2] = stuff;

    Record* r1 = malloc(sizeof(Record) + 2*sizeof(void*));
    Record* r2 = malloc(sizeof(Record) + 3*sizeof(void*));

    r1->header = HEADER_RECORD(2);
    r1->fieldset = fsRecord1;
    r1->values[0] = newElmInt(123);
    r1->values[1] = newElmString(5, "hello");

    r2->header = HEADER_RECORD(3);
    r2->fieldset = fsRecord2;
    r2->values[0] = newElmInt(456);
    r2->values[1] = newElmInt(321);
    r2->values[2] = newElmFloat(1.0);

    // The actual accessor function
    Closure* access_someField = apply(
        &record_access, 1,
        (void*[]){ newElmInt(someField) }
    );

    if (verbose) {
        printf("access_someField = %s\n",
            hex(access_someField, sizeof(Closure) + sizeof(void*))
        );
        printf("fsRecord1: addr=%llx val=%s\n",
            (u64)fsRecord1,
            hex(fsRecord1, sizeof(FieldSet) + 2*sizeof(u32))
        );
        printf("fsRecord2: addr=%llx val=%s\n",
            (u64)fsRecord2,
            hex(fsRecord2, sizeof(FieldSet) + 3*sizeof(u32))
        );
    }

    ElmInt* int1 = apply(
        access_someField, 1,
        (void*[]){ r1 }
    );

    ElmInt* int2 = apply(
        access_someField, 1,
        (void*[]){ r2 }
    );

    /*
        r3 = { r2
               | things = 111
               , stuff = 3.14
             }
    */
    ElmInt* updated_thing = newElmInt(111);
    ElmFloat* updated_stuff = newElmFloat(3.14);
    Record* r3 = record_update(
        r2, 2, (u32[]){ things, stuff },
        (void*[]){ updated_thing, updated_stuff }
    );

    mu_assert("Record accessor should work on r1", int1->value = 123);
    mu_assert("Record accessor should work on r2", int2->value = 321);

    mu_assert("Updated record should have same fieldset",
        r3->fieldset == r2->fieldset
    );
    mu_assert("Updated record should have same header",
        memcmp(r3, r2, sizeof(Header)) == 0
    );
    mu_assert("Updated record should have 3 correct fields",
        r3->values[0] == updated_thing &&
        r3->values[1] == r2->values[1] &&
        r3->values[2] == updated_stuff
    );

    return NULL;
}



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
    mu_run_test(test_records);
    mu_run_test(test_apply);
    mu_run_test(test_eq);

    return NULL;
}
