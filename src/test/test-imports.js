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
    return wasmWrapper.writeJsonValue(value);
  },

  testJsonValueRoundTrip: function (jsonStringAddr) {
    const json = wasmWrapper.readWasmValue(jsonStringAddr);
    const jsValue = JSON.parse(json);
    return wasmWrapper.writeJsonValue(jsValue);
  }
});
