import {
  mem32,
  mem16,
  wasmExports,
  wasmConstAddrs,
  Tag,
  WORD,
  TAG_MASK,
  SIZE_MASK,
  SIZE_SHIFT,
  CLOSURE_N_MASK,
  CLOSURE_MAX_MASK
} from './shared';
import { appTypes } from './generated';

const textDecoder = new TextDecoder('utf16le');
const identity = (f: Function) => f;
export const elmFunctionWrappers = [identity, identity, F2, F3, F4];

export function readValue(addr: number): any {
  const index = addr / WORD;
  const header = mem32[index];
  const tag: Tag = header & TAG_MASK;
  const size = (header & SIZE_MASK) >> SIZE_SHIFT;

  switch (tag) {
    case Tag.Int: {
      return mem32[index + 1];
    }
    case Tag.Float: {
      return wasmExports.readFloat(addr + WORD);
    }
    case Tag.Char: {
      const idx16 = index << 1;
      const word0 = mem16[idx16 + 1];
      const word1 = mem16[idx16 + 2];
      let s = String.fromCharCode(word0);
      if (word1) s += String.fromCharCode(word1);
      return _Utils_chr(s);
    }
    case Tag.String: {
      let bytes = new Uint8Array(mem32.slice(index + 1, size - 1));
      const paddingBytes = bytes[-1];
      bytes = bytes.slice(0, size * WORD - paddingBytes);
      return textDecoder.decode(bytes);
    }
    case Tag.List: {
      return addr === wasmConstAddrs.Nil
        ? _List_Nil
        : _List_Cons(readValue(mem32[index + 1]), readValue(mem32[index + 2]));
    }
    case Tag.Tuple2: {
      return _Utils_Tuple2(
        readValue(mem32[index + 1]),
        readValue(mem32[index + 2])
      );
    }
    case Tag.Tuple3: {
      return _Utils_Tuple3(
        readValue(mem32[index + 1]),
        readValue(mem32[index + 2]),
        readValue(mem32[index + 3])
      );
    }
    case Tag.Custom: {
      const elmConst = wasmConstAddrs[addr]; // True/False/Unit
      if (elmConst) return elmConst;
      const wasmCtor = mem32[index + 1];
      const jsCtor = appTypes.ctors[wasmCtor];
      const custom: Record<string, any> = { $: jsCtor };
      let fieldCharCode = 'a'.charCodeAt(0);
      for (let i = index + 2; i < index + size; i++) {
        const field = String.fromCharCode(fieldCharCode++);
        const childAddr = mem32[i];
        custom[field] = readValue(childAddr);
      }
      return custom;
    }
    case Tag.Record: {
      const record: Record<string, any> = {};
      const fgIndex = mem32[index + 1] / WORD;
      const fgSize = mem32[fgIndex];
      for (let i = 1; i <= fgSize; i++) {
        const fieldId = mem32[fgIndex + i];
        const valAddr = mem32[index + 1 + i];
        const fieldName = appTypes.fields[fieldId];
        const value = readValue(valAddr);
        record[fieldName] = value;
      }
      return record;
    }
    case Tag.Closure: {
      const argsInfo = mem32[index + 1];
      const n_values = argsInfo & CLOSURE_N_MASK;
      const max_values = argsInfo & CLOSURE_MAX_MASK;
      const arity = max_values - n_values;
      const evaluator = mem32[index + 2];
      const appliedArgs: any[] = [];
      for (let i = index + 3; i < index + size; i++) {
        appliedArgs.push(readValue(mem32[i]));
      }
      function wasmClosureCallback() {
        const privateArgs = appliedArgs.slice();
        for (let arg of arguments) {
          privateArgs.push(arg);
        }
        // write a *full* Closure to memory
        // call exported `apply`
      }
      const FN = elmFunctionWrappers[arity];
      return FN(wasmClosureCallback);
    }
    default:
      throw new Error(
        `Tried to decode value with unsupported tag "${Tag[tag]}"`
      );
  }
}
