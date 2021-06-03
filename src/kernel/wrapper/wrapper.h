#ifndef WRAPPER_H
#define WRAPPER_H

#include <stddef.h>
#include <stdbool.h>
#include "../core/types.h"

// Compiler-generated data
extern FieldGroup* Wrapper_appFieldGroups[];
extern void** Wrapper_mainsArray[];
extern Closure g_author_project_WebAssembly_intercept;
extern const u32 FIELD_init;
extern const u32 FIELD_update;
extern const u32 FIELD_view;
extern const u32 FIELD_subscriptions;

// JS imported functions
extern ptrdiff_t getJsRefArrayIndex(u32 jsRefId, u32 index);
extern void* getJsRefObjectField(u32 jsRefId, ElmString* fieldStringAddr);
extern void* getJsRefValue(u32 jsRefId);
extern void markJsRef(u32 jsRefId);
extern void sweepJsRefs(bool isFullGc);
extern f64 parseFloat(u16* addr, size_t len16);
extern void Platform_stepper(void*);

#endif
