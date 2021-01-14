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

void* stack[16384];  // 64 KB
void** sp = stack;

void main() {
  ElmInt* model = &int_0;

  *sp++ = &int_1;
  *sp++ = model;
  eval_author_project_Main_update();
}


/*
But hang on aren't we meant to be avoiding replay?
Instead of replay, sometimes GC_malloc just takes ages because it has to do a GC,

and when it returns, **all the stack pointers to the heap have different values**

So we can never trust those to stay the same over time
We have to use double pointers for locals
The stack is an indirection layer to the heap
*/

void eval_author_project_Main_update() {
  void** ret;
  void** msg = sp - 1;
  void** model = sp - 2;
  // end of function preamble

  *sp++ = &int_0;
  *sp++ = *msg;
  eval_Utils_gt();
  if (*sp-- == &True) {
    *sp++ = &int_1;
    *sp++ = *model; // fresh deref => OK if GC moved it during eval_Utils_gt call
    eval_Basics_add();
    ret = sp - 2; // return value location
    *ret = *sp; // copy data downwards
    sp = ret;
  } else {
    *sp++ = &int_1;
    *sp++ = *model;
    eval_Basics_sub();
    ret = sp - 2; // return value location
    *ret = *sp; // copy data downwards
    sp = ret;
  }
}
