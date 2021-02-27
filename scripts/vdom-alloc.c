#include "../src/kernel/core/gc/internals.h"
#include "../src/kernel/kernel.h"

enum vdom_ctor {
  VDOM_NODE,           // values: tag, facts, children
  VDOM_NODE_KEYED,     // values: tag, keys, facts, children
  VDOM_NODE_NS,        // values: namespace, tag, facts, children
  VDOM_NODE_NS_KEYED,  // values: namespace, tag, keys, facts, children
  VDOM_NODE_TEXT,      // values: content
  VDOM_NODE_TAGGER,    // values: tagger, child
  VDOM_NODE_THUNK,     // values: thunk, refs, node
  // ------------
  VDOM_FACT_EVENT,     // value : handler
  VDOM_FACT_STYLE,     // value : String
  VDOM_FACT_PROP,      // value : Json
  VDOM_FACT_ATTR,      // value : String
  VDOM_FACT_ATTR_NS,   // value : (String, String)
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

#define VDOM_BATCH_BYTES 4096
#define VDOM_BATCH_PER_CHUNK (GC_SYSTEM_MEM_CHUNK / VDOM_BATCH_BYTES)
#define VDOM_BATCH_WORDS (VDOM_BATCH_BYTES / sizeof(size_t))

typedef u16 VdomFlags;  // number of bits must be at least VDOM_BATCH_PER_CHUNK

#define VDOM_CHUNK_WORDS \
  ((GC_SYSTEM_MEM_CHUNK - 2 * sizeof(VdomFlags) - 2 * sizeof(void*)) / sizeof(size_t))

struct vdom_chunk {
  struct vdom_chunk* next;
  VdomFlags live_flags;
  VdomFlags generation_flags;
  size_t words[VDOM_CHUNK_WORDS];
};

struct vdom_state {
  struct vdom_chunk* first_chunk;
  struct vdom_chunk* current_chunk;
  struct vdom_node* vdom_old;
  struct vdom_node* vdom_current;
  size_t* next_node;
  size_t* next_fact;
  size_t* bottom_node;
  size_t* bottom_fact;
  bool generation;
};

static struct vdom_state state;


#ifdef DEBUG
static clear_dead_batches(struct vdom_chunk* chunk) {
  VdomFlags bit = 1;
  for (size_t i = 0; i < VDOM_CHUNK_WORDS; i += VDOM_BATCH_WORDS, bit <<= 1) {
    if (chunk->live_flags & bit) {
      continue;
    } else {
      for (size_t j = i; j < i + VDOM_BATCH_WORDS; ++j) {
        chunk->words[j] = 0;
      }
    }
  }
}
#else
#define clear_dead_batches(x)
#endif


static void next_generation() {
  VdomFlags generation = state.generation ? (VdomFlags)(-1) : 0;
  for (struct vdom_chunk* chunk = state.first_chunk; chunk->next; chunk = chunk->next) {
    chunk->live_flags &= (chunk->generation_flags ^ generation);
    clear_dead_batches(chunk);
  }
  state.generation = !state.generation;
  state.vdom_old = state.vdom_current;
}


void init_vdom_allocator() {
  struct vdom_chunk* first_chunk = GC_get_memory_from_system(GC_SYSTEM_MEM_CHUNK);
  first_chunk->next = NULL;
  first_chunk->live_flags = 0;
  first_chunk->generation_flags = 0;

  state = (struct vdom_state){
      .first_chunk = first_chunk,
      .current_chunk = first_chunk,
      .vdom_old = NULL,
      .vdom_current = NULL,
      .next_node = first_chunk->words[VDOM_CHUNK_WORDS - 1],
      .next_fact = first_chunk->words[VDOM_CHUNK_WORDS - VDOM_BATCH_WORDS - 1],
      .bottom_node = first_chunk->words[VDOM_CHUNK_WORDS - VDOM_BATCH_WORDS],
      .bottom_fact = first_chunk->words[VDOM_CHUNK_WORDS - 2 * VDOM_BATCH_WORDS],
      .generation = 0,
  };
}


void start_new_batch(size_t** top, size_t** bottom) {
  struct vdom_chunk* chunk = state.current_chunk;
  VdomFlags bit = 1 << 63;
  size_t* found = NULL;
  for (size_t i = VDOM_CHUNK_WORDS - 1; bit; i -= VDOM_BATCH_WORDS, bit >>= 1) {
    if (chunk->live_flags & bit) continue;
    found = &chunk->words[i];
    break;
  }
  assert(found);  // TODO
  *top = found;
  *bottom = found - VDOM_BATCH_WORDS;
  if (*bottom < chunk->words) {
    *bottom = chunk->words;
  }
}


size_t* start_new_node_batch() {
  start_new_batch(&state.next_node, &state.bottom_node);
  return state.next_node;
}


size_t* start_new_fact_batch() {
  start_new_batch(&state.next_fact, &state.bottom_fact);
  return state.next_fact;
}


size_t* allocate_node(size_t words) {
  size_t* allocated = state.next_node;
  state.next_node -= words;
  if (state.next_node < state.bottom_node) {
    allocated = start_new_node_batch();
    state.bottom_node = 123;
    state.next_node = allocated;  // something something I dunno
  }
  return allocated;
}


size_t* allocate_fact() {
  size_t* allocated = state.next_fact;
  state.next_fact -= 3;
  if (state.next_fact < state.bottom_fact) {
    allocated = start_new_fact_batch();
    state.bottom_fact = 123;
    state.next_fact = allocated;  // something something I dunno
  }
  return allocated;
}


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


size_t prepend_list_or_start_new_batch(Cons* list) {
  size_t n = 0;
  for (; list != pNil; list = list->tail) {
    n++;
    *state.next_node-- = list->head;
    if (state.next_node < state.bottom_node) {
      start_new_node_batch();
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
    size_t n_children = prepend_list_or_start_new_batch(kidList);
    if (n_children == -1) continue;

    size_t n_facts = prepend_list_or_start_new_batch(factList);
    if (n_facts == -1) continue;

    if (state.next_node - 2 < state.bottom_node) {
      start_new_node_batch();
      continue;
    }

    *state.next_node-- = (size_t*)tag;
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


void* eval_VirtualDom_style(void* args[]) {
  ElmString16* key = args[0];
  ElmString16* value = args[1];
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


void main() {
  const size_t N_FLAG_BITS = sizeof(VdomFlags) * 8;
  assert(N_FLAG_BITS == VDOM_BATCH_PER_CHUNK);

}
