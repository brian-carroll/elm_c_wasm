# The WebAssembly/JavaScript interface



The aim of this project is to gradually implement the Elm language in WebAssembly. But currently Wasm is still in MVP stage and does not have direct access to browser APIs like the DOM, `XmlHttpRequest`, `Date`, and so on. This means that for now, Elm's effectful Kernel code needs to remain in JavaScript

Normally an Elm app compiles to just one language - JavaScript. But now we have two different compile targets that need to work together - JavaScript and WebAssembly. This document outlines how that works.



## Where to draw the line between JS and Wasm?

OK, so we know we need to keep _some_ things in JavaScript. But which things exactly?

To analyse this question, it's useful to divide Elm's standard library code into several categories.



### JavaScript calling Web APIs

Many of the effectful moduels in the standard libraries interact directly with Web APIs and therefore cannot be implemented in WebAssembly yet.

This includes packages like `elm/browser`, `elm/http` and `elm/virtual-dom`. They need to stay in JavaScript.

### JavaScript for managing effects

Some Kernel modules from `elm/core` do not use Web APIs directly but are heavily involved in scheduling and managing effects. In particular, `Scheduler.js` `Platform.js`, and `Process.js`.

This _could_ be implemented in WebAssembly. But it is intimately connected with the modules in the previously that _must_ be in JavaScript. So it's much more natural and clean to just leave it in JavaScript too.

I think that choice will remain valid until WebAssembly gets Web API support.

### JavaScript for pure low-level operations

There's lots of Kernel JavaScript code in `elm/core` that is not for effects, but for low-level operations.

This includes JavaScript files in `elm/core` such as `Basics.js`, `List.js`, `Tuple.js`, `Char.js`, `String.js`, `Bitwise.js`, and `Utils.js`.

Many of the lowest-level operations in Elm need to be written directly in the _target language_ so that everything else can be built on top of them.

However for us, the main target is WebAssembly, not JavaScript. So these low-level operations should be written in C. You can see parts I've written so far [here](../src/kernel).

For example, when the WebAssembly module wants to create a `List`, it shouldn't have to call out to JavaScript! Instead we just have a C implementation of the _cons_ operator `::`.

### Elm code in the standard libraries

Lots of the code in `elm/core` and other standard libraries are written in pure Elm. This includes modules such as `Tuple`, `Maybe`, `Result`, `Dict`, `Set`, and `Array`.

These can just be compiled like any other Elm file, along with user application code. So they'll end up in the WebAssembly module, not in JavaScript.

For now, in this project, all Elm code is "hand-compiled" to C. But eventually my [fork of the Elm compiler](https://github.com/brian-carroll/elm-compiler/tree/c) should be able to do it! 🦄

## 

## Architecture

![](C:\Users\brian\Code\wasm\c\codelite\docs\JS-Wasm-wrapper.png)





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
