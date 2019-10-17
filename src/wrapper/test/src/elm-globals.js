global._List_Nil = { $: '[]' };
global._List_Cons = function(hd, tl) {
  return { $: '::', a: hd, b: tl };
};
global._Utils_Tuple0 = { $: '#0' };
global._Utils_Tuple2 = function(a, b) {
  return { $: '#2', a: a, b: b };
};
global._Utils_Tuple3 = function(a, b, c) {
  return { $: '#3', a: a, b: b, c: c };
};
global._Utils_chr = function(c) {
  return new String(c);
};

global.F = function(arity, fun, wrapper) {
  wrapper.a = arity;
  wrapper.f = fun;
  return wrapper;
};

global.F2 = function(fun) {
  return F(2, fun, function(a) {
    return function(b) {
      return fun(a, b);
    };
  });
};
global.F3 = function(fun) {
  return F(3, fun, function(a) {
    return function(b) {
      return function(c) {
        return fun(a, b, c);
      };
    };
  });
};
global.F4 = function(fun) {
  return F(4, fun, function(a) {
    return function(b) {
      return function(c) {
        return function(d) {
          return fun(a, b, c, d);
        };
      };
    };
  });
};

global.A2 = function(fun, a, b) {
  return fun.a === 2 ? fun.f(a, b) : fun(a)(b);
};
global.A3 = function(fun, a, b, c) {
  return fun.a === 3 ? fun.f(a, b, c) : fun(a)(b)(c);
};
global.A4 = function(fun, a, b, c, d) {
  return fun.a === 4 ? fun.f(a, b, c, d) : fun(a)(b)(c)(d);
};
