//============ start patch.js ===================
var catch_wasm_exceptions = false; // checking for exceptions helps with debug but slows things down
if (catch_wasm_exceptions) {
  function wrap(exp) {
    return function() {
      try {
        return exp.apply(null, arguments);
      } catch (e) {
        Module.asm._dump(); // log the state of the Wasm heap & GC
        console.error(e);
        debugger;
      }
    };
  }
  author$project$Main$wasmCount = wrap(Module.asm._export_count);
  author$project$Main$wasmCountNoTCE = wrap(Module.asm._export_count_no_tce);
  author$project$Main$wasmAdd = F2(wrap(Module.asm._export_add));
  author$project$Main$wasmAddUnboxed = F2(wrap(Module.asm._export_add_unboxed));
} else {
  author$project$Main$wasmCount = Module.asm._export_count;
  author$project$Main$wasmCountNoTCE = Module.asm._export_count_no_tce;
  author$project$Main$wasmAdd = F2(Module.asm._export_add);
  author$project$Main$wasmAddUnboxed = F2(Module.asm._export_add_unboxed);
}
//============ end patch.js ===================
