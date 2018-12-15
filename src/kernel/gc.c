/*
    Garbage collector for Elm
    -------------------------
    A mark-compact collector, based on the Compressor algorithm.

    Notable features:
    ================
    - Compression uses only a single pass over the heap

    - Uses a bitmap of live/dead bits instead of marking the values themselves.
        => Many operations involve only a few localised memory accesses

    - Calculates 'forwarding addresses' of moved pointers on the fly
        - New location has the same number of live bytes below it as the old one did
        - Speeds up the calculation by storing a table of 'offsets' to the first
            live value in each 512-byte block of memory

    - Designed for both 32 and 64-bit platforms.
        Uses the size_t data type everywhere (32 or 64 bits as per platform)

    - Takes advantage of immutable-only data
        - Can do partial compression, starting at any point and only looking
        at values _above_ it, ignoring everything below.
        - Enabled by the fact that immutable values can only reference older
        values (lower addresses).
        - Offers flexibility to do quick partial collections.

    - Takes advantage of pure functions
        - A novel stack map idea: "replay" for pure functions
        - Wasm does not allow access to registers (which it models as a stack machine)
        - Therefore we can't find or modify pointers stored in registers. So how can
            we move things around safely?
        - Elm functions are pure. So we can abort execution, move values around,
            then "replay" the same function call with the same values, but at
            the new locations.
        - On this second "replay" run, no registers can possibly hold pointers
            to the old locations.
        - Now, on "replay", every call our top-level function makes can be replaced
            with the return value we remember from the first time.
        - This means replay is fast, and doesn't regenerate the same garbage again.
            It executes very little code, produces minimal garbage, and runs fast.
        - This only works with pure functions, but in Elm that's most of the program.
        - The `apply` operator has some hooks into the GC. The GC puts some extra
            marker values into the heap to track pushes and pops of the call stack.


    References
        - "The Garbage Collection Handbook" by Jones, Hosking, and Moss (section 3.4)
        - Kermany and Petrank, 2006
        - Abuaiadh et al, 2004
*/
#include <errno.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include "types.h"
#include "gc.h"
#include "gc-internals.h"
#ifdef DEBUG
    #include <stdio.h>
#endif


GcState gc_state;

static void* stack_empty(); // pre-declaration

#ifdef _WIN32
static void* sbrk(size_t size) {
    return malloc(GC_WASM_PAGE_BYTES*5 + size);
}
#endif



/* ====================================================

                PROGRAM STARTUP

   ==================================================== */

// Call exactly once on program startup
int GC_init() {
    GcState* state = &gc_state; // local reference for debugger to see

    // Get current max address of program data
    size_t* break_ptr = sbrk(0);

    // Align to next block boundary
    size_t break_aligned = (size_t)break_ptr;
    if (break_aligned % GC_BLOCK_BYTES) {
        break_aligned |= (GC_BLOCK_BYTES-1);
        break_aligned++;
    }
    size_t* heap_bottom = (size_t*)break_aligned;

    // Initialise state with zero heap size
    *state = (GcState){
        .heap = (GcHeap){
            .start = heap_bottom,
            .end = heap_bottom,
            .system_end = heap_bottom,
            .bitmap = heap_bottom,
            .offsets = heap_bottom,
        },
        .next_alloc = heap_bottom,
        .roots = &Nil,
        .stack_map = NULL,
        .stack_depth = 0,
    };

    // Ask the system for more memory
    size_t top_of_current_page =
        (size_t)heap_bottom | (size_t)(GC_WASM_PAGE_BYTES-1);

    size_t* top_of_next_page =
        (size_t*)(top_of_current_page + GC_WASM_PAGE_BYTES + 1);

    int err = set_heap_end(&state->heap, top_of_next_page);

    if (!err) {
        stack_empty();
    }

    return err;
}

/*
  GC ROOTS
  --------
  Roots should be registered by Kernel modules once on program startup.

  Usage:
    ElmValue* my_mutable_heap_pointer;

    void My_init() {
        GC_register_root(&my_mutable_heap_pointer);
    }

  Once the mutable pointer is registered, the Kernel module
  can point it at any heap value it wants to keep alive.
  It can later update the pointer to point at a different
  ElmValue on the heap, as the program executes.
  Whenever the GC does a collection, it checks this pointer
  and preserves whatever heap value it is currently
  pointing to. If it moves the value, it will update the
  pointer to reference the new location.
*/
void* GC_register_root(ElmValue** ptr_to_mutable_ptr) {
    GcState* state = &gc_state;
    state->roots = (ElmValue*)newCons(
        ptr_to_mutable_ptr,
        state->roots
    );
    return ptr_to_mutable_ptr; // anything but pGcFull
}



/* ====================================================

                ALLOCATE & COPY

   ==================================================== */

/*
  Allocate memory on the heap
  Same interface as malloc in stdlib.h
*/
void* GC_malloc(size_t bytes) {
    GcState* state = &gc_state;
    size_t words = bytes / sizeof(size_t);

    #ifdef DEBUG
        if (bytes % sizeof(size_t)) {
            fprintf(stderr, "GC_malloc: Request for %zd bytes is misaligned\n", bytes);
        }
    #endif
    size_t* replay = state->replay_ptr;
    if (replay != NULL) {
        // replay mode
        #ifdef DEBUG
            u32 replay_words = ((Header*)replay)->size;
            if (replay_words != words) {
                fprintf(stderr, "GC_malloc: replay error. Requested size %zd doesn't match cached size %d\n", words, replay_words);
            }
        #endif
        size_t* next_replay = replay + words;
        if (next_replay >= state->next_alloc) {
            next_replay = NULL; // exit replay mode
        }
        state->replay_ptr = next_replay;
        #ifdef DEBUG
            printf("GC_malloc: replay_ptr = %p\n", state->replay_ptr);
        #endif
        return (void*)replay;
    } else {
        // normal mode
        size_t* old_heap = state->next_alloc;
        size_t* new_heap = old_heap + words;

        if (new_heap < state->heap.end) {
            state->next_alloc = new_heap;
            return old_heap;
        } else {
            return pGcFull;
        }
    }
}


void* GC_memcpy(void* dest, void* src, size_t bytes) {
    size_t words = bytes / sizeof(size_t);

    #ifdef DEBUG
        if (bytes % sizeof(size_t)) {
            fprintf(stderr, "GC_memcpy: Copy %zd bytes is misaligned\n", bytes);
        }
    #endif

    size_t* src_words = (size_t*)src;
    size_t* dest_words = (size_t*)dest;

    for (size_t i=0; i<words; i++) {
        dest_words[i] = src_words[i];
    }
    return dest; // C standard lib returns this. Normally ignored.
}




/* ====================================================

                STACK MAP

   ====================================================

    The Stack Map keeps track of which heap values are currently
    pointed to from the stack or registers.

    As we execute the program, functions grab pointers to heap values
    and manipulate them in local variables. But when we interrupt to
    do GC, we move values around, making those local pointers corrupt.
    We need a way to fix this up after GC.

    Local heap pointers must either be:
        - on the C stack (in the top part of Wasm linear memory)
        - or in registers (represented as the 'stack machine' in Wasm)

    People usually scan both, but Wasm's registers are completely
    abstracted away and un-scannable!

    So we do this:
        - When out of memory, abandon all functions (sort of like an exception)
        - Keep all heap values that were allocated by functions we interrupted
        - Keep any return values from functions that have finished running
        - Do a collection
        - Rerun the top-level function (e.g. `update` with same model and message)
        - **BUT** replace function calls with the values they returned last time
        - For all allocations, return same object as last time (at its new address)

    This fast-forwards back to where we interrupted execution, with all local
    variables referencing only the new moved pointers.

    The functions below are called from the `apply` operator
*/

static void* stack_empty() {
    GcState* state = &gc_state;
    GcStackMap* p = GC_malloc(sizeof(GcStackMap));
    if (p == pGcFull) return pGcFull;
    p->header = HEADER_GC_STACK_EMPTY;
    state->stack_map = p;
    return p;
}


void* GC_stack_push() {
    GcState* state = &gc_state;
    if (state->replay_ptr != NULL) return state->replay_ptr;

    GcStackMap* p = GC_malloc(sizeof(GcStackMap));
    if (p == pGcFull) return pGcFull;

    p->header = HEADER_GC_STACK_PUSH;
    p->older = state->stack_map;

    state->stack_map = p;
    state->stack_depth++;
    return p;
}

void* GC_stack_pop(ElmValue* result, void* push) {
    GcState* state = &gc_state;
    GcStackMap* p = GC_malloc(sizeof(GcStackMap));
    if (p == pGcFull) return pGcFull;

    p->header = HEADER_GC_STACK_POP;
    p->older = push;
    p->replay = result;

    state->stack_map = p;
    state->stack_depth--;
    return p;
}

void* GC_stack_tailcall(Closure* c, void* push) {
    GcState* state = &gc_state;
    GcStackMap* p = GC_malloc(sizeof(GcStackMap));
    if (p == pGcFull) return pGcFull;
    p->header = HEADER_GC_STACK_TC;
    p->older = push;
    p->replay = c;

    state->stack_map = p;
    // stack_depth stays the same
    return p;
}



/* ====================================================

                TAIL CALL ELIMINATION

   ==================================================== */


// GC operations for one iteration of a tail call elminated Elm function
// Allocates space for a Closure and GcStackMap so that during replay
// we can skip previous iterations (and their garbage)
// Creates lots of extra garbage in order to be able to clean it all up!
void* GC_tce_iteration(size_t n_args, void** gc_tce_data) {
    GcState* state = &gc_state;
    size_t closure_bytes = sizeof(Closure) + n_args*sizeof(void*);
    size_t cont_bytes = closure_bytes + sizeof(GcStackMap);

    void* tce_space = GC_malloc(cont_bytes);
    if (tce_space == pGcFull) return pGcFull;

    GcStackMap* tailcall = (GcStackMap*)(tce_space + closure_bytes);
    state->stack_map = tailcall;
    *gc_tce_data = tce_space;

    return tce_space; // not pGcFull
}


// Evaluate a tail call elminated Elm function,
// managing all of the GC related stuff for it
void* GC_tce_eval(
    void* (*tce_evaluator)(void*[], void**),
    Closure* c_orig,
    void* args[]
) {
    GcState* state = &gc_state;
    GcStackMap* push = state->stack_map;
    size_t n_args = (size_t)c_orig->max_values;
    size_t closure_bytes = sizeof(Closure) + n_args*sizeof(void*);

    // Copy the closure and mutate the args during iteration
    // then let it become garbage
    Closure* c_mutable;
    if (state->replay_ptr) {
        // In replay mode, reuse the Closure we created earlier,
        // retrieved from replay a moment ago in Utils_apply
        // It's our own private copy, we can mutate it without harm
        // Only way to get it is to look just below 'args'
        // c_orig is not from the same Closure, it's off-heap
        c_mutable = (Closure*)(args - sizeof(Closure)/sizeof(void*));
    } else {
        c_mutable = CAN_THROW(GC_malloc(closure_bytes));
        *c_mutable = (Closure){
            .header = HEADER_CLOSURE(n_args),
            .n_values = n_args,
            .max_values = n_args,
            .evaluator = c_orig->evaluator,
        };
        GC_memcpy(c_mutable->values, args, n_args*sizeof(void*));
    }

    // Pointer to new space allocated by tce_evaluator on every iteration
    // to place tailcall and closure in case of exception
    void* gc_tce_data;

    // Run the tail-call-eliminated evaluator
    void* result = (*tce_evaluator)(c_mutable->values, &gc_tce_data);
    if (result != pGcFull) {
        return result;
    }

    // GC Exception handling
    // Save the current args in a closure in the stack map
    // Then we can replay later, skipping earlier iterations
    // This space was already _allocated_ but not _written_
    // by GC_tce_iteration, called from tce_evaluator
    Closure* c_replay = (Closure*)gc_tce_data;
    GC_memcpy(c_replay, c_mutable, closure_bytes);

    GcStackMap* tailcall = (GcStackMap*)(gc_tce_data + closure_bytes);
    *tailcall = (GcStackMap){
        .header = HEADER_GC_STACK_TC,
        .older = push,
        .replay = c_replay
    };

    return pGcFull;
}


/* ====================================================

                REPLAY

   ==================================================== */


static void* next_heap_value(void* current) {
    Header* h = current;
    size_t* words = current;
    return words + h->size;
}


/*
    Call stack was interrupted and unwound by a GC exception
    but we saved all the live values in the stack map.
    Elm functions are pure so we can "replay" most of the same
    values without actually re-calculating them.
    Only need to re-execute functions that were live when the
    exception was thrown.
    This is fast and creates no garbage until we exit replay mode.
    This function is called from the normal 'Utils_apply'.
*/

typedef enum {
    Finished,                   // This call had finished, replay saved return value from previous execution.
    Unfinished_Sat_Normal,      // Unfinished call. Saturated (all args in one go). Re-execute it.
    Unfinished_Sat_Tail,        // Unfinished saturated tail call. Re-execute from last completed iteration.
    Unfinished_Curried_Normal,  // Unfinished normal call with curried values. Re-execute it.
    Unfinished_Curried_Tail,    // Unfinished tail call with curried values. Re-execute from last iteration.
    Partial_Application,        // Partial application, unsaturated so no execution required. Return saved value.
    Apply_Alloc_Failed,         // Apply itself failed to allocate a Closure or stackmap value. Re-execute.
    BugScenario                 // Heap doesn't make sense given that a replaying function has called Utils_apply.
} ReplayScenario;

#ifdef DEBUG
char* scenario_to_string(ReplayScenario scenario) {
    switch (scenario) {
        case Finished:                   return "Finished";
        case Unfinished_Sat_Normal:      return "Unfinished_Sat_Normal";
        case Unfinished_Sat_Tail:        return "Unfinished_Sat_Tail";
        case Unfinished_Curried_Normal:  return "Unfinished_Curried_Normal";
        case Unfinished_Curried_Tail:    return "Unfinished_Curried_Tail";
        case Partial_Application:        return "Partial_Application";
        case Apply_Alloc_Failed:         return "Apply_Alloc_Failed";
        case BugScenario:                return "BugScenario";
    }
    return "";
}
#endif


void* GC_apply_replay() {
    GcState* state = &gc_state;
    if (state->replay_ptr == NULL) return NULL;

    Tag replay_tag = ((Header*)state->replay_ptr)->tag;
    #ifdef DEBUG
        printf("GC_apply_replay: replay_ptr = %p, tag = %x\n", state->replay_ptr, replay_tag);
    #endif

    /*
    Analyse heap values to work out which of several scenarios we're in.

    replay_ptr      newer stackmap  scenario
    ----------      --------------  --------
    Push            Pop             Finished
    Push            Push            Unfinished saturated call
    Push            Tailcall        Unfinished saturated tail call
    Closure(sat)    Pop             Finished
    Closure(sat)    Push            Unfinished curried call
    Closure(sat)    Tailcall        Unfinished curried tail call
    Closure(unsat)  (N/A)           Partial application, unsaturated
    next_alloc      (N/A)           Failed to allocate Push or Closure
    */

    ReplayScenario scenario;
    GcStackMap* push;
    GcStackMap* newer;
    Closure* closure;
    size_t* after_closure = NULL;


    // Each time we advance to next heap value, need to check if we've passed end of heap.
    // Early breaks are handy here, so use do-while(0)
    do {
        if (state->replay_ptr >= state->next_alloc) {
            scenario = Apply_Alloc_Failed;  
            break;
        }
        if (replay_tag == Tag_Closure) {
            // Replay points to a Closure that was allocated last time in Utils_apply
            // It was a partially-applied Closure that had some more values applied
            closure = (Closure*)state->replay_ptr;
            if (closure->n_values != closure->max_values) {
                // Closure did not get enough new values to be saturated
                scenario = Partial_Application;
                break;
            }

            after_closure = next_heap_value(closure);
            if (after_closure >= state->next_alloc) {
                scenario = Apply_Alloc_Failed;
                break;
            }

            // Closure got saturated and started executing
            push = (GcStackMap*)after_closure;
            newer = push->newer;

            if ((size_t*)newer >= state->next_alloc) {
                scenario = Unfinished_Curried_Normal;
                break;
            }

            switch (newer->header.tag) {
                case Tag_GcStackPop:
                    scenario = Finished;
                    break;

                case Tag_GcStackPush:
                    scenario = Unfinished_Curried_Normal;
                    break;

                case Tag_GcStackTailCall:
                    scenario = Unfinished_Curried_Tail;
                    break;

                default:
                    scenario = BugScenario;
                    #ifdef DEBUG
                        fprintf(stderr, "GC_apply_replay: no newer stack item after %p\n", state->replay_ptr);
                    #endif
                    break;
            }

        } else if (replay_tag == Tag_GcStackPush) {
            // This was a saturated call (all args applied at once)
            push = (GcStackMap*)state->replay_ptr;
            newer = push->newer;
            if ((size_t*)newer >= state->next_alloc) {
                scenario = Unfinished_Sat_Normal;
                break;
            }
            switch (newer->header.tag) {
                case Tag_GcStackPop:
                    scenario = Finished;
                    break;

                case Tag_GcStackPush:
                    scenario = Unfinished_Sat_Normal;
                    break;

                case Tag_GcStackTailCall:
                    scenario = Unfinished_Sat_Tail;
                    break;

                default:
                    scenario = BugScenario;
                    #ifdef DEBUG
                        fprintf(stderr, "GC_apply_replay: expected stack map value at %p\n", newer);
                    #endif
                    break;
            }

        } else {
            scenario = BugScenario;
            #ifdef DEBUG
                fprintf(stderr, "GC_apply_replay: expected Closure or Push at %p\n", state->replay_ptr);
            #endif
        }
    } while (0);

    /*
    Decide what to do based on which scenario we're in.
    - What should Utils_apply return?
    - How to find the next position of the stackmap pointer?
    - How to find the next position of the replay pointer?

    scenario                  apply returns    stackmap         replay_next
    --------                  -------------    --------         -----------
    Finished                  Pop data         push-newer       stackmap-next
    Unfinished Sat. call      eval args        push             stackmap-next
    Unfinished Sat. tail      eval tc data     push-tc          stackmap-next
    Unfinished Curried        eval args        closure-push     stackmap-next
    Unfinished Curried tail   eval tc data     closure-push-tc  stackmap-next
    Part apply, unsaturated   closure(unsat)   (no change)      closure-next
    Part apply failed         fuller closure   (no change)      NULL (exit)
    */

    void* replay;
    size_t* replay_next;
    GcStackMap* stackmap_next;
    size_t stack_depth_increment;

    // Specific meanings for NULL
    GcStackMap* STACKMAP_UNCHANGED = NULL;
    void* RE_EXECUTE = NULL;
    void* EXIT_REPLAY_MODE = NULL;

    switch (scenario) {
        case Finished:
            replay = newer->replay; // saved return value
            stackmap_next = newer;
            stack_depth_increment = 0; // we've pushed AND popped
            replay_next = next_heap_value(stackmap_next);
            break;

        case Unfinished_Sat_Normal:
            replay = RE_EXECUTE;
            stackmap_next = push;
            stack_depth_increment = 1;
            replay_next = next_heap_value(stackmap_next);
            break;

        case Unfinished_Sat_Tail:
            replay = newer->replay; // saved full closure
            stackmap_next = newer;
            stack_depth_increment = 1; // push and tailcall = push
            replay_next = next_heap_value(stackmap_next);
            break;

        case Unfinished_Curried_Normal:
            replay = RE_EXECUTE;
            stackmap_next = push;
            stack_depth_increment = 1;
            replay_next = next_heap_value(stackmap_next);
            break;

        case Unfinished_Curried_Tail:
            replay = newer->replay; // saved full closure
            stackmap_next = newer;
            stack_depth_increment = 1; // push and tailcall = push
            replay_next = next_heap_value(stackmap_next);
            break;

        case Partial_Application:
            replay = closure;
            stackmap_next = STACKMAP_UNCHANGED;
            stack_depth_increment = 0;
            replay_next = after_closure;
            break;

        case Apply_Alloc_Failed:  
        case BugScenario:
            replay = RE_EXECUTE;
            stackmap_next = STACKMAP_UNCHANGED;
            stack_depth_increment = 0;
            replay_next = EXIT_REPLAY_MODE;
            break;
    }
    
    #ifdef DEBUG
        printf("GC_apply_replay:\n");
        printf("    scenario = %s\n", scenario_to_string(scenario));
        printf("    replay = %p\n", replay);
        printf("    stackmap_next = %p\n", stackmap_next);
        printf("    stack_depth_increment = %zd\n", stack_depth_increment);
        printf("    replay_next = %p\n", replay_next);
    #endif

    // Update the state
    // Don't do the memory access unless value is changed

    if (stackmap_next != STACKMAP_UNCHANGED) {
        state->stack_map = stackmap_next;
    }

    if (stack_depth_increment) {
        state->stack_depth += stack_depth_increment;
    }

    if (replay_next >= state->next_alloc) {
        replay_next = EXIT_REPLAY_MODE;
    }
    state->replay_ptr = replay_next;

    return replay;
}

/*
// Check compiled size of GC in Wasm
// Only 4kB!!! :)
// I don't have replay or controller yet, but wow
// And this includes some fixed overhead that emcc generates
//
int main(int argc, char** argv) {

    ElmValue** root = (ElmValue**)argv[0];
    size_t word = (size_t)argc;
    Closure* c = (Closure*)argv[1];
    ElmValue* v = (ElmValue*)argv[2];

    GC_init();
    GC_register_root(root);
    GC_malloc(word);
    void* push = GC_stack_push();
    GC_stack_tailcall(c, push);
    GC_stack_pop(v, push);

    GcHeap* heap = (GcHeap*)argv[3];
    size_t* pword = (size_t*)argv[4];
    GcState* state = (GcState*)argv[5];

    set_heap_end(heap, pword);
    mark(state, pword);
    compact(state, pword);
}
*/
