#ifndef WRAPPER_H
#define WRAPPER_H

#include <stddef.h>
#include <stdbool.h>
#include "../core/types.h"

// Compiler-generated app-specific data
extern FieldGroup* Wrapper_appFieldGroups[];
extern void** Wrapper_mainsArray[];

extern ptrdiff_t getJsRefArrayIndex(u32 jsRefId, u32 index);
extern void* getJsRefObjectField(u32 jsRefId, ElmString* fieldStringAddr);
extern void* getJsRefValue(u32 jsRefId);
extern void markJsRef(u32 jsRefId);
extern void sweepJsRefs(bool isFullGc);
extern f64 parseFloat(u16* addr, size_t len16);

#endif
