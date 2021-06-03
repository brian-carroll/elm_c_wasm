#include "./core.h"
#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#else
#define EMSCRIPTEN_KEEPALIVE
#endif

void* Platform_model;
Closure* Platform_update;
Closure* Platform_subscriptions;


EMSCRIPTEN_KEEPALIVE void Platform_initialize(
    Closure* model, Closure* update, Closure* subscriptions) {
  GC_register_root(&Platform_model);
  GC_register_root(&(void*)Platform_update);
  GC_register_root(&(void*)Platform_subscriptions);

  Platform_model = model;
  Platform_update = update;
  Platform_subscriptions = subscriptions;
}

void* eval_Platform_initialize_sendToApp(void* args[]) {
  Closure* update = ? ? ;


  var pair = A2(update, msg, model);
  stepper(model = pair.a);
  _Platform_enqueueEffects(managers, pair.b, subscriptions(model));
}
