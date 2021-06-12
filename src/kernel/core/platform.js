/*

import Elm.Kernel.Debug exposing (crash)
import Elm.Kernel.Json exposing (run, wrap, errorToString)
import Result exposing (isOk)

*/

// PROGRAMS

var _Platform_worker = F4(function (impl, flagDecoder, debugMetadata, args) {
  return _Platform_initialize(
    flagDecoder,
    args,
    impl.init,
    impl.update,
    impl.subscriptions,
    function () {
      return function () {};
    }
  );
});

// INITIALIZE A PROGRAM

function _Platform_initialize(
  flagDecoder,
  args,
  init,
  update,
  subscriptions,
  stepperBuilder
) {
  // flagDecoder is an compiler-generated JS function. Keep the decoding in JS
  var result = A2(
    _Json_run,
    flagDecoder,
    _Json_wrap(args ? args['flags'] : undefined)
  );
  _Result_isOk(result) || _Debug_crash(2, _Json_errorToString(result.a));

  // Call JS wrapper around Wasm init. Stores resulting tuple in wasm.
  var dummyPair = init(result.a);
  var stepper = stepperBuilder(wasmWrapper.sendToApp, dummyPair.a);

  var portsList = wasmWrapper.initializeEffects(stepper);
  var hasPorts = !!portsList.b;
  var ports = {};
  for (; portsList.b; portsList = portsList.b) {
    var portPair = portsList.a;
    var name = portPair.a;
    var port = portPair.b;
    ports[name] = port;
  }

  return hasPorts ? { ports: ports } : {};
}

// TRACK PRELOADS
//
// This is used by code in elm/browser and elm/http
// to register any HTTP requests that are triggered by init.
//

var _Platform_preload;

// ROUTING

const MANAGER_MSG_SELF = 0;
const MANAGER_MSG_LEAF = 1;
const MANAGER_MSG_NODE = 2;
const MANAGER_MSG_MAP = 3;
const MANAGER_MSG_FX = 4;

/*
TODO: how do I get a router out to JS?

Where does the JS get it in the first place? Effect manager Elm code for onEffects and onSelfMsg take a router.
I need to use process Id for this and make a list of them or whatever



_Http_toTask accepts a router (Http.js v2)

*/
var _Platform_sendToApp = F2(function (router, msg) {
  return wasmWrapper.call(wasmWrapper.Platform_sendToApp, [router, msg]);
});

var _Platform_sendToSelf = F2(function (router, msg) {
  return wasmWrapper.call(wasmWrapper.Platform_sendToSelf, [router, msg]);
});

// BAGS

function _Platform_leaf(home) {
  return function (value) {
    return {
      $: MANAGER_MSG_LEAF,
      a: wasmWrapper.managerNames.indexOf(home),
      b: value
    };
  };
}

function _Platform_batch(list) {
  return {
    $: MANAGER_MSG_NODE,
    a: list
  };
}

var _Platform_map = F2(function (tagger, bag) {
  return {
    $: MANAGER_MSG_MAP,
    a: tagger,
    b: bag
  };
});

// EXPORT ELM MODULES

function _Platform_export(exports) {
  scope['Elm']
    ? _Platform_mergeExportsProd(scope['Elm'], exports)
    : (scope['Elm'] = exports);
}

function _Platform_mergeExportsProd(obj, exports) {
  for (var name in exports) {
    name in obj
      ? name == 'init'
        ? _Debug_crash(6)
        : _Platform_mergeExportsProd(obj[name], exports[name])
      : (obj[name] = exports[name]);
  }
}
