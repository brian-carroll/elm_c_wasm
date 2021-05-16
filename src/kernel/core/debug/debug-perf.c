#include "debug.h"
#include "string.h"  // strcmp

PerfCounter* perf_section_start(
    u16 index, const char* function, u32 start_line, const char* code) {
  PerfCounter* counter = perf_counters + index;
  counter->current = PERF_GET_TIME();
  counter->function = function;
  counter->code = code;
  counter->start_line = start_line;
  return counter;
}

void perf_section_end(PerfCounter* counter) {
  PerfTime diff = PERF_GET_TIME() - counter->current;
  counter->total += diff;
  counter->hits++;
}


void perf_print() {
  const bool is_relative_to_baseline = false;

  safe_printf("\n");
  safe_printf("Performance profile\n");
  safe_printf("===================\n");
  if (is_relative_to_baseline) {
    safe_printf(
        "Total(rel)  Average(rel)   Average(abs)      Hits   Function          Line  "
        "Code\n");
  } else {
    safe_printf("   Total       Average      Hits   Function          Line  Code\n");
  }

  PerfCounter* sorted[perf_counters_len];
  PerfCounter* baseline_counter = NULL;
  for (u32 i = 0; i < perf_counters_len; i++) {
    sorted[i] = perf_counters + i;
    if (strcmp(sorted[i]->code, "perf_get_baseline()") == 0) {
      baseline_counter = sorted[i];
    }
  }
  assert(baseline_counter);

  bool did_swap;
  do {
    did_swap = false;
    for (u32 i = 1; i < perf_counters_len; i++) {
      PerfCounter* prev = sorted[i - 1];
      PerfCounter* curr = sorted[i];
      if (curr->total > prev->total) {
        sorted[i - 1] = curr;
        sorted[i] = prev;
        did_swap = true;
      }
    }

  } while (did_swap);

  f64 baseline = (f64)baseline_counter->total;
  for (u32 i = 0; i < perf_counters_len; i++) {
    PerfCounter* counter = sorted[i];
    if (counter->hits == 0 || counter == baseline_counter) {
      continue;
    }

    if (is_relative_to_baseline) {
      safe_printf(" %f      %f     " PERF_FORMAT "    %6d   %16s  %4d  %s\n",
          counter->total / baseline,
          counter->total / (baseline * counter->hits),
          counter->total / counter->hits,
          counter->hits,
          counter->function,
          counter->start_line,
          counter->code);
    } else {
      safe_printf(PERF_FORMAT " " PERF_FORMAT "   %6d   %16s  %4d  %s\n",
          counter->total,
          counter->total / counter->hits,
          counter->hits,
          counter->function,
          counter->start_line,
          counter->code);
    }
  }
  safe_printf("\n");
}
