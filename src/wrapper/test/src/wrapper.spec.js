require('./elm-globals');
const createElmWasmWrapper = require('../build/wrapper');
const createEmscriptenModule = require('../build/mock-app');

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

  describe('C interface', () => {
    let asm;

    beforeAll(() => {
      return createEmscriptenModule().then(Module => {
        asm = Module.asm;
      });
    });

    it('should have all required functions with correct arities', () => {
      expect(asm._getUnit.length).toBe(0);
      expect(asm._getNil.length).toBe(0);
      expect(asm._getTrue.length).toBe(0);
      expect(asm._getFalse.length).toBe(0);
      expect(asm._getNextFieldGroup.length).toBe(0);
      expect(asm._getMaxWriteAddr.length).toBe(0);
      expect(asm._getWriteAddr.length).toBe(0);
      expect(asm._finishWritingAt.length).toBe(1);
      expect(asm._readF64.length).toBe(1);
      expect(asm._writeF64.length).toBe(2);
      expect(asm._callClosure.length).toBe(1);
      expect(asm._collectGarbage.length).toBe(0);
    });

    it('should have correctly working getters', () => {
      expect(typeof asm._getUnit()).toBe('number');
      expect(typeof asm._getNil()).toBe('number');
      expect(typeof asm._getTrue()).toBe('number');
      expect(typeof asm._getFalse()).toBe('number');
      expect(typeof asm._getNextFieldGroup()).toBe('number');
      expect(typeof asm._getMaxWriteAddr()).toBe('number');
      expect(typeof asm._getWriteAddr()).toBe('number');
    });
  });

  xit('should wrap the WebAssembly module', () => {
    return createEmscriptenModule().then(Module => {
      const { buffer, asm } = Module;
      const appTypes = {
        ctors: {},
        fields: {},
        fieldGroupNames: [],
        fieldGroups: {}
      };
      createElmWasmWrapper(buffer, asm, appTypes);
      console.log(Module);
    });
  });
});
