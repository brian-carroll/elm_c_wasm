# Byte-level implementations for Elm values

## Value Headers

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

## Type tags & constrained type variables

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

## Closures

I previously wrote a [blog post][blogpost] about how to implement Elm first-class functions in WebAssembly. The Closure data structure in [types.h](/src/kernel/types.h) is based on those ideas, although it has evolved slightly in the meantime.

In a nutshell, the Closure data structure is a value that can be passed around an Elm program. It stores up any arguments that are partially applied to it, until it is "full". It also contains a function pointer, so that when the last argument is applied, the actual "evaluator" function can be called.

A working example of all of this can be the tests for the `apply` operator. Read the [source][utils-test-src] or [run the tests][utils-test-run] in your browser.

[utils-test-src]: /src/test/utils_test.c
[utils-test-run]: https://brian-carroll.github.io/elm_c_wasm/unit-tests/index.html?argv=--utils+--verbose
[blogpost]: https://dev.to/briancarroll/elm-functions-in-webassembly-50ak

&nbsp;

## Extensible Records

A good intro to Elm extensible records can be found [here](https://elm-lang.org/docs/records#access). In this project they are split into two C structs, `Record` and `FieldSet`, defined in [types.h](./src/kernel/types.h).

Field names are represented as integer "field IDs". The compiler would convert every field name in the program to a unique ID, using the same kind of [optimisation][shortnames] the Elm 0.19 compiler uses to shorten fieldnames in `--optimize` mode.

[shortnames]: https://github.com/elm/compiler/blob/0.19.0/compiler/src/Generate/JavaScript/Mode.hs#L79

The `Record` struct stores only the values, in ascending order of the corresponding field IDs. The field IDs themselves are stored in a `FieldSet`, a single structure shared by all values of the same Record type, in ascending order. To access a field by its field ID, we first look up the field ID in the `FieldSet`. If it's in the nth position, then the corresponding value will also be in the nth position in the Record itself.

### Record accessor functions

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

### Record update

Elm's record update syntax is `r2 = { r1 | field1 = newVal1, field2 = newVal2 }`

Currently, Elm implements this using a [JavaScript function][js-update]. We do something similar here with a C function called `record_update`, found in [utils.c](./src/kernel/utils.c). A pseudo-code version is below.

[js-update]: https://github.com/elm/core/blob/1.0.2/src/Elm/Kernel/Utils.js#L151

```
Clone the original record
For each field ID to be updated
Find the index of the field ID in the FieldSet
Change the pointer in the clone at the same index to point at the updated value
```

Read the [source code](/src/kernel/utils.c) or [run the tests][utils-test-run] in your browser.

&nbsp;

## Boxed vs unboxed integers

In this project, all values are "boxed" - i.e. they have a header that contains some metadata. They're all stored on the heap, and are referred to via a pointer. This setup makes a lot of sense for more complex value types like lists, tuples, records, strings. But for integers it can be a lot of overhead. The `+` operator has to fetch two structures from memory, separate the integer from its header, add the numbers, wrap the new value in a new data structure, and write it back to memory. For a numerical expression like `a-(b+c)*d`, or more complex expressions, this can be expensive.

Many language implementations "unbox" integers, so they're represented directly as machine integers without any wrapper or metadata. This can be a big performance gain for some common code patterns, but it requires a lot of book-keeping. It can be hard to tell the difference between integers and pointers, you need some system to keep track of what's what.

In this project I've avoided unboxing integers because it seems like it would be a major piece of work. I'd rather try to build a working implementation first, and optimise later.

However there are some relatively simple compiler optimisations that could reduce the cost of boxing in the nearer term. For a start, we could translate an Elm expression like `a-(b+c)*d` into the equivalent expression in C, and box only the final result rather than the result of each subexpression. This optimisation should be limited to just the code generator. In fact the Elm compiler's JS code generator already has some [special handling for numerical operators](https://github.com/elm/compiler/blob/0.19.0/compiler/src/Generate/JavaScript/Expression.hs#L526).
