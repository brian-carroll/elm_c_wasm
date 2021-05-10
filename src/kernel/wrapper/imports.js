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
    // The C main can do a GC, but wasmWrapper is not ready at that point
    wasmWrapper && wasmWrapper.markJsRef(jsRefId);
  },

  sweepJsRefs: function (isFullGc) {
    // The C main can do a GC, but wasmWrapper is not ready at that point
    wasmWrapper && wasmWrapper.sweepJsRefs(isFullGc);
  },

  parseFloat: function (addr, len16) {
    return wasmWrapper && wasmWrapper.parseFloat(addr, len16);
  }
});
