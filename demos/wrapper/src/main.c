#include <stdio.h>
#include "../../../src/kernel/kernel.h"
enum {
  CTOR_Normal,
  CTOR_Perform,
  CTOR_SetCounter,
};
enum {
  JS_Browser_element,
  JS_Json_succeed,
  JS_Platform_batch,
  JS_Platform_leaf,
  JS_Process_sleep,
  JS_Scheduler_andThen,
  JS_Scheduler_succeed,
  JS_VirtualDom_node,
  JS_VirtualDom_on,
  JS_VirtualDom_text,
};
enum {
  FIELD_init,
  FIELD_subscriptions,
  FIELD_update,
  FIELD_view,
};
Closure VirtualDom_text = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0xffff,
    .evaluator = (void*)JS_VirtualDom_text,
};
Closure VirtualDom_on = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0xffff,
    .evaluator = (void*)JS_VirtualDom_on,
};
Closure VirtualDom_node = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0xffff,
    .evaluator = (void*)JS_VirtualDom_node,
};
Closure Scheduler_succeed = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0xffff,
    .evaluator = (void*)JS_Scheduler_succeed,
};
Closure Scheduler_andThen = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0xffff,
    .evaluator = (void*)JS_Scheduler_andThen,
};
Closure Process_sleep = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0xffff,
    .evaluator = (void*)JS_Process_sleep,
};
Closure Platform_leaf = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0xffff,
    .evaluator = (void*)JS_Platform_leaf,
};
Closure Platform_batch = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0xffff,
    .evaluator = (void*)JS_Platform_batch,
};
Closure Json_succeed = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0xffff,
    .evaluator = (void*)JS_Json_succeed,
};
Closure Browser_element = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0xffff,
    .evaluator = (void*)JS_Browser_element,
};
FieldGroup fg_init_subscriptions_update_view = {
    .size = 4,
    .fields =
        {
            FIELD_init,
            FIELD_subscriptions,
            FIELD_update,
            FIELD_view,
        },
};
ElmString16 literal_string_h1 = {
    .header = HEADER_STRING(2),
    .words16 =
        {
            0x68,
            0x31,
        },
};
ElmString16 literal_string_div = {
    .header = HEADER_STRING(3),
    .words16 =
        {
            0x64,
            0x69,
            0x76,
        },
};
ElmString16 literal_string_click = {
    .header = HEADER_STRING(5),
    .words16 =
        {
            0x63,
            0x6c,
            0x69,
            0x63,
            0x6b,
        },
};
ElmString16 literal_string_button = {
    .header = HEADER_STRING(6),
    .words16 =
        {
            0x62,
            0x75,
            0x74,
            0x74,
            0x6f,
            0x6e,
        },
};
ElmString16 literal_string_br = {
    .header = HEADER_STRING(2),
    .words16 =
        {
            0x62,
            0x72,
        },
};
ElmString16 literal_string_Task = {
    .header = HEADER_STRING(4),
    .words16 =
        {
            0x54,
            0x61,
            0x73,
            0x6b,
        },
};
ElmString16 literal_string_Start_20countdown = {
    .header = HEADER_STRING(15),
    .words16 =
        {
            0x53,
            0x74,
            0x61,
            0x72,
            0x74,
            0x20,
            0x63,
            0x6f,
            0x75,
            0x6e,
            0x74,
            0x64,
            0x6f,
            0x77,
            0x6e,
        },
};
ElmString16 literal_string_Click_20the_20button_21_20_0001f60a = {
    .header = HEADER_STRING(20),
    .words16 =
        {
            0x43,
            0x6c,
            0x69,
            0x63,
            0x6b,
            0x20,
            0x74,
            0x68,
            0x65,
            0x20,
            0x62,
            0x75,
            0x74,
            0x74,
            0x6f,
            0x6e,
            0x21,
            0x20,
            0xd83d,
            0xde0a,
        },
};
ElmFloat literal_float_1234_5 = {
    .header = HEADER_FLOAT,
    .value = 1234.5,
};
ElmInt literal_int_5 = {
    .header = HEADER_INT,
    .value = 5,
};
ElmInt literal_int_1 = {
    .header = HEADER_INT,
    .value = 1,
};
ElmInt literal_int_0 = {
    .header = HEADER_INT,
    .value = 0,
};
FieldGroup* app_field_groups[] = {
    &fg_init_subscriptions_update_view,
    NULL,
};

#define elm_browser_Browser_element Browser_element

#define elm_core_Platform_Cmd_batch Platform_batch
#define elm_core_Platform_Cmd_none (*ptr_elm_core_Platform_Cmd_none)
Closure* ptr_elm_core_Platform_Cmd_none;
void* init_elm_core_Platform_Cmd_none() {
  return A1(&elm_core_Platform_Cmd_batch, &Nil);
}
void* eval_author_project_Main_init(void* args[]) {
  void* x__v0 = args[0];
  return NEW_TUPLE2(&literal_int_0, &elm_core_Platform_Cmd_none);
}
Closure author_project_Main_init = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Main_init,
};

#define elm_core_Platform_Sub_batch Platform_batch
#define elm_core_Platform_Sub_none (*ptr_elm_core_Platform_Sub_none)
Closure* ptr_elm_core_Platform_Sub_none;
void* init_elm_core_Platform_Sub_none() {
  return A1(&elm_core_Platform_Sub_batch, &Nil);
}
void* eval_author_project_Main_subscriptions(void* args[]) {
  void* x__v0 = args[0];
  return &elm_core_Platform_Sub_none;
}
Closure author_project_Main_subscriptions = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Main_subscriptions,
};

#define elm_json_Json_Decode_succeed Json_succeed

void* eval_author_project_Main_SetCounter(void* args[]) {
  return NEW_CUSTOM(CTOR_SetCounter, 1, args);
}
Closure author_project_Main_SetCounter = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Main_SetCounter,
};

void* eval_elm_core_Basics_identity(void* args[]) {
  void* x_x = args[0];
  return x_x;
}
Closure elm_core_Basics_identity = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_core_Basics_identity,
};
void* eval_author_project_Main_funcSentToJsAndBack(void* args[]) {
  void* x_next = args[0];
  void* x__v0 = args[1];
  return A1(&author_project_Main_SetCounter, x_next);
}
Closure author_project_Main_funcSentToJsAndBack = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Main_funcSentToJsAndBack,
};

void* eval_elm_core_Task_Perform(void* args[]) {
  return NEW_CUSTOM(CTOR_Perform, 1, args);
}
Closure elm_core_Task_Perform = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_core_Task_Perform,
};

Closure elm_core_Task_command = {
    .header = HEADER_CLOSURE(1),
    .n_values = 0x1,
    .max_values = 0xffff,
    .evaluator = (void*)JS_Platform_leaf,
    .values =
        {
            &literal_string_Task,
        },
};

#define elm_core_Task_andThen Scheduler_andThen

void* eval_elm_core_Basics_apR(void* args[]) {
  void* x_x = args[0];
  void* x_f = args[1];
  return A1(x_f, x_x);
}
Closure elm_core_Basics_apR = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_elm_core_Basics_apR,
};

#define elm_core_Task_succeed Scheduler_succeed
void* eval_elm_core_Task_map_lambda0(void* args[]) {
  void* x_func = args[0];
  void* x_a = args[1];
  return A1(&elm_core_Task_succeed, A1(x_func, x_a));
}
void* eval_elm_core_Task_map(void* args[]) {
  void* x_func = args[0];
  void* x_taskA = args[1];
  return A2(&elm_core_Basics_apR,
      x_taskA,
      A1(&elm_core_Task_andThen,
          NEW_CLOSURE(1,
              2,
              &eval_elm_core_Task_map_lambda0,
              ((void* []){
                  x_func,
              }))));
}
Closure elm_core_Task_map = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_elm_core_Task_map,
};
void* eval_elm_core_Task_perform(void* args[]) {
  void* x_toMessage = args[0];
  void* x_task = args[1];
  return A1(&elm_core_Task_command,
      A1(&elm_core_Task_Perform, A2(&elm_core_Task_map, x_toMessage, x_task)));
}
Closure elm_core_Task_perform = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_elm_core_Task_perform,
};

#define elm_core_Process_sleep Process_sleep
void* eval_author_project_Main_delayedSetCounter(void* args[]) {
  void* x_next = args[0];
  void* x_partiallyAppliedFuncSentToJsAndBack =
      A1(&author_project_Main_funcSentToJsAndBack, x_next);
  return A2(&elm_core_Task_perform,
      x_partiallyAppliedFuncSentToJsAndBack,
      A1(&elm_core_Process_sleep, &literal_float_1234_5));
}
Closure author_project_Main_delayedSetCounter = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Main_delayedSetCounter,
};

#define elm_core_Basics_eq Utils_equal

#define elm_core_Basics_sub Basics_sub
void* eval_author_project_Main_update(void* args[]) {
  void* x__v0 = args[0];
  void* x__v1 = args[1];
  void* x_newModel = ((Custom*)x__v0)->values[0];
  void* x_cmd = A2(&elm_core_Basics_eq, x_newModel, &literal_int_0) == &True
                    ? &elm_core_Platform_Cmd_none
                    : A1(&author_project_Main_delayedSetCounter,
                          A2(&elm_core_Basics_sub, x_newModel, &literal_int_1));
  return NEW_TUPLE2(x_newModel, x_cmd);
}
Closure author_project_Main_update = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_author_project_Main_update,
};

#define elm_html_Html_br (*ptr_elm_html_Html_br)
Closure* ptr_elm_html_Html_br;
void* init_elm_html_Html_br() {
  return A1(&VirtualDom_node, &literal_string_br);
}

#define elm_html_Html_button (*ptr_elm_html_Html_button)
Closure* ptr_elm_html_Html_button;
void* init_elm_html_Html_button() {
  return A1(&VirtualDom_node, &literal_string_button);
}

#define elm_html_Html_div (*ptr_elm_html_Html_div)
Closure* ptr_elm_html_Html_div;
void* init_elm_html_Html_div() {
  return A1(&VirtualDom_node, &literal_string_div);
}

#define elm_core_String_fromInt String_fromNumber

#define elm_html_Html_h1 (*ptr_elm_html_Html_h1)
Closure* ptr_elm_html_Html_h1;
void* init_elm_html_Html_h1() {
  return A1(&VirtualDom_node, &literal_string_h1);
}

void* eval_elm_virtual_dom_VirtualDom_Normal(void* args[]) {
  return NEW_CUSTOM(CTOR_Normal, 1, args);
}
Closure elm_virtual_dom_VirtualDom_Normal = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_virtual_dom_VirtualDom_Normal,
};

#define elm_virtual_dom_VirtualDom_on VirtualDom_on
void* eval_elm_html_Html_Events_on(void* args[]) {
  void* x_event = args[0];
  void* x_decoder = args[1];
  return A2(&elm_virtual_dom_VirtualDom_on,
      x_event,
      A1(&elm_virtual_dom_VirtualDom_Normal, x_decoder));
}
Closure elm_html_Html_Events_on = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x2,
    .evaluator = &eval_elm_html_Html_Events_on,
};
void* eval_elm_html_Html_Events_onClick(void* args[]) {
  void* x_msg = args[0];
  return A2(&elm_html_Html_Events_on,
      &literal_string_click,
      A1(&elm_json_Json_Decode_succeed, x_msg));
}
Closure elm_html_Html_Events_onClick = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_elm_html_Html_Events_onClick,
};

#define elm_virtual_dom_VirtualDom_text VirtualDom_text
#define elm_html_Html_text elm_virtual_dom_VirtualDom_text
void* eval_author_project_Main_view(void* args[]) {
  void* x_model = args[0];
  void* x_isZero = A2(&elm_core_Basics_eq, x_model, &literal_int_0);
  void* x_str = x_isZero == &True ? &literal_string_Click_20the_20button_21_20_0001f60a
                                  : A1(&elm_core_String_fromInt, x_model);
  return A2(&elm_html_Html_div,
      &Nil,
      List_create(2,
          ((void* []){
              A2(&elm_html_Html_h1,
                  &Nil,
                  List_create(1,
                      ((void* []){
                          A1(&elm_html_Html_text, x_str),
                      }))),
              x_isZero == &True
                  ? A2(&elm_html_Html_button,
                        List_create(1,
                            ((void* []){
                                A1(&elm_html_Html_Events_onClick,
                                    A1(&author_project_Main_SetCounter, &literal_int_5)),
                            })),
                        List_create(1,
                            ((void* []){
                                A1(&elm_html_Html_text,
                                    &literal_string_Start_20countdown),
                            })))
                  : A2(&elm_html_Html_br, &Nil, &Nil),
          })));
}
Closure author_project_Main_view = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0x0,
    .max_values = 0x1,
    .evaluator = &eval_author_project_Main_view,
};
#define author_project_Main_main (*ptr_author_project_Main_main)
Closure* ptr_author_project_Main_main;
void* init_author_project_Main_main() {
  return A1(&elm_browser_Browser_element,
      NEW_RECORD(&fg_init_subscriptions_update_view,
          4,
          ((void* []){
              &author_project_Main_init,
              &author_project_Main_subscriptions,
              &author_project_Main_update,
              &author_project_Main_view,
          })));
}

void** mains[] = {
    &ptr_author_project_Main_main,
    NULL,
};

int EMSCRIPTEN_KEEPALIVE main() {
  printf("main\n");
  int exit_code = GC_init();
  if (exit_code) return exit_code;
  Utils_initGlobal(&ptr_elm_core_Platform_Cmd_none, &init_elm_core_Platform_Cmd_none);
  Utils_initGlobal(&ptr_elm_core_Platform_Sub_none, &init_elm_core_Platform_Sub_none);
  Utils_initGlobal(&ptr_elm_html_Html_br, &init_elm_html_Html_br);
  Utils_initGlobal(&ptr_elm_html_Html_button, &init_elm_html_Html_button);
  Utils_initGlobal(&ptr_elm_html_Html_div, &init_elm_html_Html_div);
  Utils_initGlobal(&ptr_elm_html_Html_h1, &init_elm_html_Html_h1);
  Utils_initGlobal(&ptr_author_project_Main_main, &init_author_project_Main_main);
  Wrapper_registerFieldGroups(app_field_groups);
  printf("mains[0] %p\n", mains[0]);
  printf("*mains[0] %p\n", *mains[0]);
  Wrapper_registerMains(mains);
  return 0;
}
