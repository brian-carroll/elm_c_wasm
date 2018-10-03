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
            return 0;  // first pointer is directly after header

        case Tag_Custom:
        case Tag_Record:
            return 1;  // skip 1 pointer-sized element (4 bytes)

        case Tag_Closure:
            return 2;  // skip 2 pointer-sized elements (8 bytes)
        
        default:
            return -1;
    }
}
