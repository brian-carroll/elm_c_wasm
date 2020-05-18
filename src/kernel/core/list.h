#ifndef ELM_KERNEL_LIST
#define ELM_KERNEL_LIST

#include "basics.h"
#include "types.h"

void* List_create(size_t len, void* values[]);
void* eval_List_append(void* args[]);

Closure List_cons;
Closure List_append;
Closure List_map2;

// toArray & fromArray
// Only needed because some core Elm code contains leaked details of the JS
// implementation! e.g. String.join and String.split Direct C translations are not useful,
// so just use identity function
#define List_toArray Basics_identity
#define List_fromArray Basics_identity

#endif
