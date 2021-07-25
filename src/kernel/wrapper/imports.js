mergeInto(LibraryManager.library, {
  getJsRefArrayIndex: function (jsRefId, index) {
    return wasmWrapper.getJsRefArrayIndex(jsRefId, index);
  },

  getJsRefObjectField: function (jsRefId, fieldStringAddr) {
    return wasmWrapper.getJsRefObjectField(jsRefId, fieldStringAddr);
  },

  getJsRefValue: function (jsRefId) {
    return wasmWrapper.getJsRefValue(jsRefId);
  },

  markJsRef: function (jsRefId) {
    // The C main function can do a GC while wasmWrapper is still undefined
    wasmWrapper && wasmWrapper.markJsRef(jsRefId);
  },

  sweepJsRefs: function (isFullGc) {
    // The C main function can do a GC while wasmWrapper is still undefined
    wasmWrapper && wasmWrapper.sweepJsRefs(isFullGc);
  },

  parseFloat: function (addr, len16) {
    return wasmWrapper && wasmWrapper.parseFloat(addr, len16);
  },

  jsStepper: function (viewMetadataAddr) {
    wasmWrapper.wasmImportStepper(viewMetadataAddr);
  },

  applyJsRef: function (jsRefId, nArgs, argsAddr) {
    return wasmWrapper.applyJsRef(jsRefId, nArgs, argsAddr);
  },

  Wrapper_sleep: function (time) {
    const task = _Scheduler_binding(function (callback) {
      var id = setTimeout(function () {
        callback(_Scheduler_succeed(_Utils_Tuple0));
      }, time);

      return function () {
        clearTimeout(id);
      };
    });
    const taskAddr = wasmWrapper.writeWasmValue(task);
    return taskAddr;
  }
});
