#include <assert.h>
#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#else
#define EMSCRIPTEN_KEEPALIVE
#endif
#include "./debug.h"
#include "./gc-internals.h"
#include "./gc.h"
#include "./types.h"
#include "./utils.h"

extern GcState gc_state;

FieldGroup** appFieldGroups;
Record* mainRecord;

/*
    API exposed to C app
*/
void wrapper_register_mainRecord(Record* mainRecordFromApp) {
  mainRecord = mainRecordFromApp;
}

void wrapper_register_fieldGroups(FieldGroup** fgArrayFromApp) {
  appFieldGroups = fgArrayFromApp;
}

/*
  API exposed to JS side of wrapper
*/

size_t EMSCRIPTEN_KEEPALIVE getMainRecord() {
  return (size_t)mainRecord;
};

size_t nextFieldGroup = 0;
size_t EMSCRIPTEN_KEEPALIVE getNextFieldGroup() {
  assert(appFieldGroups != NULL);
  FieldGroup* next = appFieldGroups[nextFieldGroup];
  if (next != NULL) nextFieldGroup++;
  return (size_t)next;
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
  return (size_t)Utils_apply((Closure*)addr, 0, NULL);
}

void EMSCRIPTEN_KEEPALIVE collectGarbage() {
  GC_collect_full();
}

void EMSCRIPTEN_KEEPALIVE debugHeapState() {
  print_heap();
  print_state();
}
