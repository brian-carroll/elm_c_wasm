#include "../core.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../gc/internals.h"
#include "../../json/json.h" // for pretty printing JSON values

#include "./log.c"
#include "./debug-c.c"
#include "./debug-elm.c"

#if PERF_TIMER_ENABLED
#include "./debug-perf.c"
#endif
