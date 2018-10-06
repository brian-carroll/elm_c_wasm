#include <stdio.h>
#include <stdlib.h>
#include "../src/utils.h"
#include "../src/basics.h"
#include "./test.c" // including .c file, naughty!

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

void test_closure_example1(void) {
    Closure user_project_closure = (Closure) {
        .header = HEADER_CLOSURE(0),
        .evaluator = &eval_user_project_closure,
        .max_values = 3,
        .n_values = 0
        // .values = {}
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


void test_eq(void) {
    printf("Unit==Unit : %s\n", *eq(&Unit, &Unit) ? "True" : "False" );

    printf("True==True : %s\n", *eq(&True, &True) ? "True" : "False" );
    printf("False==False : %s\n", *eq(&False, &False) ? "True" : "False" );
    printf("True==False : %s\n", *eq(&True, &False) ? "True" : "False" );
    printf("False==True : %s\n", *eq(&False, &True) ? "True" : "False" );

    ElmInt two = (ElmInt){ .header = HEADER_INT, .value = 2 };
    ElmInt three = (ElmInt){ .header = HEADER_INT, .value = 3 };

    printf("2==2 : %s\n", *eq(&two, &two) ? "True" : "False" );
    printf("2==3 : %s\n", *eq(&two, &three) ? "True" : "False" );

    printf("True==3 : %s\n", *eq(&True, &three) ? "True" : "False" );

    ElmFloat *f = newElmFloat(123.456);
    printf("123.456==123.456 : %s (ref)\n", *eq(f, f) ? "True" : "False" );
    printf("123.456==123.456 : %s (val)\n", *eq(f, newElmFloat(123.456)) ? "True" : "False" );
    printf("123.456==1.0 : %s\n", *eq(f, newElmFloat(1.0)) ? "True" : "False" );

    printf("A==A : %s\n", *eq(newElmChar('A'), newElmChar('A')) ? "True" : "False" );
    printf("A==B : %s\n", *eq(newElmChar('A'), newElmChar('B')) ? "True" : "False" );

    ElmString* hello1 = newElmString(5, "hello");
    ElmString* hello2 = newElmString(5, "hello");
    ElmString* hello_ = newElmString(6, "hello_");
    ElmString* world = newElmString(5, "world");

    printf("ref equal hello==hello : %s\n", *eq(hello1, hello1) ? "True" : "False" );
    printf("val equal hello==hello : %s\n", *eq(hello1, hello2) ? "True" : "False" );
    printf("diff length hello==hello_ : %s\n", *eq(hello1, hello_) ? "True" : "False" );
    printf("diff value hello_==world : %s\n", *eq(hello_, world) ? "True" : "False" );
}


int main(int argc, char ** argv) {
    basics_init();
    test_closure_example1();
    test_eq();
}
