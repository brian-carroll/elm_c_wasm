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

#### IDs not generated for constructors and record fields

- CTOR_Root

  - an app route that will never be parsed from any URL!
  - no value is ever constructed with it so it's not generated
  - it kinda sorta _should_ be a compile error, or be dead-code eliminated
  - It's tested in a case expression. Maybe I'm not updating my list of constructors for case tests?
    - No, if any value was ever constructed with it, I wouldn't need to! 😜
  - Solution
    - Need to treat ctors the same as all the other shared stuff. I'd thought of this before, it's OK.

- FIELD_start
  - From Time module. Part of an unfinished API for time zones.
  - Again, there is no code in the app to generate one of these records
  - It's in an unused branch of a used function, so no DCE
  - Breaks my assumption that all field names must, at some point, be part of a record literal
  - Need to track fields and fieldgroups separately
  - Solution
    - Record literals => update fieldgroups (this is the _only_ thing we have right now)
    - Access => update fields
