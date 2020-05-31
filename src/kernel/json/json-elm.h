// Declarations of values defined in Elm code
// that are referenced from C code
#ifndef JSON_ELM_H
#define JSON_ELM_H

#include "../core/types.h"

Closure g_elm_json_Json_errorToString;
Closure g_elm_json_Json_Decode_Failure;
Closure g_elm_json_Json_Decode_Field;
Closure g_elm_json_Json_Decode_Index;
Closure g_elm_json_Json_Decode_OneOf;

#define g_elm_json_Json_Encode_string Json_wrap
#define g_elm_json_Json_Encode_int Json_wrap
#define g_elm_json_Json_Encode_float Json_wrap
#define g_elm_json_Json_Encode_bool Json_wrap

#endif
