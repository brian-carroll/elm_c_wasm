#include <stdio.h>
#include "../../../src/kernel/kernel.h"

enum {
  JS_Json_succeed,
  JS_Platform_batch,
  JS_Platform_leaf,
  JS_Scheduler_andThen,
  JS_Scheduler_succeed,
  JS_Time_now,
  JS_VirtualDom_node,
  JS_VirtualDom_on,
  JS_VirtualDom_text
};

enum {
  CTOR_GetTime,
  CTOR_GotTime,
  CTOR_Normal,
  CTOR_Perform,
  CTOR_Posix,
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

const Closure elm_core_Platform_Cmd_batch = {
    .header = HEADER_CLOSURE(0),
    .evaluator = (void*)JS_Platform_batch,
    .max_values = NEVER_EVALUATE,
};
const Closure elm_core_Platform_Cmd_none = {
    .header = HEADER_CLOSURE(1),
    .evaluator = (void*)JS_Platform_batch,
    .n_values = 1,
    .max_values = NEVER_EVALUATE,
    .values = {&Nil},
};
const Closure elm_core_Platform_Sub_batch = {
    .header = HEADER_CLOSURE(0),
    .evaluator = (void*)JS_Platform_batch,
    .max_values = NEVER_EVALUATE,
};
const Closure elm_core_Platform_Sub_none = {
    .header = HEADER_CLOSURE(1),
    .evaluator = (void*)JS_Platform_batch,
    .n_values = 1,
    .max_values = NEVER_EVALUATE,
    .values = {&Nil},
};

// Task

const Closure elm_core_Task_succeed = {
    .header = HEADER_CLOSURE(0),
    .max_values = NEVER_EVALUATE,
    .evaluator = (void*)JS_Scheduler_succeed,
};
const ElmString literal_string_Task = {
    .header = HEADER_STRING(4),
    .bytes =
        {
            'T',
            0,
            'a',
            0,
            's',
            0,
            'k',
            0,
        },
};
const Closure elm_core_Task_command = {
    .header = HEADER_CLOSURE(1),
    .max_values = NEVER_EVALUATE,
    .n_values = 1,
    .evaluator = (void*)JS_Platform_leaf,
    .values = {&literal_string_Task},
};
const Closure elm_core_Task_andThen = {
    .header = HEADER_CLOSURE(0),
    .max_values = NEVER_EVALUATE,
    .evaluator = (void*)JS_Scheduler_andThen,
};
void* eval_Task_Perform(void* args[1]) {
  Custom* p = GC_malloc(sizeof(Custom) + sizeof(void*));
  p->header = HEADER_CUSTOM(1);
  p->ctor = CTOR_Perform;
  p->values[0] = args[0];
  return p;
}
const Closure elm_core_Task_Perform = {
    .header = HEADER_CLOSURE(0),
    .max_values = 1,
    .evaluator = &eval_Task_Perform,
};

void* eval_Task_map_inner(void* args[2]) {
  Closure* func = args[0];
  void* a = args[1];
  return A1(&elm_core_Task_succeed, A1(func, a));
}
const Closure elm_core_Task_map_inner = {
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
const Closure elm_core_Task_map = {
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
const Closure elm_core_Task_perform = {
    .header = HEADER_CLOSURE(0),
    .max_values = 2,
    .evaluator = &eval_elm_core_Task_perform,
};

// Time

void* eval_elm_time_Time_Posix(void* args[1]) {
  Custom* p = GC_malloc(sizeof(Custom) + sizeof(void*));
  p->header = HEADER_CUSTOM(1);
  p->ctor = CTOR_Posix;
  p->values[0] = args[0];
  return p;
};
const Closure elm_time_Time_Posix = {
    .header = HEADER_CLOSURE(0),
    .max_values = 1,
    .evaluator = &eval_elm_time_Time_Posix,
};
#define elm_time_Time_millisToPosix elm_time_Time_Posix
void* eval_elm_time_Time_posixToMillis(void* args[1]) {
  Custom* n0 = args[0];
  void* millis = n0->values[0];
  return millis;
}
const Closure elm_time_Time_posixToMillis = {
    .header = HEADER_CLOSURE(0),
    .max_values = 1,
    .evaluator = &eval_elm_time_Time_posixToMillis,
};
const Closure elm_time_Time_now = {
    .header = HEADER_CLOSURE(1),
    .n_values = 1,
    .max_values = NEVER_EVALUATE,
    .evaluator = (void*)JS_Time_now,
    .values = {&elm_time_Time_millisToPosix},
};

// Json

const Closure elm_json_Json_Decode_succeed = {
    .header = HEADER_CLOSURE(0),
    .max_values = NEVER_EVALUATE,
    .evaluator = (void*)JS_Json_succeed,
};

// VirtualDom

const Closure elm_virtual_dom_VirtualDom_text = {
    .header = HEADER_CLOSURE(0),
    .max_values = NEVER_EVALUATE,
    .evaluator = (void*)JS_VirtualDom_text,
};

const Closure elm_virtual_dom_VirtualDom_on = {
    .header = HEADER_CLOSURE(0),
    .max_values = NEVER_EVALUATE,
    .evaluator = (void*)JS_VirtualDom_on,
};

void* eval_elm_virtual_dom_VirtualDom_Normal(void* args[1]) {
  Custom* p = GC_malloc(sizeof(Custom) + sizeof(void*));
  p->header = HEADER_CUSTOM(1);
  p->ctor = CTOR_Normal;
  p->values[0] = args[0];
  return p;
};
const Closure elm_virtual_dom_VirtualDom_Normal = {
    .header = HEADER_CLOSURE(0),
    .max_values = 1,
    .evaluator = &eval_elm_virtual_dom_VirtualDom_Normal,
};

// Html

const ElmString literal_string_br = {
    .header = HEADER_STRING(2),
    .bytes =
        {
            'b',
            0,
            'r',
            0,
        },
};
const ElmString literal_string_button = {.header = HEADER_STRING(6),
    .bytes = {
        'b',
        0,
        'u',
        0,
        't',
        0,
        't',
        0,
        'o',
        0,
        'n',
        0,
    }};
const ElmString literal_string_div = {
    .header = HEADER_STRING(3),
    .bytes =
        {
            'd',
            0,
            'i',
            0,
            'v',
            0,
        },
};
const ElmString literal_string_click = {
    .header = HEADER_STRING(5),
    .bytes =
        {
            'c',
            0,
            'l',
            0,
            'i',
            0,
            'c',
            0,
            'k',
            0,
        },
};
const Closure elm_html_Html_br = {
    .header = HEADER_CLOSURE(1),
    .evaluator = (void*)JS_VirtualDom_node,
    .n_values = 1,
    .max_values = NEVER_EVALUATE,
    .values = {&literal_string_br},
};
const Closure elm_html_Html_button = {
    .header = HEADER_CLOSURE(1),
    .evaluator = (void*)JS_VirtualDom_node,
    .n_values = 1,
    .max_values = NEVER_EVALUATE,
    .values = {&literal_string_button},
};
const Closure elm_html_Html_div = {
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
const Closure elm_html_Html_Events_on = {
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
const Closure elm_html_Html_Events_onClick = {
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

const Custom author_project_Main_GetTime = {
    .header = HEADER_CUSTOM(0),
    .ctor = CTOR_GetTime,
};

void* eval_author_project_Main_GotTime(void* args[1]) {
  Custom* p = GC_malloc(sizeof(Custom) + sizeof(void*));
  p->header = HEADER_CUSTOM(1);
  p->ctor = CTOR_GotTime;
  p->values[0] = args[0];
  return p;
};
const Closure author_project_Main_GotTime = {
    .header = HEADER_CLOSURE(0),
    .max_values = 1,
    .evaluator = &eval_author_project_Main_GotTime,
};

void* author_project_Main_cmdTime;
void* init_author_project_Main_cmdTime() {
  author_project_Main_cmdTime =
      A2(&elm_core_Task_perform, &author_project_Main_GotTime, &elm_time_Time_now);
  return NULL;
}

// init

const ElmInt literal_int_0 = {
    .header = HEADER_INT,
    .value = 0,
};
void* eval_author_project_Main_init(void* args[1]) {
  return NEW_TUPLE2(&literal_int_0, author_project_Main_cmdTime);
}
const Closure author_project_Main_init = {
    .header = HEADER_CLOSURE(0),
    .max_values = 1,
    .evaluator = &eval_author_project_Main_init,
};

// update

void* eval_author_project_Main_update(void* args[2]) {
  Custom* msg = args[0];
  void* model = args[1];
  if (msg->ctor == CTOR_GetTime) {
    return NEW_TUPLE2(model, author_project_Main_cmdTime);
  } else {
    void* posix = msg->values[0];
    return NEW_TUPLE2(
        A1(&elm_time_Time_posixToMillis, posix), &elm_core_Platform_Cmd_none);
  }
}
const Closure author_project_Main_update = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_author_project_Main_update,
    .max_values = 2,
};

const ElmString literal_string_Refresh = {
    .header = HEADER_STRING(7),
    .bytes =
        {
            'R',
            0,
            'e',
            0,
            'f',
            0,
            'r',
            0,
            'e',
            0,
            's',
            0,
            'h',
            0,
        },
};

// view

void* eval_author_project_Main_view(void* args[1]) {
  void* model = args[0];
  return A2(&elm_html_Html_div,
      &Nil,
      List_fromArray(3,
          (void* []){A1(&elm_html_Html_text, A1(&String_fromInt, model)),
              A2(&elm_html_Html_br, &Nil, &Nil),
              A2(&elm_html_Html_button,
                  List_fromArray(1,
                      (void* []){A1(
                          &elm_html_Html_Events_onClick, &author_project_Main_GetTime)}),
                  List_fromArray(1,
                      (void* []){A1(&elm_html_Html_text, &literal_string_Refresh)}))}));
}
const Closure author_project_Main_view = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_author_project_Main_view,
    .max_values = 1,
};

// subscriptions

void* eval_author_project_Main_subscriptions(void* args[1]) {
  return &elm_core_Platform_Sub_none;
}
const Closure author_project_Main_subscriptions = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_author_project_Main_subscriptions,
    .max_values = 1,
};

// Elm main (export to JS wrapper)

const Record main_record = {
    .header = HEADER_RECORD(4),
    .fieldgroup = &fg_init_subscriptions_update_view,
    .values =
        {
            &author_project_Main_init,
            &author_project_Main_subscriptions,
            &author_project_Main_update,
            &author_project_Main_view,
        },
};

// C main (Wasm module initialisation)

int EMSCRIPTEN_KEEPALIVE main(int argc, char** argv) {
  int exit_code = GC_init();
  if (exit_code) return exit_code;

  // NOTE: real codegen will have to account for heap overflow during init but meh
  GC_register_root(&author_project_Main_cmdTime);
  init_author_project_Main_cmdTime();

  wrapper_register_fieldGroups(app_field_groups);
  wrapper_register_mainRecord(&main_record);

  printf("%p author_project_Main_cmdTime\n", author_project_Main_cmdTime);
  printf("\n");
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
  printf("%p elm_time_Time_Posix\n", &elm_time_Time_Posix);
  printf("%p elm_time_Time_posixToMillis\n", &elm_time_Time_posixToMillis);
  printf("%p elm_time_Time_now\n", &elm_time_Time_now);
  printf("%p elm_json_Json_Decode_succeed\n", &elm_json_Json_Decode_succeed);
  printf("%p elm_virtual_dom_VirtualDom_text\n", &elm_virtual_dom_VirtualDom_text);
  printf("%p elm_virtual_dom_VirtualDom_on\n", &elm_virtual_dom_VirtualDom_on);
  printf("%p elm_virtual_dom_VirtualDom_Normal\n", &elm_virtual_dom_VirtualDom_Normal);
  printf("%p literal_string_br\n", &literal_string_br);
  printf("%p literal_string_button\n", &literal_string_button);
  printf("%p literal_string_div\n", &literal_string_div);
  printf("%p literal_string_click\n", &literal_string_click);
  printf("%p elm_html_Html_br\n", &elm_html_Html_br);
  printf("%p elm_html_Html_button\n", &elm_html_Html_button);
  printf("%p elm_html_Html_div\n", &elm_html_Html_div);
  printf("%p elm_html_Html_Events_on\n", &elm_html_Html_Events_on);
  printf("%p elm_html_Html_Events_onClick\n", &elm_html_Html_Events_onClick);
  printf("%p author_project_Main_GetTime\n", &author_project_Main_GetTime);
  printf("%p author_project_Main_GotTime\n", &author_project_Main_GotTime);
  printf("%p literal_int_0\n", &literal_int_0);
  printf("%p author_project_Main_init\n", &author_project_Main_init);
  printf("%p author_project_Main_update\n", &author_project_Main_update);
  printf("%p literal_string_Refresh\n", &literal_string_Refresh);
  printf("%p author_project_Main_view\n", &author_project_Main_view);
  printf("%p author_project_Main_subscriptions\n", &author_project_Main_subscriptions);
  printf("%p main_record\n", &main_record);

  printf("%p eval_Task_Perform\n", &eval_Task_Perform);
  printf("%p eval_Task_map_inner\n", &eval_Task_map_inner);
  printf("%p eval_Task_map\n", &eval_Task_map);
  printf("%p eval_elm_core_Task_perform\n", &eval_elm_core_Task_perform);
  printf("%p eval_elm_time_Time_Posix\n", &eval_elm_time_Time_Posix);
  printf("%p eval_elm_time_Time_posixToMillis\n", &eval_elm_time_Time_posixToMillis);
  printf("%p eval_elm_virtual_dom_VirtualDom_Normal\n",
      &eval_elm_virtual_dom_VirtualDom_Normal);
  printf("%p eval_elm_html_Html_Events_on\n", &eval_elm_html_Html_Events_on);
  printf("%p eval_elm_html_Html_Events_onClick\n", &eval_elm_html_Html_Events_onClick);
  printf("%p eval_author_project_Main_GotTime\n", &eval_author_project_Main_GotTime);
  printf("%p eval_author_project_Main_init\n", &eval_author_project_Main_init);
  printf("%p eval_author_project_Main_update\n", &eval_author_project_Main_update);
  printf("%p eval_author_project_Main_view\n", &eval_author_project_Main_view);
  printf("%p eval_author_project_Main_subscriptions\n",
      &eval_author_project_Main_subscriptions);

  return exit_code;
}