

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

The only calls that are made from the Wasm app to the JS runtime are Kernel calls that return "effect values": descriptions of effects such as `Cmd`, `Task`, or `VirtualDom.Node`.

But there's a challenge here. It would be nice if we didn't have to implement these effect values in WebAssembly at all. That gives us a nice decoupling between the two sides. Also, some of the effect values contain JS functions, and we don't have the ability to pass arbitrary JS functions _in_ to WebAssembly. Going the other way is fine, but we can't inspect the [scopes](https://css-tricks.com/javascript-scope-closures/) of a JS closure.

I thought of two possible approaches to achieve this:

1. Maintain a JavaScript array of effect values, and let the WebAssembly module refer to them by their _index_ in that array, which is just an integer.
> The trouble with this approach is that it's hard to know when it's safe to _delete_ values from that JS array! We'd need to be sure that nothing in Wasm is holding a copy of that reference. That would require some extension to our custom WebAssembly Garbage Collector to help manage this JS array as well as the Wasm heap! That's quite a lot of work and it's likely to be very hard to debug.

2. Use _thunks_ in WebAssembly to represent the effect values. This is the approach I've implemented, and has some very nice properties! It needs a bit of explanation though. We'll get into it below.



### Thunks

So... what's a thunk?

Well, like most programming terms, it's used slightly differently in different languages, runtimes, etc. But it generally refers to some sort of delayed calculation.

Remember the `Closure` data structure from earlier? We said that it holds all of the partially-applied arguments for a function, along with some reference to an underlying "evaluator function" that actually calculates the final result. We only call the evaluator function when the last argument is applied.

So normally, we never have any use for a Closure that contains _all_ of the arguments. As soon as the last argument is available, it's time to actually make the call. But what if we create a Closure that *does* contain the full set of the arguments, but decide not to actually calculate the result?

Well that's what I mean by a _thunk_. It's a *suspended* function call. All the info we need for a call that we want to make later, but not now.

For example, our Elm app in Wasm might want to make a call like `Process.sleep 1234.5`. Here's what that might look like as a thunk. Note that `n_values` is the same as `max_values`.

![Diagram of a Wasm Closure containing a full set of arguments](./images/thunk-effect-real-arity.png)

Here, `&literal_float_1234_5` is the address of a data structure holding a constant `Float`. I haven't drawn it out but it's similar to the `literal_int_5` from earlier. You can check out the [demo source code](/demos/wrapper/src/main.c) for full details.

If you're curious about other variations on this general idea, see [Wikipedia](https://en.wikipedia.org/wiki/Thunk) and the [Haskell wiki](https://wiki.haskell.org/Thunk).



### Using thunks for effects

This provides a nice solution for our calls from Wasm to JavaScript kernel functions. When our Wasm `update`  wants to call a JS Kernel function to get an effect value, we can _defer_ that call.

Our Wasm `update` returns a value of type `( Model, Cmd Msg )`, where the `Cmd Msg` will be an unevaluated thunk. But on its way from the Wasm app to the runtime in JS, it passes through the wrapper, whose job is to convert Wasm values to equivalent JS values. In the case of a Kernel thunk, we'll convert it to JS by just evaluating it!

This is really neat. Firstly, it means the WebAssembly module doesn't need to have any specific code for various effectful Kernel modules. Secondly it cleanly separates the memory management on each side because none of the values in the JS array ever need to be deleted - they are "permanent" functions, not dynamic values. And finally, the whole thing is completely transparent to the Elm app.



### Implementation details

There are a couple of important details to get right here.

1. We need some way to make sure the Wasm app never actually evaluates the Closure.
2. The `evaluator` field is supposed to be a C function pointer. But we want to call a JS Kernel function, not a C function!

There are many ways to make sure the Wasm app never evaluates the Closure. We _could_ generate special code for these cases. But it would be nice if we didn't have to.

Luckily there is a simple solution. We can just "lie" to the Wasm module and give the `Closure` a `max_values` field that is higher than the real arity of the JS function. The only thing `max_values` is actually used for is to know when we have enough arguments to call the underlying C function. But in this case we *never* want to call any underlying C function, we want just want to collect the arguments in the `Closure`! So we might as well just set `max_values` to the maximum possible value, which happens to be 65535.

This way, any time an argument is applied to it in Wasm, it will look like a partial application, and we'll just get another `Closure`! The behaviour just emerges "for free" from existing functionality.

> By the way, there's no need to worry that a program might apply the wrong number of arguments by mistake! If `update` tried to apply the wrong number of arguments to an effect, it would end up returning the wrong type. (Perhaps `Float -> Cmd Msg` instead of `Cmd Msg`). So the program will never make it past the type checker, and we'll never have to generate Wasm code for it!

Now our "thunk" for `Process.sleep 1234.5` changes to the following:

![Diagram of a Wasm Closure with max_values set to 65535](./images/thunk-effect-max-arity.png)



The second issue is that the `evaluator` is not actually a pointer to a C function. But that's OK. We know we're never going to evaluate it in C anyway! Instead we're going to let the JS wrapper evaluate it.

But we still need some data in the `evaluator` field so that the wrapper knows which JS function to call.

The solution is for the compiler to generate a JavaScript array of all the Kernel functions the app needs to call. And on the C side, generate a `Closure` for each JS Kernel function whose `evaluator` field is really an index of that JS array. (We just type-cast it to look like a pointer). Then when the wrapper wants to evaluate the thunk, it can look up the array to get the function, and apply whatever arguments it finds in the `Closure`.

To make the C code readable, we'll give meaningful names to the JS array indices using an `enum`.

```js
// elm.js
const jsKernelFunctions = [
  _Json_succeed,
  _Platform_batch,
  _Platform_leaf,
  _Scheduler_andThen,
  _Scheduler_succeed,
  _Process_sleep,
  _VirtualDom_node,
  _VirtualDom_on,
  _VirtualDom_text
];
```
```c
// main.c
enum {
  JS_Json_succeed,
  JS_Platform_batch,
  JS_Platform_leaf,
  JS_Scheduler_andThen,
  JS_Scheduler_succeed,
  JS_Process_sleep,
  JS_VirtualDom_node,
  JS_VirtualDom_on,
  JS_VirtualDom_text
};
```



### Summary

- The JS runtime calls the Wasm `update` function through the wrapper.
- `update` makes some calls to JS Kernel functions, but these calls are deferred using thunks.
- When `update` returns a `( Model, Cmd Msg )` to the wrapper, it gets decoded to JavaScript values that the runtime can understand.
- Any thunks in the `Cmd Msg` get evaluated by the wrapper
  - It recognises a Kernel thunk by the fact that it has `max_values=65535`
  - It uses the `evaluator` field to look up the appropriate JS function in the generated array.
  - It then decodes any of the arguments held in the `Closure` and applies them to the JS Kernel function.
  - This process is repeated recursively. (Some of the arguments in the `Closure` may also be thunks.)
- The JS version of the `( Model, Cmd Msg )` tuple is then passed back to the JS runtime, which will treat it as it would in any other Elm program. It can't tell the difference!



## Known issues

### Int / Float ambiguity

If the wrapper receives a JavaScript `number` from the JS runtime, it can't accurately detect whether it's supposed to be an `Int` or a `Float` in Elm. JavaScript doesn't make any distinction between the two. Currently I don't have a reliable solution for this!

For the moment, I'm "making do" with an unsafe temporary workaround. The current iteration of the wrapper checks for _whole_ numbers and assumes they should be written to Wasm as `Int`. But this can go wrong for `Float` values that happen to be round numbers!

I can see two possible solutions

1. Get type info from the compiler to help with encoding
   - This is difficult! The Elm compiler doesn't provide much type information to the back-end that generates the JavaScript output code. For example if the runtime wanted to send the app a `Msg` containing a number, and we knew the particular `Msg` constructor function accepts `Float`, we can use that info to encode correctly. Unfortunately the code generator doesn't have this information. (It's not crucial for generating JS, the only currently-supported compile target.)
   - It might be possible to trick the compiler into revealing this kind of information by `exposing` the message constructor functions. Then their types might be written to an `.elmi` file. It's not a great solution though.
2. Use some Elm code to help with encoding
   - I haven't thought this through fully. But it should be possible for the app to provide `elm/bytes` encoders for its `Msg` types that would make it easier to know where the `Int` and `Float` values are. Obviously this is a workaround for compiler limitations, but it might unlock progress while those limitations are still there.



### Tuple / Record ambiguity

In `--optimize` mode, the generated JS for `( 123, "hello" )` is identical to the JS for `{ a = 123, b = "hello" }`

This causes an ambiguity similar to the `Int`/`Float` ambiguity described above. The solutions are similar. We need type info from either the compiler or the app code.

In unoptimized mode, Tuple has an extra property (`$`) that allows us to distinguish it.


