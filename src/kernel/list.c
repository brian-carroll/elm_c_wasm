#include <stdlib.h>
#include "./gc.h"
#include "./types.h"

// Not a kernel function, will be generated from Elm
void* List_append_eval(void* args[2]) {
  return NULL;
}

// Allocate an entire list at once, with no recursion overhead
// First value in the array becomes the head of the list
void* List_fromArray(size_t len, void* values[]) {
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
