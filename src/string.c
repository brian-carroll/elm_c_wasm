#include "types.h"

ElmInt* String_length_bytes(ElmString *s) {
    u32 n_bytes_padded = s->header.size * 4;
    u32 padding = (u32)s->bytes[n_bytes_padded-1];
    return newElmInt(n_bytes_padded - padding);
}
