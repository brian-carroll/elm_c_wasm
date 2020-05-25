I've posted here a
[few](https://discourse.elm-lang.org/t/first-class-functions-in-webassembly/1577)
[times](https://discourse.elm-lang.org/t/elm-core-libs-in-webassembly/4443)
[before](https://discourse.elm-lang.org/t/basic-demo-of-elm-in-webassembly/4627)
about my project to compile Elm to WebAssembly.

### Summary of previous posts

- I chose to use C as an intermediate language. (At this point, everyone asks "why not Rust?"! Rust is great for preventing errors in _handwritten_ code but I found it a very annoying compile target. And the Elm compiler does plenty of checks already. More details on that decision [here](https://github.com/brian-carroll/elm_c_wasm/#why-c-and-not-rust))

- In [this post](https://discourse.elm-lang.org/t/elm-core-libs-in-webassembly/4443), I described the custom Garbage Collector, and the parts of the core libraries I'd ported to Wasm.

- In [my last post](https://discourse.elm-lang.org/t/basic-demo-of-elm-in-webassembly/4627) I described the system architecture. The Elm runtime remains in JavaScript because WebAssembly doesn't have Web APIs yet. The Wasm app talks to the runtime through a JS wrapper.

- I also showed a demo of a very basic working app. It was "hand compiled" as I didn't have a working compiler yet.


### Latest news

I've created a [fork of the Elm compiler](https://github.com/brian-carroll/elm-compiler) that has a C code generator as well as the existing JavaScript one.

[Here](https://brian-carroll.github.io/elm_c_wasm/todo-mvc/) is a working demo of Evan's _TodoMVC_ example from a few years ago. I cloned [his repo](https://github.com/evancz/elm-todomvc) and compiled the Elm code to WebAssembly. 

The forked compiler accepts `--output elm.c` as a command-line option as well as `--output elm.js` and `--output elm.html`. Once I have the C file, I use [Emscripten](https://emscripten.org/) to further compile it to WebAssembly. There are a few build steps that I coordinate using GNU `make`.


### Development Status

So is that it? Is it all working? Can I use it in production right now? Is it really fast? OMG!

Nope! Sorry!

- There are still lots of bugs. For example I have not yet managed to get Richard Feldman's [elm-spa-example](https://github.com/rtfeldman/elm-spa-example) working. It's a great test-case because it's complex enough that if I have any bugs, it's bound to show them up!

- It's not fast yet. Before I can focus on that, I need to debug it and sort out some issues with the architecture (see "current focus" below).


## Current focus

A lot of the work I'm currently doing is on the JS/Wasm interface.

Currently the app's `Model` is stored in JS but the `update` function is in Wasm. That means the model has to get passed from JS to Wasm and back again on every update cycle, getting serialised and deserialised along the way. It's very inefficient, but it was quicker to get up and running this way, because I didn't need to change anything in the JS runtime. It would be better to keep the model in Wasm.

The other issue is dealing with unserialisable JavaScript objects. For example, click events are not serialisable because they contain cyclical references. It's all to do with how the Json library is implemented. I have something that works _most_ of the time! But again, it crosses the JS/Wasm boundary too often. That means it's slow, and when I get errors, they're hard to debug. I'm working on a cleaner solution.


## String encoding

- Original post mentions UTF-8
- Discussed in Discourse previously
- I did some benchmarking and UTF-8 is very fast. Smaller size really matters.
- However in the end I went with UTF-16 for backward compatibility reasons. I wanted to be able to run my compiler in existing apps and have String.length work as expected.

## Compiler challenges

I decided to restrict myself to only touching the code-generation part of the compiler, and leaving everything else alone as much as possible. There are only a few minor changes to the command-line interface so that you can specify whether you want to generate JS or C output files.

The main drawback is that the code generator does not have access to Elm type information. All the type annotations in the AST are dropped at an earlier stage in the compiler pipeline.

### Ambiguity between Int and Float at JS/Wasm interface

When a message from the Elm runtime to the Wasm module contains a number, the wrapper needs to convert it from a JS `number` to the byte-level representation of either an Int or a Float. To choose the right one, the code generator would need to know which app Msg constructor parameters are Int and which are Float. But it doesn't.

The current workaround is to guess that a round number is always an Int! But that is quite unreliable, and a mistake could result in programs silently processing corrupted data.

However, apps that never use a Float in any Msg constructor are safe. Neither the TODO MVC example nor the Elm SPA Example use Floats in messages, so they cannot suffer from this issue.

### Ambiguity de-structuring Cons, Tuple and Custom

The AST used in the code generator makes no distinction between accessing the head of a list, and accessing the first parameter of a Tuple or Custom type. This is OK in JavaScript because the same object shape is used for all three.

But in my Wasm implementation, Cons and Tuple use smaller, more optimised structures than Custom types. So I can't use the same code to access the data inside them. Instead I generate code that detects the type at runtime, and then accesses it in the appropriate way for the detected type. This is slower than it needs to be.

The other possible workaround would be to use the same memory layout for all of these structures. But that would make Cons and Tuple2 33% bigger, wasting space. This would impact performance by reducing the amount of useful data that can fit in CPU cache.

Having type information in the AST would allow me to generate the most optimal code.

# Time module

The WebAssembly code uses 32-bit values for the Elm Int type. It could use 64-bit integers but that would prevent using a fairly basic optimisation called "unboxing" in the future.

Elm's Json.Decode.int function only allows integers in the 32-bit range. The Bitwise module uses 32-bit integers. But the Time module uses Int values that are around 42 bits.

## Build steps

If you want to run this compiler on your own app, you can follow the instructions in the README

- Install Haskell
  - I use GHC v8.0.2 with Stack but I'm sure there are other ways
- Install Emscripten
- Clone my two repos
- Copy the TODO MVC demo
- Replace the src/ folder with your own code
- Run `make` in a Bash shell
