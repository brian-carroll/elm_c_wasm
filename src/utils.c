#include <string.h>
#include "types.h"

void* apply(Closure* c_old, u8 n_applied, void* applied[]) {
    if (c_old->max_values == n_applied) {
        // avoid allocating a new closure if we don't need it
        return (*c_old->evaluator)(applied);
    } else {
        u8 n_old = c_old->n_values;
        u8 n_new = n_old + n_applied;

        size_t size_old = sizeof(Closure) + n_old * sizeof(void*);
        size_t size_applied = n_applied * sizeof(void*);
        size_t size_new = size_old + size_applied;

        Closure *c = malloc(size_new);
        memcpy(c, c_old, size_old);
        memcpy(&c->values[n_old], applied, size_applied);
        c->header = HEADER_CLOSURE(n_new);
        c->n_values = n_new;

        if (n_new == c->max_values) {
            return (*c->evaluator)(c->values);
        } else {
            return c;
        }
    }
}


// Equality with recursion (and possible stack overflow)
u8* eq(void* ptr_a, void* ptr_b) {
    ElmValue* pa = (ElmValue*)ptr_a;
    ElmValue* pb = (ElmValue*)ptr_b;

    // Reference equality shortcut
    if (pa == pb) {
        return &True;
    }

    // Bool and Unit don't have headers, so check them first
    u8 *pa_const = &pa->unit_or_bool;
    if (pa_const == &True || pa_const == &False || pa_const == &Unit) {
        // Wouldn't get here if they were equal. It would be reference equality.
        return &False;
    }

    Header ha = *(Header*)pa;
    Header hb = *(Header*)pb;
    switch (ha.tag) {
        case Tag_Int:
            return pa->elm_int.value == pb->elm_int.value
                ? &True : &False;

        case Tag_Float:
            return pa->elm_float.value == pb->elm_float.value
                ? &True : &False;

        case Tag_Char:
            return pa->elm_char.value == pb->elm_char.value
                ? &True : &False;

        case Tag_String: {
            if (ha.size != hb.size) {
                return &False;
            }
            // Cast array of bytes to array of ints, for speed
            u32* a32 = (u32*)pa->elm_string.bytes;
            u32* b32 = (u32*)pb->elm_string.bytes;
            for (int i=0; i < ha.size; i++) {
                if (a32[i] != b32[i]) {
                    return &False;
                }
            }
            return &True;
        }            

        case Tag_Nil:
            return &False; // not ref equal, so can't both be Nil

        case Tag_Cons:
            return eq(pa->cons.head, pb->cons.head)
                && eq(pa->cons.tail, pb->cons.tail)
                ? &True : &False;

        case Tag_Tuple2:
            return eq(pa->tuple2.a, pb->tuple2.a)
                && eq(pa->tuple2.b, pb->tuple2.b)
                ? &True : &False;

        case Tag_Tuple3:
            return eq(pa->tuple3.a, pb->tuple3.a)
                && eq(pa->tuple3.b, pb->tuple3.b)
                && eq(pa->tuple3.c, pb->tuple3.c)
                ? &True : &False;

        case Tag_Custom:
            if (pa->custom.ctor != pb->custom.ctor) {
                return &False;
            }
            for (int i=0; i<ha.size; i++) {
                if (!eq(pa->custom.values[i], pb->custom.values[i])) {
                    return &False;                   
                }
            }
            return &True;

        case Tag_Record:
            if (pa->record.fields != pb->record.fields) {
                return &False;
            }
            for (int i=0; i<ha.size; i++) {
                if (!eq(pa->record.values[i], pb->record.values[i])) {
                    return &False;                    
                }
            }
            return &True;

        case Tag_Closure:
        default:
            // Debug.crash
            return &False;
    }
}

