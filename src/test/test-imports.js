mergeInto(LibraryManager.library, {
  writeJsTestValue: (function () {
    i = 0;
    const TEST_JS_OBJECT_NON_CYCLIC = i++;
    const TEST_JS_OBJECT_CYCLIC = i++;
    const TEST_JS_ARRAY_NON_CYCLIC = i++;
    const TEST_JS_ARRAY_CYCLIC = i++;

    return function writeJsTestValue(id) {
      let value;
      switch (id) {
        case TEST_JS_OBJECT_NON_CYCLIC:
          value = { a: { b: 1 }, c: 2 };
          break;
        case TEST_JS_OBJECT_CYCLIC: {
          value = {};
          value.a = value;
          value.c = 2;
          break;
        }
        case TEST_JS_ARRAY_NON_CYCLIC:
          value = [[1], 2];
          break;
        case TEST_JS_ARRAY_CYCLIC: {
          value = [];
          value.push(value);
          value.push(2);
          break;
        }
        default:
          throw new Error(`Unknown JS test value ID ${id}`);
      }
      return wasmWrapper.writeJsonValue(value);
    };
  })()
});
