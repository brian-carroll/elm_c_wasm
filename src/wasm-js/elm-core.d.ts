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
declare const elm$core$Basics$True: { $: 'True' };
declare const elm$core$Basics$False: { $: 'False' };

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
