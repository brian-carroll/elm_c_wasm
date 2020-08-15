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
    console.log('testJsonValueRoundTrip', json);
    let parsed;
    try {
      parsed = JSON.parse(json);
    } catch (e) {
      console.error({ json, e });
    }
    return wasmWrapper.writeJsonValue({ $: 0, a: parsed });
  }
});
