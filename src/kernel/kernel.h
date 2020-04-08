#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#else
#define EMSCRIPTEN_KEEPALIVE
#endif

#include "./basics.h"
#include "./char.h"
#include "./debug.h"
#include "./gc.h"
#include "./list.h"
#include "./maybe.h"
#include "./string.h"
#include "./types.h"
#include "./utils.h"
#include "./wrapper.h"
