# Elm &rarr; C &rarr; WebAssembly

This repo is part of a project to compile Elm to WebAssembly, using C as an intermediate language.

The project consists of two repos:

- This repo implements parts of Elm's [core libraries](https://package.elm-lang.org/packages/elm/core/latest/) needed for any compiled code to work

  - Byte level implementations of Elm data structures and operations like function application (with currying etc.), record updates and accessors, and arithmetic operators. More info [here](./docs/data-structures.md).
  - A working garbage collector designed for Elm. More info [here](./docs/gc.md).

- I also have a [fork of the Elm compiler](https://github.com/brian-carroll/elm-compiler/tree/c) where I'm working on getting it to generate C instead of JavaScript. It's at a very early stage.

&nbsp;

# Demos

I have built a few demos here: https://brian-carroll.github.io/elm_c_wasm/

At this early stage, they're very primitive demos! Definitely not full Elm programs in Wasm!

- Unit tests for the kernel code are [here](https://brian-carroll.github.io/elm_c_wasm/unit-tests/index.html?argv=--types+--utils+--basics+--string+--verbose)
- You can see byte-level Elm data structures, arithmetic operations, record updates and accessors, and first-class functions in action.
- Unit tests for the garbage collector are [here](https://brian-carroll.github.io/elm_c_wasm/unit-tests/index.html?argv=--gc+--verbose)
- You can see print-outs of the heap, with byte-level Elm data structures, GC mark bits, etc.
- Some performance micro-benchmarks are [here](https://brian-carroll.github.io/elm_c_wasm/benchmark/index.html)
- Reported figures for the Wasm implementations are slower than the JS implementations.
  - This is partly to do with crossing back and forth across the JS/Wasm boundary _a lot_. The benchmarking loop is in JS even when functions under test are in Wasm. Browsers can't optimise this as well as pure JS yet, and it's exaggerated by testing tiny functions.
  - It's also partly to do with the way Elm implements currying using the `A2` helper. This is something that could be changed in future.
- Nevertheless, this is a good way to stress-test the implementation and flush out bugs in the GC, and to find out where the bottlenecks are (such as those described above).

&nbsp;

# System design

Currently Wasm does not have direct support for any Web APIs such as DOM, XmlHttpRequest, etc. You have to call out from WebAssembly to JavaScript to use them. That means that effectful modules like `VirtualDom`, `Browser`, and `Http`, cannot be fully ported to WebAssembly yet. A few effects could be ported, but not enough to justify the effort of porting lots of complex kernel code to C (like [Scheduler.js](https://github.com/elm/core/blob/1.0.2/src/Elm/Kernel/Scheduler.js) and [Platform.js](https://github.com/elm/core/blob/1.0.2/src/Elm/Kernel/Platform.js)), so I think it's better to leave that for later.

In the meantime, I think a good goal is to compile the pure part of the Elm program to WebAssembly and leave the effectful Kernel code in JavaScript. The way I envision it working is that there would be a "wrapper" `Program` type that acts as an interface between the Elm runtime and the WebAssembly module.

There are two ways for JavaScript and WebAssembly to talk to each other

- **Function calls**: The WebAssembly module can _import_ JavaScript functions and call them, and _export_ some of its own functions to make them callable from JavaScript. The main limitation is that the arguments can only be numbers, not data structures.
- **Data**: For data structures, you need to read and write to the WebAssembly module's memory, which appears in JavaScript as an `ArrayBuffer`. JavaScript can write bytes into the memory and then call an exported function when the data is ready (perhaps passing the offset and size of the data written). More details [here](https://developer.mozilla.org/en-US/docs/WebAssembly/Using_the_JavaScript_API#Memory).

The wrapper `Program` would convert the `msg` and `model` to a byte-level representation, write them to the WebAssembly module's memory, and then call an `update` function exposed by the compiled WebAssembly module.

The `update` will return a new `model` and `Cmd`, which will be decoded from bytes to the JavaScript structures that the Elm runtime kernel code uses.

`VirtualDom` effects could be trickier. It might be worth implementing the diff algorithm in pure Elm and using ports to apply the patches, as _gampleman_ demonstrated [here](https://gist.github.com/gampleman/cbf0434b22e1da0e3193736b87e040f5) in the Elm 0.18 days.

&nbsp;

## FAQ

&nbsp;

#### Why use an intermediate language?

It would certainly be possible to compile Elm code directly to WebAssembly. In fact that's what I did in [my first attempt](https://github.com/brian-carroll/elm-compiler/tree/wasm). There are a few drawbacks to this:

1. Writing Kernel code in WebAssembly is **not fun**.
   - Initially I thought I could build up a little Haskell DSL to abstract over raw WebAssembly, since I already had that kind of machinery in my fork of the compiler. But I gave up after just trying to implement one kernel function. I found it completely impractical to debug. I'm no fan of C but it's far more readable than WebAssembly!
2. Debugging the output of the compiler is also **not fun**
   - It's a lot easier to spot bugs in C code than in WebAssembly code
3. Compilers for C (and C++ and Rust) are _very good_ at producing high-quality optimised assembly code. They've been optimising it for decades. I probably can't beat that performance sitting on my couch in my pyjamas. Might as well use what's out there.

&nbsp;

#### Why C and not Rust?

Rust has great WebAssembly support and I tried to use it initially but

- By the time the Elm compiler actually gets to the code generation phase, the program has already been validated by Elm's type checker. So I <u>know</u> it's OK but now I have to convince Rust that it's OK, and I found this was very difficult.
- Rust is very good for _hand-written_ code that does _manual memory management_, but not so much for auto-generated code running with a garbage collector. I found it was throwing errors about a lot of things that were actually _good_ ideas in that context.
- A language implementation, particularly one with a GC, is going to involve a lot of `unsafe` Rust. That's quite advanced Rust programming. It would have been too hard for me to know when to go against the normal rules and when not to, on my very first Rust project.

&nbsp;

#### What kind of string encoding are you using?

Umm none yet! I haven't really gotten that far!
But I have put some thought into it. See [here](./docs/string-encoding.md).
