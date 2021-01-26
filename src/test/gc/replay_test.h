#ifndef REPLAY_TEST_H
#define REPLAY_TEST_H

#include "../../kernel/core/core.h"
#include "../../kernel/core/gc/internals.h"

char* replay_scenario_tests();
char* assert_heap_values(char* description, void* values[]);
extern GcState gc_state;
extern u32 TEST_INT_OFFSET;
extern size_t GARBAGE;

#endif
