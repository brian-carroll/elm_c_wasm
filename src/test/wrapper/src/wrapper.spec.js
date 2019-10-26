require('./elm-globals');
global.TextDecoder = require('util').TextDecoder;
global.formatHex = formatHex;
const wrapWasmElmApp = require('../build/wrapper');
const createEmscriptenModule = require('../build/mock-app');

/**
 * Debug formatter
 * Converts all numbers in a data structure to hex strings
 * Accepts objects, TypedArrays, arrays, etc.
 * Overall data structure is preserved. Strings, booleans etc pass through
 *
 * Example usage:
 * ```ts
 *  console.log(formatHex({ label: 'some description', addr, buffer, whatever }))
 * ```
 */
function formatHex(arg, bits = 32) {
  if (typeof arg === 'number') {
    return arg.toString(16).padStart(bits / 4, '0');
  }
  if (arg instanceof Uint16Array) {
    let hex = [];
    arg.forEach(word => {
      hex.push(formatHex(word, 16));
    });
    return hex;
  }
  if (arg instanceof Uint32Array) {
    let hex = [];
    arg.forEach(word => {
      hex.push(formatHex(word, 32));
    });
    return hex;
  }
  if (Array.isArray(arg)) {
    return arg.map(x => formatHex(x, bits));
  }
  if (typeof arg === 'object' && arg !== null) {
    const obj = {};
    Object.keys(arg).forEach(key => {
      obj[key] = formatHex(arg[key], bits);
    });
    return obj;
  }
  return arg;
}

// App-specific type info generated by the compiler
const generatedAppTypes = {
  ctors: {
    Nothing: 123,
    Just: 124,
    123: 'Nothing',
    124: 'Just'
  },
  fields: {
    address: 123,
    firstName: 456,
    lastName: 789,
    123: 'address',
    456: 'firstName',
    789: 'lastName'
  },
  fieldGroupNames: ['firstName_lastName_address', 'firstName_lastName']
};

describe('wrapper', () => {
  let asm;
  let readWasmValue;
  let writeWasmValue;

  beforeAll(() => {
    // Wait for C `main` to run before creating the wrapper
    return new Promise(resolve => {
      const Module = createEmscriptenModule();
      Module.postRun = () => {
        asm = Module.asm;
        const buffer = Module.buffer;
        const wrapper = wrapWasmElmApp(buffer, asm, generatedAppTypes);
        readWasmValue = wrapper.readWasmValue;
        writeWasmValue = wrapper.writeWasmValue;
        resolve();
      };
    });
  });

  describe('test setup', () => {
    it('should load globals', () => {
      expect(typeof _List_Nil).toBe('object');
      expect(typeof _List_Cons).toBe('function');
      expect(typeof _Utils_Tuple0).toBe('object');
      expect(typeof _Utils_Tuple2).toBe('function');
      expect(typeof _Utils_Tuple3).toBe('function');
      expect(typeof _Utils_chr).toBe('function');
      expect(typeof F).toBe('function');
      expect(typeof F2).toBe('function');
      expect(typeof F3).toBe('function');
      expect(typeof F4).toBe('function');
      expect(typeof A2).toBe('function');
      expect(typeof A3).toBe('function');
      expect(typeof A4).toBe('function');
    });
  });

  describe('C interface sanity checks', () => {
    const wrapperExportArities = {
      _getUnit: 0,
      _getNil: 0,
      _getTrue: 0,
      _getFalse: 0,
      _getNextFieldGroup: 0,
      _getMaxWriteAddr: 0,
      _getWriteAddr: 0,
      _finishWritingAt: 1,
      _readF64: 1,
      _writeF64: 2,
      _callClosure: 1,
      _collectGarbage: 0
    };

    describe('wrapper exports', () => {
      for (const [fName, arity] of Object.entries(wrapperExportArities)) {
        it(`should have an export ${fName} with arity ${arity}`, () => {
          expect(typeof asm[fName]).toBe('function');
          expect(asm[fName].length).toBe(arity);
        });
      }
    });
  });

  const testCases = [
    {
      elmName: '()',
      wasmGetter: '_getUnit',
      jsValue: _Utils_Tuple0
    },
    {
      elmName: '[]',
      wasmGetter: '_getNil',
      jsValue: _List_Nil
    },
    {
      elmName: 'True',
      wasmGetter: '_getTrue',
      jsValue: true
    },
    {
      elmName: 'False',
      wasmGetter: '_getFalse',
      jsValue: false
    },
    {
      elmName: '1234567 : Int',
      wasmGetter: '_get_test_int',
      jsValue: 1234567
    },
    {
      elmName: '3.14159265 : Float',
      wasmGetter: '_get_test_float',
      jsValue: 3.14159265
    },
    {
      elmName: "'A' : Char",
      wasmGetter: '_get_test_char16',
      jsValue: _Utils_chr('A')
    },
    {
      elmName: 'an emoji Char',
      wasmGetter: '_get_test_char32',
      jsValue: _Utils_chr('🙌')
    },
    {
      elmName: '"firstName1" : String',
      wasmGetter: '_get_test_string',
      jsValue: 'firstName1'
    },
    {
      elmName: '["firstName1", "lastName1"] : List String',
      wasmGetter: '_get_test_list',
      jsValue: _List_Cons('firstName1', _List_Cons('lastName1', _List_Nil))
    },
    {
      elmName: "(1234567, 'A')",
      wasmGetter: '_get_test_tuple2',
      jsValue: _Utils_Tuple2(1234567, _Utils_chr('A'))
    },
    {
      elmName: "(1234567, 'A', 3.14159265)",
      wasmGetter: '_get_test_tuple3',
      jsValue: _Utils_Tuple3(1234567, _Utils_chr('A'), 3.14159265)
    },
    {
      elmName: 'Nothing',
      wasmGetter: '_get_test_nothing',
      jsValue: elm$core$Maybe$Nothing
    },
    {
      elmName: 'Just 1234567',
      wasmGetter: '_get_test_just_int',
      jsValue: elm$core$Maybe$Just(1234567)
    },
    {
      elmName: 'a record of strings',
      wasmGetter: '_get_rec_address_firstName_lastName',
      jsValue: {
        address: 'addr1',
        firstName: 'firstName1',
        lastName: 'lastName1'
      }
    }
  ];

  describe('readWasmValue', () => {
    testCases.forEach(({ elmName, wasmGetter, jsValue }) => {
      it(`should correctly read ${elmName}`, () => {
        const wasmAddr = asm[wasmGetter]();
        expect(readWasmValue(wasmAddr)).toEqual(jsValue);
      });
    });
  });

  describe('writeWasmValue', () => {
    testCases.forEach(({ elmName, wasmGetter, jsValue }) => {
      it(`should correctly write ${elmName}`, () => {
        const wasmAddr = asm[wasmGetter]();
        const writtenAddr = writeWasmValue(jsValue);
        expect(asm._test_equal(wasmAddr, writtenAddr)).toBe(1);
      });
    });
  });

  describe('Closures', () => {
    it('should be able to call Wasm `increment` closure from JS', () => {
      const wasmAddr = asm._get_increment_callback();
      const callback = readWasmValue(wasmAddr);
      expect(callback(1)).toBe(2);
      expect(callback(1000000)).toBe(1000001);
    });
    xit('should be able to call JS `_VirtualDom_text` from Wasm', () => {
      expect(true).toBe(false);
    });
  });
});
