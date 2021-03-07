#include <stdio.h>
#include <string.h>
#include "../src/kernel/core/gc/internals.h"
#include "../src/kernel/kernel.h"

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
  VDOM_PATCH_PUSH = 16,       // number: child_index
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
  u8 ctor;
  ElmString16* key;
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
  struct vdom_page* next;
  VdomFlags live_flags;
  VdomFlags generation_flags;
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

static struct vdom_state state;


/* ==============================================================================

                            MEMORY ALLOCATION

============================================================================== */


#ifdef DEBUG
static void clear_dead_buckets(struct vdom_page* page) {
  VdomFlags bit = 1;
  for (size_t i = 0; i < VDOM_PAGE_WORDS; i += VDOM_BUCKET_WORDS, bit <<= 1) {
    if (page->meta.live_flags & bit) continue;
    for (size_t j = i; j < i + VDOM_BUCKET_WORDS && j < VDOM_PAGE_WORDS; ++j) {
      page->words[j] = 0;
    }
  }
}
#else
#define clear_dead_buckets(x)
#endif


size_t* start_new_node_bucket();
size_t* start_new_fact_bucket();


void init_vdom_allocator() {
  struct vdom_page* page = GC_get_memory_from_system(GC_WASM_PAGE_BYTES);
  page->meta.next = NULL;
  page->meta.live_flags = 0;
  page->meta.generation_flags = 0;

  state.first_page = page;
  state.current_page = page;
  start_new_node_bucket();
  start_new_fact_bucket();
}


static void next_generation() {
  state.generation = !state.generation;
  state.vdom_old = state.vdom_current;
  state.vdom_current = NULL;

  VdomFlags generation = state.generation ? (VdomFlags)(-1) : 0;
  for (struct vdom_page* c = state.first_page; c; c = c->meta.next) {
    c->meta.live_flags &= (c->meta.generation_flags ^ generation);
    clear_dead_buckets(c);
  }
  start_new_node_bucket();
  start_new_fact_bucket();
}


void start_new_bucket(size_t** top, size_t** bottom) {
  struct vdom_page* page = state.current_page;

  VdomFlags bit = 1 << (VDOM_BUCKETS_PER_PAGE - 1);
  size_t i = (VDOM_BUCKETS_PER_PAGE - 1) * VDOM_BUCKET_WORDS;

  for (; page->meta.live_flags & bit; bit >>= 1, i -= VDOM_BUCKET_WORDS)
    ;
  assert(bit);  // TODO, allocate new page
  size_t* found = &page->words[i];
  page->meta.live_flags |= bit;
  if (state.generation) {
    page->meta.generation_flags |= bit;
  } else {
    page->meta.generation_flags &= ~bit;
  }
  *bottom = found;
  *top = found + VDOM_BUCKET_WORDS - 1;

  size_t* max_addr = &page->words[VDOM_PAGE_WORDS - 1];
  if (*top > max_addr) {
    *top = max_addr;
  }
}


size_t* start_new_node_bucket() {
  printf("start_new_node_bucket: switching from %p ", state.bottom_node);
  start_new_bucket(&state.next_node, &state.bottom_node);
  printf("to %p\n", state.bottom_node);
  return state.next_node;
}


size_t* start_new_fact_bucket() {
  printf("start_new_fact_bucket: switching from %p ", state.bottom_fact);
  start_new_bucket(&state.next_fact, &state.bottom_fact);
  printf("to %p\n", state.bottom_fact);
  return state.next_fact;
}


size_t* start_new_patch_bucket() {
  printf("start_new_patch_bucket: switching from %p ", state.next_patch);
  start_new_bucket(&state.top_patch, &state.next_patch);
  printf("to %p\n", state.next_patch);
  return state.next_patch;
}


// Allocate from top down
// because allocation order is usually the opposite of diff traversal order
void* allocate_node(size_t words) {
  size_t* allocated = state.next_node - (words - 1);
  state.next_node = allocated - 1;
  if (state.next_node < state.bottom_node) {
    state.next_node = start_new_node_bucket();
    allocated = state.next_node - (words - 1);
    state.next_node = allocated - 1;
  }
  return allocated;
}


// Allocate from top down
// because allocation order is usually the opposite of diff traversal order
void* allocate_fact() {
  size_t words = 3;
  size_t* allocated = state.next_fact - (words - 1);
  state.next_fact = allocated - 1;
  if (state.next_fact < state.bottom_fact) {
    state.next_fact = start_new_fact_bucket();
    allocated = state.next_fact - (words - 1);
    state.next_fact = allocated - 1;
  }
  return allocated;
}


// Allocate forwards
void* allocate_patch(size_t words) {
  size_t* allocated = state.next_patch;
  if (allocated + words + 2 >= state.top_patch) {
    struct vdom_patch* link = (struct vdom_patch*)state.next_patch;
    link->ctor = VDOM_PATCH_LINK;
    link->number = 1;
    allocated = start_new_patch_bucket();
    link->values[0] = allocated;
  }
  state.next_patch += words;
  return allocated;
}


/* ==============================================================================

                            EXTERNAL API

============================================================================== */

static void* eval_VirtualDom_text(void* args[]) {
  ElmString16* string = args[0];
  struct vdom_node* node = allocate_node(2);
  node->ctor = VDOM_NODE_TEXT;
  node->n_extras = 1;
  node->n_facts = 0;
  node->n_children = 0;
  node->values[0] = string;
  return node;
}
Closure VirtualDom_text = {
    .header = HEADER_CLOSURE(0),
    .evaluator = eval_VirtualDom_text,
    .max_values = 1,
};


// need a fancier version to "organize" facts
size_t prepend_list_or_start_new_bucket(Cons* list) {
  size_t n = 0;
  for (; list != pNil; list = list->tail) {
    n++;
    *state.next_node = (size_t)list->head;
    state.next_node--;
    if (state.next_node < state.bottom_node) {
      printf("prepend_list_or_start_new_bucket: overflowed at %p\n", state.bottom_node);
      start_new_node_bucket();
      return -1;
    }
  }
  return n;
}


static void* eval_VirtualDom_node(void* args[]) {
  ElmString16* tag = args[0];
  Cons* factList = args[1];
  Cons* kidList = args[2];

  for (size_t attempts = 0; attempts < 2; ++attempts) {
    size_t n_children = prepend_list_or_start_new_bucket(kidList);
    if (n_children == -1) continue;

    // This needs to get more complicated than prepend_list_or_start_new_bucket (organize facts)
    size_t n_facts = prepend_list_or_start_new_bucket(factList);
    if (n_facts == -1) continue;

    if (state.next_node - 2 < state.bottom_node) {
      printf("eval_VirtualDom_node: overflowed at %p\n", state.bottom_node);
      start_new_node_bucket();
      continue;
    }

    *state.next_node-- = (size_t)tag;
    struct vdom_node* node = (struct vdom_node*)state.next_node--;

    *node = (struct vdom_node){
        .ctor = VDOM_NODE,
        .n_extras = 1,
        .n_facts = n_facts,
        .n_children = n_children,
    };

    return node;
  }
  assert(false);
}
Closure VirtualDom_node = {
    .header = HEADER_CLOSURE(0),
    .evaluator = eval_VirtualDom_node,
    .max_values = 3,
};


/*

Rules for organize facts:
  - if you get the same key twice, last one wins (including style!)
  - class is special, join them all with spaces (prop and attr)
  - props other than className get Json_unwrap

Implementation:
  - have a separate bucket for classnames
    - join them when creating patches, no need on creation
  - put Json_unwrap in the property function (if not className)
  - do a linear key search before inserting
    - or use a hashmap? so that generic prop will work
    - maybe detect this as a special case with IS_OUTSIDE_HEAP?
      - if anything is outside heap, do a slow path
      - otherwise pointer compare will do (common case)
      - could actually set a "slow facts" flag on the node? (steal a bit from ctor)

*/
void* eval_VirtualDom_style(void* args[]) {
  ElmString16* key = args[0];
  ElmString16* value = args[1];
  assert(key->header.tag == Tag_String);
  assert(value->header.tag == Tag_String);
  struct vdom_fact* fact = allocate_fact();
  *fact = (struct vdom_fact){
      .ctor = VDOM_FACT_STYLE,
      .key = key,
      .value = value,
  };
  return fact;
}
Closure VirtualDom_style = {
    .header = HEADER_CLOSURE(0),
    .evaluator = eval_VirtualDom_style,
    .max_values = 2,
};

/* ==============================================================================

                                  DIFF

                           size     number           values
  VDOM_PATCH_PUSH = 16,     1+0  | child_index |
  VDOM_PATCH_POP,           1+0  | nLevels     |
  VDOM_PATCH_REDRAW,        1+1  | 1           | vdom_node
  VDOM_PATCH_FACTS,         1+n  | n           | replacement_facts[]
  VDOM_PATCH_TEXT,          1+1  | 1           | replacement_string_ptr
  // VDOM_PATCH_THUNK,      1+n  | n           | sub_patches[]
  VDOM_PATCH_TAGGER,        1+2  | 2           | tagger, eventNode??
  VDOM_PATCH_REMOVE_LAST,   1+0  | n           |
  VDOM_PATCH_APPEND,        1+n  | n           | kids

============================================================================== */

void patch_redraw(struct vdom_node* node) {
  struct vdom_patch* patch = allocate_patch(2);
  patch->ctor = VDOM_PATCH_REDRAW;
  patch->number = 1;
  patch->values[0] = node;
}


bool strings_match(ElmString16* x, ElmString16* y) {
  if (x == y) return true;

  GcHeap* heap = &gc_state.heap;
  if (IS_OUTSIDE_HEAP(x) && IS_OUTSIDE_HEAP(y)) {
    return false;
  }

  size_t* x_words = (size_t*)x;
  size_t* y_words = (size_t*)y;
  for (size_t i = 0; i < x->header.size; ++i) {
    if (x_words[i] != y_words[i]) return false;
  }

  return true;
}

/**
 * Diff "facts" (props, attributes, event handlers, etc.)
 *
 * Number of facts is almost always 0 or 1. The highest in practice is maybe 5.
 * For very small numbers like this, linear array search is fast and simple.
 * Generally you don't see divs with 100 attributes and even that would be OK.
 */
static void diffFacts(struct vdom_node* oldNode, struct vdom_node* newNode) {
  u8 nOld = oldNode->n_facts;
  u8 nNew = newNode->n_facts;
  void** oldFacts = oldNode->values + oldNode->n_extras;
  void** newFacts = newNode->values + newNode->n_extras;

  // Set facts
  for (u8 n = 0; n < nNew; ++n) {
    struct vdom_fact* newFact = newFacts[n];
    struct vdom_fact* oldFact = NULL;

    // Search for a matching key, starting in the same position.
    u8 o = n;
    for (u8 i = 0; i < nOld; ++i) {
      struct vdom_fact* f = oldFacts[o];
      if (f->ctor == newFact->ctor && strings_match(f->key, newFact->key)) {
        oldFact = f;
        break;
      }
      ++o;
      if (o == nOld) o = 0;
    }

    if (oldFact && strings_match(oldFact->value, newFact->value)) {
      continue;  // key and value match => no patch needed
    }

    struct vdom_patch* setPatch;
    if (newFact->ctor == VDOM_FACT_ATTR_NS) {
      setPatch = allocate_patch(4);
      setPatch->ctor = VDOM_PATCH_SET_ATTR_NS;
      setPatch->number = 3;
      setPatch->values[0] = newFact->key;
      Tuple2* pair = newFact->value;
      ElmString16* namespace = pair->a;
      setPatch->values[1] = namespace;
      setPatch->values[2] = pair->b;
    } else {
      u8 ctor;
      switch (newFact->ctor) {
        case VDOM_FACT_EVENT:
          ctor = VDOM_PATCH_SET_EVENT;
          break;
        case VDOM_FACT_STYLE:
          ctor = VDOM_PATCH_SET_STYLE;
          break;
        case VDOM_FACT_PROP:
          ctor = VDOM_PATCH_SET_PROP;
          break;
        case VDOM_FACT_ATTR:
          ctor = VDOM_PATCH_SET_ATTR;
          break;
      }
      setPatch = allocate_patch(3);
      setPatch->ctor = ctor;
      setPatch->number = 2;
      setPatch->values[0] = newFact->key;
      setPatch->values[1] = newFact->value;
    }
  }

  // Remove facts
  for (u8 o = 0; o < nOld; ++o) {
    struct vdom_fact* oldFact = oldFacts[o];
    bool found = false;
    // Search for a matching key, starting in the same position.
    u8 n = o;
    for (u8 i = 0; i < nNew; ++i) {
      struct vdom_fact* f = newFacts[n];
      if (f->ctor == oldFact->ctor && strings_match(f->key, oldFact->key)) {
        found = true;
        break;
      }
      ++n;
      if (n == nNew) n = 0;
    }
    if (found) {
      continue;
    }
    struct vdom_patch* removePatch;
    if (oldFact->ctor == VDOM_FACT_ATTR_NS) {
      removePatch = allocate_patch(3);
      removePatch->ctor = VDOM_PATCH_REMOVE_ATTR_NS;
      removePatch->number = 2;
      removePatch->values[0] = oldFact->key;
      Tuple2* pair = oldFact->value;
      ElmString16* namespace = pair->a;
      removePatch->values[1] = namespace;
    } else {
      u8 ctor;
      switch (oldFact->ctor) {
        case VDOM_FACT_EVENT:
          ctor = VDOM_PATCH_REMOVE_EVENT;
          break;
        case VDOM_FACT_STYLE:
          ctor = VDOM_PATCH_REMOVE_STYLE;
          break;
        case VDOM_FACT_PROP:
          ctor = VDOM_PATCH_REMOVE_PROP;
          break;
        case VDOM_FACT_ATTR:
          ctor = VDOM_PATCH_REMOVE_ATTR;
          break;
        default:
          ctor = 0;
          break;
      }
      assert(ctor);
      removePatch = allocate_patch(2);
      removePatch->ctor = ctor;
      removePatch->number = 1;
      removePatch->values[0] = oldFact->key;
    }
  }
}

static void diffNodes(struct vdom_node* old, struct vdom_node* new);

static void diffChildren(struct vdom_node* oldParent, struct vdom_node* newParent) {
  u8 nOld = oldParent->n_children;
  u8 nNew = newParent->n_children;
  u8 nMin = (nOld < nNew) ? nOld : nNew;
  void** oldChildren = oldParent->values + oldParent->n_extras + oldParent->n_facts;
  void** newChildren = newParent->values + oldParent->n_extras + oldParent->n_facts;
  struct vdom_patch* push = allocate_patch(1);
  struct vdom_patch* pop = NULL;
  for (u8 i = 0; i < nMin; ++i) {
    push->ctor = VDOM_PATCH_PUSH;
    push->number = i;
    size_t* beforeChild = state.next_patch;

    diffNodes(oldChildren[i], newChildren[i]);

    size_t* afterChild = state.next_patch;
    if (afterChild != beforeChild) {
      pop = allocate_patch(1);
      pop->ctor = VDOM_PATCH_POP;
      pop->number = 0;
      push = allocate_patch(1);
    }
  }
  if (!pop) {
    push->ctor = VDOM_PATCH_NO_OP;
  }
  if (nNew > nOld) {
    u8 nAppend = nNew - nOld;
    struct vdom_patch* patch = allocate_patch(1 + nAppend);
    patch->ctor = VDOM_PATCH_APPEND;
    patch->number = nAppend;
    for (u8 i = nOld; i < nNew; ++i) {
      patch->values[i] = newChildren[i];
    }
  }
  if (nOld > nNew) {
    u8 nRemove = nOld - nNew;
    struct vdom_patch* patch = allocate_patch(1 + nRemove);
    patch->ctor = VDOM_PATCH_REMOVE_LAST;
    patch->number = nRemove;
    for (u8 i = nNew; i < nOld; ++i) {
      patch->values[i] = oldChildren[i];
    }
  }
}

static void diffNodes(struct vdom_node* old, struct vdom_node* new) {
  if (old->ctor != new->ctor) {
    patch_redraw(new);
    return;
  }
  switch (new->ctor) {
    case VDOM_NODE: {
      ElmString16* currTag = old->values[0];
      ElmString16* nextTag = new->values[0];
      if (!strings_match(currTag, nextTag)) {
        patch_redraw(new);
        return;
      }
      diffFacts(old, new);
      diffChildren(old, new);
      return;
    }
    case VDOM_NODE_TEXT: {
      ElmString16* currText = old->values[0];
      ElmString16* nextText = new->values[0];
      if (!strings_match(currText, nextText)) {
        patch_redraw(new);
      }
      return;
    }
    case VDOM_NODE_KEYED:
    case VDOM_NODE_NS:
    case VDOM_NODE_NS_KEYED:
    case VDOM_NODE_TAGGER:
    case VDOM_NODE_THUNK:
    default:
      assert(false);
      break;
  }
}


// Browser package calls a JS function, which delegates to this
void* eval_VirtualDom_diff(void* args[]) {
  struct vdom_node* currNode = args[0];
  struct vdom_node* nextNode = args[1];
  struct vdom_patch* first_patch = (struct vdom_patch*)state.next_patch;

  diffNodes(currNode, nextNode);

  // Mark the end of the patches. (We have at least 2 words of space)
  struct vdom_patch* end_marker = (struct vdom_patch*)state.next_patch;
  end_marker->ctor = VDOM_PATCH_END;
  end_marker->number = 0;

  return first_patch;
}
Closure VirtualDom_diff = {
    .header = HEADER_CLOSURE(2),
    .max_values = 2,
    .evaluator = eval_VirtualDom_diff,
};


/* ==============================================================================

                                  TEST CODE

============================================================================== */


static char* stringify_vdom_ctor(u8 ctor) {
  switch (ctor) {
    case VDOM_NODE:
      return "VDOM_NODE         ";
    case VDOM_NODE_KEYED:
      return "VDOM_NODE_KEYED   ";
    case VDOM_NODE_NS:
      return "VDOM_NODE_NS      ";
    case VDOM_NODE_NS_KEYED:
      return "VDOM_NODE_NS_KEYED";
    case VDOM_NODE_TEXT:
      return "VDOM_NODE_TEXT    ";
    case VDOM_NODE_TAGGER:
      return "VDOM_NODE_TAGGER  ";
    case VDOM_NODE_THUNK:
      return "VDOM_NODE_THUNK   ";
    case VDOM_FACT_EVENT:
      return "VDOM_FACT_EVENT   ";
    case VDOM_FACT_STYLE:
      return "VDOM_FACT_STYLE   ";
    case VDOM_FACT_PROP:
      return "VDOM_FACT_PROP    ";
    case VDOM_FACT_ATTR:
      return "VDOM_FACT_ATTR    ";
    case VDOM_FACT_ATTR_NS:
      return "VDOM_FACT_ATTR_NS ";
    default:
      return "(unknown ctor)";
  }
}

bool is_constant_string(ElmString16* s);

extern GcState gc_state;
bool is_valid_pointer(void* p) {
  if (is_constant_string(p)) {
    return true;
  }

  size_t* word = p;
  for (struct vdom_page* c = state.first_page; c; c = c->meta.next) {
    if (word > c->words && word < &c->words[VDOM_PAGE_WORDS]) {
      return true;
    }
  }

  GcHeap* heap = &gc_state.heap;
  if (!IS_OUTSIDE_HEAP(p)) {
    return true;
  }

  return false;
}

static void print_string(ElmString16* s) {
  assert(is_valid_pointer(s));
  assert(s->header.tag == Tag_String);
  for (int i = 0; i < code_units(s); ++i) {
    putchar(s->words16[i]);
  }
}

static void print_fact_as_html(struct vdom_fact* fact) {
  assert(fact);
  assert(fact->ctor == VDOM_FACT_STYLE);
  printf(" style=\"");
  print_string(fact->key);
  printf(": ");
  print_string(fact->value);
  printf(";\"");
}

static void print_node_as_html(struct vdom_node* node) {
  assert(node);
  switch (node->ctor) {
    case VDOM_NODE: {
      ElmString16* tag = node->values[0];
      printf("<");
      print_string(tag);
      int last_fact = node->n_extras + node->n_facts - 1;
      for (int i = last_fact; i >= node->n_extras; --i) {
        print_fact_as_html(node->values[i]);
      }
      printf(">\n");
      int last_child = node->n_extras + node->n_facts + node->n_children - 1;
      for (int i = last_child; i > last_fact; --i) {
        print_node_as_html(node->values[i]);
      }
      printf("</");
      print_string(tag);
      printf(">\n");
      break;
    }
    case VDOM_NODE_TEXT: {
      ElmString16* text = node->values[0];
      print_string(text);
      printf("\n");
      break;
    }
    default:
      assert(false);
      break;
  }
}

static void print_vdom_fact_header(struct vdom_fact* fact) {
  assert(fact);
  printf("    %p " FORMAT_HEX " %s ", fact, *(size_t*)fact, stringify_vdom_ctor(fact->ctor));
  print_string(fact->key);
  printf(" ");
  print_string(fact->value);
  printf("\n");
}


static void print_vdom_node_header(struct vdom_node* node) {
  assert(node);
  printf("    %p " FORMAT_HEX " %s n_extras=%d n_facts=%d n_children=%d",
      node,
      *(size_t*)node,
      stringify_vdom_ctor(node->ctor),
      node->n_extras,
      node->n_facts,
      node->n_children);
  int n_values = node->n_extras + node->n_facts + node->n_children;
  for (int i = 0; i < n_values; ++i) {
    printf(" %p", node->values[i]);
  }
  printf("\n");
}


static void print_addr_and_value(void* p) {
  printf("    %p " FORMAT_HEX "\n", p, *(size_t*)p);
}

static void print_vdom_page(struct vdom_page* page) {
  printf("Page at %p\n", page);
  printf("  next             %p\n", page->meta.next);
  printf("  live_flags       0x%04x\n", page->meta.live_flags);
  printf("  generation_flags 0x%04x\n", page->meta.generation_flags);

  VdomFlags bit = 1;
  for (size_t i = 0; i < VDOM_PAGE_WORDS; i += VDOM_BUCKET_WORDS, bit <<= 1) {
    bool live = page->meta.live_flags & bit;
    printf("  Bucket at %p (%04x) live=%x generation=%x\n",
        &page->words[i],
        bit,
        live,
        !!(page->meta.generation_flags & bit));
    bool skip = true;
    for (size_t j = i; j < i + VDOM_BUCKET_WORDS && j < VDOM_PAGE_WORDS; ++j) {
      size_t* p = &page->words[j];
      if (*p) skip = false;
      if (skip) continue;

      void* maybe_garbage = (void*)(*p);
      if (is_valid_pointer(maybe_garbage)) {
        printf("    %p " FORMAT_HEX " (garbage)\n", p, *(size_t*)p);
        continue;
      }

      u8 ctor = *(u8*)p;
      switch (ctor) {
        case VDOM_NODE:
        case VDOM_NODE_KEYED:
        case VDOM_NODE_NS:
        case VDOM_NODE_NS_KEYED:
        case VDOM_NODE_TEXT:
        case VDOM_NODE_TAGGER:
        case VDOM_NODE_THUNK: {
          struct vdom_node* node = (struct vdom_node*)p;
          size_t n_values = node->n_extras + node->n_facts + node->n_children;
          print_vdom_node_header(node);
          for (size_t k = 0; k < n_values; ++k) {
            print_addr_and_value(&node->values[k]);
          }
          j += n_values;
          break;
        }
        case VDOM_FACT_EVENT:
        case VDOM_FACT_STYLE:
        case VDOM_FACT_PROP:
        case VDOM_FACT_ATTR:
        case VDOM_FACT_ATTR_NS: {
          print_vdom_fact_header((struct vdom_fact*)p);
          print_addr_and_value(p + 1);
          print_addr_and_value(p + 2);
          j += 2;
          break;
        }
        default:
          printf("unknown ctor at %p\n", p);
          fflush(0);
          assert(false);
          break;
      }
    }
  }
}

static void print_vdom_state() {
  printf("\n");
  printf("vdom_old      %p\n", state.vdom_old);
  printf("vdom_current  %p\n", state.vdom_current);
  printf("first_page    %p\n", state.first_page);
  printf("current_page  %p\n", state.current_page);
  printf("next_node     %p\n", state.next_node);
  printf("bottom_node   %p\n", state.bottom_node);
  printf("next_fact     %p\n", state.next_fact);
  printf("bottom_fact   %p\n", state.bottom_fact);
  printf("generation    %d\n", state.generation);
  printf("\n");

  for (struct vdom_page* c = state.first_page; c; c = c->meta.next) {
    print_vdom_page(c);
  }
}

ElmString16 str_ul = {.header = HEADER_STRING(2), .words16 = {'u', 'l'}};
ElmString16 str_li = {.header = HEADER_STRING(2), .words16 = {'l', 'i'}};
ElmString16 str_color = {.header = HEADER_STRING(5), .words16 = {'c', 'o', 'l', 'o', 'r'}};
ElmString16 str_red = {.header = HEADER_STRING(3), .words16 = {'r', 'e', 'd'}};
ElmString16 str_blue = {.header = HEADER_STRING(4), .words16 = {'b', 'l', 'u', 'e'}};
ElmString16 str_padding = {.header = HEADER_STRING(7), .words16 = {'p', 'a', 'd', 'd', 'i', 'n', 'g'}};
ElmString16 str_10px = {.header = HEADER_STRING(4), .words16 = {'1', '0', 'p', 'x'}};
ElmString16 str_hello = {.header = HEADER_STRING(5), .words16 = {'h', 'e', 'l', 'l', 'o'}};
ElmString16 str_there = {.header = HEADER_STRING(5), .words16 = {'t', 'h', 'e', 'r', 'e'}};
ElmString16 str_margin = {.header = HEADER_STRING(6), .words16 = {'m', 'a', 'r', 'g', 'i', 'n'}};
ElmString16 str_auto = {.header = HEADER_STRING(4), .words16 = {'a', 'u', 't', 'o'}};
ElmString16 str_float = {.header = HEADER_STRING(5), .words16 = {'f', 'l', 'o', 'a', 't'}};
ElmString16 str_left = {.header = HEADER_STRING(4), .words16 = {'l', 'e', 'f', 't'}};
ElmString16 str_right = {.header = HEADER_STRING(5), .words16 = {'r', 'i', 'g', 'h', 't'}};
ElmString16 str_world = {.header = HEADER_STRING(5), .words16 = {'w', 'o', 'r', 'l', 'd'}};
ElmString16 str_people = {.header = HEADER_STRING(6), .words16 = {'p', 'e', 'o', 'p', 'l', 'e'}};
ElmString16 str_whats = {.header = HEADER_STRING(6), .words16 = {'w', 'h', 'a', 't', '\'', 's'}};
ElmString16 str_up = {.header = HEADER_STRING(3), .words16 = {'u', 'p', '?'}};
ElmString16 str_div = {.header = HEADER_STRING(3), .words16 = {'d', 'i', 'v'}};
ElmString16 str_p = {.header = HEADER_STRING(1), .words16 = {'p'}};
ElmString16 str_brian = {.header = HEADER_STRING(5), .words16 = {'B', 'r', 'i', 'a', 'n'}};
ElmString16 str_display = {.header = HEADER_STRING(7), .words16 = {'d', 'i', 's', 'p', 'l', 'a', 'y'}};
ElmString16 str_flex = {.header = HEADER_STRING(4), .words16 = {'f', 'l', 'e', 'x'}};

ElmString16* constant_strings[] = {
    &str_ul,
    &str_li,
    &str_color,
    &str_red,
    &str_blue,
    &str_padding,
    &str_10px,
    &str_hello,
    &str_there,
    &str_margin,
    &str_auto,
    &str_float,
    &str_left,
    &str_right,
    &str_world,
    &str_people,
    &str_whats,
    &str_up,
    &str_div,
    &str_p,
    &str_brian,
    &str_display,
    &str_flex,
};

bool is_constant_string(ElmString16* s) {
  for (int i = 0; i < sizeof(constant_strings) / sizeof(void*); ++i) {
    if (s == constant_strings[i]) return true;
  }
  return false;
}


void print_string_addresses() {
  printf("%p str_ul\n", &str_ul);
  printf("%p str_li\n", &str_li);
  printf("%p str_color\n", &str_color);
  printf("%p str_red\n", &str_red);
  printf("%p str_blue\n", &str_blue);
  printf("%p str_padding\n", &str_padding);
  printf("%p str_10px\n", &str_10px);
  printf("%p str_hello\n", &str_hello);
  printf("%p str_there\n", &str_there);
  printf("%p str_margin\n", &str_margin);
  printf("%p str_auto\n", &str_auto);
  printf("%p str_float\n", &str_float);
  printf("%p str_left\n", &str_left);
  printf("%p str_right\n", &str_right);
  printf("%p str_world\n", &str_world);
  printf("%p str_people\n", &str_people);
  printf("%p str_whats\n", &str_whats);
  printf("%p str_up\n", &str_up);
  printf("%p str_div\n", &str_div);
  printf("%p str_p\n", &str_p);
  printf("%p str_brian\n", &str_brian);
  printf("%p str_display\n", &str_display);
  printf("%p str_flex\n", &str_flex);
}


static void* view1() {
  return A3(&VirtualDom_node,
      &str_ul,
      pNil,
      List_create(3,
          ((void*[]){
              A3(&VirtualDom_node,
                  &str_li,
                  List_create(2,
                      ((void*[]){
                          A2(&VirtualDom_style, &str_color, &str_red),
                          A2(&VirtualDom_style, &str_padding, &str_10px),
                      })),
                  List_create(2,
                      ((void*[]){
                          A1(&VirtualDom_text, &str_hello),
                          A1(&VirtualDom_text, &str_there),
                      }))),
              A3(&VirtualDom_node,
                  &str_li,
                  List_create(2,
                      ((void*[]){
                          A2(&VirtualDom_style, &str_margin, &str_auto),
                          A2(&VirtualDom_style, &str_float, &str_left),
                      })),
                  List_create(2,
                      ((void*[]){
                          A1(&VirtualDom_text, &str_world),
                          A1(&VirtualDom_text, &str_people),
                      }))),
              A3(&VirtualDom_node,
                  &str_li,
                  List_create(2,
                      ((void*[]){
                          A2(&VirtualDom_style, &str_color, &str_blue),
                          A2(&VirtualDom_style, &str_float, &str_right),
                      })),
                  List_create(2,
                      ((void*[]){
                          A1(&VirtualDom_text, &str_whats),
                          A1(&VirtualDom_text, &str_up),
                      }))),
          })));
}


static void* view2() {
  return A3(&VirtualDom_node,
      &str_div,
      newCons(A2(&VirtualDom_style, &str_display, &str_flex), pNil),
      newCons(A3(&VirtualDom_node, &str_p, pNil, newCons(A1(&VirtualDom_text, &str_brian), pNil)), pNil));
}


static void* view3(int len) {
  Cons* childList = pNil;
  for (int i = 0; i < len; ++i) {
    ElmInt* num = newElmInt(i);
    ElmString16* num_str = A1(&String_fromNumber, num);
    void* text = A1(&VirtualDom_text, num_str);
    void* fact = A2(&VirtualDom_style, &str_margin, num_str);
    void* child = A3(&VirtualDom_node, &str_li, newCons(fact, pNil), newCons(text, pNil));
    childList = newCons(child, childList);
  }

  return A3(&VirtualDom_node, &str_ul, pNil, childList);
}


int main() {
  const size_t N_FLAG_BITS = sizeof(VdomFlags) * 8;
  assert(N_FLAG_BITS == VDOM_BUCKETS_PER_PAGE);

  int maybe_exit = GC_init();
  if (maybe_exit) return maybe_exit;

  init_vdom_allocator();
  print_string_addresses();

  printf("\nBEFORE ANY VIEW\n\n");
  print_vdom_state();

  state.vdom_current = view1();

  printf("\nAFTER FIRST VIEW\n\n");
  print_vdom_state();

  printf("\nSWITCH TO GENERATION 1\n\n");
  next_generation();
  print_vdom_state();

  state.vdom_current = view2();

  printf("\nAFTER SECOND VIEW\n\n");
  print_vdom_state();

  printf("\nSWITCH TO GENERATION 0\n\n");
  next_generation();
  print_vdom_state();

  state.vdom_current = view2();

  printf("\nAFTER THIRD VIEW\n\n");
  print_vdom_state();

  printf("\nSWITCH TO GENERATION 1\n\n");
  next_generation();
  print_vdom_state();

  state.vdom_current = view1();

  printf("\nAFTER FOURTH VIEW\n\n");
  print_vdom_state();

  printf("\nSWITCH TO GENERATION 0\n\n");
  next_generation();
  print_vdom_state();

  state.vdom_current = view3(100);

  printf("\nAFTER BIG VIEW\n\n");
  print_vdom_state();

  print_heap();

  printf("\n\nvdom_old:\n\n");
  print_node_as_html(state.vdom_old);

  printf("\n\nvdom_current:\n\n");
  print_node_as_html(state.vdom_current);

  return 0;
}
