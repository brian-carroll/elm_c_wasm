#ifndef ELM_H
#define ELM_H

// Values defined in core library .elm files, referenced in kernel code

#include "types.h"

extern Custom g_elm_core_Maybe_Nothing;
extern Closure g_elm_core_Maybe_Just;

extern Closure g_elm_core_Array_initialize;
extern Closure g_elm_core_List_reverse;
extern Closure g_elm_core_List_foldl;

void* eval_elm_core_Result_Ok(void* args[]);
void* eval_elm_core_Result_Err(void* args[]);
void* eval_elm_core_Result_isOk(void* args[]);

#endif
