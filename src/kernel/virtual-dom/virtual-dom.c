#include "../core/core.h"
#include "../json/json.h"
#include "./virtual-dom-elm.h"

// predeclarations
static Custom* organizeFacts(Cons* factList);

enum {
  VDOM_TEXT,
  VDOM_NODE,
  VDOM_KEYED_NODE,
  VDOM_CUSTOM,
  VDOM_TAGGER,
  VDOM_THUNK,
};


#define NUM_FACT_CTORS 5
enum {
  FACT_EVENT,
  FACT_STYLE,
  FACT_PROP,
  FACT_ATTR,
  FACT_ATTR_NS,
};

enum {
  PATCH_REDRAW,
  PATCH_FACTS,
  PATCH_TEXT,
  PATCH_THUNK,
  PATCH_TAGGER,
  PATCH_REMOVE_LAST,
  PATCH_APPEND,
  PATCH_REMOVE,
  PATCH_REORDER,
  PATCH_CUSTOM,
};


// Make all VirtualDom structs look like Elm Custom type to the GC
// We don't need all 32 bits for ctor, and we have an integer field,
// so steal some ctor bits and unbox the integer!
// Keeping things small fits more nodes in cache, and we want to go fast.
#define VDOM_BASE_FIELDS \
  Header header; \
  u8 ctor : 3; \
  u32 descendantsCount : 29;   // < 536,870,912 descendants



// TEXT


typedef struct {
  VDOM_BASE_FIELDS
  ElmString16* text;
} VdomText;

static void* eval_VirtualDom_text(void* args[]) {
  ElmString16* text = args[0];
  VdomText* p = CAN_THROW(GC_malloc(sizeof(VdomText)));
  *p = (VdomText){
    .header = HEADER_CUSTOM(1),
    .ctor = VDOM_TEXT,
    .descendantsCount = 0,
    .text = text,
  };
  return p;
}

Closure VirtualDom_text = {
    .header = HEADER_CLOSURE(0),
    .max_values = 1,
    .evaluator = &eval_VirtualDom_text,
};


// NODE

typedef struct {
  VDOM_BASE_FIELDS
  ElmString16* tag;
  Custom* facts; // object
  Custom* kids;  // array
  ElmString16* namespace;
} VdomNode;

typedef VdomNode VdomKeyedNode;

static void* eval_VirtualDom_nodeNS(void* args[]) {
  ElmString16* namespace = args[0];
  ElmString16* tag = args[1];
  Cons* factList = args[2];
  Cons* kidList = args[3];

  u32 descendantsCount = 0;
  Custom* kidArray = CAN_THROW(GC_malloc(sizeof(Custom)));
  for (; kidList != &Nil; kidList = kidList->tail) {
    CAN_THROW(GC_malloc(sizeof(void*)));
    kidArray->header.size++;
    VdomNode* kid = kidList->head;
    kidArray->values[descendantsCount] = kid;
    descendantsCount += kid->descendantsCount + 1;
  }

  Custom* facts = CAN_THROW(organizeFacts(factList));

  VdomNode* p = CAN_THROW(GC_malloc(sizeof(VdomNode)));
  *p = (VdomNode){
    .header = HEADER_CUSTOM(4),
    .ctor = VDOM_NODE,
    .descendantsCount = descendantsCount,
    .namespace = namespace,
    .tag = tag,
    .facts = facts,
    .kids = kidArray,
  };
  return p;
}

Closure VirtualDom_nodeNS = {
    .header = HEADER_CLOSURE(4),
    .max_values = 4,
    .evaluator = &eval_VirtualDom_nodeNS,
};

Closure VirtualDom_node = {
    .header = HEADER_CLOSURE(4),
    .n_values = 1,
    .max_values = 3,
    .evaluator = &eval_VirtualDom_nodeNS,
    .values = {NULL},
};


// KEYED NODE

static void* eval_VirtualDom_keyedNodeNS(void* args[]) {
  ElmString16* namespace = args[0];
  ElmString16* tag = args[1];
  Cons* factList = args[2];
  Cons* kidList = args[3];

  u32 descendantsCount = 0;
  Custom* kidArray = CAN_THROW(GC_malloc(sizeof(Custom)));
  for (; kidList != &Nil; kidList = kidList->tail) {
    CAN_THROW(GC_malloc(sizeof(void*)));
    kidArray->header.size++;
    VdomNode* kid = kidList->head;
    kidArray->values[descendantsCount] = kid;
    descendantsCount += kid->descendantsCount + 1;
  }

  Custom* facts = CAN_THROW(organizeFacts(factList));

  VdomNode* p = CAN_THROW(GC_malloc(sizeof(VdomNode)));
  *p = (VdomNode){
    .header = HEADER_CUSTOM(4),
    .ctor = VDOM_KEYED_NODE,
    .descendantsCount = descendantsCount,
    .namespace = namespace,
    .tag = tag,
    .facts = facts,
    .kids = kidArray,
  };
  return p;
}

Closure VirtualDom_keyedNodeNS = {
    .header = HEADER_CLOSURE(4),
    .max_values = 4,
    .evaluator = &eval_VirtualDom_keyedNodeNS,
};

Closure VirtualDom_keyedNode = {
    .header = HEADER_CLOSURE(4),
    .n_values = 1,
    .max_values = 3,
    .evaluator = &eval_VirtualDom_keyedNodeNS,
    .values = {NULL},
};

// CUSTOM
// doesn't seem to actually be used anywhere in elm org packages

typedef struct {
  VDOM_BASE_FIELDS
  Cons* facts;
  void* model;
  void* render;
  void* diff;
} VdomCustom;



// MAP

typedef struct {
  VDOM_BASE_FIELDS
  Closure* tagger;
  VdomNode* node;
} VdomTagger;

static void* eval_VirtualDom_map(void* args[]) {
  Closure* tagger = args[0];
  VdomNode* node = args[1];
  VdomTagger* p = CAN_THROW(GC_malloc(sizeof(VdomTagger)));
  *p = (VdomTagger){
    .header = HEADER_CUSTOM(2),
    .ctor = VDOM_TAGGER,
    .descendantsCount = 1 + node->descendantsCount,
    .tagger = tagger,
    .node = node,
  };
  return p;
}

Closure VirtualDom_map = {
    .header = HEADER_CLOSURE(0),
    .max_values = 2,
    .evaluator = &eval_VirtualDom_map,
};


// LAZY

typedef struct {
  VDOM_BASE_FIELDS
  VdomNode* node;
  void* refs[];
} VdomThunk;

static void* eval_VirtualDom_thunk(void* args[]) {
  Closure* func = args[0];
  size_t n_refs = 1 + func->max_values - func->n_values;

  VdomThunk* p = CAN_THROW(GC_malloc(sizeof(VdomThunk) + n_refs * sizeof(void*)));
  p->header = HEADER_CUSTOM(1 + n_refs);
  p->ctor = VDOM_THUNK;
  p->node = NULL;
  for (size_t i = 0; i < n_refs; ++i) {
    p->refs[i] = args[i];
  }
  return p;
}

Closure VirtualDom_lazy = {
    .evaluator = &eval_VirtualDom_thunk,
    .header = HEADER_CLOSURE(0),
    .max_values = 2,
};

Closure VirtualDom_lazy2 = {
    .evaluator = &eval_VirtualDom_thunk,
    .header = HEADER_CLOSURE(0),
    .max_values = 3,
};

Closure VirtualDom_lazy3 = {
    .evaluator = &eval_VirtualDom_thunk,
    .header = HEADER_CLOSURE(0),
    .max_values = 4,
};

Closure VirtualDom_lazy4 = {
    .evaluator = &eval_VirtualDom_thunk,
    .header = HEADER_CLOSURE(0),
    .max_values = 5,
};

Closure VirtualDom_lazy5 = {
    .evaluator = &eval_VirtualDom_thunk,
    .header = HEADER_CLOSURE(0),
    .max_values = 6,
};

Closure VirtualDom_lazy6 = {
    .evaluator = &eval_VirtualDom_thunk,
    .header = HEADER_CLOSURE(0),
    .max_values = 7,
};

Closure VirtualDom_lazy7 = {
    .evaluator = &eval_VirtualDom_thunk,
    .header = HEADER_CLOSURE(0),
    .max_values = 8,
};

Closure VirtualDom_lazy8 = {
    .evaluator = &eval_VirtualDom_thunk,
    .header = HEADER_CLOSURE(0),
    .max_values = 9,
};


// FACTS

typedef struct {
  Header header;
  u32 ctor;
  ElmString16* key;
  void* value;
  ElmString16* namespaces[]; // 0 or 1 element
} VdomFact;

static VdomFact* ctorFact(u32 ctor, ElmString16* key, void* value) {
  VdomFact* p = GC_malloc(sizeof(VdomFact));
  p->header = HEADER_CUSTOM(2);
  p->ctor = ctor;
  p->key = key;
  p->value = value;
}

static void* eval_VirtualDom_on(void* args[]) {
  return ctorFact(FACT_EVENT, args[0], args[1]);
}
Closure VirtualDom_on = {
  .header = HEADER_CLOSURE(0),
  .evaluator = eval_VirtualDom_on,
  .max_values = 2,
};

static void* eval_VirtualDom_style(void* args[]) {
  return ctorFact(FACT_STYLE, args[0], args[1]);
}
Closure VirtualDom_style = {
  .header = HEADER_CLOSURE(0),
  .evaluator = eval_VirtualDom_style,
  .max_values = 2,
};

static void* eval_VirtualDom_property(void* args[]) {
  return ctorFact(FACT_PROP, args[0], args[1]);
}
Closure VirtualDom_property = {
  .header = HEADER_CLOSURE(0),
  .evaluator = eval_VirtualDom_property,
  .max_values = 2,
};

static void* eval_VirtualDom_attribute(void* args[]) {
  return ctorFact(FACT_ATTR, args[0], args[1]);
}
Closure VirtualDom_attribute = {
  .header = HEADER_CLOSURE(0),
  .evaluator = eval_VirtualDom_attribute,
  .max_values = 2,
};

static void* eval_VirtualDom_attributeNS(void* args[]) {
  void* namespace = args[0];
  void* key = args[1];
  void* value = args[2];
  VdomFact* p = GC_malloc(sizeof(VdomFact) + sizeof(void*));
  p->header = HEADER_CUSTOM(2);
  p->ctor = FACT_ATTR_NS;
  p->key = key;
  p->value = value;
  p->namespaces[0] = namespace;
  return p;
}
Closure VirtualDom_attributeNS = {
  .header = HEADER_CLOSURE(0),
  .evaluator = eval_VirtualDom_attributeNS,
  .max_values = 3,
};



// XSS ATTACK VECTOR CHECKS

static bool string_match_ascii(size_t expect_len, char* expect, ElmString16* s) {
  if (code_units(s) < expect_len) {
    return false;
  }
  u16* words16 = s->words16;
  for (size_t i = 0; i < 5; ++i) {
    if (words16[i] != expect[i]) {
      return false;
    }
  }
  return true;
}


ElmString16 str_p = {
  .header = HEADER_STRING(1),
  .words16 = {'p'},
};
static void* eval_VirtualDom_noScript(void* args[]) {
  ElmString16* tag = args[0];
  return string_match_ascii(5, "script", tag) ? &str_p : tag;
}
Closure VirtualDom_noScript = {
  .header = HEADER_CLOSURE(0),
  .evaluator = eval_VirtualDom_noScript,
  .max_values = 1,
};

ElmString16 str_data_ = {
  .header = HEADER_STRING(1),
  .words16 = {'d','a','t','a','-'},
};
static void* eval_VirtualDom_noOnOrFormAction(void* args[]) {
  ElmString16* key = args[0];
  bool bad = string_match_ascii(2, "on", key) || string_match_ascii(10, "formAction", key);
  return bad ? A2(&String_append, &str_data_, key) : key;
}
Closure VirtualDom_noOnOrFormAction = {
  .header = HEADER_CLOSURE(0),
  .evaluator = eval_VirtualDom_noOnOrFormAction,
  .max_values = 1,
};

static void* eval_VirtualDom_noInnerHtmlOrFormAction(void* args[]) {
  ElmString16* key = args[0];
  bool bad = string_match_ascii(9, "innerHTML", key) || string_match_ascii(10, "formAction", key);
  return bad ? A2(&String_append, &str_data_, key) : key;
}
Closure VirtualDom_noInnerHtmlOrFormAction = {
  .header = HEADER_CLOSURE(0),
  .evaluator = eval_VirtualDom_noInnerHtmlOrFormAction,
  .max_values = 1,
};

static void* eval_VirtualDom_noJavaScriptUri(void* args[]) {
  ElmString16* value = args[0];
  ElmString16* trimmed = A1(&String_trimLeft, value);
  u16* words16 = trimmed->words16;
  size_t len = code_units(trimmed);
  if (len < 11) {
    return value;
  }
  char* lower = "javascript:";
  char* upper = "JAVASCRIPT:";
  for (size_t i = 0; i < 11; ++i) {
    if (words16[i] != lower[i] && words16[i] != upper[i]) {
      return value;
    }
  }
  return NEW_ELM_STRING16(0);
}
Closure VirtualDom_noJavaScriptUri = {
  .header = HEADER_CLOSURE(0),
  .evaluator = eval_VirtualDom_noJavaScriptUri,
  .max_values = 1,
};

static void* eval_VirtualDom_noJavaScriptOrHtmlUri(void* args[]) {
  ElmString16* value = args[0];
  ElmString16* trimmed = A1(&String_trimLeft, value);
  u16* words16 = trimmed->words16;
  size_t len = code_units(trimmed);

  if (len < 11) {
    return value;
  }
  char* js_lower = "javascript:";
  char* js_upper = "JAVASCRIPT:";
  for (size_t i = 0; i < 11; ++i) {
    if (words16[i] != js_lower[i] && words16[i] != js_upper[i]) {
      return value;
    }
  }

  if (len < 14) {
    return value;
  }
  char* html_lower = "data:text/html";
  char* html_upper = "DATA:TEXT/HTML";
  for (size_t i = 0; i < 14; ++i) {
    if (words16[i] != html_lower[i] && words16[i] != html_upper[i]) {
      return value;
    }
  }
  return NEW_ELM_STRING16(0);
}
Closure VirtualDom_noJavaScriptOrHtmlUri = {
  .header = HEADER_CLOSURE(0),
  .evaluator = eval_VirtualDom_noJavaScriptOrHtmlUri,
  .max_values = 1,
};


// MAP FACTS

enum {
  HandlerTagNormal = 0,
  HandlerTagMayStopPropagation = 1,
  HandlerTagMayPreventDefault = 2,
  HandlerTagCustom = 3,
};

static void* eval_mapEventTuple(void* args[]) {
  Closure* func = args[0];
  Tuple2* tuple = args[1];
  return NEW_TUPLE2(A1(func, tuple->a), tuple->b);
}
Closure mapEventTuple = {
  .header = HEADER_CLOSURE(0),
  .evaluator = eval_mapEventTuple,
  .max_values = 2,
};

static void* eval_mapEventRecord(void* args[]) {
  Closure* func = args[0];
  Record* r = args[1];

  void* accessArgs[] = {(void*)FIELD_message, r};
  void* oldMessage = Utils_access_eval(accessArgs);
  void* newMessage = A1(func, oldMessage);

  u32 updateFields[] = {FIELD_message};
  void* updateValues[] = {newMessage};
  return Utils_update(r, 1, updateFields, updateValues);
}
Closure mapEventRecord = {
  .header = HEADER_CLOSURE(0),
  .evaluator = eval_mapEventRecord,
  .max_values = 2,
};

static void* mapHandler(Closure* func, Custom* handler) {
  ElmInt* tagBoxed = A1(&g_elm_virtual_dom_VirtualDom_toHandlerInt, handler);
  i32 tag = tagBoxed->value;
  void* oldHandlerBody = handler->values[0];
  void* newHandlerBody;
  if (tag == HandlerTagNormal) {
      newHandlerBody = A2(&Json_map1, func, oldHandlerBody);
  } else {
    Closure* mapper = (tag == HandlerTagCustom) ? &mapEventRecord : &mapEventTuple;
    Custom* jsonFunc = A1(&Json_succeed, func);
    newHandlerBody = A3(&Json_map2, mapper, jsonFunc, oldHandlerBody);
  }
  return NEW_CUSTOM(handler->ctor, 1, (void*[]){newHandlerBody});
}

static void* eval_VirtualDom_mapAttribute(void* args[]) {
  Closure* func = args[0];
  VdomFact* attr = args[1];
  if (attr->ctor != FACT_EVENT) {
    return attr;
  }
  return A2(&VirtualDom_on, attr->key, mapHandler(func, attr->value));
}
Closure VirtualDom_mapAttribute = {
  .header = HEADER_CLOSURE(0),
  .evaluator = eval_VirtualDom_mapAttribute,
  .max_values = 2,
};


// ORGANIZE FACTS

/*
In JS, facts are organized in a one-or-two-level nested dictionary, like this:
Props are treated differently from other facts.
var facts = {
  'prop1': thing,
  'prop2': stuff,
  'a__1_EVENT': { click: myHandler },
  'a__1_STYLE': { color: 'blue' },
  'a__1_ATTR': {},
  'a__1_ATTR_NS': {},
}
But in this C implementation they're more like a 5-element *tuple* of dictionaries.
The closest equivalent in JS would be like this:
var facts = [
  { prop1: thing, prop2: stuff },
  { click: handler },
  { color: 'blue' },
  {},
  {},
];
We know the fixed index of each sub-dictionary, so we don't need to compare strings.
This refactoring simplifies the code
I'm not sure what the reason was for the original design.
Perhaps it's an optimisation to have fewer dereferences in common cases.
*/

static i32 objectFindIndex(Custom* object, ElmString16* key) {
  u32 n = custom_params(object);
  i32 found_index = -1;
  for (size_t i = 0; i < n; i+=2) {
    if (A2(&Utils_equal, object->values[i], key) == &True) {
      found_index = i + 1;
    }
  }
  return found_index;
}

static Custom* objectAppendPair(Custom* object, ElmString16* key, void* value) {
  u32 n = custom_params(object);
  Custom* newObject = Utils_clone(object);
  CAN_THROW(GC_malloc(2*sizeof(void*)));
  newObject->header.size += 2;
  newObject->values[n] = key;
  newObject->values[n + 1] = value;
  return newObject;
}

ElmString16 str_className = {
  .header = HEADER_STRING(9),
  .words16 = {'c', 'l', 'a', 's', 's', 'N', 'a', 'm', 'e'},
};
ElmString16 str_class = {
  .header = HEADER_STRING(5),
  .words16 = {'c', 'l', 'a', 's', 's'},
};
ElmString16 str_space = {
  .header = HEADER_STRING(1),
  .words16 = {' '},
};

static Custom* organizeFacts(Cons* factList) {
  Custom* facts[NUM_FACT_CTORS] = {
    &Json_emptyObjVal,
    &Json_emptyObjVal,
    &Json_emptyObjVal,
    &Json_emptyObjVal,
    &Json_emptyObjVal,
  };
  Cons* propClasses = &Nil;
  Cons* attrClasses = &Nil;

  for (; factList != &Nil; factList = factList->tail) {
    VdomFact* entry = factList->head;

    u32 tag = entry->ctor;
    ElmString16* key = entry->key;
    Custom* value = entry->value;

    Custom* subFacts = facts[tag];

    if (tag == FACT_PROP) {
      value = value->values[0]; // inlined Json_unwrap
      if (string_match_ascii(9, "className", key)) {
        propClasses = NEW_CONS(value, propClasses);
        continue;
      }
    } else if (tag == FACT_ATTR && string_match_ascii(5, "class", key)) {
        attrClasses = NEW_CONS(value, attrClasses);
        continue;
    }

    i32 found_index = objectFindIndex(subFacts, key);
    if (found_index >= 0) {
      subFacts->values[found_index] = value;
    } else {
      facts[tag] = CAN_THROW(objectAppendPair(subFacts, key, value));
    }
  }

  if (propClasses != &Nil) {
    ElmString16* joined = A2(&String_join, &str_space, propClasses);
    facts[FACT_PROP] = CAN_THROW(objectAppendPair(facts[FACT_PROP], &str_className, joined));
  }

  if (attrClasses != &Nil) {
    ElmString16* joined = A2(&String_join, &str_space, attrClasses);
    facts[FACT_ATTR] = CAN_THROW(objectAppendPair(facts[FACT_ATTR], &str_class, joined));
  }

  return NEW_CUSTOM(JSON_VALUE_ARRAY, NUM_FACT_CTORS, facts);
}
