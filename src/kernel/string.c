#include <stdlib.h>
#include <string.h>
#include "types.h"


// local utility function
static u32 String_bytes(ElmString *s) {
    u32 n_bytes_padded = s->header.size * SIZE_UNIT;
    u32 padding = 1 + s->bytes[n_bytes_padded-1];
    return n_bytes_padded - padding;
}


void* String_append_eval(void* args[2]) {
    ElmString* a = args[0];
    ElmString* b = args[1];

    u32 na = String_bytes(a);
    u32 nb = String_bytes(b);

    ElmString* s = newElmString(na + nb, NULL);
    memcpy(s->bytes, a->bytes, na);
    memcpy(&s->bytes[na], b->bytes, nb);

    return s;
}
Closure String_append;


void String_init() {
    String_append = F2(String_append_eval);
}
