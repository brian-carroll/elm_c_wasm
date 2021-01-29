#include "utils.h"

#include <assert.h>
#include <string.h>

#include "debug.h"
#include "elm.h"
#include "gc.h"
#include "list.h"
#include "string.h"
#include "types.h"

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#else
#define emscripten_run_script(x)
#endif

// Initialise a global value by evaluating an Elm expression
//
// global_permanent_ptr:  location of a pointer outside the heap
//           that always points to the current heap location.
//           GC will update this as it moves the value around.
// init_func:   a function that evaluates the Elm expression
//           to initialise the global value
void Utils_initGlobal(void** global_permanent_ptr, void* (*init_func)()) {
  GC_register_root(global_permanent_ptr);
  GC_stack_empty();
  for (;;) {
    void* heap_value = init_func();
    if (heap_value != pGcFull) {
      *global_permanent_ptr = heap_value;
      GC_stack_empty();
      return;
    }
    GC_collect_full();
    GC_prep_replay();
  }
}

// Destructure by index
//    custom, cons, or tuple
// Dynamic type check needed, since Cons and tuples are using "special-case" structs
// and compiler code gen doesn't have type info to distinguish them from generic Custom
void* Utils_destruct_index(ElmValue* v, size_t index) {
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

// -----------------------------------------------------------
// Clone a value
// -----------------------------------------------------------
void* Utils_clone(void* x) {
  Header* h = (Header*)x;
  if (h == pNil || (h->tag == Tag_Custom && custom_params(x) == 0)) return x;
  size_t n_bytes = SIZE_UNIT * (size_t)h->size;
  ElmValue* x_new = CAN_THROW(GC_malloc(n_bytes));
  GC_memcpy(x_new, x, n_bytes);
  return x_new;
}

// -----------------------------------------------------------
//      RECORD OPERATORS
// -----------------------------------------------------------

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

  return -1;
}

void* Utils_access_eval(void* args[2]) {
  u32 field = (u32)(size_t)args[0];  // unboxed!
  Record* record = (Record*)args[1];
  u32 index = fieldgroup_search(record->fieldgroup, field);

  if (index == -1) {
      log_error("Failed to find field %d in record access at %p\n", field, record);
  }

  return record->values[index];
}

Record* Utils_update(Record* r, u32 n_updates, u32 fields[], void* values[]) {
  Record* r_new = Utils_clone(r);
  if (r_new == pGcFull) return pGcFull;

  for (u32 i = 0; i < n_updates; ++i) {
    u32 field_pos = fieldgroup_search(r_new->fieldgroup, fields[i]);

    if (field_pos == -1) {
        log_error("Failed to find field %d in record update at %p\n", fields[i], r_new);
    }

    r_new->values[field_pos] = values[i];
  }

  return r_new;
}

// -----------------------------------------------------------
//      APPEND OPERATOR (++)
// -----------------------------------------------------------
void* eval_Utils_append(void* args[]) {
  Header* h = args[0];
  if (h->tag == Tag_List) {
    return eval_List_append(args);
  } else {
    return eval_String_append(args);
  }
}
Closure Utils_append = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_Utils_append,
    .max_values = 2,
};

// -----------------------------------------------------------
//      FUNCTION APPLICATION
// -----------------------------------------------------------

void* Utils_apply(Closure* c, u16 n_applied, void* applied[]) {
  void** args;
  do {
    Closure* replay = GC_apply_replay(c);
    if (replay) {
      if (replay->header.tag != Tag_Closure) return replay;
      if (replay->n_values < replay->max_values) return replay;
      assert(c->evaluator == replay->evaluator);
      args = replay->values;
    } else if (n_applied >= c->max_values) {
      // All args in one go (or too many args, expecting a function to be returned)
      args = applied;
    } else if (n_applied == 0) {
      // All args already applied, but result not evaluated yet. A thunk.
      // Elm syntax can't do this but runtime/kernel can
      args = c->values;
    } else {
      // Partial application, need a Closure. 'args' could be any size => heap
      u16 n_old = c->n_values;
      u16 n_new = n_old + n_applied;

      Closure* c_copy = CAN_THROW(GC_malloc(SIZE_CLOSURE(n_new)));
      GC_memcpy(c_copy, c, SIZE_CLOSURE(n_old));
      GC_memcpy(&c_copy->values[n_old], applied, SIZE_CLOSURE(n_applied));
      c_copy->header = HEADER_CLOSURE(n_new);
      c_copy->n_values = n_new;

      if (n_new < c_copy->max_values) {
        return c_copy;
      }
      args = c_copy->values; // enough args => execute
    }


    // Execute! (and let the GC know what the stack is doing)
    void* push = GC_stack_push(c->evaluator);
    void* result = CAN_THROW(c->evaluator(args));
    GC_stack_pop(c->evaluator, result, push);


    u16 n_total = c->n_values + n_applied;
    u16 n_done = c->max_values;
    if (n_total <= n_done) {
      return result;
    }

    // We have more args to apply. Go around again.
    // (The function must have returned another function, since ELm type-checked it)
    c = (Closure*)result;
    assert(c->header.tag == Tag_Closure);
    n_applied = n_total - n_done;
    applied = &args[n_done];
  } while (true);
}

// -----------------------------------------------------------
// EQUALITY OPERATOR (==)
// -----------------------------------------------------------

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

  if (pa->header_as_u32 != pb->header_as_u32) return 0;

  switch (pa->header.tag) {
    case Tag_Int:
      return pa->elm_int.value == pb->elm_int.value;

    case Tag_Float:
      return pa->elm_float.value == pb->elm_float.value;

    case Tag_Char:
      return pa->elm_char.value == pb->elm_char.value;

    case Tag_String: {
      size_t* a_words = (size_t*)pa;
      size_t* b_words = (size_t*)pb;
      // start at first word that contains characters (index 0 or 1)
      size_t start = ((void*)pa->elm_string.bytes - (void*)pa) / sizeof(void*);
      for (size_t i = start; i < pa->header.size; ++i) {
        if (a_words[i] != b_words[i]) return 0;
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

    case Tag_Record: {
      // Fieldgroup assumed to be equal due to Elm type checking
      u32 nparams = pa->header.size - (sizeof(Record) / SIZE_UNIT);
      for (u32 i = 0; i < nparams; ++i)
        if (!eq_help(pa->record.values[i], pb->record.values[i], depth + 1, pstack))
          return 0;
      return 1;
    }

    case Tag_FieldGroup:
      return 1;

    case Tag_Closure:
      // C doesn't have exceptions, would have to call out to JS.
      // For now it's a warning rather than error and returns False
      log_error(
          "Warning: Trying to use `(==)` on functions.\n"
          "There is no way to know if functions are \"the same\" in the Elm sense.\n"
          "Read more about this at "
          "https://package.elm-lang.org/packages/elm/core/latest/Basics#== which "
          "describes why it is this way and what the better version will look like.\n");
      return 0;

    case Tag_JsRef:
      return pa->js_ref.index == pb->js_ref.index;

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

  return stack == pGcFull ? pGcFull : isEqual ? &True : &False;
}
Closure Utils_equal = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eq_eval,
    .max_values = 2,
};

// INEQUALITY
static void* eval_notEqual(void* args[2]) {
  void* equal = eq_eval(args);
  if (equal == pGcFull) {
    return pGcFull;
  }
  return equal == &False ? &True : &False;
}
Closure Utils_notEqual = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_notEqual,
    .max_values = 2,
};

// -----------------------------------------------------------
// COMPARISON OPERATORS (<,>,>=,<=)
// -----------------------------------------------------------

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
Closure Utils_compare = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &compare_eval,
    .max_values = 2,
};

static void* lt_eval(void* args[2]) {
  ElmValue* x = args[0];
  ElmValue* y = args[1];
  return (compare_help(x, y) == &Utils_LT) ? &True : &False;
}
Closure Utils_lt = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &lt_eval,
    .max_values = 2,
};

static void* le_eval(void* args[2]) {
  ElmValue* x = args[0];
  ElmValue* y = args[1];
  return (compare_help(x, y) != &Utils_GT) ? &True : &False;
}
Closure Utils_le = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &le_eval,
    .max_values = 2,
};

static void* gt_eval(void* args[2]) {
  ElmValue* x = args[0];
  ElmValue* y = args[1];
  return (compare_help(x, y) == &Utils_GT) ? &True : &False;
}
Closure Utils_gt = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &gt_eval,
    .max_values = 2,
};

static void* ge_eval(void* args[2]) {
  ElmValue* x = args[0];
  ElmValue* y = args[1];
  return (compare_help(x, y) != &Utils_LT) ? &True : &False;
}
Closure Utils_ge = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &ge_eval,
    .max_values = 2,
};

Custom Utils_LT = {
    .header = HEADER_CUSTOM(0),
    .ctor = -1,
};
Custom Utils_EQ = {
    .header = HEADER_CUSTOM(0),
    .ctor = 0,
};
Custom Utils_GT = {
    .header = HEADER_CUSTOM(0),
    .ctor = 1,
};
