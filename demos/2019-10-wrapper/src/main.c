#include <stdio.h>
#include "../../../src/kernel/kernel.h"

enum {
  JS_Json_succeed,
  JS_Platform_batch,
  JS_Platform_leaf,
  JS_Process_sleep,
  JS_Scheduler_andThen,
  JS_Scheduler_succeed,
  JS_VirtualDom_node,
  JS_VirtualDom_on,
  JS_VirtualDom_text
};

enum {
  CTOR_Normal,
  CTOR_Perform,
  CTOR_Posix,
  CTOR_SetCounter,
};

enum {
  FIELD_init,
  FIELD_subscriptions,
  FIELD_update,
  FIELD_view,
};

FieldGroup fg_init_subscriptions_update_view = {
    .size = 4,
    .fields = {FIELD_init, FIELD_subscriptions, FIELD_update, FIELD_view},
};

FieldGroup* app_field_groups[] = {
    &fg_init_subscriptions_update_view,
    NULL,
};

/***********************************************
 *
 *               ELM LIBRARIES
 *
 ***********************************************/

// Platform

Closure elm_core_Platform_Cmd_batch = {
    .header = HEADER_CLOSURE(0),
    .evaluator = (void*)JS_Platform_batch,
    .max_values = NEVER_EVALUATE,
};
Closure elm_core_Platform_Cmd_none = {
    .header = HEADER_CLOSURE(1),
    .evaluator = (void*)JS_Platform_batch,
    .n_values = 1,
    .max_values = NEVER_EVALUATE,
    .values = {&Nil},
};
Closure elm_core_Platform_Sub_batch = {
    .header = HEADER_CLOSURE(0),
    .evaluator = (void*)JS_Platform_batch,
    .max_values = NEVER_EVALUATE,
};
Closure elm_core_Platform_Sub_none = {
    .header = HEADER_CLOSURE(1),
    .evaluator = (void*)JS_Platform_batch,
    .n_values = 1,
    .max_values = NEVER_EVALUATE,
    .values = {&Nil},
};

// Task

Closure elm_core_Task_succeed = {
    .header = HEADER_CLOSURE(0),
    .max_values = NEVER_EVALUATE,
    .evaluator = (void*)JS_Scheduler_succeed,
};
ElmString16 literal_string_Task = {
    .header = HEADER_STRING(4),
    .words16 =
        {
            (u16)'T',
            (u16)'a',
            (u16)'s',
            (u16)'k',
        },
};
Closure elm_core_Task_command = {
    .header = HEADER_CLOSURE(1),
    .max_values = NEVER_EVALUATE,
    .n_values = 1,
    .evaluator = (void*)JS_Platform_leaf,
    .values = {&literal_string_Task},
};
Closure elm_core_Task_andThen = {
    .header = HEADER_CLOSURE(0),
    .max_values = NEVER_EVALUATE,
    .evaluator = (void*)JS_Scheduler_andThen,
};
void* eval_Task_Perform(void* args[1]) {
  Custom* p = GC_malloc(true, sizeof(Custom) + sizeof(void*));
  p->header = HEADER_CUSTOM(1);
  p->ctor = CTOR_Perform;
  p->values[0] = args[0];
  return p;
}
Closure elm_core_Task_Perform = {
    .header = HEADER_CLOSURE(0),
    .max_values = 1,
    .evaluator = &eval_Task_Perform,
};

void* eval_Task_map_inner(void* args[2]) {
  Closure* func = args[0];
  void* a = args[1];
  return A1(&elm_core_Task_succeed, A1(func, a));
}
Closure elm_core_Task_map_inner = {
    .header = HEADER_CLOSURE(0),
    .max_values = 2,
    .evaluator = &eval_Task_map_inner,
};
void* eval_Task_map(void* args[2]) {
  Closure* func = args[0];
  void* taskA = args[1];
  Closure* callback = A1(&elm_core_Task_map_inner, func);
  return A2(&elm_core_Task_andThen, callback, taskA);
}
Closure elm_core_Task_map = {
    .header = HEADER_CLOSURE(0),
    .max_values = 2,
    .evaluator = &eval_Task_map,
};
// lower-case `perform` uses uppercase constructor `Perform`
void* eval_elm_core_Task_perform(void* args[2]) {
  void* toMessage = args[0];
  void* task = args[1];
  return A1(&elm_core_Task_command,
      A1(&elm_core_Task_Perform, A2(&elm_core_Task_map, toMessage, task)));
}
Closure elm_core_Task_perform = {
    .header = HEADER_CLOSURE(0),
    .max_values = 2,
    .evaluator = &eval_elm_core_Task_perform,
};

// Process

Closure elm_core_Process_sleep = {
    .header = HEADER_CLOSURE(0),
    .max_values = NEVER_EVALUATE,
    .evaluator = (void*)JS_Process_sleep,
};

// Json

Closure elm_json_Json_Decode_succeed = {
    .header = HEADER_CLOSURE(0),
    .max_values = NEVER_EVALUATE,
    .evaluator = (void*)JS_Json_succeed,
};

// VirtualDom

Closure elm_virtual_dom_VirtualDom_text = {
    .header = HEADER_CLOSURE(0),
    .max_values = NEVER_EVALUATE,
    .evaluator = (void*)JS_VirtualDom_text,
};

Closure elm_virtual_dom_VirtualDom_on = {
    .header = HEADER_CLOSURE(0),
    .max_values = NEVER_EVALUATE,
    .evaluator = (void*)JS_VirtualDom_on,
};

void* eval_elm_virtual_dom_VirtualDom_Normal(void* args[1]) {
  Custom* p = GC_malloc(true, sizeof(Custom) + sizeof(void*));
  p->header = HEADER_CUSTOM(1);
  p->ctor = CTOR_Normal;
  p->values[0] = args[0];
  return p;
};
Closure elm_virtual_dom_VirtualDom_Normal = {
    .header = HEADER_CLOSURE(0),
    .max_values = 1,
    .evaluator = &eval_elm_virtual_dom_VirtualDom_Normal,
};

// Html

ElmString16 literal_string_h1 = {
    .header = HEADER_STRING(2),
    .words16 =
        {
            (u16)'h',
            (u16)'1',
        },
};

ElmString16 literal_string_br = {
    .header = HEADER_STRING(2),
    .words16 =
        {
            (u16)'b',
            (u16)'r',
        },
};
ElmString16 literal_string_button = {
    .header = HEADER_STRING(6),
    .words16 =
        {
            (u16)'b',
            (u16)'u',
            (u16)'t',
            (u16)'t',
            (u16)'o',
            (u16)'n',
        },
};
ElmString16 literal_string_div = {
    .header = HEADER_STRING(3),
    .words16 =
        {
            (u16)'d',
            (u16)'i',
            (u16)'v',
        },
};
ElmString16 literal_string_click = {
    .header = HEADER_STRING(5),
    .words16 =
        {
            (u16)'c',
            (u16)'l',
            (u16)'i',
            (u16)'c',
            (u16)'k',
        },
};
Closure elm_html_Html_h1 = {
    .header = HEADER_CLOSURE(1),
    .evaluator = (void*)JS_VirtualDom_node,
    .n_values = 1,
    .max_values = NEVER_EVALUATE,
    .values = {&literal_string_h1},
};
Closure elm_html_Html_br = {
    .header = HEADER_CLOSURE(1),
    .evaluator = (void*)JS_VirtualDom_node,
    .n_values = 1,
    .max_values = NEVER_EVALUATE,
    .values = {&literal_string_br},
};
Closure elm_html_Html_button = {
    .header = HEADER_CLOSURE(1),
    .evaluator = (void*)JS_VirtualDom_node,
    .n_values = 1,
    .max_values = NEVER_EVALUATE,
    .values = {&literal_string_button},
};
Closure elm_html_Html_div = {
    .header = HEADER_CLOSURE(1),
    .evaluator = (void*)JS_VirtualDom_node,
    .n_values = 1,
    .max_values = NEVER_EVALUATE,
    .values = {&literal_string_div},
};
#define elm_html_Html_text elm_virtual_dom_VirtualDom_text

void* eval_elm_html_Html_Events_on(void* args[2]) {
  void* event = args[0];
  void* decoder = args[1];
  return A2(&elm_virtual_dom_VirtualDom_on,
      event,
      A1(&elm_virtual_dom_VirtualDom_Normal, decoder));
}
Closure elm_html_Html_Events_on = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_elm_html_Html_Events_on,
    .max_values = 2,
};

void* eval_elm_html_Html_Events_onClick(void* args[1]) {
  void* msg = args[0];
  return A2(&elm_html_Html_Events_on,
      &literal_string_click,
      A1(&elm_json_Json_Decode_succeed, msg));
}
Closure elm_html_Html_Events_onClick = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_elm_html_Html_Events_onClick,
    .max_values = 1,
};

/***********************************************
 *
 *                  APP
 *
 ***********************************************/

// constructors and constants

void* eval_author_project_Main_SetCounter(void* args[1]) {
  return ctorCustom(CTOR_SetCounter, 1, args);
};
Closure author_project_Main_SetCounter = {
    .header = HEADER_CLOSURE(0),
    .max_values = 1,
    .evaluator = &eval_author_project_Main_SetCounter,
};

// init

ElmInt literal_int_0 = {
    .header = HEADER_INT,
    .value = 0,
};
void* eval_author_project_Main_init(void* args[1]) {
  return ctorTuple2(&literal_int_0, &elm_core_Platform_Cmd_none);
}
Closure author_project_Main_init = {
    .header = HEADER_CLOSURE(0),
    .max_values = 1,
    .evaluator = &eval_author_project_Main_init,
};

// update

void* eval_author_project_Main_funcSentToJsAndBack(void* args[2]) {
  void* next = args[0];
  return A1(&author_project_Main_SetCounter, next);
}
Closure author_project_Main_funcSentToJsAndBack = {
    .header = HEADER_CLOSURE(0),
    .max_values = 2,
    .evaluator = &eval_author_project_Main_funcSentToJsAndBack,
};

ElmFloat literal_float_1234_5 = {
    .header = HEADER_FLOAT,
    .value = 1234.5,
};
void* eval_author_project_Main_delayedSetCounter(void* args[1]) {
  void* next = args[0];
  void* partiallyAppliedFuncSentToJsAndBack =
      A1(&author_project_Main_funcSentToJsAndBack, next);
  return A2(&elm_core_Task_perform,
      partiallyAppliedFuncSentToJsAndBack,
      A1(&elm_core_Process_sleep, &literal_float_1234_5));
};
Closure author_project_Main_delayedSetCounter = {
    .header = HEADER_CLOSURE(0),
    .max_values = 1,
    .evaluator = &eval_author_project_Main_delayedSetCounter,
};

ElmInt literal_int_1 = {
    .header = HEADER_INT,
    .value = 1,
};
void* eval_author_project_Main_update(void* args[2]) {
  Custom* msg = args[0];
  ElmInt* newModel = msg->values[0];
  void* cmd = A2(&Utils_equal, newModel, &literal_int_0) == &True
                  ? &elm_core_Platform_Cmd_none
                  : A1(&author_project_Main_delayedSetCounter,
                        A2(&Basics_sub, newModel, &literal_int_1));
  return ctorTuple2(newModel, cmd);
};
Closure author_project_Main_update = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_author_project_Main_update,
    .max_values = 2,
};

// view

ElmString16 literal_string_ClickTheButton = {
    .header = HEADER_STRING(17),
    .words16 =
        {
            (u16)'C',
            (u16)'l',
            (u16)'i',
            (u16)'c',
            (u16)'k',
            (u16)' ',
            (u16)'t',
            (u16)'h',
            (u16)'e',
            (u16)' ',
            (u16)'b',
            (u16)'u',
            (u16)'t',
            (u16)'t',
            (u16)'o',
            (u16)'n',
            (u16)'!',
        },
};

ElmString16 literal_string_StartCountdown = {
    .header = HEADER_STRING(15),
    .words16 =
        {
            (u16)'S',
            (u16)'t',
            (u16)'a',
            (u16)'r',
            (u16)'t',
            (u16)' ',
            (u16)'c',
            (u16)'o',
            (u16)'u',
            (u16)'n',
            (u16)'t',
            (u16)'d',
            (u16)'o',
            (u16)'w',
            (u16)'n',
        },
};
ElmInt literal_int_5 = {
    .header = HEADER_INT,
    .value = 5,
};

void* eval_author_project_Main_view(void* args[1]) {
  void* model = args[0];
  int isZero = A2(&Utils_equal, model, &literal_int_0) == &True;
  void* str = isZero ? &literal_string_ClickTheButton : A1(&String_fromInt, model);
  return A2(&elm_html_Html_div,
      &Nil,
      List_create(2,
          (void*[]){
              A2(&elm_html_Html_h1,
                  &Nil,
                  List_create(1,
                      (void*[]){
                          A1(&elm_html_Html_text, str),
                      })),
              isZero
                  ? A2(&elm_html_Html_button,
                        List_create(1,
                            (void*[]){
                                A1(&elm_html_Html_Events_onClick,
                                    A1(&author_project_Main_SetCounter, &literal_int_5)),
                            }),
                        List_create(1,
                            (void*[]){
                                A1(&elm_html_Html_text, &literal_string_StartCountdown),
                            }))
                  : A2(&elm_html_Html_br, &Nil, &Nil),
          }));
}
Closure author_project_Main_view = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_author_project_Main_view,
    .max_values = 1,
};

// subscriptions

void* eval_author_project_Main_subscriptions(void* args[1]) {
  return &elm_core_Platform_Sub_none;
}
Closure author_project_Main_subscriptions = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_author_project_Main_subscriptions,
    .max_values = 1,
};

// Elm main (export to JS wrapper)

void* eval_wasm_WasmWrapper_element(void* args[1]) {
  return args[0];
}
Closure wasm_WasmWrapper_element = {
    .header = HEADER_CLOSURE(0),
    .n_values = 0,
    .max_values = 1,
    .evaluator = &eval_wasm_WasmWrapper_element,
};

#define author_project_Main_main (*ptr_author_project_Main_main)
void* ptr_author_project_Main_main;
void* init_author_project_Main_main() {
  return A1(&wasm_WasmWrapper_element,
      ctorRecord(&fg_init_subscriptions_update_view,
          4,
          (void*[]){
              &author_project_Main_init,
              &author_project_Main_subscriptions,
              &author_project_Main_update,
              &author_project_Main_view,
          }));
}

// C main (Wasm module initialisation)

int EMSCRIPTEN_KEEPALIVE main(int argc, char** argv) {
  int exit_code = GC_init();
  if (exit_code) return exit_code;

  Utils_initGlobal(&ptr_author_project_Main_main, &init_author_project_Main_main);

  Wrapper_registerFieldGroups(app_field_groups);
  Wrapper_registerMainRecord(&ptr_author_project_Main_main);

  printf("Constant top level values:\n");
  printf("%p elm_core_Platform_Cmd_batch\n", &elm_core_Platform_Cmd_batch);
  printf("%p elm_core_Platform_Cmd_none\n", &elm_core_Platform_Cmd_none);
  printf("%p elm_core_Platform_Sub_batch\n", &elm_core_Platform_Sub_batch);
  printf("%p elm_core_Platform_Sub_none\n", &elm_core_Platform_Sub_none);
  printf("%p elm_core_Task_succeed\n", &elm_core_Task_succeed);
  printf("%p literal_string_Task\n", &literal_string_Task);
  printf("%p elm_core_Task_command\n", &elm_core_Task_command);
  printf("%p elm_core_Task_andThen\n", &elm_core_Task_andThen);
  printf("%p elm_core_Task_Perform\n", &elm_core_Task_Perform);
  printf("%p elm_core_Task_map_inner\n", &elm_core_Task_map_inner);
  printf("%p elm_core_Task_map\n", &elm_core_Task_map);
  printf("%p elm_core_Task_perform\n", &elm_core_Task_perform);
  printf("%p elm_core_Process_sleep\n", &elm_core_Process_sleep);
  printf("%p elm_json_Json_Decode_succeed\n", &elm_json_Json_Decode_succeed);
  printf("%p elm_virtual_dom_VirtualDom_text\n", &elm_virtual_dom_VirtualDom_text);
  printf("%p elm_virtual_dom_VirtualDom_on\n", &elm_virtual_dom_VirtualDom_on);
  printf("%p elm_virtual_dom_VirtualDom_Normal\n", &elm_virtual_dom_VirtualDom_Normal);
  printf("%p literal_string_h1\n", &literal_string_h1);
  printf("%p literal_string_button\n", &literal_string_button);
  printf("%p literal_string_div\n", &literal_string_div);
  printf("%p literal_string_click\n", &literal_string_click);
  printf("%p elm_html_Html_h1\n", &elm_html_Html_h1);
  printf("%p elm_html_Html_button\n", &elm_html_Html_button);
  printf("%p elm_html_Html_div\n", &elm_html_Html_div);
  printf("%p elm_html_Html_Events_on\n", &elm_html_Html_Events_on);
  printf("%p elm_html_Html_Events_onClick\n", &elm_html_Html_Events_onClick);
  printf("%p author_project_Main_SetCounter\n", &author_project_Main_SetCounter);
  printf("%p literal_int_0\n", &literal_int_0);
  printf("%p author_project_Main_init\n", &author_project_Main_init);
  printf("%p author_project_Main_funcSentToJsAndBack\n",
      &author_project_Main_funcSentToJsAndBack);
  printf("%p literal_float_1234_5\n", &literal_float_1234_5);
  printf("%p author_project_Main_delayedSetCounter\n",
      &author_project_Main_delayedSetCounter);
  printf("%p literal_int_1\n", &literal_int_1);
  printf("%p author_project_Main_update\n", &author_project_Main_update);
  printf("%p literal_string_ClickTheButton\n", &literal_string_ClickTheButton);
  printf("%p literal_string_StartCountdown\n", &literal_string_StartCountdown);
  printf("%p literal_int_5\n", &literal_int_5);
  printf("%p author_project_Main_view\n", &author_project_Main_view);
  printf("%p author_project_Main_subscriptions\n", &author_project_Main_subscriptions);
  printf("%p main_record\n", &main_record);

  printf("\n");
  printf("Closure evaluator functions:\n");
  printf("%p eval_Task_Perform\n", &eval_Task_Perform);
  printf("%p eval_Task_map_inner\n", &eval_Task_map_inner);
  printf("%p eval_Task_map\n", &eval_Task_map);
  printf("%p eval_elm_core_Task_perform\n", &eval_elm_core_Task_perform);
  printf("%p eval_elm_virtual_dom_VirtualDom_Normal\n",
      &eval_elm_virtual_dom_VirtualDom_Normal);
  printf("%p eval_elm_html_Html_Events_on\n", &eval_elm_html_Html_Events_on);
  printf("%p eval_elm_html_Html_Events_onClick\n", &eval_elm_html_Html_Events_onClick);
  printf(
      "%p eval_author_project_Main_SetCounter\n", &eval_author_project_Main_SetCounter);
  printf("%p eval_author_project_Main_init\n", &eval_author_project_Main_init);
  printf("%p eval_author_project_Main_funcSentToJsAndBack\n",
      &eval_author_project_Main_funcSentToJsAndBack);
  printf("%p eval_author_project_Main_delayedSetCounter\n",
      &eval_author_project_Main_delayedSetCounter);
  printf("%p eval_author_project_Main_update\n", &eval_author_project_Main_update);
  printf("%p eval_author_project_Main_view\n", &eval_author_project_Main_view);
  printf("%p eval_author_project_Main_subscriptions\n",
      &eval_author_project_Main_subscriptions);

  return exit_code;
}