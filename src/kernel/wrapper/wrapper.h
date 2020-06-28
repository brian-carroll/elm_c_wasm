#ifndef WRAPPER_H
#define WRAPPER_H

#include <stddef.h>
#include <stdbool.h>
#include "../core/types.h"

// Compiler-generated app-specific data
extern FieldGroup* Wrapper_appFieldGroups[];
extern void** Wrapper_mainsArray[];

ptrdiff_t getJsRefArrayIndex(u32 jsRefId, u32 index);
ptrdiff_t getJsRefObjectField(u32 jsRefId, size_t fieldStringAddr);
ptrdiff_t getJsRefValue(u32 jsRefId);
void markJsRef(u32 jsRefId);
void sweepJsRefs(bool isFullGc);

#endif
