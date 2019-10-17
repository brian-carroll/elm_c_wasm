#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#else
#define EMSCRIPTEN_KEEPALIVE
#include <stdio.h>
#endif
#include "../../../kernel/basics.h"
#include "../../../kernel/gc-internals.h"
#include "../../../kernel/gc.h"
#include "../../../kernel/types.h"
#include "../../../kernel/utils.h"

const FieldGroup fg1 = {
    .size = 3,
    .fields = {123, 456, 789},
};
const FieldGroup fg2 = {
    .size = 2,
    .fields = {123, 456},
};
const FieldGroup* appFieldGroups[] = {
    &fg1,
    &fg2,
    NULL,
};

size_t EMSCRIPTEN_KEEPALIVE main(int argc, char** argv) {
  return GC_init();
}
