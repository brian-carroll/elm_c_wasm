#ifndef GC_TEST_H
#define GC_TEST_H

#include "../kernel/core/gc/internals.h"

char* gc_test();
void gc_test_reset();

extern GcState gc_state;
void mark(GcState* state, size_t* ignore_below);

#endif
