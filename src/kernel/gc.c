#include "types.h"

// *IF* I built some kind of Garbage Collector for Wasm MVP,
// then it would need to traverse Elm data structures,
// and a function like this would be needed

int gc_first_pointer_offset(Header header) {
    switch (header.tag) {
        case Tag_Int:
        case Tag_Float:
        case Tag_Char:
        case Tag_String:
        case Tag_Nil:
            return -1; // no pointers to trace

        case Tag_Cons:
        case Tag_Tuple2:
        case Tag_Tuple3:
            #ifdef TARGET_64BIT
                return 8; // header(4) + alignment(4)
            #else
                return 4; // header(4)
            #endif

        case Tag_Custom:
            return 8;   // header(4) + ctor(4)

        case Tag_Record:
            #ifdef TARGET_64BIT
                return 16; // header(4) + alignment(4) + fieldset(8)
            #else
                return 8; // header(4) + fieldset(4)
            #endif

        case Tag_Closure:
            return 8; // header(4) + n_values(2) + max_values(2)
    }
    return 0;
}
