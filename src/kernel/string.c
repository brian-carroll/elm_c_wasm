#include <stdlib.h>
#include "types.h"


ElmInt* String_length_bytes(ElmString *s) {
    u32 n_bytes_padded = s->header.size * SIZE_UNIT;
    u8 padding = s->bytes[n_bytes_padded-1];
    return newElmInt(n_bytes_padded - (u32)padding);
}

void* string_append_eval(void* args[]) {
    return NULL;
}
