#include <assert.h>
#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#else
#define EMSCRIPTEN_KEEPALIVE
#include <stdio.h>
#endif
#include "../debug.h"
#include "../gc-internals.h"
#include "../gc.h"
#include "../types.h"
#include "../utils.h"
#include "./wrapper.h"

extern GcState gc_state;

/*
  API exposed to JS side of wrapper
*/

size_t mainsIndex = 0;
size_t EMSCRIPTEN_KEEPALIVE getNextMain() {
  assert(Wrapper_mainsArray != NULL);
  void** mainGcRoot = Wrapper_mainsArray[mainsIndex];
  if (mainGcRoot == NULL) {
    return 0;
  }
  mainsIndex++;
  void* heapVal = *mainGcRoot;
  return (size_t)heapVal;
};

size_t fgIndex = 0;
size_t EMSCRIPTEN_KEEPALIVE getNextFieldGroup() {
  assert(Wrapper_appFieldGroups != NULL);
  FieldGroup* fg = Wrapper_appFieldGroups[fgIndex];
  if (fg != NULL) fgIndex++;
  return (size_t)fg;
}

size_t EMSCRIPTEN_KEEPALIVE getUnit() {
  return (size_t)&Unit;
}
size_t EMSCRIPTEN_KEEPALIVE getNil() {
  return (size_t)&Nil;
}
size_t EMSCRIPTEN_KEEPALIVE getTrue() {
  return (size_t)&True;
}
size_t EMSCRIPTEN_KEEPALIVE getFalse() {
  return (size_t)&False;
}
Custom JsNull = {
    .header = HEADER_CUSTOM(0),
    .ctor = 0,
};
size_t EMSCRIPTEN_KEEPALIVE getJsNull() {
  return (size_t)&JsNull;
}

size_t EMSCRIPTEN_KEEPALIVE getMaxWriteAddr() {
  return (size_t)gc_state.heap.end;
}
size_t EMSCRIPTEN_KEEPALIVE getWriteAddr() {
  return (size_t)gc_state.next_alloc;
}
void EMSCRIPTEN_KEEPALIVE finishWritingAt(size_t addr) {
  gc_state.next_alloc = (size_t*)addr;
}

f64 EMSCRIPTEN_KEEPALIVE readF64(size_t addr) {
  f64* ptr = (f64*)addr;
  return *ptr;
}
void EMSCRIPTEN_KEEPALIVE writeF64(size_t addr, f64 value) {
  f64* ptr = (f64*)addr;
  *ptr = value;
}

size_t EMSCRIPTEN_KEEPALIVE evalClosure(size_t addr) {
  for (size_t attempts = 0; attempts < 1000; attempts++) {
    void* result = Utils_apply((Closure*)addr, 0, NULL);  // addr ignored on replay
    if (result != pGcFull) {
      GC_stack_empty();
      return (size_t)result;
    }
    GC_collect_full();
    GC_prep_replay();
  }
  assert(0);
}

void EMSCRIPTEN_KEEPALIVE collectGarbage() {
  GC_collect_full();
}

extern GcState gc_state;
void EMSCRIPTEN_KEEPALIVE debugHeapState() {
  mark(&gc_state, gc_state.heap.start);
  print_heap();
  print_state();
}

void EMSCRIPTEN_KEEPALIVE debugEvaluatorName(size_t addr) {
  printf("%s\n", Debug_evaluator_name((void*)addr));
}

extern size_t evalWasmThunkInJs(size_t addr);

void* Wrapper_callJsSync(size_t jsFnIndex, u16 n_args, void* args[]) {
  Closure* jsThunk = NEW_CLOSURE(n_args, NEVER_EVALUATE, (void*)jsFnIndex, args);
  size_t resultAddr = evalWasmThunkInJs((size_t)jsThunk);
  return (void*)resultAddr;
}
