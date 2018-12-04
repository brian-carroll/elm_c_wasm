#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../kernel/types.h"
#include "../kernel/utils.h"
#include "../kernel/basics.h"
#include "../kernel/gc.h"
#include "../kernel/gc-internals.h"
#include "./test.h"

extern GcState gc_state;


static void reset() {
    memset(gc_state.heap.start, 0,
        gc_state.heap.system_end - gc_state.heap.start
    );
    gc_state.next_alloc = gc_state.heap.start;
    gc_state.roots = &Nil;
    gc_state.stack_depth = 0;

    GcStackMap* p = GC_malloc(sizeof(GcStackMap));
    p->header = HEADER_GC_STACK_EMPTY;
    gc_state.stack_map = p;
}

static bool is_marked(void* p) {
    size_t* pword = (size_t*)p;
    size_t slot = pword - gc_state.heap.start;
    if (slot >> (GC_WORD_BITS-1)) return true; // off heap => not garbage, stop tracing
    size_t word = slot / GC_WORD_BITS;
    size_t bit = slot % GC_WORD_BITS;
    size_t mask = (size_t)1 << bit;

    return (gc_state.heap.bitmap[word] & mask) != 0;
}

void print_value(ElmValue* v) {
    printf("| %p |  %c   |  %2x  | ", v, is_marked(v) ? 'X' : ' ', v->header.size);
    switch (v->header.tag) {
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
            printf("Cons head: %p tail: %p", v->cons.head, v->cons.tail);
            break;
        case Tag_Tuple2:
            printf("Tuple2 a: %p b: %p", v->tuple2.a, v->tuple2.b);
            break;
        case Tag_Tuple3:
            printf("Tuple3 a: %p b: %p c: %p", v->tuple3.a, v->tuple3.b, v->tuple3.c);
            break;
        case Tag_Custom:
            printf("Custom ctor: %d ", v->custom.ctor);
            for (size_t i=0; i<custom_params(&v->custom); ++i) {
                printf("%p ", v->custom.values[i]);
            }
            break;
        case Tag_Record:
            printf("Record fieldset: %p values: ", v->record.fieldset);
            for (size_t i=0; i < v->record.fieldset->size; ++i) {
                printf("%p ", v->record.values[i]);
            }
            break;
        case Tag_Closure:
            printf("Closure n_values: %d max_values: %d evaluator: %p values: ",
                v->closure.n_values, v->closure.max_values, v->closure.evaluator
            );
            for (size_t i=0; i < v->closure.n_values; ++i) {
                printf("%p ", v->record.values[i]);
            }
            break;
        case Tag_GcContinuation:
            printf("GcContinuation %p", v->gc_cont.continuation);
            break;
        case Tag_GcStackPush:
            printf("GcStackPush newer: %p older: %p",
                v->gc_stackmap.newer, v->gc_stackmap.older
            );
            break;
        case Tag_GcStackPop:
            printf("GcStackPop newer: %p older: %p data: %p",
                v->gc_stackmap.newer, v->gc_stackmap.older, v->gc_stackmap.data
            );
            break;
        case Tag_GcStackTailCall:
            printf("GcStackTailCall newer: %p older: %p data: %p",
                v->gc_stackmap.newer, v->gc_stackmap.older, v->gc_stackmap.data
            );
            break;
        case Tag_GcStackEmpty:
            printf("GcStackEmpty");
            break;
    }
    printf("\n");
}

void print_heap(GcState *state) {
    printf("|   Address    | Mark | Size | Value\n");
    printf("| ------------ | ---- | ---- | -----\n");

    size_t* next_value = state->heap.start;
    for (size_t* p = state->heap.start; p < state->next_alloc; p++) {
        if (p == next_value) {
            ElmValue* v = (ElmValue*)p;
            print_value(v);
            if (v->header.size > 0 && v->header.size < 128) {
                next_value += v->header.size;
            } else {
                next_value++;
            }
        } else {
            printf("| %p |  %c   |      |\n", p, is_marked(p) ? 'X' : ' ');
        }
    }
}


void print_state(GcState* state) {
    printf("start = %p\n", gc_state.heap.start);
    printf("end = %p\n", gc_state.heap.end);
    printf("offsets = %p\n", gc_state.heap.offsets);
    printf("bitmap = %p\n", gc_state.heap.bitmap);
    printf("system_end = %p\n", gc_state.heap.system_end);
    printf("next_alloc = %p\n", gc_state.next_alloc);
    printf("roots = %p\n", gc_state.roots);
    printf("stack_map = %p\n", gc_state.stack_map);
    printf("stack_depth = %zd\n", gc_state.stack_depth);

    // find last non-zero word in the bitmap
    size_t bitmap_size = gc_state.heap.system_end - gc_state.heap.bitmap;
    size_t last_word = bitmap_size;
    while (gc_state.heap.bitmap[--last_word] == 0 && last_word != 0) { }

    printf("Bitmap:\n");
    for (size_t word=0; word <= last_word && word < bitmap_size; word++) {
        #ifdef TARGET_64BIT
            printf("%2zx | %016zx\n", word, gc_state.heap.bitmap[word]);
        #else
            printf("%2zx | %08zx\n", word, gc_state.heap.bitmap[word]);
        #endif
    }

    printf("\n");
}

char alive_or_dead_msg[30];
ElmValue* root_mutable_pointer;

char* gc_mark_compact_test() {
    reset();

    if (verbose) {
        printf("\n");
        printf("Stack map\n");
        printf("---------\n");
    }

    void* live[100];
    void* dead[100];
    size_t nlive=0;
    size_t ndead=0;

    if (verbose) {
        printf("GC initial state:\n");
        print_state(&gc_state);
    }

    // Mock Closures.
    // We never evaluate these during the test so they can be anything
    Closure* mock_effect_callback = &Basics_add; // Basics_add is not really an effect callback. It's just a Closure value.
    Closure* mock_closure = &Basics_mul;

    live[nlive++] = gc_state.heap.start; // stack_empty

    // Call the top-level function (an effect callback or incoming port)
    ElmValue* c1 = (ElmValue*)Utils_clone(mock_effect_callback);
    live[nlive++] = gc_state.next_alloc; // the root Cons cell we're about to allocate
    root_mutable_pointer = c1;
    GC_register_root(&root_mutable_pointer); // Effect manager is keeping this Closure alive by connecting it to the GC root.
    live[nlive++] = c1;
    if (verbose) {
        printf("Kernel module registered root:\n  located at %p\n  pointing at %p\n",
            &root_mutable_pointer, root_mutable_pointer
        );
    }

    void* push1 = GC_stack_push();
    live[nlive++] = push1;

    // The currently-running function allocates some stuff.
    // This function won't have returned by end of test, so it needs these values.
    live[nlive] = newElmInt(gc_state.stack_depth*100 + nlive + ndead); nlive++;
    live[nlive] = newElmInt(gc_state.stack_depth*100 + nlive + ndead); nlive++;
    live[nlive] = newElmInt(gc_state.stack_depth*100 + nlive + ndead); nlive++;

    // Push down to level 2. This will complete. Need its return value
    Closure* c2 = Utils_clone(mock_closure);
    live[nlive++] = c2;
    void* push2 = GC_stack_push();
    live[nlive++] = push2;

    // Temporary values from level 2, not in return value
    dead[ndead] = newElmInt(gc_state.stack_depth*100 + nlive + ndead); ndead++;
    dead[ndead] = newElmInt(gc_state.stack_depth*100 + nlive + ndead); ndead++;

    // 3rd level function call. All dead, since we have the return value of a higher level call.
    void* push3 = GC_stack_push();
    dead[ndead++] = push3;
    dead[ndead] = newElmInt(gc_state.stack_depth*100 + nlive + ndead); ndead++;
    dead[ndead] = newElmInt(gc_state.stack_depth*100 + nlive + ndead); ndead++;
    ElmInt* ret3 = newElmInt(gc_state.stack_depth*100 + nlive + ndead);
    dead[ndead++] = ret3;
    dead[ndead++] = gc_state.next_alloc; // the pop we're about to allocate
    GC_stack_pop((ElmValue*)ret3, push3);

    // return value from level 2. Keep it to provide to level 1 on replay
    ElmValue* ret2a = (ElmValue*)newElmInt(gc_state.stack_depth*100 + nlive + ndead);
    ElmValue* ret2b = (ElmValue*)newCons(ret2a, &Nil);
    live[nlive++] = ret2a;
    live[nlive++] = ret2b;
    ElmValue* ret2c = (ElmValue*)newElmInt(gc_state.stack_depth*100 + nlive + ndead);
    ElmValue* ret2d = (ElmValue*)newCons(ret2c, ret2b);
    live[nlive++] = ret2c;
    live[nlive++] = ret2d;

    // Pop back up to top-level function and allocate a few more things.
    // We actually have a choice whether these are considered alive or dead.
    // Implementation treats them as live for consistency and ease of coding
    live[nlive++] = gc_state.next_alloc; // the pop we're about to allocate
    GC_stack_pop(ret2d, push2);
    live[nlive] = newElmInt(gc_state.stack_depth*100 + nlive + ndead); nlive++;
    live[nlive] = newElmInt(gc_state.stack_depth*100 + nlive + ndead); nlive++;
    live[nlive] = newElmInt(gc_state.stack_depth*100 + nlive + ndead); nlive++;

    // Call a function that makes a tail call
    void* push_into_tailrec = GC_stack_push();
    live[nlive++] = push_into_tailrec;
    dead[ndead] = newElmInt(gc_state.stack_depth*100 + nlive + ndead); ndead++;
    dead[ndead] = newElmInt(gc_state.stack_depth*100 + nlive + ndead); ndead++;

    // Tail call. Has completed when we stop the world => dead
    Closure* ctail1 = GC_malloc(sizeof(Closure) + 2*sizeof(void*));
    ctail1->header = HEADER_CLOSURE(2);
    ctail1->n_values = 2;
    ctail1->max_values = 2;
    ctail1->evaluator = NULL; // just a mock. would be a function pointer in real life
    ctail1->values[0] = dead[ndead-1];
    ctail1->values[1] = dead[ndead-2];
    dead[ndead++] = ctail1;
    dead[ndead++] = gc_state.next_alloc; // tailcall we're about to allocate
    GC_stack_tailcall(ctail1, push_into_tailrec);

    // arguments to the next tail call
    live[nlive] = newElmInt(gc_state.stack_depth*100 + nlive + ndead); nlive++;
    live[nlive] = newElmInt(gc_state.stack_depth*100 + nlive + ndead); nlive++;

    // Tail call. still evaluating this when we stopped the world => keep closure alive
    Closure* ctail2 = GC_malloc(sizeof(Closure) + 2*sizeof(void*));
    ctail2->header = HEADER_CLOSURE(2);
    ctail2->n_values = 2;
    ctail2->max_values = 2;
    ctail1->evaluator = NULL; // just a mock. would be a function pointer in real life
    ctail2->values[0] = live[nlive-1];
    ctail2->values[1] = live[nlive-2];
    live[nlive++] = ctail2;
    live[nlive++] = gc_state.next_alloc; // stack tailcall we're about to allocate
    GC_stack_tailcall(ctail2, push_into_tailrec);

    // allocated just before we stopped the world
    live[nlive] = newElmInt(gc_state.stack_depth*100 + nlive + ndead); nlive++;
    live[nlive] = newElmInt(gc_state.stack_depth*100 + nlive + ndead); nlive++;

    if (verbose) printf("Marking...\n");
    size_t* ignore_below = (size_t*)c1;
    mark(&gc_state, ignore_below);
    if (verbose) printf("Finished marking...\n");

    if (verbose) {
        print_heap(&gc_state);
        print_state(&gc_state);
    }

    size_t dead_size = 0;
    for (size_t i=0; i<ndead; i++) {
        sprintf(alive_or_dead_msg, "%p should be dead", dead[i]);
        mu_assert(alive_or_dead_msg, !is_marked(dead[i]));
        ElmValue* v = (ElmValue*)dead[i];
        dead_size += v->header.size;
    }

    size_t live_size = 0;
    for (size_t i=0; i<nlive; i++) {
        sprintf(alive_or_dead_msg, "%p should be live", live[i]);
        mu_assert(alive_or_dead_msg, is_marked(live[i]));
        ElmValue* v = (ElmValue*)live[i];
        live_size += v->header.size;
    }

    size_t heap_size = gc_state.next_alloc - gc_state.heap.start;
    mu_assert("Stack map test should account for all allocated values",
        live_size + dead_size == heap_size
    );

    if (verbose) printf("\n\nCompacting from %p\n\n", ignore_below);
    compact(&gc_state, ignore_below);
    if (verbose) printf("Finished compacting\n");


    if (verbose) printf("\n\nMarking compacted heap...\n\n");
    mark(&gc_state, ignore_below);
    if (verbose) printf("\n\nFinished marking...\n\n");

    if (verbose) {
        printf("\n");
        print_heap(&gc_state);
        print_state(&gc_state);

        printf("next_alloc-start %zd\n", gc_state.next_alloc - gc_state.heap.start);
        printf("live_size before compaction %zd\n", live_size);
        printf("\n");
    }

    mu_assert("Compacted heap should contain exactly the number of live values",
        gc_state.next_alloc - gc_state.heap.start == live_size
    );


    // If 'mark' is able to trace correctly, forwarding addresses are OK
    size_t n_marked = 0;
    for (size_t* w = gc_state.heap.start; w < gc_state.next_alloc; w++) {
        n_marked += is_marked(w);
    }
    mu_assert("After compaction and re-marking, all values should be marked",
        n_marked == live_size
    );

    return NULL;
}


char* gc_struct_test() {

    mu_assert("GcHeap should be the size of 5 pointers", sizeof(GcHeap) == 5*sizeof(void*));
    mu_assert("GcState should be the size of 10 pointers", sizeof(GcState) == 10*sizeof(void*));

    return NULL;
}


char bitmap_msg[100];

char* gc_bitmap_test() {
    char str[] = "This is a test string that's an odd number of ints.....";
    reset();

    for (size_t i=0; i<10; i++) {
        ElmValue *p1, *p2, *p3, *p4;

        p1 = (ElmValue*)newElmInt(1);
        p2 = (ElmValue*)newElmInt(0);
        p3 = (ElmValue*)newElmString(sizeof(str), str);
        p4 = (ElmValue*)newElmInt(0);

        mark_words(&gc_state.heap, p1, p1->header.size);
        mark_words(&gc_state.heap, p3, p1->header.size);

        mu_assert("p1 should be marked", is_marked(p1));
        mu_assert("p2 should NOT be marked", !is_marked(p2));
        mu_assert("p3 should be marked", is_marked(p3));
        mu_assert("p4 should NOT be marked", !is_marked(p4));
    }

    if (verbose) {
        printf("\n");
        printf("Bitmap test\n");
        printf("-----------\n");
        print_heap(&gc_state);
        print_state(&gc_state);
        printf("\n");
    }

    size_t* bottom_of_heap = gc_state.heap.start;
    size_t* top_of_heap = gc_state.next_alloc;

    size_t* ptr = bottom_of_heap;
    size_t* bitmap = gc_state.heap.bitmap;

    size_t w = 0;
    while (ptr <= top_of_heap) {
        size_t word = bitmap[w];
        for (size_t b=0; b < GC_WORD_BITS; b++) {
            bool bitmap_bit = (word & ((size_t)1 << b)) != 0;
            sprintf(bitmap_msg, "is_marked (%d) should match the bitmap (%d)\naddr = %p  word = %zd  bit = %zd",
                is_marked(ptr), bitmap_bit, ptr, w, b
            );
            mu_assert(bitmap_msg, is_marked(ptr) == bitmap_bit);
            ptr++;
        }
        w++;
    }

    return NULL;
}


char* gc_dead_between_test() {
    reset();
    GcState* state = &gc_state;
    GcHeap* heap = &state->heap;

    size_t* first;
    size_t* last;

    heap->bitmap[0] = 0xf0f;
    first = heap->start + 4;
    last = heap->start + 8;
    mu_assert("bitmap_dead_between with 4 words dead",
        bitmap_dead_between(heap, first, last) == 4
    );

    first--;
    last++;

    mu_assert("bitmap_dead_between with 4 dead and 2 live",
        bitmap_dead_between(heap, first, last) == 4
    );

    heap->bitmap[0] = 0xf0;
    heap->bitmap[1] = 0x00;
    heap->bitmap[2] = 0xf0;
    first = heap->start + 2;
    last = heap->start + (2*GC_WORD_BITS) + 10;

    mu_assert("bitmap_dead_between across 3 bitmap words",
        bitmap_dead_between(heap, first, last)
        == ((GC_WORD_BITS-2-4) + GC_WORD_BITS + 10-4)
    );

    return NULL;
}



char* gc_test() {
    if (verbose) {
        printf("\n");
        printf("GC\n");
        printf("--\n");
    }

    mu_run_test(gc_struct_test);
    mu_run_test(gc_bitmap_test);
    mu_run_test(gc_dead_between_test);
    mu_run_test(gc_mark_compact_test);

    return NULL;
}
