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
