#include <stddef.h>
#include <stdlib.h>

#include "core.h"

// Allocate an entire list at once, with no recursion overhead
// First value in the array becomes the head of the list
void* List_create(size_t len, void* values[]) {
  Cons* head = &Nil;
  for (size_t i = 0; i < len; ++i) {
    Cons* next = GC_malloc(true, sizeof(Cons));
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
  return NEW_CONS(args[0], args[1]);
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
    result = NEW_CONS(NULL, result);
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

  Custom* growingArray = GC_malloc(true, sizeof(Custom));
  if (growingArray == pGcFull) return pGcFull;
  growingArray->header = (Header)HEADER_CUSTOM(0);

  ptrdiff_t i = 0;
  const size_t CHUNK = 8;
  for (; xs != &Nil && ys != &Nil; i += 2, xs = xs->tail, ys = ys->tail) {
    if (i % CHUNK == 0) {
      GC_malloc(false, CHUNK * sizeof(void*));
      growingArray->header.size += CHUNK;
    }
    growingArray->values[i] = xs->head;
    growingArray->values[i + 1] = ys->head;
  }

  Cons* head = &Nil;
  for (i -= 2; i >= 0; i -= 2) {
    void* y = growingArray->values[i + 1];
    void* x_ = growingArray->values[i];
    void* result = A2(f, x_, y);
    head = NEW_CONS(result, head);
  }
  return head;
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
