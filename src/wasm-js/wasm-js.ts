import './elm-core';
import './generated';
import './wasm';

const elmWasmJs = (function() {
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

  interface Header {
    tag: Tag;
    size: number;
  }

  let wasmConstAddrs: ReturnType<typeof initConstAddrs>;
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

  const initFieldGroups = () => {
    fieldGroupNames.forEach(name => {
      const addr = wasmExports.getNextFieldGroup();
      appTypes.fieldGroups[name] = addr;
      appTypes.fieldGroups[addr] = name;
    });
  };

  const init = () => {
    wasmConstAddrs = initConstAddrs();
    initFieldGroups();
  };

  const CLOSURE_N_MASK = 0xffff0000;
  const CLOSURE_MAX_MASK = 0xffff0000;
  const WORD = 4;
  const TAG_MASK = 0xf;
  const SIZE_MASK = 0xfffffff0;
  const SIZE_SHIFT = 4;

  const textDecoder = new TextDecoder('utf16le');
  const identity = (f: Function) => f;
  const elmFunctionWrappers = [identity, identity, F2, F3, F4];

  function readValue(addr: number): any {
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
          : _List_Cons(
              readValue(mem32[index + 1]),
              readValue(mem32[index + 2])
            );
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
        const freeVars: any[] = [];
        for (let i = index + 3; i < index + size; i++) {
          freeVars.push(readValue(mem32[i]));
        }
        const wasmCallback = createWasmCallback(evaluator, freeVars);
        const FN = elmFunctionWrappers[arity];
        return FN(wasmCallback);
      }
      default:
        throw new Error(
          `Tried to decode value with unsupported tag "${Tag[tag]}"`
        );
    }
  }

  function createWasmCallback(evaluator: number, freeVars: any[]) {
    return function wasmCallback() {
      const requestSpaceInWasm = () => 123; // TODO: request actual space!
      const executeWasmClosure = (addr: number) => 456; // TODO: implement this
      const nArgs = freeVars.length + arguments.length;
      let nextIndex = requestSpaceInWasm();
      let childAddrs: number[] = [];
      const iter = (v: any) => {
        const update = writeValue(nextIndex, v);
        nextIndex = update.nextIndex;
        childAddrs.push(update.addr);
      };
      for (let i = 0; i < freeVars.length; i++) {
        iter(freeVars[i]);
      }
      for (let i = 0; i < arguments.length; i++) {
        iter(arguments[i]);
      }
      const header = encodeHeader({
        tag: Tag.Closure,
        size: 3 + nArgs
      });
      const closureAddr = nextIndex;
      const argsInfo = nArgs * ((1 + 1) << 16);
      mem32[nextIndex++] = header;
      mem32[nextIndex++] = argsInfo;
      mem32[nextIndex++] = evaluator;
      childAddrs.forEach(addr => {
        mem32[nextIndex++] = addr;
      });
      const resultAddr = executeWasmClosure(closureAddr);
      return readValue(resultAddr);
    };
  }

  /**
   * Write a value to the Wasm memory and return the address where it was written
   *
   * Wasm structures all have the same general layout
   *   - header (containing the size in 32-bit words and a type tag)
   *   - body data
   *   - child pointers
   *
   * @param nextIndex Next available index into the memory
   * @param value  JavaScript Elm value to send to Wasm
   * @returns address written and next index to write
   */
  function writeValue(
    nextIndex: number,
    value: any
  ): { addr: number; nextIndex: number } {
    let tag: Tag = Tag.Unused;

    switch (typeof value) {
      case 'number': {
        const isProbablyIntegerButUnsafe = value === (value | 0);
        tag = isProbablyIntegerButUnsafe ? Tag.Int : Tag.Float;
        break;
      }
      case 'string':
        tag = Tag.String;
        break;
      case 'boolean':
        return {
          addr: value ? wasmConstAddrs.True : wasmConstAddrs.False,
          nextIndex
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
            return { addr: wasmConstAddrs.Nil, nextIndex };
          case '::':
            tag = Tag.List;
            break;
          case '#0':
            return { addr: wasmConstAddrs.Unit, nextIndex };
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
    let jsChildren: any[] = [];
    let writer: ((addr: number) => number) | undefined;
    switch (tag) {
      case Tag.Int: {
        body[0] = value;
        break;
      }
      case Tag.Float: {
        writer = (addr: number) => {
          wasmExports.writeFloat(addr, value);
          return 2; // words written
        };
        break;
      }
      case Tag.Char:
      case Tag.String: {
        writer = (addr: number) => writeString(value, addr);
        break;
      }
      case Tag.Tuple2:
      case Tag.List: {
        jsChildren = [value.a, value.b];
        break;
      }
      case Tag.Tuple3: {
        jsChildren = [value.a, value.b, value.c];
        break;
      }
      case Tag.Custom: {
        const jsCtor: string = value.$;
        body[0] = appTypes.ctors[jsCtor];
        Object.keys(value).forEach(k => {
          if (k !== '$') jsChildren.push(value[k]);
        });
        break;
      }
      case Tag.Record: {
        const keys = Object.keys(value);
        keys.sort();
        const fgName = keys.join('$');
        const fgAddr = appTypes.fieldGroups[fgName];
        body[0] = fgAddr;
        jsChildren = keys.map(k => value[k]);
        break;
      }
      case Tag.Closure: {
        break;
      }
    }

    const childAddrs: number[] = [];
    jsChildren.forEach(child => {
      const update = writeValue(nextIndex, child);
      childAddrs.push(update.addr);
      nextIndex = update.nextIndex;
    });

    const currentAddr = nextIndex * WORD;
    if (writer) {
      const size = 1 + writer((nextIndex + 1) * WORD);
      mem32[nextIndex] = encodeHeader({ tag, size });
      nextIndex += size;
    } else {
      const size = body.length + jsChildren.length;
      mem32[nextIndex++] = encodeHeader({ tag, size });
      body.forEach(word => {
        mem32[nextIndex++] = word;
      });
      childAddrs.forEach(addr => {
        mem32[nextIndex++] = addr;
      });
    }

    return {
      addr: currentAddr,
      nextIndex
    };
  }

  function encodeHeader({ tag, size }: Header): number {
    return tag | (size << SIZE_SHIFT);
  }

  function writeString(s: string, idx32: number): number {
    const offset = idx32 << 1;
    const len = s.length + (s.length % 2); // for odd length, write an extra word (gets coerced to 0)
    for (let i = 0; i < len; i++) {
      mem16[offset + i] = s.charCodeAt(i);
    }
    return len >> 1;
  }

  return {
    init,
    readValue,
    writeValue
  };
})();
