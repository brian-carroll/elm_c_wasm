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
    it('should have all required functions with correct arities', () => {
      return createEmscriptenModule().then(Module => {
        const { asm } = Module;
        const arities = {
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
        Object.keys(arities).forEach(fname => {
          const arity = arities[fname];
          const f = asm[fname];
          expect(`typeof ${fname} === '${typeof f}'`).toBe(
            `typeof ${fname} === 'function'`
          );
          expect(`arity of ${fname} === ${f.length}`).toBe(
            `arity of ${fname} === ${arity}`
          );
        });
      });
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
