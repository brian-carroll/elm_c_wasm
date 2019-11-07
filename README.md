# Elm &rarr; C &rarr; WebAssembly

This repo is part of a project to compile Elm to WebAssembly, using C as an intermediate language.

It implements parts of Elm's [core libraries](https://package.elm-lang.org/packages/elm/core/latest/) needed for any compiled code to work, including:

- Byte level implementations of all of Elm's data types
- First-class functions (currying, higher-order functions, etc.)
- The basic arithmetic operators (`+`,`-`,`*`,`/`)
- Record updates and accessors
- A working garbage collector specially designed for Elm

I also have a [fork of the Elm compiler](https://github.com/brian-carroll/elm-compiler/tree/c) where I'm working on getting it to generate C instead of JavaScript. It's at a very early stage.

&nbsp;

# Current Status

Here's roughly how I see the project progressing from here, as of October 2019. (Unless some big unknown bites me, which it might!)

- [x] Implement all [Elm value types in C](./docs/data-structures.md) and prove they work using [unit tests][core-unit-tests].
- [x] Do some initial exploration of C code generation in a [fork of the Elm compiler](https://github.com/brian-carroll/elm-compiler/tree/c). Understand how it works and what's involved.
- [x] Implement a [Garbage Collector](./docs/gc.md) in C and compile it to Wasm. Prove it works using [in-browser unit tests][gc-unit-tests] and an [elm-benchmark app][benchmark].
- [x] Create a wrapper to connect a WebAssembly module to Elm's kernel JavaScript.

  - Figure out what memory management issues exist related to crossing the language boundary. (What if Wasm's memory overflows when JS writes to it? Does Wasm need to refer to JS objects, and if so, how? How to deal with `onClick` handlers? etc.)
  - Get a "hello world" app working with kernel entirely in JS and pure Elm entirely in Wasm. Very minimal Elm code, hand-compiled to C.
- [ ] Finish code generation for the full Elm AST (outputting both C and JS)
	- Test on an example app. Perhaps Richard Feldman's [Elm SPA example](https://github.com/rtfeldman/elm-spa-example), probably something smaller first
- [ ] Look into migrating VirtualDom diffing to Wasm, with JS just applying patches.
- [ ] Look into further GC optimisations for VirtualDom as [suggested on Discourse](https://discourse.elm-lang.org/t/elm-core-libs-in-webassembly/4443)

My general approach is to explore all the different pieces, then work on the biggest unknown until I understand the important issues and/or solved some problem. Once I feel like I understand that well, move on to the next biggest unknown.

This means gradual progress on lots of pieces, with no end-to-end system for quite a long time. But I feel that focusing on the biggest unknowns first will lead to a good design and avoid going down dead ends. Not everyone likes this style but it works for me!

[core-unit-tests]: https://brian-carroll.github.io/elm_c_wasm/unit-tests/index.html?argv=--types+--utils+--basics+--string+--verbose
[gc-unit-tests]: https://brian-carroll.github.io/elm_c_wasm/unit-tests/index.html?argv=--gc+--verbose
[benchmark]: https://brian-carroll.github.io/elm_c_wasm/benchmark/index.html

&nbsp;

# Documentation

- [Demos](#demos)
- [The JavaScript/WebAssembly interface](#the-javascriptwebassembly-interface)
- [C as an intermediate language](#c-as-an-intermediate-language)
- [Elm data structures](./docs/data-structures.md)
- [String encoding](./docs/string-encoding.md)
- [Garbage Collector](./docs/gc.md)

&nbsp;

# Demos

I have built a few demos here: https://brian-carroll.github.io/elm_c_wasm/

At this early stage, they're very primitive demos! Definitely not full Elm programs in Wasm!

- [Unit tests for core libraries][core-unit-tests]

  - You can see byte-level Elm data structures, arithmetic operations, record updates and accessors, and first-class functions in action.

- [Unit tests for the GC][gc-unit-tests]

  - You can see print-outs of the heap, with byte-level Elm data structures, GC mark bits, etc.

- [Performance micro-benchmarks][benchmark]

  - Reported figures for the Wasm implementations are slower than JS. It's partly due to crossing back and forth across the JS/Wasm boundary on every iteration. Browsers can't optimise that very well yet, though Firefox seems much better at it than Chrome.

  - Despite the limitations, the benchmark is a good way to stress-test the implementation, flush out bugs in the GC, and to find out where the bottlenecks are.

&nbsp;

# The JavaScript/WebAssembly interface

Currently Wasm does not have direct support for any Web APIs such as DOM, XmlHttpRequest, etc. You have to call out from WebAssembly to JavaScript to use them. That means that effectful modules like `VirtualDom`, `Browser`, and `Http`, cannot be fully ported to WebAssembly yet. A few effects could be ported, but not enough to justify the effort of porting lots of complex kernel code to C (like [Scheduler.js](https://github.com/elm/core/blob/1.0.2/src/Elm/Kernel/Scheduler.js) and [Platform.js](https://github.com/elm/core/blob/1.0.2/src/Elm/Kernel/Platform.js)), so I think it's better to leave that for later.

In the meantime, I think a good goal is to compile the pure part of the Elm program to WebAssembly and leave the effectful Kernel code in JavaScript. There would be a "wrapper" `Program` type that acts as an interface between the Elm runtime and the WebAssembly module.

There are two ways for JavaScript and WebAssembly to talk to each other

- **Function calls**: The WebAssembly module can _import_ JavaScript functions and call them, and _export_ some of its own functions to make them callable from JavaScript. The main limitation is that the arguments can only be numbers, not data structures.
- **Data**: For data structures, you need to read and write to the WebAssembly module's memory, which appears in JavaScript as an `ArrayBuffer`. JavaScript can write bytes into the memory and then call an exported function when the data is ready (perhaps passing the offset and size of the data written). More details [here](https://developer.mozilla.org/en-US/docs/WebAssembly/Using_the_JavaScript_API#Memory).

The wrapper `Program` would convert the `msg` and `model` to a byte-level representation, write them to the WebAssembly module's memory, and then call an `update` function exposed by the compiled WebAssembly module.

The `update` will return a new `model` and `Cmd`, which will be decoded from bytes to the JavaScript structures that the Elm runtime kernel code uses.

`VirtualDom` is a unique effect in Elm in that it has no `Cmd`. I think the best approach will be to implement the diff algorithm in pure Elm and use ports to apply the patches, as Jakub Hampl [demonstrated](https://gist.github.com/gampleman/cbf0434b22e1da0e3193736b87e040f5) in Elm 0.18 a couple of years back.

&nbsp;

# C as an intermediate language

&nbsp;

## Why use an intermediate language?

It would be possible to compile Elm code directly to WebAssembly. In fact that's what I did in [my first attempt](https://github.com/brian-carroll/elm-compiler/tree/wasm). But there are several drawbacks.

Debugging the output of the compiler is **not fun** if it's WebAssembly. It's a lot easier to spot bugs in generated C code than in generated WebAssembly.

Writing Kernel code in WebAssembly is even worse. I even tried building a little DSL to generate Wasm kernel code from Haskell, but it was practically impossible to debug. I gave up on it. In theory you could compile Elm to WebAssembly but have kernel code in C, but this makes everything very hard.

Having both the compiled code and kernel in C makes everything vastly more do-able. I don't think this project would actually go anywhere if it was direct-to-Wasm.

As for performance, you might expect things to be faster if you skip the C "layer" but I think at this early stage it's more likely the other way around!

C compilers have had a lot of work put into them by hundreds of experts for over 50 years. They have a lot of advanced low-level optimisations built-in. I would have to figure out how to _manually_ do a lot of the basic optimisations before even getting started on Elm-specific ones. And I'm just one person doing a hobby project to punch in some time in the evenings.

In summary I think that using an intermediate language is a massive productivity gain, and that any downsides are more theoretical than practical.

&nbsp;

## Why C and not Rust?

Rust has great WebAssembly support and I tried to use it early on. But it didn't work out well for me.

By the time the Elm compiler actually gets to the code generation phase, the program has already been validated by Elm's type checker. So I <u>know</u> it's OK but now I have to convince Rust that it's OK, and I found this was very difficult.

Rust is very good for _hand-written_ code that does _manual memory management_, but not so much for auto-generated code running with a garbage collector. I found it was throwing errors about a lot of things that were actually _good_ ideas in that context.

Also, a language implementation, particularly one with a GC, is going to involve a lot of `unsafe` Rust. That's quite advanced Rust programming. It would have been too hard for me to know when to go against the normal rules and when not to, on my very first Rust project. Someone who was proficient in Rust may have made a different decision.

For all these reasons, I _very reluctantly_ realised that I had exhausted the alternatives and C was the only tool for this job. It's my first C project in about 10 years.

&nbsp;

# Further reading

- [Elm data structures](./docs/data-structures.md)
- [String encoding](./docs/string-encoding.md)
- [Garbage Collector](./docs/gc.md)
