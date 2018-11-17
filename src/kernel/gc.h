#ifndef ELM_KERNEL_GC
#define ELM_KERNEL_GC

#include "types.h"



#define GC_PAGE_BYTES 65536
// #define GC_BLOCK_BYTES 512
// #define GC_BLOCKS_PER_PAGE (GC_PAGE_BYTES/(GC_BLOCK_BYTES+(GC_BLOCK_BYTES/32)+sizeof(u16)))

typedef struct {
    // Compactor algorithm: track forwarding addresses using offsets and a bitmap
    //
    // u64 bitmap[GC_BLOCKS_PER_PAGE*GC_BLOCK_BYTES/(4*sizeof(u64))];
    // u16 offsets[GC_BLOCKS_PER_PAGE];
    // u32 data[GC_BLOCKS_PER_PAGE*GC_BLOCK_BYTES/4];
    u32 data[GC_PAGE_BYTES/4];
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
void GC_register_root(void* root);

void* GC_stack_push(Closure* c);
void GC_stack_tailcall(Closure* c, void* push);
void GC_stack_pop(ElmValue* result, void* push);
void* GC_next_replay();

void* GC_allocate(size_t size);


// Private functions exposed only for testing
void mark_stack_map(ElmValue* ignore_below);




#endif
