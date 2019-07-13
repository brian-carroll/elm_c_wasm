- [Project goals](#project-goals)
- [Demos](#demos)
- [Progress](#progress)
- [Big picture stuff](#big-picture-stuff)
- [Effects](#effects)
- [Elm &rarr; JS + Wasm](#elm--js--wasm)
- [String Encoding](#string-encoding)
- [Garbage Collector](#garbage-collector)
- [Closures](#closures)
- [Extensible Records](#extensible-records)
- [Value Headers](#value-headers)
- [Type tags & constrained type variables](#type-tags--constrained-type-variables)
- [Boxed vs unboxed integers](#boxed-vs-unboxed-integers)
- [Alternatives to C](#alternatives-to-c)

# Project goals

- An experiment to try to implement Elm in WebAssembly
- Uses C as an intermediate language
  - This repo contains Elm Kernel code, garbage collector, etc. The parts of the system outside of the compiler itself.
  - Elm Compiler would output C code instead of JavaScript. A second step would further compile that to Wasm.
  - This approach makes it much easier to write Kernel code and to debug the output of the Elm compiler.
- C doesn't have first-class functions or high-level data structures like JavaScript does, so we have to implement some of those things. Most of this is in [types.h](/src/kernel/types.h) and [utils.c](/src/kernel/utils.c)
- The idea is to gradually build Elm's core libraries in C and write some tests to mimic 'compiled' code from Elm programs.

&nbsp;

# Demos

https://brian-carroll.github.io/elm_c_wasm/

&nbsp;

# Progress

- Elm compiler modifications

  - [x] Initial experiments [forking](https://github.com/brian-carroll/elm-compiler/tree/wasm) the Elm compiler to generate Wasm (decided to abandon this direction)
  - [ ] Do another Elm compiler fork to generate C (coming soon...)

- Kernel code / core libs

  - [x] Implement C data structures for all Elm value types: `Int`, `Float`, `Char`, `String`, `List`, tuples, custom types, records, functions
  - [x] Function application and currying
  - [x] Extensible record updates and accessors
  - [x] A prototype Garbage Collector (7kB download size 😊 I have no idea what the performance is like! See [gc.c](/src/kernel/gc.c))
  - [x] Numerical operators from the `Basics` library (`+`, `-`, `*`, `/`)
  - [ ] Finalise String encoding issues, UTF-8, UTF-16, browser/JS interop, etc. I've given it [lots of thought](#string-encoding) but it needs discussion.
  - [ ] JSON & ports (currently most effects are not available in Wasm, which probably means using ports heavily)
  - [ ] Remaining pure kernel modules from core: `String`, `List`
  - [ ] Effectful modules from core: `Program`, `Cmd`, `Task`, `Process`, scheduler, etc.
  - [ ] HTML
  - [ ] HTTP

- My activity levels!
  - I did lots of work on this during 2018, particularly the 2nd half
  - I got busy in the first half of 2019, but my interest is reviving at the moment and I have more time on my hands again!
  - I meant to write some blog posts about this, but I ended up only writing one so far. It was on [first class functions][blogpost].

&nbsp;

# Big picture stuff

- Effects

  - Wasm MVP has no Web APIs like DOM, XHR, etc., so most effect managers must be JS only for Wasm MVP
  - Unclear how to interface with browser APIs from C, I guess some header files you `#include` and call functions on.
  - Perhaps a good intermediate step would be to generate pure code in C/Wasm and effectful code in JS

- Browser GC

  - Wasm spec talks about data types like arrays, objects, opaque references.
  - Unclear what this looks like from C. Values would have integer IDs rather than pointers so that you can't do bad things. I guess you call some library functions to create, get nth child, etc.

- Custom GC

  - Impact of GC optimizations based on immutability
    - Want some way to allow Kernel to mutate things despite GC being optimized for immutability.
    - Solution: Keep all mutations outside of the heap. If an Effect Manager needs to dynamically allocate something and mutate it, it can first create new immutable value on the heap, then just mutate an off-heap pointer to point at new instead of old. This is like the way `model` updates already work in elm.js.
    - Process IDs being references may be a bit of a pain. Ideally they'd be integers. But only really used for `kill`, which isn't fully implemented even in JS.

- Kernel ends up in multiple languages? JS and C?
  - Quite a bit of maintenance
  - Would it be good to put more of the code in Elm?

&nbsp;

# Effects

- Wasm MVP doesn't yet have access to Web APIs like DOM, `XmlHttpRequest`, etc.
- This means an Elm program in Wasm has to call out to JS to do any effects.
- Also, all communication with JS is done using typed arrays. This means everything has to be serialised, using JSON or some other format.

&nbsp;

# Elm &rarr; JS + Wasm

- It's a **lot** of work to get to compile a "hello world" Elm program to 100% Wasm. Because of dead code elimination, you can't get any output from the compiler until you have an implementation for `Program`. And that requires building a lot of really complex stuff like the effect manager system.
- I need a way for the Elm program to end up as a mix of JS and Wasm. That way I can keep the kernel in JS initially, with the Elm code compiled to Wasm. See [proof-of-concept demo](https://brian-carroll.github.io/elm_c_wasm/update-int/index.html) with `update` function in Wasm and the rest in JS.

* Annoyingly, everything crossing the JS/Wasm boundary has to be serialized and de-serialized (as JSON strings)
* How does that work for `Cmd`? Surely it's not serialisable?
  - `Cmd Msg` going from `update` to the runtime will always have a `Msg` constructor function inside it. In JS-speak this is a callback.
  - Need to spot this happening in code gen. Then export it to be callable from JS, and pass that JS version to the _real_ Elm runtime.
  - Maybe there's a need for an Elm wrapper module for this, exposing a `Program` constructor and maybe some other stuff. My code generator can just make special cases for that module.

&nbsp;

# String Encoding

WebAssembly has no string primitives, so they have to be implemented at the byte level. That makes sense because different source languages targeting WebAssembly may have different string representations, and WebAssembly needs to support that.

The String struct defined in [types.h](/src/kernel/types.h) contains header and a sequence of bytes. There are various "encodings" of characters to bytes, and the modern _de facto_ standard is UTF-8. A lot of recently-developed languages use it as their default encoding (Go, Rust, etc.).

## String representations in other languages

### Python

The Python 3 Standard library has a Unicode type whose API is Unicode code points. But as per [PEP 393][pep-393], the underlying representation is a C structure that uses different storage formats depending on the maximum character value at creation time. It also holds metadata including length, a hash (for use in dictionary keys etc.), and the representation used.

[pep-393]: https://www.python.org/dev/peps/pep-0393/

### Rust

Rust's [String][rust-string] consists of "a pointer to some bytes, a length, and a capacity. The pointer points to an internal buffer String uses to store its data. The length is the number of bytes currently stored in the buffer, and the capacity is the size of the buffer in bytes." The internal representation is UTF-8. There are APIs to convert between bytes and strings.

[rust-string]: https://doc.rust-lang.org/std/string/struct.String.html#representation

### Java

Similarly, Java's [JEP 254][jep-254] describes multiple string representations depending on the maximum character value. However all the built-in representations use units of either 8 or 16 bits. There is no built-in Unicode support but there are libraries to support it. A detailed density analysis of different string types for Java can be found [here][java-string-density].

[jep-254]: http://openjdk.java.net/jeps/254
[java-string-density]: http://cr.openjdk.java.net/~shade/density/state-of-string-density-v1.txt

### JavaScript

[This article][js-string-encoding] gives a detailed description of JavaScript's string representation. The summary is that the ECMAScript standard allows for engines to use either UTF-16 or UCS-2, which are similar but slightly different. Most engines use UTF-16.

[js-string-encoding]: https://mathiasbynens.be/notes/javascript-encoding

### OCaml

OCaml's [String][ocaml-string] library is based on a sequence of one-byte characters. Unicode support doesn't seem to be strong.

[ocaml-string]: https://caml.inria.fr/pub/docs/manual-ocaml/libref/String.html

### Summary

Most languages seem to grapple with a tradeoff between Unicode compliance, convenience, and memory density. It's considered best practice to present the application programmer with an API that treats strings as sequences of Unicode characters, while trying to have an underlying representation that is as dense as possible.

Most guides on this are targeted at application developers rather than language implementers. Best practice guides such as [The Unicode Book][unicode-book] and one from the [Flask][flask] web framework, advocate that programs should deal exclusively with Unicode characters internally, and only do encoding and decoding when dealing with external systems you can't control. In Elm this means the `String` package should provide functions to iterate over Unicode `Char`s and only Effect Managers should deal with encodings.

The internal memory representation should be something that facilitates this.

[flask]: http://flask.pocoo.org/docs/1.0/unicode/
[unicode-book]: https://unicodebook.readthedocs.io/good_practices.html

## Strings with Web APIs

Most of the browser's Web APIs use JavaScript Strings in UTF-16 format. For example `document.getElementById` expects its argument to be a [DOMString][domstring], which is UTF-16. `XmlHttpRequest` can deal with UTF-8 request and response bodies, but what about the string that specifies the URL? That's usually done with JavaScript strings. When the WebAssembly API comes out, will that require UTF-16 too? I can only suppose that the browser's underlying C++ implementation expects UTF-16, so wouldn't it present this to WebAssembly?

[domstring]: https://developer.mozilla.org/en-US/docs/Web/API/DOMString

There's limited information at this stage on how the Web APIs will work with WebAssembly. There's an [overview of the proposal][host-bindings] but it seems to leave a lot up to browser vendors. It focuses on very low-level details and doesn't say anything about specific APIs like DOM or HTTP.

[host-bindings]: https://github.com/WebAssembly/host-bindings/blob/master/proposals/host-bindings/Overview.md

The general idea is that each Web API will be represented as a "table" of numbered functions. To send a string from Wasm to a browser API, the Wasm program writes it to its own block of memory and passes the address and length to one of the API functions. The Wasm memory block is visible to JavaScript as an ArrayBuffer and also visible to browser APIs, so it can be read from there.

When the browser sends a string to Wasm, calls an "exported" function in the Wasm program to tell it how much memory to allocate for that string. The Wasm program returns a memory address for the external code to write to, and gets a callback when it is done.

The proposal does actually mention UTF-8 encoded strings as one of the possible interface types. It also mentions ArrayBuffer and JSON. The JSON data is "parsed as if it were passed to `JSON.parse()`", which sort of implies UTF-16, I think. It remains to be seen how many Web APIs will actually provide the UTF-8 String argument type.

## Conclusions on string encoding

If Elm was targeting native compilation then UTF-8 would definitely be the way to go. But it's quite focused on the browser as a platform, so it seems like there may be real performance reasons to use UTF-16. It's probably worth doing some benchmarking once the Web APIs become available in Wasm.

UTF-16 has a bit of a bad reputation because of buggy implementations that have been done in the past. There are a lot of UTF-16 string APIs that confuse characters with code units, including JavaScript's (and in some cases, even Elm's!). But as far as I know it's perfectly possible to do a _good_ implementation.

```elm
> s = "🙈🙉🙊"
"🙈🙉🙊" : String
> String.length s
6 : Int
> List.length (String.toList s)
3 : Int
```

&nbsp;

# Garbage Collector

I've built a prototype Garbage Collector in this repo (see [gc.c](/src/kernel/gc.c)). So far I can only run [unit tests][unit-tests-gc] on it, since I don't have any Elm programs compiled to Wasm yet!

[unit-tests-gc]: https://brian-carroll.github.io/elm_c_wasm/unit-tests/index.html?argv=--gc+--verbose

_The GC fits into less than 7kB of binary Wasm!_

## Features

It uses a [mark-compact algorithm](https://en.wikipedia.org/wiki/Mark-compact_algorithm) that is greatly simplified by the fact that all Elm values are immutable and can therefore only point to _older_ values.

This means that when you move a value, you only need to update references to it in _newer_ values. You don't need to scan the whole heap. Older values end up the bottom of the heap and newer ones at the top. At any time you can easily do a _partial_ collection of the heap, picking any starting point and only collecting items _above_ that.

This brings a lot of the advantages of a generational collector, without the overhead of managing regions. Hopefully this helps to keep the GC small.

## Mutable values in kernel code

Some kernel code in the core modules needs to dynamically allocate values and mutate them. But this could result in old values pointing at newer ones, breaking some of the assumptions the GC relies on.

A solution that works here is to only mutate a _pointer_ to a value. Copy the value immutably, then switch the pointer to reference it instead of the old value. The pointer has fixed size so it can be outside the garbage-collected area.

This would mean changes in the way some of the core modules work. The changes all look fairly minor except for [Scheduler.js](https://github.com/elm/core/blob/1.0.2/src/Elm/Kernel/Scheduler.js) and [Platform.js](https://github.com/elm/core/blob/1.0.2/src/Elm/Kernel/Platform.js).

## Updating pointers in the call stack

The GC also takes advantage of the fact that Elm functions are _pure_.

When resuming execution after a GC pause, we need to restore the state of the call stack to what it was before the pause. This is done using something I call "replay mode". When resuming execution, any function call that had completed before the pause is skipped and replaced with the return value it had previously.

The call stack quickly gets to its original state, and we skip the vast majority of code execution and do no new allocations. It's quite powerful to be able to competely unwind the call stack and quickly restore it. It guarantees that all pointers in the stack and registers are referencing new, valid locations.

Most GC's scan the stack and registers for stale pointers to heap values that have moved, and mutate them in place. It's tricky business at the best of times, but as far as I can tell, WebAssembly's semantics actually make it impossible!

In order to implement "replay mode", the GC inserts special markers into the heap to keep track of which call allocated each value, and which are currently active. It's an implementation of a "stack map".

For more detail, you can check out the output of the `gc_replay_test` [in your browser][unit-tests-gc], or take a look at the [test code](/src/test/gc_test.c) or the comments in the [source code](/src/kernel/gc.c).

## Scheduling collections

We should be able to schedule most collections during the idle time just after an `update`. In this case, we don't need to pause at all. The "replay mode" described above is only needed if we run out of memory during an `update`.

&nbsp;

# Closures

I previously wrote a [blog post][blogpost] about how to implement Elm first-class functions in WebAssembly. The Closure data structure in [types.h](/src/kernel/types.h) is based on those ideas, although it has evolved slightly in the meantime.

In a nutshell, the Closure data structure is a value that can be passed around an Elm program. It stores up any arguments that are partially applied to it, until it is "full". It also contains a function pointer, so that when the last argument is applied, the actual "evaluator" function can be called.

A working example of all of this can be the tests for the `apply` operator. Check out the [source][utils-test-src] or [run the tests][utils-test-run] in your browser.

[utils-test-src]: /src/test/utils_test.c
[utils-test-run]: https://brian-carroll.github.io/elm_c_wasm/unit-tests/index.html?argv=--utils+--verbose
[blogpost]: https://dev.to/briancarroll/elm-functions-in-webassembly-50ak

&nbsp;

# Extensible Records

A good intro to Elm extensible records can be found [here](https://elm-lang.org/docs/records#access). In this project they are split into two C structs, `Record` and `FieldSet`, defined in [types.h](./src/kernel/types.h).

Field names are represented as integer "field IDs". The compiler would convert every field name in the program to a unique ID, using the same kind of [optimisation][shortnames] the Elm 0.19 compiler uses to shorten fieldnames in `--optimize` mode.

[shortnames]: https://github.com/elm/compiler/blob/0.19.0/compiler/src/Generate/JavaScript/Mode.hs#L79

The `Record` struct stores only the values, in ascending order of the corresponding field IDs. The field IDs themselves are stored in a `FieldSet`, a single structure shared by all values of the same Record type, in ascending order. To access a field by its field ID, we first look up the field ID in the `FieldSet`. If it's in the nth position, then the corresponding value will also be in the nth position in the Record itself.

## Record accessor functions

Elm has special functions for accessing records, prefixed by a dot, like `.name`, which can be applied to _any_ Record type that contains a field called `name`. It's implemented using a Kernel function that takes the field ID as an Elm `Int`, and the record itself.

```elm
    access : Int -> r -> a
    access fieldId record =
        -- Kernel C code
        -- 1. Look up the FieldSet that this record points to
        -- 2. Find the index of `fieldId` in that FieldSet (binary search)
        -- 3. Return the value found at the value at that same index in `record`
```

The compiler would insert code to create each accessor function by partially applying the relevant `fieldId` to `access` function in the generated code.

```elm
    -- Compiler inserts something roughly equivalent to this to to define `.name`
    .name record = access 123 record  -- where 123 is the field ID for 'name'
```

The implementation is in [utils.c](/src/kernel/utils.c) (see `access_eval`). The code is unsafe if the field does not actually exist in the record, but it can only be called in compiler-generated code.

## Record update

Elm's record update syntax is `r2 = { r1 | field1 = newVal1, field2 = newVal2 }`

Currently, Elm implements this using a [JavaScript function][js-update]. We do something similar here with a C function called `record_update`, found in [utils.c](./src/kernel/utils.c). A pseudo-code version is below.

[js-update]: https://github.com/elm/core/blob/1.0.2/src/Elm/Kernel/Utils.js#L151

```
Clone the original record
For each field ID to be updated
	Find the index of the field ID in the FieldSet
	Change the pointer in the clone at the same index to point at the updated value
```

Check out the [source][/src/kernel/utils.c] or [run the tests][utils-test-run] in your browser.

&nbsp;

# Value Headers

Every Elm value has a header of 32 bits in size. It's defined in [types.h](./src/kernel/types.h)

```
-----------------------------------------------------
| tag (4 bits) | size (28 bits) |      Elm data     |
-----------------------------------------------------
```

`size` is measured in _words_, where a word is either 32 or 64 bits, depending on the target platform. It makes sense to use words rather than bytes because all values are aligned to word boundaries anyway. For example in a 32-bit system, we'll always place our values at addresses that evenly divide by 4 bytes. Real CPUs are optimised to work faster when pointers are aligned this way.

The only individual value that can get really large in practice is `String`. (Lists don't count, they are made up of many Cons cells.) A maximum value of 2<sup>28</sup>-1 for `size` corresponds to 1 GB on a 32-bit system or 4 GB on a 64-bit system.

We always use 32-bit headers, even on 64-bit systems. 1GB is large enough, there's no point increasing the header size. Wasm is always 32 bits but since we're using C as an intermediate language, we can also create native 64-bit binaries. That's how I run most of my tests.

&nbsp;

# Type tags & constrained type variables

To explain how the type tag in the header works, we need to discuss [constrained type variables][guide-type-vars]. This is the feature of Elm that allows some functions like `++`, `+` and `>`, to work on _more than one, but not all_ types.

[guide-type-vars]: https://guide.elm-lang.org/types/reading_types.html#constrained-type-variables

To facilitate this, we insert a "tag" as metadata into the byte level representation of every Elm value. The tag is a 4-bit number carrying information about the type and memory layout of the value. For example, the low-level implementation for `++` needs to know whether its arguments are Lists or Strings because the memory layout for each is totally different. Using the tag data, it can decide which of two code branches to execute.

| Tag |   Type   | **number** | **comparable** | **appendable** |
| :-: | :------: | :--------: | :------------: | :------------: |
|  0  |  `Int`   |     ✓      |       ✓        |                |
|  1  | `Float`  |     ✓      |       ✓        |                |
|  2  |  `Char`  |            |       ✓        |                |
|  3  | `String` |            |       ✓        |       ✓        |
|  4  |  `List`  |            |       ✓        |       ✓        |
|  5  | `Tuple2` |            |       ✓        |                |
|  6  | `Tuple3` |            |       ✓        |                |
|  7  |  Custom  |            |                |                |
|  8  |  Record  |            |                |                |
|  9  | Closure  |            |                |                |

_The remaining 6 possible values (`a`&rarr;`f`) are reserved for Garbage Collector record-keeping data.)_

For more details see the [header file](/src/kernel/types.h) defining the relevant structs, or see the [output of some tests][types-test] in your browser.

[types-test]: https://brian-carroll.github.io/elm_c_wasm/unit-tests/index.html?argv=--types+--verbose

&nbsp;

# Boxed vs unboxed integers

In this project, all values are "boxed" - i.e. they have a header that contains some metadata. They're all stored on the heap, and are referred to via a pointer. This setup makes a lot of sense for more complex value types like lists, tuples, records, strings. But for integers it can be a lot of overhead. The `+` operator has to fetch two structures from memory, separate the integer from its header, add the numbers, wrap the new value in a new data structure, and write it back to memory. For a numerical expression like `a-(b+c)*d`, or more complex expressions, this can be expensive.

Many language implementations "unbox" integers, so they're represented directly as machine integers without any wrapper or metadata. This can be a big performance gain for some common code patterns, but it requires a lot of book-keeping. It can be hard to tell the difference between integers and pointers, you need some system to keep track of what's what.

In this project I've avoided unboxing integers because it seems like it would be a major piece of work. I'd rather try to build a working implementation first, and optimise later.

However there are some relatively simple compiler optimisations that could reduce the cost of boxing. For a start, we could translate an Elm expression like `a-(b+c)*d` into the equivalent expression in C, only boxing the final result rather than the result of each subexpression. This kind of thing should be limited to just the code generator. In fact the Elm compiler's JS code generator already has some [special handling for numerical operators](https://github.com/elm/compiler/blob/0.19.0/compiler/src/Generate/JavaScript/Expression.hs#L526).

&nbsp;

# Alternatives to C

## Rust

I really wanted to use Rust. It's just generally a better language. Hey, it's even heavily influenced by Haskell and ML, just like Elm!

But using Rust to implement Elm was difficult enough that I got frustrated and demotivated and stopped working on this project for a few weeks. This project is a hobby for me, so if I'm not enjoying it then it won't happen. And at some point it was either drop the project or switch to C.

Rust is _definitely_ a better language... for handwritten code. But we're talking about _generated code_ from the Elm compiler. It's different in a few ways.

Firstly, Elm's type system guarantees that a lot of potentially bad things can't happen. But the Rust compiler doesn't know that, so the extra strictness can really be a pain. We basically have to prove to Rust that some code that came out of the Elm compiler is type safe, even though we already know it is. This turned out to be tricky, and without much real benefit.

Secondly, it's well known that "fighting the borrow checker" is something that everyone learning Rust goes through. And I have never used Rust for a real project before, so I experienced it too. Apparently you eventually learn how to deal with it. But the borrow checker isn't actually designed for garbage-collected code anyway, so it's quite a lot of pain and effort with not much reward at the end!

Some of the patterns Rust encourages didn't seem to make sense to me in a garbage collected context. Rust encourages you to keep as many of your values as possible in the stack, which makes sense when you don't have a garbage collector - you want to destroy values as soon as you don't need them anymore. In Elm, a _lot_ more stuff is going to be allocated on the heap than in a normal Rust program and that should be OK.

It really felt like at every turn, I was spending all my time _fooling_ Rust into believing my code was OK, and in the process, invalidating a lot of the extra help it should give me compared to C. I also had a couple of years experience with C from about 10 years ago, so that was a factor too. But to be clear, I had no burning desire to go back to it!

## Direct to Wasm

My first approach to this project was to directly generate WebAssembly from the Elm AST using a forked version of the compiler. I wrote a set of Haskell types to model a WebAssembly module, and created a WebAssembly DSL and code generator from that.

I wrote a few of Elm's Kernel functions using the same Haskell DSL. So the Kernel code was actually written in Haskell that would generate WebAssembly.

That all worked reasonably well. I got an example Elm program working, which implemented currying and closures.

However as soon as I had my first real piece of debugging work to do, I realised it was pretty painful. That seems kind of obvious, but I'd thought the DSL would help a lot more than it did. Wasm is just so low-level I couldn't keep enough of it in my mind at one time to be useful for debugging.

However I really learned a lot about WebAssembly by doing this.
