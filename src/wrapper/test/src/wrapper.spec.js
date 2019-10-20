require('./elm-globals');
const exportedFunctionNames = require('../exported-functions.json');
global.TextDecoder = require('util').TextDecoder;
global.bufferToHex = (buf, bits = 32) => {
  let hex = [];
  const hexLength = bits / 4;
  const pad = '0'.repeat(hexLength);
  buf.forEach(word => {
    hex.push((pad + word.toString(16)).substr(-hexLength));
  });
  return hex;
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

  beforeAll(() => {
    // Wait for C `main` to run (don't use .then() method because it's not a Promise!!!)
    return new Promise(resolve => {
      const Module = createEmscriptenModule();
      Module.postRun = () => {
        asm = Module.asm;
        const buffer = Module.buffer;
        const wrapper = createElmWasmWrapper(buffer, asm, appTypes);
        readValue = wrapper.readValue;
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

  describe('readValue', () => {
    it('should correctly decode `()`', () => {
      expect(readValue(asm._getUnit())).toBe(_Utils_Tuple0);
    });

    it('should correctly decode `[]`', () => {
      expect(readValue(asm._getNil())).toBe(_List_Nil);
    });

    it('should correctly decode `True`', () => {
      expect(readValue(asm._getTrue())).toBe(true);
    });

    it('should correctly decode `False`', () => {
      expect(readValue(asm._getFalse())).toBe(false);
    });

    it('should correctly decode `1234567 : Int`', () => {
      expect(readValue(asm._get_test_int())).toBe(1234567);
    });

    it('should correctly decode `3.14159265 : Float`', () => {
      expect(readValue(asm._get_test_float())).toBe(3.14159265);
    });

    it("should correctly decode `'A' : Char`", () => {
      const actual = readValue(asm._get_test_char16());
      expect(actual).toEqual(new String('A'));
    });

    it('should correctly decode an emoji Char', () => {
      expect(readValue(asm._get_test_char32())).toEqual(new String('🙌'));
    });

    it('should correctly decode `"firstName1" : String`', () => {
      expect(readValue(asm._get_test_string())).toEqual('firstName1');
    });

    it("should correctly decode `(1234567, 'A')`", () => {
      expect(readValue(asm._get_test_tuple2())).toEqual(
        _Utils_Tuple2(1234567, 'A')
      );
    });
    it("should correctly decode `(1234567, 'A', 3.14159265)`", () => {
      expect(readValue(asm._get_test_tuple2())).toEqual(
        _Utils_Tuple2(1234567, 'A')
      );
    });
    it('should correctly decode `Nothing`', () => {
      expect(readValue(asm._get_test_nothing())).toEqual(
        elm$core$Maybe$Nothing
      );
    });
    it('should correctly decode `Just 1234567`', () => {
      expect(readValue(asm._get_test_just_int())).toEqual(
        elm$core$Maybe$Just(1234567)
      );
    });

    it('should correctly decode a record', () => {
      const actual = readValue(asm._get_rec_address_firstName_lastName());
      expect(actual).toEqual({
        address: 'addr1',
        firstName: 'firstName1',
        lastName: 'lastName1'
      });
    });

    xit('should correctly make a callback from a Closure', () => {
      //
    });
  });
});
