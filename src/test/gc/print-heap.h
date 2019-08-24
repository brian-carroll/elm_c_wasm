#ifndef TEST_GC_PRINT_HEAP_H
#define TEST_GC_PRINT_HEAP_H

int is_marked(void* p);
void print_value(ElmValue* v);
void print_heap();
void print_state();
void print_bitmap();
void log_error(const char* fmt, ...);
#endif
