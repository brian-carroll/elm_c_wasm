/*

import Elm.Kernel.Utils exposing (Tuple0)

*/

const __1_SUCCEED = 0;
const __1_FAIL = 1;
const __1_BINDING = 2;
const __1_AND_THEN = 3;
const __1_ON_ERROR = 4;
const __1_RECEIVE = 5;
const __2_PROCESS = 0;

// TASKS

function _Scheduler_succeed(value) {
  return {
    $: __1_SUCCEED,
    __value: value
  };
}

function _Scheduler_fail(error) {
  return {
    $: __1_FAIL,
    __value: error
  };
}

function _Scheduler_binding(callback) {
  return {
    $: __1_BINDING,
    __callback: callback,
    __kill: null
  };
}

var _Scheduler_andThen = F2(function (callback, task) {
  return {
    $: __1_AND_THEN,
    __callback: callback,
    __task: task
  };
});

var _Scheduler_onError = F2(function (callback, task) {
  return {
    $: __1_ON_ERROR,
    __callback: callback,
    __task: task
  };
});

function _Scheduler_receive(callback) {
  return {
    $: __1_RECEIVE,
    __callback: callback
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
