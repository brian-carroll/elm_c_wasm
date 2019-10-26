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
    .fields[0] = FIELD_init,
    .fields[1] = FIELD_subscriptions,
    .fields[2] = FIELD_update,
    .fields[3] = FIELD_view,
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
    .max_values = NEVER_EVALUATE,
    .values[0] = &Nil,
};
const Closure elm_core_Platform_Sub_batch = {
    .header = HEADER_CLOSURE(0),
    .evaluator = (void*)JS_Platform_batch,
    .max_values = NEVER_EVALUATE,
};
const Closure elm_core_Platform_Sub_none = {
    .header = HEADER_CLOSURE(1),
    .evaluator = (void*)JS_Platform_batch,
    .max_values = NEVER_EVALUATE,
    .values[0] = &Nil,
};

// Task

const Closure elm_core_Task_succeed = {
    .header = HEADER_CLOSURE(0),
    .max_values = NEVER_EVALUATE,
    .evaluator = (void*)JS_Scheduler_succeed,
};
const ElmString literal_string_Task = {
    .header = HEADER_STRING(4),
    .bytes[0] = 'T',
    .bytes[2] = 'a',
    .bytes[4] = 's',
    .bytes[8] = 'k',
};
const Closure elm_core_Task_command = {
    .header = HEADER_CLOSURE(1),
    .max_values = NEVER_EVALUATE,
    .evaluator = (void*)JS_Platform_leaf,
    .values[0] = &literal_string_Task,
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
    .max_values = NEVER_EVALUATE,
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

// Time

void* eval_elm_time_Time_Posix(void* args[1]) {
  Custom* p = GC_malloc(sizeof(Custom) + sizeof(void*));
  p->header = HEADER_CUSTOM(1);
  p->ctor = CTOR_Posix;
  p->values[0] = args[0];
  return p;
};
Closure elm_time_Time_Posix = {
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
Closure elm_time_Time_posixToMillis = {
    .header = HEADER_CLOSURE(0),
    .max_values = 1,
    .evaluator = &eval_elm_time_Time_posixToMillis,
};
Closure elm_time_Time_now = {
    .header = HEADER_CLOSURE(1),
    .max_values = NEVER_EVALUATE,
    .evaluator = (void*)JS_Time_now,
    .values[0] = &elm_time_Time_millisToPosix,
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
  Custom* p = GC_malloc(sizeof(Custom) + sizeof(void*));
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

ElmString literal_string_br = {
    .header = HEADER_STRING(2),
    .bytes[0] = 'b',
    .bytes[2] = 'r',
};
ElmString literal_string_button = {
    .header = HEADER_STRING(6),
    .bytes[0] = 'b',
    .bytes[2] = 'u',
    .bytes[4] = 't',
    .bytes[6] = 't',
    .bytes[8] = 'o',
    .bytes[10] = 'n',
};
ElmString literal_string_div = {
    .header = HEADER_STRING(3),
    .bytes[0] = 'd',
    .bytes[2] = 'i',
    .bytes[4] = 'v',
};
ElmString literal_string_click = {
    .header = HEADER_STRING(5),
    .bytes[0] = 'c',
    .bytes[2] = 'l',
    .bytes[4] = 'i',
    .bytes[6] = 'c',
    .bytes[8] = 'k',
};
Closure elm_html_Html_br = {
    .header = HEADER_CLOSURE(1),
    .evaluator = (void*)JS_VirtualDom_node,
    .max_values = NEVER_EVALUATE,
    .values[0] = &literal_string_br,
};
Closure elm_html_Html_button = {
    .header = HEADER_CLOSURE(1),
    .evaluator = (void*)JS_VirtualDom_node,
    .max_values = NEVER_EVALUATE,
    .values[0] = &literal_string_button,
};
Closure elm_html_Html_div = {
    .header = HEADER_CLOSURE(1),
    .evaluator = (void*)JS_VirtualDom_node,
    .max_values = NEVER_EVALUATE,
    .values[0] = &literal_string_div,
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

Custom author_project_Main_GetTime = {
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
Closure author_project_Main_GotTime = {
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

ElmInt literal_int_0 = {
    .header = HEADER_INT,
    .value = 0,
};
void* eval_author_project_Main_init(void* args[1]) {
  return NEW_TUPLE2(&literal_int_0, author_project_Main_cmdTime);
}
Closure author_project_Main_init = {
    .header = HEADER_CLOSURE(0),
    .max_values = 1,
    .evaluator = &eval_author_project_Main_init,
};

// update

void* eval_author_project_Main_update(void* args[2]) {
  Custom* msg = args[0];
  void* model = args[1];
  if (msg->ctor == CTOR_GetTime) {
    return NEW_TUPLE2(model, &author_project_Main_cmdTime);
  } else {
    void* posix = msg->values[0];
    return NEW_TUPLE2(
        A1(&elm_time_Time_posixToMillis, posix), &elm_core_Platform_Cmd_none);
  }
}
Closure author_project_Main_update = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_author_project_Main_update,
    .max_values = 2,
};

ElmString literal_string_Refresh = {
    .header = HEADER_STRING(7),
    .bytes[0] = 'R',
    .bytes[2] = 'e',
    .bytes[4] = 'f',
    .bytes[6] = 'r',
    .bytes[8] = 'e',
    .bytes[10] = 's',
    .bytes[12] = 'h',
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
Closure author_project_Main_view = {
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
    .values[0] = &author_project_Main_init,
    .values[1] = &author_project_Main_subscriptions,
    .values[2] = &author_project_Main_update,
    .values[3] = &author_project_Main_view,
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

  return exit_code;
}