#include <stddef.h>
#include <stdlib.h>

#include "core.h"

// Allocate an entire list at once, with no recursion overhead
// First value in the array becomes the head of the list
void* List_create(size_t len, void* values[]) {
  Cons* head = &Nil;
  for (size_t i = 0; i < len; ++i) {
    Cons* next = GC_allocate(true, SIZE_LIST);
    *next = (Cons){
        .header = HEADER_LIST,
        .head = values[len - 1 - i],
        .tail = head,
    };
    head = next;
  }
  return head;
}

// cons

static void* eval_List_cons(void* args[]) {
  return newCons(args[0], args[1]);
}
Closure List_cons = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_List_cons,
    .max_values = 2,
};

// append
// we need a Kernel version so that Utils_append always has something to refer to
void* eval_List_append(void* args[]) {
  Cons* xs = args[0];
  Cons* ys = args[1];

  if (xs == &Nil) return ys;
  if (ys == &Nil) return xs;

  Cons* result = ys;
  for (Cons* x = xs; x != &Nil; x = x->tail) {
    result = newCons(NULL, result);
  }
  for (Cons* x = xs, *r = result; x != &Nil; x = x->tail, r = r->tail) {
    r->head = x->head;
  }
  return result;
}
Closure List_append = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_List_append,
    .max_values = 2,
};

// map2

static void* eval_List_map2(void* args[]) {
  Closure* f = args[0];
  Cons* xs = args[1];
  Cons* ys = args[2];

  Cons* tmp = newCons(NULL, pNil);
  Cons* end = tmp;
  for (; xs != pNil && ys != pNil; xs = xs->tail, ys = ys->tail) {
    Cons* next = newCons(A2(f, xs->head, ys->head), pNil);
    end->tail = next;
    end = next;
  }
  return tmp->tail;
}
Closure List_map2 = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_List_map2,
    .max_values = 3,
};

// Dummy to get core tests running
// Possible implementation here https://www.chiark.greenend.org.uk/~sgtatham/algorithms/listsort.c
// But it seems to rely on mutation and out-of-order lists which would be bad for our GC
void* eval_List_sortBy(void* args[]) {
  return &Nil;
}
Closure List_sortBy = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_List_sortBy,
    .max_values = 2,
};
