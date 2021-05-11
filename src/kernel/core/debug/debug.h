#ifndef DEBUG_H
#define DEBUG_H

//
// Elm code debugging
//

extern Closure Debug_toString;
extern Closure Debug_log;
extern Closure Debug_todo;

//
// C runtime debugging
//

#if TARGET_64BIT
#define FORMAT_HEX "%016zx"
#define FORMAT_PTR "%16p"
#define FORMAT_PTR_LEN 16
#else
#define FORMAT_HEX "%08zx"
#define FORMAT_PTR "%8p"
#define FORMAT_PTR_LEN 8
#endif

void Debug_pretty(const char* label, void* p);
void Debug_print_offset(const char* label, void* p);
bool Debug_is_target_addr(void* p);
bool Debug_is_target_in_range(void* from, void* to);
extern char* Debug_ctors[];
extern char* Debug_fields[];
extern char* Debug_jsValues[];
extern int Debug_ctors_size;
extern int Debug_fields_size;
extern int Debug_jsValues_size;
char* Debug_evaluator_name(void*);

void Debug_pause();
bool sanity_check(void* v);

#define LOG_ALWAYS 0x01
#define LOG_GC 0x02
#define LOG_GC_MARK 0x04
#define LOG_GC_COMPACT 0x08
#define LOG_GC_ALLOCATE 0x10

#ifndef LOG_FLAGS
#define LOG_FLAGS LOG_ALWAYS
#endif

//
// Performance profiling
//

#ifndef DEBUG

#define PERF_TIMER_ENABLED 0
#define PERF_TIMED_STATEMENT(statement) statement
#define perf_print()

#else

#define PERF_TIMER_ENABLED 1

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#define PERF_GET_TIME() emscripten_get_now()
typedef f64 PerfTime;
#define PERF_FORMAT "%'11lf"
#elif _MSC_VER
#include <intrin.h>
#define PERF_GET_TIME() __rdtsc()
typedef u64 PerfTime;
#define PERF_FORMAT "%'11ld"
#else
#include <x86intrin.h>
#define PERF_GET_TIME() __rdtsc()
typedef u64 PerfTime;
#define PERF_FORMAT "%'11ld"
#endif


typedef struct perf_counter {
  PerfTime current;
  PerfTime total;
  const char* function;
  const char* code;
  u32 hits;
  u32 start_line;
} PerfCounter;

extern PerfCounter perf_counters[];
extern u32 perf_counters_len;
PerfCounter* perf_section_start(
    u16 index, const char* function, u32 start_line, const char* code);
void perf_section_end(PerfCounter* counter);
void perf_print();

#define PERF_TIMED_STATEMENT(statement)                                      \
  {                                                                          \
    PerfCounter* current_perf_counter =                                      \
        perf_section_start(__COUNTER__, __FUNCTION__, __LINE__, #statement); \
    { statement; }                                                           \
    perf_section_end(current_perf_counter);                                  \
  }


#endif  // DEBUG

#endif  // DEBUG_H
