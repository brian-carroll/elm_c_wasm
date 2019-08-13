//============ start patch.js ===================
author$project$Main$wasmCount = function wasmCount(remaining) {
  return Module.ccall('export_count', 'number', ['number'], [remaining]);
};
//============ end patch.js ===================
