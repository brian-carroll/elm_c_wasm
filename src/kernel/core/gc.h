#ifndef ELM_KERNEL_GC
#define ELM_KERNEL_GC

#include "types.h"
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



#endif
