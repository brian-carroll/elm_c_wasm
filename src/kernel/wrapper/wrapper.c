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

EMSCRIPTEN_KEEPALIVE void* getMains() {
  return Wrapper_mainsArray;
};

EMSCRIPTEN_KEEPALIVE void* getFieldGroups() {
  return Wrapper_appFieldGroups;
}

EMSCRIPTEN_KEEPALIVE void* getUnit() {
  return &Unit;
}

EMSCRIPTEN_KEEPALIVE void* getNil() {
  return &Nil;
}

EMSCRIPTEN_KEEPALIVE void* getTrue() {
  return &True;
}

EMSCRIPTEN_KEEPALIVE void* getFalse() {
  return &False;
}

EMSCRIPTEN_KEEPALIVE void* getJsNull() {
  return &Json_encodeNull;
}

EMSCRIPTEN_KEEPALIVE void* getMaxWriteAddr() {
  return gc_state.heap.end;
}

EMSCRIPTEN_KEEPALIVE void* getWriteAddr() {
  return gc_state.next_alloc;
}

EMSCRIPTEN_KEEPALIVE void finishWritingAt(size_t* addr) {
  gc_state.next_alloc = addr;
}

EMSCRIPTEN_KEEPALIVE f64 readF64(f64* addr) {
  return *addr;
}

EMSCRIPTEN_KEEPALIVE void writeF64(f64* addr, f64 value) {
  *addr = value;
}

EMSCRIPTEN_KEEPALIVE void* evalClosure(Closure* c) {
  return GC_execute(c);
}

EMSCRIPTEN_KEEPALIVE void collectGarbage() {
  GC_collect_minor();
}

EMSCRIPTEN_KEEPALIVE void debugHeapState() {
  mark(&gc_state, gc_state.heap.start);
  print_heap();
  print_state();
}

EMSCRIPTEN_KEEPALIVE void debugAddrRange(size_t* start, size_t len) {
  print_heap_range(start, start + len);
}

EMSCRIPTEN_KEEPALIVE void debugEvaluatorName(void* addr) {
  safe_printf("\nevaluator %zd: %s\n\n", addr, Debug_evaluator_name(addr));
}

EMSCRIPTEN_KEEPALIVE void debugStackMap() {
  print_stack_map();
}
