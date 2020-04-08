#ifndef ELM_KERNEL_LIST
#define ELM_KERNEL_LIST

void* List_create(size_t len, void* values[]);

Closure List_cons;
Closure List_map2;
Closure List_toArray;
#define List_fromArray List_toArray

#endif
