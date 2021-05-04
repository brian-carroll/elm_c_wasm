/*
API:

Debug.toString
- init
- grow
- ensure_space (amortize a load of end-checks into one)
- copy_ascii
- builderToString

Json.stringify
- init
- grow
- copy_ascii
- write_char (16-bit char, and without for loop)
- write_indent
- builderToString

*/
#include "./gc/internals.h"
u16* String_copy(u16* to, ElmString16* s);

struct string_builder {
  Cons* first_section;
  Cons* last_section;
  u16* cursor;
  u16* end;
  u32 prev_sections_length;
};
typedef struct string_builder StringBuilder;


#define STRING_BUILDER_MIN_SECTION_BYTES 1024


static void StringBuilder_addSection(StringBuilder* sb) {
  Cons* cell = newCons(NULL, &Nil); // allocate BEFORE the string to leave plenty of room
  sb->last_section.tail = cell;

  // Allocate a string at least min size and as large as we can get
  ElmString16* s = newElmString16(STRING_BUILDER_MIN_SECTION_BYTES >> 1);
  size_t* end = gc_state.end_of_alloc_patch;
  s->header.size = end - (size_t*)s;

  cell->head = s;

  ElmString16* prev = sb->last_section.head;
  sb->last_section = cell;
  sb->cursor = s->words16;
  sb->end = (u16*)end;
  if (prev) {
    sb->prev_sections_length += code_units(prev);
  }
}


StringBuilder StringBuilder_create() {
  Cons* tmp = newCons(NULL, &Nil);
  StringBuilder sb = { .last_section = tmp };
  StringBuilder_addSection(&sb);
  sb.first_section = sb.last_section;
  return sb;
}


ElmString16* StringBuilder_toString(StringBuilder* sb) {
  // Shrink the last string to actual used size
  ElmString16* s = sb->last_section.head;
  u32 used_chars = sb->cursor - s->words16;
  s->header.size = SIZE_STRING(used_chars);
  size_t* after_string = (size_t*)s + s->header.size;
  for (u16* zero = sb->cursor; zero < (u16*)after_string; zero++) {
    *zero = 0;
  }

  // Give back unused memory to the allocator
  ptrdiff_t leftover_space = gc_state.end_of_alloc_patch - after_string;
  GC_allocate(false, -leftover_space);

  // Early-out for single section
  if (sb->first_section == sb->last_section) {
    return s;
  }

  // Concatenate the sections
  u32 len = prev_sections_length + used_chars;
  ElmString16* result = newElmString16(len);
  u16* dest = result->words16;
  for (Cons* cell = sb->first_section; cell != pNil; cell = cell->tail) {
    dest = String_copy(dest, cell->head);
  }
  return result;
}
