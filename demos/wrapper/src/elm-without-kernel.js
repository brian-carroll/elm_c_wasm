const util = require('util');

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
var _Utils_Tuple0 = { $: '#0' };

var _Json_succeed = function() {
  return {
    $: '_Json_succeed',
    arguments: Array.prototype.slice.call(arguments)
  };
};
var _String_fromNumber = function(x) {
  return x.toString();
};
var _List_fromArray = function() {
  return { $: '::', arguments: Array.prototype.slice.call(arguments) };
};
var _Platform_batch = function() {
  return {
    $: '_Platform_batch',
    arguments: Array.prototype.slice.call(arguments)
  };
};
var _List_Nil = { $: '[]' };

var _Time_now = function() {
  return { $: '_Time_now', arguments: Array.prototype.slice.call(arguments) };
};
var _Utils_Tuple2 = function(a, b) {
  return { $: '#2', a, b };
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
var elm$json$Json$Decode$succeed = _Json_succeed;
var elm$core$Task$succeed = _Scheduler_succeed;

// ----------- INIT -------------------
var elm$core$Task$command = _Platform_leaf('Task');
var elm$core$Task$Perform = function(a) {
  return { $: 'Perform', a: a };
};
var elm$core$Task$andThen = _Scheduler_andThen;
var elm$core$Task$map = F2(function(func, taskA) {
  return A2(
    elm$core$Task$andThen,
    function(a) {
      return elm$core$Task$succeed(func(a));
    },
    taskA
  );
});
var elm$time$Time$Posix = function(a) {
  return { $: 'Posix', a: a };
};
var elm$time$Time$millisToPosix = elm$time$Time$Posix;
var author$project$Main$GotTime = function(a) {
  return { $: 'GotTime', a: a };
};
var elm$time$Time$now = _Time_now(elm$time$Time$millisToPosix);
var elm$core$Task$perform = F2(function(toMessage, task) {
  return elm$core$Task$command(
    elm$core$Task$Perform(A2(elm$core$Task$map, toMessage, task))
  );
});
var author$project$Main$cmdTime = A2(
  elm$core$Task$perform,
  author$project$Main$GotTime,
  elm$time$Time$now
);

var author$project$Main$init = function(_n0) {
  return _Utils_Tuple2(0, author$project$Main$cmdTime);
};

// ------------- UPDATE -----------------
var elm$core$Platform$Cmd$batch = _Platform_batch;
var elm$core$Platform$Cmd$none = elm$core$Platform$Cmd$batch(_List_Nil);

var elm$time$Time$posixToMillis = function(_n0) {
  var millis = _n0.a;
  return millis;
};

var author$project$Main$update = F2(function(msg, model) {
  if (msg.$ === 'GetTime') {
    return _Utils_Tuple2(model, author$project$Main$cmdTime);
  } else {
    var posix = msg.a;
    return _Utils_Tuple2(
      elm$time$Time$posixToMillis(posix),
      elm$core$Platform$Cmd$none
    );
  }
});
// ---------------- VIEW ---------------
var author$project$Main$GetTime = { $: 'GetTime' };
var elm$html$Html$div = _VirtualDom_node('div');
var elm$html$Html$br = _VirtualDom_node('br');
var elm$html$Html$button = _VirtualDom_node('button');
var elm$virtual_dom$VirtualDom$text = _VirtualDom_text;
var elm$html$Html$text = elm$virtual_dom$VirtualDom$text;
var elm$virtual_dom$VirtualDom$Normal = function(a) {
  return { $: 'Normal', a: a };
};
var elm$virtual_dom$VirtualDom$on = _VirtualDom_on;
var elm$core$String$fromInt = _String_fromNumber;
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

var author$project$Main$view = function(model) {
  return A2(
    elm$html$Html$div,
    _List_Nil,
    _List_fromArray([
      elm$html$Html$text(elm$core$String$fromInt(model)),
      A2(elm$html$Html$br, _List_Nil, _List_Nil),
      A2(
        elm$html$Html$button,
        _List_fromArray([
          elm$html$Html$Events$onClick(author$project$Main$GetTime)
        ]),
        _List_fromArray([elm$html$Html$text('Refresh')])
      )
    ])
  );
};
// ---------------- SUBSCRIPTIONS -------------
var elm$core$Platform$Sub$batch = _Platform_batch;
var elm$core$Platform$Sub$none = elm$core$Platform$Sub$batch(_List_Nil);

// ----------------- MAIN ---------------------
var author$project$Main$main = author$project$WasmWrapper$element({
  init: author$project$Main$init,
  subscriptions: function(_n0) {
    return elm$core$Platform$Sub$none;
  },
  update: author$project$Main$update,
  view: author$project$Main$view
});
// --------------- TEST -----------------
function log(description, object) {
  console.log(description, util.inspect(object, { depth: Infinity }));
}
function author$project$WasmWrapper$element({
  init,
  subscriptions,
  update,
  view
}) {
  log('\ninit\n', init(_Utils_Tuple0));
  log('\nsubscriptions\n', subscriptions(0));
  log('\nupdate Get\n', A2(update, author$project$Main$GetTime, 0));
  log('\nupdate Got\n', A2(update, author$project$Main$GotTime(Date.now()), 0));
  log('\nview\n', view(0));
}
