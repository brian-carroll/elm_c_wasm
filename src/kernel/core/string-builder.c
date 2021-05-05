#include "core.h"
#include "./gc/internals.h"
u16* String_copy(u16* to, ElmString16* s);


#define STRING_BUILDER_MIN_CODE_UNITS 512


void StringBuilder_startSection(StringBuilder* sb, size_t min_code_units) {
  GcState* state = &gc_state;
  Cons* cell = newCons(NULL, &Nil);  // allocate BEFORE the string to leave plenty of room
  sb->last_section->tail = cell;

  // Allocate a string at least min size and as large as we can get
  ElmString16* s = newElmString16(min_code_units);
  size_t* end = state->end_of_alloc_patch;
  size_t extra_space = end - state->next_alloc - 1;
  GC_allocate(false, extra_space);
  s->header.size = end - (size_t*)s;

  cell->head = s;

  sb->last_section = cell;
  sb->cursor = s->words16;
  sb->end = (u16*)end;
}


void StringBuilder_finishSection(StringBuilder* sb) {
  ElmString16* s = sb->last_section->head;
  u32 used_chars = sb->cursor - s->words16;
  s->header.size = SIZE_STRING(used_chars);
  size_t* after_string = (size_t*)s + s->header.size;
  for (u16* zero = sb->cursor; zero < (u16*)after_string; zero++) {
    *zero = 0;
  }

  ptrdiff_t leftover_space = (size_t*)sb->end - after_string - 1;
  GC_allocate(false, -leftover_space);

  sb->finished_sections_length += used_chars;
}


void StringBuilder_init(StringBuilder* sb) {
  sb->last_section = newCons(NULL, &Nil);
  StringBuilder_startSection(sb, STRING_BUILDER_MIN_CODE_UNITS);
  sb->first_section = sb->last_section;
}


ElmString16* StringBuilder_toString(StringBuilder* sb) {
  StringBuilder_finishSection(sb);

  if (sb->first_section == sb->last_section) {
    return sb->first_section->head;
  }

  ElmString16* result = newElmString16(sb->finished_sections_length);
  u16* dest = result->words16;
  for (Cons* cell = sb->first_section; cell != pNil; cell = cell->tail) {
    dest = String_copy(dest, cell->head);
  }
  return result;
}


void StringBuilder_ensureSpace(StringBuilder* sb, size_t need) {
  size_t available = sb->end - sb->cursor;
  if (available < need) {
    StringBuilder_finishSection(sb);
    size_t len =
        need > STRING_BUILDER_MIN_CODE_UNITS ? need : STRING_BUILDER_MIN_CODE_UNITS;
    StringBuilder_startSection(sb, len);
  }
}


void StringBuilder_copyAscii(StringBuilder* sb, char* src) {
  char* from = src;
  u16* to = sb->cursor;
  u16* end = sb->end;

  for (; *from; to++, from++) {
    if (to >= end) {
      StringBuilder_finishSection(sb);
      StringBuilder_startSection(sb, 0);
      to = sb->cursor;
      end = sb->end;
    }
    *to = *from;
  }

  sb->cursor = to;
}


void StringBuilder_writeChar(StringBuilder* sb, char c) {
  if (sb->cursor >= sb->end) {
    StringBuilder_finishSection(sb);
    StringBuilder_startSection(sb, 0);
  }
  *(sb->cursor) = c;
  sb->cursor++;
}


void StringBuilder_writeIndent(StringBuilder* sb, u32 indent_current) {
  if (!indent_current) return;
  StringBuilder_ensureSpace(sb, indent_current);
  u16* to = sb->cursor;
  for (size_t i = 0; i < indent_current; i++) {
    *to++ = ' ';
  }
  sb->cursor = to;
}
