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
  VDOM_FACT_EVENT,    // value : handler
  VDOM_FACT_STYLE,    // value : String
  VDOM_FACT_PROP,     // value : Json
  VDOM_FACT_ATTR,     // value : String
  VDOM_FACT_ATTR_NS,  // value : (String, String)
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

struct vdom_chunk_metadata {
  struct vdom_chunk* next;
  VdomFlags live_flags;
  VdomFlags generation_flags;
};

#define VDOM_CHUNK_WORDS ((GC_SYSTEM_MEM_CHUNK - sizeof(struct vdom_chunk_metadata)) / sizeof(size_t))

struct vdom_chunk {
  size_t words[VDOM_CHUNK_WORDS];
  struct vdom_chunk_metadata meta;
};

struct vdom_state {
  struct vdom_node* vdom_old;
  struct vdom_node* vdom_current;
  struct vdom_chunk* first_chunk;
  struct vdom_chunk* current_chunk;
  size_t* next_node;
  size_t* bottom_node;
  size_t* next_fact;
  size_t* bottom_fact;
  bool generation;
};

static struct vdom_state state;


#ifdef DEBUG
static clear_dead_batches(struct vdom_chunk* chunk) {
  VdomFlags bit = 1;
  for (size_t i = 0; i < VDOM_CHUNK_WORDS; i += VDOM_BATCH_WORDS, bit <<= 1) {
    if (chunk->meta.live_flags & bit) continue;
    for (size_t j = i; j < i + VDOM_BATCH_WORDS; ++j) {
      chunk->words[j] = 0;
    }
  }
}
#else
#define clear_dead_batches(x)
#endif


void init_vdom_allocator() {
  struct vdom_chunk* chunk = GC_get_memory_from_system(GC_SYSTEM_MEM_CHUNK);
  chunk->meta.next = NULL;
  chunk->meta.live_flags = 3 << (VDOM_BATCH_PER_CHUNK - 2);
  chunk->meta.generation_flags = 0;

  state = (struct vdom_state){
      .first_chunk = chunk,
      .current_chunk = chunk,
      .vdom_old = NULL,
      .vdom_current = NULL,
      .next_node = &chunk->words[VDOM_CHUNK_WORDS - 1],
      .bottom_node = &chunk->words[VDOM_CHUNK_WORDS - VDOM_BATCH_WORDS],
      .next_fact = &chunk->words[VDOM_CHUNK_WORDS - VDOM_BATCH_WORDS - 1],
      .bottom_fact = &chunk->words[VDOM_CHUNK_WORDS - 2 * VDOM_BATCH_WORDS],
      .generation = 0,
  };
}


static void next_generation() {
  VdomFlags generation = state.generation ? (VdomFlags)(-1) : 0;
  for (struct vdom_chunk* c = state.first_chunk; c; c = c->meta.next) {
    c->meta.live_flags &= (c->meta.generation_flags ^ generation);
    clear_dead_batches(c);
  }
  state.generation = !state.generation;
  state.vdom_old = state.vdom_current;
}


void start_new_batch(size_t** top, size_t** bottom) {
  struct vdom_chunk* chunk = state.current_chunk;
  VdomFlags bit = 1 << (VDOM_BATCH_PER_CHUNK - 1);
  size_t* found = NULL;
  for (size_t i = VDOM_CHUNK_WORDS - 1; bit; i -= VDOM_BATCH_WORDS, bit >>= 1) {
    if (chunk->meta.live_flags & bit) continue;
    found = &chunk->words[i];
    chunk->meta.live_flags |= bit;
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


void* allocate_node(size_t words) {
  size_t* allocated = state.next_node - (words - 1);
  state.next_node = allocated - 1;
  if (state.next_node < state.bottom_node) {
    state.next_node = start_new_node_batch();
    allocated = state.next_node - (words - 1);
    state.next_node = allocated - 1;
  }
  return allocated;
}

void* allocate_fact() {
  size_t words = 3;
  size_t* allocated = state.next_fact - (words - 1);
  state.next_fact = allocated - 1;
  if (state.next_fact < state.bottom_fact) {
    state.next_fact = start_new_fact_batch();
    allocated = state.next_fact - (words - 1);
    state.next_fact = allocated - 1;
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
    *state.next_node-- = (size_t)list->head;
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


static void print_string(ElmString16* s) {
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


static void print_addr_and_value(size_t* p) {
  printf("    %p " FORMAT_HEX "\n", p, *p);
}

static void print_vdom_chunk(struct vdom_chunk* chunk) {
  printf("chunk at %p\n", chunk);
  printf("  next             %p\n", chunk->meta.next);
  printf("  live_flags       0x%04x\n", chunk->meta.live_flags);
  printf("  generation_flags 0x%04x\n", chunk->meta.generation_flags);

  VdomFlags bit = 1;
  for (size_t i = 0; i < VDOM_CHUNK_WORDS; i += VDOM_BATCH_WORDS, bit <<= 1) {
    bool live = chunk->meta.live_flags & bit;
    printf("  Batch at %p live=%x generation=%x\n", &chunk->words[i], live, chunk->meta.generation_flags & bit);
    if (!live) continue;
    bool skip = true;
    for (size_t j = i; j < i + VDOM_BATCH_WORDS && j < VDOM_CHUNK_WORDS; ++j) {
      size_t* p = &chunk->words[j];
      if (*p) skip = false;
      if (skip) continue;
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
          // assert(false);
          break;
      }
    }
  }
}

static void print_vdom_state() {
  printf("\n");
  printf("vdom_old\t%p\n", state.vdom_old);
  printf("vdom_current\t%p\n", state.vdom_current);
  printf("first_chunk\t%p\n", state.first_chunk);
  printf("current_chunk\t%p\n", state.current_chunk);
  printf("next_node\t%p\n", state.next_node);
  printf("bottom_node\t%p\n", state.bottom_node);
  printf("next_fact\t%p\n", state.next_fact);
  printf("bottom_fact\t%p\n", state.bottom_fact);
  printf("generation\t%d\n", state.generation);
  printf("\n");

  for (struct vdom_chunk* c = state.first_chunk; c; c = c->meta.next) {
    print_vdom_chunk(c);
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
}


static void* view() {
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


int main() {
  const size_t N_FLAG_BITS = sizeof(VdomFlags) * 8;
  assert(N_FLAG_BITS == VDOM_BATCH_PER_CHUNK);

  int maybe_exit = GC_init();
  if (maybe_exit) return maybe_exit;

  init_vdom_allocator();
  print_string_addresses();
  print_vdom_state();

  state.vdom_current = view();


  print_heap();
  print_vdom_state();

  print_node_as_html(state.vdom_current);

  return 0;
}
