#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../kernel/types.h"
#include "../kernel/utils.h"
#include "../kernel/basics.h"
#include "../kernel/gc.h"
#include "./test.h"


/*
    stack map
        create by calling apply
        need a sample Elm program
            a few levels of calls
            different numbers of calls
            some allocations
            self tail calls
            mutual tail calls
        Ideally some kind of generator like jasmine marbles
        Maybe one evaluator that parses a string to know what to do
        Configure each function evaluator
            [a,a,a,c(1),a,a,a,c(2),a,c(3),a,a,a,c(self)]
        Configure the point at which it bails
            After the 3rd allocation in function 4

    Wait...
    What errors am I trying to catch?
        Stupid coding mistakes due to C being C
        Don't get carried away
        Design tests bottom-up

    Stack map
        objects are created as expected
        If I create a bunch of stackmap items in a row they are linked
        If I create other objects in-between, they are skipped

    Mark stack map
        Create a stack map
        Create some junk
        Mark it
        Check the right things are marked

    For each test, create a new heap, then fill it with what you want

*/

void print_heap(GcState *state) {

    Header* from = (Header*)state->pages[0].data;
    Header* to = (Header*)state->current_heap;

    printf("|   Address    | Mark | Size | Value\n");
    printf("| ------------ | ---- | ---- | -----\n");
    for (Header* h = from ; h < to ; h += h->size) {
        if (h->size == 0) {
            printf("Zero-size object at %llx = %s\n", (u64)h, hex(h,4));
            printf("Aborting print_heap\n");
            return;
        }
        ElmValue* v = (ElmValue*)h;
        printf("| %llx |  %c   |  %2d  | ", (u64)v, v->header.gc_mark ? 'X' : ' ', v->header.size);
        switch (h->tag) {
            case Tag_Int:
                printf("Int %d", v->elm_int.value);
                break;
            case Tag_Float:
                printf("Float %f", v->elm_float.value);
                break;
            case Tag_Char:
                printf("Char 0x%2x", v->elm_char.value);
                break;
            case Tag_String:
                printf("String \"%s\"", v->elm_string.bytes);
                break;
            case Tag_Nil:
                printf("Nil");
                break;
            case Tag_Cons:
                printf("Cons head=%llx tail=%llx", (u64)v->cons.head, (u64)v->cons.tail);
                break;
            case Tag_Tuple2:
                printf("Tuple2 a=%llx b=%llx", (u64)v->tuple2.a, (u64)v->tuple2.b);
                break;
            case Tag_Tuple3:
                printf("Tuple3 a=%llx b=%llx c=%llx", (u64)v->tuple3.a, (u64)v->tuple3.b, (u64)v->tuple3.c);
                break;
            case Tag_Custom:
                printf("Custom ctor=%d values=", v->custom.ctor);
                for (u32 i=0; i<custom_params(&v->custom); ++i) {
                    printf("%llx,", (u64)v->custom.values[i]);
                }
                break;
            case Tag_Record:
                printf("Record fieldset=%llx values=", (u64)v->record.fieldset);
                for (u32 i=0; i < v->record.fieldset->size; ++i) {
                    printf("%llx,", (u64)v->record.values[i]);
                }
                break;
            case Tag_Closure:
                printf("Closure n_values=%d max_values=%d evaluator=%llx values=",
                    v->closure.n_values, v->closure.max_values, (u64)v->closure.evaluator
                );
                for (u32 i=0; i < v->closure.n_values; ++i) {
                    printf("%llx,", (u64)v->record.values[i]);
                }
                break;
            case Tag_GcFull:
                printf("GcFull continuation=%llx", (u64)v->gc_full.continuation);
                break;
        }
        printf("\n");
    }
}


void print_state(GcState* state) {
    printf("system_max_heap=%llx\n", (u64)state->system_max_heap);
    printf("max_heap=%llx\n", (u64)state->max_heap);
    printf("current_heap=%llx\n", (u64)state->current_heap);
    printf("roots=%llx\n", (u64)state->roots);
    printf("stack_map=%llx\n", (u64)state->stack_map);
    printf("stack_depth=%d\n", state->stack_depth);
    printf("pages[0]=%llx\n", (u64)state->pages);
    printf("\n");
}


char* gc_stackmap_test() {
    printf("\n");
    printf("Stack map\n");
    printf("---------\n");

    GcState* state = GC_init();
    printf("GC initial state:\n");
    print_state(state);

    Closure* c1 = GC_allocate(sizeof(Closure));
    memcpy(c1, &Basics_add, sizeof(Closure));
    // void* push1 = 
    GC_stack_push(c1);
    newElmInt(123);
    newElmInt(123);
    newElmInt(123);

    Closure* c2 = GC_allocate(sizeof(Closure));
    memcpy(c2, &Basics_mul, sizeof(Closure));
    void* push2 = GC_stack_push(c2);
    newElmInt(123);
    newElmInt(123);
    Cons* ret2 = newCons(
        newElmInt(456),
        newCons(
            newElmInt(456),
            &Nil
        )
    );

    GC_stack_pop(ret2, push2);
    newElmInt(123);
    newElmInt(123);
    newElmInt(123);

    printf("GC final state:\n");
    print_state(state);

    printf("Marking stack map:\n");
    ElmValue* ignore_below = (ElmValue*)c1;
    mark_stack_map(ignore_below);
    printf("\n");

    printf("Final heap state:\n");
    print_heap(state);

    return NULL;
}



char* gc_mark_test() {


    return NULL;
}


char* gc_test() {
    if (verbose) {
        printf("\n");
        printf("GC\n");
        printf("--\n");
    }

    mu_run_test(gc_stackmap_test);
    mu_run_test(gc_mark_test);
    return NULL;
}
