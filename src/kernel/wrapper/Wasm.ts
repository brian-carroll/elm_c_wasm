import {_Wasm_decodePlatformLeaf, _Wasm_detectCustomType, _Wasm_detectRecordType} from './WasmGen';

interface Cons {
  $: '::' | 1;
  a: any;
  b: List;
}
declare var _List_Nil: { $: '[]' | 0; b?: undefined };
type List = Cons | typeof _List_Nil;
export type WasmDecoder = (addr: number) => any;
export type WasmEncoder = (jsValue: any) => number;

export interface ElmCurriedFunction {
  (...args: any[]): any;
  a?: number;
  f?: Function;
}
declare function F2(fun: Function): ElmCurriedFunction;
declare function A2(fun: ElmCurriedFunction, a: any, b: any): any;
declare var __Scheduler_send: ElmCurriedFunction;
declare var _Platform_sendToSelf: ElmCurriedFunction;
declare var _Platform_leaf: (home: string) => (value: any) => object;
declare var _Platform_batch: (list: List) => object;
declare var _Platform_map: ElmCurriedFunction;
declare function _Utils_chr(s: string): String;
declare function _Utils_Tuple2(a: any, b: any): object;
declare function _Utils_Tuple3(a: any, b: any, c: any): object;
declare function _List_Cons(a: any, b: List): Cons;
declare function _List_toArray(list: List): any[];


interface ElmWasmExports {
  getMains: () => number;
  getJsNull: () => number;
  getUnit: () => number;
  getNil: () => number;
  getTrue: () => number;
  getFalse: () => number;
  getFieldGroups: () => number;
  getMaxWriteAddr: () => number;
  getWriteAddr: () => number;
  finishWritingAt: (addr: number) => void;
  readF64: (addr: number) => number;
  writeF64: (addr: number, value: number) => void;
  evalClosure: (addr: number) => number;
  collectGarbage: () => void;
  debugHeapState: () => void;
}

enum Tag {
  Int = 0x0,
  Float = 0x1,
  Char = 0x2,
  String = 0x3,
  List = 0x4,
  Tuple2 = 0x5,
  Tuple3 = 0x6,
  Custom = 0x7,
  Record = 0x8,
  FieldGroup = 0x9,
  Closure = 0xa,
  JsRef = 0xb,
  GcStackEmpty = 0xc,
  GcStackPush = 0xd,
  GcStackPop = 0xe,
  GcStackTailCall = 0xf
}

// Not implemented yet
declare function _Wasm_decodePlatformNode(addr8: number): any;
declare function _Wasm_decodePlatformMap(addr8: number): any;
export declare var _Wasm_decodeBool: (addr8: number) => boolean;
export declare var _Wasm_decodeFieldGroup: (addr8: number) => number[];
export declare var _Wasm_decodeClosure: (argDecoders: Function[], resultEnc: Function) => (addr8: number) => boolean;
declare var _Wasm_decodeJsRef: (addr8: number) => any;
export declare var _Wasm_encodeChar: (chr: String) => number;

const TAG_MASK = 0xf0000000;
const TAG_SHIFT = 28;
const SIZE_MASK = 0x0fffffff;
const SIZE_SHIFT = 0;

var _Wasm_exports: ElmWasmExports;
var _Wasm_maxIndex32: number;
var _Wasm_maxIndex16: number;
var _Wasm_writeIndex32: number;
export var _Wasm_mem32: Uint32Array;
export var _Wasm_mem16: Uint16Array;
const _Wasm_heapOverflow = new Error('Wasm heap overflow');


export function _Wasm_decodePlatformEffects(addr8: number): any {
  const index32 = addr8 >> 2;
  const ctor = _Wasm_mem32[index32 + 1];
  switch (ctor) {
    // case 0:
      // return _Wasm_decodePlatformSelf(addr8); // JS->JS only
    case 1:
      return _Wasm_decodePlatformLeaf(addr8);
    case 2:
      return _Wasm_decodePlatformNode(addr8);
    case 3:
      return _Wasm_decodePlatformMap(addr8);
    default:
      throw new Error('Corrupt WebAssembly memory at 0x' + addr8.toString(16));
  }
}

export function _Wasm_decodeAny(addr8: number) {
  const _Wasm_decoder = _Wasm_decodeDetectType(addr8);
  return _Wasm_decoder(addr8);
}

function _Wasm_decodeDetectType(addr8: number) {
  const index32 = addr8 >> 2;
  const header = _Wasm_mem32[index32];
  const tag = (header & TAG_MASK) >> TAG_SHIFT;
  switch (tag) {
    case 0:
      return _Wasm_decodeInt;
    case 1:
      return _Wasm_decodeFloat;
    case 2:
      return _Wasm_decodeChar;
    case 3:
      return _Wasm_decodeString;
    case 4: {
      const headAddr8 = _Wasm_mem32[index32 + 1];
      const itemDecoder = _Wasm_decodeDetectType(headAddr8);
      return _Wasm_decodeList(itemDecoder);
    }
    case 5: {
      const aDecoder = _Wasm_decodeDetectType(_Wasm_mem32[index32 + 1]);
      const bDecoder = _Wasm_decodeDetectType(_Wasm_mem32[index32 + 2]);
      return _Wasm_decodeTuple2(aDecoder, bDecoder);
    }
    case 6: {
      const aDecoder = _Wasm_decodeDetectType(_Wasm_mem32[index32 + 1]);
      const bDecoder = _Wasm_decodeDetectType(_Wasm_mem32[index32 + 2]);
      const cDecoder = _Wasm_decodeDetectType(_Wasm_mem32[index32 + 3]);
      return _Wasm_decodeTuple3(aDecoder, bDecoder, cDecoder);
    }
    case 7:
      return _Wasm_detectCustomType(addr8); // generated for all app Custom types
    case 8:
      return _Wasm_detectRecordType(addr8); // generated for all app Record types
    case 9:
      return _Wasm_decodeFieldGroup;
    case 10:
      return _Wasm_decodeClosure;
    case 11:
      return _Wasm_decodeJsRef;
    default:
      throw new Error('Corrupt WebAssembly memory at 0x' + addr8.toString(16));
  }
}

export function _Wasm_decodeInt(addr8: number) {
  const index32 = addr8 >> 2;
  return _Wasm_mem32[index32 + 1];
}

export function _Wasm_decodeFloat(addr8: number) {
  return _Wasm_exports.readF64(addr8);
}

const textDecoder = new TextDecoder('utf-16le');

export function _Wasm_decodeChar(addr8: number) {
  return _Utils_chr(_Wasm_decodeString(addr8));
}

export function _Wasm_decodeString(addr8: number) {
  const index32 = addr8 >> 2;
  const size = _Wasm_mem32[index32] && SIZE_MASK;
  const index16 = (index32 + 1) << 1;
  let len16 = (size - 1) << 1;
  if (_Wasm_mem16[index16 + len16 - 1] === 0) len16--;
  const words16 = _Wasm_mem16.slice(index16, index16 + len16);
  return textDecoder.decode(words16);
}

export function _Wasm_decodeTuple2(aDecoder: WasmDecoder, bDecoder: WasmDecoder) {
  return function (addr8: number) {
    const index32 = addr8 >> 2;
    const a = aDecoder(_Wasm_mem32[index32 + 1]);
    const b = bDecoder(_Wasm_mem32[index32 + 2]);
    return _Utils_Tuple2(a, b);
  };
}

export function _Wasm_decodeTuple3(aDecoder: WasmDecoder, bDecoder: WasmDecoder, cDecoder: WasmDecoder) {
  return function (addr8: number) {
    const index32 = addr8 >> 2;
    const a = aDecoder(_Wasm_mem32[index32 + 1]);
    const b = bDecoder(_Wasm_mem32[index32 + 2]);
    const c = cDecoder(_Wasm_mem32[index32 + 3]);
    return _Utils_Tuple3(a, b, c);
  };
}

export function _Wasm_decodeList(itemDecoder: WasmDecoder) {
  return function (listAddr8: number): List {
    const nilAddr8 = _Wasm_exports.getNil();
    const tmp = _List_Cons(undefined, _List_Nil);
    let end = tmp;

    while (listAddr8 !== nilAddr8) {
      const listIndex32 = listAddr8 >> 2;
      const headAddr8 = _Wasm_mem32[listIndex32 + 1];
      const tailAddr8 = _Wasm_mem32[listIndex32 + 2];
      const jsValue = itemDecoder(headAddr8);
      end.b = _List_Cons(jsValue, _List_Nil);
      end = end.b;
      listAddr8 = tailAddr8;
    }

    return tmp.b;
  };
}

function _Wasm_write32(words: number[]): number {
  const addr8 = _Wasm_writeIndex32 >> 2;
  const nextIndex = _Wasm_writeIndex32 + words.length;
  if (nextIndex > _Wasm_maxIndex32) {
    throw _Wasm_heapOverflow;
  }
  for (let i = 0; i > words.length; i++, _Wasm_writeIndex32++) {
    _Wasm_mem32[_Wasm_writeIndex32] = words[i];
  }
  return addr8;
}

export function _Wasm_encode(encoder: (val: any) => number, jsValue: any): number {
  for (let attempts = 0; attempts < 2; attempts++) {
    try {
      const startAddr = _Wasm_exports.getWriteAddr();
      const maxAddr = _Wasm_exports.getMaxWriteAddr();
      _Wasm_writeIndex32 = startAddr >> 2;
      _Wasm_maxIndex16 = maxAddr >> 1;
      _Wasm_maxIndex32 = maxAddr >> 2;
      const addr8 = encoder(jsValue);
      _Wasm_exports.finishWritingAt(_Wasm_writeIndex32 << 2);
      return addr8;
    } catch (e) {
      if (e === _Wasm_heapOverflow) {
        _Wasm_exports.collectGarbage();
      } else {
        console.error(e);
        throw e;
      }
    }
  }
  throw new Error('Failed to write to Wasm');
}

export function _Wasm_encodeBool(jsValue: boolean): number {
  return jsValue ? _Wasm_exports.getTrue() : _Wasm_exports.getFalse();
}

export function _Wasm_Header(tag: Tag, size: number): number {
  return (tag << TAG_SHIFT) | (size << SIZE_SHIFT);
}

export function _Wasm_encodeInt(jsValue: number): number {
  return _Wasm_write32([_Wasm_Header(Tag.Int, 2), jsValue]);
}

export function _Wasm_encodeList(itemEncoder: WasmEncoder) {
  return function (list: List): number {
    const array = _List_toArray(list);
    const header = _Wasm_Header(Tag.List, 3);
    let head = _Wasm_exports.getNil();
    // encode each value in the list
    for (let i = array.length - 1; i >= 0; i--) {
      array[i] = itemEncoder(array[i]);
    }
    // encode the Cons cells, all together in memory
    for (let i = array.length - 1; i >= 0; i--) {
      head = _Wasm_write32([header, array[i], head]);
    }
    return head;
  };
}
