# Discourse post

## what's the update since last time?

- a working app
- as much as feasible in Wasm.
  - The part that's not feasible in current state of Wasm ecosystem is Effects.

# Wrapper docs outline

## What are the key issues?

- With the standard Elm compiler, both the generated code and the Kernel code are in the same language - JavaScript.
- But now we have Elm code compiled to WebAssembly and Kernel code in JavaScript.
- This means that there are two completely different sets of representations for Elm values like Lists, Tuples, Records, functions, etc.
- For documentation on how the Elm data types are implemented in WebAssembly, [click here](./data-structures.md)

- Wasm

  - sources
    - user Elm code
    - std lib Elm code
    - pure modules from std lib JS kernel, translated to C
  - value representations
    - byte-level structures, implemented in C
  - memory management
    - Custom WebAssembly Garbage Collector

- JS
  - sources
    - effectful modules from std lib JS kernel
  - value representations
    - JS structures as per standard Elm
  - memory management
    - JavaScript engine's Garbage Collector

| Wasm side                             | JS side                           |
| ------------------------------------- | --------------------------------- |
| user Elm code + standard lib Elm code | standard libs JS kernel only      |
| pure core modules                     | effectful core modules            |
| new binary value representations      | standard JS value representations |
| custom-built GC                       | browser GC                        |

- This is crossing a language boundary _within_ the Elm app itself. It
- To get an app working, the key challenge was dealing with this _internal_ boundary.
- However I'm putting it in the same place as the existing pure/impure boundary. So not that new.

## Avoiding memory management issues

- Never hang on to anything that crosses the boundary. Materialise it on the other side instead.
- Originally thought I could/should keep a "working set" in JS of stuff Wasm is using in a JS array, where Wasm refers to indices of it. But there's no good way to know when to deallocate. Wasm could put one of those refs in its Model and hold on to it forever. Hard to know if this is happening or not without creating some connection between this array and the Wasm GC.
- More reliable to just "pass the object through the boundary". Or pass "ownership" of it.
- Results in some objects getting copied twice if they go back and forth.

## Kernel Thunks

### The problem:

- Wasm can't access Web APIs. It has to go through JS.
- Therefore we want to leave effectful kernel modules in JS because they all need web APIs.
- But how does Wasm refer to functions and values from those JS modules? The update function needs to call functions that return Cmd's and Tasks, manipulate them as values, and return them from update, init, etc.
- But we just said we want those kernel modules to be in JS, and Wasm can't have a pointer to a JS value!

### first idea

- Maybe we could keep the JS values in an array on the JS side and Wasm could refer to them by index?
- Yeah but it's tricky to know if it's safe to _delete_ things from this JS array! Need to know that Wasm has no references to it, which ties it in to the Wasm GC. It can be done but it's not very satisfactory.

### eventual solution

- Calls to JS Kernel functions are all treated as "lazy". If an Elm function calls some function that returns a Cmd, that Cmd value is represented as a "suspended" call. A thunk that is not evaluated until needed. It's quite Haskell-like!
- When a Cmd is returned from `update`, the JS/Wasm wrapper evaluates the thunk before passing the Cmd along to the JS kernel.
- This means we don't need to have Wasm representations of Kernel data structures. Those values are represented as thunks that are never evaluated on the Wasm side, but only once they cross the boundary into JS-land.
- we only have refs to _global constants_ implemented in JS (most of them functions), so we never have to deal with de-allocation

## Process of writing to Wasm

- Check what address to write to (next alloc from GC), and how much space there is.
- Do a depth-first recursive traversal of the JS structure to write. Want to write children first because in this immutable-only heap, values may only hold references to _previously existing_ values.
- When writing is finished, tell the GC where to put its next_alloc
- During writing we may overflow the max allowed space. In this case, abandon everything, do a GC, and start again. Would be very complex to move all the values and then adjust pointers on the JS side! Take the perf hit and just redo. Make it rare by picking good numbers.

## Closures

- When reading a function out of Wasm, we need to attach its free variables to the "outside" of the function so that we can recover them when passing back _in_ to Wasm. They're also closed-over as well so we the JS runtime has two copies of the same thing. Could get rid of one of them. I haven't explored that. Code was simpler keeping both.
- Some kind of step-by-step walk through of a callback going out and coming back.

## Kernel functions

- Matching data structures in C and JS. JS array holds functions. C enum where the names are in the same order so values match the JS indices.

- Thunks approach! Infinite arity!
  - This is super interesting, definitely highlight it

## App types

## Two types of Kernel code

- Pure low-level ops
  - These get replicated in Wasm
- Effectful things
  - Almost all of these are using Web APIs so I'm leaving them in JS. Could put more of it in Wasm but benefit is questionable given the amount of work involved. (Scheduler.js is a bit of a beast)

Compiler will need to know which is which. Needs some kind of baked-in list of core modules implemented in Wasm. Already has similar kinds of things though.

## Ambiguities in translation

- We have two different representations of Elm values
- There are some ambiguities that are OK within _one_ world but not OK when we start translating between them
- Tuple2 = Record {a,b} in `--optimize` mode.
- Int = Float. No way to tell at low level. `1.0 : Float` looks the same as `1 : Int`. Within standard Elm it's OK because it has been type checked at source level so implementation doesn't care. But we care when crossing the boundary. In theory could get type info from compiler but it doesn't provide it because JS doesn't use it.
- However main use case for JS/Wasm translation is Msg constructors. Maybe could export these from `Main.elm` so that Elm compiler saves the interface and we can use it for code gen? Provide some metadata to the wrapper about which types take Ints or Floats. But in most cases you don't have to use an actual Msg constructor, the callback can be any function that returns a Msg. So there is some more sophisticated type info needed in this case. Those callbacks would need to be exported I guess. But long term the compiler prob needs to propagate this type info in order to support Wasm. At least until Wasm gets its own Web APIs.
- JS int is neither 32 nor 64 bit. Bitwise lib treats as 32 bit but Time.now needs 40 bits. This is inherited from JS but hard to achieve compatibility in Wasm, and when crossing the boundary back and forth.
- Perhaps there's a case for making it all explicit in Elm source code, but that would require more progress on code gen first.
- Custom constructors: Need to have a 1:1 correspondence between JS strings and Wasm integer IDs. When JS is not using `--optimize`, we can't just use the integers that `--optimize` _would_ have used. That could lead to ambiguities sending values from Wasm to JS.
