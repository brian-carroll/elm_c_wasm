#include <stdbool.h>
#include "./types.h"
#ifndef ELM_KERNEL_DEBUG
#define ELM_KERNEL_DEBUG

#ifdef TARGET_64BIT
#define FORMAT_HEX "%016zx"
#define FORMAT_PTR "%14p"
#else
#define FORMAT_HEX "%08zx"
#define FORMAT_PTR "%8p"
#endif

bool is_marked(void* p);
void print_value(void* p);
void print_value_full(void* p);
void print_heap();
void print_state();
void print_bitmap();
void log_error(char* fmt, ...);
#endif
