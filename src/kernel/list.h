#ifndef ELM_KERNEL_LIST
#define ELM_KERNEL_LIST

void* List_create(size_t len, void* values[]);
void* eval_List_append(void* args[]);

Closure List_cons;
Closure List_append;
Closure List_map2;
Closure List_toArray;
#define List_fromArray List_toArray

#endif
