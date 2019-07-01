//============ start patch.js ===================
author$project$Main$update = F2(function wasmUpdate(msg, model) {
  return Module.ccall('update', 'number', ['number', 'number'], [msg, model]);
});
//============ end patch.js ===================
