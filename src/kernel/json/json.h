#ifndef ELM_JSON_H
#define ELM_JSON_H

#include "../core/types.h"

// Pick offsets so we can easily read lower digits in either decimal or hex
#define JSON_CTOR_OFFSET 0x2000 * 10000
#define KERNEL_CTOR_OFFSET 0x400 * 1000

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
Custom Json_Value_null;

// Decode

Closure Json_succeed;
Closure Json_fail;

Custom Json_decodeInt;
Custom Json_decodeBool;
Custom Json_decodeFloat;
Custom Json_decodeValue;
Custom Json_decodeString;

Closure Json_decodeList;
Closure Json_decodeArray;
Closure Json_decodeNull;
Closure Json_decodeField;
Closure Json_decodeIndex;
Closure Json_decodeKeyValuePairs;
Closure Json_andThen;
Closure Json_oneOf;
Closure Json_map1;
Closure Json_map2;
Closure Json_map3;
Closure Json_map4;
Closure Json_map5;
Closure Json_map6;
Closure Json_map7;
Closure Json_map8;

Closure Json_run;
Closure Json_runOnString;

// Encode

Closure Json_wrap;
Closure Json_unwrap;
Closure Json_emptyArray;
Closure Json_emptyObject;
Closure Json_addField;
Closure Json_addEntry;
#define Json_encodeNull Json_Value_null
Closure Json_encode;

#endif
