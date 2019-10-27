//============ start patch.js ===================
console.log('start of patch.js');
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

const wasmTeaRecord = wrapWasmElmApp(
  EmscriptenModule.buffer,
  EmscriptenModule.asm,
  generatedAppTypes,
  jsKernelFunctions
);

var author$project$WasmWrapper$element = function(jsTeaRecord) {
  // Ignore the JS implementation and swap in the Wasm implementation
  console.log('author$project$WasmWrapper$element');
  // Provide some lines for debugger breakpoints
  const init = flags => {
    const resultWasm = wasmTeaRecord.init(flags);
    const resultJs = jsTeaRecord.init(flags);
    console.log('init', flags, '\n', resultWasm, resultJs);
    return resultWasm;
  };
  const subscriptions = model => {
    const resultWasm = wasmTeaRecord.subscriptions(model);
    const resultJs = jsTeaRecord.subscriptions(model);
    console.log('subscriptions', model, '\n', resultWasm, resultJs);
    return resultWasm;
  };
  const update = F2(function(msg, model) {
    const resultWasm = A2(wasmTeaRecord.update, msg, model);
    const resultJs = A2(jsTeaRecord.update, msg, model);
    console.log('update', msg, model, '\n', resultWasm, resultJs);
    return resultWasm;
  });
  const view = model => {
    const resultWasm = wasmTeaRecord.view(model);
    const resultJs = jsTeaRecord.view(model);
    console.log('view', model, '\n', resultWasm, resultJs);
    return resultWasm;
  };

  return _Browser_element({
    init,
    subscriptions,
    update,
    view
  });
};
console.log('end patch.js');
//============ end patch.js ===================
