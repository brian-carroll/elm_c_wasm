#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#else
#define emscripten_run_script(x)
#endif

// Execute the JS `debugger` statement (browser devtools)
void Debug_pause() {
  emscripten_run_script("debugger;");
}

#ifdef __EMSCRIPTEN__
void log_error(char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  // print_heap();
  print_state();
  // fprintf(stderr, fmt, args);
  printf(fmt, args);
  va_end(args);
  // emscripten_run_script("debugger;");
  exit(EXIT_FAILURE);
}
#else
void log_error(char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  printf(fmt, args);
  va_end(args);
  exit(EXIT_FAILURE);
}
#endif

#if defined(DEBUG) && defined(DEBUG_LOG)
void log_debug(char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  printf(fmt, args);
  va_end(args);
}
#else
void log_debug(char* fmt, ...) {}
#endif
