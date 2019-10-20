/* --------------------------------------------------

          ELM'S JAVASCRIPT KERNEL

  TypeScript declarations for some values and functions
    in Elm's generated JS.
  Tells the TypeScript compiler that these values
    are defined "globally" somewhere.
  Doesn't cover `--optimize` mode.

  -------------------------------------------------- */

declare function _List_Cons<H, T>(hd: H, tl: T): { $: '::'; a: H; b: T };
declare const _List_Nil: { $: '[]' };
declare const _Utils_Tuple0: { $: '#0' };
declare function _Utils_Tuple2<A, B>(a: A, b: B): { $: '#2'; a: A; b: B };
declare function _Utils_Tuple3<A, B, C>(
  a: A,
  b: B,
  c: C
): { $: '#2'; a: A; b: B; c: C };
declare function _Utils_chr(c: string): String;

/* Curry
 */
declare function F2<A, B, C>(fun: (a: A, b: B) => C): (a: A) => (b: B) => C;

declare function F3<A, B, C, D>(
  fun: (a: A, b: B, c: C) => D
): (a: A) => (b: B) => (c: C) => D;

declare function F4<A, B, C, D, E>(
  fun: (a: A, b: B, c: C, d: D) => E
): (a: A) => (b: B) => (c: C) => (d: D) => E;

/* Apply
 */
declare function A2<A, B, C>(fun: (a: A) => (b: B) => C, a: A, b: B): C;

declare function A3<A, B, C, D>(
  fun: (a: A) => (b: B) => (c: C) => D,
  a: A,
  b: B,
  c: C
): D;

declare function A4<A, B, C, D, E>(
  fun: (a: A) => (b: B) => (c: C) => (d: D) => E,
  a: A,
  b: B,
  c: C,
  d: D
): E;

/*----------------------------
  TEST HELPER DECLARATIONS
-----------------------------*/

declare function formatHex(
  arg: Uint16Array | Uint32Array | number | object,
  bits?: 16 | 32
): string | string[] | object;
