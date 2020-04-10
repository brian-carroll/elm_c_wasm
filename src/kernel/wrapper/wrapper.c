#include <assert.h>
#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#else
#define EMSCRIPTEN_KEEPALIVE
#endif
#include "../debug.h"
#include "../gc-internals.h"
#include "../gc.h"
#include "../types.h"
#include "../utils.h"

extern GcState gc_state;

FieldGroup** appFieldGroups;
void*** mainsArray;

/*
    API exposed to C app
*/

void Wrapper_registerMains(void** mainsArrayFromApp[]) {
  mainsArray = mainsArrayFromApp;
}

void Wrapper_registerFieldGroups(FieldGroup** fgArrayFromApp) {
  appFieldGroups = fgArrayFromApp;
}

/*
  API exposed to JS side of wrapper
*/

size_t mainsIndex = 0;
size_t EMSCRIPTEN_KEEPALIVE getNextMain() {
  assert(mainsArray != NULL);
  void** mainGcRoot = mainsArray[mainsIndex];
  if (mainGcRoot != NULL) mainsIndex++;
  void* heapVal = *mainGcRoot;
  return (size_t)heapVal;
};

size_t fgIndex = 0;
size_t EMSCRIPTEN_KEEPALIVE getNextFieldGroup() {
  assert(appFieldGroups != NULL);
  FieldGroup* fg = appFieldGroups[fgIndex];
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
      return (size_t)result;
    }
    GC_collect_full();
    GC_start_replay();
  }
  assert(0);
}

void EMSCRIPTEN_KEEPALIVE collectGarbage() {
  GC_collect_full();
}

void EMSCRIPTEN_KEEPALIVE debugHeapState() {
  print_heap();
  print_state();
}