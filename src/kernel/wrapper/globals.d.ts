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

declare const F2: Function;
declare const F3: Function;
declare const F4: Function;
declare const F5: Function;
declare const F6: Function;
declare const F7: Function;
declare const F8: Function;
declare const F9: Function;

/*----------------------------
  TEST HELPER DECLARATIONS
-----------------------------*/

declare function formatHex(
  arg: Uint16Array | Uint32Array | number | object,
  bits?: 16 | 32
): string | string[] | object;
