Next steps
==========
- Nested equality
- Record accessors & updates
    - Functions in utils, curried with an ElmInt
    - Test code in test_utils
- Destructuring
- Case expression / pattern match
    - Come up with some C structures corresponding to Haskell types
    - Use JS as a reference
- Cover all the Expr types


Unboxing integers
=================
What would it actually take?

All containers would need a bitmap. Could be just a single word (32 or 64)
It has to be pointer-sized so that for large containers, we can put the bitmap in an separate look-up table (like Haskell). LSB can be the marker of large/small container, like OCaml does for a different purpose.
Maybe it's simpler to just always externalise the bitmap.

This would also allow unboxing Unit, Bool, and Char.

So whenever we want to access a field of a container, we have to call a function to get it, rather than directly using the struct.

**Key Problem:** How does `compare` know that it's looking at an `Int` rather than a pointer to some structure? Not really feasible at runtime. Code gen would need to do something different for Int. Which means the AST going into code gen would need to have type information. Currently it doesn't, and I'm not going down that rabbit hole.


64 bit platforms
================
I've discovered that it's a _much_ faster dev cycle to compile to a binary executable on my laptop and run tests that way. Compile time alone is ~6x faster, and you don't have to switch windows and reload and all that. Once all tests are passing in binary, I can run a WebAssembly test in the browser and fix up any remaining issues.

But my laptop is 64 bits and Wasm is 32, so a bit of work is needed to support both. It's also nice for future server-side Elm and whatnot.

Most of the C code is independent of 32 vs 64 bits. We just use array indices or structure fields and the compiler figures out the pointer offsets.

It mainly affects the `size` field of the header, which is intended for GC and for string length.

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
