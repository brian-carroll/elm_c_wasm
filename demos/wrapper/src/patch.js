//============ start patch.js ===================

/*
 * Replace the JS implementation of the app with the
 * Wasm implementation
 * Also provide some app-specific configuration to the wrapper
 * The compiler will generate all of this eventually.
 * Note: Wasm likes everything to be a number, so arrays are handy.
 */

// Wrapper config for app-specific types
// - JS names of custom type constructors. Array index = Wasm ctor ID
// - JS names of record field names. Array index = Wasm field ID
// - All "field groups" (concrete Record types) used in the app
//    Order must match a corresponding C array
const appTypes = {
  ctors: ['GetTime', 'GotTime', 'Normal', 'Perform', 'Posix'],
  fields: ['init', 'subscriptions', 'update', 'view'],
  fieldGroupNames: ['init$subscriptions$update$view']
};

// Wrapper config for interfacing to the Elm Kernel in JS
// C code refers to JS Kernel functions by index number,
// using a corresponding C enum that must be in the same order
// Build script must insert this code after Kernel definitions
const jsKernelFunctions = [
  _Json_succeed,
  _Platform_batch,
  _Platform_leaf,
  _Scheduler_andThen,
  _Scheduler_succeed,
  _Time_now,
  _VirtualDom_node,
  _VirtualDom_on,
  _VirtualDom_text
];

// Apply the wrapper to the Wasm module, passing the config params too
// wrapWasmElmApp must be in scope where this code is inserted.
const wasmTeaRecord = wrapWasmElmApp(
  EmscriptenModule.buffer,
  EmscriptenModule.asm,
  appTypes,
  jsKernelFunctions
);

// Swap in the wrapped Wasm app instead of the generated JS app
//
// Override a `var` in Elm's generated JS before it gets used.
// We're relying on this code being inserted in the right place by the
// build script `build-combine.sh`. That's _after_ the original `var`,
// but _before_ author$project$Main$main
var author$project$WasmWrapper$element = function(jsTeaRecord) {
  // Ignore the JS app implementation and swap in the Wasm implementation
  let init;
  let subscriptions;
  let update;
  let view;
  const debug = false;
  if (!debug) {
    init = wasmTeaRecord.init;
    subscriptions = wasmTeaRecord.subscriptions;
    update = wasmTeaRecord.update;
    view = wasmTeaRecord.view;
  } else {
    // Log out some debug info and provide handy places to put breakpoints
    init = flags => {
      const resultWasm = wasmTeaRecord.init(flags);
      const resultJs = jsTeaRecord.init(flags);
      console.log('init', flags, '\n', resultWasm, resultJs);
      return resultWasm;
    };
    subscriptions = model => {
      const resultWasm = wasmTeaRecord.subscriptions(model);
      const resultJs = jsTeaRecord.subscriptions(model);
      console.log('subscriptions', model, '\n', resultWasm, resultJs);
      return resultWasm;
    };
    update = F2(function(msg, model) {
      const resultWasm = A2(wasmTeaRecord.update, msg, model);
      const resultJs = A2(jsTeaRecord.update, msg, model);
      console.log('update', msg, model, '\n', resultWasm, resultJs);
      return resultWasm;
    });
    view = model => {
      const resultWasm = wasmTeaRecord.view(model);
      const resultJs = jsTeaRecord.view(model);
      console.log('view', model, '\n', resultWasm, resultJs);
      return resultWasm;
    };
  }
  // As promised in WasmWrapper.elm, we use Browser.element
  // to make a `Program` out of the app's TEA functions.
  return _Browser_element({
    init,
    subscriptions,
    update,
    view
  });
};
//============ end patch.js ===================
