#include <string.h>
#include <stdio.h>
#include "types.h"


u32 fieldset_search(FieldSet* fieldset, u32 search) {
    u32 first = 0;
    u32 last = fieldset->size - 1;
    u32* array = fieldset->fields;

    while (first <= last) {
        u32 middle = (first + last)/2;
        if (array[middle] == search) {
            return middle;
        } else if (array[middle] < search) {
            first = middle + 1;
        } else {
            last = middle - 1;
        }
    }

    fprintf(stderr, "Failed to find field %d in record fieldset at %llx\n",
        search, (u64)fieldset
    );
    return 0;
}


void* record_access_eval(void* args[]) {
    ElmInt* field = (ElmInt*)args[0];
    Record* record = (Record*)args[1];

    u32 index = fieldset_search(record->fieldset, field->value);
    return record->values[index];
}

Closure record_access;


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
static u32 eq_help(ElmValue* pa, ElmValue* pb) {

    // Reference equality shortcut
    if (pa == pb) {
        return 1;
    }

    // True, False and Unit are constants with no headers. Check for them first.
    u8* pa_const = &pa->unit_or_bool;
    if (pa_const == &True || pa_const == &False || pa_const == &Unit) {
        return 0; // These values only have reference equality. Can't be equal if we got this far.
    }

    // It's not Bool or Unit so it must have a header.
    Header ha = *(Header*)pa;
    Header hb = *(Header*)pb;
    switch (ha.tag) {
        case Tag_Int:
            return pa->elm_int.value == pb->elm_int.value;

        case Tag_Float:
            return pa->elm_float.value == pb->elm_float.value;

        case Tag_Char:
            return pa->elm_char.value == pb->elm_char.value;

        case Tag_String: {
            if (ha.size != hb.size) return 0;
            u32* a_ints = (u32*)pa->elm_string.bytes;
            u32* b_ints = (u32*)pb->elm_string.bytes;
            for (u32 i=0; i<ha.size; ++i) {
                if (a_ints[i] != b_ints[i]) return 0;
            }
            return 1;
        }

        case Tag_Nil:
            return 0; // if we got this far, can't both be Nil

        case Tag_Cons:
            return eq_help(pa->cons.head, pb->cons.head)
                && eq_help(pa->cons.tail, pb->cons.tail);

        case Tag_Tuple2:
            return eq_help(pa->tuple2.a, pb->tuple2.a)
                && eq_help(pa->tuple2.b, pb->tuple2.b);

        case Tag_Tuple3:
            return eq_help(pa->tuple3.a, pb->tuple3.a)
                && eq_help(pa->tuple3.b, pb->tuple3.b)
                && eq_help(pa->tuple3.c, pb->tuple3.c);

        case Tag_Custom:
            if (pa->custom.ctor != pb->custom.ctor) {
                return 0;
            }
            for (u32 i=0; i<ha.size; i++) {
                if (!eq_help(pa->custom.values[i], pb->custom.values[i])) {
                    return 0;
                }
            }
            return 1;

        case Tag_Record:
            if (pa->record.fieldset != pb->record.fieldset) {
                return 0;
            }
            for (u32 i=0; i<ha.size; i++) {
                if (!eq_help(pa->record.values[i], pb->record.values[i])) {
                    return 0;
                }
            }
            return 1;

        case Tag_Closure:
            fprintf(stderr, "Elm Warning: Functions can't be compared for equality. Returning 'False'.\n"); // TODO: implement exceptions?
            return 0;

        default:
            fprintf(stderr, "Elm Warning: Tried to apply '==' to an unknown value type. Returning 'False'.\n"); // TODO: implement exceptions?
            return 0;
    }
}

static void* eq_eval(void* args[]) {
    return eq_help(args[0], args[1]) ? &True : &False;
}

Closure eq;

void utils_init() {
    eq = CLOSURE(eq_eval, 2);
    record_access = CLOSURE(record_access_eval, 2);
}
