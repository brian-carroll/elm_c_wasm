#include "../kernel/kernel.h"
#include "test.h"
#include <stdio.h>

extern struct vdom_state vdom_state;

// forward declare some internal package functions
void init_vdom_allocator();
void next_generation();


/* ==============================================================================

                                  TEST CODE

============================================================================== */


static char* stringify_vdom_ctor(u8 ctor) {
  switch (ctor) {
    // clang-format off
    case VDOM_NODE:                 return "VDOM_NODE";
    case VDOM_NODE_KEYED:           return "VDOM_NODE_KEYED";
    case VDOM_NODE_NS:              return "VDOM_NODE_NS";
    case VDOM_NODE_NS_KEYED:        return "VDOM_NODE_NS_KEYED";
    case VDOM_NODE_TEXT:            return "VDOM_NODE_TEXT";
    case VDOM_NODE_TAGGER:          return "VDOM_NODE_TAGGER";
    case VDOM_NODE_THUNK:           return "VDOM_NODE_THUNK";
    case VDOM_FACT_EVENT:           return "VDOM_FACT_EVENT";
    case VDOM_FACT_STYLE:           return "VDOM_FACT_STYLE";
    case VDOM_FACT_PROP:            return "VDOM_FACT_PROP";
    case VDOM_FACT_ATTR:            return "VDOM_FACT_ATTR";
    case VDOM_FACT_ATTR_NS:         return "VDOM_FACT_ATTR_NS";
    case VDOM_PATCH_PUSH:           return "VDOM_PATCH_PUSH";
    case VDOM_PATCH_POP:            return "VDOM_PATCH_POP";
    case VDOM_PATCH_LINK:           return "VDOM_PATCH_LINK";
    case VDOM_PATCH_NO_OP:          return "VDOM_PATCH_NO_OP";
    case VDOM_PATCH_END:            return "VDOM_PATCH_END";
    case VDOM_PATCH_REDRAW:         return "VDOM_PATCH_REDRAW";
    case VDOM_PATCH_SET_EVENT:      return "VDOM_PATCH_SET_EVENT";
    case VDOM_PATCH_SET_STYLE:      return "VDOM_PATCH_SET_STYLE";
    case VDOM_PATCH_SET_PROP:       return "VDOM_PATCH_SET_PROP";
    case VDOM_PATCH_SET_ATTR:       return "VDOM_PATCH_SET_ATTR";
    case VDOM_PATCH_SET_ATTR_NS:    return "VDOM_PATCH_SET_ATTR_NS";
    case VDOM_PATCH_REMOVE_EVENT:   return "VDOM_PATCH_REMOVE_EVENT";
    case VDOM_PATCH_REMOVE_STYLE:   return "VDOM_PATCH_REMOVE_STYLE";
    case VDOM_PATCH_REMOVE_PROP:    return "VDOM_PATCH_REMOVE_PROP";
    case VDOM_PATCH_REMOVE_ATTR:    return "VDOM_PATCH_REMOVE_ATTR";
    case VDOM_PATCH_REMOVE_ATTR_NS: return "VDOM_PATCH_REMOVE_ATTR_NS";
    case VDOM_PATCH_TEXT:           return "VDOM_PATCH_TEXT";
    case VDOM_PATCH_TAGGER:         return "VDOM_PATCH_TAGGER";
    case VDOM_PATCH_REMOVE_LAST:    return "VDOM_PATCH_REMOVE_LAST";
    case VDOM_PATCH_APPEND:         return "VDOM_PATCH_APPEND";
    case VDOM_PATCH_REMOVE:         return "VDOM_PATCH_REMOVE";
    case VDOM_PATCH_REORDER:        return "VDOM_PATCH_REORDER";
    // clang-format on
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
  for (struct vdom_page* c = vdom_state.first_page; c; c = c->meta.next) {
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
  if (!node) {
    printf("(nil)\n");
    return;
  }
  printf("    %p " FORMAT_HEX " %s ", node, *(size_t*)node, stringify_vdom_ctor(node->ctor));
  if (node->ctor == VDOM_NODE) {
    printf("<");
    print_string(node->values[0]);
    printf("> ");
  }
  printf("n_extras=%d n_facts=%d n_children=%d", node->n_extras, node->n_facts, node->n_children);
  int n_values = node->n_extras + node->n_facts + node->n_children;
  for (int i = 0; i < n_values; ++i) {
    printf(" %p", node->values[i]);
  }
  printf("\n");
}


static u32 print_vdom_patch_header(struct vdom_patch* patch) {
  assert(patch);
  printf("    %p " FORMAT_HEX " %s number=%d", patch, *(size_t*)patch, stringify_vdom_ctor(patch->ctor), patch->number);
  u32 n_values;
  switch (patch->ctor) {
    case VDOM_PATCH_PUSH:
    case VDOM_PATCH_REMOVE_LAST:
      n_values = 0;
      break;
    default:
      n_values = patch->number;
      break;
  }
  for (int i = 0; i < n_values; ++i) {
    printf(" %p", patch->values[i]);
  }
  printf("\n");
  return n_values;
}


static void print_addr_and_value(void* p) {
  printf("    %p " FORMAT_HEX "\n", p, *(size_t*)p);
}

static void print_vdom_page(struct vdom_page* page) {
  printf("Page at %p\n", page);
  printf("  next             %p\n", page->meta.next);
  printf("  live_flags       0x%04x\n", page->meta.live_flags);
  printf("  generation_flags 0x%04x\n", page->meta.generation_flags);
  printf("  patch_flags      0x%04x\n", page->meta.patch_flags);

  VdomFlags bit = 1;
  for (size_t i = 0; i < VDOM_PAGE_WORDS; i += VDOM_BUCKET_WORDS, bit <<= 1) {
    bool live = !!(page->meta.live_flags & bit);
    bool gen = !!(page->meta.generation_flags & bit);
    bool patches = !!(page->meta.patch_flags & bit);
    printf("  Bucket at %p (%04x) live=%x generation=%x patches=%x\n", &page->words[i], bit, live, gen, patches);
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
        case 0:
          continue;
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
        case VDOM_PATCH_PUSH:
        case VDOM_PATCH_POP:
        case VDOM_PATCH_LINK:
        case VDOM_PATCH_NO_OP:
        case VDOM_PATCH_END:
        case VDOM_PATCH_REDRAW:
        case VDOM_PATCH_SET_EVENT:
        case VDOM_PATCH_SET_STYLE:
        case VDOM_PATCH_SET_PROP:
        case VDOM_PATCH_SET_ATTR:
        case VDOM_PATCH_SET_ATTR_NS:
        case VDOM_PATCH_REMOVE_EVENT:
        case VDOM_PATCH_REMOVE_STYLE:
        case VDOM_PATCH_REMOVE_PROP:
        case VDOM_PATCH_REMOVE_ATTR:
        case VDOM_PATCH_REMOVE_ATTR_NS:
        case VDOM_PATCH_TEXT:
        case VDOM_PATCH_TAGGER:
        case VDOM_PATCH_REMOVE_LAST:
        case VDOM_PATCH_APPEND:
        case VDOM_PATCH_REMOVE:
        case VDOM_PATCH_REORDER: {
          struct vdom_patch* patch = (struct vdom_patch*)p;
          u32 n_values = print_vdom_patch_header(patch);
          for (size_t k = 0; k < n_values; ++k) {
            print_addr_and_value(&patch->values[k]);
          }
          j += n_values;
          break;
        }
        default:
          printf("unknown ctor %d at %p\n", ctor, p);
          fflush(0);
          assert(false);
          break;
      }
    }
  }
}

static void print_vdom_state() {
  printf("\n");
  printf("vdom_old      %p\n", vdom_state.vdom_old);
  printf("vdom_current  %p\n", vdom_state.vdom_current);
  printf("first_page    %p\n", vdom_state.first_page);
  printf("current_page  %p\n", vdom_state.current_page);
  printf("next_node     %p\n", vdom_state.next_node);
  printf("bottom_node   %p\n", vdom_state.bottom_node);
  printf("next_fact     %p\n", vdom_state.next_fact);
  printf("bottom_fact   %p\n", vdom_state.bottom_fact);
  printf("next_patch    %p\n", vdom_state.next_patch);
  printf("top_patch     %p\n", vdom_state.top_patch);
  printf("generation    %d\n", vdom_state.generation);
  printf("\n");

  for (struct vdom_page* c = vdom_state.first_page; c; c = c->meta.next) {
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


char* virtual_dom_test() {
  if (verbose) {
    printf("\n\n\n");
    printf("####################################################\n");
    printf("\n");
    printf("Virtual DOM\n");
    printf("-----------\n");
  }

  const size_t N_FLAG_BITS = sizeof(VdomFlags) * 8;
  assert(N_FLAG_BITS == VDOM_BUCKETS_PER_PAGE);

  int maybe_exit = GC_init();
  mu_expect_equal("GC should initialise", maybe_exit, 0);

  init_vdom_allocator();
  print_string_addresses();

  // printf("\nBEFORE ANY VIEW\n\n");
  // print_vdom_state();

  vdom_state.vdom_current = view1();
  A2(&VirtualDom_diff, vdom_state.vdom_old, vdom_state.vdom_current);

  // printf("\nAFTER FIRST VIEW\n\n");
  // print_vdom_state();

  // printf("\nSWITCH TO GENERATION 1\n\n");
  next_generation();
  // print_vdom_state();

  // vdom_state.vdom_current = view2();
  // A2(&VirtualDom_diff, vdom_state.vdom_old, vdom_state.vdom_current);

  // printf("\nAFTER SECOND VIEW\n\n");
  // print_vdom_state();

  // printf("\nSWITCH TO GENERATION 0\n\n");
  // next_generation();
  // print_vdom_state();

  // vdom_state.vdom_current = view2();
  // A2(&VirtualDom_diff, vdom_state.vdom_old, vdom_state.vdom_current);

  // printf("\nAFTER THIRD VIEW\n\n");
  // print_vdom_state();

  // printf("\nSWITCH TO GENERATION 1\n\n");
  // next_generation();
  // print_vdom_state();

  // vdom_state.vdom_current = view1();
  // A2(&VirtualDom_diff, vdom_state.vdom_old, vdom_state.vdom_current);

  // printf("\nAFTER FOURTH VIEW\n\n");
  // print_vdom_state();

  // printf("\nSWITCH TO GENERATION 0\n\n");
  // next_generation();
  // print_vdom_state();

  vdom_state.vdom_current = view3(100);
  A2(&VirtualDom_diff, vdom_state.vdom_old, vdom_state.vdom_current);

  printf("\nAFTER BIG VIEW\n\n");
  print_vdom_state();

  printf("\n\n\n");
  print_heap();

  if (vdom_state.vdom_old) {
    printf("\n\nvdom_old:\n\n");
    print_node_as_html(vdom_state.vdom_old);
  }

  printf("\n\nvdom_current:\n\n");
  print_node_as_html(vdom_state.vdom_current);

  return NULL;
}
