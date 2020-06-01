#include <assert.h>
#include <math.h>
#include <stdbool.h>
#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#else
#define EMSCRIPTEN_KEEPALIVE
#include <stdio.h>
#endif

#include "../core/core.h"
#include "../wrapper/wrapper.h"
#include "json-elm.h"
#include "json-internal.h"
#include "json.h"

// Don't need A1,A2 macros for tail calls, or calls that don't allocate
#define TAIL_RESULT_OK(ptr) eval_elm_core_Result_Ok((void*[]){ptr})
#define TAIL_RESULT_ERR(ptr) eval_elm_core_Result_Err((void*[]){ptr})
#define RESULT_IS_OK(ptr) eval_elm_core_Result_isOk((void*[]){ptr})  // doesn't allocate

enum JsonFields {
  /*a*/ JsonField_msg,
  /*b*/ JsonField_decoder,
  /*c*/ JsonField_value,
  /*d*/ JsonField_field,
  /*e*/ JsonField_index,
  /*f*/ JsonField_func,
  /*g*/ JsonField_decoders,
  /*h*/ JsonField_callback,
};

static void* eval_Json_succeed(void* args[]) {
  return NEW_CUSTOM(DECODER_SUCCEED, 1, args);
}
Closure Json_succeed = {
    .header = HEADER_CLOSURE(0),
    .max_values = 1,
    .evaluator = &eval_Json_succeed,
};

static void* eval_Json_fail(void* args[]) {
  return NEW_CUSTOM(DECODER_FAIL, 1, args);
}
Closure Json_fail = {
    .header = HEADER_CLOSURE(0),
    .max_values = 1,
    .evaluator = &eval_Json_fail,
};

Custom Json_decodeInt = {
    .header = HEADER_CUSTOM(0),
    .ctor = DECODER_INT,
};
Custom Json_decodeBool = {
    .header = HEADER_CUSTOM(0),
    .ctor = DECODER_BOOL,
};
Custom Json_decodeFloat = {
    .header = HEADER_CUSTOM(0),
    .ctor = DECODER_FLOAT,
};
Custom Json_decodeValue = {
    .header = HEADER_CUSTOM(0),
    .ctor = DECODER_VALUE,
};
Custom Json_decodeString = {
    .header = HEADER_CUSTOM(0),
    .ctor = DECODER_STRING,
};

static void* eval_Json_decodeList(void* args[]) {
  void* decoder = args[0];
  return NEW_CUSTOM(DECODER_LIST, 2, ((void*[]){&Json_encodeNull, decoder}));
}
Closure Json_decodeList = {
    .header = HEADER_CLOSURE(0),
    .max_values = 1,
    .evaluator = &eval_Json_decodeList,
};

static void* eval_Json_decodeArray(void* args[]) {
  void* decoder = args[0];
  return NEW_CUSTOM(DECODER_ARRAY, 2, ((void*[]){&Json_encodeNull, decoder}));
}
Closure Json_decodeArray = {
    .header = HEADER_CLOSURE(0),
    .max_values = 1,
    .evaluator = &eval_Json_decodeArray,
};

static void* eval_Json_decodeNull(void* args[]) {
  void* value = args[0];
  return NEW_CUSTOM(
      DECODER_NULL, 3, ((void*[]){&Json_encodeNull, &Json_encodeNull, value}));
}
Closure Json_decodeNull = {
    .header = HEADER_CLOSURE(0),
    .max_values = 1,
    .evaluator = &eval_Json_decodeNull,
};

static void* eval_Json_decodeField(void* args[]) {
  void* field = args[0];
  void* decoder = args[1];
  return NEW_CUSTOM(DECODER_FIELD,
      4,
      ((void*[]){
          /*a*/ &Json_encodeNull,
          /*b*/ decoder,
          /*c*/ &Json_encodeNull,
          /*d*/ field,
      }));
}
Closure Json_decodeField = {
    .header = HEADER_CLOSURE(0),
    .max_values = 2,
    .evaluator = &eval_Json_decodeField,
};

static void* eval_Json_decodeIndex(void* args[]) {
  void* index = args[0];
  void* decoder = args[1];
  return NEW_CUSTOM(DECODER_INDEX,
      5,
      ((void*[]){
          /*a*/ &Json_encodeNull,
          /*b*/ decoder,
          /*c*/ &Json_encodeNull,
          /*d*/ &Json_encodeNull,
          /*e*/ index,
      }));
}
Closure Json_decodeIndex = {
    .header = HEADER_CLOSURE(0),
    .max_values = 2,
    .evaluator = &eval_Json_decodeIndex,
};

static void* eval_Json_decodeKeyValuePairs(void* args[]) {
  void* decoder = args[0];
  return NEW_CUSTOM(DECODER_KEY_VALUE, 2, ((void*[]){&Json_encodeNull, decoder}));
}
Closure Json_decodeKeyValuePairs = {
    .header = HEADER_CLOSURE(0),
    .max_values = 1,
    .evaluator = &eval_Json_decodeKeyValuePairs,
};

static void* eval_Json_andThen(void* args[]) {
  void* callback = args[0];
  void* decoder = args[1];
  return NEW_CUSTOM(DECODER_AND_THEN,
      8,
      ((void*[]){
          /*a*/ &Json_encodeNull,
          /*b*/ decoder,
          /*c*/ &Json_encodeNull,
          /*d*/ &Json_encodeNull,
          /*e*/ &Json_encodeNull,
          /*f*/ &Json_encodeNull,
          /*g*/ &Json_encodeNull,
          /*h*/ callback,
      }));
}
Closure Json_andThen = {
    .header = HEADER_CLOSURE(0),
    .max_values = 2,
    .evaluator = &eval_Json_andThen,
};

static void* eval_Json_oneOf(void* args[]) {
  void* decoders = args[0];
  return NEW_CUSTOM(DECODER_ONE_OF,
      7,
      ((void*[]){
          /*a*/ &Json_encodeNull,
          /*b*/ &Json_encodeNull,
          /*c*/ &Json_encodeNull,
          /*d*/ &Json_encodeNull,
          /*e*/ &Json_encodeNull,
          /*f*/ &Json_encodeNull,
          /*g*/ decoders,
      }));
}
Closure Json_oneOf = {
    .header = HEADER_CLOSURE(0),
    .max_values = 1,
    .evaluator = &eval_Json_oneOf,
};

// ----------------------------------------------------
//
// DECODING OBJECTS
//
// ----------------------------------------------------

static void* eval_Json_mapMany(void* args[]) {
  size_t n_decoders = (size_t)args[0];
  Closure* f = args[1];
  Custom* decoders = NEW_CUSTOM(JSON_VALUE_ARRAY, (u32)n_decoders, &args[2]);
  return NEW_CUSTOM(DECODER_MAP,
      7,
      ((void*[]){
          /*a*/ &Json_encodeNull,
          /*b*/ &Json_encodeNull,
          /*c*/ &Json_encodeNull,
          /*d*/ &Json_encodeNull,
          /*e*/ &Json_encodeNull,
          /*f*/ f,
          /*g*/ decoders,
      }));
}
Closure Json_map1 = {
    .evaluator = &eval_Json_mapMany,
    .header = HEADER_CLOSURE(1),
    .n_values = 1,
    .max_values = 3,
    .values = {(void*)1},
};
Closure Json_map2 = {
    .evaluator = &eval_Json_mapMany,
    .header = HEADER_CLOSURE(1),
    .n_values = 1,
    .max_values = 4,
    .values = {(void*)2},
};
Closure Json_map3 = {
    .evaluator = &eval_Json_mapMany,
    .header = HEADER_CLOSURE(1),
    .n_values = 1,
    .max_values = 5,
    .values = {(void*)3},
};
Closure Json_map4 = {
    .evaluator = &eval_Json_mapMany,
    .header = HEADER_CLOSURE(1),
    .n_values = 1,
    .max_values = 6,
    .values = {(void*)4},
};
Closure Json_map5 = {
    .evaluator = &eval_Json_mapMany,
    .header = HEADER_CLOSURE(1),
    .n_values = 1,
    .max_values = 7,
    .values = {(void*)5},
};
Closure Json_map6 = {
    .evaluator = &eval_Json_mapMany,
    .header = HEADER_CLOSURE(1),
    .n_values = 1,
    .max_values = 8,
    .values = {(void*)6},
};
Closure Json_map7 = {
    .evaluator = &eval_Json_mapMany,
    .header = HEADER_CLOSURE(1),
    .n_values = 1,
    .max_values = 9,
    .values = {(void*)7},
};
Closure Json_map8 = {
    .evaluator = &eval_Json_mapMany,
    .header = HEADER_CLOSURE(1),
    .n_values = 1,
    .max_values = 10,
    .values = {(void*)8},
};

// ----------------------------------------------------
//
// DECODE
//
// ----------------------------------------------------

ElmString16 str_err_expecting = {
    .words16 = {'E', 'x', 'p', 'e', 'c', 't', 'i', 'n', 'g', ' '},
    .header = HEADER_STRING(10),
};

ElmString16 str_err_Bool = {
    .header = HEADER_STRING(6), .words16 = {'a', ' ', 'B', 'O', 'O', 'L'}};
ElmString16 str_err_Int = {
    .header = HEADER_STRING(6), .words16 = {'a', 'n', ' ', 'I', 'N', 'T'}};
ElmString16 str_err_Float = {
    .header = HEADER_STRING(7), .words16 = {'a', ' ', 'F', 'L', 'O', 'A', 'T'}};
ElmString16 str_err_String = {
    .header = HEADER_STRING(8), .words16 = {'a', ' ', 'S', 'T', 'R', 'I', 'N', 'G'}};
ElmString16 str_err_Null = {.header = HEADER_STRING(4), .words16 = {'n', 'u', 'l', 'l'}};
ElmString16 str_err_List = {
    .header = HEADER_STRING(6), .words16 = {'a', ' ', 'L', 'I', 'S', 'T'}};
ElmString16 str_err_Array = {
    .header = HEADER_STRING(8), .words16 = {'a', 'n', ' ', 'A', 'R', 'R', 'A', 'Y'}};
ElmString16 str_err_Object = {
    .header = HEADER_STRING(9), .words16 = {'a', 'n', ' ', 'O', 'B', 'J', 'E', 'C', 'T'}};
ElmString16 str_err_backtick = {.header = HEADER_STRING(1), .words16 = {'`'}};
ElmString16 str_err_Field = {
    .header = HEADER_STRING(30),
    .words16 =
        {
            'a',
            'n',
            ' ',
            'O',
            'B',
            'J',
            'E',
            'C',
            'T',
            ' ',
            'w',
            'i',
            't',
            'h',
            ' ',
            'a',
            ' ',
            'f',
            'i',
            'e',
            'l',
            'd',
            ' ',
            'n',
            'a',
            'm',
            'e',
            'd',
            ' ',
            '`',
        },
};
ElmString16 str_err_Index = {
    .header = HEADER_STRING(27),
    .words16 =
        {
            'a',
            ' ',
            'L',
            'O',
            'N',
            'G',
            'E',
            'R',
            ' ',
            'a',
            'r',
            'r',
            'a',
            'y',
            '.',
            ' ',
            'N',
            'e',
            'e',
            'd',
            ' ',
            'i',
            'n',
            'd',
            'e',
            'x',
            ' ',
        },
};
ElmString16 str_err_Index_but_only_see = {
    .header = HEADER_STRING(14),
    .words16 = {' ', 'b', 'u', 't', ' ', 'o', 'n', 'l', 'y', ' ', 's', 'e', 'e', ' '},
};
ElmString16 str_err_Index_entries = {
    .header = HEADER_STRING(8),
    .words16 = {' ', 'e', 'n', 't', 'r', 'i', 'e', 's'},
};
ElmString16 str_invalid_json = {
    .header = HEADER_STRING(23),
    .words16 =
        {
            'T',
            'h',
            'i',
            's',
            ' ',
            'i',
            's',
            ' ',
            'n',
            'o',
            't',
            ' ',
            'v',
            'a',
            'l',
            'i',
            'd',
            ' ',
            'J',
            'S',
            'O',
            'N',
            '!',
        },
};

void* Json_expecting(ElmString16* type, void* value) {
  ElmString16* s = CAN_THROW(eval_String_append((void*[]){&str_err_expecting, type}));
  return TAIL_RESULT_ERR(A2(&g_elm_json_Json_Decode_Failure, s, WRAP(value)));
}

void* Json_runHelp(Custom* decoder, ElmValue* value);

void* eval_Json_array_get(void* args[]) {
  Custom* json_array = args[0];
  ElmInt* i = args[1];
  return json_array->values[i->value];
}

Custom* Json_runArrayDecoder(Custom* decoder, Custom* value, bool as_list) {
  u32 len = custom_params(value);
  Custom* array = NEW_CUSTOM(JSON_VALUE_ARRAY, len, NULL);
  for (u32 i = 0; i < len; i++) {
    Custom* result = CAN_THROW(Json_runHelp(decoder, value->values[i]));
    if (RESULT_IS_OK(result) == &False) {
      return TAIL_RESULT_ERR(
          A2(&g_elm_json_Json_Decode_Index, NEW_ELM_INT(i), result->values[0]));
    }
    array->values[i] = result->values[0];
  }

  void* elm_value = as_list
                        ? CAN_THROW(List_create(len, array->values))
                        : A2(&g_elm_core_Array_initialize,
                              NEW_ELM_INT(len),
                              NEW_CLOSURE(1, 2, eval_Json_array_get, (void*[]){array}));

  return TAIL_RESULT_OK(elm_value);
}

void* Json_runHelp(Custom* decoder, ElmValue* value) {
  switch (decoder->ctor) {
    case DECODER_BOOL:
      if (&value->custom == &True || &value->custom == &False) {
        return TAIL_RESULT_OK(value);
      }
      return Json_expecting(&str_err_Bool, value);

    case DECODER_INT:
      if (value->header.tag == Tag_Float) {
        f64 f = value->elm_float.value;
        if ((i32)0x80000000 <= f && f <= 0x7fffffff && floor(f) == f) {
          return TAIL_RESULT_OK(NEW_ELM_INT((i32)f));
        }
      }
      return Json_expecting(&str_err_Int, value);

    case DECODER_FLOAT:
      if (value->header.tag == Tag_Float) {
        return TAIL_RESULT_OK(value);
      }
      return Json_expecting(&str_err_Float, value);

    case DECODER_STRING:
      if (value->header.tag == Tag_String) {
        return TAIL_RESULT_OK(value);
      }
      return Json_expecting(&str_err_String, value);

    case DECODER_NULL:
      if (&value->custom == &Json_encodeNull) {
        return TAIL_RESULT_OK(decoder->values[JsonField_value]);
      }
      return Json_expecting(&str_err_Null, value);

    case DECODER_VALUE:
      return TAIL_RESULT_OK(WRAP(value));

    case DECODER_LIST:
      if (value->header.tag == Tag_JsRef) {
        value = (ElmValue*)getJsRefValue(value->js_ref.index);
      }
      if (value->header.tag == Tag_Custom && value->custom.ctor == JSON_VALUE_ARRAY) {
        return Json_runArrayDecoder(
            decoder->values[JsonField_decoder], &value->custom, true);
      }
      return Json_expecting(&str_err_List, value);

    case DECODER_ARRAY: {
      if (value->header.tag == Tag_JsRef) {
        value = (ElmValue*)getJsRefValue(value->js_ref.index);
      }
      if (value->header.tag == Tag_Custom && value->custom.ctor == JSON_VALUE_ARRAY) {
        return Json_runArrayDecoder(
            decoder->values[JsonField_decoder], &value->custom, false);
      }
      return Json_expecting(&str_err_Array, value);
    }

    case DECODER_FIELD: {
      ElmString16* field = decoder->values[JsonField_field];
      void* field_value = NULL;

      if (value->header.tag == Tag_Custom && value->custom.ctor == JSON_VALUE_OBJECT) {
        Custom* object = &value->custom;
        u32 n_params = custom_params(object);
        for (size_t i = 0; i < n_params; i += 2) {
          if (A2(&Utils_equal, object->values[i], field) == &True) {
            field_value = object->values[i + 1];
            break;
          }
        }
      } else if (value->header.tag == Tag_JsRef) {
        field_value = (void*)getJsRefObjectField(value->js_ref.index, (size_t)field);
      }

      if (field_value != NULL) {
        Custom* result = Json_runHelp(decoder->values[JsonField_decoder], field_value);
        return RESULT_IS_OK(result) == &True
                   ? result
                   : TAIL_RESULT_ERR(
                         A2(&g_elm_json_Json_Decode_Field, field, result->values[0]));
      } else {
        ElmString16* msg = A2(
            &String_append, &str_err_Field, A2(&String_append, field, &str_err_backtick));
        return Json_expecting(msg, value);
      }
    }

    case DECODER_INDEX: {
      ElmInt* index = decoder->values[JsonField_index];
      void* index_value = NULL;

      if (value->header.tag == Tag_Custom && value->custom.ctor == JSON_VALUE_ARRAY) {
        u32 len = custom_params(&value->custom);
        if (index->value >= len) {
          ElmString16* msg = A2(&String_append,
              &str_err_Index,
              A2(&String_append,
                  A1(&String_fromNumber, index),
                  A2(&String_append,
                      &str_err_Index_but_only_see,
                      A2(&String_append,
                          A1(&String_fromNumber, NEW_ELM_INT(len)),
                          &str_err_Index_entries))));
          return Json_expecting(msg, value);
        }
        index_value = value->custom.values[index->value];
      } else if (value->header.tag == Tag_JsRef) {
        index_value = (void*)getJsRefArrayIndex(value->js_ref.index, index->value);
      }

      if (index_value == NULL) {
        return Json_expecting(&str_err_Array, value);
      }

      Custom* result = Json_runHelp(decoder->values[JsonField_decoder], index_value);
      return RESULT_IS_OK(result) == &True
                 ? result
                 : TAIL_RESULT_ERR(
                       A2(&g_elm_json_Json_Decode_Index, index, result->values[0]));
    }

    case DECODER_KEY_VALUE: {
      if (value->header.tag == Tag_JsRef) {
        value = (ElmValue*)getJsRefValue(value->js_ref.index);
      }
      if (value->header.tag != Tag_Custom || value->custom.ctor != JSON_VALUE_OBJECT) {
        return Json_expecting(&str_err_Object, value);
      }
      Custom* object = &value->custom;
      u32 n_params = custom_params(object);
      Cons* keyValuePairs = &Nil;
      for (i32 i = n_params - 2; i >= 0; i -= 2) {
        ElmString16* key = object->values[i];
        Custom* result =
            Json_runHelp(decoder->values[JsonField_decoder], object->values[i + 1]);
        if (RESULT_IS_OK(result) == &False) {
          return TAIL_RESULT_ERR(
              A2(&g_elm_json_Json_Decode_Field, key, result->values[0]));
        }
        keyValuePairs = NEW_CONS(NEW_TUPLE2(key, result->values[0]), keyValuePairs);
      }
      return TAIL_RESULT_OK(keyValuePairs);
    }

    case DECODER_MAP: {
      Closure* answer = decoder->values[JsonField_func];
      Custom* decoders = decoder->values[JsonField_decoders];
      u32 n_decoders = custom_params(decoders);
      for (size_t i = 0; i < n_decoders; i++) {
        Custom* result = Json_runHelp(decoders->values[i], value);
        if (RESULT_IS_OK(result) == &False) {
          return result;
        }
        answer = A1(answer, result->values[0]);
      }
      return TAIL_RESULT_OK(answer);
    }

    case DECODER_AND_THEN: {
      Custom* result = Json_runHelp(decoder->values[JsonField_decoder], value);
      return RESULT_IS_OK(result) == &False
                 ? result
                 : Json_runHelp(
                       A1(decoder->values[JsonField_callback], result->values[0]), value);
    }

    case DECODER_ONE_OF: {
      Cons* errors = &Nil;
      Cons* temp = decoder->values[JsonField_decoders];
      for (; temp != &Nil; temp = temp->tail) {
        Custom* result = Json_runHelp(temp->head, value);
        if (RESULT_IS_OK(result) == &True) {
          return result;
        }
        errors = NEW_CONS(result->values[0], errors);
      }
      return TAIL_RESULT_ERR(
          A1(&g_elm_json_Json_Decode_OneOf, A1(&g_elm_core_List_reverse, errors)));
    }

    case DECODER_FAIL: {
      return TAIL_RESULT_ERR(A2(
          &g_elm_json_Json_Decode_Failure, decoder->values[JsonField_msg], WRAP(value)));
    }

    case DECODER_SUCCEED: {
      return TAIL_RESULT_OK(decoder->values[JsonField_msg]);
    }

    default:
      assert(0);
  }
  return NULL;
}

static void* eval_runOnString(void* args[]) {
  Custom* decoder = args[0];
  ElmString16* string = args[1];

  ElmValue* json = CAN_THROW(parse_json(string));
  if (json == NULL) {
    return TAIL_RESULT_ERR(
        A2(&g_elm_json_Json_Decode_Failure, &str_invalid_json, WRAP(string)));
  }
  return Json_runHelp(decoder, json);
}
Closure Json_runOnString = {
    .header = HEADER_CLOSURE(0),
    .max_values = 2,
    .evaluator = &eval_runOnString,
};

static void* eval_Json_run(void* args[]) {
  Custom* decoder = args[0];
  Custom* wrapped = args[1];
  void* unwrapped = wrapped->values[0];
  return Json_runHelp(decoder, unwrapped);
}
Closure Json_run = {
    .header = HEADER_CLOSURE(0),
    .max_values = 2,
    .evaluator = &eval_Json_run,
};

size_t EMSCRIPTEN_KEEPALIVE get_eval_Json_run() {
  void* ptr = &eval_Json_run;
  return (size_t)ptr;
}
