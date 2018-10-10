#define mu_assert(message, test) do { if (!(test)) return message; } while (0)
#define mu_run_test(test) do { char *message = test(); tests_run++; \
                                if (message != NULL) return message; } while (0)
// extern int tests_run;


// Debug function
char* hex(void* addr, int size) {
    char *str;
    u32 i, c=0, n=(9*size)/4;
    str = malloc(n);
    for (i=0; i < size; c += 9, i += 4) {
        // Print in actual byte order (little endian)
        sprintf(str + c,
            "%02x%02x%02x%02x|",
            *(u8*)(addr + i),
            *(u8*)(addr + i + 1),
            *(u8*)(addr + i + 2),
            *(u8*)(addr + i + 3)
        );
    }
    str[c-1] = 0; // erase last "|" and terminate the string
    return str;
}

// Print a memory address held in a pointer
//   Addresses are printed in little-endian format
//   => Easier to see what pointers are pointing to it
//   => Harder to subtract in your head if you're interested in sizes & memory layout
char* hex_ptr(void* ptr) {
    return hex(&ptr, sizeof(void*));
}
