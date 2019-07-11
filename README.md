# Project goals

- An experiment to try to implement Elm in WebAssembly
- Uses C as an intermediate language
  - This repo contains Elm Kernel code, garbage collector, etc. The parts of the system outside of the compiler itself.
  - Elm Compiler would output C code instead of JavaScript. A second step would further compile that to Wasm.
  - This approach makes it much easier to write Kernel code and to debug the output of the Elm compiler.
- C doesn't have first-class functions or high-level data structures like JavaScript does, so we have to implement some of those things. Most of this is in [types.h](/src/kernel/types.h) and [utils.c](/src/kernel/utils.c)
- The idea is to gradually build Elm's core libraries in C and write some tests to mimic 'compiled' code from Elm programs.

# Demos

https://brian-carroll.github.io/elm_c_wasm/


# Progress

- Kernel code / core libs

  - [x] Implement C data structures for all Elm value types: `Int`, `Float`, `Char`, `String`, `List`, tuples, custom types, records, functions
  - [x] Function application and currying
  - [x] Extensible record updates and accessors
  - [x] A prototype Garbage Collector (7kB download size 😊 No idea what the performance is like! See [gc.c](/src/kernel/gc.c))
  - [x] Numerical operators from the `Basics` library (`+`, `-`, `*`, `/`)
  - [ ] List module
  - [ ] Finalise String encoding issues, UTF-8, UTF-16, browser/JS interop, etc.
  - [ ] Complete String module
  - [ ] JSON & ports (currently most effects are not available in Wasm, which probably means using ports heavily)
  - [ ] `Program`, `Cmd`, `Task`, `Process`, scheduler, etc.

- Elm compiler modifications
  - [x] Initial experiments [forking][compiler] the Elm compiler to generate Wasm (decided to abandon this direction)
  - [ ] Modify Elm compiler to generate C

[compiler]: https://github.com/brian-carroll/elm-compiler/tree/wasm

- My activity levels!
  - I did lots of work on this during 2018, particularly the 2nd half
  - Other things in my life got busy in the first half of 2019, but my interest is reviving at the moment and I have more time on my hands again!
  - I meant to write some blog posts and see if I could get some interest from the community, but I ended up only writing one. It was on [first class functions][blogpost].

# Big picture stuff

- Effects
  - Wasm MVP has no Web APIs like DOM, XHR, etc., so most effect managers must be JS only for Wasm MVP
  - Unclear how to interface with browser APIs from C, I guess some header files you `#include` and call functions on.

- Browser GC
  - Wasm spec talks about data types like arrays, objects, opaque references.
  - Unclear what this looks like from C. I guess some header files with functions you can call. Will a pointer to an array look like a pointer or like an integer? Seems like they'd have to prevent dereferencing so I guess you treat it as an integer ID and all the functions that operate on that type take this ID as an arg.

- Custom GC
  - Impact of GC optimizations based on immutability
    - Want some way to allow Kernel to mutate things despite GC being optimized for immutability.
    - Solution: Keep all mutations outside of the heap. If an Effect Manager needs to dynamically allocate something and mutate it, it can first create new immutable value on the heap, then just mutate an off-heap pointer to point at new instead of old. This is like the way `model` updates already work in elm.js.
    - Process IDs being references may be a bit of a pain. Ideally they'd be integers. But only really used for `kill`, which isn't fully implemented even in JS.

- Kernel is now in multiple languages
  - Quite a bit of maintenance
  - Would it be good to put more of the code in Elm?

# Notes

## Mixing JS and Wasm in compiled output

- I'd been getting a bit overwhelmed with the amount of work necessary to get to compile a "hello world" Elm program to Wasm. Because of dead code elimination, you can't get any output from the compiler until you have an implementation for `Program`. And that requires building a lot of really complex stuff like the effect manager system.
- I need a way for the Elm program to end up as a mix of JS and Wasm. That way I can keep the kernel in JS initially, with the Elm code compiled to Wasm. Afterwards I'll see about gradually replacing the kernel with Wasm. Maybe that's not even needed?
- The barrier here is the fact that everything crossing the JS/Wasm boundary has to be serialized and de-serialized (as JSON strings)
- How does that work for `Cmd`?
  - Underlying union has a variant for Process_map containing a function, but this variant appears to never be used?!
  - `Cmd Msg` going from `update` to the runtime will have a `Msg` constructor function inside it
- JS calling `update`
  - Need to export a curried version of `update` so that JS `A2` works
  - Exporting an Elm function from Wasm to JS
    - Do all the currying in JS land. Inner function calls into Wasm.
    - Wasm export

## Effects

TODO

## GC

- I've built a prototype Garbage Collector so that I could run some Elm programs (well, hand-compiled ones since I don't have a compiler yet!)
- It uses a mark-compact algorithm that takes advantage of the fact that all Elm values are immutable and can therefore only point to _older_ values.
- _The GC fits into less than 7kB of binary Wasm!_
- I have a plan for how kernel code in C/Wasm can do mutations but still use this immutable GC. You only need to mutate a fixed number of "GC roots", which are pointers that sit outside the managed heap, pointing at values inside it.

## Closures

I previously wrote a [blog post][blogpost] about how to implement Elm first-class functions in WebAssembly. The Closure data structure in [types.h](./src/kernel/types.h) is based on those ideas, although it has evolved slightly in the meantime.

In a nutshell, the Closure data structure is a value that can be passed around an Elm program. It stores up any arguments that are partially applied to it, until it is "full". It also contains a function pointer, so that when the last argument is applied, that function can be called. A working example of all of this can be found in `test_apply` in [utils_test.c](./src/test/utils_test.c).

The version in the blog post used the same number of bytes regardless of the number of closed-over values held in the Closure. The new version is a bit more compact, using only the space it needs.

[blogpost]: https://dev.to/briancarroll/elm-functions-in-webassembly-50ak

## Extensible Records

- A good intro to Elm extensible records can be found [here](https://elm-lang.org/docs/records#access).
- In this project they are split into two C structures, `Record` and `FieldSet`, defined in [types.h](./src/kernel/types.h)
- The `Record` stores the values only. The field names are stored in a `FieldSet`, shared by all values of the same Record type.
- Field names are represented as integers. The compiler will convert every field name in the project to a unique integer, using the same kind of optimisation the Elm 0.19 compiler uses to map field names to short combinations of letters.

- Record accessor functions

  - Elm has special functions for accessing records, prefixed by a dot, like `.name`. The important thing is that this function can be applied to _any_ Record type that contains a field called `name`.
  - Implemented using a Kernel function that takes the field ID as an Elm Int, and the record itself

  ```elm
      recordAccess : Int -> r -> a
      recordAccess fieldId record =
          -- Kernel C code
          -- 1. Find the position of the `fieldId` in the record's FieldSet
          -- 2. Look up the same position in `record`
          -- 3. Return the value found
  ```

  - An accessor function for a particular field is created by partially applying the field ID in the generated code. In Elm syntax it would look something like the following:

  ```elm
      .name : { r | name : a } -> a
      .name = recordAccess 123  -- where 123 represents the field called 'name'
  ```

  - Field lookups are implemented as a binary search, which requires the fields to be pre-sorted by the Elm compiler
  - `recordAccess` is not safe if the field does not actually exist in the record, but it's not available to user Elm code, it can only be emitted by the compiler.

- Record update
  - `r2 = { r1 | field1 = newVal1, field2 = newVal2 }`
  - This Elm syntax is implemented using a C function `record_update`, found in [utils.c](./src/kernel/utils.c)
  - First it clones the original record
  - Then for each field to be updated
    - Finds the index of the field in the record's FieldSet
    - Changes the value at the same index in the Record

## SuperTypes / constrained type variables

 [constrained type variables][guide-type-vars] allow some functions like `++`, `+` and `>`, to work on *more than one, but not all* value types.

[guide-type-vars]: https://guide.elm-lang.org/types/reading_types.html#constrained-type-variables


|          | **number** | **comparable** | **appendable** | **compappend** |
| :------: | :--------: | :------------: | :------------: | :------------: |
|  `Int`   |     ✓      |       ✓        |                |                |
| `Float`  |     ✓      |       ✓        |                |                |
|  `Char`  |            |       ✓        |                |                |
| `String` |            |       ✓        |       ✓        |       ✓        |
|  `List`  |            |      ✓\*       |       ✓        |      ✓\*       |
| `Tuple`  |            |      ✓\*       |                |                |

\* Lists and Tuples are only comparable only if their contents are comparable

Low-level functions that operate on these type variables need to be able to look at an Elm value and decide which concrete type it is. For example the `compare` function (which is the basis for  `<`, `>`, `<=`, and `>=`) can accept five different types, and needs to run different low-level code for each.

This implementation adds a header to the byte level representation of every Elm data value. It's a 32-bit number that includes both a type tag (4 bits) and the size of that value in memory (28 bits).

| Name    | Tag |
| ------- | --- |
| Int     |  0  |
| Float   |  1  |
| Char    |  2  |
| String  |  3  |
| Nil     |  4  |
| Cons    |  5  |
| Tuple2  |  6  |
| Tuple3  |  7  |
| Custom  |  8  |
| Record  |  9  |
| Closure |  a  |

The remaining 5 possible values (`b` &rarr; `f`) are reserved for Garbage Collector record-keeping data.

## Headers

This implementation prefixes every Elm value with a header of 32 bits in size.
```
-----------------------------------------------------
| tag (4 bits) | size (28 bits) |      Elm data     |
-----------------------------------------------------
```
`size` is measured in _words_, where a word is either 32 or 64 bits, depending on the target platform. It makes sense to use words rather than bytes because all values are aligned to word boundaries anyway. For example in a 32-bit system, we'll always place our values at addresses that evenly divide by 4 bytes. Real CPUs are optimised to work faster when pointers are aligned this way.

The only individual value that can get really large in practice is `String`. (Lists don't count, they are made up of many Cons cells.) A maximum value of 2<sup>28</sup>-1 for `size` corresponds to 1 GB on a 32-bit system or 4 GB on a 64-bit system.

We always use 32-bit headers, even on 64-bit systems. 1GB is large enough, there's no point increasing the header size. Wasm is always 32 bits but since we're using C as an intermediate language, we can also create native 64-bit binaries. That's how I run most of my tests.

## Boxed vs unboxed numbers

TODO

## Padding & alignment

TODO

## Memory: stack, heap, static, registers

TODO

## Dropping type info

TODO

## Alternatives to C

### Rust

I really wanted to use Rust. It's just generally a better language. Hey, it's even heavily influenced by Haskell and ML, just like Elm!

But using Rust to implement Elm was difficult enough that I got frustrated and demotivated and stopped working on this project for a few weeks. This project is a hobby for me, so if I'm not enjoying it then it won't happen. And at some point it was either drop the project or switch to C.

Rust is _definitely_ a better language... for handwritten code. But we're talking about _generated code_ from the Elm compiler. It's different in a few ways.

Firstly, Elm's type system guarantees that a lot of potentially bad things can't happen. But the Rust compiler doesn't know that, so the extra strictness can really be a pain. We basically have to prove to Rust that some code that came out of the Elm compiler is type safe, even though we already know it is. This turned out to be tricky, and without much real benefit.

Secondly, it's well known that "fighting the borrow checker" is something that everyone learning Rust goes through. And I have never used Rust for a real project before, so I experienced it too. Apparently you eventually learn how to deal with it. But the borrow checker isn't actually designed for garbage-collected code anyway, so it's quite a lot of pain and effort with not much reward at the end!

Some of the patterns Rust encourages didn't seem to make sense to me in a garbage collected context. Rust encourages you to keep as many of your values as possible in the stack, which makes sense when you don't have a garbage collector - you want to destroy values as soon as you don't need them anymore. In Elm, a _lot_ more stuff is going to be allocated on the heap than in a normal Rust program and that should be OK.

It really felt like at every turn, I was spending all my time _fooling_ Rust into believing my code was OK, and in the process, invalidating a lot of the extra help it should give me compared to C. I also had a couple of years experience with C from about 10 years ago, so that was a factor too. But to be clear, I had no burning desire to go back to it!

### Direct to Wasm

My first approach to this project was to directly generate WebAssembly from the Elm AST using a forked version of the compiler. I wrote a set of Haskell types to model a WebAssembly module, and created a WebAssembly DSL and code generator from that.

I wrote a few of Elm's Kernel functions using the same Haskell DSL. So the Kernel code was actually written in Haskell that would generate WebAssembly.

That all worked reasonably well. I got an example Elm program working, which implemented currying and closures.

However as soon as I had my first real piece of debugging work to do, I realised it was pretty painful. That seems kind of obvious, but I'd thought the DSL would help a lot more than it did. Wasm is just so low-level I couldn't keep enough of it in my mind at one time to be useful for debugging.

However I really learned a lot about WebAssembly by doing this.
