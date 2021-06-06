#include "./core.h"

void Queue_push(Queue* q, void* value) {
  Cons* newBack = newCons(value, &Nil);
  if (q->back != &Nil) {
    q->back->tail = newBack;
  }
  q->back = newBack;
  if (q->front == &Nil) {
    q->front = newBack;
  }
}

void* Queue_shift(Queue* q) {
  if (q->front == &Nil) {
    return NULL;
  }
  void* out = q->front->head;
  q->front = q->front->tail;
  return out;
}
