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

  jsRefToWasmRecord: function(jsRefId) {
    return wasmWrapper.jsRefToWasmRecord(jsRefId);
  },

  Wrapper_setupOutgoingPort: function () {
    return wasmWrapper.setupOutgoingPort();
  },

  Wrapper_setupIncomingPort: function (managerId) {
    return wasmWrapper.setupIncomingPort(managerId);
  },

  Wrapper_sleep: function (time) {
    return wasmWrapper.Wrapper_sleep(time);
  }
});
