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

void* stack[16384];  // can we do some call graph analysis?
void** sp = stack;

void main() {
  ElmInt* model = &int_0;

  *sp++ = &int_1;
  *sp++ = model;
  eval_author_project_Main_update();
}


/*
Simplify the return sequence by copying stack pointer on entry
This also means we only need one return sequence at the end,
regardless of diferent number of calls or ops on each branch
*/
void eval_author_project_Main_update() {
  void** ret = sp - 2; // where we want to put the return value
  void** msg = sp - 1; // don't pop, just reference
  void** model = sp - 2; // don't pop, just reference
  // end of function preamble

  *sp++ = &int_0;
  *sp++ = *msg;
  eval_Utils_gt();
  if (*sp-- == &True) {
    *sp++ = &int_1;
    *sp++ = *model;
    eval_Basics_add();
  } else {
    *sp++ = &int_1;
    *sp++ = *model;
    eval_Basics_sub();
  }
  *ret = *sp;  // copy return data
  sp = ret;    // move stack pointer
}
