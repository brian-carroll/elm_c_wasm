Closure* gptr_author_project_Main_main;

#ifndef __EMSCRIPTEN__
Cons* mock_js_Platform_initialize() {
  Record* wrappedTEA = gptr_author_project_Main_main->values[0];
  size_t field_init = fg_init_subscriptions_update_view.fields[0];
  Closure* wrappedInit = Utils_access_eval((void*[]){(void*)field_init, wrappedTEA});
  Custom* dummyFlags = &Unit;
  Tuple2* dummyPair = A1(wrappedInit, dummyFlags);
  Cons* portList = Platform_initializeEffects();
  return portList;
}
#else
void mock_js_Platform_initialize() {
  return;
}
#endif
