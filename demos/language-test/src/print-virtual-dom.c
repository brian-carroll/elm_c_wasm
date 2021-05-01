/*
Print VirtualDom as text to stdout

Why do this? Because Wasm debuggers are awful, I want to use C debuggers.
You don't get variable names, just var0, var1, var2
Pointers are not dereferenced or typed, they're just address numbers.
*/

#include <stdio.h>
#include <string.h>
#include "../../../src/kernel/kernel.h"

extern Closure VirtualDom_node;
extern Closure VirtualDom_text;
// extern Closure VirtualDom_style;
// extern Closure VirtualDom_property;

bool is_html_tag(char* c_tag, ElmString16* elm_tag) {
  size_t c_len = strlen(c_tag);
  size_t elm_len = code_units(elm_tag);
  if (c_len != elm_len) return false;
  for (size_t i = 0; i < elm_len; ++i) {
    if (c_tag[i] != elm_tag->words16[i]) return false;
  }
  return true;
}


void print_virtual_dom_help(int indent, Closure* vdom) {
  if (vdom->evaluator == VirtualDom_text.evaluator) {
    ElmString16* text = vdom->values[0];
    u32 len = code_units(text);
    for (u32 i = 0; i < len; ++i) {
      u16 w = text->words16[i];
      if (w <= 0xff) putchar(w);
    }
  } else {
    ElmString16* tag = vdom->values[0];
    // Cons* facts = vdom->values[1];
    Cons* children = vdom->values[2];

    bool newline = false;

    if (is_html_tag("h2", tag)) {
      safe_printf("\n## ");
      newline = true;
    } else if (is_html_tag("h3", tag)) {
      safe_printf("### ");
      newline = true;
    } else if (is_html_tag("ul", tag)) {
      indent += 2;
    } else if (is_html_tag("li", tag)) {
      for (int i = 0; i < indent; ++i) {
        putchar(' ');
      }
      safe_printf("- ");
      newline = true;
    } else if (is_html_tag("br", tag)) {
      newline = true;
    }

    for (; children != pNil; children = children->tail) {
      Closure* child = children->head;
      print_virtual_dom_help(indent, child);
    }

    if (newline) putchar('\n');
  }
}

void print_virtual_dom(Closure* vdom) {
  print_virtual_dom_help(-2, vdom);
}
