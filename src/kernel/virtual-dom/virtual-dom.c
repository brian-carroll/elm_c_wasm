#include <stdarg.h> // varargs
#include <stdio.h>
#include "virtual-dom.h"


struct vdom_state vdom_state;


/* ==============================================================================

                            MEMORY ALLOCATION

============================================================================== */


static void clear_dead_buckets(struct vdom_page* page) {
  VdomFlags bit = 1;
  for (size_t i = 0; i < VDOM_PAGE_WORDS; i += VDOM_BUCKET_WORDS, bit <<= 1) {
    if (page->meta.live_flags & bit) continue;
    for (size_t j = i; j < i + VDOM_BUCKET_WORDS && j < VDOM_PAGE_WORDS; ++j) {
      page->words[j] = 0;
    }
  }
}


static size_t* start_new_node_bucket();
static size_t* start_new_fact_bucket();


void init_vdom_allocator() {
  struct vdom_page* page = GC_get_memory_from_system(GC_WASM_PAGE_BYTES);
  page->meta.next = NULL;
  page->meta.live_flags = 0;
  page->meta.generation_flags = 0;

  vdom_state.first_page = page;
  vdom_state.current_page = page;
  start_new_node_bucket();
  start_new_fact_bucket();
}


void next_generation() {
  vdom_state.generation = !vdom_state.generation;
  vdom_state.vdom_old = vdom_state.vdom_current;
  vdom_state.vdom_current = NULL;

  VdomFlags generation = vdom_state.generation ? (VdomFlags)(-1) : 0;
  for (struct vdom_page* c = vdom_state.first_page; c; c = c->meta.next) {
    c->meta.live_flags &= (c->meta.generation_flags ^ generation);
    c->meta.live_flags &= ~c->meta.patch_flags;
    c->meta.patch_flags = 0;
    clear_dead_buckets(c);
  }
  start_new_node_bucket();
  start_new_fact_bucket();
}


static void start_new_bucket(size_t** top, size_t** bottom, bool is_patch_bucket) {
  struct vdom_page* page = vdom_state.current_page;

  VdomFlags bit = 1 << (VDOM_BUCKETS_PER_PAGE - 1);
  size_t i = (VDOM_BUCKETS_PER_PAGE - 1) * VDOM_BUCKET_WORDS;

  for (; page->meta.live_flags & bit; bit >>= 1, i -= VDOM_BUCKET_WORDS)
    ;
  assert(bit);  // TODO, allocate new page
  size_t* found = &page->words[i];
  page->meta.live_flags |= bit;
  if (is_patch_bucket) {
    page->meta.patch_flags |= bit;
  }
  if (vdom_state.generation) {
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


static size_t* start_new_node_bucket() {
  start_new_bucket(&vdom_state.next_node, &vdom_state.bottom_node, false);
  return vdom_state.next_node;
}


static size_t* start_new_fact_bucket() {
  start_new_bucket(&vdom_state.next_fact, &vdom_state.bottom_fact, false);
  return vdom_state.next_fact;
}


static size_t* start_new_patch_bucket() {
  start_new_bucket(&vdom_state.top_patch, &vdom_state.next_patch, true);
  return vdom_state.next_patch;
}


// Allocate from top down
// Tends to make diffing very cache-friendly, since
// allocation order is usually the opposite of traversal order
static void* allocate_node(size_t words) {
  size_t* allocated = vdom_state.next_node - (words - 1);
  vdom_state.next_node = allocated - 1;
  if (vdom_state.next_node < vdom_state.bottom_node) {
    vdom_state.next_node = start_new_node_bucket();
    allocated = vdom_state.next_node - (words - 1);
    vdom_state.next_node = allocated - 1;
  }
  return allocated;
}


// Allocate from top down
// Tends to make diffing very cache-friendly, since
// allocation order is usually the opposite of traversal order
static void* allocate_fact() {
  size_t words = 3;
  size_t* allocated = vdom_state.next_fact - (words - 1);
  vdom_state.next_fact = allocated - 1;
  if (vdom_state.next_fact < vdom_state.bottom_fact) {
    vdom_state.next_fact = start_new_fact_bucket();
    allocated = vdom_state.next_fact - (words - 1);
    vdom_state.next_fact = allocated - 1;
  }
  return allocated;
}


// Allocate forwards
static void* allocate_patch(size_t words) {
  size_t* allocated = vdom_state.next_patch;
  if (allocated + words + 2 >= vdom_state.top_patch) {
    struct vdom_patch* link = (struct vdom_patch*)vdom_state.next_patch;
    link->ctor = VDOM_PATCH_LINK;
    link->number = 1;
    allocated = start_new_patch_bucket();
    link->values[0] = allocated;
  }
  vdom_state.next_patch += words;
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
static size_t prepend_list_or_start_new_bucket(Cons* list) {
  size_t n = 0;
  for (; list != pNil; list = list->tail) {
    n++;
    *vdom_state.next_node = (size_t)list->head;
    vdom_state.next_node--;
    if (vdom_state.next_node < vdom_state.bottom_node) {
      printf("prepend_list_or_start_new_bucket: overflowed at %p\n", vdom_state.bottom_node);
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

    if (vdom_state.next_node - 2 < vdom_state.bottom_node) {
      printf("eval_VirtualDom_node: overflowed at %p\n", vdom_state.bottom_node);
      start_new_node_bucket();
      continue;
    }

    *vdom_state.next_node-- = (size_t)tag;
    struct vdom_node* node = (struct vdom_node*)vdom_state.next_node--;

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

void* eval_VirtualDom_attribute(void* args[]) {
  ElmString16* key = args[0];
  ElmString16* value = args[1];
  assert(key->header.tag == Tag_String);
  assert(value->header.tag == Tag_String);
  struct vdom_fact* fact = allocate_fact();
  *fact = (struct vdom_fact){
      .ctor = VDOM_FACT_ATTR,
      .key = key,
      .value = value,
  };
  return fact;
}
Closure VirtualDom_attribute = {
    .header = HEADER_CLOSURE(0),
    .evaluator = eval_VirtualDom_attribute,
    .max_values = 2,
};

/* ==============================================================================

                                  DIFF

============================================================================== */

static struct vdom_patch* create_patch(u8 ctor, u32 nValues, ...) {
  va_list args;
  struct vdom_patch* patch = allocate_patch(1 + nValues);
  patch->ctor = ctor;
  patch->number = nValues;
  va_start(args, nValues);
  for (u32 i = 0; i < nValues; i++) {
    patch->values[i] = va_arg(args, void*);
  }
  va_end(args);
  return patch;
}

static struct vdom_patch* create_patch_from_array(u8 ctor, u32 nValues, void* values[]) {
  struct vdom_patch* patch = allocate_patch(1 + nValues);
  patch->ctor = ctor;
  patch->number = nValues;
  for (u32 i = 0; i < nValues; i++) {
    patch->values[i] = values[i];
  }
  return patch;
}


static bool strings_match(ElmString16* x, ElmString16* y) {
  if (x == y) return true;

  GcHeap* heap = &gc_state.heap;
  if (IS_OUTSIDE_HEAP(x) && IS_OUTSIDE_HEAP(y)) {
    // Both are string literals (like "div" or "color").
    // Constants are deduped by the compiler, so different addresses => different values.
    return false;
  }

  // At least one string is dynamically allocated. Need slower full check.
  // Compare in 32-bit chunks for efficiency. Header contains length.
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
  struct vdom_fact** oldFacts = (struct vdom_fact**)oldNode->values + oldNode->n_extras;
  struct vdom_fact** newFacts = (struct vdom_fact**)newNode->values + newNode->n_extras;

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

    if (newFact->ctor == VDOM_FACT_ATTR_NS) {
      Tuple2* pair = newFact->value;
      ElmString16* namespace = pair->a;
      void* value = pair->b;
      create_patch(VDOM_PATCH_SET_ATTR_NS, 3, namespace, newFact->key, value);
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
      create_patch(ctor, 2, newFact->key, newFact->value);
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
    if (oldFact->ctor == VDOM_FACT_ATTR_NS) {
      Tuple2* pair = oldFact->value;
      ElmString16* namespace = pair->a;
      create_patch(VDOM_PATCH_REMOVE_ATTR_NS, 2, oldFact->key, namespace);
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
      create_patch(ctor, 1, oldFact->key);
    }
  }
}

static void diffNodes(struct vdom_node* old, struct vdom_node* new);

static void diffChildren(struct vdom_node* oldParent, struct vdom_node* newParent) {
  u8 nOld = oldParent->n_children;
  u8 nNew = newParent->n_children;
  u8 nMin = (nOld < nNew) ? nOld : nNew;
  struct vdom_node** oldChildren = (struct vdom_node**)oldParent->values + oldParent->n_extras + oldParent->n_facts;
  struct vdom_node** newChildren = (struct vdom_node**)newParent->values + newParent->n_extras + newParent->n_facts;
  struct vdom_patch* push = allocate_patch(1);
  struct vdom_patch* pop = NULL;

  for (u8 i = 0; i < nMin; ++i) {
    size_t* beforeChild = vdom_state.next_patch;

    struct vdom_node* oldChild = oldChildren[i];
    struct vdom_node* newChild = newChildren[i];
    diffNodes(oldChild, newChild);

    size_t* afterChild = vdom_state.next_patch;
    if (afterChild != beforeChild) {
      push->ctor = VDOM_PATCH_PUSH;
      push->number = nOld - 1 - i;
      pop = create_patch(VDOM_PATCH_POP, 0);
      push = allocate_patch(1);
    }
  }

  // Deallocate the last push. We always allocate one too many.
  vdom_state.next_patch = push;

  if (nNew > nOld) {
    create_patch_from_array(VDOM_PATCH_APPEND, nNew - nOld, (void**)&newChildren[nOld]);
  }

  if (nOld > nNew) {
    struct vdom_patch* patch = create_patch(VDOM_PATCH_REMOVE_LAST, 0);
    patch->number = nOld - nNew;
  }
}


static void diffNodes(struct vdom_node* old, struct vdom_node* new) {
  if (!old || (old->ctor != new->ctor)) {
    create_patch(VDOM_PATCH_REDRAW, 1, new);
    return;
  }
  switch (new->ctor) {
    case VDOM_NODE: {
      ElmString16* currTag = old->values[0];
      ElmString16* nextTag = new->values[0];
      if (!strings_match(currTag, nextTag)) {
        create_patch(VDOM_PATCH_REDRAW, 1, new);
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
        create_patch(VDOM_PATCH_REDRAW, 1, new);
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
static void* eval_VirtualDom_diff(void* args[]) {
  struct vdom_node* currNode = args[0];
  struct vdom_node* nextNode = args[1];
  start_new_patch_bucket();
  struct vdom_patch* first_patch = (struct vdom_patch*)vdom_state.next_patch;

  diffNodes(currNode, nextNode);

  // Mark the end of the patches. (We have at least 2 words of space)
  struct vdom_patch* end_marker = (struct vdom_patch*)vdom_state.next_patch;
  end_marker->ctor = VDOM_PATCH_END;
  end_marker->number = 0;

  return first_patch;
}
Closure VirtualDom_diff = {
    .header = HEADER_CLOSURE(2),
    .max_values = 2,
    .evaluator = eval_VirtualDom_diff,
};
