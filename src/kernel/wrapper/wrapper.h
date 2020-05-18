#ifndef WRAPPER_H
#define WRAPPER_H

#include "../core/types.h"

// Compiler-generated app-specific data
extern FieldGroup* Wrapper_appFieldGroups[];
extern void** Wrapper_mainsArray[];

void* Wrapper_callJsSync(size_t jsFnIndex, u16 n_args, void* args[]);

#endif
