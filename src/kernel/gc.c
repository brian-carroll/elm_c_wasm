#include <stdio.h>
#include "types.h"

// *IF* I built some kind of Garbage Collector for Wasm MVP,
// then it would need to traverse Elm data structures,
// and a function like this would be needed

u32 gc_first_pointer_offset(Header header) {
    switch (header.tag) {
        case Tag_Int:
        case Tag_Float:
        case Tag_Char:
        case Tag_String:
        case Tag_Nil:
            return 0; // no pointers to trace

        case Tag_Cons:
        case Tag_Tuple2:
        case Tag_Tuple3:
            return sizeof(Cons) - 2*sizeof(void*); // header + padding

        case Tag_Custom:
            return sizeof(Custom);

        case Tag_Record:
            return sizeof(Record);

        case Tag_Closure:
            return sizeof(Closure);
    }
    // C compiler wants _any_ int value to be handled (int could be typecast to enum)
    // But I also want warnings about missing cases, so no default.
    // A return value here satisfies both requirements.
    fprintf(stderr, "Unknown type tag: %d\n", header.tag);
    return 0;
}
