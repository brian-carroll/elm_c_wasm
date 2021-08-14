#include "core.h"

#include "../../lib/stb/stb_sprintf.c"

#include "basics.c"
#include "bitwise.c"
#include "char.c"
#include "dynamic-array.c"
#include "js-array.c"
#include "list.c"
#include "platform.c"
#include "process.c"
#include "queue.c"
#include "scheduler.c"
#include "string.c"
#include "types.c"
#include "utils.c"

#include "debug/debug.c"
#include "gc/gc.c"
#include "string-builder.c"

#if PERF_TIMER_ENABLED
PerfCounter perf_counters[__COUNTER__];
u32 perf_counters_len = ARRAY_LEN(perf_counters);
#endif
