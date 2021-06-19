/*

import Elm.Kernel.Json exposing (run, wrap)
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
  // This isn't working for some reason
  // var result = A2(
  //   _Json_run,
  //   flagDecoder,
  //   _Json_wrap(args ? args['flags'] : undefined)
  // );
  // if (result.$ !== 'Ok' && result.$ !== 0) { // isOk is not working
  //   throw new Error('https://github.com/elm/core/blob/1.0.0/hints/2.md');
  // }
  var flags = undefined; // TODO
  var result = {
    $: 'Ok',
    a: { $: 0, a: flags }
  };

  // Call JS wrapper around Wasm init. Stores resulting tuple in wasm.
  var dummyPair = init(result.a);

  var stepper = stepperBuilder(sendToApp, dummyPair.a);
  function sendToApp(msg, viewMetadata) {
    wasmWrapper.call(wasmWrapper.sendToApp_revArgs, [viewMetadata, msg]);
  }

  var portsList = wasmWrapper.Platform_initializeEffects(stepper);
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

var $elm$core$Task$command = _Platform_leaf('Task');

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
    if (name in obj) {
      if (name === 'init') {
        throw new Error('https://github.com/elm/core/blob/1.0.0/hints/6.md');
      } else {
        _Platform_mergeExportsProd(obj[name], exports[name]);
      }
    } else {
      obj[name] = exports[name];
    }
  }
}
