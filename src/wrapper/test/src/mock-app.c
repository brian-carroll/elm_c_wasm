#include <emscripten/emscripten.h>
#include <stdio.h>

int EMSCRIPTEN_KEEPALIVE addTwo(a, b) {
  int result = a + b;
  printf("Calculating %d + %d = %d\n", a, b, result);
  return result;
}

int EMSCRIPTEN_KEEPALIVE main(int argc, char** argv) {
  printf("Running main\n");
  return 0;
}