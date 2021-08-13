#include "core.h"

#include <string.h>



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
  ElmValue* x_new = GC_allocate(true, h->size);
  GC_memcpy(x_new, x, h->size);
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

void* Utils_access_eval(void* args[]) {
  u32 field = (u32)(size_t)args[0];  // unboxed!
  Record* record = args[1];
  if (record->header.tag == Tag_JsRef) {
    JsRef* jsRef = args[1];
    record = jsRefToWasmRecord(jsRef->id);
  }
  ASSERT_EQUAL(record->header.tag, Tag_Record);
  u32 index = fieldgroup_search(record->fieldgroup, field);

  if (index == -1) {
      log_error("Failed to find field %d in record access at %p\n", field, record);
  }

  return record->values[index];
}

Record* Utils_update(Record* r, u32 n_updates, u32 fields[], void* values[]) {
  if (r->header.tag == Tag_JsRef) {
    JsRef* jsRef = (JsRef*)r;
    r = jsRefToWasmRecord(jsRef->id);
  }
  Record* r_new = Utils_clone(r);
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

void* Utils_apply(void* func, u16 n_applied, void* applied[]) {
  ElmValue* v = func;
  if (v->header.tag == Tag_JsRef) {
    GcStackMapIndex stack_frame = GC_stack_push_frame('J', func);
    JsRef* jsRef = func;
    void* result = applyJsRef(jsRef->id, n_applied, applied);
    GC_stack_pop_frame(func, result, stack_frame);
    return result;
  }
  ASSERT(v->header.tag == Tag_Closure, v->header.tag);

  Closure* c = func;
  void** args;
  do {
    if (n_applied >= c->max_values) {
      // All args in one go (or too many args, expecting a function to be returned)
      args = applied;
    } else if (n_applied == 0) {
      // All args already applied, but result not evaluated yet. A thunk.
      // Elm syntax can't do this but runtime/kernel can
      args = c->values;
    } else {
      // Partial application, need a Closure. 'args' could be any size.
      u16 n_old = c->n_values;
      u16 n_new = n_old + n_applied;

      Closure* c_copy = GC_allocate(true, SIZE_CLOSURE(n_new));
      c_copy->header = (Header)HEADER_CLOSURE(n_new);
      c_copy->n_values = n_new;
      c_copy->max_values = c->max_values;
      c_copy->evaluator = c->evaluator;
      for (u32 i = 0; i < n_old; ++i) {
        c_copy->values[i] = c->values[i];
      }
      for (u32 i = 0; i < n_applied; ++i) {
        c_copy->values[n_old + i] = applied[i];
      }

      if (n_new < c_copy->max_values) {
        return c_copy;
      }
      args = c_copy->values; // enough args => execute
    }


    // Execute! (and let the GC know what the stack is doing)
    GcStackMapIndex stack_frame = GC_stack_push_frame('C', c->evaluator);
    void* result = c->evaluator(args);

    // if (!result) {
    //   safe_printf("NULL returned from %s\n", Debug_evaluator_name(c->evaluator));
    //   char label[5] = "arg 0";
    //   for (int i=0; i< n_applied; i++) {
    //     Debug_pretty(label, args[i]);
    //     label[4]++;
    //   }
    // }


    GC_stack_pop_frame(c->evaluator, result, stack_frame);


    u16 n_total = c->n_values + n_applied;
    u16 n_done = c->max_values;
    if (n_total <= n_done) {
      return result;
    }

    // We have more args to apply. Go around again.
    // (The function must have returned another function, since ELm type-checked it)
    c = (Closure*)result;
    ASSERT(c->header.tag == Tag_Closure, c->header.tag);
    n_applied = n_total - n_done;
    applied = &args[n_done];
  } while (true);
}

// -----------------------------------------------------------
// EQUALITY OPERATOR (==)
// -----------------------------------------------------------

static ElmValue* eq_stack_push(ElmValue* pa, ElmValue* pb, ElmValue** pstack) {
  Tuple2* t2 = newTuple2(pa, pb);
  Cons* c = newCons(t2, *pstack);
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
      for (size_t i = 0; i < pa->header.size; ++i) {
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

    case Tag_Task:
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
      // Cannot get to here from working code that type checks.
      return 1;

    case Tag_Closure: {
      if (pa->closure.evaluator != pb->closure.evaluator) return 0;
      u32 n_values = pa->closure.n_values;
      for (u32 i = 0; i < n_values; ++i) {
        if (!eq_help(pa->closure.values[i], pb->closure.values[i], depth + 1, pstack))
          return 0;
      }
      return 1;
    }
    // log_error(
    //     "Warning: Trying to use `(==)` on functions.\n"
    //     "There is no way to know if functions are \"the same\" in the Elm sense.\n"
    //     "Read more about this at "
    //     "https://package.elm-lang.org/packages/elm/core/latest/Basics#== which "
    //     "describes why it is this way and what the better version will look like.\n");
    // return 0;

    case Tag_JsRef:
      return pa->js_ref.id == pb->js_ref.id;

    case Tag_Process:
    default:
      return 0;
  }
}

void* eq_eval(void* args[]) {
  ElmValue* nil = (ElmValue*)&Nil;
  ElmValue* stack = nil;
  u32 isEqual = eq_help(args[0], args[1], 0, &stack);

  while (isEqual && stack != nil) {
    // eq_help reached max recursion depth. Pick up again where it left off.
    Tuple2* pair = stack->cons.head;
    stack = stack->cons.tail;
    isEqual = eq_help(pair->a, pair->b, 0, &stack);
  }

  return isEqual ? &True : &False;
}
Closure Utils_equal = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eq_eval,
    .max_values = 2,
};

// INEQUALITY
void* eval_notEqual(void* args[]) {
  void* equal = eq_eval(args);
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
  if (x == y) return &g_elm_core_Basics_EQ;

  switch (x->header.tag) {
    case Tag_Int:
      if (x->elm_int.value == y->elm_int.value)
        return &g_elm_core_Basics_EQ;
      else if (x->elm_int.value < y->elm_int.value)
        return &g_elm_core_Basics_LT;
      else
        return &g_elm_core_Basics_GT;

    case Tag_Float:
      if (x->elm_float.value == y->elm_float.value)
        return &g_elm_core_Basics_EQ;
      else if (x->elm_float.value < y->elm_float.value)
        return &g_elm_core_Basics_LT;
      else
        return &g_elm_core_Basics_GT;

    case Tag_Char:
      if (x->elm_char.value == y->elm_char.value)
        return &g_elm_core_Basics_EQ;
      else if (x->elm_char.value < y->elm_char.value)
        return &g_elm_core_Basics_LT;
      else
        return &g_elm_core_Basics_GT;

    case Tag_String: {
      // https://tc39.es/ecma262/#sec-abstract-relational-comparison
      size_t lx = code_units(&x->elm_string);
      size_t ly = code_units(&y->elm_string);
      size_t len = lx < ly ? lx : ly;
      size_t i = 0;
      u16 cx, cy;
      for (i = 0; i < len; ++i) {
        cx = x->elm_string.words16[i];
        cy = y->elm_string.words16[i];
        if (cx != cy) break;
      }
      Custom* result;
      if (i == len) {
        if (lx == ly) {
          result = &g_elm_core_Basics_EQ;
        } else {
          result = (lx < ly) ? &g_elm_core_Basics_LT : &g_elm_core_Basics_GT;
        }
      } else {
        result = (cx < cy) ? &g_elm_core_Basics_LT : &g_elm_core_Basics_GT;
      }
      return result;
    }

    case Tag_List:
      if (y == pNil)
        return &g_elm_core_Basics_GT;
      else if (x == pNil)
        return &g_elm_core_Basics_LT;
      else
        while (1) {
          Custom* order_head = compare_help(x->cons.head, y->cons.head);
          if (order_head != &g_elm_core_Basics_EQ) return order_head;
          x = x->cons.tail;
          y = y->cons.tail;
          if (x == pNil || y == pNil) return compare_help(x, y);
        }

    case Tag_Tuple2: {
      Custom* ord;
      ord = compare_help(x->tuple2.a, y->tuple2.a);
      if (ord != &g_elm_core_Basics_EQ) return ord;
      ord = compare_help(x->tuple2.b, y->tuple2.b);
      return ord;
    }

    case Tag_Tuple3: {
      Custom* ord;
      ord = compare_help(x->tuple3.a, y->tuple3.a);
      if (ord != &g_elm_core_Basics_EQ) return ord;
      ord = compare_help(x->tuple3.b, y->tuple3.b);
      if (ord != &g_elm_core_Basics_EQ) return ord;
      ord = compare_help(x->tuple3.c, y->tuple3.c);
      return ord;
    }

    default:
      return NULL; // wtf
  }
}

void* compare_eval(void* args[]) {
  ElmValue* x = args[0];
  ElmValue* y = args[1];
  return compare_help(x, y);
}
Closure Utils_compare = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &compare_eval,
    .max_values = 2,
};

void* lt_eval(void* args[]) {
  ElmValue* x = args[0];
  ElmValue* y = args[1];
  return (compare_help(x, y) == &g_elm_core_Basics_LT) ? &True : &False;
}
Closure Utils_lt = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &lt_eval,
    .max_values = 2,
};

void* le_eval(void* args[]) {
  ElmValue* x = args[0];
  ElmValue* y = args[1];
  return (compare_help(x, y) != &g_elm_core_Basics_GT) ? &True : &False;
}
Closure Utils_le = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &le_eval,
    .max_values = 2,
};

void* gt_eval(void* args[]) {
  ElmValue* x = args[0];
  ElmValue* y = args[1];
  return (compare_help(x, y) == &g_elm_core_Basics_GT) ? &True : &False;
}
Closure Utils_gt = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &gt_eval,
    .max_values = 2,
};

void* ge_eval(void* args[]) {
  ElmValue* x = args[0];
  ElmValue* y = args[1];
  return (compare_help(x, y) != &g_elm_core_Basics_LT) ? &True : &False;
}
Closure Utils_ge = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &ge_eval,
    .max_values = 2,
};
