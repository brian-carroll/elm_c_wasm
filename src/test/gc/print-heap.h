
#include <stdbool.h>
#include "../../kernel/types.h"
#ifndef TEST_GC_PRINT_HEAP_H
#define TEST_GC_PRINT_HEAP_H

#ifdef TARGET_64BIT
#define FORMAT_HEX "%016zx"
#define FORMAT_PTR "%14p"
#else
#define FORMAT_HEX "%08zx"
#define FORMAT_PTR "%8p"
#endif

bool is_marked(void* p);
void print_value(ElmValue* v);
void print_heap();
void print_state();
void print_bitmap();
void log_error(const char* fmt, ...);
#endif
