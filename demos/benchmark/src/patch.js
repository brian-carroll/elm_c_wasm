//============ start patch.js ===================
function wrap(exp) {
  return function() {
    try {
      return exp.apply(null, arguments);
    } catch (e) {
      Module.asm._dump();
      console.error(e);
      debugger;
    }
  };
}

var author$project$Main$wasmCount = wrap(Module.asm._export_count);
var author$project$Main$wasmCountNoTCE = wrap(Module.asm._export_count_no_tce);
var author$project$Main$wasmAdd = F2(wrap(Module.asm._export_add));
var author$project$Main$wasmAddUnboxed = F2(
  wrap(Module.asm._export_add_unboxed)
);
//============ end patch.js ===================
