#include "../kernel/gc.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../kernel/basics.h"
#include "../kernel/debug.h"
#include "../kernel/gc-internals.h"
#include "../kernel/types.h"
#include "../kernel/utils.h"
#include "./gc/replay_test.h"
#include "./gc/stackmap_test.h"
#include "./test.h"

extern GcState gc_state;

struct fn {
  void* evaluator;
  char* name;
};
#define NUM_FUNC_NAMES 5
struct fn func_map[NUM_FUNC_NAMES];

#define MAX_STACKMAP_NAMES 100
struct sn {
  GcStackMap* stackmap;
  char* name;
} stackmap_names[MAX_STACKMAP_NAMES];

char* find_stackmap_func_name(GcStackMap* sm) {
  char* name = "unknown";
  for (int i = 0; i < MAX_STACKMAP_NAMES; i++) {
    if (stackmap_names[i].stackmap == sm) {
      name = stackmap_names[i].name;
    }
  }
  return name;
}

char* find_closure_func_name(Closure* c) {
  char* name = "unknown";
  for (int i = 0; i < NUM_FUNC_NAMES; i++) {
    if (func_map[i].evaluator == c->evaluator) {
      name = func_map[i].name;
    }
  }
  return name;
}

void gc_test_reset() {
  GcState* state = &gc_state;
  for (size_t* word = state->heap.start; word < state->heap.system_end; word++) {
    *word = 0;
  }
  state->next_alloc = state->heap.start;
  state->roots = &Nil;
  state->stack_depth = 0;
  GC_stack_empty();
}

char alive_or_dead_msg[50];
void* root_mutable_pointer;

char* gc_mark_compact_test() {
  GcState* state = &gc_state;
  gc_test_reset();

  if (verbose)
    printf(
        "\n"
        "#############################################################################\n"
        "\n"
        "gc_mark_compact_test\n"
        "--------------------\n"
        "\n");

  void* live[100];
  void* dead[100];
  size_t nlive = 0;
  size_t ndead = 0;

  if (verbose) {
    printf("GC initial state:\n");
    print_state();
  }

  // Mock Closures.
  // We never evaluate these during the test so they can be anything
  Closure* mock_effect_callback = &Basics_add;  // Basics_add is not really an effect
                                                // callback. It's just a Closure value.
  Closure* mock_closure = &Basics_mul;

  live[nlive++] = state->heap.start;  // stack_empty

  // Call the top-level function (an effect callback or incoming port)
  ElmValue* c1 = (ElmValue*)Utils_clone(mock_effect_callback);
  live[nlive++] = state->next_alloc;  // the root Cons cell we're about to allocate
  root_mutable_pointer = c1;
  GC_register_root(&root_mutable_pointer);  // Effect manager is keeping this Closure
                                            // alive by connecting it to the GC root.
  live[nlive++] = c1;
  if (verbose) {
    printf("Kernel module registered root:\n  located at %p\n  pointing at %p\n",
        &root_mutable_pointer,
        root_mutable_pointer);
  }

  void* push1 = GC_stack_push();
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
  Closure* c2 = Utils_clone(mock_closure);
  live[nlive++] = c2;
  void* push2 = GC_stack_push();
  live[nlive++] = push2;

  // Temporary values from level 2, not in return value
  dead[ndead] = NEW_ELM_INT(state->stack_depth * 100 + nlive + ndead);
  ndead++;
  dead[ndead] = NEW_ELM_INT(state->stack_depth * 100 + nlive + ndead);
  ndead++;

  // 3rd level function call. All dead, since we have the return value of a higher level
  // call.
  void* push3 = GC_stack_push();
  dead[ndead++] = push3;
  dead[ndead] = NEW_ELM_INT(state->stack_depth * 100 + nlive + ndead);
  ndead++;
  dead[ndead] = NEW_ELM_INT(state->stack_depth * 100 + nlive + ndead);
  ndead++;
  ElmInt* ret3 = NEW_ELM_INT(state->stack_depth * 100 + nlive + ndead);
  dead[ndead++] = ret3;
  dead[ndead++] = state->next_alloc;  // the pop we're about to allocate
  GC_stack_pop((ElmValue*)ret3, push3);

  // return value from level 2. Keep it to provide to level 1 on replay
  ElmValue* ret2a = (ElmValue*)NEW_ELM_INT(state->stack_depth * 100 + nlive + ndead);
  ElmValue* ret2b = (ElmValue*)NEW_CONS(ret2a, &Nil);
  live[nlive++] = ret2a;
  live[nlive++] = ret2b;
  ElmValue* ret2c = (ElmValue*)NEW_ELM_INT(state->stack_depth * 100 + nlive + ndead);
  ElmValue* ret2d = (ElmValue*)NEW_CONS(ret2c, ret2b);
  live[nlive++] = ret2c;
  live[nlive++] = ret2d;

  // Pop back up to top-level function and allocate a few more things.
  // We actually have a choice whether these are considered alive or dead.
  // Implementation treats them as live for consistency and ease of coding
  live[nlive++] = state->next_alloc;  // the pop we're about to allocate
  GC_stack_pop(ret2d, push2);
  live[nlive] = NEW_ELM_INT(state->stack_depth * 100 + nlive + ndead);
  nlive++;
  live[nlive] = NEW_ELM_INT(state->stack_depth * 100 + nlive + ndead);
  nlive++;
  live[nlive] = NEW_ELM_INT(state->stack_depth * 100 + nlive + ndead);
  nlive++;

  // Call a function that makes a tail call
  void* push_into_tailrec = GC_stack_push();
  live[nlive++] = push_into_tailrec;
  dead[ndead] = NEW_ELM_INT(state->stack_depth * 100 + nlive + ndead);
  ndead++;
  dead[ndead] = NEW_ELM_INT(state->stack_depth * 100 + nlive + ndead);
  ndead++;

  // Tail call. Has completed when we stop the world => dead
  Closure* ctail1 = GC_malloc(sizeof(Closure) + 2 * sizeof(void*));
  ctail1->header = HEADER_CLOSURE(2);
  ctail1->n_values = 2;
  ctail1->max_values = 2;
  ctail1->evaluator = Basics_sub.evaluator;
  ctail1->values[0] = dead[ndead - 1];
  ctail1->values[1] = dead[ndead - 2];
  dead[ndead++] = ctail1;
  dead[ndead++] = state->next_alloc;  // tailcall we're about to allocate
  GC_stack_tailcall(ctail1, push_into_tailrec);

  // arguments to the next tail call
  live[nlive] = NEW_ELM_INT(state->stack_depth * 100 + nlive + ndead);
  nlive++;
  live[nlive] = NEW_ELM_INT(state->stack_depth * 100 + nlive + ndead);
  nlive++;

  // Tail call. still evaluating this when we stopped the world => keep closure alive
  Closure* ctail2 = GC_malloc(sizeof(Closure) + 2 * sizeof(void*));
  ctail2->header = HEADER_CLOSURE(2);
  ctail2->n_values = 2;
  ctail2->max_values = 2;
  ctail1->evaluator = Basics_sub.evaluator;
  ctail2->values[0] = live[nlive - 1];
  ctail2->values[1] = live[nlive - 2];
  live[nlive++] = ctail2;
  live[nlive++] = state->next_alloc;  // stack tailcall we're about to allocate
  GC_stack_tailcall(ctail2, push_into_tailrec);

  // allocated just before we stopped the world
  live[nlive] = NEW_ELM_INT(state->stack_depth * 100 + nlive + ndead);
  nlive++;
  live[nlive] = NEW_ELM_INT(state->stack_depth * 100 + nlive + ndead);
  nlive++;

  if (verbose) printf("Marking...\n");
  size_t* ignore_below = state->heap.start;
  mark(&gc_state, ignore_below);
  if (verbose) printf("Finished marking...\n");

  if (verbose) {
    print_heap();
    print_state();
  }

  size_t dead_size = 0;
  for (size_t i = 0; i < ndead; i++) {
    sprintf(alive_or_dead_msg, "Value #%zd at %p should be dead", i, dead[i]);
    mu_assert(alive_or_dead_msg, !is_marked(dead[i]));
    ElmValue* v = (ElmValue*)dead[i];
    dead_size += v->header.size;
  }

  size_t live_size = 0;
  for (size_t i = 0; i < nlive; i++) {
    sprintf(alive_or_dead_msg, "Value #%zd at %p should be live", i, live[i]);
    mu_assert(alive_or_dead_msg, is_marked(live[i]));
    ElmValue* v = (ElmValue*)live[i];
    live_size += v->header.size;
  }

  size_t heap_size = state->next_alloc - state->heap.start;
  mu_assert("Stack map test should account for all allocated values",
      live_size + dead_size == heap_size);

  if (verbose) printf("\n\nCompacting from %p\n\n", ignore_below);
  compact(&gc_state, ignore_below);
  if (verbose) printf("Finished compacting\n");

  if (verbose) printf("\n\nMarking compacted heap...\n\n");
  mark(&gc_state, ignore_below);
  if (verbose) printf("\n\nFinished marking...\n\n");

  if (verbose) {
    printf("\n");
    // print_heap();
    // print_state();

    printf("next_alloc-start %zd\n", state->next_alloc - state->heap.start);
    printf("live_size before compaction %zd\n", live_size);
    printf("\n");
  }

  mu_assert("Compacted heap should contain exactly the number of live values",
      state->next_alloc - state->heap.start == live_size);

  // If 'mark' is able to trace correctly, forwarding addresses are OK
  size_t n_marked = 0;
  for (size_t* w = state->heap.start; w < state->next_alloc; w++) {
    n_marked += is_marked(w);
  }
  mu_assert("After compaction and re-marking, all values should be marked",
      n_marked == live_size);

  return NULL;
}

char bitmap_msg[100];

char* gc_bitmap_test() {
  GcState* state = &gc_state;
  char str[] = "This is a test string that's an odd number of ints.....";
  gc_test_reset();

  for (size_t i = 0; i < 10; i++) {
    ElmValue *p1, *p2, *p3, *p4;

    p1 = (ElmValue*)NEW_ELM_INT(0x101);
    p2 = (ElmValue*)NEW_ELM_INT(0x102);
    p3 = (ElmValue*)NEW_ELM_STRING(sizeof(str), str);
    p4 = (ElmValue*)NEW_ELM_INT(0x103);

    mark_words(&state->heap, p1, p1->header.size);
    mark_words(&state->heap, p3, p3->header.size);

    mu_assert("p1 should be marked", !!is_marked(p1));
    mu_assert("p2 should NOT be marked", !is_marked(p2));
    mu_assert("p3 should be marked", !!is_marked(p3));
    mu_assert("p4 should NOT be marked", !is_marked(p4));
  }

  if (verbose) {
    printf(
        "\n"
        "#############################################################################\n"
        "\n"
        "gc_bitmap_test\n"
        "--------------\n"
        "\n");
    print_heap();
    print_state();
    printf("\n");
  }

  size_t* bottom_of_heap = state->heap.start;
  size_t* top_of_heap = state->next_alloc;

  size_t* ptr = bottom_of_heap;
  size_t* bitmap = state->heap.bitmap;

  size_t w = 0;
  while (ptr <= top_of_heap) {
    size_t word = bitmap[w];
    for (size_t b = 0; b < GC_WORD_BITS; b++) {
      bool bitmap_bit = (word & ((size_t)1 << b)) != 0;
      sprintf(bitmap_msg,
          "is_marked (%d) should match the bitmap (%d) addr = %p  word = %zd  bit = %zd",
          is_marked(ptr),
          bitmap_bit,
          ptr,
          w,
          b);
      mu_assert(bitmap_msg, is_marked(ptr) == bitmap_bit);
      ptr++;
    }
    w++;
  }

  return NULL;
}

char gc_bitmap_next_test_str[100];

char* gc_bitmap_next_test() {
  if (verbose) {
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
  sprintf(gc_bitmap_next_test_str,
      "bitmap_next assertion %d from word %zd mask %0zx",
      assertion++,
      word,
      mask);
  bitmap_next_test_wrapper(&word, &mask);
  mu_assert(gc_bitmap_next_test_str, word == 0 && mask == 2);

  word = 0;
  mask = 2;
  sprintf(gc_bitmap_next_test_str,
      "bitmap_next assertion %d from word %zd mask %0zx",
      assertion++,
      word,
      mask);
  bitmap_next_test_wrapper(&word, &mask);
  mu_assert(gc_bitmap_next_test_str, word == 0 && mask == 4);

  word = 1;
  mask = 1;
  sprintf(gc_bitmap_next_test_str,
      "bitmap_next assertion %d from word %zd mask %0zx",
      assertion++,
      word,
      mask);
  bitmap_next_test_wrapper(&word, &mask);
  mu_assert(gc_bitmap_next_test_str, word == 1 && mask == 2);

  word = 1;
  mask = 2;
  sprintf(gc_bitmap_next_test_str,
      "bitmap_next assertion %d from word %zd mask %0zx",
      assertion++,
      word,
      mask);
  bitmap_next_test_wrapper(&word, &mask);
  mu_assert(gc_bitmap_next_test_str, word == 1 && mask == 4);

  word = 0;
  char* format_str;
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

char* gc_dead_between_test() {
  gc_test_reset();
  GcState* state = &gc_state;
  GcHeap* heap = &state->heap;

  size_t* first;
  size_t* last;

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
      bitmap_dead_between(heap, first, last) ==
          ((GC_WORD_BITS - 2 - 4) + GC_WORD_BITS + 10 - 4));

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

void* fibHelp_tce(void* args[3], void** gc_tce_data) {
  while (1) {
    ElmInt* iters = args[0];
    ElmInt* prev1 = args[1];
    ElmInt* prev2 = args[2];

    if (A2(&Utils_le, iters, &literal_1) == &True) {
      return prev1;
    } else {
      ElmInt* next_iters = A2(&Basics_sub, iters, &literal_1);
      ElmInt* next_prev1 = A2(&Basics_add, prev1, prev2);
      ElmInt* next_prev2 = prev1;

      CAN_THROW(GC_tce_iteration(3, gc_tce_data));

      // *Mutate* args last, after everything that can throw
      // Allows us to resume after GC exception
      args[0] = next_iters;
      args[1] = next_prev1;
      args[2] = next_prev2;
    }
  }
}

const Closure fibHelp;
void* fibHelp_eval(void* args[3]) {
  return GC_tce_eval(&fibHelp_tce, &fibHelp, args);
}
const Closure fibHelp = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &fibHelp_eval,
    .max_values = 3,
};

/*
fib : Int -> Int
fib n =
    if n <= 0 then
        0
    else
        (fibHelp n) 1 0
*/
void* fib_eval(void* args[1]) {
  ElmInt* n = args[0];
  if (A2(&Utils_le, n, &literal_0) == &True) {
    return &literal_0;
  } else {
    // Use currying just to execute more branches of GC code
    Closure* curried = A1(&fibHelp, n);
    return A2(curried, &literal_1, &literal_0);
  }
}
const Closure fib = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &fib_eval,
    .max_values = 1,
};

ElmValue* gc_replay_test_catch() {
  return A1(&fib, &literal_n);
}

int sn_idx = 0;

void gc_debug_stack_trace(GcStackMap* p, Closure* c) {
  char* name = find_closure_func_name(c);

  // if (strcmp(name, "unknown") == 0) {
  //     printf("can't find name for closure %p at stackmap %p\n",
  //         c, p
  //     );
  // } else {
  //     printf("Closure %p at stackmap %p is %s\n",
  //         c, p, name
  //     );
  // }

  if (sn_idx < MAX_STACKMAP_NAMES) {
    stackmap_names[sn_idx++] = (struct sn){.stackmap = p, .name = name};
  }
}

char* gc_replay_test() {
  GcState* state = &gc_state;
  if (verbose) {
    printf(
        "\n"
        "##############################################################################\n"
        "\n"
        "gc_replay_test\n"
        "--------------\n"
        "\n"
        "- Set up heap to be 'nearly full'\n"
        "- Call an Elm function that doesn't have enough heap space to finish\n"
        "- Expect a GC exception to be thrown (interrupting execution and unravelling "
        "the call stack)\n"
        "- Collect garbage (mark & compact)\n"
        "- Expect compacted heap to be consistent ('mark' succeeds)\n"
        "- Restore the state of the stack with new pointer locations & resume execution\n"
        "- Expect execution to complete & return the correct result\n"
        "\n"
        "fib : Int -> Int\n"
        "fib n =\n"
        "    if n <= 0 then\n"
        "        0\n"
        "    else\n"
        "        (fibHelp n) 1 0\n"
        "\n"
        "fibHelp : Int -> Int -> Int -> Int\n"
        "fibHelp iters prev1 prev2 =\n"
        "    if iters <= 1 then\n"
        "        prev1\n"
        "    else\n"
        "        fibHelp (iters - 1) (prev1 + prev2) prev1\n"
        "\n");
  }
  gc_test_reset();

  func_map[0] = (struct fn){fib.evaluator, "fib"};
  func_map[1] = (struct fn){fibHelp.evaluator, "fibHelp"};
  func_map[2] = (struct fn){Utils_le.evaluator, "Utils_le"};
  func_map[3] = (struct fn){Basics_add.evaluator, "Basics_add"};
  func_map[4] = (struct fn){Basics_sub.evaluator, "Basics_sub"};

  literal_0 = (ElmInt){.header = HEADER_INT, .value = 0};
  literal_1 = (ElmInt){.header = HEADER_INT, .value = 1};
  literal_n = (ElmInt){.header = HEADER_INT, .value = 10};

#ifdef TARGET_64BIT
  size_t not_quite_enough_space = 220;
#else
  size_t not_quite_enough_space = 300;
#endif
  size_t* ignore_below = state->heap.end - not_quite_enough_space;
  state->next_alloc = ignore_below;

  if (verbose)
    printf("Set allocation pointer to leave only %zu (%zu-bit) words of heap space\n",
        not_quite_enough_space,
        (sizeof(void*)) * 8);

  // wrapper function to prevent GC exception exiting the test
  ElmValue* result = gc_replay_test_catch();

  mu_assert("Expect GC exception when 'fib' called with insufficient heap space",
      result->header.tag == Tag_GcException);

  if (verbose) {
    printf("\n");
    printf("stack depth = %zd\n", state->stack_depth);
    printf("\n");
    printf("Function addresses:\n");
    for (int i = 0; i < NUM_FUNC_NAMES; i++)
      printf("%p : %s\n", func_map[i].evaluator, func_map[i].name);

    printf("\n");
    printf("Value addresses:\n");
    printf("True = %p\n", &True);
    printf("False = %p\n", &False);
    printf("Int 0 = %p\n", &literal_0);
    printf("Int 1 = %p\n", &literal_1);
    printf("Int %d = %p\n", literal_n.value, &literal_n);
    printf("\n");
  }

  if (verbose) printf("Marking interrupted heap...\n\n");

  mark(&gc_state, ignore_below);

  if (verbose) {
    printf("Finished marking...\n\n");

    print_heap();
    print_state();
  }

  if (verbose) printf("\n\nCompacting from %p\n\n", ignore_below);

  compact(&gc_state, ignore_below);

  if (verbose) {
    printf("Finished compacting\n");
    print_state();
    printf("Reversing stack map linked list to prepare for replay\n\n");
  }

  reverse_stack_map(&gc_state);

  if (verbose) {
    printf("Finished reversing\n");
    print_state();
    printf("Marking compacted heap (to check integrity, would skip in 'real life')\n\n");
  }

  mark(&gc_state, ignore_below);

  mu_assert("Compacted heap should be traceable by 'mark'",
      bitmap_dead_between(&state->heap, ignore_below, state->next_alloc) == 0);

  if (verbose) {
    print_heap();
    print_state();
  }

  GcStackMap* empty = (GcStackMap*)state->heap.start;
  state->replay_ptr = empty->newer;
  state->stack_depth = 0;

  if (verbose) {
    printf("\nSetting replay pointer to start of stackmap: %p\n", state->replay_ptr);
    printf(
        "Replaying interrupted functions to restore stack state, then continuing "
        "execution...\n");
  }

  ElmValue* result_replay = gc_replay_test_catch();

  if (verbose) {
    printf("\nFinished replay and 2nd execution run...\n");
    printf(
        "Aaaand the %dth Fibonacci number is ...drumroll please...\n", literal_n.value);
    print_value(result_replay);
  }

  i32 answers[29] = {
      0,
      1,
      1,
      2,
      3,
      5,
      8,
      13,
      21,
      34,
      55,
      89,
      144,
      233,
      377,
      610,
      987,
      1597,
      2584,
      4181,
      6765,
      10946,
      17711,
      28657,
      46368,
      75025,
      121393,
      196418,
      317811,
  };
  i32 answer = answers[literal_n.value];

  bool pass = result_replay->elm_int.value == answer;

  if (verbose && !pass) {
    print_heap();
    print_state();
  }

  mu_assert("should return the correct result after resuming", pass);

  return NULL;
}

char* gc_test() {
  if (verbose)
    printf(
        "##############################################################################\n"
        "\n"
        "                              Garbage Collector tests\n");

  mu_run_test(gc_replay_test);
  mu_run_test(replay_scenario_tests);
  mu_run_test(stackmap_mark_test);
  mu_run_test(gc_bitmap_test);
  mu_run_test(gc_dead_between_test);
  mu_run_test(gc_mark_compact_test);
  mu_run_test(gc_bitmap_next_test);

  return NULL;
}
