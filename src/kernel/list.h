#ifndef ELM_KERNEL_LIST
#define ELM_KERNEL_LIST

void* List_append_eval(void* args[]);
void* List_fromArray(size_t len, void* values[]);
#define List_create List_fromArray

#endif
