#include "./utils.h"
#include <string.h>
#include "./gc.h"
#include "./list.h"
#include "./string.h"
#include "./types.h"

#if defined(DEBUG) || defined(DEBUG_LOG)
#include <stdio.h>
#include "../test/gc/print-heap.h"
extern void gc_debug_stack_trace(GcStackMap* sm, Closure* c);
#else
#define log_error(...)
#endif

void* Utils_destruct_index(ElmValue* v, size_t index) {
  // Destructure by index: custom, cons, or tuple
  // Dynamic type check needed, since Cons and tuples are using "special-case" structs
  // and compiler code gen doesn't have type info to distinguish them from generic Custom
  // All cases except Custom should compile to same assembly. But it's more robust than
  // 'if'
  void** children;
  switch (v->header.tag) {
    case Tag_Custom:
      children = v->custom.values;
      break;
    case Tag_List:
      children = &v->cons.head;
      break;
    case Tag_Tuple2:
      children = &v->tuple2.a;
      break;
    case Tag_Tuple3:
    default:
      children = &v->tuple3.a;
      break;
  }

  return children[index];
}

void* Utils_clone(void* x) {
  Header* h = (Header*)x;
  if (h == pNil || (h->tag == Tag_Custom && custom_params(x) == 1)) return x;
  size_t n_bytes = SIZE_UNIT * (size_t)h->size;
  ElmValue* x_new = CAN_THROW(GC_malloc(n_bytes));
  GC_memcpy(x_new, x, n_bytes);
  return x_new;
}

static u32 fieldgroup_search(FieldGroup* fieldgroup, u32 search) {
  u32 first = 0;
  u32 last = fieldgroup->size - 1;
  u32* array = fieldgroup->fields;

  while (first <= last) {
    u32 middle = (first + last) / 2;
    if (array[middle] == search) {
      return middle;
    } else if (array[middle] < search) {
      first = middle + 1;
    } else {
      last = middle - 1;
    }
  }

  log_error("Failed to find field %d in record fieldgroup at %zx\n",
      search,
      (size_t)fieldgroup);

  return 0;
}

static void* access_eval(void* args[2]) {
  ElmInt* field = (ElmInt*)args[0];
  Record* record = (Record*)args[1];

  u32 index = fieldgroup_search(record->fieldgroup, field->value);
  return record->values[index];
}
const Closure Utils_access = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &access_eval,
    .max_values = 2,
};

Record* Utils_update(Record* r, u32 n_updates, u32 fields[], void* values[]) {
  Record* r_new = Utils_clone(r);
  if (r_new == pGcFull) return pGcFull;

  for (u32 i = 0; i < n_updates; ++i) {
    u32 field_pos = fieldgroup_search(r_new->fieldgroup, fields[i]);
    r_new->values[field_pos] = values[i];
  }

  return r_new;
}

void* Utils_apply(Closure* c_old, u8 n_applied, void* applied[]) {
  void** args;
  Closure* c;

  void* replay = GC_apply_replay();
  if (replay != NULL) {
    c = replay;
    if (c->header.tag != Tag_Closure) {
      return replay;
    }
    if (c->n_values != c->max_values) {
      return replay;
    }
    args = c->values;
  } else if (c_old->max_values == n_applied) {
    // 'saturated' call. No need to allocate a new Closure.
    c = c_old;
    args = applied;
  } else if (n_applied == 0) {
    // a full closure (thunk)
    // from JS wrapper (or resumed tailcall? not sure if this happens)
    c = c_old;
    args = c_old->values;
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
  // #ifdef DEBUG
  // gc_debug_stack_trace(push, c);
  // #endif

  ElmValue* result = (*c->evaluator)(args);
  if ((void*)result == pGcFull) {
    return result;
  }

  void* pop = CAN_THROW(GC_stack_pop(result, push));
  // #ifdef DEBUG
  // gc_debug_stack_trace(pop, c);
  // #else
  (void)pop;  // suppress "unused variable" warning

  // #endif

  return result;
}

static ElmValue* eq_stack_push(ElmValue* pa, ElmValue* pb, ElmValue** pstack) {
  Tuple2* t2 = NEW_TUPLE2(pa, pb);
  Cons* c = NEW_CONS(t2, *pstack);
  return (ElmValue*)c;
}

static u32 eq_help(ElmValue* pa, ElmValue* pb, u32 depth, ElmValue** pstack) {
  if (depth > 100) {
    *pstack = eq_stack_push(pa, pb, pstack);
    return 1;
  }

  if (pa == pb) return 1;

  if (pa->header.tag != pb->header.tag) return 0;

  switch (pa->header.tag) {
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
      for (u32 i = 0; i < pa->header.size; ++i) {
        if (a_ints[i] != b_ints[i]) return 0;
      }
      return 1;
    }

    case Tag_List:
      return (pa != pNil) && (pb != pNil) &&
             eq_help(pa->cons.head, pb->cons.head, depth + 1, pstack) &&
             eq_help(pa->cons.tail, pb->cons.tail, depth + 1, pstack);

    case Tag_Tuple2:
      return eq_help(pa->tuple2.a, pb->tuple2.a, depth + 1, pstack) &&
             eq_help(pa->tuple2.b, pb->tuple2.b, depth + 1, pstack);

    case Tag_Tuple3:
      return eq_help(pa->tuple3.a, pb->tuple3.a, depth + 1, pstack) &&
             eq_help(pa->tuple3.b, pb->tuple3.b, depth + 1, pstack) &&
             eq_help(pa->tuple3.c, pb->tuple3.c, depth + 1, pstack);

    case Tag_Custom: {
      if (pa->custom.ctor != pb->custom.ctor) return 0;
      u32 nparams = custom_params(&pa->custom);
      for (u32 i = 0; i < nparams; ++i)
        if (!eq_help(pa->custom.values[i], pb->custom.values[i], depth + 1, pstack))
          return 0;
      return 1;
    }

    case Tag_Record:
      // Elm guarantees same Record type => same fieldgroup
      for (u32 i = 0; i < pa->record.fieldgroup->size; ++i)
        if (!eq_help(pa->record.values[i], pb->record.values[i], depth + 1, pstack))
          return 0;
      return 1;

    case Tag_Closure:
// C doesn't have exceptions, would have to call out to JS.
// For now it's a warning rather than error and returns False
#ifdef DEBUG
      fprintf(stderr,
          "Warning: Trying to use `(==)` on functions.\nThere is no way to know if "
          "functions are \"the same\" in the "
          "Elm sense.\nRead more about this at "
          "https://package.elm-lang.org/packages/elm/core/latest/Basics#== which "
          "describes why it is this way and what the better version will look like.\n");
#endif
      return 0;

    case Tag_GcException:
    case Tag_GcStackEmpty:
    case Tag_GcStackPush:
    case Tag_GcStackPop:
    case Tag_GcStackTailCall:
    case Tag_Unused:
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

  return stack == pGcFull ? pGcFull : isEqual ? &True : &False;
}
const Closure Utils_eq = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eq_eval,
    .max_values = 2,
};

static void* append_eval(void* args[2]) {
  Header* h = (Header*)args[0];

  switch (h->tag) {
    case Tag_String:
      return String_append_eval(args);

    case Tag_List:
      return List_append_eval(args);

    default:
      log_error("Tried to Utils_append non-appendable\n");
      return args[0];
  }
}
const Closure Utils_append = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &append_eval,
    .max_values = 2,
};

static void* compare_help(ElmValue* x, ElmValue* y) {
  if (x == y) return &Utils_EQ;

  switch (x->header.tag) {
    case Tag_Int:
      if (x->elm_int.value == y->elm_int.value)
        return &Utils_EQ;
      else if (x->elm_int.value < y->elm_int.value)
        return &Utils_LT;
      else
        return &Utils_GT;

    case Tag_Float:
      if (x->elm_float.value == y->elm_float.value)
        return &Utils_EQ;
      else if (x->elm_float.value < y->elm_float.value)
        return &Utils_LT;
      else
        return &Utils_GT;

    case Tag_Char:
      if (x->elm_char.value == y->elm_char.value)
        return &Utils_EQ;
      else if (x->elm_char.value < y->elm_char.value)
        return &Utils_LT;
      else
        return &Utils_GT;

    case Tag_String:
      // TODO
      // iterate, decode char, compare, loop
      // Probably call something from String lib
      // foldl doesn't have early return so maybe write something that does?
      return NULL;

    case Tag_List:
      if (y == pNil)
        return &Utils_GT;
      else if (x == pNil)
        return &Utils_LT;
      else
        while (1) {
          Custom* order_head = compare_help(x->cons.head, y->cons.head);
          if (order_head != &Utils_EQ) return order_head;
          x = x->cons.tail;
          y = y->cons.tail;
          if (x == pNil || y == pNil) return compare_help(x, y);
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
const Closure Utils_compare = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &compare_eval,
    .max_values = 2,
};

static void* lt_eval(void* args[2]) {
  ElmValue* x = args[0];
  ElmValue* y = args[1];
  return (compare_help(x, y) == &Utils_LT) ? &True : &False;
}
const Closure Utils_lt = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &lt_eval,
    .max_values = 2,
};

static void* le_eval(void* args[2]) {
  ElmValue* x = args[0];
  ElmValue* y = args[1];
  return (compare_help(x, y) != &Utils_GT) ? &True : &False;
}
const Closure Utils_le = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &le_eval,
    .max_values = 2,
};

static void* gt_eval(void* args[2]) {
  ElmValue* x = args[0];
  ElmValue* y = args[1];
  return (compare_help(x, y) == &Utils_GT) ? &True : &False;
}
const Closure Utils_gt = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &gt_eval,
    .max_values = 2,
};

static void* ge_eval(void* args[2]) {
  ElmValue* x = args[0];
  ElmValue* y = args[1];
  return (compare_help(x, y) != &Utils_LT) ? &True : &False;
}
const Closure Utils_ge = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &ge_eval,
    .max_values = 2,
};

const Custom Utils_LT = {
    .header = HEADER_CUSTOM(0),
    .ctor = -1,
};
const Custom Utils_EQ = {
    .header = HEADER_CUSTOM(0),
    .ctor = 0,
};
const Custom Utils_GT = {
    .header = HEADER_CUSTOM(0),
    .ctor = 1,
};
