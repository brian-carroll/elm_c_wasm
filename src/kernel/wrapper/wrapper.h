#ifndef WRAPPER_H
#define WRAPPER_H

#include "../types.h"

// Compiler-generated app-specific data
extern FieldGroup* Wrapper_appFieldGroups[];
extern void** Wrapper_mainsArray[];

void* Wrapper_callJsSync(u32 jsFnIndex, u16 n_args, void* args[]);

#endif
