# The WebAssembly/JavaScript interface

The aim of this project is to gradually implement the Elm language in WebAssembly. But currently Wasm is still in MVP stage and does not have direct access to browser APIs like the DOM, `XmlHttpRequest`, `Date`, and so on. (A few WebAssembly proposals need to get implemented first, such as [GC](https://github.com/WebAssembly/gc/blob/master/proposals/gc/Overview.md#host-types) and [type imports](https://github.com/WebAssembly/proposal-type-imports/blob/master/proposals/type-imports/Overview.md).)

This means that for now, some of Elm's Kernel code needs to remain in JavaScript. The rest can be implemented in WebAssembly, but that means we have two different compile targets that need to work together! That's something new in Elm and it presents a few challenges that we need to overcome.

Let's do some analysis on this in the context of Elm, draw some conclusions, and discuss the solution implemented in this project.



## Where to draw the line between JS and Wasm?

To analyse this question, it's useful to divide Elm code into several categories.

### Application code

Well, this is the easy one. It's all in Elm. It can't directly call Web APIs, because those are *effects* and only the Elm runtime can do effects. Application code can only _specify_ which effects should be run. So it's fine to implement all of the app code in WebAssembly. In fact, if we didn't, that would kind of be the end of this project!

Decision: This category of code belongs in **WebAssembly**.

### Elm code in the standard libraries

Lots of the code in `elm/core` and other standard libraries is written in pure Elm. This includes modules such as `Tuple`, `Maybe`, `Result`, `Dict`, `Set`, and `Array`. These can just be compiled like any other Elm file, along with user application code. So they'll end up in WebAssembly too.

> <u>Note</u>: For now, all Elm code is "hand-compiled" to C. But eventually my [fork of the Elm compiler](https://github.com/brian-carroll/elm-compiler/tree/c) should be able to do it! 🦄 Hand compiling small programs makes sense in this prototyping stage.

Decision: This category of code belongs in **WebAssembly**.



### Kernel code that calls Web APIs

This is the stuff that definitely has to be in JavaScript due to limitations of the WebAssembly MVP. It includes kernel code from `elm/virtual-dom` and almost any package that can give you a `Cmd` or `Task` value, like `elm/http`, `elm/time`, and so on.

> In some cases there is a possiblity of moving *parts* of kernel files into WebAssembly. VirtualDom creation and diffing, for example. Only the "patching" part of VirtualDom *needs* to be in JS. So this categorisation doesn't necessarily follow the same boundaries as the existing files. But for the most part, it does.

Decision: This category of code belongs in **JavaScript**.



### Kernel code for the effects runtime

This category is often what people mean when they talk about "the Elm runtime". It's the part of `elm/core` that manages and schedules the effects that you request from your program. It's responsible for making calls to your `init`, `update`,  `subscriptions`, and `view`. When those functions return a `Cmd`, `Sub`, or VirtualDom tree, this is the code that schedules and dispatches the effects, and calls you back with any resulting messages.

The main files involved are `Scheduler.js`, `Platform.js`, and `Process.js`. They don't use Web APIs themselves, so this code could be implemented in WebAssembly _or_ JavaScript. The choice is not quite as obvious as for the other categories mentioned here!

I've spent quite a bit of time and effort investigating this (because it's interesting!) But I concluded that porting this code to WebAssembly is a huge amount of effort for not much benefit. The thing is, the effects runtime is intimately connected with the "effect manager" kernel code, most of which _does_ have to be implemented in JS. So if we built WebAssembly implementations of `Scheduler.js` and friends, they would be doing an _awful lot_ of back-and-forth across the language boundary. So I think it's best to just leave it in JavaScript too. (Besides, this code is pretty difficult stuff that's easy to get wrong!)

Decision: This category of code belongs in **JavaScript**.

### Kernel code for low-level data structures

There's lots of Kernel JavaScript code in `elm/core` that is not for effects at all, but for low-level data structures and operations. This includes JavaScript files in `elm/core` such as `Basics.js`, `List.js`, `Tuple.js`, `Char.js`, `String.js`, `Bitwise.js`, and `Utils.js`.

Many of the lowest-level operations in Elm need to be written directly in the _target language_ so that everything else can be built on top of them. However for us, the main target is WebAssembly, not JavaScript. So these low-level operations should be written in C.

For example, when the WebAssembly module wants to create a `List`, it shouldn't have to call out to JavaScript! Instead we just have C implementations of things like the _cons_ operator `::`. You can see parts I've written so far [here](../src/kernel).

However it's worth noting that the effects runtime and the effect managers do use `List` and `Tuple` and so on. So we need it in JavaScript too! We end up with two copies of the same basic functionality. There's not much we can do about this, it comes from the fact that we have two target languages.

Decision: This category of code belongs in **WebAssembly *and* JavaScript**!



## Architecture

Based on the analysis above, here's what the system ends up looking like:

![](C:\Users\brian\Code\wasm\c\codelite\docs\JS-Wasm-wrapper.png)

So the split is *roughly* "pure code on one side and effectful code on the other". That's nice because Elm already makes a sharp distinction between those two things! 

> The key thing that makes this work out neatly is the choice to keep the "effects runtime" in JavaScript.





Need to give people a more <u>concrete</u> picture of what's going on

- I'm using TypeScript just as a way to write less-buggy JS. Not heavily dependent on it. I've made sure the generated JS is basically the same thing with type annotations stripped out.
- for some types, we need hints from the compiler to be able to convert between the representations
- wrapper reading and writing bytes and stuff.
  - writing
    - detecting Elm type of JS things
    - if it's a tree, write children first
    - encode into bytes
    - handle heap overflows and retries
- main record
- WasmWrapper.elm
- build scripts





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
