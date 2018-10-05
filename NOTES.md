64 bit platforms
================
So while doing testing I've discovered that it's much nicer to compile to native binary on my laptop and run things there.
But my laptop is 64 bits and Wasm is 32.
So it would be handy to support both. But also a bit of a pain to do.

It mainly affects the `size` field of the header, which is not used for very much except GC and string length.

Padding is added for the following Elm types by gcc for 64-bit target:

|padded?| structure |
| ----- | --------- |
|       | ElmInt    |
| ✔️    | ElmFloat  |
|       | ElmChar   |
|       | ElmString |
|       | Nil       |
| ✔️    | Cons      |
| ✔️    | Tuple2    |
| ✔️    | Tuple3    |
|       | Custom    |
| ✔️    | Record    |
|       | Closure   |

- gcc is making sure that pointers and floats are on 64-bit boundaries for a 64-bit target
- Elm Float has a float, so is padded
- Const, Tuple2, Tuple3 and Record have only the header before the first pointer.
- Int, Char, String, and Nil don't have either floats or pointers don't need this padding
- Custom and Closure already happen to have their pointers on a 64-bit boundary don't need it )
- OCaml expands header to 64 bits on 64-bit machines. But that doesn't help me here! Custom and Closure would end up getting padded instead.

OK so there doesn't seem to be any way to avoid the 64-bit alignment happening.

### So what should `size` be?
I want it to indicate the memory size of the structure's body, shifted by a few bits to gain some range. I decided to exclude the header from the size because it's metadata and things get too self-referential.

Also String length is more understandable this way.


### How to handle in GC
- GC wants to copy the whole structure, header and all
- For a Cons cell, I have a size of 2 and I need to copy 24 bytes.
- For an ElmChar I have a size of 0.5! **Can't use pointer units, need int units**
- Best way to handle it is probably in the `#define` statements for the headers. Make `size` always be right for the target.
- Function to find first pointer must also be different for 64-bit target.

### How to handle in String length & padding
- If `size` is measured in pointers, then just add more padding to the end of the string on a 64-bit target, and make sure the algorithm works.



Assertions & Exceptions
=======================
Is there any way to add assertions in C and optimise them away in prod?
Would be great to sprinkle them around to do type checks and other invariant checks.
Maybe an alternative is to have a `#define ELM_WARNINGS` that enables some `printf` warning statements.

For the cases where Elm actually throws exceptions, can I even do that?
What happens if I divide by zero? Nuthin'

Unit tests probably cover a lot of it though.


Build system
============
- This is C, so might as well use `make` and follow the herd.
- No need for `configure`
- Tests can target a binary rather than a browser
- Project structure
```
    src/
        kernel/
            types.c
            utils.c
            basics.c
        user/
            example1.c
            example2.c
    tests/
        kernel/
            types_tests.c
            utils_tests.c
            basics_tests.c
        user/
            example1_tests.c
            example2_tests.c
    build/
        kernel/
            *.o
        user/
            *.o
        tests/
            *.o
    dist/
        prod/
            *.js, *.html, *.wasm
        dev/
            *.js, *.html, *.wasm
        tests/
            run
            kernel/
                types_tests
                utils_tests
                basics_tests
            user/
                example1_tests
                example2_tests
```
- Makefile conventional target names
    - `all` the default... I'll use it for dev build
    - `clean` delete generated stuff
    - `dist` production build
    - `check` run tests


Testing
=======
- Need it! Made lots of mistakes in String.
- https://stackoverflow.com/questions/65820/unit-testing-c-code
- Start with stupidly simple zero-magic [MinUnit](http://www.jera.com/techinfo/jtns/jtn002.html) because I'll lose motivation if I have to deal with setting things up in C.



Optimisations
=============

Effect of various emcc compiler [optimisation settings](https://kripken.github.io/emscripten-site/docs/tools_reference/emcc.html)

| option | time | wasm | js |
| ------ | ---- | ---- | -- |
| -O0 | 6.13 | 58,785 | 103,092 |
| -O1 | 4.85 | 28,695 | 68,823 |
| -O2 | 7.60 | 32,520 | 27,899 |
| -O3 | 9.49 | 30,209 | 21,999 |
| -Os | 9.06 | 25,744 | 21,999 |
| -Oz | 8.92 | 25,670 | 21,999 |

`-Os` and `-Oz` are size-only optimisations, the rest are a size/speed tradeoff.
- `-O3` is recommended for production.
- `-O1` seems good for development.

Less allocation
===============
Feel like I'm allocating too much stuff, and maybe doing it inside functions is a bad practice. Maybe the caller should make the decision on whether something is stack or heap.

Rust guidance is not to pass pointers back from functions as a way to save stack space. They said pass back the whole data structure and let the caller decide what to do with it. But also there was something about Rust being clever enough (for what wasn't clear but maybe it can use heap if it wants?).

So let's take `apply`. Can I pass back a whole data structure from that? It's so general it could be anything. Well any _top-level_ container anyway. And it could always be another closure of any size. Call site may not necessarily know the size at compile time, like if it's a higher order function.

Could apply return some union of either a pointer to a heap object OR a full stack object?

Structures whose (shallow) sizes are not known at compile time, at the call site
- Closure
- String
- Record (it's a concrete record type but which one? Can a function return an extensible record type other than the one you gave it? Actually I don't think so.)

Containers with type parameters are fixed-size but can point to other things. The pointer is the same size regardless of the type parameter's value.

This is what Rust was trying to make me do! Make everything known-size. It requires farming out Closure args and string body to separate structures.

Why do I care? Stack is no faster than heap to access, but heap involves more management (GC).

If I'm doing this, then the stack version of the object needs to be completely stand-alone and independent of the GC header. In other words String length can't be just in GC header, has to be in the String object itself so that it can exist in the stack.
Or I suppose you could put useless GC headers in the stack.
