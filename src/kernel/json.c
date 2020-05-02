#include "./types.h"
#include "./wrapper/wrapper.h"

#define CTOR_KERNEL_ARRAY 0xffffffff
#define KERNEL_CTOR_OFFSET 1024 * 1000

enum {
  SUCCEED = KERNEL_CTOR_OFFSET,
  FAIL,
  INT,
  BOOL,
  FLOAT,
  VALUE,
  STRING,
  LIST,
  ARRAY,
  NULL_,
  FIELD,
  INDEX,
  KEY_VALUE,
  MAP,
  AND_THEN,
  ONE_OF,
};

static void* eval_Json_succeed(void* args[]) {
  return NEW_CUSTOM(SUCCEED, 1, args);
}
Closure Json_succeed = {
    .header = HEADER_CLOSURE(0),
    .max_values = 1,
    .evaluator = &eval_Json_succeed,
};

static void* eval_Json_fail(void* args[]) {
  return NEW_CUSTOM(FAIL, 1, args);
}
Closure Json_fail = {
    .header = HEADER_CLOSURE(0),
    .max_values = 1,
    .evaluator = &eval_Json_fail,
};

Custom Json_decodeInt = {
    .header = HEADER_CUSTOM(0),
    .ctor = INT,
};
Custom Json_decodeBool = {
    .header = HEADER_CUSTOM(0),
    .ctor = BOOL,
};
Custom Json_decodeFloat = {
    .header = HEADER_CUSTOM(0),
    .ctor = FLOAT,
};
Custom Json_decodeValue = {
    .header = HEADER_CUSTOM(0),
    .ctor = VALUE,
};
Custom Json_decodeString = {
    .header = HEADER_CUSTOM(0),
    .ctor = STRING,
};

static void* eval_Json_decodeList(void* args[]) {
  return NEW_CUSTOM(LIST, 1, args);
}
Closure Json_decodeList = {
    .header = HEADER_CLOSURE(0),
    .max_values = 1,
    .evaluator = &eval_Json_decodeList,
};

static void* eval_Json_decodeArray(void* args[]) {
  return NEW_CUSTOM(ARRAY, 1, args);
}
Closure Json_decodeArray = {
    .header = HEADER_CLOSURE(0),
    .max_values = 1,
    .evaluator = &eval_Json_decodeArray,
};

static void* eval_Json_decodeNull(void* args[]) {
  return NEW_CUSTOM(NULL_, 1, args);
}
Closure Json_decodeNull = {
    .header = HEADER_CLOSURE(0),
    .max_values = 1,
    .evaluator = &eval_Json_decodeNull,
};

static void* eval_Json_decodeField(void* args[]) {
  return NEW_CUSTOM(FIELD, 2, args);
}
Closure Json_decodeField = {
    .header = HEADER_CLOSURE(0),
    .max_values = 2,
    .evaluator = &eval_Json_decodeField,
};

static void* eval_Json_decodeIndex(void* args[]) {
  return NEW_CUSTOM(INDEX, 2, args);
}
Closure Json_decodeIndex = {
    .header = HEADER_CLOSURE(0),
    .max_values = 2,
    .evaluator = &eval_Json_decodeIndex,
};

static void* eval_Json_decodeKeyValuePairs(void* args[]) {
  return NEW_CUSTOM(KEY_VALUE, 1, args);
}
Closure Json_decodeKeyValuePairs = {
    .header = HEADER_CLOSURE(0),
    .max_values = 1,
    .evaluator = &eval_Json_decodeKeyValuePairs,
};

static void* eval_Json_andThen(void* args[]) {
  return NEW_CUSTOM(AND_THEN, 2, args);
}
Closure Json_andThen = {
    .header = HEADER_CLOSURE(0),
    .max_values = 2,
    .evaluator = &eval_Json_andThen,
};

static void* eval_Json_oneOf(void* args[]) {
  return NEW_CUSTOM(ONE_OF, 1, args);
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
  u32 n_decoders = (u32)args[0];
  Closure* f = args[1];
  Custom* decoders = NEW_CUSTOM(CTOR_KERNEL_ARRAY, n_decoders, &args[2]);
  return NEW_CUSTOM(MAP, 2, ((void*[]){f, decoders}));
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
  - parse an array and create a red-black thingummy each time
- field
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

static void* eval_runOnString(void* args[]) {
  size_t jsIndex = (size_t)args[0];
  return Wrapper_callJsSync(jsIndex, 2, &args[1]);
}
Closure Json_runOnString = {
    .header = HEADER_CLOSURE(0),
    .n_values = 1,
    .max_values = 3,
    .evaluator = &eval_runOnString,
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