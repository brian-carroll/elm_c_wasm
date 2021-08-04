#ifndef WRAPPER_H
#define WRAPPER_H

#include <stddef.h>
#include <stdbool.h>
#include "../core/types.h"

// Compiler-generated data
extern FieldGroup* Wrapper_appFieldGroups[];
extern void** Wrapper_mainsArray[];
extern Closure g_author_project_WebAssembly_intercept;
extern FieldGroup fg_init_subscriptions_update_view;

// JS imported functions
extern ptrdiff_t getJsRefArrayIndex(u32 jsRefId, u32 index);
extern void* getJsRefObjectField(u32 jsRefId, ElmString* fieldStringAddr);
extern void* getJsRefValue(u32 jsRefId);
extern void markJsRef(u32 jsRefId);
extern void sweepJsRefs(bool isFullGc);
extern f64 parseFloat(u16* addr, size_t len16);
extern void jsStepper(void* viewMetadata);
extern void* applyJsRef(u32 jsRefId, u32 nArgs, void* args[]);
extern Record* jsRefToWasmRecord(u32 jsRefId);
extern Tuple2* Wrapper_setupOutgoingPort();
extern u32 Wrapper_setupIncomingPort(u32 managerId);
extern Task* Wrapper_sleep(f64 time);

#endif
