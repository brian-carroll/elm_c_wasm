#ifndef JSON_INTERNAL_H
#define JSON_INTERNAL_H

#include "json.h"

void* parse_json(ElmString16* json);

void stringify(u32 indent, u32 indent_current, void* p, StrBuilder* sb);
extern size_t stringify_alloc_chunk_words;
#define WRAP(ptr) A1(&Json_wrap, ptr)

#endif
