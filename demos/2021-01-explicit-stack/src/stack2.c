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

void* stack[16384]; // 64 KB
void** sp = stack;

void main() {
  ElmInt* model = &int_0;

  *sp++ = &int_1;
  *sp++ = model;
  eval_author_project_Main_update();
}


void eval_author_project_Main_update() {
  void* local_0_msg = *sp--;
  void* local_1_model = *sp--;
  // end of function preamble

  *sp++ = &int_0;
  *sp++ = local_0_msg;
  eval_Utils_gt();
  if (*sp-- == &True) {
    *sp++ = &int_1;
    *sp++ = local_1_model;
    eval_Basics_add();
  } else {
    *sp++ = &int_1;
    *sp++ = local_1_model;
    eval_Basics_sub();
  }
}

void eval_Utils_gt() {
  ElmValue* x = *sp--;
  ElmValue* y = *sp--;
  *sp++ = (compare_help(x, y) == &Utils_GT) ? &True : &False;
}

static void* compare_help(ElmValue* x, ElmValue* y) {
  if (x == y) return &Utils_EQ;
}

void evalHigherOrderFunc() {
  void* local_0_closure = *sp--;
  void* local_1_arg1 = *sp--;
  void* local_2_arg2 = *sp--;
  // end of function preamble

  // pointlessly load into stack again because no optimisation
  *sp++ = local_1_arg1;  // LoadLocal
  *sp++ = local_2_arg2;  // LoadLocal

  Closure* c = (Closure*)local_0_closure;
  if (c->n_values == 0 && c->max_values == 2) {
    c->evaluator(NULL);  // doesn't need any args
  } else {
    Utils_apply(c, 2, NULL);
  }
}

