#include <assert.h>
#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#else
#define EMSCRIPTEN_KEEPALIVE
#endif
#include "../core/core.h"
#include "../core/gc/internals.h"
#include "../json/json.h"
#include "wrapper.h"

extern GcState gc_state;

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
  Closure* c = (Closure*)addr;
  void* result = GC_execute(c);
  return (size_t)result;
}

EMSCRIPTEN_KEEPALIVE void collectGarbage() {
  GC_collect_major();
}

EMSCRIPTEN_KEEPALIVE void debugHeapState() {
  mark(&gc_state, gc_state.heap.start);
  print_heap();
  print_state();
}

EMSCRIPTEN_KEEPALIVE void debugAddrRange(size_t start, size_t len) {
  print_heap_range((void*)start, (void*)(start + len));
}

EMSCRIPTEN_KEEPALIVE void debugEvaluatorName(size_t addr) {
  safe_printf("\nevaluator %zd: %s\n\n", addr, Debug_evaluator_name((void*)addr));
}

EMSCRIPTEN_KEEPALIVE void debugStackMap() {
  print_stack_map();
}
