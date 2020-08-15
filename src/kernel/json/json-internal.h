#ifndef JSON_INTERNAL_H
#define JSON_INTERNAL_H

#include "json.h"

void* parse_json(ElmString16* json);

void* stringify(u32 indent, u32 indent_current, void* p, u16** cursor, u16** end);
extern size_t stringify_alloc_chunk;
#define WRAP(ptr) A1(&Json_wrap, ptr)

#endif
