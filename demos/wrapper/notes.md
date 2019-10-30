# Docs outline

- Avoiding memory management issues

  - Never hang on to anything that crosses the boundary. Materialise it on the other side instead.
  - Originally thought I could/should keep a "working set" in JS of stuff Wasm is using in a JS array, where Wasm refers to indices of it. But there's no good way to know when to deallocate. Wasm could put one of those refs in its Model and hold on to it forever. Hard to know if this is happening or not without creating some connection between this array and the Wasm GC.
  - More reliable to just "pass the object through the boundary". Or pass "ownership" of it.
  - Results in some objects getting copied twice if they go back and forth.

- Process of writing

  - Check what address to write to (next alloc from GC), and how much space there is.
  - Do a depth-first recursive traversal of the JS structure to write. Want to write children first because in this immutable-only heap, values may only hold references to _previously existing_ values.
  - When writing is finished, tell the GC where to put its next_alloc
  - During writing we may overflow the max allowed space. In this case, abandon everything, do a GC, and start again. Would be very complex to move all the values and then adjust pointers on the JS side! Take the perf hit and just redo. Make it rare by picking good numbers.

- Closures

  - When reading a function out of Wasm, we need to attach its free variables to the "outside" of the function so that we can recover them when passing back _in_ to Wasm. They're also closed-over as well so we the JS runtime has two copies of the same thing. Could get rid of one of them. I haven't explored that. Code was simpler keeping both.
  - Some kind of step-by-step walk through of a callback going out and coming back.

- Kernel functions

  - Matching data structures in C and JS. JS array holds functions. C enum where the names are in the same order so values match the JS indices.

- App types

* Two types of Kernel code

  - Pure low-level ops - these get replicated in Wasm
  - Effectful things - almost without exception these are using Web APIs so I'm leaving them in JS. Could put more of it in Wasm but benefit is questionable given the amount of work involved. (Scheduler.js is a bit of a beast)
  - Compiler will need to know which is which. Needs some kind of baked-in list of core modules implemented in Wasm. Already has similar kinds of things though.

* Ambiguities in translation
  - We have two different representations of Elm values
  - There are some ambiguities that are OK within _one_ world but not OK when we start translating between them
  - Tuple2 = Record {a,b} in `--optimize` mode.
  - Int = Float. No way to tell at low level. `1.0 : Float` looks the same as `1 : Int`. Within standard Elm it's OK because it has been type checked at source level so implementation doesn't care. But we care when crossing the boundary. In theory could get type info from compiler but it doesn't provide it because JS doesn't use it.
  - However main use case for JS/Wasm translation is Msg constructors. Maybe could export these from `Main.elm` so that Elm compiler saves the interface and we can use it for code gen? Provide some metadata to the wrapper about which types take Ints or Floats. But in most cases you don't have to use an actual Msg constructor, the callback can be any function that returns a Msg. So there is some more sophisticated type info needed in this case. Those callbacks would need to be exported I guess. But long term the compiler prob needs to propagate this type info in order to support Wasm. At least until Wasm gets its own Web APIs.
  - JS int is neither 32 nor 64 bit. Bitwise lib treats as 32 bit but Time.now needs 40 bits. This is inherited from JS but hard to achieve compatibility in Wasm, and when crossing the boundary back and forth.
  - Perhaps there's a case for making it all explicit in Elm source code, but that would require more progress on code gen first.
  - Custom constructors: Need to have a 1:1 correspondence between JS strings and Wasm integer IDs. When JS is not using `--optimize`, we can't just use the integers that `--optimize` _would_ have used. That could lead to ambiguities sending values from Wasm to JS.
