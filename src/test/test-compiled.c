
// ---------------------------------------------------------
//
//  "Compiler-generated" values for use in tests
//
// ---------------------------------------------------------

#include "../kernel/core/core.h"
#include "../kernel/json/json.h"
#include "test.h"
bool sanity_check(void* v);

void* eval_List_reverse(void* args[]) {
  Cons* list = args[0];
  Cons* result = &Nil;
  for (; list != &Nil; list = list->tail) {
    result = newCons(list->head, result);
  }
  return result;
}
Closure g_elm_core_List_reverse = {
    .header = HEADER_CLOSURE(0),
    .max_values = 1,
    .evaluator = &eval_List_reverse,
};


Closure g_elm_core_List_foldl;
void* eval_elm_core_List_foldl(void* args[]) {
  void* x_func = args[0];
  void* x_acc = args[1];
  void* x_list = args[2];
tce_loop:;
  void* case0;
  do {
    if (x_list == &Nil) {
      case0 = x_acc;
      break;
    } else {
      void* x_x = ((Tuple3*)(x_list))->a;
      void* x_xs = ((Tuple3*)(x_list))->b;
      void* tmp1 = A2(x_func, x_x, x_acc);
      void* tmp2 = x_xs;
      x_acc = tmp1;
      x_list = tmp2;
      ASSERT_SANITY(x_acc);
      ASSERT_SANITY(x_list);
      GC_stack_tailcall(3, x_func, x_acc, x_list);
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
  return newCustom(CTOR_Just, 1, args);
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
  return newCustom(CTOR_Ok, 1, args);
}
void* eval_elm_core_Result_Err(void* args[]) {
  return newCustom(CTOR_Err, 1, args);
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
  return newCustom(CTOR_Failure, 2, args);
}
void* eval_elm_json_Json_Decode_Field(void* args[]) {
  return newCustom(CTOR_Field, 2, args);
}
void* eval_elm_json_Json_Decode_Index(void* args[]) {
  return newCustom(CTOR_Index, 2, args);
}
void* eval_elm_json_Json_Decode_OneOf(void* args[]) {
  return newCustom(CTOR_OneOf, 1, args);
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
  Custom* mock_array = newCustom(CTOR_MockElmArray, len->value, NULL);
  for (size_t i = 0; i < len->value; i++) {
    mock_array->values[i] = A1(fn, newElmInt(i));
  }
  return mock_array;
}
Closure g_elm_core_Array_initialize = {
    .header = HEADER_CLOSURE(0),
    .max_values = 2,
    .evaluator = &eval_elm_core_Array_initialize,
};

enum field {
  FIELD_init,
  FIELD_subscriptions,
  FIELD_update,
  FIELD_view,
};

FieldGroup fg_init_subscriptions_update_view = {
    .header = HEADER_FIELDGROUP(4),
    .size = 4,
    .fields =
        {
            FIELD_init,
            FIELD_subscriptions,
            FIELD_update,
            FIELD_view,
        },
};

FieldGroup* Wrapper_appFieldGroups[] = {&fg_init_subscriptions_update_view, NULL};
void** Wrapper_mainsArray[] = {NULL};

// char Debug_evaluator_name_buf[1024];
// char* Debug_evaluator_name(void* p) {
//   stbsp_sprintf(Debug_evaluator_name_buf, "%p", p);
//   return Debug_evaluator_name_buf;
// }

#define TEST_CTOR(x) #x,
char* Debug_ctors[NUM_TEST_CTORS] = {
#include "./test-ctors.inc"
};
#undef TEST_CTOR

char* Debug_fields[4] = {
    "FIELD_init",
    "FIELD_subscriptions",
    "FIELD_update",
    "FIELD_view",
};
int Debug_fields_size = 4;
char* Debug_jsValues[1] = {NULL};
int Debug_jsValues_size = 0;
int Debug_ctors_size = NUM_TEST_CTORS;

Custom g_elm_core_Basics_LT = {
    .header = HEADER_CUSTOM(0),
    .ctor = CTOR_LT,
};
Custom g_elm_core_Basics_EQ = {
    .header = HEADER_CUSTOM(0),
    .ctor = CTOR_EQ,
};
Custom g_elm_core_Basics_GT = {
    .header = HEADER_CUSTOM(0),
    .ctor = CTOR_GT,
};

