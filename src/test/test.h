#define mu_assert(message, test) do { if (!(test)) return message; } while (0)
#define mu_run_test(test) do { char *message = test(); tests_run++; \
                                if (message != NULL) return message; } while (0)
extern int tests_run, verbose;

char* hex(void* addr, int size);
char* hex_ptr(void* ptr);
