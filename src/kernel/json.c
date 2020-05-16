#include "./json.h"

#include <stdio.h>

#include "./wrapper/wrapper.h"
#include "./string.h"

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
  return NEW_CUSTOM(DECODER_LIST, 2, ((void*[]){&Unit, decoder}));
}
Closure Json_decodeList = {
    .header = HEADER_CLOSURE(0),
    .max_values = 1,
    .evaluator = &eval_Json_decodeList,
};

static void* eval_Json_decodeArray(void* args[]) {
  void* decoder = args[0];
  return NEW_CUSTOM(DECODER_ARRAY, 2, ((void*[]){&Unit, decoder}));
}
Closure Json_decodeArray = {
    .header = HEADER_CLOSURE(0),
    .max_values = 1,
    .evaluator = &eval_Json_decodeArray,
};

static void* eval_Json_decodeNull(void* args[]) {
  void* value = args[0];
  return NEW_CUSTOM(DECODER_NULL, 3, ((void*[]){&Unit, &Unit, value}));
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
          /*a*/ &Unit,
          /*b*/ decoder,
          /*c*/ &Unit,
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
          /*a*/ &Unit,
          /*b*/ decoder,
          /*c*/ &Unit,
          /*d*/ &Unit,
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
  return NEW_CUSTOM(DECODER_KEY_VALUE, 2, ((void*[]){&Unit, decoder}));
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
          /*a*/ &Unit,
          /*b*/ decoder,
          /*c*/ &Unit,
          /*d*/ &Unit,
          /*e*/ &Unit,
          /*f*/ &Unit,
          /*g*/ &Unit,
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
          /*a*/ &Unit,
          /*b*/ &Unit,
          /*c*/ &Unit,
          /*d*/ &Unit,
          /*e*/ &Unit,
          /*f*/ &Unit,
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
          /*a*/ &Unit,
          /*b*/ &Unit,
          /*c*/ &Unit,
          /*d*/ &Unit,
          /*e*/ &Unit,
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

/*
parse JSON values from a UTF-16 string

https://ecma-international.org/publications/files/ECMA-ST/ECMA-404.pdf

- boolean
  - if first letter is t
    - loop through the next 3 checking that they're "rue"
  - if first letter is f
    - loop through the next 4 checking that they're "alse"
- int
  - swscanf from wchar.h
- float
  - swscanf from wchar.h
- string
  - double quote
    - escaped 8 things  \" \\ \/ \b \f \n \r \t
    - escaped unicode
      - \u002F (four digits, lowercase u, upper or lower a-f)
      - Can have two in a row, already encoded as UTF-16 => easy
      - unicode doesn't have to be escaped, it just can be
    - no ASCII control chars allowed
- null
  - check for 4-char sequence
- value
  - Use a special Custom to wrap a string
  - Special-case it in the wrapper. Read it through JSON.stringify()
  - Json_run recognises the wrapped string and delegates to Json_runOnString

- list
  - parse an array and create a Cons each time
- array
  - parse an array and call g_elm_core_Array_insert each time
- field
  - run through the field string, matching all the chars
  - grab the string and pass it to the decoder
- index
  - skip through an array and then recurse
- key value
  - Make a list of tuples (String, <decoded thing>)
- map
- and then
- one of
- fail
- succeed

*/

void* runStringHelp(Custom* decoder, u16** cursor, u16* end) {
  // void* result;

  // skip_whitespace(cursor, end);

  // switch (decoder->ctor) {
  //   case DECODER_SUCCEED:
  //     break;
  //   case DECODER_FAIL:
  //     break;
  //   case DECODER_INT:
  //     result = parse_int(cursor, end);
  //     break;
  //   case DECODER_BOOL:
  //     result = parse_bool(cursor, end);
  //     break;
  //   case DECODER_FLOAT:
  //     result = parse_number(cursor, end);
  //     break;
  //   case DECODER_VALUE:
  //     // data structure representing the actual JSON
  //     // ADT with 0=JSref, 1=prim, 2=array, 3=object
  //     // wrapper should know about this at some point
  //     // need an array parser and an object parser
  //     break;
  //   case DECODER_STRING:
  //     result = parse_string(cursor, end);
  //     break;
  //   case DECODER_LIST:
  //     // run the array parser & convert the Custom to a List
  //     // use List_create!
  //     break;
  //   case DECODER_ARRAY:
  //     // run the array parser & convert the Custom to an Elm Array
  //     break;
  //   case DECODER_NULL:
  //     result = parse_null(cursor, end);
  //     break;
  //   case DECODER_FIELD:
  //     // fields might not be in order!!
  //     // pre-parsing would give faster access to each field
  //     result = parse_string(cursor, end);
  //     break;
  //   case DECODER_INDEX:
  //     // skip bits of the array
  //     break;
  //   case DECODER_KEY_VALUE:
  //     break;
  //   case DECODER_MAP:
  //     break;
  //   case DECODER_AND_THEN:
  //     break;
  //   case DECODER_ONE_OF:
  //     break;
  //   default:
  //     break;
  // }
  return NULL;
}

static void* eval_runOnString(void* args[]) {
  Custom* decoder = args[0];
  ElmString16* string = args[1];

  u16* cursor = string->words16;
  u16* end = string->words16 + code_units(string);

  return runStringHelp(decoder, &cursor, end);
}
Closure Json_runOnString = {
    .header = HEADER_CLOSURE(0),
    .max_values = 2,
    .evaluator = &eval_runOnString,
};

static void* eval_run(void* args[]) {
  size_t jsIndex = (size_t)args[0];
  return Wrapper_callJsSync(jsIndex, 2, &args[1]);
}
Closure Json_run = {
    .header = HEADER_CLOSURE(0),
    .n_values = 1,
    .max_values = 3,
    .evaluator = &eval_run,
    .values = {NULL},
};

/*

const jsRefStore = [];

class JsRef() {
  constructor(jsValue) {
    this.$ = KERNEL_CTOR_OFFSET;
    this.a = jsValue;
    this.index = jsRefStore.push(this) - 1;
  }
}


var _Json_encode = F2(function(indentLevel, value)

function _Json_wrap(value) {
  const jsRef = new JsRef(value);
  return jsRef;
}
function _Json_unwrap(value) {
  if (!(value instanceof JsRef)) {
    console.warn('unwrapping non-JsRef', value);
  }
  return value.a;
}

function _Json_emptyArray() { return []; }
function _Json_emptyObject() { return {}; }

var _Json_addField = F3(function(key, value, object)

function _Json_addEntry(func)
var _Json_encodeNull = _Json_wrap(null);
*/