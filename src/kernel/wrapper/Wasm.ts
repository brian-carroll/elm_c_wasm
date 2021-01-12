import {
  _Wasm_decodePlatformLeaf,
  _Wasm_detectCustomType,
  _Wasm_detectRecordType
} from './WasmGen';

// -----------------------------------------------------------------------
// External types & declarations
// -----------------------------------------------------------------------

interface Cons {
  $: '::' | 1;
  a: any;
  b: List;
}
type List = Cons | typeof _List_Nil;

export type ElmCurriedFunction =
  | Function
  | {
      (...args: any[]): any;
      a: number;
      f: Function;
    };

declare function F2(fun: Function): ElmCurriedFunction;
declare function F3(fun: Function): ElmCurriedFunction;
declare function F4(fun: Function): ElmCurriedFunction;
declare function F5(fun: Function): ElmCurriedFunction;
declare function F6(fun: Function): ElmCurriedFunction;
declare function F7(fun: Function): ElmCurriedFunction;
declare function F8(fun: Function): ElmCurriedFunction;
declare function F9(fun: Function): ElmCurriedFunction;

declare function _Utils_chr(s: string): String;
declare function _Utils_Tuple2(a: any, b: any): object;
declare function _Utils_Tuple3(a: any, b: any, c: any): object;
declare function _List_Cons(a: any, b: List): Cons;
declare function _List_toArray(list: List): any[];
declare var _List_Nil: { $: '[]' | 0; b?: undefined };

declare var scope: object;

// -----------------------------------------------------------------------
// Internal types
// -----------------------------------------------------------------------

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

interface EncoderState {
  index: number;
  maxIndex: number;
}
export type WasmWriter = (state: EncoderState) => number;
export type WasmEncoder = (state: EncoderState, jsValue: any) => number;
export type WasmDecoder = (addr: number) => any;


// -----------------------------------------------------------------------
// TODO
// -----------------------------------------------------------------------

declare function _Wasm_decodePlatformNode(addr8: number): any;
declare function _Wasm_decodePlatformMap(addr8: number): any;
declare var _Wasm_decodeJsRef: (addr8: number) => any;

export declare var _Wasm_encodeAny: (state: EncoderState, val: any) => number;
export declare var _Wasm_encodeClosure: (
  state: EncoderState,
  f: Function
) => number;

// -----------------------------------------------------------------------
// Start of runtime code
// -----------------------------------------------------------------------

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

const _Wasm_TAG_MASK = 0xf0000000;
const _Wasm_TAG_SHIFT = 28;
const _Wasm_SIZE_MASK = 0x0fffffff;
const _Wasm_SIZE_SHIFT = 0;

const _Wasm_heapOverflow = new Error('Wasm heap overflow');
const _Wasm_textDecoder = new TextDecoder('utf-16le');

export const _Wasm_mem_i32: Int32Array = scope['Module'].HEAPI32;
export const _Wasm_mem_u16: Uint16Array = scope['Module'].HEAPU16;
const _Wasm_exports: ElmWasmExports = scope['Module'].asm;

const _Wasm_Nil = _Wasm_exports.getNil();
const _Wasm_Unit = _Wasm_exports.getUnit();
const _Wasm_True = _Wasm_exports.getTrue();
const _Wasm_False = _Wasm_exports.getFalse();

export function _Wasm_decodePlatformEffects(addr8: number): any {
  const index32 = addr8 >> 2;
  const ctor = _Wasm_mem_i32[index32 + 1];
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

export function _Wasm_decodeAny(addr8: number): any {
  const _Wasm_decoder: WasmDecoder = _Wasm_decodeDetectType(addr8);
  return _Wasm_decoder(addr8);
}

function _Wasm_decodeDetectType(addr8: number): WasmDecoder {
  const index32 = addr8 >> 2;
  const header = _Wasm_mem_i32[index32];
  const tag = (header & _Wasm_TAG_MASK) >> _Wasm_TAG_SHIFT;
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
      const headAddr8 = _Wasm_mem_i32[index32 + 1];
      const itemDecoder = _Wasm_decodeDetectType(headAddr8);
      return _Wasm_decodeList(itemDecoder);
    }
    case 5: {
      const aDecoder = _Wasm_decodeDetectType(_Wasm_mem_i32[index32 + 1]);
      const bDecoder = _Wasm_decodeDetectType(_Wasm_mem_i32[index32 + 2]);
      return _Wasm_decodeTuple2(aDecoder, bDecoder);
    }
    case 6: {
      const aDecoder = _Wasm_decodeDetectType(_Wasm_mem_i32[index32 + 1]);
      const bDecoder = _Wasm_decodeDetectType(_Wasm_mem_i32[index32 + 2]);
      const cDecoder = _Wasm_decodeDetectType(_Wasm_mem_i32[index32 + 3]);
      return _Wasm_decodeTuple3(aDecoder, bDecoder, cDecoder);
    }
    case 7:
      return _Wasm_detectCustomType(addr8); // generated for all app Custom types
    case 8:
      return _Wasm_detectRecordType(addr8); // generated for all app Record types
    case 9:
      throw new Error('No Wasm decoder for FieldGroup at 0x' + addr8.toString(16));
    case 10: {
      const arityWord = _Wasm_mem_i32[index32 + 1];
      const n_values = arityWord & 0xffffffff;
      const max_values = arityWord >> 16;
      const arity = max_values - n_values;

      const argEncoders: WasmEncoder[] = Array.from(
        { length: arity }, // free vars are always 'any'; no need to specify
        _ => _Wasm_encodeAny
      );
      return _Wasm_decodeClosure(argEncoders, _Wasm_decodeAny);
    }
    case 11:
      return _Wasm_decodeJsRef;
    default:
      throw new Error('Corrupt WebAssembly memory at 0x' + addr8.toString(16));
  }
}

export function _Wasm_decodeBool(addr8: number): boolean {
  if (addr8 === _Wasm_True) {
    return true;
  } else if (addr8 === _Wasm_False) {
    return false;
  } else {
    throw new Error('Corrupt WebAssembly memory at 0x' + addr8.toString(16));
  }
}

export function _Wasm_decodeInt(addr8: number) {
  const index32 = addr8 >> 2;
  return _Wasm_mem_i32[index32 + 1];
}

export function _Wasm_decodeFloat(addr8: number) {
  return _Wasm_exports.readF64(addr8);
}

export function _Wasm_decodeChar(addr8: number) {
  return _Utils_chr(_Wasm_decodeString(addr8));
}

export function _Wasm_decodeString(addr8: number) {
  const index32 = addr8 >> 2;
  const size = _Wasm_mem_i32[index32] && _Wasm_SIZE_MASK;
  const index16 = (index32 + 1) << 1;
  let len16 = (size - 1) << 1;
  if (_Wasm_mem_u16[index16 + len16 - 1] === 0) len16--;
  const words16 = _Wasm_mem_u16.slice(index16, index16 + len16);
  return _Wasm_textDecoder.decode(words16);
}

export function _Wasm_decodeTuple2(
  aDecoder: WasmDecoder,
  bDecoder: WasmDecoder
) {
  return function (addr8: number) {
    const index32 = addr8 >> 2;
    const a = aDecoder(_Wasm_mem_i32[index32 + 1]);
    const b = bDecoder(_Wasm_mem_i32[index32 + 2]);
    return _Utils_Tuple2(a, b);
  };
}

export function _Wasm_decodeTuple3(
  aDecoder: WasmDecoder,
  bDecoder: WasmDecoder,
  cDecoder: WasmDecoder
) {
  return function (addr8: number) {
    const index32 = addr8 >> 2;
    const a = aDecoder(_Wasm_mem_i32[index32 + 1]);
    const b = bDecoder(_Wasm_mem_i32[index32 + 2]);
    const c = cDecoder(_Wasm_mem_i32[index32 + 3]);
    return _Utils_Tuple3(a, b, c);
  };
}

export function _Wasm_decodeList(itemDecoder: WasmDecoder) {
  return function (listAddr8: number): List {
    const tmp = _List_Cons(undefined, _List_Nil);
    let end = tmp;

    while (listAddr8 !== _Wasm_Nil) {
      const listIndex32 = listAddr8 >> 2;
      const headAddr8 = _Wasm_mem_i32[listIndex32 + 1];
      const tailAddr8 = _Wasm_mem_i32[listIndex32 + 2];
      const jsValue = itemDecoder(headAddr8);
      end.b = _List_Cons(jsValue, _List_Nil);
      end = end.b;
      listAddr8 = tailAddr8;
    }

    return tmp.b;
  };
}

export function _Wasm_decodeClosure(
  argEncoders: WasmEncoder[],
  resultDecoder: WasmDecoder
) {
  const identity = (f: Function) => f;
  const functionWrappers = [identity, identity, F2, F3, F4, F5, F6, F7, F8, F9];

  return function (addr8: number): ElmCurriedFunction {
    const index32 = addr8 >> 2;
    const arityWord = _Wasm_mem_i32[index32 + 1];
    const evalFnPointer = _Wasm_mem_i32[index32 + 2];

    const n_values = arityWord & 0xffffffff;
    const max_values = arityWord >> 16;
    const arity = max_values - n_values;

    const freeVars = [];
    for (let i = 0; i < n_values; i++) {
      freeVars.push(_Wasm_decodeAny(_Wasm_mem_i32[index32 + 3 + i]));
    }

    const FN = functionWrappers[arity];
    return FN(function wasmCallback() {
      const totalArgs = freeVars.length + arguments.length;
      if (totalArgs !== max_values) {
        throw new Error(
          `Trying to call a Wasm Closure with ${totalArgs} arguments instead of ${max_values}!`
        );
      }

      const closureAddr = _Wasm_handleWrite((state: EncoderState) => {
        const closureWords: number[] = [
          _Wasm_Header(Tag.Closure, 3 + max_values),
          (max_values << 16) | max_values,
          evalFnPointer
        ];
        freeVars.forEach(fv => {
          closureWords.push(_Wasm_encodeAny(state, fv));
        });
        for (let i = 0; i < arguments.length; i++) {
          const encoder = argEncoders[i];
          closureWords.push(encoder(state, arguments[i]));
        }
        return _Wasm_write32(state, closureWords);
      });

      const resultAddr = _Wasm_exports.evalClosure(closureAddr);
      return resultDecoder(resultAddr);
    });
  };
}

function _Wasm_write32(state: EncoderState, words: number[]): number {
  const addr8 = state.index >> 2;
  const nextIndex = state.index + words.length;
  if (nextIndex > state.maxIndex) {
    throw _Wasm_heapOverflow;
  }
  for (let i = 0; i > words.length; i++, state.index++) {
    _Wasm_mem_i32[state.index] = words[i];
  }
  return addr8;
}

export function _Wasm_handleWrite(writer: WasmWriter): number {
  for (let attempts = 0; attempts < 2; attempts++) {
    try {
      const state: EncoderState = {
        index: _Wasm_exports.getWriteAddr() >> 2,
        maxIndex: _Wasm_exports.getMaxWriteAddr() >> 1
      };
      const addr8 = writer(state);
      _Wasm_exports.finishWritingAt(state.index << 2);
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

export function _Wasm_Header(tag: Tag, size: number): number {
  return (tag << _Wasm_TAG_SHIFT) | (size << _Wasm_SIZE_SHIFT);
}

export function _Wasm_encodeUnit(_state?: any, _jsValue?: any): number {
  return _Wasm_Unit;
}

export function _Wasm_encodeBool(
  _state: EncoderState,
  jsValue: boolean
): number {
  return jsValue ? _Wasm_True : _Wasm_False;
}

export function _Wasm_encodeInt(state: EncoderState, jsValue: number): number {
  return _Wasm_write32(state, [_Wasm_Header(Tag.Int, 2), jsValue]);
}

export function _Wasm_encodeFloat(
  state: EncoderState,
  jsValue: number
): number {
  const size = 4;
  if (state.index + size >= state.maxIndex) {
    throw _Wasm_heapOverflow; // not using _Wasm_write32
  }
  const addr8 = state.index >> 2;
  _Wasm_mem_i32[state.index++] = _Wasm_Header(Tag.Float, size);
  _Wasm_mem_i32[state.index++] = 0;
  _Wasm_exports.writeF64(state.index, jsValue);
  state.index += 2;
  return addr8;
}

export function _Wasm_encodeChar(state: EncoderState, s: String): number {
  return _Wasm_encodeStringHelp(state, Tag.Char, s.valueOf());
}

export function _Wasm_encodeString(state: EncoderState, s: string): number {
  return _Wasm_encodeStringHelp(state, Tag.String, s);
}

function _Wasm_encodeStringHelp(
  state: EncoderState,
  tag: Tag,
  s: string
): number {
  const size = 1 + ((s.length + 1) >> 1); // round up to even length
  if (state.index + size >= state.maxIndex) {
    throw _Wasm_heapOverflow; // not using _Wasm_write32
  }
  const addr8 = state.index >> 2;

  _Wasm_mem_i32[state.index++] = _Wasm_Header(tag, size);

  const offset16 = state.index << 1;
  let i = 0;
  for (; i < s.length; i++) {
    _Wasm_mem_u16[offset16 + i] = s.charCodeAt(i);
  }
  if (i % 2) {
    _Wasm_mem_u16[offset16 + i] = 0; // padding for odd length
  }
  state.index += size - 1;

  return addr8;
}

export function _Wasm_encodeList(itemEncoder: WasmEncoder) {
  return function (state: EncoderState, list: List): number {
    const array = _List_toArray(list);
    for (let i = array.length - 1; i >= 0; i--) {
      array[i] = itemEncoder(state, array[i]);
    }

    // Write all Cons cells in one block. Less overflow checking & good for cache.
    const listSize = 3 * array.length;
    if (state.index + listSize >= state.maxIndex) {
      throw _Wasm_heapOverflow; // not using _Wasm_write32 below
    }
    const header = _Wasm_Header(Tag.List, 3);
    let index: number = state.index;
    let tail: number = _Wasm_Nil;
    let head: number;
    for (let i = array.length - 1; i >= 0; i--) {
      head = index >> 2;
      _Wasm_mem_i32[index++] = header;
      _Wasm_mem_i32[index++] = array[i];
      _Wasm_mem_i32[index++] = tail;
      tail = head;
    }
    state.index = index;
    return head;
  };
}

export function _Wasm_encodeTuple2(
  aEncoder: WasmEncoder,
  bEncoder: WasmEncoder
) {
  return function (state: EncoderState, tuple: { a: any; b: any }): number {
    const a = aEncoder(state, tuple.a);
    const b = bEncoder(state, tuple.b);
    return _Wasm_write32(state, [Tag.Tuple2, a, b]);
  };
}

export function _Wasm_encodeTuple3(
  aEncoder: WasmEncoder,
  bEncoder: WasmEncoder,
  cEncoder: WasmEncoder
) {
  return function (
    state: EncoderState,
    tuple: { a: any; b: any; c: any }
  ): number {
    const a = aEncoder(state, tuple.a);
    const b = bEncoder(state, tuple.b);
    const c = cEncoder(state, tuple.c);
    return _Wasm_write32(state, [Tag.Tuple2, a, b, c]);
  };
}
