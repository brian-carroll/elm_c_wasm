#include "./types.h"
#include "./wrapper/wrapper.h"

Closure Json_addEntry = {
    .header = HEADER_CLOSURE(0),
    .max_values = NEVER_EVALUATE,
    .evaluator = (void*)JS_Json_addEntry,
};
Closure Json_addField = {
    .header = HEADER_CLOSURE(0),
    .max_values = NEVER_EVALUATE,
    .evaluator = (void*)JS_Json_addField,
};
Closure Json_andThen = {
    .header = HEADER_CLOSURE(0),
    .max_values = NEVER_EVALUATE,
    .evaluator = (void*)JS_Json_andThen,
};
Closure Json_decodeArray = {
    .header = HEADER_CLOSURE(0),
    .max_values = NEVER_EVALUATE,
    .evaluator = (void*)JS_Json_decodeArray,
};
Closure Json_decodeBool = {
    .header = HEADER_CLOSURE(0),
    .max_values = NEVER_EVALUATE,
    .evaluator = (void*)JS_Json_decodeBool,
};
Closure Json_decodeField = {
    .header = HEADER_CLOSURE(0),
    .max_values = NEVER_EVALUATE,
    .evaluator = (void*)JS_Json_decodeField,
};
Closure Json_decodeFloat = {
    .header = HEADER_CLOSURE(0),
    .max_values = NEVER_EVALUATE,
    .evaluator = (void*)JS_Json_decodeFloat,
};
Closure Json_decodeIndex = {
    .header = HEADER_CLOSURE(0),
    .max_values = NEVER_EVALUATE,
    .evaluator = (void*)JS_Json_decodeIndex,
};
Closure Json_decodeInt = {
    .header = HEADER_CLOSURE(0),
    .max_values = NEVER_EVALUATE,
    .evaluator = (void*)JS_Json_decodeInt,
};
Closure Json_decodeKeyValuePairs = {
    .header = HEADER_CLOSURE(0),
    .max_values = NEVER_EVALUATE,
    .evaluator = (void*)JS_Json_decodeKeyValuePairs,
};
Closure Json_decodeList = {
    .header = HEADER_CLOSURE(0),
    .max_values = NEVER_EVALUATE,
    .evaluator = (void*)JS_Json_decodeList,
};
Closure Json_decodeNull = {
    .header = HEADER_CLOSURE(0),
    .max_values = NEVER_EVALUATE,
    .evaluator = (void*)JS_Json_decodeNull,
};
Closure Json_decodeString = {
    .header = HEADER_CLOSURE(0),
    .max_values = NEVER_EVALUATE,
    .evaluator = (void*)JS_Json_decodeString,
};
Closure Json_decodeValue = {
    .header = HEADER_CLOSURE(0),
    .max_values = NEVER_EVALUATE,
    .evaluator = (void*)JS_Json_decodeValue,
};
Closure Json_emptyArray = {
    .header = HEADER_CLOSURE(0),
    .max_values = NEVER_EVALUATE,
    .evaluator = (void*)JS_Json_emptyArray,
};
Closure Json_emptyObject = {
    .header = HEADER_CLOSURE(0),
    .max_values = NEVER_EVALUATE,
    .evaluator = (void*)JS_Json_emptyObject,
};
Closure Json_map1 = {
    .header = HEADER_CLOSURE(0),
    .max_values = NEVER_EVALUATE,
    .evaluator = (void*)JS_Json_map1,
};
Closure Json_map2 = {
    .header = HEADER_CLOSURE(0),
    .max_values = NEVER_EVALUATE,
    .evaluator = (void*)JS_Json_map2,
};
Closure Json_map3 = {
    .header = HEADER_CLOSURE(0),
    .max_values = NEVER_EVALUATE,
    .evaluator = (void*)JS_Json_map3,
};
Closure Json_map4 = {
    .header = HEADER_CLOSURE(0),
    .max_values = NEVER_EVALUATE,
    .evaluator = (void*)JS_Json_map4,
};
Closure Json_map5 = {
    .header = HEADER_CLOSURE(0),
    .max_values = NEVER_EVALUATE,
    .evaluator = (void*)JS_Json_map5,
};
Closure Json_map6 = {
    .header = HEADER_CLOSURE(0),
    .max_values = NEVER_EVALUATE,
    .evaluator = (void*)JS_Json_map6,
};
Closure Json_map7 = {
    .header = HEADER_CLOSURE(0),
    .max_values = NEVER_EVALUATE,
    .evaluator = (void*)JS_Json_map7,
};
Closure Json_map8 = {
    .header = HEADER_CLOSURE(0),
    .max_values = NEVER_EVALUATE,
    .evaluator = (void*)JS_Json_map8,
};
Closure Json_oneOf = {
    .header = HEADER_CLOSURE(0),
    .max_values = NEVER_EVALUATE,
    .evaluator = (void*)JS_Json_oneOf,
};
Closure Json_wrap = {
    .header = HEADER_CLOSURE(0),
    .max_values = NEVER_EVALUATE,
    .evaluator = (void*)JS_Json_wrap,
};

Closure Json_succeed = {
    .header = HEADER_CLOSURE(0),
    .max_values = NEVER_EVALUATE,
    .evaluator = (void*)JS_Json_succeed,
};
Closure Json_fail = {
    .header = HEADER_CLOSURE(0),
    .max_values = NEVER_EVALUATE,
    .evaluator = (void*)JS_Json_fail,
};

/**
 * ENCODING
 */
Closure Json_encode = {
    .header = HEADER_CLOSURE(0),
    .max_values = NEVER_EVALUATE,
    .evaluator = (void*)JS_Json_encode,
};
Closure Json_encodeNull = {
    .header = HEADER_CLOSURE(0),
    .max_values = NEVER_EVALUATE,
    .evaluator = (void*)JS_Json_encodeNull,
};

/**
 * RUN DECODER
 */
static void* eval_run(void* args[]) {
  Closure* jsThunk = NEW_CLOSURE(2, NEVER_EVALUATE, (void*)JS_Json_run, args);
  size_t resultAddr = Wrapper_evaluateInJs((size_t)jsThunk);
  return (void*)resultAddr;
}
Closure Json_run = {
    .header = HEADER_CLOSURE(0),
    .max_values = 2,
    .evaluator = eval_run,
};

static void* eval_runOnString(void* args[]) {
  Closure* jsThunk = NEW_CLOSURE(2, NEVER_EVALUATE, (void*)JS_Json_runOnString, args);
  size_t resultAddr = Wrapper_evaluateInJs((size_t)jsThunk);
  return (void*)resultAddr;
}
Closure Json_runOnString = {
    .header = HEADER_CLOSURE(0),
    .max_values = 2,
    .evaluator = eval_runOnString,
};
