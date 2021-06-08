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

EMSCRIPTEN_KEEPALIVE void* allocate(size_t size) {
  return GC_allocate(true, size);
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

EMSCRIPTEN_KEEPALIVE void* get_Scheduler_rawSpawn() {
  return &eval_Scheduler_rawSpawn;
}

EMSCRIPTEN_KEEPALIVE void* get_Scheduler_spawn() {
  return &eval_Scheduler_spawn;
}

EMSCRIPTEN_KEEPALIVE void* get_sendToApp() {
  return &sendToApp;
}

EMSCRIPTEN_KEEPALIVE Cons* initializeEffects() {
  return Platform_initializeEffects();
}


// DEBUG EXPORTS


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


/*
 * Intercept Elm Architecture function calls
 * Keep the app model in Wasm to minimise encoding/decoding overhead
 */

// ElmValue* model;
extern ElmValue* Platform_model;
extern ManagerMsg* Platform_initCmd;

static void* eval_wrapInit(void* args[]) {
  Closure* userInit = args[0];
  u16 initArity = userInit->max_values - userInit->n_values;
  Tuple2* modelAndCmd = Utils_apply(userInit, initArity, &args[1]);
  Platform_model = modelAndCmd->a;
  Platform_initCmd = modelAndCmd->b;

  // DEBUG_PRETTY(Platform_model);
  // DEBUG_PRETTY(Platform_initCmd);

  return &Unit;
}


static void* eval_wrapView(void* args[]) {
  Closure* userView = args[0];
  // Use Wasm model, ignore JS dumy model at args[1]
  return A1(userView, Platform_model);
}


// Elm function that the user has to insert in front of the Program constructor
static void* eval_intercept(void* args[]) {
  Record* userTEA = args[0];

  size_t FIELD_init = fg_init_subscriptions_update_view.fields[0];
  size_t FIELD_subscriptions = fg_init_subscriptions_update_view.fields[1];
  size_t FIELD_update = fg_init_subscriptions_update_view.fields[2];
  size_t FIELD_view = fg_init_subscriptions_update_view.fields[3];

  Closure* userInit = Utils_access_eval((void*[]){(void*)(size_t)FIELD_init, userTEA});
  Closure* userUpdate =
      Utils_access_eval((void*[]){(void*)(size_t)FIELD_update, userTEA});
  Closure* userSubs =
      Utils_access_eval((void*[]){(void*)(size_t)FIELD_subscriptions, userTEA});
  Closure* userView = Utils_access_eval((void*[]){(void*)(size_t)FIELD_view, userTEA});

  // Init has different arity for different Program constructors
  u16 initArity = userInit->max_values - userInit->n_values;
  Closure* wrappedInit = newClosure(1, 1 + initArity, eval_wrapInit, (void*[]){userInit});
  Closure* wrappedView = newClosure(1, 2, eval_wrapView, (void*[]){userView});

  // init and view are both wrapped in Browser.js
  Record* wrappedTEA = Utils_update(
      userTEA, 2, (u32[]){FIELD_init, FIELD_view}, (void*[]){wrappedInit, wrappedView});

  Platform_initOnIntercept(userUpdate, userSubs);

  // wrappedTEA will become part of the `main` thunk, which is a GC root.
  return wrappedTEA;
}
Closure g_author_project_WebAssembly_intercept = {
    .header = HEADER_CLOSURE(0),
    .max_values = 1,
    .evaluator = eval_intercept,
};
