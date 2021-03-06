#include "core.h"

#include "../../lib/stb/stb_sprintf.c"

#include "basics.c"
#include "bitwise.c"
#include "char.c"
#include "js-array.c"
#include "list.c"
#include "string.c"
#include "types.c"
#include "utils.c"
#include "gc/gc.c"
#include "string-builder.c"
#include "debug/debug.c"

#if PERF_TIMER_ENABLED
PerfCounter perf_counters[__COUNTER__];
u32 perf_counters_len = ARRAY_LEN(perf_counters);
#endif
