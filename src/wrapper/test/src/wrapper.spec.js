require('./elm-globals');
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
  ctors: {},
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
    let asm;

    beforeAll(() => {
      return createEmscriptenModule().then(Module => {
        asm = Module.asm;
      });
    });

    describe('wrapper exports', () => {
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
      for (const [fName, arity] of Object.entries(wrapperExportArities)) {
        it(`should have an export '${fName}' with arity ${arity}`, () => {
          expect(typeof asm[fName]).toBe('function');
          expect(asm[fName].length).toBe(arity);
        });
      }
    });

    describe('test exports', () => {
      const testExportArities = {
        _get_rec_address_firstName_lastName: 0,
        _get_rec_firstName_lastName: 0
      };
      for (const [fName, arity] of Object.entries(testExportArities)) {
        it(`should have an export '${fName}' with arity ${arity}`, () => {
          expect(typeof asm[fName]).toBe('function');
          expect(asm[fName].length).toBe(arity);
        });
      }
    });
  });

  describe('readValue', () => {
    let asm;
    let readValue;

    beforeAll(() => {
      return createEmscriptenModule().then(Module => {
        asm = Module.asm;
        const buffer = Module.buffer;
        const wrapper = createElmWasmWrapper(buffer, asm, appTypes);
        readValue = wrapper.readValue;
      });
    });

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

    it("should correctly decode `'🙌' : Char`", () => {
      expect(readValue(asm._get_test_char32())).toEqual(new String('🙌'));
    });

    it('should decode the 3-field test record', () => {
      const actual = readValue(asm._get_rec_address_firstName_lastName());
      expect(actual).toEqual({
        address: 'addr1',
        firstName: 'firstName1',
        lastName: 'lastName1'
      });
    });
  });
});
