Richard Feldman's SPA example, compiled to WebAssembly

Serves as a vehicle to develop and debug the code generator

**Not working yet**

- Missing functions in core libraries

  - A5
  - CTOR_Field
  - CTOR_Index
  - CTOR_OneOf
  - String_all
  - String_length
  - String_slice
  - String_toInt
  - String_uncons
  - elm_core_Basics_negate
  - elm_core_List_foldrHelper
  - elm_json_Json_Decode_errorToString
  - elm_json_Json_Decode_errorToStringHelp

- Code gen bugs

  - `args` array is not defined

    - eval_elm_core_List_foldl
    - eval_elm_core_List_rangeHelp
    - eval_elm_json_Json_Decode_errorToStringHelp
    - eval_elm_parser_Parser_Advanced_chompWhileHelp
