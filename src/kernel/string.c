#include <stdlib.h>
#include <string.h>
#include "types.h"


// local utility function
size_t String_bytes(ElmString *s) {
    size_t total_bytes = (size_t)(s->header.size * SIZE_UNIT);
    u8* struct_bytes = (u8*)s;
    u8 last_byte = struct_bytes[total_bytes-1];
    size_t len = (total_bytes - sizeof(Header)) - (size_t)(last_byte+1);
    return len;
}


void* String_append_eval(void* args[2]) {
    ElmString* a = args[0];
    ElmString* b = args[1];

    u32 na = String_bytes(a);
    u32 nb = String_bytes(b);

    ElmString* s = newElmString(na + nb, NULL);
    if (s == pGcFull) return pGcFull;
    memcpy(s->bytes, a->bytes, na);
    memcpy(&s->bytes[na], b->bytes, nb);

    return s;
}
Closure String_append;


void String_init() {
    String_append = F2(String_append_eval);
}
