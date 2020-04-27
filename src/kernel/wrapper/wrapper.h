#ifndef WRAPPER_H
#define WRAPPER_H

#include "../types.h"

// Compiler-generated app-specific data
extern FieldGroup* Wrapper_appFieldGroups[];
extern void** Wrapper_mainsArray[];

// JS imported function
extern size_t Wrapper_evaluateInJs(size_t addr);

#endif
