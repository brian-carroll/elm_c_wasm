#include "./replay_test.h"

char* test_replay_finished();
char* test_replay_saturated();
char* test_replay_curried();
char* test_replay_tce_saturated_iter1();
char* test_replay_tce_saturated_iter2();
char* test_replay_tce_curried_iter1();
char* test_replay_tce_curried_iter2();
char* test_replay_apply_alloc_failed();

Tag mock_func_ops[10];  // list of operations for the mock function, encoded as tags
char mock_func_err[1024];
u32 TEST_INT_OFFSET = 256000;  // Not zero! Unlikely to happen by accident
size_t GARBAGE = -1;

static void* eval_mock_func_tce(void* args[2], void** gc_tce_data) {
  ElmInt* last_alloc = args[0];
  i32 start = last_alloc ? last_alloc->value : 0;
  for (i32 i = start;; i++) {
    switch (mock_func_ops[i]) {
      case Tag_Int:
        last_alloc = NEW_ELM_INT(i);
        if (gc_state.replay_ptr) {
          Tag replayed_tag = ((Header*)last_alloc)->tag;
          if (replayed_tag != Tag_Int) {
            print_heap();
            print_state();
            sprintf(mock_func_err,
                "%p: Replay returned tag %x instead of %x",
                last_alloc,
                replayed_tag,
                Tag_Int);
            return mock_func_err;
          }
        }
        break;
      case Tag_GcException:
        return pGcFull;
      case Tag_GcStackPop:
        return last_alloc;
      case Tag_GcStackTailCall:
        last_alloc->value = i + 1;
        *gc_tce_data = CAN_THROW(GC_tce_iteration(2));
        args[0] = last_alloc;
        break;
      default:
        fprintf(stderr, "Unhandled tag in eval_mock_func %x\n", mock_func_ops[i]);
    }
  }
}

void* eval_mock_func_tail(void* args[2]) {
  return GC_tce_eval(&eval_mock_func_tce, &eval_mock_func_tail, 2, args);
}
Closure mock_func_tail = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_mock_func_tail,
    .max_values = 2,
};

Closure mock_func;

void* eval_mock_func(void* args[2]) {
  void* last_alloc = NULL;
  for (size_t i = 0;; i++) {
    switch (mock_func_ops[i]) {
      case Tag_Int:
        last_alloc = NEW_ELM_INT(TEST_INT_OFFSET + i);
        if (gc_state.replay_ptr) {
          Tag replayed_tag = ((Header*)last_alloc)->tag;
          if (replayed_tag != Tag_Int) {
            print_heap();
            print_state();
            sprintf(mock_func_err,
                "%p: Replay returned tag %x instead of %x",
                last_alloc,
                replayed_tag,
                Tag_Int);
            return mock_func_err;
          }
        }
        break;
      case Tag_GcException:
        return pGcFull;
      case Tag_GcStackPop:
        return last_alloc;
      default:
        fprintf(stderr, "Unhandled tag in eval_mock_func %x\n", mock_func_ops[i]);
    }
  }
}

Closure mock_func = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_mock_func,
    .max_values = 2,
};

char assert_heap_message[1024];

char* assert_heap_values(char* description, void* values[]) {
  size_t* heap_value = gc_state.heap.start;
  size_t i = 0;
  size_t* bad_addr = NULL;
  size_t expected_value;

  while (true) {
    size_t* p = (size_t*)values[i];
    if (p == NULL) break;  // marker for end of expected heap array
    if (*p == GARBAGE) {
      i++;
      heap_value++;
      continue;
    }
    ElmValue* v = (ElmValue*)p;
    size_t* v_end = p + v->header.size;
    size_t* heap_word = heap_value;

    do {
      expected_value = *p;
      if (*heap_word != expected_value && expected_value != GARBAGE) {
        bad_addr = heap_word;
        if (verbose) {
          printf("\nMismatch at %p ! Expected to match the following:\n", heap_word);
          print_heap_range((size_t*)v, v_end);
        }
        break;
      }
      heap_word++;
      p++;
    } while (p < v_end);

    if (bad_addr) break;
    heap_value = heap_word;
    i++;
  }

  assertions_made++;
  if (bad_addr) {
    tests_failed++;
    print_heap();
    sprintf(assert_heap_message,
        "FAIL: %s\nExpected %p to be " FORMAT_HEX " but found " FORMAT_HEX "\n",
        description,
        bad_addr,
        expected_value,
        *bad_addr);
    printf("%s", assert_heap_message);
    return assert_heap_message;
  } else if (gc_state.next_alloc != heap_value) {
    tests_failed++;
    print_heap();
    print_state();
    sprintf(assert_heap_message,
        "FAIL: %s\nExpected next_alloc to be %p but it is %p\n",
        description,
        heap_value,
        gc_state.next_alloc);
    printf("%s", assert_heap_message);
    return assert_heap_message;
  } else {
    if (verbose) printf("PASS: %s\n", description);
    return NULL;
  }
}

static char* run() {
  mu_run_test(test_replay_finished);
  mu_run_test(test_replay_saturated);
  mu_run_test(test_replay_curried);
  mu_run_test(test_replay_tce_saturated_iter1);
  mu_run_test(test_replay_tce_saturated_iter2);
  mu_run_test(test_replay_tce_curried_iter1);
  mu_run_test(test_replay_tce_curried_iter2);
  mu_run_test(test_replay_apply_alloc_failed);
  return NULL;
}

char* replay_scenario_tests() {
  if (verbose) {
    printf(
        "\n"
        "##############################################################################\n"
        "\n"
        "replay_scenario_tests\n"
        "---------------------\n"
        "\n");
  }
  char* err = run();
  if (err && verbose) {
    // print_heap();
    print_state();
  }
  return err;
}
