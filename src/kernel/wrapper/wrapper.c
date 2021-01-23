#include <assert.h>
#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#include <stdio.h>
#else
#define EMSCRIPTEN_KEEPALIVE
#include <stdio.h>
#endif
#include "../core/core.h"
#include "../core/gc-internals.h"
#include "../json/json.h"
#include "wrapper.h"

extern GcState gc_state;

u32 field_init;
u32 field_update;
u32 field_subscriptions;
u32 field_view;

// Call from C main
void Wrapper_main(u32 init, u32 update, u32 subscriptions, u32 view) {
  field_init = init;
  field_update = update;
  field_subscriptions = subscriptions;
  field_view = view;
  for (size_t i = 0; Wrapper_modelsArray[i]; i++) {
    GC_register_root(Wrapper_modelsArray + i);
  }
}

/*
  API exposed to JS side of wrapper
*/

EMSCRIPTEN_KEEPALIVE size_t getMains() {
  return (size_t)Wrapper_mainsArray;
};

EMSCRIPTEN_KEEPALIVE size_t getFieldGroups() {
  return (size_t)Wrapper_appFieldGroups;
}

EMSCRIPTEN_KEEPALIVE size_t getUnit() {
  return (size_t)&Unit;
}

EMSCRIPTEN_KEEPALIVE size_t getNil() {
  return (size_t)&Nil;
}

EMSCRIPTEN_KEEPALIVE size_t getTrue() {
  return (size_t)&True;
}

EMSCRIPTEN_KEEPALIVE size_t getFalse() {
  return (size_t)&False;
}

EMSCRIPTEN_KEEPALIVE size_t getJsNull() {
  return (size_t)&Json_encodeNull;
}

EMSCRIPTEN_KEEPALIVE size_t getMaxWriteAddr() {
  return (size_t)gc_state.heap.end;
}

EMSCRIPTEN_KEEPALIVE size_t getWriteAddr() {
  return (size_t)gc_state.next_alloc;
}

EMSCRIPTEN_KEEPALIVE void finishWritingAt(size_t addr) {
  gc_state.next_alloc = (size_t*)addr;
}

EMSCRIPTEN_KEEPALIVE f64 readF64(size_t addr) {
  f64* ptr = (f64*)addr;
  return *ptr;
}

EMSCRIPTEN_KEEPALIVE void writeF64(size_t addr, f64 value) {
  f64* ptr = (f64*)addr;
  *ptr = value;
}

EMSCRIPTEN_KEEPALIVE size_t evalClosure(size_t addr) {
  GC_stack_empty();
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

EMSCRIPTEN_KEEPALIVE void collectGarbage() {
  GC_collect_full();
}

EMSCRIPTEN_KEEPALIVE void debugHeapState() {
  mark(&gc_state, gc_state.heap.start);
  print_heap();
  print_state();
}

EMSCRIPTEN_KEEPALIVE void debugAddrRange(size_t start, size_t len) {
  print_heap_range((void*)start, (void*)start + len);
}

EMSCRIPTEN_KEEPALIVE void debugEvaluatorName(size_t addr) {
  printf("%s\n", Debug_evaluator_name((void*)addr));
}

static Closure* copyTeaFunction(size_t mainIdx, u32 fieldId) {
  Closure** mainRoot = Wrapper_mainsArray[mainIdx];
  Closure* main = *mainRoot;
  Record* tea = main->values[0];
  Closure* fn = Utils_access_eval((void*[]){fieldId, tea});
  Closure* copy = Utils_clone(fn);
  copy->n_values = copy->max_values;
  copy->header.size = SIZE_CLOSURE(copy->max_values);
  return copy;
}

EMSCRIPTEN_KEEPALIVE size_t copyInit(size_t mainIdx) {
  return (size_t)copyTeaFunction(mainIdx, field_init);
}

EMSCRIPTEN_KEEPALIVE size_t copyUpdate(size_t mainIdx) {
  return (size_t)copyTeaFunction(mainIdx, field_update);
}

EMSCRIPTEN_KEEPALIVE size_t copySubs(size_t mainIdx) {
  return (size_t)copyTeaFunction(mainIdx, field_subscriptions);
}

EMSCRIPTEN_KEEPALIVE size_t copyView(size_t mainIdx) {
  return (size_t)copyTeaFunction(mainIdx, field_view);
}

EMSCRIPTEN_KEEPALIVE size_t getModel(size_t mainIdx) {
  return (size_t)Wrapper_modelsArray[mainIdx];
}

EMSCRIPTEN_KEEPALIVE void setModel(size_t mainIdx, size_t modelAddr) {
  Wrapper_modelsArray[mainIdx] = (void*)modelAddr;
}
