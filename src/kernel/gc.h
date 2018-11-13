#ifndef ELM_KERNEL_GC
#define ELM_KERNEL_GC

#include "types.h"

void GC_init();
void GC_register_root(void* root);

void GC_stack_push(Closure* c);
void GC_stack_tailcall(Closure* c);
void GC_stack_pop(ElmValue* result);
void* GC_next_replay();

void* GC_allocate(u32 size);

#endif
