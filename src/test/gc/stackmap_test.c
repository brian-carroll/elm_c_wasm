#include "../../kernel/core/core.h"
#include "../../kernel/core/gc/internals.h"
#include "../test.h"

void gc_test_reset();

ElmString16* create_test_string(char* prefix, const char* function_name) {
  char buf[100];
  snprintf(buf, sizeof(buf), "%s %s", prefix, function_name);
  return create_string(buf);
}
#define JUNK_STRING() create_test_string("JUNK", __FUNCTION__)
#define LIVE_STRING() create_test_string("LIVE", __FUNCTION__)


void* eval_stack_tail_overflow(void* args[]) {
  Cons* list = args[0];
  ElmInt* count = args[1];
  for(;;) {
    switch (count->value) {
      case 3:
      case 2: {
        // two iterations: one on entry, one after the first tailcall
        JUNK_STRING();
        list = newCons(LIVE_STRING(), list);
        count = newElmInt(count->value - 1);
        GC_stack_tailcall(2, list, count);
        break;
      }
      case 1: {
        // fill up the heap with junk to trigger GC
        count = newElmInt(count->value - 1);
        for (;;) {
          ElmString16* last_alloc = (ElmString16*)list;
          ElmString16* junk = JUNK_STRING();
          if (junk < last_alloc) {
            if (verbose) {
              printf("allocation pointer wrapped around to %p\n", junk);
            }
            break; 
          }
          last_alloc = junk;
          GC_stack_tailcall(2, list, count);
        }
        break;
      }
      case 0: {
        // last iteration
        JUNK_STRING();
        list = newCons(LIVE_STRING(), list);
        return list;
      }
      default: {
        assert(false);
      }
    }
  }
}
Closure stack_tail_overflow = {
    .header = HEADER_CLOSURE(2),
    .max_values = 2,
    .evaluator = eval_stack_tail_overflow,
};


void* eval_stack_normal_overflow(void* args[]) {
  Cons* list = args[0];
  ElmString16* live1 = LIVE_STRING();

  Cons* listAfterMinorGC = A2(&stack_tail_overflow, newCons(live1, list), newElmInt(3));

  ElmString16* live2 = LIVE_STRING();
  Cons* result = newCons(live2, listAfterMinorGC);
  return result;
}
Closure stack_normal_overflow = {
    .header = HEADER_CLOSURE(1),
    .max_values = 1,
    .evaluator = eval_stack_normal_overflow,
};


void* eval_stack_tail_complete(void* args[]) {
  Cons* list = args[0];

  for (int i = 0; i < 3; ++i) {
    JUNK_STRING();
    ElmString16* live = LIVE_STRING();
    list = newCons(live, list);
    GC_stack_tailcall(1, list);
  }

  return list;
}
Closure stack_tail_complete = {
    .header = HEADER_CLOSURE(1),
    .max_values = 1,
    .evaluator = eval_stack_tail_complete,
};


void* eval_stack_normal_complete(void* args[]) {
  Cons* list = args[0];
  ElmString16* live = LIVE_STRING();
  JUNK_STRING();
  Cons* result = newCons(live, list);
  return result;
}
Closure stack_normal_complete = {
    .header = HEADER_CLOSURE(1),
    .max_values = 1,
    .evaluator = eval_stack_normal_complete,
};


void* eval_stackmap_test(void* args[]) {
  Cons* list = &Nil;
  JUNK_STRING();
  list = A1(&stack_tail_complete, list);
  JUNK_STRING();
  list = A1(&stack_normal_complete, list);
  JUNK_STRING();
  list = A1(&stack_normal_overflow, list);
  return list;
}
Closure stackmap_test = {
    .header = HEADER_CLOSURE(1),
    .max_values = 1,
    .evaluator = eval_stackmap_test,
};


void gc_stackmap_test_callback() {
  if (verbose) {
    printf(__FUNCTION__);
    print_stack_map();
    print_state();
    PRINT_BITMAP();
    print_heap();
  }
}

void gc_stackmap_test() {
  if (verbose) {
    printf("\n");
    printf("gc_stackmap_test\n");
    printf("-----------\n");
  }

  gc_test_mark_callback = gc_stackmap_test_callback;

  Cons* stringList = A1(&stackmap_test, &Nil);

  char* expected[] = {
      "LIVE eval_stack_tail_complete",
      "LIVE eval_stack_tail_complete",
      "LIVE eval_stack_tail_complete",
      "LIVE eval_stack_normal_complete",
      "LIVE eval_stack_normal_overflow",
      "LIVE eval_stack_tail_overflow",
      "LIVE eval_stack_tail_overflow",
      "LIVE eval_stack_tail_overflow",
      "LIVE eval_stack_normal_overflow",
  };
  const int len = sizeof(expected) / sizeof(expected[0]);
  ElmString16* actual[len];

  // reverse the list of live strings, to get them in allocation order
  Cons* list = stringList;
  int i = len - 1;
  for (; list != pNil && i >= 0; list = list->tail, i--) {
    actual[i] = list->head;
  }
  mu_assert("should have the correct number of live values", list == pNil && i == -1);

  // test live strings in order
  for (i = 0; i < len; i++) {
    char description[100];
    snprintf(description, sizeof(description), "returned string %d", i);
    expect_string(description, expected[i], actual[i]);
  }
}
