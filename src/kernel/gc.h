#ifndef ELM_KERNEL_GC
#define ELM_KERNEL_GC

#include "types.h"


int GC_init();

void* GC_register_root(ElmValue** root);
void* GC_malloc(size_t bytes);

void* GC_stack_push();
void* GC_stack_pop(ElmValue* result, void* push);
void* GC_stack_tailcall(Closure* c, void* push);

void* GC_tce_iteration(size_t n_args, size_t** gc_tce_data);
void* GC_tce_eval(
    void* (*tce_evaluator)(void*[], size_t**),
    Closure* c,
    void* args[]
);

void* GC_apply_replay();

#endif
