// TASKS

function _Scheduler_succeed(value) {
  const t = new wasmWrapper.Task();
  t.$ = 'SUCCEED';
  t.value = value;
  return t;
}

function _Scheduler_fail(error) {
  const t = new wasmWrapper.Task();
  t.$ = 'FAIL';
  t.value = error;
  return t;
}

function _Scheduler_binding(callback) {
  const t = new wasmWrapper.Task();
  t.$ = 'BINDING';
  t.callback = callback;
  t.kill = null;
  return t;
}

var _Scheduler_andThen = F2(function (callback, task) {
  const t = new wasmWrapper.Task();
  t.$ = 'AND_THEN';
  t.callback = callback;
  t.task = task;
  return t;
});

var _Scheduler_onError = F2(function (callback, task) {
  const t = new wasmWrapper.Task();
  t.$ = 'ON_ERROR';
  t.callback = callback;
  t.task = task;
  return t;
});

function _Scheduler_receive(callback) {
  const t = new wasmWrapper.Task();
  t.$ = 'RECEIVE';
  t.callback = callback;
  return t;
}

// PROCESSES

// Used in lots of JS kernel code: Http, Time, Browser...
function _Scheduler_rawSpawn(task) {
  return wasmWrapper.call(wasmWrapper.Scheduler_rawSpawn, [task]);
}

// Used by Browser.js `on`
function _Scheduler_spawn(task) {
  return wasmWrapper.call(wasmWrapper.Scheduler_spawn, [task]);
}

/*
Remaining functions are not needed in JS, only Wasm
_Scheduler_rawSend: Only used in _Platform_dispatchEffects (Wasm)
_Scheduler_send: Only used in _Platform_sendToSelf (Wasm)
_Scheduler_kill: Not used by any elm or elm-explorations JS kernel code
_Scheduler_enqueue: internal to Scheduler
_Scheduler_step: internal to Scheduler
*/
