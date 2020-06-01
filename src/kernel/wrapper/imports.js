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
    return wasmWrapper.markJsRef(jsRefId);
  },

  sweepJsRefs: function (isFullGc) {
    return wasmWrapper.sweepJsRefs(isFullGc);
  }
});
