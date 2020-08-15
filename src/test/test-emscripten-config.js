var wasmWrapper;
Module = {
  arguments: typeof process !== 'undefined'
    ? process.argv.slice(2)
    : new URL(window.location).searchParams.get('argv') || '',
  postRun: [],
  preRun: function () {
    const wasmBuffer = Module.buffer;
    const wasmExports = Module.asm;
    const generatedAppTypes = {
      ctors: ["CTOR_Nothing", "CTOR_Just", "CTOR_Ok", "CTOR_Err", "CTOR_Failure", "CTOR_Field", "CTOR_Index", "CTOR_OneOf"],
      fields: [],
      fieldGroups: [],
    };
    const kernelFuncRecord = {};
    wasmWrapper = wrapWasmElmApp(wasmBuffer, wasmExports, generatedAppTypes, kernelFuncRecord);
  },
  print: function (text) {
    if (arguments.length > 1)
      text = Array.prototype.slice.call(arguments).join(' ');
    console.log(text);
  },
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


function _List_Cons(hd, tl) { return { $: '::', a: hd, b: tl }; }
var _List_Nil = { $: '[]' };
var _Utils_Tuple0 = { $: '#0' };
function _Utils_Tuple2(a, b) { return { $: '#2', a: a, b: b }; }
function _Utils_Tuple3(a, b, c) { return { $: '#3', a: a, b: b, c: c }; }
function _Utils_chr(c) { return new String(c); }

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
