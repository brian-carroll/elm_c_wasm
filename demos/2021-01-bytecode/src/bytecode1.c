#include "../../../../src/kernel/kernel.h"

ElmInt int_1 = {
    .header = HEADER_INT,
    .value = 1,
};
ElmInt int_0 = {
    .header = HEADER_INT,
    .value = 0,
};
ElmInt int__1 = {
    .header = HEADER_INT,
    .value = -1,
};

#define g_author_project_Main_initialModel int_0

enum {
  FUNC_update,
  FUNC_view,
};

void* stack[1024];
u32 stackIdx = 0;

void main() {
  ElmInt* model = &int_0;

  stack[stackIdx++] = &int_1;
  stack[stackIdx++] = model;
  eval_author_project_Main_update();
}

/*
  If
    Apply
     - LoadConst Basics_gt
        - alias to Utils_gt
        - and for top level const Closure with no children we emit the evaluator
     - LoadLocal msg
     - LoadConst int_0
    Apply
     - LoadEvalFunc Basics_add
     - LoadLocal model
     - LoadConst int_1
    Apply
     - LoadEvalFunc Basics_sub
     - LoadLocal msg
     - LoadConst int_1
*/
void eval_author_project_Main_update() {
  void* local_0_msg = stack[stackIdx--];
  void* local_1_model = stack[stackIdx--];
  // end of function preamble

  stack[stackIdx++] = &int_0;       // LoadConst
  stack[stackIdx++] = local_0_msg;  // LoadLocal
  eval_Utils_gt();
  if (stack[stackIdx--] == &True) {
    stack[stackIdx++] = &int_1;         // LoadConst
    stack[stackIdx++] = local_1_model;  // LoadLocal
    eval_Basics_add();
  } else {
    stack[stackIdx++] = &int_1;         // LoadConst
    stack[stackIdx++] = local_1_model;  // LoadLocal
    eval_Basics_sub();
  }
}

void eval_Utils_gt() {
  ElmValue* x = stack[stackIdx--];
  ElmValue* y = stack[stackIdx--];
  stack[stackIdx++] = (compare_help(x, y) == &Utils_GT) ? &True : &False;
}

static void* compare_help(ElmValue* x, ElmValue* y) {
  if (x == y) return &Utils_EQ;
}

void evalHigherOrderFunc() {
  void* local_0_closure = stack[stackIdx--];
  void* local_1_arg1 = stack[stackIdx--];
  void* local_2_arg2 = stack[stackIdx--];
  // end of function preamble

  // pointlessly load into stack again because no optimisation
  stack[stackIdx++] = local_1_arg1;  // LoadLocal
  stack[stackIdx++] = local_2_arg2;  // LoadLocal

  Closure* c = (Closure*)local_0_closure;
  if (c->n_values == 0 && c->max_values == 2) {
    c->evaluator(NULL);  // doesn't need any args
  } else {
    Utils_apply(c, 2, NULL);
  }
}
