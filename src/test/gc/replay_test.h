#ifndef REPLAY_TEST_H
#define REPLAY_TEST_H

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "../../kernel/debug.h"
#include "../../kernel/gc-internals.h"
#include "../../kernel/gc.h"
#include "../../kernel/types.h"
#include "../../kernel/utils.h"
#include "../gc_test.h"
#include "../test.h"

char* replay_scenario_tests();
Closure mock_func;
void* eval_mock_func(void**);
Closure mock_func_tail;
void* eval_mock_func_tail(void* args[]);
char* assert_heap_values(char* description, void* values[]);
extern GcState gc_state;
extern Tag mock_func_ops[10];
extern u32 INT_OFFSET;

#endif