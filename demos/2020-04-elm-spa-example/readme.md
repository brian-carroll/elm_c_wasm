## Richard Feldman's SPA example, compiled to WebAssembly

Serves as a vehicle to develop and debug the code generator, and motivation to expand the set of C kernel functions.

Richard's repo is a Git submodule of this one, so I can use the original source code. I just change the build system and the index.html file.

### Issues currently causing C compiler errors

#### Elm functions not generated

- elm_core_Basics_negate
- elm_core_List_foldrHelper
- elm_json_Json_Decode_errorToString
- elm_json_Json_Decode_errorToStringHelp

#### Constructor IDs not generated

All of these are used in elm_json_Json_Decode_errorToStringHelp. Maybe if that bug is fixed, this one will go away too.

- CTOR_Field
- CTOR_Index
- CTOR_OneOf

#### `args` array not defined for some recursive functions

- eval_elm_core_List_foldl
- eval_elm_core_List_rangeHelp
- eval_elm_json_Json_Decode_errorToStringHelp
- eval_elm_parser_Parser_Advanced_chompWhileHelp
