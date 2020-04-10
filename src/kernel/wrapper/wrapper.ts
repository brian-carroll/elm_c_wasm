/// <reference path = "globals.d.ts" />

/**
 * Functions exported from the Wasm module
 */
interface ElmWasmExports {
  _getNextMain: () => number;
  _getUnit: () => number;
  _getNil: () => number;
  _getTrue: () => number;
  _getFalse: () => number;
  _getNextFieldGroup: () => number;
  _getMaxWriteAddr: () => number;
  _getWriteAddr: () => number;
  _finishWritingAt: (addr: number) => void;
  _readF64: (addr: number) => number;
  _writeF64: (addr: number, value: number) => void;
  _evalClosure: (addr: number) => number;
  _collectGarbage: () => void;
  _debugHeapState: () => void;
}

/**
 * App-specific type info known to Elm compiler
 */
interface GeneratedAppTypes {
  ctors: string[];
  fields: string[];
  fieldGroups: string[];
}

/*********************************************************************************************
 * `wrapWasmElmApp`
 *
 * Wrap a WebAssembly instance and connect it to Elm's kernel JavaScript.
 * WebAssembly doesn't support Web APIs directly yet, so we need JS to implement most effects.
 *
 * After compiling TS to JS, this function definition must be inserted into Elm's generated JS.
 * That requires a build step for now - I'm using Makefiles and Bash scripts - but eventually,
 * the Elm compiler will do it.
 *
 * @param wasmBuffer         The `ArrayBuffer` memory block shared between JS and Wasm
 * @param wasmExports        Object of exported functions from the Wasm module
 * @param generatedAppTypes  App-specific type info passed from Elm compiler to this wrapper
 * @param kernelFunctions    Array of all JS kernel functions called by the Elm Wasm module
 *
 /********************************************************************************************/
function wrapWasmElmApp(
  wasmBuffer: ArrayBuffer,
  wasmExports: ElmWasmExports,
  generatedAppTypes: GeneratedAppTypes,
  kernelFunctions: Function[]
) {
  if (!(wasmBuffer instanceof ArrayBuffer))
    throw new Error('Expected wasmMemory to be an ArrayBuffer');

  /* --------------------------------------------------

               INITIALISATION & CONSTANTS

  -------------------------------------------------- */

  const mem32 = new Uint32Array(wasmBuffer);
  const mem16 = new Uint16Array(wasmBuffer);

  const wasmConstAddrs = (function () {
    const Unit = wasmExports._getUnit();
    const Nil = wasmExports._getNil();
    const True = wasmExports._getTrue();
    const False = wasmExports._getFalse();
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
  })();

  /**
   * Enum objects for app-specific type information
   * Convert from arrays to objects so that we get
   * fast mapping both ways, JS->Wasm and Wasm->JS
   */
  interface NameToInt {
    [name: string]: number;
  }
  interface IntToName {
    [int: number]: string;
  }
  interface AppTypes {
    ctors: NameToInt & IntToName;
    fields: NameToInt & IntToName;
    fieldGroups: NameToInt & IntToName;
  }

  const appTypes: AppTypes = {
    ctors: arrayToEnum(generatedAppTypes.ctors),
    fields: arrayToEnum(generatedAppTypes.fields),
    fieldGroups: generatedAppTypes.fieldGroups.reduce(
      (enumObj: NameToInt & IntToName, name) => {
        const addr = wasmExports._getNextFieldGroup();
        enumObj[name] = addr;
        enumObj[addr] = name;
        return enumObj;
      },
      {}
    )
  };

  function arrayToEnum(names: string[]): NameToInt & IntToName {
    return names.reduce((enumObj: NameToInt & IntToName, name, index) => {
      enumObj[name] = index;
      enumObj[index] = name;
      return enumObj;
    }, {});
  }

  const WORD = 4;
  const TAG_MASK = 0xf0000000;
  const TAG_SHIFT = 28;
  const SIZE_MASK = 0x0fffffff;
  const SIZE_SHIFT = 0;
  const NEVER_EVALUATE = 0xffff;

  const textDecoder = new TextDecoder('utf-16le');
  const identity = (f: Function) => f;
  const elmFunctionWrappers = [identity, identity, F2, F3, F4];

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

  /* --------------------------------------------------

                READ ELM VALUE FROM WASM

  -------------------------------------------------- */

  interface ClosureMetadata {
    n_values: number;
    max_values: number;
    evaluator: number;
    argsIndex: number;
  }

  function readWasmValue(addr: number): any {
    const index = addr >> 2;
    const header = mem32[index];
    const tag: Tag = (header & TAG_MASK) >>> TAG_SHIFT;
    const size = (header & SIZE_MASK) >>> SIZE_SHIFT;

    switch (tag) {
      case Tag.Int: {
        return mem32[index + 1];
      }
      case Tag.Float: {
        return wasmExports._readF64(addr + 2 * WORD);
      }
      case Tag.Char:
      case Tag.String: {
        const idx16 = (index + 1) << 1;
        let len16 = (size - 1) << 1;
        if (mem16[idx16 + len16 - 1] === 0) len16--;
        const words16 = mem16.slice(idx16, idx16 + len16);
        const jsString = textDecoder.decode(words16);
        return tag === Tag.Char ? _Utils_chr(jsString) : jsString;
      }
      case Tag.List: {
        return addr === wasmConstAddrs.Nil
          ? _List_Nil
          : _List_Cons(
              readWasmValue(mem32[index + 1]),
              readWasmValue(mem32[index + 2])
            );
      }
      case Tag.Tuple2: {
        return _Utils_Tuple2(
          readWasmValue(mem32[index + 1]),
          readWasmValue(mem32[index + 2])
        );
      }
      case Tag.Tuple3: {
        return _Utils_Tuple3(
          readWasmValue(mem32[index + 1]),
          readWasmValue(mem32[index + 2]),
          readWasmValue(mem32[index + 3])
        );
      }
      case Tag.Custom: {
        const elmConst = wasmConstAddrs[addr]; // True/False/Unit
        if (elmConst !== undefined) return elmConst;
        const wasmCtor = mem32[index + 1];
        const jsCtor = appTypes.ctors[wasmCtor];
        const custom: Record<string, any> = { $: jsCtor };
        const fieldNames = 'abcdefghijklmnopqrstuvwxyz';
        const nFields = size - 2;
        for (let i = 0; i < nFields; i++) {
          const field = fieldNames[i];
          const childAddr = mem32[index + 2 + i];
          custom[field] = readWasmValue(childAddr);
        }
        return custom;
      }
      case Tag.Record: {
        const record: Record<string, any> = {};
        const fgIndex = mem32[index + 1] >> 2;
        const fgSize = mem32[fgIndex];
        const fields = appTypes.fields;
        for (let i = 1; i <= fgSize; i++) {
          const fieldId = mem32[fgIndex + i];
          const valAddr = mem32[index + 1 + i];
          const fieldName = fields[fieldId];
          const value = readWasmValue(valAddr);
          record[fieldName] = value;
        }
        return record;
      }
      case Tag.Closure: {
        const idx16 = index << 1;
        const metadata: ClosureMetadata = {
          n_values: mem16[idx16 + 2],
          max_values: mem16[idx16 + 3],
          evaluator: mem32[index + 2],
          argsIndex: index + 3
        };
        return metadata.max_values === NEVER_EVALUATE
          ? evalKernelThunk(metadata)
          : createWasmCallback(metadata);
      }
      default:
        throw new Error(
          'Tried to decode value with unsupported tag ' +
            (Tag[tag] || '0x' + tag.toString(16))
        );
    }
  }

  function evalKernelThunk(metadata: ClosureMetadata): any {
    const { n_values, evaluator, argsIndex } = metadata;
    let kernelValue = kernelFunctions[evaluator];
    for (let i = argsIndex; i < argsIndex + n_values; i++) {
      const arg = readWasmValue(mem32[i]);
      kernelValue = kernelValue(arg);
    }
    return kernelValue as any;
  }

  function createWasmCallback(metadata: ClosureMetadata): Function {
    const { n_values, max_values, evaluator, argsIndex } = metadata;
    const freeVars: any[] = [];
    for (let i = argsIndex; i < argsIndex + n_values; i++) {
      freeVars.push(readWasmValue(mem32[i]));
    }
    function wasmCallback() {
      const args = freeVars.slice();
      for (let i = 0; i < arguments.length; i++) {
        args.push(arguments[i]);
      }
      const n_values = args.length;
      if (n_values !== max_values) {
        console.error({ wasmCallback, args });
        throw new Error(
          `Trying to call a Wasm Closure with ${n_values} args instead of ${max_values}!`
        );
      }
      const builder: WasmBuilder = {
        body: [(max_values << 16) | n_values, evaluator],
        jsChildren: args,
        bodyWriter: null
      };
      const addr = handleWasmWrite((startIndex: number) => {
        return writeFromBuilder(startIndex, builder, Tag.Closure);
      });
      const resultAddr = wasmExports._evalClosure(addr);
      const resultValue = readWasmValue(resultAddr);
      return resultValue;
    }
    // Attach info in case we have to write this Closure back to Wasm
    wasmCallback.freeVars = freeVars;
    wasmCallback.evaluator = evaluator;
    wasmCallback.max_values = max_values;
    const arity = max_values - n_values;
    const FN = elmFunctionWrappers[arity];
    return FN(wasmCallback);
  }

  /* --------------------------------------------------

                WRITE ELM VALUES TO WASM

  -------------------------------------------------- */

  let maxWriteIndex32: number;
  let maxWriteIndex16: number;
  const heapOverflowError = new Error('Wasm heap overflow');

  function write32(index: number, value: number) {
    if (index > maxWriteIndex32) {
      throw heapOverflowError;
    }
    mem32[index] = value;
  }

  function write16(index: number, value: number) {
    if (index > maxWriteIndex16) {
      throw heapOverflowError;
    }
    mem16[index] = value;
  }

  interface WriteResult {
    addr: number; // address the value was written to (8-bit resolution, handy for pointers)
    nextIndex: number; // next available index for writing (32-bit resolution, handy for alignment)
  }

  function handleWasmWrite(writer: (nextIndex: number) => WriteResult): number {
    for (let attempts = 0; attempts < 2; attempts++) {
      try {
        const maxAddr = wasmExports._getMaxWriteAddr();
        maxWriteIndex16 = maxAddr >> 1;
        maxWriteIndex32 = maxAddr >> 2;
        const startAddr = wasmExports._getWriteAddr();
        const startIndex = startAddr >> 2;
        const result: WriteResult = writer(startIndex);
        wasmExports._finishWritingAt(result.nextIndex << 2);
        return result.addr;
      } catch (e) {
        if (e === heapOverflowError) {
          wasmExports._collectGarbage();
        } else {
          console.error(e);
          throw e;
        }
      }
    }
    throw new Error('Failed to write to Wasm');
  }

  /**
   * Write an Elm value to the Wasm memory
   * Serialises to bytes before writing
   * May throw an error
   */
  function writeWasmValue(nextIndex: number, value: any): WriteResult {
    if (value == null) {
      return { addr: 0, nextIndex };
    }
    const typeInfo: TypeInfo = detectElmType(value);
    if (typeInfo.kind === 'constAddr') {
      return { addr: typeInfo.value, nextIndex };
    }
    const tag: Tag = typeInfo.value;
    const builder: WasmBuilder = wasmBuilder(tag, value);
    return writeFromBuilder(nextIndex, builder, tag);
  }

  // Info needed to build any Elm Wasm value
  // The shape is always the same, which helps JS engines to optimise
  type WasmBuilder =
    | {
        body: [];
        jsChildren: [];
        bodyWriter: (bodyAddr: number) => number;
      }
    | {
        body: number[];
        jsChildren: any[];
        bodyWriter: null;
      };

  type TypeInfo =
    | {
        kind: 'tag';
        value: Tag;
      }
    | {
        kind: 'constAddr';
        value: number;
      };

  function detectElmType(elmValue: any): TypeInfo {
    switch (typeof elmValue) {
      case 'number': {
        // There's no way to tell `1 : Int` from `1.0 : Float` at this low level. But `1.2` is definitely a Float.
        // So _for now_ take a _horribly unsafe_ guess, by checking if it's a round number or not.
        // Not cool. This is Elm! Long term, the ambiguity needs to be solved at some higher level.
        // Maybe some lib like `JSON.Encode` so the app dev decides? Pity for it not to be automatic though!
        const isRoundNumberSoGuessInt = elmValue === Math.round(elmValue);
        return {
          kind: 'tag',
          value: isRoundNumberSoGuessInt ? Tag.Int : Tag.Float
        };
      }
      case 'string':
        return { kind: 'tag', value: Tag.String };

      case 'boolean':
        return {
          kind: 'constAddr',
          value: elmValue ? wasmConstAddrs.True : wasmConstAddrs.False
        };

      case 'function':
        return { kind: 'tag', value: Tag.Closure };

      case 'object': {
        if (elmValue instanceof String) {
          return { kind: 'tag', value: Tag.Char };
        }
        switch (elmValue.$) {
          case undefined:
            return { kind: 'tag', value: Tag.Record };
          case '[]':
            return { kind: 'constAddr', value: wasmConstAddrs.Nil };
          case '::':
            return { kind: 'tag', value: Tag.List };
          case '#0':
            return { kind: 'constAddr', value: wasmConstAddrs.Unit };
          case '#2':
            return { kind: 'tag', value: Tag.Tuple2 };
          case '#3':
            return { kind: 'tag', value: Tag.Tuple3 };
          default:
            return { kind: 'tag', value: Tag.Custom };
        }
      }
    }
    console.error(elmValue);
    throw new Error('Cannot determine type of Elm value');
  }

  function wasmBuilder(tag: Tag, value: any): WasmBuilder {
    switch (tag) {
      case Tag.Int:
        return {
          body: [value],
          jsChildren: [],
          bodyWriter: null
        };
      case Tag.Float:
        return {
          body: [],
          jsChildren: [],
          bodyWriter: (bodyAddr: number) => {
            write32(bodyAddr >> 2, 0);
            const afterPadding = bodyAddr + WORD;
            wasmExports._writeF64(afterPadding, value);
            return 3; // words written
          }
        };
      case Tag.Char:
      case Tag.String:
        return {
          body: [],
          jsChildren: [],
          bodyWriter: (bodyAddr: number) => {
            const s: string = value;
            const offset16 = bodyAddr >> 1;
            const lenAligned = s.length + (s.length % 2); // for odd length, write an extra word (gets coerced to 0)
            for (let i = 0; i < lenAligned; i++) {
              write16(offset16 + i, s.charCodeAt(i));
            }
            const wordsWritten = lenAligned >> 1;
            return wordsWritten;
          }
        };
      case Tag.Tuple2:
      case Tag.List:
        return {
          body: [],
          jsChildren: [value.a, value.b],
          bodyWriter: null
        };
      case Tag.Tuple3:
        return {
          body: [],
          jsChildren: [value.a, value.b, value.c],
          bodyWriter: null
        };
      case Tag.Custom: {
        const jsCtor: string = value.$;
        const jsChildren: any[] = [];
        Object.keys(value).forEach(k => {
          if (k !== '$') jsChildren.push(value[k]);
        });
        return {
          body: [appTypes.ctors[jsCtor]],
          jsChildren,
          bodyWriter: null
        };
      }
      case Tag.Record: {
        // JS doesn't have the concept of fieldgroups but Wasm does.
        // It's a structure containing info about a specific Record type
        // Need to look it up in the appTypes.
        const keys = Object.keys(value);
        keys.sort();
        const fgName = keys.join('$');
        const fgAddr = appTypes.fieldGroups[fgName];
        return {
          body: [fgAddr],
          jsChildren: keys.map(k => value[k]),
          bodyWriter: null
        };
      }
      case Tag.Closure: {
        const fun = value.f || value;
        if (fun.evaluator) {
          const { freeVars, max_values, evaluator } = fun;
          const n_values = freeVars.length;
          return {
            body: [(max_values << 16) | n_values, evaluator],
            jsChildren: freeVars,
            bodyWriter: null
          };
        } else {
          let evaluator = kernelFunctions.findIndex(f => f === value);
          if (evaluator === -1) {
            kernelFunctions.push(value);
            evaluator = kernelFunctions.length - 1;
          }
          return {
            body: [NEVER_EVALUATE << 16, evaluator],
            jsChildren: [],
            bodyWriter: null
          };
        }
      }
    }
    console.error(value);
    throw new Error(`Can't write to WebAssembly for tag "${Tag[tag]}"`);
  }

  function writeFromBuilder(
    nextIndex: number,
    builder: WasmBuilder,
    tag: Tag
  ): WriteResult {
    if (builder.bodyWriter) {
      /**
       * Special cases: Float (64-bit data) or String/Char (16-bit data)
       */
      const headerIndex = nextIndex;
      const addr = headerIndex << 2;
      const wordsWritten = builder.bodyWriter(addr + WORD);
      const size = 1 + wordsWritten;
      write32(headerIndex, encodeHeader(tag, size));
      return {
        addr,
        nextIndex: headerIndex + size
      };
    } else {
      /**
       * Normal cases (32-bit data)
       */
      const { body, jsChildren } = builder;
      const childAddrs: number[] = [];
      jsChildren.forEach(child => {
        const update = writeWasmValue(nextIndex, child); // recurse
        childAddrs.push(update.addr);
        nextIndex = update.nextIndex;
      });

      const addr = nextIndex << 2;
      const size = 1 + body.length + childAddrs.length;
      write32(nextIndex++, encodeHeader(tag, size));
      body.forEach(word => {
        write32(nextIndex++, word);
      });
      childAddrs.forEach(pointer => {
        write32(nextIndex++, pointer);
      });
      return { addr, nextIndex };
    }
  }

  function encodeHeader(tag: Tag, size: number): number {
    return (tag << TAG_SHIFT) | (size << SIZE_SHIFT);
  }

  /* --------------------------------------------------

                    EXPORTS

  -------------------------------------------------- */

  const mains: any[] = [];
  while (true) {
    const mainAddr = wasmExports._getNextMain();
    if (!mainAddr) break;
    mains.push(readWasmValue(mainAddr));
  }

  return {
    mains,
    // functions for testing
    readWasmValue,
    writeWasmValue: (value: any) =>
      handleWasmWrite(nextIndex => writeWasmValue(nextIndex, value))
  };
}
