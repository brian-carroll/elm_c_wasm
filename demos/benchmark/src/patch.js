//============ start patch.js ===================
author$project$Main$countUnboxed = function wasmCount(remaining) {
  return Module.ccall('export_count', 'number', ['number'], [remaining]);
};
//============ end patch.js ===================
