interface ElmImports {
  _List_Cons(hd: any, tl: object): object;
  _List_Nil: object;
  _Utils_Tuple0: object;
  _Utils_Tuple2(a: any, b: any): object;
  _Utils_Tuple3(a: any, b: any, c: any): object;
  _Utils_chr(c: string): String;
  F2: Function;
  F3: Function;
  F4: Function;
  F5: Function;
  F6: Function;
  F7: Function;
  F8: Function;
  F9: Function;
}

/**
 * Functions exported from the Wasm module
 */
interface EmscriptenModule {
  HEAPU16: Uint16Array;
  HEAPU32: Uint32Array;
  _getMains: () => number;
  _getJsNull: () => number;
  _getUnit: () => number;
  _getNil: () => number;
  _getTrue: () => number;
  _getFalse: () => number;
  _getFieldGroups: () => number;
  _allocate: (size: number) => number;
  _readF64: (addr: number) => number;
  _writeF64: (addr: number, value: number) => void;
  _evalClosure: (addr: number) => number;
  _debugHeapState: () => void;
  _debugAddrRange: (start: number, size: number) => void;
  _debugStackMap: () => void;
  _debugEvaluatorName: (evalId: number) => void;
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
 * This needs to be compiled from TypeScript to JavaScript in a build step.
 * Then Emscripten bundles it along with its own code and the Elm compiler output.
 *
 * @param emscriptenModule   Emscripten toolchain output
 * @param elmImports         Values imported into the wrapper from Elm app
 * @param generatedAppTypes  App-specific type info passed from Elm compiler to this wrapper
 * @param kernelFuncRecord   Record of all JS kernel functions called by the Elm Wasm module
 *
 /********************************************************************************************/
function wrapWasmElmApp(
  emscriptenModule: EmscriptenModule,
  elmImports: ElmImports,
  generatedAppTypes: GeneratedAppTypes,
  kernelFuncRecord: Record<string, ElmCurriedFunction>
) {
  /* --------------------------------------------------

               INITIALISATION & CONSTANTS

  -------------------------------------------------- */

  const mem32: Uint32Array = emscriptenModule.HEAPU32;
  const mem16: Uint16Array = emscriptenModule.HEAPU16;

  const wasmConstAddrs = (function () {
    const Unit = emscriptenModule._getUnit();
    const Nil = emscriptenModule._getNil();
    const True = emscriptenModule._getTrue();
    const False = emscriptenModule._getFalse();
    const JsNull = emscriptenModule._getJsNull();
    return {
      Unit,
      Nil,
      True,
      False,
      JsNull,
      [Unit]: elmImports._Utils_Tuple0,
      [Nil]: elmImports._List_Nil,
      [True]: true,
      [False]: false,
      [JsNull]: null
    };
  })();

  const {
    _List_Cons,
    _List_Nil,
    _Utils_Tuple2,
    _Utils_Tuple3,
    _Utils_chr
  } = elmImports;

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
    fieldGroups: mapFieldGroups()
  };
  function mapFieldGroups(): NameToInt & IntToNames {
    let fgPointersAddr = emscriptenModule._getFieldGroups();
    return generatedAppTypes.fieldGroups.reduce(
      (enumObj: NameToInt & IntToNames, name: string) => {
        const fgIndex = fgPointersAddr >> 2;
        const fgAddr = mem32[fgIndex];
        fgPointersAddr += 4;
        if (!fgAddr) throw new Error('Ran out of Wasm fieldgroups');
        enumObj[name] = fgAddr;
        enumObj[fgAddr] = name.split(' ');
        return enumObj;
      },
      {}
    );
  }

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
    elmImports.F2,
    elmImports.F3,
    elmImports.F4,
    elmImports.F5,
    elmImports.F6,
    elmImports.F7,
    elmImports.F8,
    elmImports.F9
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
        return emscriptenModule._readF64(addr + 2 * WORD);
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
      const size = 3 + n_values;
      const closureAddr = emscriptenModule._allocate(size);
      const index = closureAddr >> 2;
      mem32[index] = encodeHeader(Tag.Closure, size);
      mem32[index + 1] = (max_values << 16) | n_values;
      mem32[index + 2] = evaluator;
      for (let i = 0; i < args.length; i++) {
        mem32[index + 3 + i] = writeWasmValue(args[i]);
      }
      const resultAddr = emscriptenModule._evalClosure(closureAddr);
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

  type Address = number;

  function writeWasmValue(elmValue: any): Address {
    if (elmValue === null || elmValue === undefined) {
      return wasmConstAddrs.JsNull;
    }
    switch (typeof elmValue) {
      case 'number': {
        // There's no way to tell `1 : Int` from `1.0 : Float` at this low level. But `1.2` is definitely a Float.
        // So _for now_ take a _horribly unsafe_ guess, by checking if it's a round number or not.
        // Not cool. This is Elm! Long term, the ambiguity needs to be solved at some higher level.
        // Maybe some lib like `JSON.Encode` so the app dev decides? Pity for it not to be automatic though!
        const isRoundNumberSoGuessInt = elmValue === Math.round(elmValue);
        if (isRoundNumberSoGuessInt) {
          const addr = emscriptenModule._allocate(2);
          const index = addr >> 2;
          mem32[index] = encodeHeader(Tag.Int, 2);
          mem32[index + 1] = elmValue;
          return addr;
        } else {
          const addr = emscriptenModule._allocate(4);
          const index = addr >> 2;
          mem32[index] = encodeHeader(Tag.Float, 4);
          mem32[index + 1] = 0;
          emscriptenModule._writeF64(addr + 8, elmValue);
          return addr;
        }
      }

      case 'string': {
        const len = elmValue.length;
        const size = 1 + Math.ceil(len / 2);
        const addr = emscriptenModule._allocate(size);
        const index = addr >> 2;
        mem32[index] = encodeHeader(Tag.String, size);
        const words16 = (index + 1) << 1;
        for (let i = 0; i < len; i++) {
          mem16[words16 + i] = elmValue.charCodeAt(i);
        }
        if (len & 1) {
          mem16[words16 + len] = 0;
        }
        return addr;
      }

      case 'boolean':
        return elmValue ? wasmConstAddrs.True : wasmConstAddrs.False;

      case 'function':
        return writeClosure(elmValue);

      case 'object': {
        if (elmValue instanceof String) {
          const addr = emscriptenModule._allocate(2);
          const index = addr >> 2;
          mem32[index] = encodeHeader(Tag.Char, 2);
          mem32[index + 1] =
            elmValue.charCodeAt(0) | (elmValue.charCodeAt(1) << 16);
          return addr;
        }
        if (Array.isArray(elmValue)) {
          return writeJsonArray(elmValue, JsShape.NOT_CIRCULAR);
        }
        switch (elmValue.$) {
          case undefined:
            return writeRecord(elmValue);
          case '[]':
            return wasmConstAddrs.Nil;
          case '#0':
            return wasmConstAddrs.Unit;
          case '::':
          case '#2': {
            const a = writeWasmValue(elmValue.a);
            const b = writeWasmValue(elmValue.b);
            const addr = emscriptenModule._allocate(3);
            const index = addr >> 2;
            const tag = elmValue.$ === '::' ? Tag.List : Tag.Tuple2;
            mem32[index] = encodeHeader(tag, 3);
            mem32[index + 1] = a;
            mem32[index + 2] = b;
            return addr;
          }
          case '#3': {
            const a = writeWasmValue(elmValue.a);
            const b = writeWasmValue(elmValue.b);
            const c = writeWasmValue(elmValue.b);
            const addr = emscriptenModule._allocate(4);
            const index = addr >> 2;
            mem32[index] = encodeHeader(Tag.Tuple3, 4);
            mem32[index + 1] = a;
            mem32[index + 2] = b;
            mem32[index + 3] = c;
            return addr;
          }
          default:
            return typeof elmValue.$ === 'string'
              ? writeUserCustom(elmValue)
              : writeKernelCustom(elmValue);
        }
      }
    }
    console.error(elmValue);
    throw new Error('Cannot determine type of Elm value');
  }

  function writeJsonArray(value: any[], jsShape: JsShape): Address {
    const size = 2 + value.length;
    const addr = emscriptenModule._allocate(size);
    const index = addr >> 2;
    mem32[index] = encodeHeader(Tag.Custom, size);
    mem32[index + 1] = JsonValue.ARRAY;
    for (let i = 0; i < value.length; i++) {
      mem32[index + 2 + i] = writeJsonValue(value[i], jsShape);
    }
    return addr;
  }

  function writeUserCustom(value: Record<string, any>): Address {
    const jsCtor: string = value.$;
    const keys = Object.keys(value);
    const nKeys = keys.length - 1; // subtract 1 for '$'
    const size = 2 + nKeys;
    const addr = emscriptenModule._allocate(size);
    const index = addr >> 2;
    mem32[index] = encodeHeader(Tag.Custom, size);
    mem32[index + 1] = appTypes.ctors[jsCtor];
    for (let k = 0; k < nKeys; k++) {
      const key = keys[k];
      if (key !== '$') {
        mem32[index + 2 + k] = writeWasmValue(value[key]);
      }
    }

    return addr;
  }

  /**
   * JS Kernel objects have numeric constructor, single-letter keys
   * For Wasm we store each value at an index corresponding to that letter
   * {a:123, j: 456} will be length 10, not 2
   */
  function writeKernelCustom(value: Record<string, any>): Address {
    const jsCtor: number = value.$;
    const keys = Object.keys(value);
    const nKeys = keys.length - 1; // subtract 1 for '$'

    // prettier-ignore
    const kernelKeys = ['a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'];
    const childAddrs: Address[] = [];
    for (let k = 0; k < nKeys; k++) {
      const key = keys[k];
      if (key !== '$') {
        const wasmIndex = kernelKeys.indexOf(key); // encode the letter key by position
        if (wasmIndex === -1) {
          throw new Error(`Unsupported Kernel Custom field '${key}'`);
        }
        childAddrs[wasmIndex] = writeWasmValue(value[key]);
      }
    }

    const size = 2 + childAddrs.length;
    const addr = emscriptenModule._allocate(size);
    const index = addr >> 2;
    mem32[index] = encodeHeader(Tag.Custom, size);
    mem32[index + 1] = KERNEL_CTOR_OFFSET + jsCtor;
    for (let i = 0; i < childAddrs.length; i++) {
      mem32[index + 2 + i] = childAddrs[i];
    }
    return addr;
  }

  function writeRecord(value: Record<string, any>): Address {
    const keys = Object.keys(value).sort();

    const size = 2 + keys.length;
    const addr = emscriptenModule._allocate(size);
    const index = addr >> 2;
    mem32[index] = encodeHeader(Tag.Record, size);

    const fgName = keys.join(' ');
    const fgAddrStatic = appTypes.fieldGroups[fgName];
    mem32[index + 1] = fgAddrStatic || writeFieldGroup(keys);
    for (let k = 0; k < keys.length; k++) {
      const key = keys[k];
      mem32[index + 2 + k] = writeWasmValue(value[key]);
    }
    return 0;
  }

  function writeFieldGroup(fieldNames: string[]): Address {
    const size = 1 + fieldNames.length;
    const addr = emscriptenModule._allocate(size);
    const index = addr >> 2;
    mem32[index] = encodeHeader(Tag.FieldGroup, size);
    for (let k = 0; k < fieldNames.length; k++) {
      const key = fieldNames[k];
      mem32[index + 1 + k] = appTypes.fields[key];
    }
    return addr;
  }

  function writeClosure(value: any): Address {
    const fun = value.f || value;
    if (fun.evaluator) {
      const { freeVars, max_values, evaluator } = fun;
      const n_values = freeVars.length;
      const size = 3 + n_values;
      const addr = emscriptenModule._allocate(size);
      const index = addr >> 2;
      mem32[index] = encodeHeader(Tag.Closure, size);
      mem32[index + 1] = (max_values << 16) | n_values;
      mem32[index + 2] = evaluator;
      for (let i = 0; i < freeVars.length; i++) {
        mem32[index + 3 + i] = writeWasmValue(freeVars[i]);
      }
      return addr;
    } else {
      let evaluator = kernelFunctions.findIndex(f => f === value);
      if (evaluator === -1) {
        kernelFunctions.push(value);
        evaluator = kernelFunctions.length - 1;
      }
      const size = 3;
      const addr = emscriptenModule._allocate(size);
      const index = addr >> 2;
      mem32[index] = encodeHeader(Tag.Closure, size);
      mem32[index + 1] = NEVER_EVALUATE << 16;
      mem32[index + 2] = evaluator;
      return addr;
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
    return writeJsonValue(value, JsShape.MAYBE_CIRCULAR);
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
    return writeJsonValue(value, JsShape.MAYBE_CIRCULAR);
  }

  function getJsRefValue(jsRefId: number): number {
    const value = jsHeap[jsRefId].value;
    return writeJsonValue(value, JsShape.NOT_CIRCULAR);
  }

  function writeJsonValue(
    value: any,
    jsShape = JsShape.MAYBE_CIRCULAR
  ): Address {
    if (typeof value === 'number') {
      // Json number always written as Float, decoder can convert to Int later
      const addr = emscriptenModule._allocate(4);
      const index = addr >> 2;
      mem32[index] = encodeHeader(Tag.Float, 4);
      mem32[index + 1] = 0;
      emscriptenModule._writeF64(addr + 8, value);
      return addr;
    }
    if (typeof value !== 'object') {
      return writeWasmValue(value);
    }
    if (value === null) {
      return wasmConstAddrs.JsNull;
    }
    if ('$' in value) {
      // Wrapped value from Json module. Re-wrap it the Wasm way.
      const unwrapped = writeJsonValue(value.a, jsShape);
      const addr = emscriptenModule._allocate(3);
      const index = addr >> 2;
      mem32[index] = encodeHeader(Tag.Custom, 3);
      mem32[index + 1] = JsonValue.WRAP;
      mem32[index + 2] = unwrapped;
      return addr;
    }
    if (jsShape === JsShape.MAYBE_CIRCULAR) {
      const addr = emscriptenModule._allocate(2);
      const index = addr >> 2;
      mem32[index] = encodeHeader(Tag.JsRef, 2);
      mem32[index + 1] = allocateJsRef(value);
      return addr;
    }
    if (Array.isArray(value)) {
      return writeJsonArray(value, JsShape.MAYBE_CIRCULAR);
    }
    const keys = Object.keys(value);
    const size = 2 + keys.length * 2;
    const addr = emscriptenModule._allocate(size);
    const index = addr >> 2;
    mem32[index] = encodeHeader(Tag.Custom, size);
    mem32[index + 1] = JsonValue.OBJECT;
    for (let i = 0; i < keys.length; i++) {
      const key = keys[i];
      const two_i = 2 * i;
      mem32[index + 2 + two_i] = writeWasmValue(key);
      mem32[index + 3 + two_i] = writeJsonValue(value[key], jsShape);
    }
    return addr;
  }

  function call(evaluator: number, args: any[]) {
    function thunk() {}
    thunk.evaluator = evaluator;
    thunk.freeVars = args;
    thunk.max_values = args.length;
    const closureAddr = writeClosure(thunk);
    const resultAddr = emscriptenModule._evalClosure(closureAddr);
    return readWasmValue(resultAddr);
  }

  /* --------------------------------------------------

                    EXPORTS

  -------------------------------------------------- */

  const mains: any[] = [];

  const deref = (addr: number) => mem32[addr >> 2];
  let mainsArrayEntryAddr = emscriptenModule._getMains();
  while (true) {
    const gcRootAddr = deref(mainsArrayEntryAddr);
    if (!gcRootAddr) break;
    const mainAddr = deref(gcRootAddr);
    if (!mainAddr) break;
    mains.push(readWasmValue(mainAddr));
    mainsArrayEntryAddr += 4;
  }

  return {
    mains,
    readWasmValue,
    writeWasmValue,
    writeJsonValue,
    call,
    getJsRefArrayIndex,
    getJsRefObjectField,
    getJsRefValue,
    markJsRef,
    sweepJsRefs,
    parseFloat: (addr: number, len16: number) => {
      const idx16 = addr >> 1;
      const words16 = mem16.slice(idx16, idx16 + len16);
      const jsString = textDecoder.decode(words16);
      return parseFloat(jsString);
    }
  };
}
