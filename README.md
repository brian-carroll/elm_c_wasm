# Elm &rarr; C &rarr; WebAssembly

This repo is part of a project to compile Elm to WebAssembly, using C as an intermediate language.

It implements parts of Elm's [core libraries](https://package.elm-lang.org/packages/elm/core/latest/) needed for any compiled code to work, including:

- Byte level implementations of Elm data structures
- Function application with currying etc., arithmetic operators (`+`,`-`,`*`,`/`), record updates and accessors.
- A working garbage collector designed for Elm.

I also have a [fork of the Elm compiler](https://github.com/brian-carroll/elm-compiler/tree/c) where I'm working on getting it to generate C instead of JavaScript. It's at a very early stage.

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

- [Unit tests for core libraries](https://brian-carroll.github.io/elm_c_wasm/unit-tests/index.html?argv=--types+--utils+--basics+--string+--verbose)

  - You can see byte-level Elm data structures, arithmetic operations, record updates and accessors, and first-class functions in action.

- [Unit tests for the GC](https://brian-carroll.github.io/elm_c_wasm/unit-tests/index.html?argv=--gc+--verbose)

  - You can see print-outs of the heap, with byte-level Elm data structures, GC mark bits, etc.

- [Performance micro-benchmarks](https://brian-carroll.github.io/elm_c_wasm/benchmark/index.html)

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
