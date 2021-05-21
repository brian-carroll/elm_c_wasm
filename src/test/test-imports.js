mergeInto(LibraryManager.library, {
  testCircularJsValue: function (isArray) {
    let value;
    if (!isArray) {
      value = {};
      value.a = value; // circular ref, can't decode
      value.b = 123; // Can decode if decoder ignores the other field
    } else {
      value = [];
      value.push(value); // circular ref, can't decode
      value.push(123); // Can decode if decoder ignores the other entry
    }
    return wasmWrapper.writeJsonValue({ $: 0, a: value });
  },

  testJsonValueRoundTrip: function (jsonStringAddr) {
    const json = wasmWrapper.readWasmValue(jsonStringAddr);
    let parsed;
    try {
      parsed = JSON.parse(json);
    } catch (e) {
      console.error({ json, e });
    }
    return wasmWrapper.writeJsonValue({ $: 0, a: parsed });
  },

  testElmValueRoundTrip: function (addr) {
    const jsValue = wasmWrapper.readWasmValue(addr);
    return wasmWrapper.writeWasmValue(jsValue);
  },

  writeTestValue: function (index) {
    const record = {
      init: 'init value',
      subscriptions: 'subscriptions value',
      update: 'update value',
      view: 'view value'
    };
    function kernelIncrementBy1(x) {
      return x + 1;
    }
    const testValueArray = [
      /*  0 */ undefined,
      /*  1 */ null,
      /*  2 */ 123,
      /*  3 */ 3.14,
      /*  4 */ 'hello world',
      /*  5 */ true,
      /*  6 */ false,
      /*  7 */ _Utils_chr('x'),
      /*  8 */ ['Kernel', 'array'],
      /*  9 */ _List_Nil,
      /* 10 */ _Utils_Tuple0,
      /* 11 */ _Utils_Tuple2('U', 'V'),
      /* 12 */ _Utils_Tuple3('X', 'Y', 'Z'),
      /* 13 */ _List_Cons(1, _List_Cons(2, _List_Cons(3, _List_Nil))),
      /* 14 */ record,
      /* 15 */ { $: 5, b: 'kernel', e: 'union' },
      /* 16 */ { $: 'Just', a: 321 },
      /* 17 */ kernelIncrementBy1
    ];
    return wasmWrapper.writeWasmValue(testValueArray[index]);
  },

  testElmCallback: function (addr) {
    const wasmCallback = wasmWrapper.readWasmValue(addr);
    return A2(wasmCallback, 999, { $: 'Just', a: 'hello' });
  }
});
