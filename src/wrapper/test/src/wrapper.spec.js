require('./elm-globals');
global.TextDecoder = require('util').TextDecoder;
global.formatHex = (arg, bits = 32) => {
  if (typeof arg === 'number') {
    const hexLength = bits / 4;
    const pad = '0'.repeat(hexLength);
    return (pad + arg.toString(16)).substr(-hexLength);
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
};

const createElmWasmWrapper = require('../build/wrapper');
const createEmscriptenModule = require('../build/mock-app');

const appTypes = {
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
  fieldGroupNames: ['firstName_lastName_address', 'firstName_lastName'],
  fieldGroups: {}
};

describe('wrapper', () => {
  let asm;
  let readValue;
  let writeValue;

  beforeAll(() => {
    // Wait for C `main` to run (don't use .then() method because it's not a Promise!!!)
    return new Promise(resolve => {
      const Module = createEmscriptenModule();
      Module.postRun = () => {
        asm = Module.asm;
        const buffer = Module.buffer;
        const wrapper = createElmWasmWrapper(buffer, asm, appTypes);
        readValue = wrapper.readValue;
        writeValue = wrapper.writeValue;
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

  describe('readValue', () => {
    testCases.forEach(({ elmName, wasmGetter, jsValue }) => {
      it(`should correctly read ${elmName}`, () => {
        const wasmAddr = asm[wasmGetter]();
        expect(readValue(wasmAddr)).toEqual(jsValue);
      });
    });
  });

  describe('writeValue', () => {
    testCases.forEach(({ elmName, wasmGetter, jsValue }) => {
      it(`should correctly write ${elmName}`, () => {
        const wasmAddr = asm[wasmGetter]();
        const writtenAddr = writeValue(jsValue);
        expect(asm._test_equal(wasmAddr, writtenAddr)).toBe(1);
      });
    });
  });
});
