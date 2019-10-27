//============ start patch.js ===================
const generatedAppTypes = {
  ctors: {
    GetTime: 0,
    GotTime: 1,
    Normal: 2,
    Perform: 3,
    Posix: 4,
    0: 'GetTime',
    1: 'GotTime',
    2: 'Normal',
    3: 'Perform',
    4: 'Posix'
  },
  fields: {
    init: 1,
    subscriptions: 2,
    update: 3,
    view: 4,
    0: 'init',
    1: 'subscriptions',
    2: 'update',
    3: 'view'
  },
  fieldGroupNames: ['init$subscriptions$update$view']
};

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

var author$project$WasmWrapper$element = function(_jsTeaRecord) {
  // Ignore the JS implementation and swap in the Wasm implementation
  const wasmTeaRecord = wrapWasmElmApp(
    Module.buffer,
    Module.asm,
    generatedAppTypes,
    jsKernelFunctions
  );
  return _Browser_element(wasmTeaRecord);
};
//============ end patch.js ===================
