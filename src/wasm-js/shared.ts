import './elm-core';

export declare const mem32: Uint32Array;
export declare const mem16: Uint16Array;
export declare const wasmExports: {
  getUnit: () => number;
  getNil: () => number;
  getTrue: () => number;
  getFalse: () => number;
  writeFloat: (addr: number, value: number) => void;
  readFloat: (addr: number) => number;
};

export enum Tag {
  Int = 0x0,
  Float = 0x1,
  Char = 0x2,
  String = 0x3,
  List = 0x4,
  Tuple2 = 0x5,
  Tuple3 = 0x6,
  Custom = 0x7,
  Record = 0x8,
  Closure = 0x9,
  GcException = 0xa,
  GcStackEmpty = 0xb,
  GcStackPush = 0xc,
  GcStackPop = 0xd,
  GcStackTailCall = 0xe,
  Unused = 0xf
}

export interface Header {
  tag: Tag;
  size: number;
}

export let wasmConstAddrs: ReturnType<typeof initConstAddrs>;
const initConstAddrs = () => {
  const Unit = wasmExports.getUnit();
  const Nil = wasmExports.getNil();
  const True = wasmExports.getTrue();
  const False = wasmExports.getFalse();
  return {
    Unit,
    Nil,
    True,
    False,
    [Unit]: _Utils_Tuple0,
    [Nil]: _List_Nil,
    [True]: true,
    [False]: false
  };
};

export const init = () => {
  wasmConstAddrs = initConstAddrs();
};

export const CLOSURE_N_MASK = 0xffff0000;
export const CLOSURE_MAX_MASK = 0xffff0000;
export const WORD = 4;
export const TAG_MASK = 0xf;
export const SIZE_MASK = 0xfffffff0;
export const SIZE_SHIFT = 4;
