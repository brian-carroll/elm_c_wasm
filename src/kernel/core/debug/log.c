#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#else
#define emscripten_run_script(x)
#endif

#ifdef __EMSCRIPTEN__
#include <stdio.h>
#define WRITE_BYTES_TO_STDOUT(buf, count) fwrite(buf, sizeof(char), count, stdout)
#elif defined(_WIN32)
#include <io.h>
#define WRITE_BYTES_TO_STDOUT(buf, count) _write(1, buf, count)
#else
#include <unistd.h>
#define WRITE_BYTES_TO_STDOUT(buf, count) write(STDOUT_FILENO, buf, count)
#endif

#define LOG_BUFFER_BYTES 1024

/**
 * A 'printf' that doesn't call malloc. (The stdio.h implementation does.)
 * Our GC needs to be the only memory manager in the program
 * Otherwise printf would segfault after the GC resizes the heap
 */
void safe_vprintf(const char* format, va_list va) {
  char buf[LOG_BUFFER_BYTES];
  int count = stbsp_vsnprintf(buf, sizeof(buf), format, va);

  int written = WRITE_BYTES_TO_STDOUT(buf, count);
  assert(written == count);
}

void safe_printf(const char* format, ...) {
  va_list va;
  va_start(va, format);
  safe_vprintf(format, va);
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
  safe_vprintf(fmt, args);
  va_end(args);
  // emscripten_run_script("debugger;");
  exit(EXIT_FAILURE);
}
#else
void log_error(char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  safe_vprintf(fmt, args);
  va_end(args);
  exit(EXIT_FAILURE);
}
#endif


#if defined(DEBUG) && defined(DEBUG_LOG)
void log_debug(char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  safe_vprintf(fmt, args);
  va_end(args);
}
#else
void log_debug(char* fmt, ...) {}
#endif
