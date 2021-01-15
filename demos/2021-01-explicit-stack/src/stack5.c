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

// both the locals and the stack are GC roots
void* locals[8]; // compiler has to count this
void* stack[16384];  // can we do some call graph analysis?
void** sp = stack;


void main() {
  ElmInt* model = &int_0;

  *sp++ = &int_1;
  *sp++ = model;
  eval_author_project_Main_update();
}


/*
This return sequence is a pain
We have the return value but we have to copy it down the stack
because we had to keep the locals around as double pointers

So what if we make another global array for the locals/
That can be a GC root too

SHOWSTOPPER ISSUE: um.. the locals are global and will override each other

*/

void eval_author_project_Main_update() {
  locals[0] = *sp--; // msg
  locals[1] = *sp--; // model

  // stack neutral

  *sp++ = &int_0;
  *sp++ = locals[0];
  eval_Utils_gt();
  // stack is +!

  if (*sp-- == &True) {
    // stack neutral

    *sp++ = &int_1;
    *sp++ = locals[1];
    eval_Basics_add();
    // stacl +1 which is perfect for returning
  } else {
    // stack neutral

    *sp++ = &int_1;
    *sp++ = locals[1];
    eval_Basics_sub();
    // stack +1 which is perfect for returning
  }
}
