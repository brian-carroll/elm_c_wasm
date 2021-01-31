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

// stack
typedef struct {
  EvalFunction evaluator; // for debug
  void* start;
  void* end;
} GcLiveSection;

void GC_stack_clear();
GcLiveSection* GC_stack_push(EvalFunction evaluator);
void GC_stack_pop(EvalFunction evaluator, void* result, GcLiveSection* push);
void* GC_stack_tailcall(EvalFunction evaluator, u32 n_free, void* free_vars[], u32 n_explicit_args, void* explicit_args[]);
void* GC_apply_replay(EvalFunction evaluator);


#endif
