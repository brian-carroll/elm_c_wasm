/*
Print VirtualDom as text to stdout

Why do this? Because Wasm debuggers are awful, I want to use C debuggers.
You don't get variable names, just var0, var1, var2
Pointers are not dereferenced or typed, they're just address numbers.
*/

#include <stdio.h>
#include "../../../src/kernel/kernel.h"

extern Closure VirtualDom_node;
extern Closure VirtualDom_text;
// extern Closure VirtualDom_style;
// extern Closure VirtualDom_property;

extern ElmString16 str_div;
extern ElmString16 str_h2;
extern ElmString16 str_h3;
extern ElmString16 str_ul;
extern ElmString16 str_li;
extern ElmString16 str_br;
extern ElmString16 str_p;
extern ElmString16 str_pre;

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

    if (tag == &str_h2) {
      printf("\n## ");
      newline = true;
    } else if (tag == &str_h3) {
      printf("### ");
      newline = true;
    } else if (tag == &str_ul) {
      indent += 2;
    } else if (tag == &str_li) {
      for (int i = 0; i < indent; ++i) {
        putchar(' ');
      }
      printf("- ");
      newline = true;
    } else if (tag == &str_br) {
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
