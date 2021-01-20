#ifndef ELM_KERNEL_LIST
#define ELM_KERNEL_LIST

#include "basics.h"
#include "types.h"

void* List_create(size_t len, void* values[]);
void* eval_List_append(void* args[]);

extern Closure List_cons;
extern Closure List_append;
extern Closure List_map2;
extern Closure List_sortBy;

// toArray & fromArray
// Needed because core Elm code leaks details of the JS implementation
// e.g. String.join and String.split use JS built-ins that need arrays
// In C, we might as well just operate on Elm data.
#define List_toArray Basics_identity
#define List_fromArray Basics_identity

#endif
