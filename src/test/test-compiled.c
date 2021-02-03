
// ---------------------------------------------------------
//
//  "Compiler-generated" values for use in tests
//
// ---------------------------------------------------------

#include "../kernel/core/core.h"
#include "../kernel/json/json.h"
#include "test.h"

void* eval_List_reverse(void* args[]) {
  Cons* list = args[0];
  Cons* result = &Nil;
  for (; list != &Nil; list = list->tail) {
    result = NEW_CONS(list->head, result);
  }
  return result;
}
Closure g_elm_core_List_reverse = {
    .header = HEADER_CLOSURE(0),
    .max_values = 1,
    .evaluator = &eval_List_reverse,
};

Closure g_elm_core_List_foldl;
void * eval_elm_core_List_foldl(void * args[]) {
    void * x_func = args[0];
    void * x_acc = args[1];
    void * x_list = args[2];
    u32 n_free = 0;
    void* push = GC_stack_get_current_section();
    tce_loop:
    ;
    void * case0;
    do {
        if (x_list == &Nil) {
            case0 = x_acc;
            break;
        } else {
            void * x_x = ((Tuple3 * )(x_list))->a;
            void * x_xs = ((Tuple3 * )(x_list))->b;
            void * tmp1 = A2(x_func, x_x, x_acc);
            x_list = x_xs;
            x_acc = tmp1;
            x_func = x_func;
            CAN_THROW(GC_stack_tailcall(push, n_free, args, 3, ((void * []){ x_func, x_acc, x_list })));
            goto tce_loop;
            case0 = NULL;
            break;
        };
    } while (0);
    return case0;
}
Closure g_elm_core_List_foldl = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x3,
    .evaluator = &eval_elm_core_List_foldl,
};

void* eval_elm_core_Maybe_Just(void* args[]) {
  return ctorCustom(CTOR_Just, 1, args);
}
Closure g_elm_core_Maybe_Just = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_core_Maybe_Just,
};
Custom g_elm_core_Maybe_Nothing = {
    .header = HEADER_CUSTOM(0),
    .ctor = CTOR_Nothing,
};

void* eval_elm_core_Result_Ok(void* args[]) {
  return ctorCustom(CTOR_Ok, 1, args);
}
void* eval_elm_core_Result_Err(void* args[]) {
  return ctorCustom(CTOR_Err, 1, args);
}
void* eval_elm_core_Result_isOk(void* args[]) {
  void* x_result = args[0];
  void* tmp0;
  u32 tmp1 = ((Custom*)x_result)->ctor;
  do {
    if (tmp1 == CTOR_Ok) {
      tmp0 = &True;
      break;
    } else {
      tmp0 = &False;
      break;
    };
  } while (0);
  return tmp0;
}
Closure g_elm_core_Result_isOk = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_core_Result_isOk,
};

void* eval_elm_json_Json_Decode_Failure(void* args[]) {
  return ctorCustom(CTOR_Failure, 2, args);
}
void* eval_elm_json_Json_Decode_Field(void* args[]) {
  return ctorCustom(CTOR_Field, 2, args);
}
void* eval_elm_json_Json_Decode_Index(void* args[]) {
  return ctorCustom(CTOR_Index, 2, args);
}
void* eval_elm_json_Json_Decode_OneOf(void* args[]) {
  return ctorCustom(CTOR_OneOf, 1, args);
}
Closure g_elm_json_Json_Decode_Failure = {
    .header = HEADER_CLOSURE(0),
    .max_values = 2,
    .evaluator = &eval_elm_json_Json_Decode_Failure,
};
Closure g_elm_json_Json_Decode_Field = {
    .header = HEADER_CLOSURE(0),
    .max_values = 2,
    .evaluator = &eval_elm_json_Json_Decode_Field,
};
Closure g_elm_json_Json_Decode_Index = {
    .header = HEADER_CLOSURE(0),
    .max_values = 2,
    .evaluator = &eval_elm_json_Json_Decode_Index,
};
Closure g_elm_json_Json_Decode_OneOf = {
    .header = HEADER_CLOSURE(0),
    .max_values = 1,
    .evaluator = &eval_elm_json_Json_Decode_OneOf,
};

void* eval_elm_core_Array_initialize(void* args[]) {
  ElmInt* len = args[0];
  Closure* fn = args[1];
  Custom* mock_array = NEW_CUSTOM(CTOR_MockElmArray, len->value, NULL);
  for (size_t i = 0; i < len->value; i++) {
    mock_array->values[i] = A1(fn, NEW_ELM_INT(i));
  }
  return mock_array;
}
Closure g_elm_core_Array_initialize = {
    .header = HEADER_CLOSURE(0),
    .max_values = 2,
    .evaluator = &eval_elm_core_Array_initialize,
};

FieldGroup* Wrapper_appFieldGroups[] = {NULL};
void** Wrapper_mainsArray[] = {NULL};

// char Debug_evaluator_name_buf[1024];
// char* Debug_evaluator_name(void* p) {
//   sprintf(Debug_evaluator_name_buf, "%p", p);
//   return Debug_evaluator_name_buf;
// }

int Debug_ctors_size = 8;
char* Debug_ctors[] = {
    "CTOR_Nothing",
    "CTOR_Just",
    "CTOR_Ok",
    "CTOR_Err",
    "CTOR_Failure",
    "CTOR_Field",
    "CTOR_Index",
    "CTOR_OneOf",
};
char* Debug_fields[] = {};
char* Debug_jsValues[] = {};
int Debug_fields_size = 0;
int Debug_jsValues_size = 0;

size_t Json_run_eval_index = 123;
