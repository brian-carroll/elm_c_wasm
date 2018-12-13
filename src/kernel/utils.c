#include <string.h>
#include "./types.h"
#include "./utils.h"
#include "./string.h"
#include "./list.h"
#include "./gc.h"

#ifdef DEBUG
    #include <stdio.h>
    extern void gc_test_stack_debug(GcStackMap* sm, Closure* c);
#endif


void* Utils_clone(void* x) {
    Header* h = (Header*)x;
    if (h->tag == Tag_Nil || (h->tag == Tag_Custom && h->size == 1)) {
        return x;
    }
    size_t n_bytes = SIZE_UNIT * (size_t)h->size;
    ElmValue* x_new = GC_malloc(n_bytes);
    if (x_new == NULL) return pGcFull;
    GC_memcpy(x_new, x, n_bytes);
    return x_new;
}


static u32 fieldset_search(FieldSet* fieldset, u32 search) {
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

    #ifdef DEBUG
        fprintf(stderr, "Failed to find field %d in record fieldset at %zx\n",
            search, (size_t)fieldset
        );
    #endif

    return 0;
}


static void* access_eval(void* args[2]) {
    ElmInt* field = (ElmInt*)args[0];
    Record* record = (Record*)args[1];

    u32 index = fieldset_search(record->fieldset, field->value);
    return record->values[index];
}

Closure Utils_access;


Record* Utils_update(Record* r, u32 n_updates, u32 fields[], void* values[]) {
    Record* r_new = Utils_clone(r);
    if (r_new == pGcFull) return pGcFull;

    for (u32 i=0; i<n_updates; ++i) {
        u32 field_pos = fieldset_search(r_new->fieldset, fields[i]);
        r_new->values[field_pos] = values[i];
    }

    return r_new;
}


void* Utils_apply(Closure* c_old, u8 n_applied, void* applied[]) {
    void** args;
    Closure *c;
    
    printf("\nUtils_apply: apply closure at %p\n", c_old);

    void* replay = GC_apply_replay();
    if (replay != NULL) {
        c = replay;
        if (c->header.tag != Tag_Closure) {
            printf("Utils_apply: replay %p\n", replay);
            return replay;
        }
        if (c->n_values != c->max_values) {
            printf("Utils_apply: replay partially applied Closure at %p\n", replay);
            return replay;
        }
        printf("Utils_apply: executing replayed closure at %p\n", c);
        args = c->values;
    } else if (c_old->max_values == n_applied) {
        // 'saturated' call. No need to allocate a new Closure.
        c = c_old;
        args = applied;
    } else {
        u8 n_old = c_old->n_values;
        u8 n_new = n_old + n_applied;

        size_t size_old = sizeof(Closure) + n_old * sizeof(void*);
        size_t size_applied = n_applied * sizeof(void*);
        size_t size_new = size_old + size_applied;

        c = CAN_THROW(GC_malloc(size_new));

        GC_memcpy(c, c_old, size_old);
        GC_memcpy(&c->values[n_old], applied, size_applied);
        c->header = HEADER_CLOSURE(n_new);
        c->n_values = n_new;

        if (n_new != c->max_values) {
            // Partial application (not calling evaluator => no stack push)
            return c;
        }
        args = c->values;
    }

    void* push = CAN_THROW(GC_stack_push());
    #ifdef DEBUG
        gc_test_stack_debug(push, c); // help to show function names in debug output
    #endif

    ElmValue* result = (*c->evaluator)(args);
    if ((void*)result == pGcFull) {
        return result;
    }

    void* pop = CAN_THROW(GC_stack_pop(result, push));
    #ifdef DEBUG
        gc_test_stack_debug(pop, c); // help to show function names in debug output
    #else
        (void)pop; // suppress compiler warning about unused variable in non-DEBUG mode
    #endif

    return result;
}


ElmValue* eq_stack_push(ElmValue* pa, ElmValue* pb, ElmValue** pstack) {
    Tuple2* t2 = newTuple2(pa, pb);
    Cons* c = newCons(t2, *pstack);
    return (ElmValue*)c;
}


static u32 eq_help(ElmValue* pa, ElmValue* pb, u32 depth, ElmValue** pstack) {

    if (depth > 100) {
        *pstack = eq_stack_push(pa, pb, pstack);
        return 1;
    }

    if (pa == pb) {
        return 1;
    }

    if (pa->header.tag != pb->header.tag) {
        return 0;
    }

    switch (pa->header.tag) {
        case Tag_Nil:
            return 0;

        case Tag_Int:
            return pa->elm_int.value == pb->elm_int.value;

        case Tag_Float:
            return pa->elm_float.value == pb->elm_float.value;

        case Tag_Char:
            return pa->elm_char.value == pb->elm_char.value;

        case Tag_String: {
            if (pa->header.size != pb->header.size) return 0;
            u32* a_ints = (u32*)pa->elm_string.bytes;
            u32* b_ints = (u32*)pb->elm_string.bytes;
            for (u32 i=0; i < pa->header.size; ++i) {
                if (a_ints[i] != b_ints[i]) return 0;
            }
            return 1;
        }

        case Tag_Cons:
            return eq_help(pa->cons.head, pb->cons.head, depth + 1, pstack)
                && eq_help(pa->cons.tail, pb->cons.tail, depth + 1, pstack);

        case Tag_Tuple2:
            return eq_help(pa->tuple2.a, pb->tuple2.a, depth + 1, pstack)
                && eq_help(pa->tuple2.b, pb->tuple2.b, depth + 1, pstack);

        case Tag_Tuple3:
            return eq_help(pa->tuple3.a, pb->tuple3.a, depth + 1, pstack)
                && eq_help(pa->tuple3.b, pb->tuple3.b, depth + 1, pstack)
                && eq_help(pa->tuple3.c, pb->tuple3.c, depth + 1, pstack);

        case Tag_Custom: {
            if (pa->custom.ctor != pb->custom.ctor) {
                return 0;
            }
            u32 nparams = custom_params(&pa->custom);
            for (u32 i=0; i < nparams; ++i) {
                if (!eq_help(pa->custom.values[i], pb->custom.values[i], depth + 1, pstack)) {
                    return 0;
                }
            }
            return 1;
        }

        case Tag_Record:
            // Elm guarantees same Record type => same fieldset
            for (u32 i=0; i < pa->record.fieldset->size; ++i) {
                if (!eq_help(pa->record.values[i], pb->record.values[i], depth + 1, pstack)) {
                    return 0;
                }
            }
            return 1;

        case Tag_Closure:
            // C doesn't have exceptions, would have to call out to JS.
            // For now it's a warning rather than error and returns False
            #ifdef DEBUG
                fprintf(stderr, "Warning: Trying to use `(==)` on functions.\nThere is no way to know if functions are \"the same\" in the Elm sense.\nRead more about this at https://package.elm-lang.org/packages/elm/core/latest/Basics#== which describes why it is this way and what the better version will look like.\n");
            #endif
            return 0;

        case Tag_GcException:
        case Tag_GcStackEmpty:
        case Tag_GcStackPush:
        case Tag_GcStackPop:
        case Tag_GcStackTailCall:
            return 0;
    }
}


static void* eq_eval(void* args[2]) {
    ElmValue* nil = (ElmValue*)&Nil;
    ElmValue* stack = nil;
    u32 isEqual = eq_help(args[0], args[1], 0, &stack);

    while (isEqual && stack != nil && stack != pGcFull) {
        // eq_help reached max recursion depth. Pick up again where it left off.
        Tuple2* pair = stack->cons.head;
        stack = stack->cons.tail;
        isEqual = eq_help(pair->a, pair->b, 0, &stack);
    }

    return stack == pGcFull
        ? pGcFull
        : isEqual ? &True : &False;
}

Closure Utils_eq;



static void* append_eval(void* args[2]) {
    Header* h = (Header*)args[0];

    switch (h->tag) {
        case Tag_String:
            return String_append_eval(args);

        case Tag_Nil:
        case Tag_Cons:
            return List_append_eval(args);

        default:
            #ifdef DEBUG
                fprintf(stderr, "Tried to Utils_append non-appendable\n");
            #endif
            return args[0];
    }
}

Closure Utils_append;



static void* compare_help(ElmValue* x, ElmValue* y) {
    switch (x->header.tag) {
        case Tag_Int:
            if (x->elm_int.value == y->elm_int.value) {
                return &Utils_EQ;
            } else if (x->elm_int.value < y->elm_int.value) {
                return &Utils_LT;
            } else {
                return &Utils_GT;
            }

        case Tag_Float:
            if (x->elm_float.value == y->elm_float.value) {
                return &Utils_EQ;
            } else if (x->elm_float.value < y->elm_float.value) {
                return &Utils_LT;
            } else {
                return &Utils_GT;
            }

        case Tag_Char:
            if (x->elm_char.value == y->elm_char.value) {
                return &Utils_EQ;
            } else if (x->elm_char.value < y->elm_char.value) {
                return &Utils_LT;
            } else {
                return &Utils_GT;
            }

        case Tag_String:
            // TODO
            // iterate, decode char, compare, loop
            // Probably call something from String lib
            // foldl doesn't have early return so maybe write something that does?
            return &Utils_EQ;

        case Tag_Nil:
            if (y->header.tag == Tag_Nil) {
                return &Utils_EQ;
            } else {
                return &Utils_LT;
            }

        case Tag_Cons:
            if (y->header.tag == Tag_Nil) {
                return &Utils_GT;
            } else {
                while (1) {
                    Custom* order_head = compare_help(x->cons.head, y->cons.head);
                    if (order_head != &Utils_EQ) {
                        return order_head;
                    }
                    x = x->cons.tail;
                    y = y->cons.tail;
                    if (x->header.tag == Tag_Nil || y->header.tag == Tag_Nil) {
                        return compare_help(x, y);
                    }
                }
            }

        case Tag_Tuple2: {
            Custom* ord;
            ord = compare_help(x->tuple2.a, y->tuple2.a);
            if (ord != &Utils_EQ) return ord;
            ord = compare_help(x->tuple2.b, y->tuple2.b);
            return ord;
        }

        case Tag_Tuple3: {
            Custom* ord;
            ord = compare_help(x->tuple3.a, y->tuple3.a);
            if (ord != &Utils_EQ) return ord;
            ord = compare_help(x->tuple3.b, y->tuple3.b);
            if (ord != &Utils_EQ) return ord;
            ord = compare_help(x->tuple3.c, y->tuple3.c);
            return ord;
        }

        default:
            return NULL;
    }
}

static void* compare_eval(void* args[2]) {
    ElmValue* x = args[0];
    ElmValue* y = args[1];
    return compare_help(x, y);
}

Closure Utils_compare;


static void* lt_eval(void* args[2]) {
    ElmValue* x = args[0];
    ElmValue* y = args[1];
    return (compare_help(x, y) == &Utils_LT) ? &True : &False;
}
Closure Utils_lt;


static void* le_eval(void* args[2]) {
    ElmValue* x = args[0];
    ElmValue* y = args[1];
    return (compare_help(x, y) != &Utils_GT) ? &True : &False;
}
Closure Utils_le;


static void* gt_eval(void* args[2]) {
    ElmValue* x = args[0];
    ElmValue* y = args[1];
    return (compare_help(x, y) == &Utils_GT) ? &True : &False;
}
Closure Utils_gt;


static void* ge_eval(void* args[2]) {
    ElmValue* x = args[0];
    ElmValue* y = args[1];
    return (compare_help(x, y) != &Utils_LT) ? &True : &False;
}
Closure Utils_ge;


void Utils_init() {
    Utils_eq = F2(eq_eval);
    Utils_access = F2(access_eval);
    Utils_append = F2(append_eval);
    Utils_compare = F2(compare_eval);
    Utils_lt = F2(lt_eval);
    Utils_le = F2(le_eval);
    Utils_gt = F2(gt_eval);
    Utils_ge = F2(ge_eval);

    Utils_LT = (Custom){
        .header = HEADER_CUSTOM(0),
        .ctor = -1,
    };
    Utils_EQ = (Custom){
        .header = HEADER_CUSTOM(0),
        .ctor = 0,
    };
    Utils_GT = (Custom){
        .header = HEADER_CUSTOM(0),
        .ctor = 1,
    };
}
