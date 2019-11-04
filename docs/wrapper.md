# The WebAssembly/JavaScript interface

The aim of this project is to gradually implement the Elm language in WebAssembly. But currently Wasm is still in MVP stage and does not have direct access to browser APIs like the DOM, `XmlHttpRequest`, `Date`, and so on. (A few WebAssembly proposals need to get implemented first, such as [GC](https://github.com/WebAssembly/gc/blob/master/proposals/gc/Overview.md#host-types) and [type imports](https://github.com/WebAssembly/proposal-type-imports/blob/master/proposals/type-imports/Overview.md).)

This means that for now, some of Elm's Kernel code needs to remain in JavaScript. The rest can be implemented in WebAssembly, but that means we have two different compile targets that need to work together! We need to have a wrapper around the WebAssembly module to transparently connect it to the JS runtime.

Essentially we have the Elm app in WebAssembly and the effectful "Elm runtime" in JavaScript. The only Kernel code on the WebAssembly side is what's required to implement the Elm data types: integers, floats, characters, strings, lists, tuples, records, custom types, and functions.

![Diagram. Effectful kernel code on the left. Pure functional WebAssembly module on the right with a wrapper around it to interface to JS. JS calls Wasm for init, update, subscriptions, view. Wasm calls JS to get effects.](./images/JS-Wasm-wrapper.png)

The "Effects runtime" is the part of `elm/core` that calls `init`, `update`, `subscriptions`, and `view`. It also manages and schedules the effects that you request by returning `Cmd`, `Sub`, or `VirtualDom.Node` values. The main files in this box are `Scheduler.js`, `Platform.js`, and `Process.js`. They don't use Web APIs themselves, so this code could be implemented in WebAssembly _or_ JavaScript. But most of the code is interacting with "Effectful kernel modules" that generally _do_ have to be implemented in JS.  So it makes sense for the runtime to be in JS as well.



## The Wrapper

The "wrapper" shown in the architecture diagram above translates between Wasm and JS data structures, and allows JS and Wasm functions to call each other.

The web platform provides _low level_ tools for JS/Wasm interaction. Functions crossing the language boundary can only operate on integers and floats. There's no built-in support for strings or objects. We need to build abstractions on top of this low level API to make the JS/Wasm boundary "invisible" to the Elm program.

The Elm code representing the wrapper is very simple, just passing values through to `Browser.element`. But the "compiler" knows about this module and treats it specially. 

```elm
module WasmWrapper exposing (element)
import Browser
import Html exposing (Html)

element :
  { init : flags -> (model, Cmd msg)
  , view : model -> Html msg
  , update : msg -> model -> ( model, Cmd msg )
  , subscriptions : model -> Sub msg
  }
  -> Program flags model msg
element mainRecord =
    Browser.element mainRecord
```

Actually for now it's not the compiler that does anything special, it's just a [build script](../demos/wrapper/build-combine.sh) that modifies the standard Elm compiler's JS output to change the definition of `WasmWrapper.element`.

The real implementation of `WasmWrapper.element` is a JS function that reads `mainRecord` from the WebAssembly module, converts it to the equivalent JavaScript object, and passes that on to `Browser.element`. As far as `Browser.element` is concerned, its argument looks exactly as expected.

The JS representation of `mainRecord` is an object containing 4 functions. So we're _reading_ functions out of WebAssembly as bytes! Cool! In the next couple of sections, we'll take a closer look at how that works.

> **Notes**
>
> - I thought about using `elm/bytes` instead of encoding and decoding in JS, but it's not ideally suited to this situation. We need to send _functions_ back and forth as well as data. And there are some low-level interactions with the Wasm module itself that need to be written in JavaScript.
>- The source for the wrapper is in TypeScript. I'm very familiar with it and it helps avoid bugs. But I've made sure not to make it a hard dependency. The TypeScript code could be abandoned and replaced with the (very readable) JavaScript version.



## Elm functions in WebAssembly

To understand how we pass functions across the JS/Wasm boundary, we need to take a look at how Elm functions are implemented on the Wasm side.

The C language doesn't allow you to pass functions around as values, nor to "partially apply" them. But it does allow _function pointers_ to be passed around as values. We represent an Elm function as a data structure called `Closure` that contains a pointer to a C function and pointers to any partially-applied arguments. This structure is used by the "function application" operator, which implements features like partial application, higher-order functions, and so on.

Let's look at the Wasm representation of the partially applied Elm function `(+) 5 : Int -> Int`. This function adds 5 to any integer, and its representation is shown in the diagram below.

The header indicates that it's a `Closure` with a `size` of 4 words (a "word" being 32 bits). It has one applied value (`n_values=1`) and expects 2 values to be applied in total (`max_values=2`). The `evaluator` field points to the C function `Basics_add_eval`, which will be called when the last argument is applied. The `values[0]` field points to the partially applied argument `literal_int_5`.

![Diagram of the Wasm data structures for Closure and Int](./images/closure-example.png)

<u>Note</u>: Elm `Int` is implemented as a data structure with a header, because it helps with "constrained type variables" like `number` and `comparable`. There are more efficient ways integers could be represented in the future, but that requires significant compiler changes.

For further reading you can check out the documentation on [Elm data structures in WebAssembly](./data-structures.md), my [blog post on Closures](https://dev.to/briancarroll/elm-functions-in-webassembly-50ak), the source for the function application operator [`Utils_apply`](../src/kernel/utils.c), and the [C structures for all the Elm types](../src/kernel/types.h).



## Calls from JS runtime to Wasm app

The main calls from the JS runtime to the Wasm app are `init`, `update`, `subscriptions`, `view`. But there are also `Msg` constructors - functions that wrap the result of a `Cmd` just before the runtime calls `update`. (They're often constructors from a `type Msg`  declaration, but can be more complex).

The Wrapper reads these functions from the Wasm memory as `Closure`s, and creates JS functions from them.

The JS function contains a reference to the information that was found in the `Closure`, such as the Wasm `evaluator` function pointer and any partially-applied arguments.

When the JS function is called with some arguments, it encodes them to bytes, writes them to the Wasm memory, and creates a new `Closure`, like the original but now containing all its arguments.

Once we have a full `Closure` in Wasm memory, we are ready to evaluate it. The Wasm module has an exported function called `evalClosure`, visible in JS. The JS wrapper calls it, passing the address of the `Closure` it has just written (which is just an integer).

Next, the Wasm module evaluates the `Closure`, calling the underlying C function with the arguments supplied. It writes the resulting Elm value to memory and returns the address of the result. Again this address is just an integer so it can be returned from `evalClosure` back to the JS wrapper.

Now the JS wrapper has received the address of the result, it can read the bytes and decode them to a JS representation of an Elm value.

If this was an `update` call then the result would be a tuple of a model and a command. If it was a `view` call then the result is a tree of virtual DOM nodes. In all cases it's a JS value that looks exactly as the runtime would expect in any other Elm program!



## Calls from Wasm app to JS runtime

The only calls that are made from the Wasm app to the JS runtime are Kernel calls that return descriptions of effects, such as `Cmd`, `Task`, or `VirtualDom.Node`. But there's no Wasm representation of these values because we decided not to implement this stuff in Wasm!

Instead we represent all effect values as "thunks": `Closure`s that never actually get evaluated in WebAssembly. A thunk represents a value that will _eventually_ be evaluated, but not until it gets decoded by the wrapper.

They are only evaluated by the JavaScript wrapper when it's converting a return value from `update`, `view` and friends. If you've ever used Haskell, you'll be familiar with the concept of thunks.



```markdown
need a concrete example
ideally combining effects in lists or some shit (maybe don't need?)
VirtualDom is the obvious choice but don't want to draw too much attention to that

Fuck it, do vdom and explain shit
fuck it's a bit of a cunt though

```



compile time

- bake in JS closures

runtime

- update calls some effect thing
- it gets treated as a partial call even if all arguments are actually present
- 









When the Elm app (in Wasm) makes a call to a Kernel function like `VirtualDom.text "hello"`, it gets back a `Closure` that represents 'whatever value results from calling the JS function `_VirtualDom_text` with argument "hello"'. Since the `VirtualDom.Node` type is opaque in Elm anyway, no Elm function can ever know the difference. These thunks can be nested to any depth. When the `view` function eventually returns to the runtime, the wrapper will want to convert its return value to JS. When it notices that the return value is an unevaluated Kernel call, it will actually *evaluate* the call and get the return value of the JavaScript kernel function.

So in summary, we are doing effects in JS and not Wasm, so we'd prefer not to have to write Wasm representations of effect values. Therefore we _defer_ the actual call to the Kernel function until we're passing through the wrapper. Inside the Wasm Elm app, it just looks like a `Closure`, which is something the Wasm app knows how to handle.

In the implementation, all Kernel `Closure`s are given an "almost infinite" arity (`max_values=0xffff`) to ensure they're always considered to be "partially applied", which means the Wasm code will never evaluate them.

Wasm can only refer to things using numbers, so the wrapper stores a JS array of Kernel functions and Wasm uses its index in that array. The array index is stored in the `evaluator` field of the `Closure`, where we would otherwise store the address of a C function (but JS kernel calls never get evaluated in C).



## Known issues

### Int vs Float ambiguity

If the Elm runtime passes a JavaScript `number` to the app through the WebAssembly wrapper, the wrapper can't accurately detect whether it's supposed to be an `Int` or a `Float` in Elm. JavaScript doesn't make any distinction between the two. Currently I don't have a reliable solution for this!

For the moment, I'm "making do" with an unsafe temporary workaround. The wrapper checks for _whole_ numbers and assumes they should be written to Wasm as `Int`. But this does the wrong thing for `Float` values that happen to be round numbers!

I can see two possible solutions

1. Get type info from the compiler to help with encoding
   - This is difficult! The Elm compiler doesn't provide much type information to the back-end that generates the JavaScript output code. For example if the runtime wanted to send the app a `Msg` containing a number, and we knew the particular `Msg` constructor function accepts `Float`, we can use that info to encode correctly. Unfortunately the code generator doesn't have this information. (It's not crucial for generating JS, the only currently-supported compile target.)
   - It might be possible to trick the compiler into revealing this kind of information by `exposing` the message constructor functions. Then their types might be written to an `.elmi` file. It's not a great solution though.
2. Use some Elm code to help with encoding
   - I haven't thought this through fully. But it should be possible for the app to provide `elm/bytes` encoders for its `Msg` types that would make it easier to know where the `Int` and `Float` values are. Obviously this is a workaround for compiler limitations, but it might unlock progress while those limitations are still there.

### Tuple vs Record ambiguity

In `--optimize` mode, the generated JS for `( 123, "hello" )` is identical to the JS for `{ a = 123, b = "hello" }`

This causes an ambiguity similar to the `Int`/`Float` ambiguity described above. The solutions are similar. We need type info from either the compiler or the app code. In unoptimized mode, Tuple has an extra property that allows us to distinguish it.



## Rejected Idea: "working set" for JS kernel values

Before I thought of treating Kernel calls as thunks, I had another plan. It didn't work, but I thought I'd share in case it saves someone else pain in the future!

Functions like `update` and `view` need some way to work with temporary Kernel values. Lower-level `update` functions could create ten `Cmd` values, pass them around the program before finally returning just one from the top-level `update`. In the case of `view`, we need to create a whole tree of JS values intermingled with Wasm `List`s and `String`s. Some parts of the view could be created only to be thrown away later by an `if` expression.

The original idea for dealing with this was to immediately call out to JS to get the effect value but put it in a "working set" array in JS. Then I'd send the index of that JS value to Wasm, wrapped in some C structure that could be called `JsRef` or something. Wasm functions could work with the `JsRef` values.

The problem with this idea is memory management. It's very hard to know when it's OK to _delete_ something from this "working set" array of JS objects. We need to know that no Wasm value is still referring to it. The program could have put that `JsRef` into the Model, or into a `Closure` that'll get passed to the runtime and back again later. Dealing with this would require an extension to the GC to manage the "working set" array as well as the Wasm memory.

Oh dear. This is all possible, but it's going to be a _nightmare_ to debug!

But wait, what if JS values were represented as _thunks_ on the Wasm side?

Now Wasm only needs references to _permanent_ functions on the JS side. It doesn't handle any dynamically created JS values, so there's no memory management issue. The JS kernel values are only "materialised" as they pass through the JS wrapper on their way to the runtime.

Once we've send a value from Wasm to JS, it gets materialised on the JS side and managed by the JavaScript GC, so Wasm can forget about it. It's garbage. If JS ever needs to send it back, it will just write a new copy.

This completely decouples the memory management on either side of the JS/Wasm boundary, which is great!