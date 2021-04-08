#ifndef JSON_INTERNAL_H
#define JSON_INTERNAL_H

#include "json.h"

void* parse_json(ElmString16* json);

struct string_builder {
  ElmString16* s;
  u16* cursor;
  u16* end;
};
typedef struct string_builder StringBuilder;

void stringify(u32 indent, u32 indent_current, void* p, StringBuilder* sb);
extern size_t stringify_alloc_chunk_words;
#define WRAP(ptr) A1(&Json_wrap, ptr)

#endif
