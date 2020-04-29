mergeInto(LibraryManager.library, {
  evalWasmThunkInJs: function (thunkAddr) {
    try {
      const evaluated = wasmWrapper.readWasmValue(thunkAddr);
      return wasmWrapper.writeWasmValue(evaluated);
    } catch (e) {
      console.error(e);
      debugger;
    }
  }
});
