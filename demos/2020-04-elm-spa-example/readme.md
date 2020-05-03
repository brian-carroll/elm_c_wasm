## Richard Feldman's SPA example, compiled to WebAssembly

Serves as a vehicle to develop and debug the code generator, and motivation to expand the set of C kernel functions.

Richard's repo is a Git submodule of this one, so I can use the original source code. I just change the build system and the index.html file.

### Issues currently causing C compiler errors

#### Kernel code not written yet

- String_contains
- String_indexes
- String_startsWith
- String_trim

#### Closures used before they are declared

- elm_core_Basics_negate

  - This ends up being self-referential
  - The `negate` function is `x -> -x`, but the `-` operator is generated as the `negate` function!
  - JS code gen does optimisations for saturated Basics calls, which gets around this circular definition. I was going to leave those optimisations for later but apparently need them now!
