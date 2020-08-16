#include "../core/core.h"

enum {
  VDOM_TEXT,
  VDOM_NODE,
  VDOM_KEYED_NODE,
  VDOM_CUSTOM,
  VDOM_TAGGER,
  VDOM_THUNK,
};

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

static Custom* VirtualDom_organizeFacts(Cons* factList) {
  return NULL;
}

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

  VdomNode* p = CAN_THROW(GC_malloc(sizeof(VdomNode)));
  *p = (VdomNode){
    .header = HEADER_CUSTOM(4),
    .ctor = VDOM_NODE,
    .descendantsCount = descendantsCount,
    .namespace = namespace,
    .tag = tag,
    .facts = VirtualDom_organizeFacts(factList),
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

  VdomNode* p = CAN_THROW(GC_malloc(sizeof(VdomNode)));
  *p = (VdomNode){
    .header = HEADER_CUSTOM(4),
    .ctor = VDOM_KEYED_NODE,
    .descendantsCount = descendantsCount,
    .namespace = namespace,
    .tag = tag,
    .facts = VirtualDom_organizeFacts(factList),
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


void* eval_VirtualDom_on(void* args[]) {
  return NEW_CUSTOM(FACT_EVENT, 2, args);
}
Closure VirtualDom_on = {
  .header = HEADER_CLOSURE(0),
  .evaluator = eval_VirtualDom_on,
  .max_values = 2,
};

void* eval_VirtualDom_style(void* args[]) {
  return NEW_CUSTOM(FACT_STYLE, 2, args);
}
Closure VirtualDom_style = {
  .header = HEADER_CLOSURE(0),
  .evaluator = eval_VirtualDom_style,
  .max_values = 2,
};

void* eval_VirtualDom_property(void* args[]) {
  return NEW_CUSTOM(FACT_PROP, 2, args);
}
Closure VirtualDom_property = {
  .header = HEADER_CLOSURE(0),
  .evaluator = eval_VirtualDom_property,
  .max_values = 2,
};

void* eval_VirtualDom_attribute(void* args[]) {
  return NEW_CUSTOM(FACT_ATTR, 2, args);
}
Closure VirtualDom_attribute = {
  .header = HEADER_CLOSURE(0),
  .evaluator = eval_VirtualDom_attribute,
  .max_values = 2,
};

void* eval_VirtualDom_attributeNS(void* args[]) {
  void* namespace = args[0];
  void* key = args[1];
  void* value = args[2];
  void* rearranged[] = {key, value, namespace};
  return NEW_CUSTOM(FACT_ATTR_NS, 3, rearranged);
}
Closure VirtualDom_attributeNS = {
  .header = HEADER_CLOSURE(0),
  .evaluator = eval_VirtualDom_attributeNS,
  .max_values = 3,
};



// XSS ATTACK VECTOR CHECKS

ElmString16 str_p = {
  .header = HEADER_STRING(1),
  .words16 = {'p'},
};
void* eval_VirtualDom_noScript(void* args[]) {
  ElmString16* tag = args[0];
  char str[] = "script";
  u16* words16 = tag->words16;
  for (size_t i = 0; i < 5; ++i) {
    if (words16[i] != str[i]) {
      return tag;
    }
  }
  return &str_p;
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
void* eval_VirtualDom_noOnOrFormAction(void* args[]) {
  ElmString16* key = args[0];
  bool on = key->words16[0] == 'o' && key->words16[1] == 'n';
  if (!on) {
    char str[] = "formAction";
    u16* words16 = key->words16;
    for (size_t i = 0; i < 5; ++i) {
      if (words16[i] != str[i]) {
        return key;
      }
    }
  }
  return A2(&String_append, &str_data_, key);
}
Closure VirtualDom_noOnOrFormAction = {
  .header = HEADER_CLOSURE(0),
  .evaluator = eval_VirtualDom_noOnOrFormAction,
  .max_values = 1,
};

void* eval_VirtualDom_noInnerHtmlOrFormAction(void* args[]) {
  ElmString16* key = args[0];
  u16 first = key->words16[0];
  char* str;
  size_t len;
  if (first == 'i') {
    str = "innerHTML";
    len = 9;
  } else if (first == 'f') {
    str = "formAction";
    len = 10;
  } else {
    return key;
  }
  u16* words16 = key->words16;
  for (size_t i = 1; i < len; ++i) {
    if (words16[i] != str[i]) {
      return key;
    }
  }
  return A2(&String_append, &str_data_, key);
}
Closure VirtualDom_noInnerHtmlOrFormAction = {
  .header = HEADER_CLOSURE(0),
  .evaluator = eval_VirtualDom_noInnerHtmlOrFormAction,
  .max_values = 1,
};

void* eval_VirtualDom_noJavaScriptUri(void* args[]) {
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

void* eval_VirtualDom_noJavaScriptOrHtmlUri(void* args[]) {
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

