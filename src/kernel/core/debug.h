#include <stdbool.h>

#include "types.h"
#ifndef ELM_KERNEL_DEBUG
#define ELM_KERNEL_DEBUG

#ifdef TARGET_64BIT
#define FORMAT_HEX "%016zx"
#define FORMAT_PTR "%14p"
#define FORMAT_PTR_LEN 14
#else
#define FORMAT_HEX "%08zx"
#define FORMAT_PTR "%8p"
#define FORMAT_PTR_LEN 8
#endif

void Debug_pretty(const char* label, void* p);
extern char* Debug_ctors[];
extern char* Debug_fields[];
extern char* Debug_jsValues[];
extern int Debug_ctors_size;
extern int Debug_fields_size;
extern int Debug_jsValues_size;
char* Debug_evaluator_name(void*);
extern char Debug_unknown_evaluator[];

void Debug_pause();

bool is_marked(void* p);
void print_value(void* p);
void print_value_full(void* p);
void print_heap_range(size_t* start, size_t* end);
void print_heap();
void print_state();
void print_bitmap();
void log_error(char* fmt, ...);
#endif
