import './elm-core';
declare const wasmInst: WebAssembly.Instance;
declare const mem32: Uint32Array;
declare const mem16: Uint16Array;

interface WasmExports {
  getUnit: () => number;
  getNil: () => number;
  getTrue: () => number;
  getFalse: () => number;
  writeFloat: (addr: number, value: number) => void;
  readFloat: (addr: number) => number;
}
declare const wasmExports: WasmExports;

const elmFunctionWrappers = [f => f, f => f, F2, F3, F4];

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
  Closure = 0x9,
  GcException = 0xa,
  GcStackEmpty = 0xb,
  GcStackPush = 0xc,
  GcStackPop = 0xd,
  GcStackTailCall = 0xe,
  Unused = 0xf
}

const wasmFields = {
  thing: 0,
  stuff: 1,
  0: 'thing',
  1: 'stuff'
};

const wasmCtors = {
  0: 'Apple',
  1: 'Banana',
  Apple: 0,
  Banana: 1
};

const wasmConstAddrs = (() => {
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
    [True]: elm$core$Basics$True,
    [False]: elm$core$Basics$False
  };
})();

const textDecoder = new TextDecoder('utf16le');

const CLOSURE_N_MASK = 0xffff0000;
const CLOSURE_MAX_MASK = 0xffff0000;
const WORD = 4;

const TAG_MASK = 0xf;
const SIZE_MASK = 0xfffffff0;
const SIZE_SHIFT = 4;
interface Header {
  tag: Tag;
  size: number;
}
function encodeHeader({ tag, size }: Header): number {
  return tag | (size << SIZE_SHIFT);
}

function readValue(addr: number) {
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
      const elmConst = wasmConstAddrs[addr];
      if (elmConst) return elmConst;
      const wasmCtor = mem32[index + 1];
      const jsCtor = wasmCtors[wasmCtor];
      const custom = { $: jsCtor };
      let fieldCharCode = 'a'.charCodeAt(0);
      for (let i = index + 2; i < index + size; i++) {
        const field = String.fromCharCode(fieldCharCode++);
        const childAddr = mem32[i];
        custom[field] = readValue(childAddr);
      }
      return custom;
    }
    case Tag.Record: {
      const record = {};
      const fgIndex = mem32[index + 1] / WORD;
      const fgSize = mem32[fgIndex];
      for (let i = 1; i <= fgSize; i++) {
        const fieldId = mem32[fgIndex + i];
        const valAddr = mem32[index + 1 + i];
        const fieldName = wasmFields[fieldId];
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
      const appliedArgs = [];
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

const writeString = (s: string) => (idx32: number): number => {
  const offset = idx32 << 1;
  const len = s.length + (s.length % 2); // for odd length, write an extra word (NaN written as 0)
  for (let i = 0; i < len; i++) {
    mem16[offset + i] = s.charCodeAt(i);
  }
  return len >> 1;
};

/**
 * Write a value to the Wasm memory and return the address where it was written
 * @param mem32 WebAssembly memory
 * @param maxIndex  Current index into the memory
 * @param value  JavaScript Elm value to send to Wasm
 * @returns address written and next index to write
 */
function writeValue(
  maxIndex: number,
  value: any
): { addr: number; maxIndex: number } {
  let tag: Tag;

  switch (typeof value) {
    case 'number':
      tag = Number.isInteger(value) ? Tag.Int : Tag.Float; // UNSAFE!
      break;
    case 'string':
      tag = Tag.String;
      break;
    case 'boolean':
      return {
        addr: value ? wasmConstAddrs.True : wasmConstAddrs.False,
        maxIndex
      };
    case 'function':
      tag = Tag.Closure;
      break;
    case 'object': {
      if (value instanceof String) {
        tag = Tag.Char;
        break;
      }
      switch (value.$) {
        case undefined:
          tag = Tag.Record;
          break;
        case '[]':
          return { addr: wasmConstAddrs.Nil, maxIndex };
        case '::':
          tag = Tag.List;
          break;
        case '#0':
          return { addr: wasmConstAddrs.Unit, maxIndex };
        case '#2':
          tag = Tag.Tuple2;
          break;
        case '#3':
          tag = Tag.Tuple3;
          break;
        case '#0':
        default:
          tag = Tag.Custom;
          break;
      }
    }
  }

  let body: number[] = [];
  let children: any[] = [];
  let writer: (addr: number) => number;
  switch (tag) {
    case Tag.Int: {
      body[0] = value | 0;
      break;
    }
    case Tag.Float: {
      writer = addr => {
        wasmExports.writeFloat(addr, value);
        return 2; // words written
      };
      break;
    }
    case Tag.Char:
    case Tag.String: {
      writer = writeString(value);
      break;
    }
    case Tag.Tuple2:
    case Tag.List: {
      children = [value.a, value.b];
      break;
    }
    case Tag.Tuple3: {
      children = [value.a, value.b, value.c];
      break;
    }
    case Tag.Custom: {
      body[0] = wasmCtors[value.$];
      Object.keys(value).forEach(k => {
        if (k !== '$') children.push(value[k]);
      });
      break;
    }
    case Tag.Record: {
      break;
    }
    case Tag.Closure: {
      break;
    }
  }

  let index = maxIndex;

  const childAddrs: number[] = [];
  children.forEach(child => {
    const update = writeValue(index, child);
    childAddrs.push(update.addr);
    index = update.maxIndex;
  });

  const currentAddr = index * WORD;
  if (writer) {
    const size = 1 + writer((index + 1) * WORD);
    mem32[index] = encodeHeader({ tag, size });
    index += size;
  } else {
    const size = body.length + children.length;
    mem32[index++] = encodeHeader({ tag, size });
    body.forEach(word => {
      mem32[index++] = word;
    });
    childAddrs.forEach(addr => {
      mem32[index++] = addr;
    });
  }

  return {
    addr: currentAddr,
    maxIndex: index
  };
}
