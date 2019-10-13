import './elm-core';
declare const wasmInst: WebAssembly.Instance;

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

const wasmConstants = (() => {
  const Unit: number = wasmInst.exports.getUnit();
  const Nil: number = wasmInst.exports.getNil();
  const True: number = wasmInst.exports.getTrue();
  const False: number = wasmInst.exports.getFalse();
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
function encodeHeader(tag: Tag, size: number): number {
  return tag | (size << SIZE_SHIFT);
}

function readValue(memory: Uint32Array, addr: number) {
  const index = addr / WORD;
  const header = memory[index];
  const tag: Tag = header & TAG_MASK;
  const size = (header & SIZE_MASK) >> SIZE_SHIFT;

  switch (tag) {
    case Tag.Int: {
      return memory[index + 1];
    }
    case Tag.Float: {
      return wasmInst.exports.readFloat(addr + WORD);
    }
    case Tag.Char: {
      // blatant monoculturalism for now because this shit is confusing
      return _Utils_chr(String.fromCharCode(memory[index + 1]));
    }
    case Tag.String: {
      let bytes = new Uint8Array(memory.slice(index + 1, size - 1));
      const paddingBytes = bytes[-1];
      bytes = bytes.slice(0, size * WORD - paddingBytes);
      return textDecoder.decode(bytes);
    }
    case Tag.List: {
      return addr === wasmConstants.Nil
        ? _List_Nil
        : _List_Cons(
            readValue(memory, memory[index + 1]),
            readValue(memory, memory[index + 2])
          );
    }
    case Tag.Tuple2: {
      return _Utils_Tuple2(
        readValue(memory, memory[index + 1]),
        readValue(memory, memory[index + 2])
      );
    }
    case Tag.Tuple3: {
      return _Utils_Tuple3(
        readValue(memory, memory[index + 1]),
        readValue(memory, memory[index + 2]),
        readValue(memory, memory[index + 3])
      );
    }
    case Tag.Custom: {
      const elmConst = wasmConstants[addr];
      if (elmConst) return elmConst;
      const wasmCtor = memory[index + 1];
      const jsCtor = wasmCtors[wasmCtor];
      const custom = { $: jsCtor };
      let fieldCharCode = 'a'.charCodeAt(0);
      for (let i = index + 2; i < index + size; i++) {
        const field = String.fromCharCode(fieldCharCode++);
        const childAddr = memory[i];
        custom[field] = readValue(memory, childAddr);
      }
      return custom;
    }
    case Tag.Record: {
      const record = {};
      const fgIndex = memory[index + 1] / WORD;
      const fgSize = memory[fgIndex];
      for (let i = 1; i <= fgSize; i++) {
        const fieldId = memory[fgIndex + i];
        const valAddr = memory[index + 1 + i];
        const fieldName = wasmFields[fieldId];
        const value = readValue(memory, valAddr);
        record[fieldName] = value;
      }
      return record;
    }
    case Tag.Closure: {
      const argsInfo = memory[index + 1];
      const n_values = argsInfo & CLOSURE_N_MASK;
      const max_values = argsInfo & CLOSURE_MAX_MASK;
      const arity = max_values - n_values;
      const evaluator = memory[index + 2];
      const appliedArgs = [];
      for (let i = index + 3; i < index + size; i++) {
        appliedArgs.push(readValue(memory, memory[i]));
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
      throw new Error(`Tried to decode value with tag "${Tag[tag]}"`);
  }
}

/**
 * Write a value to the Wasm memory and return the address where it was written
 * @param memory WebAssembly memory
 * @param maxIndex  Current index into the memory
 * @param value  JavaScript Elm value to send to Wasm
 */
function writeValue(
  memory: Uint32Array,
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
        addr: value ? wasmConstants.True : wasmConstants.False,
        maxIndex
      };
    case 'function':
      tag = Tag.Closure;
      break;
    case 'object': {
      if (value instanceof String) tag = Tag.Char;
      switch (value.$) {
        case undefined:
          tag = Tag.Record;
          break;
        case '[]':
          return { addr: wasmConstants.Nil, maxIndex };
        case '::':
          tag = Tag.List;
          break;
        case '#0':
          return { addr: wasmConstants.Unit, maxIndex };
        case '#2':
          tag = Tag.Tuple2;
          break;
        case '#3':
          tag = Tag.Tuple3;
          break;
        case '#0':
        default:
          tag = Tag.Custom;
      }
    }
  }

  switch (tag) {
    case Tag.Int: {
      const size = 2;
      memory[maxIndex] = encodeHeader(tag, size);
      memory[maxIndex + 1] = value | 0;
      return { addr: maxIndex * WORD, maxIndex: maxIndex + size };
    }
    case Tag.Float: {
      const size = 3;
      memory[maxIndex] = encodeHeader(tag, size);
      wasmInst.exports.writeFloat(maxIndex + 1, value);
      return { addr: maxIndex * WORD, maxIndex: maxIndex + size };
    }
    case Tag.Char: {
      const size = 2;
      memory[maxIndex] = encodeHeader(tag, size);
      const utf16 = [0, 1].map(i => value.charCodeAt(i));
      const encoded32le = utf16[1] * (1 << 16) + utf16[0];
      memory[maxIndex + 1] = encoded32le;
      return { addr: maxIndex * WORD, maxIndex: maxIndex + size };
    }
    case Tag.String: {
      const s: string = value;
      const size = ((s.length + 1) >> 1) + 1;
      memory[maxIndex] = encodeHeader(tag, size);
      const mem16 = new Uint16Array(memory.slice(maxIndex + 1));
      for (let i = 0; i < s.length; i++) {
        mem16[i] = s.charCodeAt(i);
      }
      if (s.length % 2) {
        mem16[s.length] = 2 << 8; // padding size indicator
      }
      return { addr: maxIndex * WORD, maxIndex: maxIndex + size };
    }
    case Tag.Tuple2:
    case Tag.List: {
      const size = 3;
      const a = writeValue(memory, maxIndex, value.a);
      const b = writeValue(memory, maxIndex, value.b);
      const newIndex = b.maxIndex;
      memory[newIndex] = encodeHeader(tag, size);
      memory[newIndex + 1] = a.addr;
      memory[newIndex + 2] = b.addr;
      return { addr: newIndex * WORD, maxIndex: newIndex + size };
    }
    case Tag.Tuple3: {
      const size = 4;
      const a = writeValue(memory, maxIndex, value.a);
      const b = writeValue(memory, maxIndex, value.b);
      const c = writeValue(memory, maxIndex, value.c);
      const newIndex = c.maxIndex;
      memory[newIndex] = encodeHeader(tag, size);
      memory[newIndex + 1] = a.addr;
      memory[newIndex + 2] = b.addr;
      memory[newIndex + 3] = c.addr;
      return { addr: newIndex * WORD, maxIndex: newIndex + size };
    }
    case Tag.Custom: {
      return null;
    }
    case Tag.Record: {
      return null;
    }
    case Tag.Closure: {
      return null;
    }
  }
}
