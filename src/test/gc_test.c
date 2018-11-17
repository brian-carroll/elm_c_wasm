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
                // Assume that Custom type objects are stack map objects
                // Mostly true in GC tests if not in 'real life'
                switch (v->custom.ctor) {
                    case 0: printf("GcStackEmpty"); break;
                    case 1: printf("GcStackPush"); break;
                    case 2: printf("GcStackPop"); break;
                    case 3: printf("GcStackTailCall"); break;
                }
                printf(" values= ");
                for (u32 i=0; i<custom_params(&v->custom); ++i) {
                    printf("%llx ", (u64)v->custom.values[i]);
                }
                break;
            case Tag_Record:
                printf("Record fieldset=%llx values= ", (u64)v->record.fieldset);
                for (u32 i=0; i < v->record.fieldset->size; ++i) {
                    printf("%llx ", (u64)v->record.values[i]);
                }
                break;
            case Tag_Closure:
                printf("Closure n_values=%d max_values=%d evaluator=%llx values= ",
                    v->closure.n_values, v->closure.max_values, (u64)v->closure.evaluator
                );
                for (u32 i=0; i < v->closure.n_values; ++i) {
                    printf("%llx ", (u64)v->record.values[i]);
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
    if (verbose) {
        printf("\n");
        printf("Stack map\n");
        printf("---------\n");
    }

    void* live[100];
    void* dead[100];
    u32 nlive=0, ndead=0;


    GcState* state = GC_init();
    if (verbose) {
        printf("GC initial state:\n");
        print_state(state);
    }

    Closure* c1 = GC_allocate(sizeof(Closure));
    memcpy(c1, &Basics_add, sizeof(Closure));
    live[nlive++] = c1;
    void* push1 = GC_stack_push(c1);
    live[nlive++] = push1;
    live[nlive++] = newElmInt(state->stack_depth);
    live[nlive++] = newElmInt(state->stack_depth);
    live[nlive++] = newElmInt(state->stack_depth);

    Closure* c2 = GC_allocate(sizeof(Closure));
    memcpy(c2, &Basics_mul, sizeof(Closure));
    live[nlive++] = c2;
    void* push2 = GC_stack_push(c2);
    live[nlive++] = push2;
    dead[ndead++] = newElmInt(state->stack_depth);
    dead[ndead++] = newElmInt(state->stack_depth);
    
    void* push3 = GC_stack_push(&Basics_sub);
    dead[ndead++] = push3;
    dead[ndead++] = newElmInt(state->stack_depth);
    dead[ndead++] = newElmInt(state->stack_depth);
    ElmInt* ret3 = newElmInt(state->stack_depth);
    dead[ndead++] = ret3;
    dead[ndead++] = state->current_heap; // the pop we're about to allocate
    GC_stack_pop(ret3, push3);


    ElmValue* ret2a = (ElmValue*)newElmInt(state->stack_depth);
    ElmValue* ret2b = (ElmValue*)newCons(ret2a, &Nil);
    ElmValue* ret2c = (ElmValue*)newElmInt(state->stack_depth);
    ElmValue* ret2d = (ElmValue*)newCons(ret2c, ret2b);
    live[nlive++] = ret2a;
    live[nlive++] = ret2b;
    live[nlive++] = ret2c;
    live[nlive++] = ret2d;
    live[nlive++] = state->current_heap; // the pop we're about to allocate

    GC_stack_pop(ret2d, push2);
    live[nlive++] = newElmInt(state->stack_depth);
    live[nlive++] = newElmInt(state->stack_depth);
    live[nlive++] = newElmInt(state->stack_depth);

    if (verbose) {
        printf("GC final state:\n");
        print_state(state);
    }

    ElmValue* ignore_below = (ElmValue*)c1;
    mark_stack_map(ignore_below);

    if (verbose) {
        printf("Final heap state:\n");
        print_heap(state);
    }
    u32 tested_size = 0;
    char* msg;
    while (ndead--) {
        ElmValue* v = (ElmValue*)dead[ndead];
        msg = malloc(30);
        sprintf(msg, "%llx should be dead", (u64)v);
        mu_assert(msg, v->header.gc_mark == 0);
        tested_size += v->header.size;
    }

    while (nlive--) {
        ElmValue* v = (ElmValue*)live[nlive];
        msg = malloc(30);
        sprintf(msg, "%llx should be live", (u64)v);
        mu_assert(msg, v->header.gc_mark == 1);
        tested_size += v->header.size;
    }

    u32 heap_size = state->current_heap - state->pages[0].data;
    mu_assert("Stack map test should account for all allocated values",
        tested_size == heap_size
    );

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
