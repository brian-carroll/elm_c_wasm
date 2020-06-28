#ifndef REPLAY_TEST_H
#define REPLAY_TEST_H

#include "../../kernel/core/core.h"
#include "../../kernel/core/gc-internals.h"

char* replay_scenario_tests();
extern Closure mock_func;
void* eval_mock_func(void**);
extern Closure mock_func_tail;
void* eval_mock_func_tail(void* args[]);
char* assert_heap_values(char* description, void* values[]);
GcState gc_state;
Tag mock_func_ops[10];
u32 TEST_INT_OFFSET;
size_t GARBAGE;

#endif
