## Richard Feldman's SPA example, compiled to WebAssembly

Serves as a vehicle to develop and debug the code generator, and motivation to expand the set of C kernel functions.

Richard's repo is a Git submodule of this one, so I can use the original source code. I just change the build system and the index.html file.

### Issues currently causing C compiler errors

#### Kernel code not written yet

- Basics_ceiling
- Basics_fdiv
- Basics_floor
- Basics_idiv
- Basics_modBy
- Basics_not
- Basics_toFloat
- String_trim
- Utils_notEqual

#### Closures used before they are declared

- elm_core_Basics_negate

  - negate is `x -> -x`
  - but `-` generates `negate`
  - JS code gen does optimisations for saturated Basics calls that gets around this circular definition. Was going to leave this for later but apparently need it now!

#### IDs not generated for constructors and record fields

These are all from `elm` packages. Could be a dependency issue. Maybe they're indirect deps via JS kernel code?

- CTOR_BadStatus
- CTOR_EQ
- CTOR_Field
- CTOR_Http
- CTOR_Index
- CTOR_Internal
- CTOR_LT
- CTOR_OneOf
- CTOR_Root

- FIELD_fragment
- FIELD_host
- FIELD_path
- FIELD_port\_
- FIELD_protocol
- FIELD_query
- FIELD_start
