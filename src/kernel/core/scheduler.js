const TASK_SUCCEED = 0;
const TASK_FAIL = 1;
const TASK_BINDING = 2;
const TASK_AND_THEN = 3;
const TASK_ON_ERROR = 4;
const TASK_RECEIVE = 5;

// TASKS

function _Scheduler_succeed(value) {
  return {
    $: TASK_SUCCEED,
    a: value
  };
}

function _Scheduler_fail(error) {
  return {
    $: TASK_FAIL,
    a: error // value
  };
}

function _Scheduler_binding(callback) {
  return {
    $: TASK_BINDING,
    b: callback,
    c: null // kill
  };
}

var _Scheduler_andThen = F2(function (callback, task) {
  return {
    $: TASK_AND_THEN,
    b: callback,
    d: task
  };
});

var _Scheduler_onError = F2(function (callback, task) {
  return {
    $: TASK_ON_ERROR,
    b: callback,
    d: task
  };
});

function _Scheduler_receive(callback) {
  return {
    $: TASK_RECEIVE,
    b: callback
  };
}

// PROCESSES

// Used in lots of JS kernel code: Http, Time, Browser...
function _Scheduler_rawSpawn(task) {
  return wasmWrapper.call(wasmWrapper.Scheduler_rawSpawn, [task])
}

// Used by Browser.js for `on`
function _Scheduler_spawn(task) {
  return wasmWrapper.call(wasmWrapper.Scheduler_spawn, [task])
}

/*
Remaining functions are not needed in JS, only Wasm
_Scheduler_rawSend: Only used in _Platform_dispatchEffects (Wasm)
_Scheduler_send: Only used in _Platform_sendToSelf (Wasm)
_Scheduler_kill: Not used by any elm or elm-explorations JS kernel code
_Scheduler_enqueue: internal to Scheduler
_Scheduler_step: internal to Scheduler
*/
