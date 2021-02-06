#ifndef ELM_KERNEL_GC
#define ELM_KERNEL_GC

#include "./types.h"
#include <stddef.h>

// manage
int GC_init();
void* GC_register_root(void** root);
void GC_collect_full();
void GC_collect_nursery();
void* GC_execute(Closure* c);

// allocate
void* GC_malloc(ptrdiff_t bytes);
void* GC_memcpy(void* dest, void* src, size_t bytes);


typedef u32 GcStackMapIndex;

void GC_stack_reset(Closure*);
void GC_stack_push(void* value);
void GC_stack_pop(EvalFunction evaluator, void* result, GcStackMapIndex push);
GcStackMapIndex GC_stack_get_current_pos();
Closure* GC_stack_tailcall(GcStackMapIndex push, Closure* old, u32 n_explicit_args, void* explicit_args[]);
void* GC_apply_replay(EvalFunction evaluator);


#endif
