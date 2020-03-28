#include <stdlib.h>
#include "./gc.h"
#include "./types.h"

// Allocate an entire list at once, with no recursion overhead
// First value in the array becomes the head of the list
void* List_create(size_t len, void* values[]) {
  void* space = GC_malloc(sizeof(Cons) * len);
  if (space == pGcFull) return pGcFull;
  Cons* cells = space;
  Cons* head = &Nil;
  for (size_t i = 0; i < len; ++i) {
    cells[i] = (Cons){
        .header = HEADER_LIST,
        .head = values[len - 1 - i],
        .tail = head,
    };
    head = &cells[i];
  }
  return head;
}

// toArray & fromArray
// These functions shouldn't exist in C but some Elm code in core libs
// contains leaked details of the JS implementation!
// Intended for use with JS built-ins like String.prototype.join()
// Just pass through for now. Maybe some day refactor the core Elm files.
static void* eval_List_identity(void* args[]) {
  return args[0];
}
Closure List_toArray = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_List_identity,
    .max_values = 1,
};
#define List_fromArray List_toArray

// cons

static void* eval_List_cons(void* args[]) {
  return NEW_CONS(args[0], args[1]);
}
Closure List_cons = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_List_cons,
    .max_values = 2,
};

// map2

static void* eval_List_map2(void* args[]) {
  const size_t CHUNK = 8;
  Closure* f = args[0];
  void* xs = args[1];
  void* ys = args[2];

  Custom* tmpArray = GC_malloc(sizeof(Custom));
  if (tmpArray == pGcFull) return pGcFull;
  tmpArray->header = HEADER_CUSTOM(0);

  ptrdiff_t i = 0;
  for (; xs != &Nil && ys != &Nil; i += 2, xs = xs->tail, ys = ys->tail) {
    if (i % CHUNK == 0) {
      if (GC_malloc(CHUNK * sizeof(void*)) == pGcFull) {
        return pGcFull;
      }
      tmpArray->header.size += CHUNK;
    }
    tmpArray->values[i] = xs->head;
    tmpArray->values[i + 1] = ys->head;
  }

  Cons* head = &Nil;
  for (i -= 2; i >= 0; i -= 2) {
    void* x = tmpArray->values[i];
    void* y = tmpArray->values[i + 1];
    void* result = A2(f, x, y);
    head = NEW_CONS(result, head);
  }
  return head;
}
Closure List_map2 = {
    .header = HEADER_CLOSURE(0),
    .evaluator = &eval_List_map2,
    .max_values = 3,
};
