#include "internals.h"

/* ====================================================

                ELM VALUE HEADERS

    Most of the header-related code is in core/types.h
   ==================================================== */

size_t child_count(ElmValue* v) {
  switch (v->header.tag) {
    case Tag_Int:
    case Tag_Float:
    case Tag_Char:
    case Tag_String:
      return 0;

    case Tag_List:
      return v == pNil ? 0 : 2;

    case Tag_Tuple2:
      return 2;

    case Tag_Tuple3:
      return 3;

    case Tag_Custom:
      return v->header.size - (sizeof(Custom) / SIZE_UNIT);

    case Tag_Record:
      return v->header.size - (sizeof(Record) / SIZE_UNIT);

    case Tag_FieldGroup:
      return 0;

    case Tag_Closure:
      return v->closure.n_values;

    case Tag_JsRef:
    case Tag_GcStackEmpty:
      return 0;

    case Tag_GcStackPush:
      return 1;

    case Tag_GcStackPop:
    case Tag_GcStackTailCall:
      return 2;
  }
}