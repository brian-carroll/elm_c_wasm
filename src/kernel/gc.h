#ifndef ELM_KERNEL_GC
#define ELM_KERNEL_GC

#include "types.h"

int GC_init();

void* GC_register_root(void** root);
void* GC_malloc(size_t bytes);
void* GC_memcpy(void* dest, void* src, size_t bytes);

void* GC_stack_empty();
void* GC_stack_push();
void* GC_stack_pop(ElmValue* result, void* push);
void* GC_stack_tailcall(Closure* c, void* push);

void* GC_tce_iteration(size_t n_args);
void* GC_tce_eval(void* (*tce_eval)(void* [], void**),
    void* (*eval)(void* []),
    u32 n_args,
    void* args[]);
void GC_prep_replay();
void* GC_apply_replay();  // called from Utils_apply

void GC_collect_full();
void GC_collect_nursery();

#endif
