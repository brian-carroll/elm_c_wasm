#include <stdio.h>
#include "types.h"

void* max_heap;
Header* current_heap;
Header* root;
Header* stack_map;


static void mark_trace(Header* h, void* ignore_below) {
    ElmValue* v = (ElmValue*)h;
    void* first = h;
    void* last = NULL;
    u32 nparams;

    h->gc_mark = GcAlive;

    switch (h->tag) {
        case Tag_Int:
        case Tag_Float:
        case Tag_Char:
        case Tag_String:
        case Tag_Nil:
            break;

        case Tag_Cons:
            first = &v->cons.head;
            last = &v->cons.tail;
            break;

        case Tag_Tuple2:
            first = &v->tuple2.a;
            last = &v->tuple2.b;
            break;

        case Tag_Tuple3:
            first = &v->tuple3.a;
            last = &v->tuple3.c;
            break;

        case Tag_Custom:
            nparams = ((h->size * SIZE_UNIT) - sizeof(u32)) / sizeof(void*);
            if (nparams > 0) {
                first = &v->custom.values[0];
                last = nparams ? &v->custom.values[nparams-1] : NULL;
            }
            break;

        case Tag_Record:
            nparams = v->record.fieldset->size;
            if (nparams > 0) {
                first = &v->record.values[0];
                last = &v->record.values[nparams-1];
            }
            break;

        case Tag_Closure:
            nparams = v->closure.n_values;
            if (nparams > 0) {
                first = &v->closure.values[0];
                last = &v->closure.values[nparams-1];
            }
            break;
    }

    for (void* child = first; child <= last; child += sizeof(void*)) {
        if (child >= ignore_below) { // Old things can't point to newer things
            mark_trace(child, ignore_below);
        }
    }
}

static void clear_marks(Header* h) {
    while (h < current_heap) {
        h->gc_mark = GcDead;
        h += h->size * SIZE_UNIT;
    }
}

static void mark(void* ignore_below) {
    /*
        clear marks above ignore_below

    */
    // loop over all GC roots
}