#ifndef VIRTUAL_DOM_H
#define VIRTUAL_DOM_H

#include "../core/types.h"
#include "../core/gc/internals.h"

/* ==============================================================================

                            TYPES & MACROS

============================================================================== */

enum vdom_ctor {
  VDOM_NODE = 1,       // values: tag, facts, children
  VDOM_NODE_KEYED,     // values: tag, keys, facts, children
  VDOM_NODE_NS,        // values: namespace, tag, facts, children
  VDOM_NODE_NS_KEYED,  // values: namespace, tag, keys, facts, children
  VDOM_NODE_TEXT,      // values: content
  VDOM_NODE_TAGGER,    // values: tagger, child
  VDOM_NODE_THUNK,     // values: thunk, refs, node
  // ------------
  VDOM_FACT_EVENT,    // value: handler
  VDOM_FACT_STYLE,    // value: String
  VDOM_FACT_PROP,     // value: Json
  VDOM_FACT_ATTR,     // value: String
  VDOM_FACT_ATTR_NS,  // value: (String, String)
  // ------------
  VDOM_PATCH_PUSH,       // number: child_index
  VDOM_PATCH_POP,             // (no data)
  VDOM_PATCH_LINK,            // values: vdom_patch
  VDOM_PATCH_NO_OP,           // (no data)
  VDOM_PATCH_END,             // (no data)
  VDOM_PATCH_REDRAW,          // values: vdom_node
  VDOM_PATCH_SET_EVENT,       // number: 2, values: key, value
  VDOM_PATCH_SET_STYLE,       // number: 2, values: key, value
  VDOM_PATCH_SET_PROP,        // number: 2, values: key, value
  VDOM_PATCH_SET_ATTR,        // number: 2, values: key, value
  VDOM_PATCH_SET_ATTR_NS,     // number: 3, values: namespace, key, value
  VDOM_PATCH_REMOVE_EVENT,    // number: 1, values: key
  VDOM_PATCH_REMOVE_STYLE,    // number: 1, values: key
  VDOM_PATCH_REMOVE_PROP,     // number: 1, values: key
  VDOM_PATCH_REMOVE_ATTR,     // number: 1, values: key
  VDOM_PATCH_REMOVE_ATTR_NS,  // number: 2, values: namespace, key
  VDOM_PATCH_TEXT,            // values: replacement_string_ptr
  // VDOM_PATCH_THUNK,        // values: sub_patches[]
  VDOM_PATCH_TAGGER,       // values: tagger, eventNode??
  VDOM_PATCH_REMOVE_LAST,  // number: num_to_remove
  VDOM_PATCH_APPEND,       // number: nKids, values: kids
  VDOM_PATCH_REMOVE,       // (keyed only)
  VDOM_PATCH_REORDER,      // (keyed only) patches, inserts, endInserts
  // VDOM_PATCH_CUSTOM,       // ? no idea what this is
};

struct vdom_node {
  u8 ctor;
  u8 n_extras;
  u8 n_facts;
  u8 n_children;
  void* values[];
};

struct vdom_fact {
  size_t ctor;
  ElmString* key;
  void* value;
};

struct vdom_patch {
  u8 ctor;
  u32 number : 24;
  void* values[];
};

#define VDOM_BUCKET_BYTES 4096
#define VDOM_BUCKETS_PER_PAGE (GC_WASM_PAGE_BYTES / VDOM_BUCKET_BYTES)
#define VDOM_BUCKET_WORDS (VDOM_BUCKET_BYTES / sizeof(size_t))

typedef u16 VdomFlags;  // number of bits must be at least VDOM_BUCKETS_PER_PAGE

struct vdom_page_metadata {
  VdomFlags live_flags;
  VdomFlags generation_flags;
  VdomFlags patch_flags;
  struct vdom_page* next;
};

#define VDOM_PAGE_WORDS ((GC_WASM_PAGE_BYTES - sizeof(struct vdom_page_metadata)) / sizeof(size_t))

struct vdom_page {
  size_t words[VDOM_PAGE_WORDS];
  struct vdom_page_metadata meta;
};

struct vdom_state {
  struct vdom_node* vdom_old;
  struct vdom_node* vdom_current;
  struct vdom_page* first_page;
  struct vdom_page* current_page;
  size_t* next_node;
  size_t* bottom_node;
  size_t* next_fact;
  size_t* bottom_fact;
  size_t* next_patch;
  size_t* top_patch;
  bool generation;
};


/* ==============================================================================

                            PUBLIC API

============================================================================== */

extern Closure VirtualDom_text;
extern Closure VirtualDom_node;
extern Closure VirtualDom_style;
extern Closure VirtualDom_attribute;
extern Closure VirtualDom_diff;


#endif
