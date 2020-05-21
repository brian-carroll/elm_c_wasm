#ifndef ELM_H
#define ELM_H

#include "types.h"

Custom g_elm_core_Maybe_Nothing;
Closure g_elm_core_Maybe_Just;

Closure g_elm_core_Array_initialize;
Closure g_elm_core_List_reverse;
Closure g_elm_core_List_foldl;

void* eval_elm_core_Result_Ok(void* args[]);
void* eval_elm_core_Result_Err(void* args[]);
void* eval_elm_core_Result_isOk(void* args[]);

#endif
