#include "./core.h"

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#else
#define EMSCRIPTEN_KEEPALIVE
#endif


// Forward declarations

extern void Platform_stepper(void* model);  // imported JS function (wrapper around view)
void Platform_enqueueEffects(Custom* managers, ManagerMsg* cmdBag, ManagerMsg* subBag);
Process* Platform_instantiateManager(ManagerConfig* info, Closure* sendToApp);
void Platform_dispatchEffects(Custom* managers, ManagerMsg* cmdBag, ManagerMsg* subBag);
void Platform_gatherEffects(
    bool isCmd, ManagerMsg* bag, Custom* effectsDict, Cons* taggers);
ManagerMsg* Platform_toEffect(bool isCmd, size_t home, Cons* taggers, void* value);
ManagerMsg* Platform_insert(bool isCmd, ManagerMsg* newEffect, ManagerMsg* effects);
void* Platform_setupOutgoingPort(ElmString* name);
void* Platform_setupIncomingPort(ElmString* name, Closure* sendToApp);
Cons* Platform_setupEffects(Custom* managers, Closure* sendToApp);


// Globals

ElmValue* Platform_model;
ManagerMsg* Platform_initCmd;
Closure* Platform_update;
Closure* Platform_subscriptions;
Custom* Platform_effectManagers;  // configs
Custom* Platform_managers;        // Processes


void Platform_initOnIntercept(Closure* update, Closure* subscriptions) {
  Platform_update = update;
  Platform_subscriptions = subscriptions;
  GC_register_root((void**)&Platform_model);
  GC_register_root((void**)&Platform_update);
  GC_register_root((void**)&Platform_subscriptions);
}


void* eval_Platform_initialize_sendToApp(void* args[]) {
  void* msg = args[0];
  Tuple2* pair = A2(Platform_update, msg, Platform_model);
  Platform_model = pair->a;
  Platform_stepper(Platform_model);
  void* cmd = pair->b;
  void* sub = A1(Platform_subscriptions, Platform_model);
  Platform_enqueueEffects(Platform_managers, cmd, sub);
  return NULL;
}
Closure sendToApp = {
    .header = HEADER_CLOSURE(0),
    .evaluator = eval_Platform_initialize_sendToApp,
    .max_values = 1,
};


Cons* Platform_initializeEffects() {
  if (!Platform_subscriptions || !Platform_model) {
    safe_printf(
      "To run a WebAssembly app, you need to pass your program configuration record through "
      "WebAssembly.intercept before constructing the Program. For example:\n"
      "\n"
      "    Browser.application <|\n"
      "        WebAssembly.intercept\n"
      "            { init = init\n"
      "            , onUrlChange = onUrlChange\n"
      "            , onUrlRequest = onUrlRequest\n"
      "            , subscriptions = subscriptions\n"
      "            , update = update\n"
      "            , view = view\n"
      "            }\n"
    );
    exit(EXIT_FAILURE);
  }
  Platform_managers = newCustom(KERNEL_CTOR_OFFSET, Platform_managers_size, NULL);
  GC_register_root((void**)&Platform_managers);
  Closure* sendToApp = newClosure(0, 1, eval_Platform_initialize_sendToApp, NULL);
  Cons* portsList = Platform_setupEffects(Platform_managers, sendToApp);
  ManagerMsg* sub = A1(Platform_subscriptions, Platform_model);
  Platform_enqueueEffects(Platform_managers, Platform_initCmd, sub);
  Platform_initCmd = NULL;
  return portsList;
}


/* ====================================================

                 EFFECT MANAGERS

   ==================================================== */

Cons* Platform_setupEffects(Custom* managers, Closure* sendToApp) {
  Cons* portsList = &Nil;

  for (u32 i = 0; i < Platform_managers_size; i++) {
    ManagerConfig* manager = Platform_effectManagers->values[i];

    if (manager->ctor == MANAGER_PORT_OUT) {
      PortConfig* port = (PortConfig*)manager;
      Tuple2* keyValuePair =
          newTuple2(port->name, Platform_setupOutgoingPort(port->name));
      portsList = newCons(keyValuePair, portsList);
    } else if (manager->ctor == MANAGER_PORT_IN) {
      PortConfig* port = (PortConfig*)manager;
      Tuple2* keyValuePair =
          newTuple2(port->name, Platform_setupIncomingPort(port->name, sendToApp));
      portsList = newCons(keyValuePair, portsList);
    }

    managers->values[i] = Platform_instantiateManager(manager, sendToApp);
  }

  return portsList;
}


ManagerConfig* Platform_createManager(void* init,
    Closure* onEffects,
    Closure* onSelfMsg,
    Closure* cmdMap,
    Closure* subMap) {
  const u32 size = sizeof(ManagerConfig) / SIZE_UNIT;
  ManagerConfig* manager = GC_allocate(true, size);
  manager->header = (Header){.tag = Tag_Custom, .size = size};
  manager->ctor = KERNEL_CTOR_OFFSET;

  manager->init = init;
  manager->onEffects = onEffects;
  manager->onSelfMsg = onSelfMsg;
  manager->cmdMap = cmdMap;
  manager->subMap = subMap;

  return manager;
}


static void* eval_manager_loop_receive(void* args[]) {
  ManagerConfig* info = args[0];
  Router* router = args[1];
  void* state = args[2];
  ManagerMsg* msg = args[3];

  if (msg->ctor == MANAGER_MSG_SELF) {
    return A3(info->onSelfMsg, router, msg->self.msg, state);
  } else if (info->cmdMap && info->subMap) {
    return A4(info->onEffects, router, msg->fx.cmds, msg->fx.subs, state);
  } else if (info->cmdMap) {
    return A3(info->onEffects, router, msg->fx.cmds, state);
  } else {
    return A3(info->onEffects, router, msg->fx.subs, state);
  }
}


static void* eval_manager_loop(void* args[]) {
  // ManagerConfig* info = args[0];
  // Router* router = args[1];
  // void* state = args[2];
  Closure* loop = newClosure(2, 3, eval_manager_loop, args);
  Closure* receive = newClosure(3, 4, eval_manager_loop_receive, args);
  Task* receiveTask = eval_Scheduler_receive((void*[]){receive});
  Task* loopTask = eval_Scheduler_andThen((void*[]){loop, receiveTask});
  return loopTask;
}


Process* Platform_instantiateManager(ManagerConfig* info, Closure* sendToApp) {
  const u32 size = sizeof(Router) / SIZE_UNIT;
  Router* router = GC_allocate(true, size);
  router->header = (Header){.tag = Tag_Custom, .size = size};
  router->ctor = KERNEL_CTOR_OFFSET;
  router->sendToApp = sendToApp;

  Closure* loop = newClosure(2, 3, eval_manager_loop, (void*[]){info, router});
  Task* initTask = info->init;
  Task* loopTask = eval_Scheduler_andThen((void*[]){loop, initTask});
  router->selfProcess = eval_Scheduler_rawSpawn((void*[]){loopTask});

  return router->selfProcess;
}


/* ====================================================

                 ROUTING

   ==================================================== */


static void* eval_Platform_sendToApp_lambda(void* args[]) {
  Router* router = args[0];
  void* msg = args[1];
  Closure* callback = args[2];

  A1(router->sendToApp, msg);
  A1(callback, eval_Scheduler_succeed(&pUnit));
  return NULL;
}
void* eval_Platform_sendToApp(void* args[]) {
  Closure* lambda = newClosure(2, 3, eval_Platform_sendToApp_lambda, args);
  return eval_Scheduler_binding((void*[]){lambda});
}
Closure Platform_sendToApp = {
    .header = HEADER_CLOSURE(0),
    .max_values = 2,
    .evaluator = eval_Platform_sendToApp,
};


void* eval_Platform_sendToSelf(void* args[]) {
  Router* router = args[0];
  void* msg = args[1];
  return eval_Scheduler_send((void*[]){
      router->selfProcess,
      newCustom(MANAGER_MSG_SELF, 1, &msg),
  });
}
Closure Platform_sendToSelf = {
    .header = HEADER_CLOSURE(0),
    .max_values = 2,
    .evaluator = eval_Platform_sendToSelf,
};


/* ====================================================

                 BAGS

   ==================================================== */

void* eval_Platform_leaf(void* args[]) {
  return newCustom(MANAGER_MSG_LEAF, 2, args);
}
Closure Platform_leaf = {
    .header = HEADER_CLOSURE(0),
    .max_values = 2,
    .evaluator = eval_Platform_leaf,
};


void* eval_Platform_batch(void* args[]) {
  return newCustom(MANAGER_MSG_NODE, 1, args);
}
Closure Platform_batch = {
    .header = HEADER_CLOSURE(0),
    .max_values = 1,
    .evaluator = eval_Platform_batch,
};


void* eval_Platform_map(void* args[]) {
  return newCustom(MANAGER_MSG_MAP, 2, args);
}
Closure Platform_map = {
    .header = HEADER_CLOSURE(0),
    .max_values = 2,
    .evaluator = eval_Platform_map,
};


/* ====================================================

          PIPE BAGS INTO EFFECT MANAGERS

   ==================================================== */


// The queue is necessary to avoid ordering issues for synchronous commands.
// Don't try to 'cleverly' skip the queue in some cases! See comments in Platform.js
bool Platform_effectsActive = false;
Queue Platform_effectsQueue = {
    .front = &Nil,
    .back = &Nil,
};


void Platform_enqueueEffects(Custom* managers, ManagerMsg* cmdBag, ManagerMsg* subBag) {
  Queue_push(&Platform_effectsQueue, newTuple3(managers, cmdBag, subBag));

  if (Platform_effectsActive) return;

  Platform_effectsActive = true;
  for (;;) {
    Tuple3* fx = Queue_shift(&Platform_effectsQueue);
    if (!fx) break;
    Platform_dispatchEffects(fx->a, fx->b, fx->c);
  }
  Platform_effectsActive = false;
}


void Platform_dispatchEffects(Custom* managers, ManagerMsg* cmdBag, ManagerMsg* subBag) {
  u32 n_managers = custom_params(managers);
  Custom* effectsDict = newCustom(KERNEL_CTOR_OFFSET, n_managers, NULL);
  for (u32 i = 0; i < n_managers; i++) {
    effectsDict->values[i] = NULL;
  }
  Platform_gatherEffects(true, cmdBag, effectsDict, &Nil);
  Platform_gatherEffects(false, subBag, effectsDict, &Nil);
  for (u32 home = 0; home < n_managers; home++) {
    Process* managerProc = managers->values[home];
    ManagerMsg* msg;
    if (effectsDict->values[home]) {
      msg = effectsDict->values[home];
    } else {
      msg = (ManagerMsg*)newCustom(MANAGER_MSG_FX, 2, (void*[]){&Nil, &Nil});
    }
    eval_Scheduler_rawSend((void*[]){managerProc, msg});
  }
}


void Platform_gatherEffects(
    bool isCmd, ManagerMsg* bag, Custom* effectsDict, Cons* taggers) {
  switch (bag->ctor) {
    case MANAGER_MSG_LEAF: {
      size_t home = bag->leaf.home;
      ManagerMsg* effect = Platform_toEffect(isCmd, home, taggers, bag->leaf.value);
      effectsDict->values[home] =
          Platform_insert(isCmd, effect, effectsDict->values[home]);
      return;
    }
    case MANAGER_MSG_NODE: {
      for (Cons* list = bag->node.bags; list != &Nil; list = list->tail) {
        Platform_gatherEffects(isCmd, list->head, effectsDict, taggers);
      }
      return;
    }
    case MANAGER_MSG_MAP: {
      Cons* nextTaggers = newCons(bag->map.func, taggers);
      Platform_gatherEffects(isCmd, bag->map.bag, effectsDict, nextTaggers);
      return;
    }
    default: {
      log_error(
          "ERROR in Platform_gatherEffects, unknown effect constructor %d\n", bag->ctor);
    }
  }
}


void* eval_applyTaggers(void* args[]) {
  Cons* taggers = args[0];
  void* x = args[1];
  for (Cons* temp = taggers; temp != &Nil; temp = temp->tail) {
    Closure* tagger = temp->head;
    x = A1(tagger, x);
  }
  return x;
}
ManagerMsg* Platform_toEffect(bool isCmd, size_t home, Cons* taggers, void* value) {
  Closure* applyTaggers = newClosure(1, 2, eval_applyTaggers, (void*[]){taggers});
  ManagerConfig* manager = Platform_effectManagers->values[home];
  Closure* map = isCmd ? manager->cmdMap : manager->subMap;
  ManagerMsg* result = A2(map, applyTaggers, value);
  return result;
}


ManagerMsg* Platform_insert(bool isCmd, ManagerMsg* newEffect, ManagerMsg* effects) {
  if (!effects) {
    effects = (ManagerMsg*)newCustom(MANAGER_MSG_FX, 2, (void*[]){&Nil, &Nil});
  }
  if (isCmd) {
    effects->fx.cmds = newCons(newEffect, effects->fx.cmds);
  } else {
    effects->fx.subs = newCons(newEffect, effects->fx.subs);
  }
  return effects;
};


/* ====================================================

                 PORTS

   ==================================================== */


void* Platform_setupOutgoingPort(ElmString* name) {
  return NULL;  // TODO
}

void* Platform_setupIncomingPort(ElmString* name, Closure* sendToApp) {
  return NULL;  // TODO
}
