#include "../types.h"
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
      // Include FieldGroup since it could be heap-allocated
      // Http module has no literals for some Record types so compiler emits no FieldGroups
      return v->header.size - (sizeof(Record) / SIZE_UNIT) + 1;

    case Tag_FieldGroup:
      return 0;

    case Tag_Closure:
      return v->closure.n_values;

    case Tag_Process:
      return (sizeof(Process) - sizeof(Custom)) / SIZE_UNIT;

    case Tag_Task:
      return (sizeof(Task) - sizeof(Custom)) / SIZE_UNIT;

    case Tag_JsRef:
    default:
      return 0;
  }
}


#define SANITY_MAX_CHILDREN 512

bool sanity_check(void* p) {
  if (!p) return true;
  Header h = *(Header*)p;
  switch (h.tag) {
    case Tag_Int:
      return (h.size == SIZE_INT);
    case Tag_Float:
      return (h.size == SIZE_FLOAT);
    case Tag_Char:
      return (h.size == SIZE_CHAR);
    case Tag_String:
      return (h.size < (GC_INITIAL_HEAP_MB * 1024 * 1024 / 16)/SIZE_UNIT);
    case Tag_List:
      return (h.size == SIZE_LIST);
    case Tag_Tuple2:
      return (h.size == SIZE_TUPLE2);
    case Tag_Tuple3:
      return (h.size == SIZE_TUPLE3);
    case Tag_Custom:
      return (h.size >= SIZE_CUSTOM(0)) && (h.size < SIZE_CUSTOM(SANITY_MAX_CHILDREN));
    case Tag_Record:
      return (h.size >= SIZE_RECORD(0)) && (h.size < SIZE_RECORD(SANITY_MAX_CHILDREN));
    case Tag_FieldGroup:
      return (h.size >= SIZE_FIELDGROUP(0)) && (h.size < SIZE_FIELDGROUP(SANITY_MAX_CHILDREN));
    case Tag_Closure:
      return (h.size >= SIZE_CLOSURE(0)) && (h.size < SIZE_CLOSURE(SANITY_MAX_CHILDREN));
    case Tag_JsRef:
      return (h.size == SIZE_JS_REF);
    case Tag_Process:
      return (h.size == (sizeof(Process) / SIZE_UNIT));
    case Tag_Task:
      return (h.size == (sizeof(Task) / SIZE_UNIT));
    default:
      return false;
  }
}