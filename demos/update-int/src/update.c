#include <emscripten/emscripten.h>

int EMSCRIPTEN_KEEPALIVE update(int msg, int model)
{
  return (msg > 0) ? model + 1 : model - 1;
}