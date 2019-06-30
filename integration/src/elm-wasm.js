var update = F2(function(msg, model) {
  var address = writeToWasm([msg, model]);
  var modelCmdTupleAddr = wasm.exports.update.apply(null, address);
  var len = wasm.exports.lastReturnValJsonLength();
  var modelCmdTuple = readFromWasm(modelCmdTupleAddr, len);
  return modelCmdTuple;
});

function readFromWasm(startAddr, len) {
  var endAddr = startAddr + len;
  // copy stuff or whatever
  return JSON.parse(stuff);
}

function writeToWasm(jsValue) {
  var jsonString = JSON.stringify(jsValue);
  var bytes = new ByteThingy(jsonString); // UInt8Array or whatever the hell it is
  var startAddr = wasm.exports.receiveJsonStart(bytes.length);
  var endAddr = startAddr + bytes.length;
  for (var i = startAddr; i < endAddr; i++) {
    wasm.memory[i] = bytes[i];
  }
  wasm.exports.receiveJsonEnd(startAddr);
  return startAddr;
}
