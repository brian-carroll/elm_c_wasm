require('./elm-globals');
const createElmWasmWrapper = require('../build/wrapper');
const createEmscriptenModule = require('../build/mock-app');

describe('wrapper', () => {
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

  it('should load the Wasm module', () => {
    return createEmscriptenModule().then(Module => {
      const { asm } = Module;
      expect(asm._addTwo(3, 2)).toBe(5);
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
