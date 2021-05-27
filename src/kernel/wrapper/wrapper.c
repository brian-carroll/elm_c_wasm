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
 * Cache Wasm values to minimise encoding/decoding overhead
 *
 * JS must only use cache indices, not memory addresses,
 * since GC compactor can move values around in the heap.
 */

Custom* cache;
u32 cache_size = 64;


EMSCRIPTEN_KEEPALIVE u32 addToCache(ElmValue* value) {
  u32 index = 0;
  for (; index < cache_size; index++) {
    if (cache->values[index] == NULL) {
      break;
    }
    if (cache->values[index] == value) {
      return index;
    }
    if (A2(&Utils_equal, cache->values[index], value) == &True) {
      return index;
    }
  }
  if (index == cache_size) {
    const u32 HUGE = 1024;
    u32 extra = cache_size > HUGE ? HUGE : cache_size;
    u32 new_cache_size = cache_size + extra;
    Custom* new_cache = newCustom(JSON_VALUE_ARRAY, new_cache_size, NULL);
    for (int i = 0; i < cache_size; i++) {
      new_cache->values[i] = cache->values[i];
    }
    for (int i = cache_size; i < new_cache_size; i++) {
      new_cache->values[i] = NULL;
    }
    cache = new_cache;
    cache_size = new_cache_size;
    safe_printf("Grew wrapper cache to %d\n", new_cache_size);
  }
  cache->values[index] = value;
  return index;
}


EMSCRIPTEN_KEEPALIVE void* retrieveFromCache(u32 index) {
  if (index > cache_size) {
    return NULL;
  }
  return cache->values[index];
}


/*
 * Intercept Elm Architecture function calls
 * Keep the app model in Wasm to minimise encoding/decoding overhead
 */

ElmValue* model;


static void* eval_wrapInit(void* args[]) {
  Closure* userInit = args[0];
  u16 initArity = userInit->max_values - userInit->n_values;
  Tuple2* modelAndCmd = Utils_apply(userInit, initArity, &args[1]);
  model = modelAndCmd->a;  // initialise Wasm model
  void* cmd = modelAndCmd->b;

  // DEBUG_PRETTY(model);
  // DEBUG_PRETTY(cmd);

  return newTuple2(&Unit, cmd);  // return Cmds to JS
}


static void* eval_wrapUpdate(void* args[]) {
  Closure* userUpdate = args[0];
  void* msg = args[1];

  // DEBUG_PRETTY(msg);
  // DEBUG_PRETTY(model);

  Tuple2* modelAndCmd = A2(userUpdate, msg, model);
  model = modelAndCmd->a;  // update Wasm model
  void* cmd = modelAndCmd->b;

  // DEBUG_PRETTY(model);
  // DEBUG_PRETTY(cmd);

  return newTuple2(&Unit, cmd);  // return Cmds to JS
}


static void* eval_wrapSubs(void* args[]) {
  Closure* userSubs = args[0];
  // Use Wasm model, ignore JS dummy model at args[1]
  return A1(userSubs, model);
}


static void* eval_wrapView(void* args[]) {
  Closure* userView = args[0];
  // Use Wasm model, ignore JS dumy model at args[1]
  return A1(userView, model);
}


// Elm function that the user has to insert in front of the Program constructor
static void* eval_intercept(void* args[]) {
  GC_register_root((void**)&model);
  GC_register_root((void**)&cache);
  cache = newCustom(KERNEL_CTOR_OFFSET, cache_size, NULL);
  for (int i = 0; i < cache_size; i++) {
    cache->values[i] = NULL;
  }

  Record* userTEA = args[0];
  Closure* userInit = Utils_access_eval((void*[]){(void*)(size_t)FIELD_init, userTEA});
  Closure* userUpdate =
      Utils_access_eval((void*[]){(void*)(size_t)FIELD_update, userTEA});
  Closure* userSubs =
      Utils_access_eval((void*[]){(void*)(size_t)FIELD_subscriptions, userTEA});
  Closure* userView = Utils_access_eval((void*[]){(void*)(size_t)FIELD_view, userTEA});

  // Init has different arity for different Program constructors
  u16 initArity = userInit->max_values - userInit->n_values;
  Closure* wrappedInit = newClosure(1, 1 + initArity, eval_wrapInit, (void*[]){userInit});
  Closure* wrappedUpdate = newClosure(1, 3, eval_wrapUpdate, (void*[]){userUpdate});
  Closure* wrappedSubs = newClosure(1, 2, eval_wrapSubs, (void*[]){userSubs});
  Closure* wrappedView = newClosure(1, 2, eval_wrapView, (void*[]){userView});

  Record* wrappedTEA = Utils_update(userTEA,
      4,
      (u32[]){FIELD_init, FIELD_update, FIELD_subscriptions, FIELD_view},
      (void*[]){wrappedInit, wrappedUpdate, wrappedSubs, wrappedView});

  // wrappedTEA will become part of the `main` thunk, which is a GC root.
  return wrappedTEA;
}
Closure g_author_project_WebAssembly_intercept = {
    .header = HEADER_CLOSURE(0),
    .max_values = 1,
    .evaluator = eval_intercept,
};
