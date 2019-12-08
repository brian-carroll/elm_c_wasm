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
  ctors: ['Normal', 'Perform', 'Posix', 'SetCounter'],
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
  _Process_sleep,
  _VirtualDom_node,
  _VirtualDom_on,
  _VirtualDom_text
];

// Apply the wrapper to the Wasm module, passing the config params too
// wrapWasmElmApp must be in scope where this code is inserted.
const wasmWrapper = wrapWasmElmApp(
  EmscriptenModule.buffer, // The `ArrayBuffer` memory block shared between JS and Wasm
  EmscriptenModule.asm, // Object of exported functions from the Wasm module
  appTypes, // App-specific type info passed from Elm compiler to this wrapper
  jsKernelFunctions // Array of all JS kernel functions called by the Elm Wasm module
);

// In the real build, we can just assign to main instead of doing this.
// But for the Bash build, text insertion is easier than text replacement
var author$project$WasmWrapper$element = function(_) {
  return wasmWrapper.mains[0];
};
//============ end patch.js ===================
