#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#else
#include <stdio.h> // fwrite for safe_printf
#define emscripten_run_script(x)
#endif

#define LOG_BUFFER_BYTES 1024

/**
 * A 'printf' that doesn't call malloc
 * Our GC needs to be the only memory manager in the program
 * stdio printf causes segfault after the GC resizes the heap
 */
void safe_printf(const char* format, ...) {
  va_list va;
  va_start(va, format);
  char buf[LOG_BUFFER_BYTES];
  int count = stbsp_vsnprintf(buf, sizeof(buf), format, va);
  fwrite(buf, sizeof(char), count, stdout);
  va_end(va);
}


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
  safe_printf(fmt, args);
  va_end(args);
  // emscripten_run_script("debugger;");
  exit(EXIT_FAILURE);
}
#else
void log_error(char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  safe_printf(fmt, args);
  va_end(args);
  exit(EXIT_FAILURE);
}
#endif


#if defined(DEBUG) && defined(DEBUG_LOG)
void log_debug(char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  safe_printf(fmt, args);
  va_end(args);
}
#else
void log_debug(char* fmt, ...) {}
#endif
