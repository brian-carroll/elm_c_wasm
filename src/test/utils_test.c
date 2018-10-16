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
    if (verbose) printf("\n\nEquality\n--------\n");

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

    Cons* cons2 = newCons(&two, &Nil);
    Cons* cons2a = newCons(&two, &Nil);
    Cons* cons3 = newCons(&three, &Nil);
    Cons* cons23 = newCons(&two, cons3);
    Cons* cons23a = newCons(&two, cons3);
    Cons* cons32 = newCons(&three, cons2);
    Cons* cons22 = newCons(&two, cons2);

    mu_assert("Expect: [] == []", apply(&eq, 2, (void*[]){&Nil, &Nil}) == &True);
    mu_assert("Expect: [] /= [2]", apply(&eq, 2, (void*[]){&Nil, cons2}) == &False);
    mu_assert("Expect: [2] /= []", apply(&eq, 2, (void*[]){cons2, &Nil}) == &False);
    mu_assert("Expect: [2] == [2] (by ref)", apply(&eq, 2, (void*[]){cons2, cons2}) == &True);
    mu_assert("Expect: [2] == [2] (by value)", apply(&eq, 2, (void*[]){cons2, cons2a}) == &True);
    mu_assert("Expect: [2] /= [3]", apply(&eq, 2, (void*[]){cons2, cons3}) == &False);
    mu_assert("Expect: [2] /= [2,3]", apply(&eq, 2, (void*[]){cons2, cons23}) == &False);
    mu_assert("Expect: [2,3] == [2,3] (by ref)", apply(&eq, 2, (void*[]){cons23, cons23}) == &True);
    mu_assert("Expect: [2,3] == [2,3] (by value)", apply(&eq, 2, (void*[]){cons23, cons23a}) == &True);
    mu_assert("Expect: [3,2] /= [2,2]", apply(&eq, 2, (void*[]){cons32, cons22}) == &False);
    mu_assert("Expect: [2,3] /= [2,2]", apply(&eq, 2, (void*[]){cons23, cons22}) == &False);


    Tuple2* tuple23 = newTuple2(&two, &three);
    Tuple2* tuple23a = newTuple2(&two, &three);
    Tuple2* tuple32 = newTuple2(&three, &two);
    Tuple2* tuple22 = newTuple2(&two, &two);

    mu_assert("Expect: (2,3) == (2,3) (by ref)", apply(&eq, 2, (void*[]){tuple23, tuple23}) == &True);
    mu_assert("Expect: (2,3) == (2,3) (by value)", apply(&eq, 2, (void*[]){tuple23, tuple23a}) == &True);
    mu_assert("Expect: (3,2) /= (2,2)", apply(&eq, 2, (void*[]){tuple32, tuple22}) == &False);
    mu_assert("Expect: (2,3) /= (2,2)", apply(&eq, 2, (void*[]){tuple23, tuple22}) == &False);

    ElmInt one = (ElmInt){ .header = HEADER_INT, .value = 1 };
    Tuple3* tuple123 = newTuple3(&one, &two, &three);
    Tuple3* tuple123a = newTuple3(&one, &two, &three);
    Tuple3* tuple111 = newTuple3(&one, &one, &one);
    Tuple3* tuple211 = newTuple3(&two, &one, &one);
    Tuple3* tuple121 = newTuple3(&one, &two, &one);
    Tuple3* tuple112 = newTuple3(&one, &one, &two);
    
    mu_assert("Expect: (1,2,3) == (1,2,3) (by ref)", apply(&eq, 2, (void*[]){tuple123, tuple123}) == &True);
    mu_assert("Expect: (1,2,3) == (1,2,3) (by value)", apply(&eq, 2, (void*[]){tuple123, tuple123a}) == &True);
    mu_assert("Expect: (1,1,1) /= (2,1,1)", apply(&eq, 2, (void*[]){tuple111, tuple211}) == &False);
    mu_assert("Expect: (1,1,1) /= (1,2,1)", apply(&eq, 2, (void*[]){tuple111, tuple121}) == &False);
    mu_assert("Expect: (1,1,1) /= (1,1,2)", apply(&eq, 2, (void*[]){tuple111, tuple112}) == &False);


    Cons* bigList1 = newCons(&one, &Nil);
    Cons* bigList2 = newCons(&one, &Nil);
    Cons* bigList3 = cons2;
    for (u32 i=0; i<123; ++i) {
        bigList1 = newCons(&one, bigList1);
        bigList2 = newCons(&one, bigList2);
        bigList3 = newCons(&one, bigList3);
    }
    mu_assert("Expect: [1,1,1, ... ,1] == [1,1,1, ... ,1]", apply(&eq, 2, (void*[]){bigList1, bigList2}) == &True);
    mu_assert("Expect: [1,1,1, ... ,1] /= [1,1,1, ... ,2]", apply(&eq, 2, (void*[]){bigList1, bigList3}) == &False);


    Custom* custom_1_1A = malloc(sizeof(Custom) + 2*sizeof(void*));
    custom_1_1A->header = HEADER_CUSTOM(2);
    custom_1_1A->ctor = 1;
    custom_1_1A->values[0] = &one;
    custom_1_1A->values[1] = &a1;

    Custom* custom_1_1A_clone = clone(custom_1_1A);

    Custom* custom_2_1A = clone(custom_1_1A);
    custom_2_1A->ctor = 2;

    Custom* custom_1_2A = clone(custom_1_1A);
    custom_1_2A->values[0] = &two;

    Custom* custom_1_1B = clone(custom_1_1A);
    custom_1_1B->values[1] = &b;

    mu_assert("Expect: Ctor1 1 'A' == Ctor1 1 'A' (ref)", apply(&eq, 2, (void*[]){custom_1_1A, custom_1_1A}) == &True);
    mu_assert("Expect: Ctor1 1 'A' == Ctor1 1 'A' (value)", apply(&eq, 2, (void*[]){custom_1_1A, custom_1_1A_clone}) == &True);
    mu_assert("Expect: Ctor1 1 'A' /= Ctor2 1 'A'", apply(&eq, 2, (void*[]){custom_1_1A, custom_2_1A}) == &False);
    mu_assert("Expect: Ctor1 1 'A' /= Ctor1 2 'A'", apply(&eq, 2, (void*[]){custom_1_1A, custom_1_2A}) == &False);
    mu_assert("Expect: Ctor1 1 'A' /= Ctor1 1 'B'", apply(&eq, 2, (void*[]){custom_1_1A, custom_1_1B}) == &False);


    FieldSet* fs = malloc(sizeof(FieldSet) + 2*sizeof(void*));
    fs->size = 2;
    fs->fields[0] = 123;
    fs->fields[1] = 321;

    size_t rsize = sizeof(Record) + 2*sizeof(void*);
    Record* rec12 = malloc(rsize);
    rec12->header = HEADER_RECORD(2);
    rec12->fieldset = fs;
    rec12->values[0] = &one;
    rec12->values[1] = &two;

    Record* rec12a = clone(rec12);
    Record* rec22 = clone(rec12);
    rec22->values[0] = &two;
    Record* rec13 = clone(rec12);
    rec13->values[1] = &three;

    if (verbose) {
        printf("rec12  = %s\n", hex(rec12, rsize));
        printf("rec12a = %s\n", hex(rec12a, rsize));
        printf("rec22  = %s\n", hex(rec22, rsize));
        printf("rec13  = %s\n", hex(rec13, rsize));
    }
    mu_assert("Expect: {a=1, b=2} == {a=1, b=2} (ref)", apply(&eq, 2, (void*[]){rec12, rec12}) == &True);
    mu_assert("Expect: {a=1, b=2} == {a=1, b=2} (value)", apply(&eq, 2, (void*[]){rec12, rec12a}) == &True);
    mu_assert("Expect: {a=1, b=2} /= {a=2, b=2}", apply(&eq, 2, (void*[]){rec12, rec22}) == &False);
    mu_assert("Expect: {a=1, b=2} /= {a=1, b=3}", apply(&eq, 2, (void*[]){rec12, rec13}) == &False);



    return NULL;
}

char* utils_test() {
    mu_run_test(test_records);
    mu_run_test(test_apply);
    mu_run_test(test_eq);

    return NULL;
}
