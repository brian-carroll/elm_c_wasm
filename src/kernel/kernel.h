#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#else
#define EMSCRIPTEN_KEEPALIVE
#endif

#include "core/core.h"
#include "json/json.h"
#include "wrapper/wrapper.h"
#include "elm-test/elm-test.h"
