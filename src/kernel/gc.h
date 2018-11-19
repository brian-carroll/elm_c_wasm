#ifndef ELM_KERNEL_GC
#define ELM_KERNEL_GC

#include "types.h"



#define GC_PAGE_BYTES 65536
#define GC_BLOCK_BYTES 512
#define GC_PAGE_BLOCKS (GC_PAGE_BYTES/(GC_BLOCK_BYTES+(GC_BLOCK_BYTES/32)+sizeof(u16)))
#define GC_PAGE_SLOTS (GC_PAGE_BLOCKS*GC_BLOCK_BYTES/4)
#define GC_BITMAP_WORDSIZE 64

// fields ordered by decreasing unit size to avoid alignment padding
typedef struct {
    u64 bitmap[GC_PAGE_SLOTS/GC_BITMAP_WORDSIZE];
    u32 data[GC_PAGE_SLOTS];
    u16 offsets[GC_PAGE_BLOCKS];
    u8 unused[GC_PAGE_BYTES - (8*GC_PAGE_SLOTS/GC_BITMAP_WORDSIZE + 4*GC_PAGE_SLOTS + 2*GC_PAGE_BLOCKS)];
} GcPage;


typedef struct {
    ElmValue* roots;   // Linked list of all "GC roots" except stack map
    Custom* stack_map; // Separate "GC root" to track heap pointers in use by live functions.
    u32 stack_depth;

    void* system_max_heap; // The point at which we need new memory from the OS/browser
    void* max_heap; // The point at which allocation fails (keeping some space for GC internal use)
    u32* current_heap; // Next allocated address

    GcPage pages[1];
} GcState;


GcState* GC_init();
void GC_register_root(ElmValue** root);

void* GC_stack_push();
void GC_stack_tailcall(Closure* c, void* push);
void GC_stack_pop(ElmValue* result, void* push);
void* GC_next_replay();

void* GC_allocate(size_t size);


#endif
