/*----------------------------
  TEST HELPER DECLARATIONS
-----------------------------*/

declare function formatHex(
  arg: Uint16Array | Uint32Array | number | object,
  bits?: 16 | 32
): string | string[] | object;
