#ifndef WRAPPER_H
#define WRAPPER_H

/*
TODO:
Refactor this to expose the variables instead of setter functions.
Perhaps call them `Wrapper_mains` and `Wrapper_fieldGroups`
The code generator can then just assign to those.
Either way, the compiler needs to have knowledge of two C kernel
names. Doesn't really matter if they're functions or values.
In fact code gen currently has to make up names for the values.
It's a bit more straightforward and saves a couple of instructions.
*/
#include "../types.h"

extern FieldGroup* Wrapper_appFieldGroups[];
extern void** Wrapper_mainsArray[];

#endif
