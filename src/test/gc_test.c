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

struct fn
{
    void *evaluator;
    char *name;
};
#define NUM_FUNC_NAMES 6
struct fn func_map[NUM_FUNC_NAMES];

#define MAX_STACKMAP_NAMES 100
struct sn
{
    GcStackMap *stackmap;
    char *name;
} stackmap_names[MAX_STACKMAP_NAMES];

char *find_stackmap_func_name(GcStackMap *sm)
{
    char *name = "unknown";
    for (int i = 0; i < MAX_STACKMAP_NAMES; i++)
    {
        if (stackmap_names[i].stackmap == sm)
        {
            name = stackmap_names[i].name;
        }
    }
    return name;
}

char *find_closure_func_name(Closure *c)
{
    char *name = "unknown";
    for (int i = 0; i < NUM_FUNC_NAMES; i++)
    {
        if (func_map[i].evaluator == c->evaluator)
        {
            name = func_map[i].name;
        }
    }
    return name;
}

void gc_test_reset()
{
    GcState *state = &gc_state;
    size_t *bm_word = state->heap.start;
    while (bm_word < state->heap.system_end)
    {
        *bm_word = 0;
        bm_word++;
    }
    state->next_alloc = state->heap.start;
    state->roots = &Nil;
    state->stack_depth = 0;

    GcStackMap *p = GC_malloc(sizeof(GcStackMap));
    p->header = HEADER_GC_STACK_EMPTY;
    state->stack_map = p;
}

static bool is_marked(void *p)
{
    GcState *state = &gc_state;
    size_t *pword = (size_t *)p;
    size_t slot = pword - state->heap.start;
    if (slot >> (GC_WORD_BITS - 1))
        return true; // off heap => not garbage, stop tracing
    size_t word = slot / GC_WORD_BITS;
    size_t bit = slot % GC_WORD_BITS;
    size_t mask = (size_t)1 << bit;

    return (state->heap.bitmap[word] & mask) != 0;
}

void print_value(ElmValue *v)
{
    printf("| %p |  %c   |%5d | ", v, is_marked(v) ? 'X' : ' ', v->header.size);
    switch (v->header.tag)
    {
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
        for (size_t i = 0; i < custom_params(&v->custom); ++i)
        {
            printf("%p ", v->custom.values[i]);
        }
        break;
    case Tag_Record:
        printf("Record fieldset: %p values: ", v->record.fieldset);
        for (size_t i = 0; i < v->record.fieldset->size; ++i)
        {
            printf("%p ", v->record.values[i]);
        }
        break;
    case Tag_Closure:
        printf("Closure (%s) n_values: %d max_values: %d values: ",
               find_closure_func_name(&v->closure),
               v->closure.n_values, v->closure.max_values);
        for (size_t i = 0; i < v->closure.n_values; ++i)
        {
            printf("%p ", v->closure.values[i]);
        }
        break;
    case Tag_GcException:
        printf("GcException");
        break;
    case Tag_GcStackPush:
        printf("GcStackPush (%s) newer: %p older: %p",
               find_stackmap_func_name(&v->gc_stackmap),
               v->gc_stackmap.newer, v->gc_stackmap.older);
        break;
    case Tag_GcStackPop:
        printf("GcStackPop (%s) newer: %p older: %p replay: %p",
               find_stackmap_func_name(&v->gc_stackmap),
               v->gc_stackmap.newer, v->gc_stackmap.older, v->gc_stackmap.replay);
        break;
    case Tag_GcStackTailCall:
        printf("GcStackTailCall newer: %p older: %p replay: %p",
               v->gc_stackmap.newer, v->gc_stackmap.older, v->gc_stackmap.replay);
        break;
    case Tag_GcStackEmpty:
        printf("GcStackEmpty newer: %p", v->gc_stackmap.newer);
        break;
    }
    printf("\n");
}

void print_heap(GcState *state)
{
#ifdef TARGET_64BIT
    printf("|    Address     | Mark | Size | Value\n");
    printf("| -------------- | ---- | ---- | -----\n");

#else
    printf("| Address  | Mark | Size | Value\n");
    printf("| -------- | ---- | ---- | -----\n");
#endif

    size_t *first_value = state->heap.start;
    ElmValue *v = (ElmValue *)first_value;
    if (v->header.tag == Tag_GcStackEmpty)
    {
        print_value(v);
        first_value += v->header.size;
    }
    // skip junk zeros used to force GC in tests
    size_t *zeros = first_value;
    while (*first_value == 0)
        first_value++;

    if (first_value > zeros)
    {
        printf("| %p |      |%5zd | (zeros)\n", zeros, first_value - zeros);
    }

    size_t *next_value = first_value;
    for (size_t *p = first_value; p < state->next_alloc; p++)
    {
        if (p == next_value)
        {
            v = (ElmValue *)p;
            print_value(v);
            if (v->header.size > 0 && v->header.size < 128)
            {
                next_value += v->header.size;
            }
            else
            {
                next_value++;
            }
        }
        else
        {
            printf("| %p |  %c   |      |\n", p, is_marked(p) ? 'X' : ' ');
        }
    }
}

void print_state(GcState *state)
{
    printf("start = %p\n", state->heap.start);
    printf("end = %p\n", state->heap.end);
    printf("offsets = %p\n", state->heap.offsets);
    printf("bitmap = %p\n", state->heap.bitmap);
    printf("system_end = %p\n", state->heap.system_end);
    printf("next_alloc = %p\n", state->next_alloc);
    printf("roots = %p\n", state->roots);
    printf("stack_map = %p\n", state->stack_map);
    printf("stack_depth = %zd\n", state->stack_depth);

    // find last non-zero word in the bitmap
    size_t bitmap_size = state->heap.system_end - state->heap.bitmap;
    size_t last_word = bitmap_size;

    printf("Bitmap (size %zd):\n", bitmap_size);
    for (size_t word = 0; word <= last_word && word < bitmap_size; word++)
    {
        size_t value = state->heap.bitmap[word];
        if (value)
        {
#ifdef TARGET_64BIT
            printf("%3zd | %016zx\n", word, value);
#else
            printf("%3zd | %08zx\n", word, value);
#endif
        }
    }

    printf("\n");
}

char alive_or_dead_msg[30];
ElmValue *root_mutable_pointer;

char *gc_mark_compact_test()
{
    GcState *state = &gc_state;
    gc_test_reset();

    if (verbose)
    {
        printf("\n");
        printf("########################################################################################\n");
        printf("gc_mark_compact_test\n");
        printf("--------------------\n");
        printf("\n");
    }

    void *live[100];
    void *dead[100];
    size_t nlive = 0;
    size_t ndead = 0;

    if (verbose)
    {
        printf("GC initial state:\n");
        print_state(&gc_state);
    }

    // Mock Closures.
    // We never evaluate these during the test so they can be anything
    Closure *mock_effect_callback = &Basics_add; // Basics_add is not really an effect callback. It's just a Closure value.
    Closure *mock_closure = &Basics_mul;

    live[nlive++] = state->heap.start; // stack_empty

    // Call the top-level function (an effect callback or incoming port)
    ElmValue *c1 = (ElmValue *)Utils_clone(mock_effect_callback);
    live[nlive++] = state->next_alloc; // the root Cons cell we're about to allocate
    root_mutable_pointer = c1;
    GC_register_root(&root_mutable_pointer); // Effect manager is keeping this Closure alive by connecting it to the GC root.
    live[nlive++] = c1;
    if (verbose)
    {
        printf("Kernel module registered root:\n  located at %p\n  pointing at %p\n",
               &root_mutable_pointer, root_mutable_pointer);
    }

    void *push1 = GC_stack_push();
    live[nlive++] = push1;

    // The currently-running function allocates some stuff.
    // This function won't have returned by end of test, so it needs these values.
    live[nlive] = NEW_ELM_INT(state->stack_depth * 100 + nlive + ndead);
    nlive++;
    live[nlive] = NEW_ELM_INT(state->stack_depth * 100 + nlive + ndead);
    nlive++;
    live[nlive] = NEW_ELM_INT(state->stack_depth * 100 + nlive + ndead);
    nlive++;

    // Push down to level 2. This will complete. Need its return value
    Closure *c2 = Utils_clone(mock_closure);
    live[nlive++] = c2;
    void *push2 = GC_stack_push();
    live[nlive++] = push2;

    // Temporary values from level 2, not in return value
    dead[ndead] = NEW_ELM_INT(state->stack_depth * 100 + nlive + ndead);
    ndead++;
    dead[ndead] = NEW_ELM_INT(state->stack_depth * 100 + nlive + ndead);
    ndead++;

    // 3rd level function call. All dead, since we have the return value of a higher level call.
    void *push3 = GC_stack_push();
    dead[ndead++] = push3;
    dead[ndead] = NEW_ELM_INT(state->stack_depth * 100 + nlive + ndead);
    ndead++;
    dead[ndead] = NEW_ELM_INT(state->stack_depth * 100 + nlive + ndead);
    ndead++;
    ElmInt *ret3 = NEW_ELM_INT(state->stack_depth * 100 + nlive + ndead);
    dead[ndead++] = ret3;
    dead[ndead++] = state->next_alloc; // the pop we're about to allocate
    GC_stack_pop((ElmValue *)ret3, push3);

    // return value from level 2. Keep it to provide to level 1 on replay
    ElmValue *ret2a = (ElmValue *)NEW_ELM_INT(state->stack_depth * 100 + nlive + ndead);
    ElmValue *ret2b = (ElmValue *)NEW_CONS(ret2a, &Nil);
    live[nlive++] = ret2a;
    live[nlive++] = ret2b;
    ElmValue *ret2c = (ElmValue *)NEW_ELM_INT(state->stack_depth * 100 + nlive + ndead);
    ElmValue *ret2d = (ElmValue *)NEW_CONS(ret2c, ret2b);
    live[nlive++] = ret2c;
    live[nlive++] = ret2d;

    // Pop back up to top-level function and allocate a few more things.
    // We actually have a choice whether these are considered alive or dead.
    // Implementation treats them as live for consistency and ease of coding
    live[nlive++] = state->next_alloc; // the pop we're about to allocate
    GC_stack_pop(ret2d, push2);
    live[nlive] = NEW_ELM_INT(state->stack_depth * 100 + nlive + ndead);
    nlive++;
    live[nlive] = NEW_ELM_INT(state->stack_depth * 100 + nlive + ndead);
    nlive++;
    live[nlive] = NEW_ELM_INT(state->stack_depth * 100 + nlive + ndead);
    nlive++;

    // Call a function that makes a tail call
    void *push_into_tailrec = GC_stack_push();
    live[nlive++] = push_into_tailrec;
    dead[ndead] = NEW_ELM_INT(state->stack_depth * 100 + nlive + ndead);
    ndead++;
    dead[ndead] = NEW_ELM_INT(state->stack_depth * 100 + nlive + ndead);
    ndead++;

    // Tail call. Has completed when we stop the world => dead
    Closure *ctail1 = GC_malloc(sizeof(Closure) + 2 * sizeof(void *));
    ctail1->header = HEADER_CLOSURE(2);
    ctail1->n_values = 2;
    ctail1->max_values = 2;
    ctail1->evaluator = Basics_sub.evaluator;
    ctail1->values[0] = dead[ndead - 1];
    ctail1->values[1] = dead[ndead - 2];
    dead[ndead++] = ctail1;
    dead[ndead++] = state->next_alloc; // tailcall we're about to allocate
    GC_stack_tailcall(ctail1, push_into_tailrec);

    // arguments to the next tail call
    live[nlive] = NEW_ELM_INT(state->stack_depth * 100 + nlive + ndead);
    nlive++;
    live[nlive] = NEW_ELM_INT(state->stack_depth * 100 + nlive + ndead);
    nlive++;

    // Tail call. still evaluating this when we stopped the world => keep closure alive
    Closure *ctail2 = GC_malloc(sizeof(Closure) + 2 * sizeof(void *));
    ctail2->header = HEADER_CLOSURE(2);
    ctail2->n_values = 2;
    ctail2->max_values = 2;
    ctail1->evaluator = Basics_sub.evaluator;
    ctail2->values[0] = live[nlive - 1];
    ctail2->values[1] = live[nlive - 2];
    live[nlive++] = ctail2;
    live[nlive++] = state->next_alloc; // stack tailcall we're about to allocate
    GC_stack_tailcall(ctail2, push_into_tailrec);

    // allocated just before we stopped the world
    live[nlive] = NEW_ELM_INT(state->stack_depth * 100 + nlive + ndead);
    nlive++;
    live[nlive] = NEW_ELM_INT(state->stack_depth * 100 + nlive + ndead);
    nlive++;

    if (verbose)
        printf("Marking...\n");
    size_t *ignore_below = (size_t *)c1;
    mark(&gc_state, ignore_below);
    if (verbose)
        printf("Finished marking...\n");

    if (verbose)
    {
        // print_heap(&gc_state);
        // print_state(&gc_state);
    }

    size_t dead_size = 0;
    for (size_t i = 0; i < ndead; i++)
    {
        sprintf(alive_or_dead_msg, "%p should be dead", dead[i]);
        mu_assert(alive_or_dead_msg, !is_marked(dead[i]));
        ElmValue *v = (ElmValue *)dead[i];
        dead_size += v->header.size;
    }

    size_t live_size = 0;
    for (size_t i = 0; i < nlive; i++)
    {
        sprintf(alive_or_dead_msg, "%p should be live", live[i]);
        mu_assert(alive_or_dead_msg, is_marked(live[i]));
        ElmValue *v = (ElmValue *)live[i];
        live_size += v->header.size;
    }

    size_t heap_size = state->next_alloc - state->heap.start;
    mu_assert("Stack map test should account for all allocated values",
              live_size + dead_size == heap_size);

    if (verbose)
        printf("\n\nCompacting from %p\n\n", ignore_below);
    compact(&gc_state, ignore_below);
    if (verbose)
        printf("Finished compacting\n");

    if (verbose)
        printf("\n\nMarking compacted heap...\n\n");
    mark(&gc_state, ignore_below);
    if (verbose)
        printf("\n\nFinished marking...\n\n");

    if (verbose)
    {
        printf("\n");
        // print_heap(&gc_state);
        // print_state(&gc_state);

        printf("next_alloc-start %zd\n", state->next_alloc - state->heap.start);
        printf("live_size before compaction %zd\n", live_size);
        printf("\n");
    }

    mu_assert("Compacted heap should contain exactly the number of live values",
              state->next_alloc - state->heap.start == live_size);

    // If 'mark' is able to trace correctly, forwarding addresses are OK
    size_t n_marked = 0;
    for (size_t *w = state->heap.start; w < state->next_alloc; w++)
    {
        n_marked += is_marked(w);
    }
    mu_assert("After compaction and re-marking, all values should be marked",
              n_marked == live_size);

    return NULL;
}

char *gc_struct_test()
{

    mu_assert("GcHeap should be the size of 5 pointers", sizeof(GcHeap) == 5 * sizeof(void *));
    mu_assert("GcState should be the size of 10 pointers", sizeof(GcState) == 10 * sizeof(void *));

    return NULL;
}

char bitmap_msg[100];

char *gc_bitmap_test()
{
    GcState *state = &gc_state;
    char str[] = "This is a test string that's an odd number of ints.....";
    gc_test_reset();

    for (size_t i = 0; i < 10; i++)
    {
        ElmValue *p1, *p2, *p3, *p4;

        p1 = (ElmValue *)NEW_ELM_INT(1);
        p2 = (ElmValue *)NEW_ELM_INT(0);
        p3 = (ElmValue *)NEW_ELM_STRING(sizeof(str), str);
        p4 = (ElmValue *)NEW_ELM_INT(0);

        mark_words(&state->heap, p1, p1->header.size);
        mark_words(&state->heap, p3, p3->header.size);

        mu_assert("p1 should be marked", is_marked(p1));
        mu_assert("p2 should NOT be marked", !is_marked(p2));
        mu_assert("p3 should be marked", is_marked(p3));
        mu_assert("p4 should NOT be marked", !is_marked(p4));
    }

    if (verbose)
    {
        printf("\n");
        printf("########################################################################################\n");
        printf("gc_bitmap_test\n");
        printf("--------------\n");
        printf("\n");
        print_heap(&gc_state);
        print_state(&gc_state);
        printf("\n");
    }

    size_t *bottom_of_heap = state->heap.start;
    size_t *top_of_heap = state->next_alloc;

    size_t *ptr = bottom_of_heap;
    size_t *bitmap = state->heap.bitmap;

    size_t w = 0;
    while (ptr <= top_of_heap)
    {
        size_t word = bitmap[w];
        for (size_t b = 0; b < GC_WORD_BITS; b++)
        {
            bool bitmap_bit = (word & ((size_t)1 << b)) != 0;
            sprintf(bitmap_msg, "is_marked (%d) should match the bitmap (%d)\naddr = %p  word = %zd  bit = %zd",
                    is_marked(ptr), bitmap_bit, ptr, w, b);
            mu_assert(bitmap_msg, is_marked(ptr) == bitmap_bit);
            ptr++;
        }
        w++;
    }

    return NULL;
}

char gc_bitmap_next_test_str[100];

char *gc_bitmap_next_test()
{
    if (verbose)
    {
        printf("\n");
        printf("gc_bitmap_next_test\n");
        printf("\n");
    }
    gc_test_reset();

    size_t word;
    size_t mask;

    int assertion = 1;

    word = 0;
    mask = 1;
    sprintf(gc_bitmap_next_test_str, "bitmap_next assertion %d from word %zd mask %0zx", assertion++, word, mask);
    bitmap_next_test_wrapper(&word, &mask);
    mu_assert(gc_bitmap_next_test_str, word == 0 && mask == 2);

    word = 0;
    mask = 2;
    sprintf(gc_bitmap_next_test_str, "bitmap_next assertion %d from word %zd mask %0zx", assertion++, word, mask);
    bitmap_next_test_wrapper(&word, &mask);
    mu_assert(gc_bitmap_next_test_str, word == 0 && mask == 4);

    word = 1;
    mask = 1;
    sprintf(gc_bitmap_next_test_str, "bitmap_next assertion %d from word %zd mask %0zx", assertion++, word, mask);
    bitmap_next_test_wrapper(&word, &mask);
    mu_assert(gc_bitmap_next_test_str, word == 1 && mask == 2);

    word = 1;
    mask = 2;
    sprintf(gc_bitmap_next_test_str, "bitmap_next assertion %d from word %zd mask %0zx", assertion++, word, mask);
    bitmap_next_test_wrapper(&word, &mask);
    mu_assert(gc_bitmap_next_test_str, word == 1 && mask == 4);

    word = 0;
    char *format_str;
#ifdef TARGET_64BIT
    mask = 0x8000000000000000;
    format_str = "bitmap_next assertion %d from word %zd mask %016zx";
#else
    mask = 0x80000000;
    format_str = "bitmap_next assertion %d from word %zd mask %08zx";
#endif

    mu_assert("bitmap_next: highest bit is correctly set in test", (mask << 1) == 0);
    assertion++;

    sprintf(gc_bitmap_next_test_str, format_str, assertion++, word, mask);
    bitmap_next_test_wrapper(&word, &mask);
    mu_assert(gc_bitmap_next_test_str, word == 1 && mask == 1);

    return NULL;
}

char *gc_dead_between_test()
{
    gc_test_reset();
    GcState *state = &gc_state;
    GcHeap *heap = &state->heap;

    size_t *first;
    size_t *last;

    heap->bitmap[0] = 0xf0f;
    first = heap->start + 4;
    last = heap->start + 8;
    mu_assert("bitmap_dead_between with 4 words dead",
              bitmap_dead_between(heap, first, last) == 4);

    first--;
    last++;

    mu_assert("bitmap_dead_between with 4 dead and 2 live",
              bitmap_dead_between(heap, first, last) == 4);

    heap->bitmap[0] = 0xf0;
    heap->bitmap[1] = 0x00;
    heap->bitmap[2] = 0xf0;
    first = heap->start + 2;
    last = heap->start + (2 * GC_WORD_BITS) + 10;

    mu_assert("bitmap_dead_between across 3 bitmap words",
              bitmap_dead_between(heap, first, last) == ((GC_WORD_BITS - 2 - 4) + GC_WORD_BITS + 10 - 4));

    return NULL;
}

/*
fibHelp : Int -> Int -> Int -> Int
fibHelp iters prev1 prev2 =
    if iters <= 1 then
        prev1
    else
        fibHelp (iters - 1) (prev1 + prev2) prev1
*/
ElmInt literal_0;
ElmInt literal_1;
ElmInt literal_n;

void *fibHelp_tce(void *args[3], void **gc_tce_data)
{
    while (1)
    {
        ElmInt *iters = args[0];
        ElmInt *prev1 = args[1];
        ElmInt *prev2 = args[2];

        if (A2(&Utils_le, iters, &literal_1) == &True)
        {
            return prev1;
        }
        else
        {
            ElmInt *next_iters = A2(&Basics_sub, iters, &literal_1);
            ElmInt *next_prev1 = A2(&Basics_add, prev1, prev2);
            ElmInt *next_prev2 = prev1;

            CAN_THROW(GC_tce_iteration(3, gc_tce_data));

            // update args last, after everything that can throw
            args[0] = next_iters;
            args[1] = next_prev1;
            args[2] = next_prev2;
        }
    }
}

Closure fibHelp;
void *fibHelp_eval(void *args[3])
{
    return GC_tce_eval(&fibHelp_tce, &fibHelp, args);
}

/*
fib : Int -> Int
fib n =
    if n <= 0 then
        0
    else
        (fibHelp n) 1 0
*/
Closure fib;
void *fib_eval(void *args[1])
{
    ElmInt *n = args[0];
    if (A2(&Utils_le, n, &literal_0) == &True)
    {
        return &literal_0;
    }
    else
    {
        // Use currying just to execute more branches of GC code
        Closure *curried = A1(&fibHelp, n);
        return A2(curried, &literal_1, &literal_0);
    }
}

ElmValue *gc_replay_test_catch()
{
    return A1(&fib, &literal_n);
}

int sn_idx = 0;

void gc_debug_stack_trace(GcStackMap *p, Closure *c)
{

    char *name = find_closure_func_name(c);

    // if (strcmp(name, "unknown") == 0) {
    //     printf("can't find name for closure %p at stackmap %p\n",
    //         c, p
    //     );
    // } else {
    //     printf("Closure %p at stackmap %p is %s\n",
    //         c, p, name
    //     );
    // }

    if (sn_idx < MAX_STACKMAP_NAMES)
    {
        stackmap_names[sn_idx++] = (struct sn){
            .stackmap = p,
            .name = name};
    }
}

char *gc_replay_test()
{
    GcState *state = &gc_state;
    if (verbose)
    {
        printf("\n");
        printf("########################################################################################\n");
        printf("gc_replay_test\n");
        printf("--------------\n");
        printf("\n");
    }
    gc_test_reset();

    fib = F1(fib_eval);
    fibHelp = F3(fibHelp_eval);

    func_map[0] = (struct fn){fib.evaluator, "fib"};
    func_map[1] = (struct fn){fibHelp.evaluator, "fibHelp"};
    func_map[2] = (struct fn){Utils_le.evaluator, "Utils_le"};
    func_map[3] = (struct fn){Basics_add.evaluator, "Basics_add"};
    func_map[4] = (struct fn){Basics_sub.evaluator, "Basics_sub"};
    func_map[5] = (struct fn){Basics_mul.evaluator, "Basics_mul"};

    size_t *ignore_below = state->heap.start + 1024; // nice empty heap
    // size_t *ignore_below = state->heap.end - 220;    // pretend memory is nearly full

    state->next_alloc = ignore_below;

    literal_0 = (ElmInt){
        .header = HEADER_INT,
        .value = 0};
    literal_1 = (ElmInt){
        .header = HEADER_INT,
        .value = 1};
    literal_n = (ElmInt){
        .header = HEADER_INT,
        .value = 10};

    // wrapper function to prevent GC exception exiting the test
    ElmValue *result = gc_replay_test_catch();

    if (verbose)
    {
        for (int i = 0; i < NUM_FUNC_NAMES; i++)
        {
            printf("%p : %s\n", func_map[i].evaluator, func_map[i].name);
        }
        printf("stack depth = %zd\n", state->stack_depth);
        printf("True = %p\n", &True);
        printf("False = %p\n", &False);
        printf("Int 0 = %p\n", &literal_0);
        printf("Int 1 = %p\n", &literal_1);
        printf("Int %d = %p\n", literal_n.value, &literal_n);
    }

    // mu_assert("Expect heap overflow",
    //           result->header.tag == Tag_GcException);

    state->stack_depth = 1;

    if (verbose)
        printf("\n\nMarking interrupted heap...\n\n");
    mark(&gc_state, ignore_below);

    if (verbose)
    {
        printf("\n\nFinished marking...\n\n");

        print_heap(&gc_state);
        print_state(&gc_state);
    }

    if (verbose)
        printf("\n\nCompacting from %p\n\n", ignore_below);
    compact(&gc_state, ignore_below);
    if (verbose)
        printf("Finished compacting\n");

    if (verbose)
        printf("\n\nReversing stack map...\n\n");
    reverse_stack_map(&gc_state);
    if (verbose)
        printf("Finished reversing\n");

    if (verbose)
        printf("\n\nMarking compacted heap...\n\n");
    mark(&gc_state, ignore_below);
    if (verbose)
    {
        printf("\n\nFinished marking...\n\n");

        print_heap(&gc_state);
        print_state(&gc_state);
    }

    mu_assert("Compacted heap should be traceable by 'mark'",
              bitmap_dead_between(
                  &state->heap,
                  ignore_below,
                  state->next_alloc) == 0);

    if (verbose)
        printf("Setup for replay\n");
    GcStackMap *empty = (GcStackMap *)state->heap.start;
    state->replay_ptr = empty->newer;

    if (verbose)
        printf("Replay from replay_ptr = %p\n", state->replay_ptr);
    ElmValue *result_replay = gc_replay_test_catch();

    if (verbose)
    {
        printf("\n\nFinished replay...\n\n");

        print_heap(&gc_state);
        print_state(&gc_state);

        printf("result:\n");
        print_value(result_replay);
    }

    i32 answers[29] = {
        0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377,
        610, 987, 1597, 2584, 4181, 6765, 10946, 17711, 28657,
        46368, 75025, 121393, 196418, 317811};
    i32 answer = answers[literal_n.value];

    mu_assert("should return the correct result after replay",
              result_replay->elm_int.value == answer);

    return NULL;
}

char *gc_test()
{
    if (verbose)
    {
        printf("\n");
        printf("GC\n");
        printf("--\n");
    }

    //     mu_run_test(gc_struct_test);
    //     mu_run_test(gc_bitmap_test);
    //     mu_run_test(gc_dead_between_test);
    //     mu_run_test(gc_mark_compact_test);
    //     mu_run_test(gc_bitmap_next_test);
    mu_run_test(gc_replay_test);

    return NULL;
}
