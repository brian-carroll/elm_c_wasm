#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../kernel/types.h"
#include "../kernel/utils.h"
#include "../kernel/basics.h"
#include "../kernel/gc.h"
#include "./test.h"


// Predeclare internal functions from gc.c tested here
void mark(ElmValue* ignore_below);
bool is_marked(void* p);
void mark_value(void* p);


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
        printf("| %12llx |  %c   |  %2d  | ", (u64)v, is_marked(v) ? 'X' : ' ', v->header.size);
        switch (h->tag) {
            case Tag_Int:
                printf("Int %d", v->elm_int.value);
                break;
            case Tag_Float:
                printf("Float %f", v->elm_float.value);
                break;
            case Tag_Char:
                printf("Char 0x%8x", v->elm_char.value);
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
                printf(" values: ");
                for (u32 i=0; i<custom_params(&v->custom); ++i) {
                    printf("%llx ", (u64)v->custom.values[i]);
                }
                break;
            case Tag_Record:
                printf("Record fieldset=%llx values: ", (u64)v->record.fieldset);
                for (u32 i=0; i < v->record.fieldset->size; ++i) {
                    printf("%llx ", (u64)v->record.values[i]);
                }
                break;
            case Tag_Closure:
                printf("Closure n_values=%d max_values=%d evaluator=%llx values: ",
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
    printf("bottom of heap=%llx\n", (u64)state->pages[0].data);

    // find last non-zero word in the bitmap
    u32 bitmap_size = GC_PAGE_SLOTS/GC_BITMAP_WORDSIZE;
    u32 last_word = bitmap_size;
    while (state->pages[0].bitmap[--last_word] == 0 && last_word != 0) { }

    printf("Bitmap:\n");
    for (u32 word=0; word <= last_word && word < bitmap_size; word++) {
        printf("%2x | %016llx\n", word, state->pages[0].bitmap[word]);
    }

    printf("\n");
}

char alive_or_dead_msg[30];

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

    // Mock Closures.
    // We never evaluate these during the test so they can be anything
    Closure* mock_effect_callback = &Basics_add; // Basics_add is not really an effect callback. It's just a Closure value.
    Closure* mock_closure = &Basics_mul;


    // Call the top-level function (an effect callback or incoming port)
    ElmValue* c1 = (ElmValue*)Utils_clone(mock_effect_callback);
    live[nlive++] = state->current_heap; // the root Cons cell we're about to allocate
    GC_register_root(&c1); // Effect manager is keeping this Closure alive by connecting it to the GC root.
    live[nlive++] = c1;

    void* push1 = GC_stack_push();
    live[nlive++] = push1;

    // The currently-running function allocates some stuff.
    // This function won't have returned by end of test, so it needs these values.
    live[nlive++] = newElmInt(state->stack_depth);
    live[nlive++] = newElmInt(state->stack_depth);
    live[nlive++] = newElmInt(state->stack_depth);

    // Push down to level 2. This will complete. Need its return value
    Closure* c2 = Utils_clone(mock_closure);
    live[nlive++] = c2;
    void* push2 = GC_stack_push();
    live[nlive++] = push2;

    // Temporary values from level 2, not in return value
    dead[ndead++] = newElmInt(state->stack_depth);
    dead[ndead++] = newElmInt(state->stack_depth);

    // 3rd level function call. All dead, since we have the return value of a higher level call.
    void* push3 = GC_stack_push();
    dead[ndead++] = push3;
    dead[ndead++] = newElmInt(state->stack_depth);
    dead[ndead++] = newElmInt(state->stack_depth);
    ElmInt* ret3 = newElmInt(state->stack_depth);
    dead[ndead++] = ret3;
    dead[ndead++] = state->current_heap; // the pop we're about to allocate
    GC_stack_pop((ElmValue*)ret3, push3);

    // return value from level 2. Keep it to provide to level 1 on replay
    ElmValue* ret2a = (ElmValue*)newElmInt(state->stack_depth);
    ElmValue* ret2b = (ElmValue*)newCons(ret2a, &Nil);
    ElmValue* ret2c = (ElmValue*)newElmInt(state->stack_depth);
    ElmValue* ret2d = (ElmValue*)newCons(ret2c, ret2b);
    live[nlive++] = ret2a;
    live[nlive++] = ret2b;
    live[nlive++] = ret2c;
    live[nlive++] = ret2d;

    // Pop back up to top-level function and allocate a few more things.
    // We actually have a choice whether these are considered alive or dead.
    // Implementation treats them as live for consistency and ease of coding
    live[nlive++] = state->current_heap; // the pop we're about to allocate
    GC_stack_pop(ret2d, push2);
    live[nlive++] = newElmInt(state->stack_depth);
    live[nlive++] = newElmInt(state->stack_depth);
    live[nlive++] = newElmInt(state->stack_depth);

    // Call a function that makes a tail call
    void* push_into_tailrec = GC_stack_push();
    live[nlive++] = push_into_tailrec;
    dead[ndead++] = newElmInt(state->stack_depth);
    dead[ndead++] = newElmInt(state->stack_depth);

    // Tail call. Has completed when we stop the world => dead
    Closure* ctail1 = GC_allocate(sizeof(Closure) + 2*sizeof(void*));
    ctail1->header = HEADER_CLOSURE(2);
    ctail1->n_values = 2;
    ctail1->max_values = 2;
    ctail1->evaluator = NULL; // just a mock. would be a function pointer in real life
    ctail1->values[0] = dead[ndead-1];
    ctail1->values[1] = dead[ndead-2];
    dead[ndead++] = ctail1;
    dead[ndead++] = state->current_heap; // tailcall we're about to allocate
    GC_stack_tailcall(ctail1, push_into_tailrec);

    // arguments to the next tail call
    live[nlive++] = newElmInt(state->stack_depth);
    live[nlive++] = newElmInt(state->stack_depth);

    // Tail call. still evaluating this when we stopped the world => keep closure alive
    Closure* ctail2 = GC_allocate(sizeof(Closure) + 2*sizeof(void*));
    ctail2->header = HEADER_CLOSURE(2);
    ctail2->n_values = 2;
    ctail2->max_values = 2;
    ctail1->evaluator = NULL; // just a mock. would be a function pointer in real life
    ctail2->values[0] = live[nlive-1];
    ctail2->values[1] = live[nlive-2];
    live[nlive++] = ctail2;
    live[nlive++] = state->current_heap; // stack tailcall we're about to allocate
    GC_stack_tailcall(ctail2, push_into_tailrec);

    // allocated just before we stopped the world
    live[nlive++] = newElmInt(state->stack_depth);
    live[nlive++] = newElmInt(state->stack_depth);


    ElmValue* ignore_below = (ElmValue*)c1;
    mark(ignore_below);

    if (verbose) {
        printf("Final heap state:\n");
        print_heap(state);
        printf("\n");
        printf("GC final state:\n");
        print_state(state);
    }

    u32 tested_size = 0;
    while (ndead--) {
        ElmValue* v = (ElmValue*)dead[ndead];
        sprintf(alive_or_dead_msg, "%llx should be dead", (u64)v);
        mu_assert(alive_or_dead_msg, !is_marked(v));
        tested_size += v->header.size;
    }

    while (nlive--) {
        ElmValue* v = (ElmValue*)live[nlive];
        sprintf(alive_or_dead_msg, "%llx should be live", (u64)v);
        mu_assert(alive_or_dead_msg, is_marked(v));
        tested_size += v->header.size;
    }

    u32 heap_size = state->current_heap - state->pages[0].data;
    mu_assert("Stack map test should account for all allocated values",
        tested_size == heap_size
    );

    return NULL;
}


char* gc_page_struct_test() {
    GcState* state = GC_init();

    size_t size_bitmap = sizeof(state->pages[0].bitmap);
    size_t size_offsets = sizeof(state->pages[0].offsets);
    size_t size_data = sizeof(state->pages[0].data);
    size_t size_unused = sizeof(state->pages[0].unused);

    if (verbose) {
        printf("\n");
        printf("GC_PAGE_BYTES = %d\n", GC_PAGE_BYTES);
        printf("GC_BLOCK_BYTES = %d\n", GC_BLOCK_BYTES);
        printf("GC_PAGE_BLOCKS = %ld\n", GC_PAGE_BLOCKS);
        printf("GC_PAGE_SLOTS = %ld\n", GC_PAGE_SLOTS);
        printf("\n");
        printf("sizeof(bitmap) = %ld\n", size_bitmap);
        printf("sizeof(offsets) = %ld\n", size_offsets);
        printf("sizeof(data) = %ld\n", size_data);
        printf("sizeof(unused) = %ld\n", size_unused);
        printf("sizeof(GcPage) = %ld\n", sizeof(GcPage));
        printf("\n");
        printf("total overhead = %3.2f%%\n", 100.0-(100.0*size_data/GC_PAGE_BYTES));
    }

    mu_assert("Bitmap should have one bit for each int of data",
        size_bitmap*32 == size_data     // no integer division here
    );
    mu_assert("GcPage should have correct size",
        sizeof(GcPage) == GC_PAGE_BYTES
    );

    return NULL;
}


char* gc_bitmap_test() {
    char str[] = "This is a test string that's an odd number of ints.....";

    GcState* state = GC_init();

    for (u32 i=0; i<10; i++) {
        void *p1, *p2, *p3, *p4;

        p1 = newElmInt(1);
        p2 = newElmInt(0);
        p3 = newElmString(sizeof(str), str);
        p4 = newElmInt(0);

        mark_value(p1);
        mark_value(p3);

        mu_assert("p1 should be marked", is_marked(p1));
        mu_assert("p2 should NOT be marked", !is_marked(p2));
        mu_assert("p3 should be marked", is_marked(p3));
        mu_assert("p4 should NOT be marked", !is_marked(p4));
    }

    if (verbose) {
        printf("\n");
        printf("Bitmap test\n");
        printf("-----------\n");
        print_heap(state);
        print_state(state);
        printf("\n");
    }

    u32* bottom_of_heap = state->pages[0].data;
    u32* top_of_heap = state->current_heap;

    u32* ptr = bottom_of_heap;
    u64* bitmap = state->pages[0].bitmap;

    u32 w = 0;
    while (ptr <= top_of_heap) {
        u64 word = bitmap[w];
        for (u32 b=0; b<64; b++) {
            bool bitmap_bit = (word & ((u64)1 << b)) != 0;
            mu_assert("is_marked should match the bitmap", is_marked(ptr) == bitmap_bit);
            ptr++;
        }
        w++;
    }

    return NULL;
}


char* gc_test() {
    if (verbose) {
        printf("\n");
        printf("GC\n");
        printf("--\n");
    }

    mu_run_test(gc_page_struct_test);
    mu_run_test(gc_bitmap_test);
    mu_run_test(gc_stackmap_test);
    return NULL;
}
