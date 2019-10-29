/* -------------------------------------------------

An Elm app without the kernel

This is the part of the compiled JS code of Main.elm
that comes from pure Elm code only.
Kernel functions (whose names begin with underscores) are
replaced with mocks.
The plan is to manually translate this to C, compile
that C to WebAssembly, and then connect the WebAssembly
module to the _real_ Kernel using the wrapper I'm
developing.
This JavaScript gives me a good template to work with
while I'm writing the C code and the wrapper.
-------------------------------------------------*/

const util = require('util');

/***************************************
    CURRYING
****************************************/

function F(arity, fun, wrapper) {
  wrapper.a = arity;
  wrapper.f = fun;
  return wrapper;
}
function F2(fun) {
  return F(2, fun, function(a) {
    return function(b) {
      return fun(a, b);
    };
  });
}
function A2(fun, a, b) {
  return fun.a === 2 ? fun.f(a, b) : fun(a)(b);
}

/***************************************
    JS KERNEL CALLED FROM WASM
****************************************/

var _Json_succeed = function() {
  return {
    $: '_Json_succeed',
    arguments: Array.prototype.slice.call(arguments)
  };
};
var _Platform_batch = function() {
  return {
    $: '_Platform_batch',
    arguments: Array.prototype.slice.call(arguments)
  };
};
var _Process_sleep = function() {
  return {
    $: '_Process_sleep',
    arguments: Array.prototype.slice.call(arguments)
  };
};
var _Platform_leaf = function _Platform_leaf(home) {
  return function(value) {
    return { $: '_Platform_leaf', home, value };
  };
};
var _Scheduler_succeed = function() {
  return {
    $: '_Scheduler_succeed',
    arguments: Array.prototype.slice.call(arguments)
  };
};
var _Scheduler_andThen = F2(function() {
  return {
    $: '_Scheduler_andThen',
    arguments: Array.prototype.slice.call(arguments)
  };
});
var _VirtualDom_text = function() {
  return {
    $: '_VirtualDom_text',
    arguments: Array.prototype.slice.call(arguments)
  };
};
var _VirtualDom_nodeNS = F2(function(namespace, tag) {
  return F2(function(factList, kidList) {
    return { $: '_VirtualDom_nodeNS', namespace, tag, factList, kidList };
  });
});
var _VirtualDom_node = _VirtualDom_nodeNS(undefined);
var _VirtualDom_on = F2(function(key, handler) {
  return {
    $: '_VirtualDom_on',
    arguments: Array.prototype.slice.call(arguments)
  };
});

/***************************************
    GENERATED KERNEL ARRAY
****************************************/

// JS will crash if anything not defined by now
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

/***************************************
    ELM LIBRARIES
****************************************/

// Basic types
var _List_Nil = { $: '[]' };
var _Utils_Tuple0 = { $: '#0' };
var _String_fromNumber = function(x) {
  return x.toString();
};
var _Utils_Tuple2 = function(a, b) {
  return { $: '#2', a, b };
};
var _List_fromArray = function() {
  return { $: '::', arguments: Array.prototype.slice.call(arguments) };
};

// String
var elm$core$String$fromInt = _String_fromNumber;

// Platform
var elm$core$Platform$Cmd$batch = _Platform_batch;
var elm$core$Platform$Cmd$none = elm$core$Platform$Cmd$batch(_List_Nil);
var elm$core$Platform$Sub$batch = _Platform_batch;
var elm$core$Platform$Sub$none = elm$core$Platform$Sub$batch(_List_Nil);

// Task
var elm$core$Task$succeed = _Scheduler_succeed;
var elm$core$Task$command = _Platform_leaf('Task');
var elm$core$Task$andThen = _Scheduler_andThen;
var elm$core$Task$Perform = function(a) {
  return { $: 'Perform', a: a };
};
var elm$core$Task$map$inner = F2(function(func, a) {
  return elm$core$Task$succeed(func(a));
});
var elm$core$Task$map = F2(function(func, taskA) {
  var callback = elm$core$Task$map$inner(func);
  return A2(elm$core$Task$andThen, callback, taskA);
});
var elm$core$Task$perform = F2(function(toMessage, task) {
  return elm$core$Task$command(
    elm$core$Task$Perform(A2(elm$core$Task$map, toMessage, task))
  );
});

// Process
var elm$core$Process$sleep = _Process_sleep;

// Json
var elm$json$Json$Decode$succeed = _Json_succeed;

// VirtualDom
var elm$virtual_dom$VirtualDom$text = _VirtualDom_text;
var elm$virtual_dom$VirtualDom$on = _VirtualDom_on;
var elm$virtual_dom$VirtualDom$Normal = function(a) {
  return { $: 'Normal', a: a };
};

// Html
var elm$html$Html$h1 = _VirtualDom_node('h1');
var elm$html$Html$br = _VirtualDom_node('br');
var elm$html$Html$button = _VirtualDom_node('button');
var elm$html$Html$div = _VirtualDom_node('div');
var elm$html$Html$text = elm$virtual_dom$VirtualDom$text;
var elm$html$Html$Events$on = F2(function(event, decoder) {
  return A2(
    elm$virtual_dom$VirtualDom$on,
    event,
    elm$virtual_dom$VirtualDom$Normal(decoder)
  );
});
var elm$html$Html$Events$onClick = function(msg) {
  return A2(
    elm$html$Html$Events$on,
    'click',
    elm$json$Json$Decode$succeed(msg)
  );
};

// Browser
var elm$browser$Browser$element = _Browser_element;

/***************************************
    WRAPPER
****************************************/

function wrapWasmElmApp(
  wasmBuffer,
  wasmExports,
  generatedAppTypes,
  kernelFunctions
) {
  log('wrapper args', {
    wasmBuffer,
    wasmExports,
    generatedAppTypes,
    kernelFunctions
  });

  function element({ init, subscriptions, update, view }) {
    // do the actual Wasm wrapping here
    const wrapped_init = init;
    const wrapped_subscriptions = subscriptions;
    const wrapped_update = update;
    const wrapped_view = view;
    return elm$browser$Browser$element({
      init: wrapped_init,
      subscriptions: wrapped_subscriptions,
      update: wrapped_update,
      view: wrapped_view
    });
  }
  return { element };
}

/***************************************
    GENERATED WRAPPER CONFIG
****************************************/

const wasmBuffer = new ArrayBuffer(123);
const wasmExports = {
  /* stuff */
};
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

var author$project$WasmWrapper$element = function(ignored_js_tea) {
  return _Browser_element(
    wrapWasmElmApp(
      wasmBuffer,
      wasmExports,
      generatedAppTypes,
      jsKernelFunctions
    )
  );
};

/***************************************
    PROGRAM
****************************************/

var author$project$Main$SetCounter = function(a) {
  return { $: 'SetCounter', a: a };
};

var author$project$Main$init = function(_n0) {
  return _Utils_Tuple2(0, elm$core$Platform$Cmd$none);
};

var author$project$Main$funcSentToJsAndBack = F2(function(next, _n0) {
  return author$project$Main$SetCounter(next);
});

var author$project$Main$delayedSetCounter = function(next) {
  var partiallyAppliedFuncSentToJsAndBack = author$project$Main$funcSentToJsAndBack(
    next
  );
  return A2(
    elm$core$Task$perform,
    partiallyAppliedFuncSentToJsAndBack,
    elm$core$Process$sleep(1000)
  );
};

var author$project$Main$update = F2(function(msg, _n0) {
  var newModel = msg.a;
  var cmd = !newModel
    ? elm$core$Platform$Cmd$none
    : author$project$Main$delayedSetCounter(newModel - 1);
  return _Utils_Tuple2(newModel, cmd);
});

var author$project$Main$view = function(model) {
  var isZero = !model;
  var str = isZero ? 'Click the button!' : elm$core$String$fromInt(model);
  return A2(
    elm$html$Html$div,
    _List_Nil,
    _List_fromArray([
      A2(
        elm$html$Html$h1,
        _List_Nil,
        _List_fromArray([elm$html$Html$text(str)])
      ),
      isZero
        ? A2(
            elm$html$Html$button,
            _List_fromArray([
              elm$html$Html$Events$onClick(author$project$Main$SetCounter(5))
            ]),
            _List_fromArray([elm$html$Html$text('Start countdown')])
          )
        : A2(elm$html$Html$br, _List_Nil, _List_Nil)
    ])
  );
};

var author$project$Main$main = author$project$WasmWrapper$element({
  init: author$project$Main$init,
  subscriptions: function(_n0) {
    return elm$core$Platform$Sub$none;
  },
  update: author$project$Main$update,
  view: author$project$Main$view
});

/***************************************
    MOCK WRAPPER
****************************************/

function wrapWasmElmApp(
  wasmBuffer,
  wasmExports,
  generatedAppTypes,
  kernelFunctions
) {
  log('wrapper args', {
    wasmBuffer,
    wasmExports,
    generatedAppTypes,
    kernelFunctions
  });

  return {
    init: author$project$Main$init,
    subscriptions: function(_n0) {
      return elm$core$Platform$Sub$none;
    },
    update: author$project$Main$update,
    view: author$project$Main$view
  };
}

/***************************************
    TEST
****************************************/

function log(description, object) {
  console.log(`\n${description}\n`, util.inspect(object, { depth: Infinity }));
}

// called from JS, not Wasm
function _Browser_element({ init, subscriptions, update, view }) {
  return function main() {
    log('init', init(_Utils_Tuple0));
    log('subscriptions', subscriptions(0));
    log('update', A2(update, author$project$Main$SetCounter, 5));
    log('view', view(0));
  };
}

author$project$Main$main();
