#include "../kernel/basics.h"
#include "../kernel/gc.h"
#include "../kernel/types.h"
#include "../kernel/utils.h"

// var $elm$core$Basics$add = /* VarKernel */_Basics_add;
#define elm_core_Basics_add Basics_add
// **NOTE**
// need to pass global name into expression generator

// var $author$project$TestModule$outerScopeValue = /* Int */1;
const ElmInt author_project_TestModule_outerScopeValue = {
    .header = HEADER_INT,
    .value = 1,
};

// var $author$project$TestModule$closure = /* Function */F2(
// 	function (a, b) {
// 		return /* Call *//* VarGlobal */$author$project$TestModule$outerScopeValue
//       +  /* Call */(/* VarLocal */a + /* VarLocal */b);
// 	});
void* eval_author_project_TestModule_closure(void* args[2]) {
  void* a = args[0];
  void* b = args[1];
  return A2(&elm_core_Basics_add, &author_project_TestModule_outerScopeValue,
            A2(&elm_core_Basics_add, a, b));
}
const Closure author_project_TestModule_closure = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_author_project_TestModule_closure,
    .max_values = 2,
};
// **NOTE**
// closing over a VarGlobal is too easy! Need more complex example

// // Define, deps=$author$project$TestModule$closure
// var $author$project$TestModule$curried =
//      /* Call *//* VarGlobal*/$author$project$TestModule$closure(/* Int */2);
ElmValue* ptr_author_project_TestModule_curried;
#define author_project_TestModule_curried (*ptr_author_project_TestModule_curried)
void* init_author_project_TestModule_curried() {
  ptr_author_project_TestModule_curried =
      A1(&author_project_TestModule_closure, NEW_ELM_INT(2));
  return NULL;
}

// // Define, deps=$author$project$TestModule$curried
// var $author$project$TestModule$main =
//        /* Call *//* VarGlobal*/$author$project$TestModule$curried(/* Int */3);
ElmValue* ptr_author_project_TestModule_main;
#define author_project_TestModule_main (*ptr_author_project_TestModule_main)
void* init_author_project_TestModule_main() {
  ptr_author_project_TestModule_main =
      A1(&author_project_TestModule_curried, NEW_ELM_INT(3));
  return NULL;
}

int main() {
  GC_init();
  GC_register_root(&ptr_author_project_TestModule_curried);
  GC_register_root(&ptr_author_project_TestModule_main);
  init_author_project_TestModule_curried();
  init_author_project_TestModule_main();
  return ((ElmInt*)&author_project_TestModule_main)->value;
}
