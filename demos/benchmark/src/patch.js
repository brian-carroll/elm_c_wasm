//============ start patch.js ===================
var author$project$Main$wasmCount = function wasmCount(remaining) {
  return Module.ccall('export_count', 'number', ['number'], [remaining]);
};
var author$project$Main$wasmAdd = F2(function wasmAdd(a, b) {
  return Module.ccall('export_add', 'number', ['number', 'number'], [a, b]);
});
var author$project$Main$wasmAddUnboxed = F2(function wasmAddUnboxed(a, b) {
  return Module.ccall(
    'export_add_unboxed',
    'number',
    ['number', 'number'],
    [a, b]
  );
});

//============ end patch.js ===================
