Next steps
==========
- Platform/runtime
    - Goals
        - be able to create a `Program`
        - figure out some of the tricky stuff to do with effects and JS interop
    - Translate Scheduler.js and Platform.js to C & Elm
        - Maybe Elm isn't the best way to go. Might be better in C.
        - The only constraint I really need is that new must point to old. Other than that I can mutate values.
    - Scheduler & Tasks
    - Ports, Effect Managers, HTTP, vdom
- Other core libs
    - JSON (required for all the effects stuff)
- More handwritten prototypes of stuff that will be code generated
    - _interesting but not really blockers or priorities_
    - Destructuring
    - Case expression / pattern match (Decider, Choice)
    - Cover all the `Expr` constructors


GC next steps
=============
- Implement replay mode
- Refactor GC state types
    - figure out ElmValue, Header, u32, etc.
    - try to use arrays where possible rather than pointer arithmetic
- Implement compaction
- Measure GC code size in Wasm
    - Create example Elm program to generate a list or something
    - Compile option to include only allocation, no collection.



Dynamic allocation
==================
- `brk` moves the top of the data segment + heap.
- This is how to ensure the heap is in the right place
- GC_init needs to call `brk`
- Have some min amount to grab at a time. 64kB? 1MB? 128MB?
- On init, round up current size to a page, then add a few more pages

- Contiguous heap
    - Much nicer for the main data area to be contiguous rather than paged
    - When we grow total memory, bitmap grows too
    - Reserve room for the bitmap at the _top_ of the heap, enough for a full GC
    - Bitmap is only used during the actual collection, so can be created fresh each time (reset first)
    - If it were at the bottom, we'd have to do a slide _up_ to make room for it when we grow memory
        - We'd have to move the _oldest_ values out of the way to make room for the bitmap. Better to leave them happily sit at the bottom forever.


Code structure:

Mutator API
- GC_init
- GC_register_root()
- GC_malloc()
- GC_maybe_collect()
    - Just finished update, view, or dispatch_effects
    - The only things on the call stack are GC roots

- GC_stack_push
- GC_stack_pop
- GC_stack_tailcall
- GC_next_replay

Mutator API can use hidden state but always delegates to functions handling state explicitly


GC operations
- slide_down(ignore_below)
- mark(ignore_below)
- grow()
- shrink()
- controller(state)
    - make the decisions. how much garbage is available to collect, are we in the middle of executing, should we grow or shrink memory, do we have enough idle time for things?
- move_value(from, to)
    - case tag
    - forward pointers



Bitmap operations
- Mark
    - reset()
    - mark_value(u32 slot)
- Controller
    - find_enough_garbage_bottom(u32 target_garbage)
    - find_enough_garbage_top(u32 target_garbage)
    - count_garbage(u32 from, u32 to)
- Compact 
    - bitmap_start_iteration(u32 slot, u32* word, u64* mask)
    - u32 slot = next_block(bool live, u32* word, u64* mask)
    - record_block_offset(u32 slot, BitmapIter* iter)
    - forwarding_address(u32 old_slot, Bitmap* bitmap)



- forwarding_address(old_pointer, compaction_start, offsets, bitmap)
    - old_block_start = old_pointer & BLOCK_MASK
    - offset_in_block = bitmap_live_between(bitmap, old_block_start, old_pointer)
    - offset_index = (old_block_start - compaction_start) / block_size
    - new_block_start = offsets[offset_index]
    - return new_block_start + offset_in_block;


Compaction(compaction_start)
- to = bitmap_next_garbage(bitmap, compaction_start)
- from = bitmap_next_live(bitmap, to)
- next_block_ptr = NULL

- while (from < top)
    - next_garbage = bitmap_next_garbage(from)
    - while(from < next_garbage)
        - Update offsets if this is first in block
            - If (from >= next_block_ptr)
                - current_block_idx = pointer_to_block_idx(from)
                - offsets[current_block_idx] = to
                - next_block_ptr = (from & BLOCK_MASK) + block_size

        - Move current value
            - find_children(value)
            - for each non-pointer
                - `*to++ = *from++`
            - for each child pointer
                - if (from < compaction_start)
                    - `*to++ = *from++;`
                - else
                    - `*to++ = forwarding_address(*from++, compaction_start, offsets, bitmap)`

        - Comments
            - `from` is now pointing to next object
            - `to` is now pointing to next free slot
            - could have advanced many blocks, not just 1 (e.g. long string)
                - so what do I do with the intermediate offsets? Hopefully nobody pointing to them!

        - Go to next value if live
    <!-- from == next_garbage -->
    - from = bitmap_next_live(bitmap, next_garbage)
    - Go to next patch of live values


bitmap should contain its own top word and top bit mask, and the bottom of heap that it represents




BitmapIter is iteration state as we iterate the bitmap linearly through the heap during compaction
word and bitmask (1-bit high)


Alignment
- Slots should be sizeof(void*) because that's the alignment the compiler seems to expect.

Controller algo
- What can be known before marking?
    - How big is nursery?
    - How big is older generation area?
    - How much space left before we need more?
    - Are we executing on the stack, or between updates?
    - Historical stats for prediction
        - What % reduction do we normally get for nursery and older gen? IIR filter. (Complex algo design to use this, but small code) Easier to just do the marking?
- What can be known after marking?
    - Amount of garbage and live in nursery
    - Amount of garbage and live in older gen
- What actions can we take? Cheapest first:
    - Do nothing
    - Between updates, do a minor GC
    - Between updates, do a major GC
    - Between updates, request more memory and slide up
    - Interrupt execution and do minor GC
    - Interrupt execution and do major GC
    - Interrupt execution and request more memory
        - slide up now or later (bitmap allocated during slide)
- How to map inputs to actions?
    - Requesting more memory
        - I know it's expensive but how expensive relative to GC? Needs benchmarking.
        - When to fall back to it?
        - Too many minor GCs in a row
        - Not very much garbage lying around
- Dumb algorithm
    - Do a minor GC every N updates
    - Do a major GC every M updates
    - Grow whenever we need more memory
    - Total heap size will balance out at some multiple of what's needed per update
    - How/when to shrink?
        - Keep track of spare room over last X cycles (X>M)
        - If spare room > 2 pages then reduce



GC thoughts
===========
- Stack map
    - Build stack map tracking into `apply`
    - On entering a new Closure, cons a `StackPush` onto the stack map. It's a marker for which values were created in which functions.
    - On returning from a Closure, create a `StackPop`
        - A Cons where the tail is the `StackPush` where we entered the same Closure
        - Threading the StackPush pointer through `apply` to the Stack Pop allows us to skip over all deeper function calls
        - Replaces the old head, skipping over any other pushes and pops that happened in between (because they're not on the call stack anymore)
        - `StackPop` has a return value pointer
    - On returning with a tail-call closure, create a `StackTailCall`
        - try to find a `StackPush` that entered this Closure and link to it
        - if instead we find another `StackTailCall` that went _into_ the current closure,
            bypass it and link to _its_ `StackPush`
    - The address of each is, by definition, the location of the heap pointer when it was created
    ```elm
    type StackMap
        = StackPush Closure StackMap
        | StackPop ElmValue StackMap
        | StackTailCall Closure TailClosure StackMap
        | StackRoot
    ```

- Does StackPush need to reference its Closure?
    - Only makes a difference for the entry function
    - All Closures for deeper stack levels are either:
        - Allocated by live functions and so will get marked by them
        - Located off-heap as global constants
    - The entry point for the stack is always an effect callback, like a DOM event handler or HTTP response handler, etc.
    - This is in an effect manager, and likely has some closed-over values in it (such as which Msg to sendToApp)
    - We can only unwind & replay the stack below `sendToApp`. It's only below that that things are guaranteed pure and replayable. Never pop back up *into* `sendToApp`. Stay below it.
    - Maybe `sendToApp` itself places a special marker in the stackmap like `StackReplayLimit` instead of `StackEmpty`.
    - When unwinding, `apply` needs to look out for the top level replayable closure.
    - So we're never allowed to use `apply` for impure functions


- Effects and replay
    - What happens to my impure Elm Kernel code?
        - Only have a few impure functions, they can set a marker to prevent replaying an effectful function.
        - I've only done this in Scheduler. All they do is mutate a global state pointer, which is a GC root as well.
        - Maybe better off in C anyway.
    - What do the stack and heap look like in effects?
        - Effects happen after an update cycle via `_Platform_dispatchEffects`
        - Generates an effect object per manager
        - Does a `_Scheduler_rawSend` for each
            - Pushes a message into the mailbox of that process

    - Actual effects triggered by `_Scheduler_step`, which is called from `_Scheduler_rawSpawn` and `_Scheduler_rawSend`


- Replacing stack pointers using indirect pointers / double pointers
    - Might rely on C compiler optimisations not being too clever
    - If C puts things in registers, we're screwed. Can't continue running those functions after GC.
    - Would have to use `volatile` for every pointer to an Elm value so it rereads them
    - So this basically blocks good assembly optimisations


- Unwinding the C call stack
    - Need to do this in order to GC & replay, otherwise stack overflow
    - Need almost an 'exception' mechanism to bail out
    - Something more monadic-looking? Like a `HeapOverflow` header tag?
        - a special runtime-only Elm value
        - check for it on every allocation/call
        - Use macros to avoid having to remember to return? Also easy to disable GC for future Wasm.


- No point partially unwinding the stack, calculating garbage is expensive
    - Easy to find the gaps, harder to determine how much of that is the return value.
    - Rerunning higher level functions frees way more stuff


- Replay algorithm
    - On heap overflow
        - return `HeapOverflow` from bottom function to unwind the "real" stack in C
        - `apply` watches out for the highest function (`update` or `view`). When it sees it, stops unwinding the stack and doesn't return.
        - Now `apply` triggers the prep for GC & replay. Afterwards it will call the evaluator again
    - Prepare for GC & replay
        - Mark all the stuff allocated by the target function as "live"
        - StackMap is a GC root. Everything we don't need has been removed (except Pushes but keep them for simplicity)
        - Count the length of the Pop replay list and store it in a GC variable.
    - Minor GC
        - No need to worry about marking stuff from other GC roots
            - they are all older than the stack and we're not going to sweep/compact them!
            - browser prevents async things coming in during this tick
        - Mark everything allocated by the target function
            - Two loops: outer loop iterates over StackMap (descending addresses), inner loop iterates _up_ through heap objects.
            - Need a separate "mark" algorithm because we're not traversing trees of objects via pointers
        - Compact everything from the Push into the target function
        - Set heap pointer
        - Return all the way up to the `apply` where we unwound the stack map and let it call the evaluator
    - Replay
        - `malloc` becomes a playback function
            - Tracks a replay pointer instead of normal heap pointer.
            - Returns the replay pointer (to an object created on previous run) and advances it by the size of that object
        - `apply` becomes a playback function
            - Get the `n`th item in the pop list and decrement `n` (it's a fair bit of iterating but not too bad)
            - Increment malloc's replay pointer by the size of a Pop.
    - Continue as normal
        - Pop list size has dropped to zero, nothing left to replay => `apply` returns to normal mode
        - malloc replay pointer has reached heap pointer => `malloc` returns to normal mode


- Self recursion
    - Self tail calls are optimised to `while` loops. What to do if we run out of memory in the middle of one?
    - Before going into the `while` loop, allocate a Closure and a stack map item just in case we need them
        - Stack map item is a `Pop` or some special thing for tail calls
    - If `malloc` or `apply` ever return `HeapOverflow`
        - break out of the `while`.
        - Copy the args into the Closure (or special `TailCall` closure with unique tag)
        - return to apply with `HeapOverflowContinuation` or `HeapOverflow TailCall`
        - `apply` puts the continuation into the stack map
        - after GC, continue by calling the evaluator again with the `TailCall`
    - General recursion easy to add on
    - Self-recursion doesn't pay the extra cost of general recursion (trampolining)

- Algo for `apply`
    - If not replay mode
        - Create a `StackPush`
        - If it's a saturated call, result = evaluate(args)
        - If it's a partial application
            - allocate a closure (maybe result = `HeapOverflow`)
            - if not saturated, result = closure
            - if saturated, result = evaluate(closure)
        - Inspect result's type tag
            - `HeapOverflow` => return `HeapOverflow`
            - `HeapOverflowTailCall` => create `StackTailCall`, return `HeapOverflow`
            - `TailCall` => create `StackTailCall`, loop back
            - anything else => create `StackPop`, return value
    - If replay mode
        - Save replay_pointer to return_pointer
        - Decrement n_replays
        - For next time: Iterate through StackMap, stop after n_replays StackPops, set replay_pointer to this.
        - if return_pointer points at a full closure, evaluate it
        - otherwise return return_pointer

- Do I need `StackTailCall` or will `StackPop` do?
    - Self recursion interrupted => create stackpop with a full closure, special case it in 'replay mode'


- Reversing the StackMap or pop list for replay
    - Before doing the replay, reverse the pop list
    - => need to keep space on the heap for it
    - need a running calculation of how many pops you need to reverse the list to a new one
    - would need Pop objects to have a running total in them
        - whenever a Pop bypasses a load of the stackmap, increments from the pop before the push that it just skipped from
    - the speedup of replay is maybe not worth the complexity and the extra processing to track it on _every_ pop


Mark/compact algorithms
=======================
Fastest from book is 'Compressor'
- Keep a bitmap of live ints
- Break memory into 256 or 512-byte blocks



Initialising static values
==========================
There are lots of values that belong in the 'static' area of memory rather than the stack or the heap areas.
- In a C program, these are global variables declared outside any function, and are pre-loaded before `main` is called.
- Wasm has a special feature for this, the data segments. Memory is pre-loaded from the data segments as soon as the module loads. In Wasm text format they are specified as strings.
- Using this mechanism would be nice for fast start-up time, but it's tricky because C doesn't allow it for `struct`s.
- We can also initialize them dynamically at the start of `main`.
- Elm static values
    - Top level functions: Closures with no 'free values' => size is known, which is good
    - Elm primitive literals anywhere in the program (Int, Float, String, Char)
    - Built-ins like Unit, True, False, Nil
    - Record fieldset
    - Custom type constructor (function or constant)
    - Top-level constant expressions (evaluated once on program initialization)
    - Top-level recursive values (special case of constant expressions)
- Pointers vs values
    - Most Elm expressions will return pointers, not values. Code gen way more feasible that way.
    - Therefore most code refers to pointers to the static value rather than the value itself.
    - This is kind of how C strings work.
- C compiler constraints
    - To initialise a static value in the declaration itself, you can only use _constant expressions_
    - This is a specific C syntax term, not just the obvious meaning. Only numeric literals, string literals, arithmetic ops, ternary op
    - In particular _structs are not considered 'constant expressions'_, even if all the fields are constant. This is quite annoying.

- Implementation options
    - Static allocation & initialisation
        - C compiler won't let us use structs, but maybe we can typecast...
        - C strings actually have pointer type, so we can cast them to another pointer type!.
        - `Closure* myElmFunction = (Closure*)"\0\1\123\111";` (with octal escapes)
        - Value is anonymous, only the pointer has a name. Nice. C code less cluttered with names.
        - BUT... either Elm compiler needs to know byte-level details (=> generated C is not cross-platform, bummer)
        - OR we hide those details in a C macro somehow (including conversion to octal? Yuck)
        - AND for Closures we would need to put a function pointer into this string literal, which is **impossible**.
        - Can't be preloaded as zero then overwritten from C `main`. This results in a segmentation fault.
        - Initialising from a C string is possible, as long as Elm compiler can generate a Header, for the following structs:
            - ElmString
            - ElmChar
            - ElmInt
            - ElmFloat (need to know 32 or 64 bit, need to get encoding right)
            - FieldSet
        - With this technique there's no way to ensure 32 or 64 bit alignment. C strings have no reason to be aligned. gcc does it for longer strings, but emcc doesn't. But meh.
        - OK so I'll do this for string literals but nothing else. Strings are the only things that can be big, the rest don't matter.

    - Static allocation, dynamic initialisation
        - Two variables: value & pointer
        - init function initialises the value, and points the pointer at it.
        - The value can be just local to the file. The pointer is what we export via the header
        - More clutter in the code, which is a bit annoying. Need a naming convention.
        - Trickier for variable sized structures
            - Variable size: String, Record, FieldSet, Custom
            - C has assumes the variable part of the struct has zero size. (Actually true for Closures on init.)
            - Would need to use type casting. Make the _value_ an array of bytes, with number of elements given by `sizeof` the real structure. Then just cast it to the real structure when initialising it.
            - For strings, prefer not to have two chunks of memory reserved!

    - Dynamic allocation, dynamic initialisation
        - We could just allocate all of this stuff on the heap.
        - Code looks the same as anywhere else, which makes things easier.
        - Just use malloc in the initialization function.
        - As long as GC is implemented correctly, they will never get collected. GC may waste time checking them to make sure they're still alive though.
        - Value is anonymous, only the pointer has a name. Nice. C code less cluttered with names.
        - It's slower startup. Gives the memory manager lots of work to do on initialisation. Meh, it's totally fine. But if I can find a nice way to statically allocate I'd prefer to do that.

    - How to do dynamic initialisation
        - Create a local struct literal and then `memcpy`
            - bonkers, why do this?
        - Assign to struct literal
        - Field by field


- Need to be initialised dynamically, C won't allow structs to be global static constants
    - This is a limitation of C (and actually Rust too). Must be some reason for it but I suspect it's a reason that may not apply in the case of language implementation as opposed to handwritten C code.
        - Global initialisers have to be C primitive literals: 
- Literals in expressions
    - May want to actually turn them into globals
    - Or create them on the stack in C? But this is tricky


Compiler special cases for numeric binops
=========================================
Could have special cases in code gen for Basics (0.19 JS code gen already has some).

While current expression is a (saturated) call to a Basics binop, recurse down the AST, emitting a C expression that gets evaluated in the stack or registers.

When the recursion finishes (end of expression or we reach a non-Basics function call or something), then and only then, save it to the heap.

That wrapper function could work on any `number`, checking the header of the final value of the expression for Int or Float. Or we could look up the return type of the function at the top of the AST in a Map. If we already know it's Int or Float, then there's no need to check at runtime. Let compiler optimise away some header code.

There are three versions of that final wrapper function, depending on the return type of the function at the top of the AST tree (which we can look up in a Haskell Map). The return value will either be number, Int or Float.

Basics functions:
- 14 return Float
- 7 return number
- 7 return Int

Would have both heap and stack versions of each Basics numeric op, where the heap version is built on top of the stack version but has a wrapper around it that does the allocation. Stack versions are marked `inline`.

Advantage is that the header is available to `number` functions. `Int` and `Float` functions will just have no header-related code.

As long as we can inline these functions, this should give the C compiler enough freedom to optimise things. They could also be function-like macros, but that might be more awkward.

It's still doing more header checks than a monomorphised version would, but at least it gets rid of intermediate allocations.


Unboxing integers
=================
What would it actually take?

All containers would need a bitmap. Could be just a single word (32 or 64)
It has to be pointer-sized so that for large containers, we can put the bitmap in an separate look-up table (like Haskell). LSB can be the marker of large/small container, like OCaml does for a different purpose.
Maybe it's simpler to just always externalise the bitmap.

This would also allow unboxing Unit, Bool, and Char.

So whenever we want to access a field of a container, we have to call a function to get it, rather than directly using the struct.

**Key Problem:** How does `compare` know that it's looking at an `Int` rather than a pointer to some structure? Not really feasible at runtime. Code gen would need to do something different for Int. Which means the AST going into code gen would need to have type information. Currently it doesn't, and I'm not going down that rabbit hole.


64 bit platforms
================
I've discovered that it's a _much_ faster dev cycle to compile to a binary executable on my laptop and run tests that way. Compile time is ~6x faster, and you don't have to open a browser window or set up some hot reload thing. Once all tests are passing in binary, I can run a WebAssembly test in the browser and fix up any remaining issues.

But my laptop is 64 bits and Wasm is 32, so a bit of work is needed to support both. It's also nice for future server-side Elm and whatnot.

Most of the C code is independent of 32 vs 64 bits. We just use array indices or structure fields and the compiler figures out the pointer offsets.

It mainly affects the `size` field of the header, which is intended for GC and for string length.

Padding is added for the following Elm types by gcc for 64-bit target:

|padded?| structure |
| ----- | --------- |
|       | ElmInt    |
| ✔️    | ElmFloat  |
|       | ElmChar   |
|       | ElmString |
|       | Nil       |
| ✔️    | Cons      |
| ✔️    | Tuple2    |
| ✔️    | Tuple3    |
|       | Custom    |
| ✔️    | Record    |
|       | Closure   |

- gcc is making sure that pointers and floats are on 64-bit boundaries for a 64-bit target
- All structure instances are placed on 64-bit boundaries, and the header is 32 bits, so anything that has only pointers or only floats in the body will get padding inserted
- Elm Float has only a float in the body.
- Const, Tuple2, Tuple3 and Record have only pointers in the body.
- Int, Char, String, and Nil don't have either floats or pointers so don't need this padding
- Custom and Closure already happen to have their pointers on a 64-bit boundary don't need padding
- OCaml expands header to 64 bits on 64-bit machines. But that doesn't help me here! Custom and Closure would end up getting padded instead.

OK so there doesn't seem to be any way to avoid the 64-bit alignment happening.

### So what should `size` be?
I want it to indicate the memory size of the structure's body, shifted by a few bits to gain some range. I decided to exclude the header from the size because it's metadata and things get too self-referential.

Also String length is more understandable this way.


### How to handle in GC
- GC wants to copy the whole structure, header and all
- For a Cons cell, I have a size of 2 and I need to copy 24 bytes.
- For an ElmChar I have a size of 0.5! **Can't use pointer units, need int units**
- Best way to handle it is probably in the `#define` statements for the headers. Make `size` always be right for the target.
- Function to find first pointer must also be different for 64-bit target.

### How to handle in String length & padding
- If `size` is measured in pointers, then just add more padding to the end of the string on a 64-bit target, and make sure the algorithm works.



Assertions & Exceptions
=======================
Is there any way to add assertions in C and optimise them away in prod?
Would be great to sprinkle them around to do type checks and other invariant checks.
Maybe an alternative is to have a `#define ELM_WARNINGS` that enables some `printf` warning statements.

For the cases where Elm actually throws exceptions, can I even do that?
What happens if I divide by zero? Nuthin'

Unit tests probably cover a lot of it though.


Optimisations
=============

Compilation time and file size for various emcc compiler [optimisation settings](https://kripken.github.io/emscripten-site/docs/tools_reference/emcc.html)

| option | time | wasm | js |
| ------ | ---- | ---- | -- |
| -O0 | 6.13 | 58,785 | 103,092 |
| -O1 | 4.85 | 28,695 | 68,823 |
| -O2 | 7.60 | 32,520 | 27,899 |
| -O3 | 9.49 | 30,209 | 21,999 |
| -Os | 9.06 | 25,744 | 21,999 |
| -Oz | 8.92 | 25,670 | 21,999 |

`-Os` and `-Oz` are size-only optimisations, the rest are a size/speed tradeoff.
- `-O3` is recommended for production.
- `-O1` seems good for development.

Less allocation
===============
Feel like I'm allocating too much stuff, and maybe doing it inside functions is a bad practice. Maybe the caller should make the decision on whether something is stack or heap.

Rust guidance is not to pass pointers back from functions as a way to save stack space. They said pass back the whole data structure and let the caller decide what to do with it. But also there was something about Rust being clever enough (for what wasn't clear but maybe it can use heap if it wants?).

So let's take `apply`. Can I pass back a whole data structure from that? It's so general it could be anything. Well any _top-level_ container anyway. And it could always be another closure of any size. Call site may not necessarily know the size at compile time, like if it's a higher order function.

Could apply return some union of either a pointer to a heap object OR a full stack object?

Structures whose (shallow) sizes are not known at compile time, at the call site
- Closure
- String
- Record (it's a concrete record type but which one? Can a function return an extensible record type other than the one you gave it? Actually I don't think so.)

Containers with type parameters are fixed-size but can point to other things. The pointer is the same size regardless of the type parameter's value.

This is what Rust was trying to make me do! Make everything known-size. It requires farming out Closure args and string body to separate structures.

Why do I care? Stack is no faster than heap to access, but heap involves more management (GC).

If I'm doing this, then the stack version of the object needs to be completely stand-alone and independent of the GC header. In other words String length can't be just in GC header, has to be in the String object itself so that it can exist in the stack.
Or I suppose you could put useless GC headers in the stack.
