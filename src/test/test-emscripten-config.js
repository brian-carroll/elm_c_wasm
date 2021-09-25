var wasmWrapper;

function _List_Cons(hd, tl) { return { $: '::', a: hd, b: tl }; }
var _List_Nil = { $: '[]' };
var _Utils_Tuple0 = { $: '#0' };
function _Utils_Tuple2(a, b) { return { $: '#2', a: a, b: b }; }
function _Utils_Tuple3(a, b, c) { return { $: '#3', a: a, b: b, c: c }; }
function _Utils_chr(c) { return new String(c); }

function F(arity, fun, wrapper) {
  wrapper.a = arity;
  wrapper.f = fun;
  return wrapper;
}
function F2(fun) {
  return F(2, fun, function(a) { return function(b) { return fun(a,b); }; })
}
function F3(fun) {
  return F(3, fun, function(a) {
    return function(b) { return function(c) { return fun(a, b, c); }; };
  });
}
function F4(fun) {
  return F(4, fun, function(a) { return function(b) { return function(c) {
    return function(d) { return fun(a, b, c, d); }; }; };
  });
}
function F5(fun) {
  return F(5, fun, function(a) { return function(b) { return function(c) {
    return function(d) { return function(e) { return fun(a, b, c, d, e); }; }; }; };
  });
}
function F6(fun) {
  return F(6, fun, function(a) { return function(b) { return function(c) {
    return function(d) { return function(e) { return function(f) {
    return fun(a, b, c, d, e, f); }; }; }; }; };
  });
}
function F7(fun) {
  return F(7, fun, function(a) { return function(b) { return function(c) {
    return function(d) { return function(e) { return function(f) {
    return function(g) { return fun(a, b, c, d, e, f, g); }; }; }; }; }; };
  });
}
function F8(fun) {
  return F(8, fun, function(a) { return function(b) { return function(c) {
    return function(d) { return function(e) { return function(f) {
    return function(g) { return function(h) {
    return fun(a, b, c, d, e, f, g, h); }; }; }; }; }; }; };
  });
}
function F9(fun) {
  return F(9, fun, function(a) { return function(b) { return function(c) {
    return function(d) { return function(e) { return function(f) {
    return function(g) { return function(h) { return function(i) {
    return fun(a, b, c, d, e, f, g, h, i); }; }; }; }; }; }; }; };
  });
}

// Used in test-imports, not in wrapper
function A2(fun, a, b) {
  return fun.a === 2 ? fun.f(a, b) : fun(a)(b);
}
function A3(fun, a, b, c) {
  return fun.a === 3 ? fun.f(a, b, c) : fun(a)(b)(c);
}
function A4(fun, a, b, c, d) {
  return fun.a === 4 ? fun.f(a, b, c, d) : fun(a)(b)(c)(d);
}
function A5(fun, a, b, c, d, e) {
  return fun.a === 5 ? fun.f(a, b, c, d, e) : fun(a)(b)(c)(d)(e);
}
function A6(fun, a, b, c, d, e, f) {
  return fun.a === 6 ? fun.f(a, b, c, d, e, f) : fun(a)(b)(c)(d)(e)(f);
}
function A7(fun, a, b, c, d, e, f, g) {
  return fun.a === 7 ? fun.f(a, b, c, d, e, f, g) : fun(a)(b)(c)(d)(e)(f)(g);
}
function A8(fun, a, b, c, d, e, f, g, h) {
  return fun.a === 8 ? fun.f(a, b, c, d, e, f, g, h) : fun(a)(b)(c)(d)(e)(f)(g)(h);
}
function A9(fun, a, b, c, d, e, f, g, h, i) {
  return fun.a === 9 ? fun.f(a, b, c, d, e, f, g, h, i) : fun(a)(b)(c)(d)(e)(f)(g)(h)(i);
}


var elmImports = {_List_Cons: _List_Cons, _List_Nil: _List_Nil, _Utils_Tuple0: _Utils_Tuple0, _Utils_Tuple2: _Utils_Tuple2, _Utils_Tuple3: _Utils_Tuple3, _Utils_chr: _Utils_chr, F2: F2, F3: F3, F4: F4, F5: F5, F6: F6, F7: F7, F8: F8, F9: F9};

const arguments = typeof process !== 'undefined'
  ? process.argv.slice(2)
  : [new URL(window.location).searchParams.get('argv') || '-av'];

console.log({ arguments })

Module = {
  arguments,
  postRun: [],
  onRuntimeInitialized: function () {
    const generatedAppTypes = {
      ctors: ["Nothing", "Just", "Ok", "Err", "Failure", "Field", "Index", "OneOf"],
      fields: ["init", "subscriptions", "update", "view"],
      fieldGroups: ["init subscriptions update view"],
    };
    const kernelImports = [];
    const managerNames = ['Task'];
    wasmWrapper = wrapWasmElmApp(Module, elmImports, generatedAppTypes, kernelImports, managerNames);
  },
  print: (function() {
    var element;
    if (typeof document !== 'undefined') {
      element = document.getElementById('output');
      if (element) element.value = ''; // clear browser cache
    }
    return function (text) {
      if (arguments.length > 1)
        text = Array.prototype.slice.call(arguments).join(' ');
      console.log(text);
      if (element) {
        element.textContent += text + '\n';
      }
    }
  })(),
  printErr: function (text) {
    if (arguments.length > 1)
      text = Array.prototype.slice.call(arguments).join(' ');
    if (0) {
      // XXX disabled for safety typeof dump == 'function') {
      dump(text + '\n'); // fast, straight to the real console
    } else {
      console.error(text);
    }
  },
  setStatus: function (text) {
    if (!Module.setStatus.last)
      Module.setStatus.last = { time: Date.now(), text: '' };
    if (text === Module.setStatus.last.text) return;
    var m = text.match(/([^(]+)\((\d+(\.\d+)?)\/(\d+)\)/);
    var now = Date.now();
    if (m && now - Module.setStatus.last.time < 30) return; // if this is a progress update, skip it if too soon
    Module.setStatus.last.time = now;
    Module.setStatus.last.text = text;
    console.log(text);
  },
  totalDependencies: 0,
  monitorRunDependencies: function (left) {
    this.totalDependencies = Math.max(this.totalDependencies, left);
    Module.setStatus(
      left
        ? 'Preparing... (' +
            (this.totalDependencies - left) +
            '/' +
            this.totalDependencies +
            ')'
        : 'All downloads complete.'
    );
  }
};
if (typeof process !== 'undefined') {
  process.on('uncaughtException', err => {
    console.error(err);
    process.exit(1); //mandatory (as per the Node.js docs)
  });  
}
