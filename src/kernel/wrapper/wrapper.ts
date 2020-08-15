/// <reference path = "globals.d.ts" />

/**
 * Functions exported from the Wasm module
 */
interface ElmWasmExports {
  _getNextMain: () => number;
  _getJsNull: () => number;
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

interface ElmCurriedFunction {
  (...args: any[]): any;
  a: number;
  f: Function;
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
 * @param kernelFuncRecord   Record of all JS kernel functions called by the Elm Wasm module
 *
 /********************************************************************************************/
function wrapWasmElmApp(
  wasmBuffer: ArrayBuffer,
  wasmExports: ElmWasmExports,
  generatedAppTypes: GeneratedAppTypes,
  kernelFuncRecord: Record<string, ElmCurriedFunction>
) {
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
    const JsNull = wasmExports._getJsNull();
    return {
      Unit,
      Nil,
      True,
      False,
      JsNull,
      [Unit]: _Utils_Tuple0,
      [Nil]: _List_Nil,
      [True]: true,
      [False]: false,
      [JsNull]: null
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
  interface IntToNames {
    [int: number]: string[];
  }
  interface AppTypes {
    ctors: NameToInt & IntToName;
    fields: NameToInt & IntToName;
    fieldGroups: NameToInt & IntToNames;
  }

  const KERNEL_CTOR_OFFSET = 1024 * 1000;
  const JSON_CTOR_OFFSET = 0x2000 * 10000;
  enum JsonValue {
    WRAP = JSON_CTOR_OFFSET,
    NULL = JSON_CTOR_OFFSET + 1,
    OBJECT = JSON_CTOR_OFFSET + 2,
    ARRAY = JSON_CTOR_OFFSET + 3
  }

  const appTypes: AppTypes = {
    ctors: arrayToEnum(generatedAppTypes.ctors),
    fields: arrayToEnum(generatedAppTypes.fields),
    fieldGroups: generatedAppTypes.fieldGroups.reduce(
      (enumObj: NameToInt & IntToNames, name) => {
        const addr = wasmExports._getNextFieldGroup();
        enumObj[name] = addr;
        enumObj[addr] = name.split(' ');
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

  const kernelFunctions = Object.values(kernelFuncRecord);
  const kernelFunctionNames = Object.keys(kernelFuncRecord); // for debug

  const WORD = 4;
  const TAG_MASK = 0xf0000000;
  const TAG_SHIFT = 28;
  const SIZE_MASK = 0x0fffffff;
  const SIZE_SHIFT = 0;
  const NEVER_EVALUATE = 0xffff;

  const textDecoder = new TextDecoder('utf-16le');
  const identity = (f: Function) => f;
  const elmFunctionWrappers = [
    identity,
    identity,
    F2,
    F3,
    F4,
    F5,
    F6,
    F7,
    F8,
    F9
  ];

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
    if (!addr) return null;
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
        const elmConst = wasmConstAddrs[addr]; // True/False/Unit/JsNull
        if (elmConst !== undefined) return elmConst;
        const wasmCtor = mem32[index + 1];

        if (wasmCtor == JsonValue.ARRAY) {
          const jsonArray: any[] = [];
          mem32.slice(index + 2, index + size).forEach(childAddr => {
            jsonArray.push(readWasmValue(childAddr));
          });
          return jsonArray;
        }

        if (wasmCtor == JsonValue.OBJECT) {
          const jsonObj: Record<string, any> = {};
          const pointers = mem32.slice(index + 2, index + size);
          for (let i = 0; i < pointers.length; i += 2) {
            const key = readWasmValue(pointers[i]);
            const val = readWasmValue(pointers[i + 1]);
            jsonObj[key] = val;
          }
          return jsonObj;
        }

        const jsCtor: number | string =
          wasmCtor >= KERNEL_CTOR_OFFSET
            ? wasmCtor - KERNEL_CTOR_OFFSET
            : appTypes.ctors[wasmCtor];
        const custom: Record<string, any> = { $: jsCtor };
        const fieldNames = 'abcdefghijklmnopqrstuvwxyz';
        for (let i = 0; i < size - 2; i++) {
          const field = fieldNames[i];
          const childAddr = mem32[index + 2 + i];
          custom[field] = readWasmValue(childAddr);
        }
        return custom;
      }
      case Tag.Record: {
        const record: Record<string, any> = {};
        const fgAddr = mem32[index + 1];
        const fieldNames =
          appTypes.fieldGroups[fgAddr] || readWasmValue(fgAddr);
        fieldNames.forEach((fieldName, i) => {
          const valAddr = mem32[index + 2 + i];
          record[fieldName] = readWasmValue(valAddr);
        });
        return record;
      }
      case Tag.FieldGroup: {
        const fieldNames: string[] = [];
        mem32.slice(index + 2, index + size).forEach(fieldId => {
          fieldNames.push(appTypes.fields[fieldId]);
        });
        return fieldNames;
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
    let { n_values, evaluator, argsIndex } = metadata;
    let kernelFn: ElmCurriedFunction = kernelFunctions[evaluator];
    while (n_values) {
      let f: Function;
      let nArgs: number;
      if (kernelFn.a && kernelFn.f && n_values >= kernelFn.a) {
        f = kernelFn.f;
        nArgs = kernelFn.a;
      } else {
        f = kernelFn;
        nArgs = kernelFn.length || 1;
      }
      const args: any[] = [];
      mem32.slice(argsIndex, argsIndex + nArgs).forEach(argAddr => {
        args.push(readWasmValue(argAddr));
      });
      n_values -= nArgs;
      argsIndex += nArgs;
      kernelFn = f(...args);
    }
    return kernelFn as any;
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
    const typeInfo: TypeInfo = detectElmType(value);
    switch (typeInfo.kind) {
      case 'constAddr':
        return { addr: typeInfo.value, nextIndex };
      case 'tag': {
        const tag: Tag = typeInfo.value;
        const builder: WasmBuilder = wasmBuilder(tag, value);
        return writeFromBuilder(nextIndex, builder, tag);
      }
      case 'jsonWrap': {
        const unwrapped = value.a;
        const unwrappedResult = writeJsonValue(
          nextIndex,
          unwrapped,
          JsShape.MAYBE_CIRCULAR
        );
        nextIndex = unwrappedResult.nextIndex;
        const tag = Tag.Custom;
        const builder: WasmBuilder = {
          body: [JsonValue.WRAP, unwrappedResult.addr],
          jsChildren: [],
          bodyWriter: null
        };
        return writeFromBuilder(nextIndex, builder, tag);
      }
      case 'kernelArray': {
        const tag = Tag.Custom;
        const builder: WasmBuilder = {
          body: [JsonValue.ARRAY],
          jsChildren: value,
          bodyWriter: null
        };
        return writeFromBuilder(nextIndex, builder, tag);
      }
    }
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
      }
    | {
        kind: 'jsonWrap';
      }
    | {
        kind: 'kernelArray';
      };

  class FieldGroup {
    constructor(public fieldNames: string[]) {}
  }

  function detectElmType(elmValue: any): TypeInfo {
    if (elmValue === null || elmValue === undefined) {
      return { kind: 'constAddr', value: wasmConstAddrs.JsNull };
    }
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
        if (elmValue instanceof FieldGroup) {
          return { kind: 'tag', value: Tag.FieldGroup };
        }
        if (Array.isArray(elmValue)) {
          return { kind: 'kernelArray' };
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
          case JsonValue.WRAP:
            return { kind: 'jsonWrap' };
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
            if (offset16 + lenAligned > maxWriteIndex16) {
              throw heapOverflowError;
            }
            for (let i = 0; i < lenAligned; i++) {
              mem16[offset16 + i] = s.charCodeAt(i);
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
        const jsCtor: string | number = value.$;
        const keys = Object.keys(value).filter(k => k !== '$');
        if (typeof jsCtor === 'string') {
          return {
            body: [appTypes.ctors[jsCtor]],
            jsChildren: keys.map(k => value[k]),
            bodyWriter: null
          };
        } else {
          const jsChildren: any[] = [];
          const fieldNames = 'abcdefghijklmnopqrstuvwxyz'.split('');
          keys.forEach(k => {
            const i = fieldNames.indexOf(k);
            if (i === -1) {
              throw new Error(`Unsupported Kernel Custom field '${k}'`);
            }
            jsChildren[i] = value[k];
          });
          return {
            body: [KERNEL_CTOR_OFFSET + jsCtor],
            jsChildren,
            bodyWriter: null
          };
        }
      }
      case Tag.Record: {
        const body: number[] = [];
        const jsChildren: any[] = [];
        const keys = Object.keys(value).sort();
        const fgName = keys.join(' ');
        const fgAddrStatic = appTypes.fieldGroups[fgName];
        if (fgAddrStatic) {
          body.push(fgAddrStatic);
        } else {
          jsChildren.push(new FieldGroup(keys));
        }
        keys.forEach(k => jsChildren.push(value[k]));
        return {
          body,
          jsChildren,
          bodyWriter: null
        };
      }
      case Tag.FieldGroup: {
        const fieldNames: string[] = value.fieldNames;
        const body = [fieldNames.length];
        fieldNames.forEach(name => body.push(appTypes.fields[name]));
        return {
          body,
          jsChildren: [],
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
      case Tag.JsRef: {
        return {
          body: [allocateJsRef(value)],
          jsChildren: [],
          bodyWriter: null
        };
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
      for (let i = 0; i < jsChildren.length; i++) {
        const child = jsChildren[i];
        const update = writeWasmValue(nextIndex, child); // recurse
        childAddrs.push(update.addr);
        nextIndex = update.nextIndex;
      }

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

          WRITE CIRCULAR JS VALUES TO WASM

  -------------------------------------------------- */

  enum JsShape {
    NOT_CIRCULAR,
    MAYBE_CIRCULAR
  }

  interface JsHeapEntry {
    value: any;
    isMarked: boolean;
    isOldGen: boolean;
  }

  const unusedJsHeapSlot = {};
  const jsHeap: JsHeapEntry[] = [];

  function allocateJsRef(value: any): number {
    let id = 0;
    while (id < jsHeap.length && jsHeap[id].value !== unusedJsHeapSlot) id++;
    if (id == jsHeap.length) {
      jsHeap.push({ isMarked: false, isOldGen: false, value });
    } else {
      jsHeap[id].value = value;
    }
    return id;
  }

  function markJsRef(id: number): void {
    jsHeap[id].isMarked = true;
  }

  function sweepJsRefs(isFullGc: boolean): void {
    let lastUsedSlot = 0;
    jsHeap.forEach((slot, index) => {
      let shouldKeep = slot.isMarked;
      if (isFullGc) {
        slot.isOldGen = shouldKeep;
      } else if (slot.isOldGen) {
        shouldKeep = true;
      }
      if (shouldKeep) {
        lastUsedSlot = index;
      } else {
        slot.value = unusedJsHeapSlot;
      }
      slot.isMarked = false;
    });
    jsHeap.splice(lastUsedSlot + 1, jsHeap.length);
  }

  function getJsRefArrayIndex(jsRefId: number, index: number): number {
    const array = jsHeap[jsRefId].value;
    if (!Array.isArray(array)) return 0;
    if (index >= array.length) return -(array.length + 1);
    const value = array[index];
    return handleWasmWrite(nextIndex =>
      writeJsonValue(nextIndex, value, JsShape.MAYBE_CIRCULAR)
    );
  }

  function getJsRefObjectField(
    jsRefId: number,
    fieldStringAddr: number
  ): number {
    const obj = jsHeap[jsRefId].value;
    if (typeof obj !== 'object' || obj === null) return 0;
    const field = readWasmValue(fieldStringAddr);
    if (!(field in obj)) return 0;
    const value = obj[field];
    return handleWasmWrite(nextIndex =>
      writeJsonValue(nextIndex, value, JsShape.MAYBE_CIRCULAR)
    );
  }

  function getJsRefValue(jsRefId: number): number {
    const value = jsHeap[jsRefId].value;
    return handleWasmWrite(nextIndex =>
      writeJsonValue(nextIndex, value, JsShape.NOT_CIRCULAR)
    );
  }

  function writeJsonValue(
    nextIndex: number,
    value: any,
    jsShape: JsShape
  ): WriteResult {
    switch (typeof value) {
      case 'boolean':
        return {
          addr: value ? wasmConstAddrs.True : wasmConstAddrs.False,
          nextIndex
        };
      case 'number':
        return writeFromBuilder(
          nextIndex,
          wasmBuilder(Tag.Float, value),
          Tag.Float
        );
      case 'string':
        return writeFromBuilder(
          nextIndex,
          wasmBuilder(Tag.String, value),
          Tag.String
        );
      case 'object': {
        if (value === null) {
          return { addr: wasmConstAddrs.JsNull, nextIndex };
        }
        if ('$' in value) {
          const unwrapped = writeJsonValue(nextIndex, value.a, jsShape);
          return writeFromBuilder(
            unwrapped.nextIndex,
            {
              body: [JsonValue.WRAP, unwrapped.addr],
              jsChildren: [],
              bodyWriter: null
            },
            Tag.Custom
          );
        }
        if (jsShape === JsShape.MAYBE_CIRCULAR) {
          return writeFromBuilder(
            nextIndex,
            wasmBuilder(Tag.JsRef, value),
            Tag.JsRef
          );
        }
        const body: number[] = [];
        if (Array.isArray(value)) {
          body.push(JsonValue.ARRAY);
          value.forEach(elem => {
            const result = writeJsonValue(
              nextIndex,
              elem,
              JsShape.NOT_CIRCULAR
            );
            nextIndex = result.nextIndex;
            body.push(result.addr);
          });
        } else {
          body.push(JsonValue.OBJECT);
          Object.keys(value).forEach(key => {
            const keyResult = writeJsonValue(nextIndex, key, jsShape);
            nextIndex = keyResult.nextIndex;
            body.push(keyResult.addr);
            const valueResult = writeJsonValue(
              nextIndex,
              value[key],
              JsShape.NOT_CIRCULAR
            );
            nextIndex = valueResult.nextIndex;
            body.push(valueResult.addr);
          });
        }
        const builder: WasmBuilder = {
          body,
          jsChildren: [],
          bodyWriter: null
        };
        return writeFromBuilder(nextIndex, builder, Tag.Custom);
      }
      default:
        // typeof is 'undefined', 'function', 'symbol', or 'bigint'
        // Preserve the value so it can be passed in one port and out another
        return writeFromBuilder(
          nextIndex,
          wasmBuilder(Tag.JsRef, value),
          Tag.JsRef
        );
    }
  }

  function call(evaluator: number, args: any[]) {
    function thunk() {}
    thunk.evaluator = evaluator;
    thunk.freeVars = args;
    thunk.max_values = args.length;

    const closureAddr = handleWasmWrite(nextIndex =>
      writeWasmValue(nextIndex, thunk)
    );
    const resultAddr = wasmExports._evalClosure(closureAddr);
    return readWasmValue(resultAddr);
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
    readWasmValue,
    writeWasmValue: (value: any) =>
      handleWasmWrite(nextIndex => writeWasmValue(nextIndex, value)),
    writeJsonValue: (value: any) =>
      handleWasmWrite(nextIndex =>
        writeJsonValue(nextIndex, value, JsShape.MAYBE_CIRCULAR)
      ),
    call,
    getJsRefArrayIndex,
    getJsRefObjectField,
    getJsRefValue,
    markJsRef,
    sweepJsRefs
  };
}
