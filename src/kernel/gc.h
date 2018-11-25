#ifndef ELM_KERNEL_GC
#define ELM_KERNEL_GC

#include "types.h"


int GC_init();

void GC_register_root(ElmValue** root);
void* GC_malloc(size_t bytes);

void* GC_stack_push();
void GC_stack_tailcall(Closure* c, void* push);
void GC_stack_pop(ElmValue* result, void* push);
// void* GC_next_replay();

#endif
