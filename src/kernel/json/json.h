#ifndef ELM_JSON_H
#define ELM_JSON_H

#include "../core/types.h"

// Pick offsets so we can easily read lower digits in either decimal or hex
#define JSON_CTOR_OFFSET 0x2000 * 10000

enum ctor_decoder {
  DECODER_SUCCEED = KERNEL_CTOR_OFFSET,
  DECODER_FAIL,
  DECODER_INT,
  DECODER_BOOL,
  DECODER_FLOAT,
  DECODER_VALUE,
  DECODER_STRING,
  DECODER_LIST,
  DECODER_ARRAY,
  DECODER_NULL,
  DECODER_FIELD,
  DECODER_INDEX,
  DECODER_KEY_VALUE,
  DECODER_MAP,
  DECODER_AND_THEN,
  DECODER_ONE_OF,
};

enum ctor_value {
  JSON_VALUE_WRAP = JSON_CTOR_OFFSET,
  JSON_VALUE_NULL,
  JSON_VALUE_OBJECT,
  JSON_VALUE_ARRAY,
};

// Decode

extern Closure Json_succeed;
extern Closure Json_fail;

extern Custom Json_decodeInt;
extern Custom Json_decodeBool;
extern Custom Json_decodeFloat;
extern Custom Json_decodeValue;
extern Custom Json_decodeString;

extern Closure Json_decodeList;
extern Closure Json_decodeArray;
extern Closure Json_decodeNull;
extern Closure Json_decodeField;
extern Closure Json_decodeIndex;
extern Closure Json_decodeKeyValuePairs;
extern Closure Json_andThen;
extern Closure Json_oneOf;
extern Closure Json_map1;
extern Closure Json_map2;
extern Closure Json_map3;
extern Closure Json_map4;
extern Closure Json_map5;
extern Closure Json_map6;
extern Closure Json_map7;
extern Closure Json_map8;

extern Closure Json_run;
extern Closure Json_runOnString;
void* Json_runHelp(Custom* decoder, ElmValue* value);

// Encode

extern Custom Json_null; // represents literal JS `null` (unwrapped)
extern Custom Json_encodeNull; // Json-wrapped
extern Closure Json_wrap;
extern Closure Json_unwrap;
extern Closure Json_emptyArray;
extern Closure Json_emptyObject;
extern Closure Json_addField;
extern Closure Json_addEntry;
extern Closure Json_encode;

#endif
