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
IDEA: ONLY PUSH, DON'T POP TILL THE VERY END.
THAT WAY YOU DON'T LOSE INFO WHEN DOING A REPLAY

PROBLEM: GAPS IN THE STACK ON FINAL RETURN.
RETURNED VALUE FROM eval_Basics_add IS IN THE WRONG PLACE,
WE CAN'T POP THE STUFF BELOW IT, NEED TO ACTUALLY MOVE DATA


*/
void eval_author_project_Main_update() {
  void** ret;
  void* local_0_msg = *(sp - 1);
  void* local_1_model = *(sp - 2);
  // end of function preamble

  *sp++ = &int_0;
  *sp++ = local_0_msg;
  eval_Utils_gt(); // this could do a GC
  if (*sp == &True) {
    *sp++ = &int_1;
    *sp++ = local_1_model; // which means this ref is now garbage
    eval_Basics_add();
    ret = sp - 2; // return value location
    *ret = *sp; // copy data downwards
    sp = ret;
  } else {
    *sp++ = &int_1;
    *sp++ = local_1_model;
    eval_Basics_sub();
    ret = sp - 2; // return value location
    *ret = *sp; // copy data downwards
    sp = ret;
  }
}
