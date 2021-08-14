(function(scope){
'use strict';

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

console.warn('Compiled in DEV mode. Follow the advice at https://elm-lang.org/0.19.1/optimize for better performance and smaller assets.');
// Begin Kernel $elm$kernel$List$$ deps: $elm$kernel$Utils$$, $elm$core$Basics$EQ, $elm$core$Basics$LT



var _List_Nil_UNUSED = { $: 0 };
var _List_Nil = { $: '[]' };

function _List_Cons_UNUSED(hd, tl) { return { $: 1, a: hd, b: tl }; }
function _List_Cons(hd, tl) { return { $: '::', a: hd, b: tl }; }


var _List_cons = F2(_List_Cons);

function _List_fromArray(arr)
{
	var out = _List_Nil;
	for (var i = arr.length; i--; )
	{
		out = _List_Cons(arr[i], out);
	}
	return out;
}

function _List_toArray(xs)
{
	for (var out = []; xs.b; xs = xs.b) // WHILE_CONS
	{
		out.push(xs.a);
	}
	return out;
}

var _List_map2 = F3(function(f, xs, ys)
{
	for (var arr = []; xs.b && ys.b; xs = xs.b, ys = ys.b) // WHILE_CONSES
	{
		arr.push(A2(f, xs.a, ys.a));
	}
	return _List_fromArray(arr);
});

var _List_map3 = F4(function(f, xs, ys, zs)
{
	for (var arr = []; xs.b && ys.b && zs.b; xs = xs.b, ys = ys.b, zs = zs.b) // WHILE_CONSES
	{
		arr.push(A3(f, xs.a, ys.a, zs.a));
	}
	return _List_fromArray(arr);
});

var _List_map4 = F5(function(f, ws, xs, ys, zs)
{
	for (var arr = []; ws.b && xs.b && ys.b && zs.b; ws = ws.b, xs = xs.b, ys = ys.b, zs = zs.b) // WHILE_CONSES
	{
		arr.push(A4(f, ws.a, xs.a, ys.a, zs.a));
	}
	return _List_fromArray(arr);
});

var _List_map5 = F6(function(f, vs, ws, xs, ys, zs)
{
	for (var arr = []; vs.b && ws.b && xs.b && ys.b && zs.b; vs = vs.b, ws = ws.b, xs = xs.b, ys = ys.b, zs = zs.b) // WHILE_CONSES
	{
		arr.push(A5(f, vs.a, ws.a, xs.a, ys.a, zs.a));
	}
	return _List_fromArray(arr);
});

var _List_sortBy = F2(function(f, xs)
{
	return _List_fromArray(_List_toArray(xs).sort(function(a, b) {
		return _Utils_cmp(f(a), f(b));
	}));
});

var _List_sortWith = F2(function(f, xs)
{
	return _List_fromArray(_List_toArray(xs).sort(function(a, b) {
		var ord = A2(f, a, b);
		return ord === $elm$core$Basics$EQ ? 0 : ord === $elm$core$Basics$LT ? -1 : 1;
	}));
});

// End Kernel $elm$kernel$List$$ deps: $elm$kernel$Utils$$, $elm$core$Basics$EQ, $elm$core$Basics$LT

// Begin Kernel $elm$kernel$JsArray$$ deps: $elm$kernel$Utils$$



var _JsArray_empty = [];

function _JsArray_singleton(value)
{
    return [value];
}

function _JsArray_length(array)
{
    return array.length;
}

var _JsArray_initialize = F3(function(size, offset, func)
{
    var result = new Array(size);

    for (var i = 0; i < size; i++)
    {
        result[i] = func(offset + i);
    }

    return result;
});

var _JsArray_initializeFromList = F2(function (max, ls)
{
    var result = new Array(max);

    for (var i = 0; i < max && ls.b; i++)
    {
        result[i] = ls.a;
        ls = ls.b;
    }

    result.length = i;
    return _Utils_Tuple2(result, ls);
});

var _JsArray_unsafeGet = F2(function(index, array)
{
    return array[index];
});

var _JsArray_unsafeSet = F3(function(index, value, array)
{
    var length = array.length;
    var result = new Array(length);

    for (var i = 0; i < length; i++)
    {
        result[i] = array[i];
    }

    result[index] = value;
    return result;
});

var _JsArray_push = F2(function(value, array)
{
    var length = array.length;
    var result = new Array(length + 1);

    for (var i = 0; i < length; i++)
    {
        result[i] = array[i];
    }

    result[length] = value;
    return result;
});

var _JsArray_foldl = F3(function(func, acc, array)
{
    var length = array.length;

    for (var i = 0; i < length; i++)
    {
        acc = A2(func, array[i], acc);
    }

    return acc;
});

var _JsArray_foldr = F3(function(func, acc, array)
{
    for (var i = array.length - 1; i >= 0; i--)
    {
        acc = A2(func, array[i], acc);
    }

    return acc;
});

var _JsArray_map = F2(function(func, array)
{
    var length = array.length;
    var result = new Array(length);

    for (var i = 0; i < length; i++)
    {
        result[i] = func(array[i]);
    }

    return result;
});

var _JsArray_indexedMap = F3(function(func, offset, array)
{
    var length = array.length;
    var result = new Array(length);

    for (var i = 0; i < length; i++)
    {
        result[i] = A2(func, offset + i, array[i]);
    }

    return result;
});

var _JsArray_slice = F3(function(from, to, array)
{
    return array.slice(from, to);
});

var _JsArray_appendN = F3(function(n, dest, source)
{
    var destLen = dest.length;
    var itemsToCopy = n - destLen;

    if (itemsToCopy > source.length)
    {
        itemsToCopy = source.length;
    }

    var size = destLen + itemsToCopy;
    var result = new Array(size);

    for (var i = 0; i < destLen; i++)
    {
        result[i] = dest[i];
    }

    for (var i = 0; i < itemsToCopy; i++)
    {
        result[i + destLen] = source[i];
    }

    return result;
});

// End Kernel $elm$kernel$JsArray$$ deps: $elm$kernel$Utils$$

// Begin Kernel $elm$kernel$Debug$$ deps: $elm$core$Array$toList, $elm$core$Dict$toList, $elm$core$Set$toList



// LOG

var _Debug_log_UNUSED = F2(function(tag, value)
{
	return value;
});

var _Debug_log = F2(function(tag, value)
{
	console.log(tag + ': ' + _Debug_toString(value));
	return value;
});


// TODOS

function _Debug_todo(moduleName, region)
{
	return function(message) {
		_Debug_crash(8, moduleName, region, message);
	};
}

function _Debug_todoCase(moduleName, region, value)
{
	return function(message) {
		_Debug_crash(9, moduleName, region, value, message);
	};
}


// TO STRING

function _Debug_toString_UNUSED(value)
{
	return '<internals>';
}

function _Debug_toString(value)
{
	return _Debug_toAnsiString(false, value);
}

function _Debug_toAnsiString(ansi, value)
{
	if (typeof value === 'function')
	{
		return _Debug_internalColor(ansi, '<function>');
	}

	if (typeof value === 'boolean')
	{
		return _Debug_ctorColor(ansi, value ? 'True' : 'False');
	}

	if (typeof value === 'number')
	{
		return _Debug_numberColor(ansi, value + '');
	}

	if (value instanceof String)
	{
		return _Debug_charColor(ansi, "'" + _Debug_addSlashes(value, true) + "'");
	}

	if (typeof value === 'string')
	{
		return _Debug_stringColor(ansi, '"' + _Debug_addSlashes(value, false) + '"');
	}

	if (typeof value === 'object' && '$' in value)
	{
		var tag = value.$;

		if (typeof tag === 'number')
		{
			return _Debug_internalColor(ansi, '<internals>');
		}

		if (tag[0] === '#')
		{
			var output = [];
			for (var k in value)
			{
				if (k === '$') continue;
				output.push(_Debug_toAnsiString(ansi, value[k]));
			}
			return '(' + output.join(',') + ')';
		}

		if (tag === 'Set_elm_builtin')
		{
			return _Debug_ctorColor(ansi, 'Set')
				+ _Debug_fadeColor(ansi, '.fromList') + ' '
				+ _Debug_toAnsiString(ansi, $elm$core$Set$toList(value));
		}

		if (tag === 'RBNode_elm_builtin' || tag === 'RBEmpty_elm_builtin')
		{
			return _Debug_ctorColor(ansi, 'Dict')
				+ _Debug_fadeColor(ansi, '.fromList') + ' '
				+ _Debug_toAnsiString(ansi, $elm$core$Dict$toList(value));
		}

		if (tag === 'Array_elm_builtin')
		{
			return _Debug_ctorColor(ansi, 'Array')
				+ _Debug_fadeColor(ansi, '.fromList') + ' '
				+ _Debug_toAnsiString(ansi, $elm$core$Array$toList(value));
		}

		if (tag === '::' || tag === '[]')
		{
			var output = '[';

			value.b && (output += _Debug_toAnsiString(ansi, value.a), value = value.b)

			for (; value.b; value = value.b) // WHILE_CONS
			{
				output += ',' + _Debug_toAnsiString(ansi, value.a);
			}
			return output + ']';
		}

		var output = '';
		for (var i in value)
		{
			if (i === '$') continue;
			var str = _Debug_toAnsiString(ansi, value[i]);
			var c0 = str[0];
			var parenless = c0 === '{' || c0 === '(' || c0 === '[' || c0 === '<' || c0 === '"' || str.indexOf(' ') < 0;
			output += ' ' + (parenless ? str : '(' + str + ')');
		}
		return _Debug_ctorColor(ansi, tag) + output;
	}

	if (typeof DataView === 'function' && value instanceof DataView)
	{
		return _Debug_stringColor(ansi, '<' + value.byteLength + ' bytes>');
	}

	if (typeof File !== 'undefined' && value instanceof File)
	{
		return _Debug_internalColor(ansi, '<' + value.name + '>');
	}

	if (typeof value === 'object')
	{
		var output = [];
		for (var key in value)
		{
			var field = key[0] === '_' ? key.slice(1) : key;
			output.push(_Debug_fadeColor(ansi, field) + ' = ' + _Debug_toAnsiString(ansi, value[key]));
		}
		if (output.length === 0)
		{
			return '{}';
		}
		return '{ ' + output.join(', ') + ' }';
	}

	return _Debug_internalColor(ansi, '<internals>');
}

function _Debug_addSlashes(str, isChar)
{
	var s = str
		.replace(/\\/g, '\\\\')
		.replace(/\n/g, '\\n')
		.replace(/\t/g, '\\t')
		.replace(/\r/g, '\\r')
		.replace(/\v/g, '\\v')
		.replace(/\0/g, '\\0');

	if (isChar)
	{
		return s.replace(/\'/g, '\\\'');
	}
	else
	{
		return s.replace(/\"/g, '\\"');
	}
}

function _Debug_ctorColor(ansi, string)
{
	return ansi ? '\x1b[96m' + string + '\x1b[0m' : string;
}

function _Debug_numberColor(ansi, string)
{
	return ansi ? '\x1b[95m' + string + '\x1b[0m' : string;
}

function _Debug_stringColor(ansi, string)
{
	return ansi ? '\x1b[93m' + string + '\x1b[0m' : string;
}

function _Debug_charColor(ansi, string)
{
	return ansi ? '\x1b[92m' + string + '\x1b[0m' : string;
}

function _Debug_fadeColor(ansi, string)
{
	return ansi ? '\x1b[37m' + string + '\x1b[0m' : string;
}

function _Debug_internalColor(ansi, string)
{
	return ansi ? '\x1b[36m' + string + '\x1b[0m' : string;
}

function _Debug_toHexDigit(n)
{
	return String.fromCharCode(n < 10 ? 48 + n : 55 + n);
}


// CRASH


function _Debug_crash_UNUSED(identifier)
{
	throw new Error('https://github.com/elm/core/blob/1.0.0/hints/' + identifier + '.md');
}


function _Debug_crash(identifier, fact1, fact2, fact3, fact4)
{
	switch(identifier)
	{
		case 0:
			throw new Error('What node should I take over? In JavaScript I need something like:\n\n    Elm.Main.init({\n        node: document.getElementById("elm-node")\n    })\n\nYou need to do this with any Browser.sandbox or Browser.element program.');

		case 1:
			throw new Error('Browser.application programs cannot handle URLs like this:\n\n    ' + document.location.href + '\n\nWhat is the root? The root of your file system? Try looking at this program with `elm reactor` or some other server.');

		case 2:
			var jsonErrorString = fact1;
			throw new Error('Problem with the flags given to your Elm program on initialization.\n\n' + jsonErrorString);

		case 3:
			var portName = fact1;
			throw new Error('There can only be one port named `' + portName + '`, but your program has multiple.');

		case 4:
			var portName = fact1;
			var problem = fact2;
			throw new Error('Trying to send an unexpected type of value through port `' + portName + '`:\n' + problem);

		case 5:
			throw new Error('Trying to use `(==)` on functions.\nThere is no way to know if functions are "the same" in the Elm sense.\nRead more about this at https://package.elm-lang.org/packages/elm/core/latest/Basics#== which describes why it is this way and what the better version will look like.');

		case 6:
			var moduleName = fact1;
			throw new Error('Your page is loading multiple Elm scripts with a module named ' + moduleName + '. Maybe a duplicate script is getting loaded accidentally? If not, rename one of them so I know which is which!');

		case 8:
			var moduleName = fact1;
			var region = fact2;
			var message = fact3;
			throw new Error('TODO in module `' + moduleName + '` ' + _Debug_regionToString(region) + '\n\n' + message);

		case 9:
			var moduleName = fact1;
			var region = fact2;
			var value = fact3;
			var message = fact4;
			throw new Error(
				'TODO in module `' + moduleName + '` from the `case` expression '
				+ _Debug_regionToString(region) + '\n\nIt received the following value:\n\n    '
				+ _Debug_toString(value).replace('\n', '\n    ')
				+ '\n\nBut the branch that handles it says:\n\n    ' + message.replace('\n', '\n    ')
			);

		case 10:
			throw new Error('Bug in https://github.com/elm/virtual-dom/issues');

		case 11:
			throw new Error('Cannot perform mod 0. Division by zero error.');
	}
}

function _Debug_regionToString(region)
{
	if (region.start.line === region.end.line)
	{
		return 'on line ' + region.start.line;
	}
	return 'on lines ' + region.start.line + ' through ' + region.end.line;
}

// End Kernel $elm$kernel$Debug$$ deps: $elm$core$Array$toList, $elm$core$Dict$toList, $elm$core$Set$toList

// Begin Kernel $elm$kernel$Utils$$ deps: $elm$kernel$Debug$$, $elm$kernel$List$$, $elm$core$Basics$EQ, $elm$core$Basics$GT, $elm$core$Basics$LT, $elm$core$Dict$toList, $elm$core$Set$toList



// EQUALITY

function _Utils_eq(x, y)
{
	for (
		var pair, stack = [], isEqual = _Utils_eqHelp(x, y, 0, stack);
		isEqual && (pair = stack.pop());
		isEqual = _Utils_eqHelp(pair.a, pair.b, 0, stack)
		)
	{}

	return isEqual;
}

function _Utils_eqHelp(x, y, depth, stack)
{
	if (x === y)
	{
		return true;
	}

	if (typeof x !== 'object' || x === null || y === null)
	{
		typeof x === 'function' && _Debug_crash(5);
		return false;
	}

	if (depth > 100)
	{
		stack.push(_Utils_Tuple2(x,y));
		return true;
	}

	/**/
	if (x.$ === 'Set_elm_builtin')
	{
		x = $elm$core$Set$toList(x);
		y = $elm$core$Set$toList(y);
	}
	if (x.$ === 'RBNode_elm_builtin' || x.$ === 'RBEmpty_elm_builtin')
	{
		x = $elm$core$Dict$toList(x);
		y = $elm$core$Dict$toList(y);
	}
	//*/

	/**_UNUSED/
	if (x.$ < 0)
	{
		x = $elm$core$Dict$toList(x);
		y = $elm$core$Dict$toList(y);
	}
	//*/

	for (var key in x)
	{
		if (!_Utils_eqHelp(x[key], y[key], depth + 1, stack))
		{
			return false;
		}
	}
	return true;
}

var _Utils_equal = F2(_Utils_eq);
var _Utils_notEqual = F2(function(a, b) { return !_Utils_eq(a,b); });



// COMPARISONS

// Code in Generate/JavaScript.hs, Basics.js, and List.js depends on
// the particular integer values assigned to LT, EQ, and GT.

function _Utils_cmp(x, y, ord)
{
	if (typeof x !== 'object')
	{
		return x === y ? /*EQ*/ 0 : x < y ? /*LT*/ -1 : /*GT*/ 1;
	}

	/**/
	if (x instanceof String)
	{
		var a = x.valueOf();
		var b = y.valueOf();
		return a === b ? 0 : a < b ? -1 : 1;
	}
	//*/

	/**_UNUSED/
	if (typeof x.$ === 'undefined')
	//*/
	/**/
	if (x.$[0] === '#')
	//*/
	{
		return (ord = _Utils_cmp(x.a, y.a))
			? ord
			: (ord = _Utils_cmp(x.b, y.b))
				? ord
				: _Utils_cmp(x.c, y.c);
	}

	// traverse conses until end of a list or a mismatch
	for (; x.b && y.b && !(ord = _Utils_cmp(x.a, y.a)); x = x.b, y = y.b) {} // WHILE_CONSES
	return ord || (x.b ? /*GT*/ 1 : y.b ? /*LT*/ -1 : /*EQ*/ 0);
}

var _Utils_lt = F2(function(a, b) { return _Utils_cmp(a, b) < 0; });
var _Utils_le = F2(function(a, b) { return _Utils_cmp(a, b) < 1; });
var _Utils_gt = F2(function(a, b) { return _Utils_cmp(a, b) > 0; });
var _Utils_ge = F2(function(a, b) { return _Utils_cmp(a, b) >= 0; });

var _Utils_compare = F2(function(x, y)
{
	var n = _Utils_cmp(x, y);
	return n < 0 ? $elm$core$Basics$LT : n ? $elm$core$Basics$GT : $elm$core$Basics$EQ;
});


// COMMON VALUES

var _Utils_Tuple0_UNUSED = 0;
var _Utils_Tuple0 = { $: '#0' };

function _Utils_Tuple2_UNUSED(a, b) { return { a: a, b: b }; }
function _Utils_Tuple2(a, b) { return { $: '#2', a: a, b: b }; }

function _Utils_Tuple3_UNUSED(a, b, c) { return { a: a, b: b, c: c }; }
function _Utils_Tuple3(a, b, c) { return { $: '#3', a: a, b: b, c: c }; }

function _Utils_chr_UNUSED(c) { return c; }
function _Utils_chr(c) { return new String(c); }


// RECORDS

function _Utils_update(oldRecord, updatedFields)
{
	var newRecord = {};

	for (var key in oldRecord)
	{
		newRecord[key] = oldRecord[key];
	}

	for (var key in updatedFields)
	{
		newRecord[key] = updatedFields[key];
	}

	return newRecord;
}


// APPEND

var _Utils_append = F2(_Utils_ap);

function _Utils_ap(xs, ys)
{
	// append Strings
	if (typeof xs === 'string')
	{
		return xs + ys;
	}

	// append Lists
	if (!xs.b)
	{
		return ys;
	}
	var root = _List_Cons(xs.a, ys);
	xs = xs.b
	for (var curr = root; xs.b; xs = xs.b) // WHILE_CONS
	{
		curr = curr.b = _List_Cons(xs.a, ys);
	}
	return root;
}

// End Kernel $elm$kernel$Utils$$ deps: $elm$kernel$Debug$$, $elm$kernel$List$$, $elm$core$Basics$EQ, $elm$core$Basics$GT, $elm$core$Basics$LT, $elm$core$Dict$toList, $elm$core$Set$toList

// Begin Kernel $elm$kernel$Basics$$ deps: $elm$kernel$Debug$$



// MATH

var _Basics_add = F2(function(a, b) { return a + b; });
var _Basics_sub = F2(function(a, b) { return a - b; });
var _Basics_mul = F2(function(a, b) { return a * b; });
var _Basics_fdiv = F2(function(a, b) { return a / b; });
var _Basics_idiv = F2(function(a, b) { return (a / b) | 0; });
var _Basics_pow = F2(Math.pow);

var _Basics_remainderBy = F2(function(b, a) { return a % b; });

// https://www.microsoft.com/en-us/research/wp-content/uploads/2016/02/divmodnote-letter.pdf
var _Basics_modBy = F2(function(modulus, x)
{
	var answer = x % modulus;
	return modulus === 0
		? _Debug_crash(11)
		:
	((answer > 0 && modulus < 0) || (answer < 0 && modulus > 0))
		? answer + modulus
		: answer;
});


// TRIGONOMETRY

var _Basics_pi = Math.PI;
var _Basics_e = Math.E;
var _Basics_cos = Math.cos;
var _Basics_sin = Math.sin;
var _Basics_tan = Math.tan;
var _Basics_acos = Math.acos;
var _Basics_asin = Math.asin;
var _Basics_atan = Math.atan;
var _Basics_atan2 = F2(Math.atan2);


// MORE MATH

function _Basics_toFloat(x) { return x; }
function _Basics_truncate(n) { return n | 0; }
function _Basics_isInfinite(n) { return n === Infinity || n === -Infinity; }

var _Basics_ceiling = Math.ceil;
var _Basics_floor = Math.floor;
var _Basics_round = Math.round;
var _Basics_sqrt = Math.sqrt;
var _Basics_log = Math.log;
var _Basics_isNaN = isNaN;


// BOOLEANS

function _Basics_not(bool) { return !bool; }
var _Basics_and = F2(function(a, b) { return a && b; });
var _Basics_or  = F2(function(a, b) { return a || b; });
var _Basics_xor = F2(function(a, b) { return a !== b; });

// End Kernel $elm$kernel$Basics$$ deps: $elm$kernel$Debug$$

// Begin Kernel $elm$kernel$String$$ deps: $elm$kernel$List$$, $elm$kernel$Utils$$, $elm$core$Maybe$Just, $elm$core$Maybe$Nothing



var _String_cons = F2(function(chr, str)
{
	return chr + str;
});

function _String_uncons(string)
{
	var word = string.charCodeAt(0);
	return !isNaN(word)
		? $elm$core$Maybe$Just(
			0xD800 <= word && word <= 0xDBFF
				? _Utils_Tuple2(_Utils_chr(string[0] + string[1]), string.slice(2))
				: _Utils_Tuple2(_Utils_chr(string[0]), string.slice(1))
		)
		: $elm$core$Maybe$Nothing;
}

var _String_append = F2(function(a, b)
{
	return a + b;
});

function _String_length(str)
{
	return str.length;
}

var _String_map = F2(function(func, string)
{
	var len = string.length;
	var array = new Array(len);
	var i = 0;
	while (i < len)
	{
		var word = string.charCodeAt(i);
		if (0xD800 <= word && word <= 0xDBFF)
		{
			array[i] = func(_Utils_chr(string[i] + string[i+1]));
			i += 2;
			continue;
		}
		array[i] = func(_Utils_chr(string[i]));
		i++;
	}
	return array.join('');
});

var _String_filter = F2(function(isGood, str)
{
	var arr = [];
	var len = str.length;
	var i = 0;
	while (i < len)
	{
		var char = str[i];
		var word = str.charCodeAt(i);
		i++;
		if (0xD800 <= word && word <= 0xDBFF)
		{
			char += str[i];
			i++;
		}

		if (isGood(_Utils_chr(char)))
		{
			arr.push(char);
		}
	}
	return arr.join('');
});

function _String_reverse(str)
{
	var len = str.length;
	var arr = new Array(len);
	var i = 0;
	while (i < len)
	{
		var word = str.charCodeAt(i);
		if (0xD800 <= word && word <= 0xDBFF)
		{
			arr[len - i] = str[i + 1];
			i++;
			arr[len - i] = str[i - 1];
			i++;
		}
		else
		{
			arr[len - i] = str[i];
			i++;
		}
	}
	return arr.join('');
}

var _String_foldl = F3(function(func, state, string)
{
	var len = string.length;
	var i = 0;
	while (i < len)
	{
		var char = string[i];
		var word = string.charCodeAt(i);
		i++;
		if (0xD800 <= word && word <= 0xDBFF)
		{
			char += string[i];
			i++;
		}
		state = A2(func, _Utils_chr(char), state);
	}
	return state;
});

var _String_foldr = F3(function(func, state, string)
{
	var i = string.length;
	while (i--)
	{
		var char = string[i];
		var word = string.charCodeAt(i);
		if (0xDC00 <= word && word <= 0xDFFF)
		{
			i--;
			char = string[i] + char;
		}
		state = A2(func, _Utils_chr(char), state);
	}
	return state;
});

var _String_split = F2(function(sep, str)
{
	return str.split(sep);
});

var _String_join = F2(function(sep, strs)
{
	return strs.join(sep);
});

var _String_slice = F3(function(start, end, str) {
	return str.slice(start, end);
});

function _String_trim(str)
{
	return str.trim();
}

function _String_trimLeft(str)
{
	return str.replace(/^\s+/, '');
}

function _String_trimRight(str)
{
	return str.replace(/\s+$/, '');
}

function _String_words(str)
{
	return _List_fromArray(str.trim().split(/\s+/g));
}

function _String_lines(str)
{
	return _List_fromArray(str.split(/\r\n|\r|\n/g));
}

function _String_toUpper(str)
{
	return str.toUpperCase();
}

function _String_toLower(str)
{
	return str.toLowerCase();
}

var _String_any = F2(function(isGood, string)
{
	var i = string.length;
	while (i--)
	{
		var char = string[i];
		var word = string.charCodeAt(i);
		if (0xDC00 <= word && word <= 0xDFFF)
		{
			i--;
			char = string[i] + char;
		}
		if (isGood(_Utils_chr(char)))
		{
			return true;
		}
	}
	return false;
});

var _String_all = F2(function(isGood, string)
{
	var i = string.length;
	while (i--)
	{
		var char = string[i];
		var word = string.charCodeAt(i);
		if (0xDC00 <= word && word <= 0xDFFF)
		{
			i--;
			char = string[i] + char;
		}
		if (!isGood(_Utils_chr(char)))
		{
			return false;
		}
	}
	return true;
});

var _String_contains = F2(function(sub, str)
{
	return str.indexOf(sub) > -1;
});

var _String_startsWith = F2(function(sub, str)
{
	return str.indexOf(sub) === 0;
});

var _String_endsWith = F2(function(sub, str)
{
	return str.length >= sub.length &&
		str.lastIndexOf(sub) === str.length - sub.length;
});

var _String_indexes = F2(function(sub, str)
{
	var subLen = sub.length;

	if (subLen < 1)
	{
		return _List_Nil;
	}

	var i = 0;
	var is = [];

	while ((i = str.indexOf(sub, i)) > -1)
	{
		is.push(i);
		i = i + subLen;
	}

	return _List_fromArray(is);
});


// TO STRING

function _String_fromNumber(number)
{
	return number + '';
}


// INT CONVERSIONS

function _String_toInt(str)
{
	var total = 0;
	var code0 = str.charCodeAt(0);
	var start = code0 == 0x2B /* + */ || code0 == 0x2D /* - */ ? 1 : 0;

	for (var i = start; i < str.length; ++i)
	{
		var code = str.charCodeAt(i);
		if (code < 0x30 || 0x39 < code)
		{
			return $elm$core$Maybe$Nothing;
		}
		total = 10 * total + code - 0x30;
	}

	return i == start
		? $elm$core$Maybe$Nothing
		: $elm$core$Maybe$Just(code0 == 0x2D ? -total : total);
}


// FLOAT CONVERSIONS

function _String_toFloat(s)
{
	// check if it is a hex, octal, or binary number
	if (s.length === 0 || /[\sxbo]/.test(s))
	{
		return $elm$core$Maybe$Nothing;
	}
	var n = +s;
	// faster isNaN check
	return n === n ? $elm$core$Maybe$Just(n) : $elm$core$Maybe$Nothing;
}

function _String_fromList(chars)
{
	return _List_toArray(chars).join('');
}


// End Kernel $elm$kernel$String$$ deps: $elm$kernel$List$$, $elm$kernel$Utils$$, $elm$core$Maybe$Just, $elm$core$Maybe$Nothing

// Begin Kernel $elm$kernel$Char$$ deps: $elm$kernel$Utils$$



function _Char_toCode(char)
{
	var code = char.charCodeAt(0);
	if (0xD800 <= code && code <= 0xDBFF)
	{
		return (code - 0xD800) * 0x400 + char.charCodeAt(1) - 0xDC00 + 0x10000
	}
	return code;
}

function _Char_fromCode(code)
{
	return _Utils_chr(
		(code < 0 || 0x10FFFF < code)
			? '\uFFFD'
			:
		(code <= 0xFFFF)
			? String.fromCharCode(code)
			:
		(code -= 0x10000,
			String.fromCharCode(Math.floor(code / 0x400) + 0xD800, code % 0x400 + 0xDC00)
		)
	);
}

function _Char_toUpper(char)
{
	return _Utils_chr(char.toUpperCase());
}

function _Char_toLower(char)
{
	return _Utils_chr(char.toLowerCase());
}

function _Char_toLocaleUpper(char)
{
	return _Utils_chr(char.toLocaleUpperCase());
}

function _Char_toLocaleLower(char)
{
	return _Utils_chr(char.toLocaleLowerCase());
}

// End Kernel $elm$kernel$Char$$ deps: $elm$kernel$Utils$$

// Begin Kernel $elm$kernel$Json$$ deps: $elm$kernel$List$$, $elm$kernel$Utils$$, $elm$core$Result$Err, $elm$json$Json$Decode$Failure, $elm$json$Json$Decode$Field, $elm$json$Json$Decode$Index, $elm$core$Result$Ok, $elm$json$Json$Decode$OneOf, $elm$json$Json$Decode$errorToString, $elm$core$Array$initialize, $elm$core$Result$isOk, $elm$core$List$reverse



/**/
function _Json_errorToString(error)
{
	return $elm$json$Json$Decode$errorToString(error);
}
//*/


// CORE DECODERS

function _Json_succeed(msg)
{
	return {
		$: 0,
		a: msg
	};
}

function _Json_fail(msg)
{
	return {
		$: 1,
		a: msg
	};
}

function _Json_decodePrim(decoder)
{
	return { $: 2, b: decoder };
}

var _Json_decodeInt = _Json_decodePrim(function(value) {
	return (typeof value !== 'number')
		? _Json_expecting('an INT', value)
		:
	(-2147483647 < value && value < 2147483647 && (value | 0) === value)
		? $elm$core$Result$Ok(value)
		:
	(isFinite(value) && !(value % 1))
		? $elm$core$Result$Ok(value)
		: _Json_expecting('an INT', value);
});

var _Json_decodeBool = _Json_decodePrim(function(value) {
	return (typeof value === 'boolean')
		? $elm$core$Result$Ok(value)
		: _Json_expecting('a BOOL', value);
});

var _Json_decodeFloat = _Json_decodePrim(function(value) {
	return (typeof value === 'number')
		? $elm$core$Result$Ok(value)
		: _Json_expecting('a FLOAT', value);
});

var _Json_decodeValue = _Json_decodePrim(function(value) {
	return $elm$core$Result$Ok(_Json_wrap(value));
});

var _Json_decodeString = _Json_decodePrim(function(value) {
	return (typeof value === 'string')
		? $elm$core$Result$Ok(value)
		: (value instanceof String)
			? $elm$core$Result$Ok(value + '')
			: _Json_expecting('a STRING', value);
});

function _Json_decodeList(decoder) { return { $: 3, b: decoder }; }
function _Json_decodeArray(decoder) { return { $: 4, b: decoder }; }

function _Json_decodeNull(value) { return { $: 5, c: value }; }

var _Json_decodeField = F2(function(field, decoder)
{
	return {
		$: 6,
		d: field,
		b: decoder
	};
});

var _Json_decodeIndex = F2(function(index, decoder)
{
	return {
		$: 7,
		e: index,
		b: decoder
	};
});

function _Json_decodeKeyValuePairs(decoder)
{
	return {
		$: 8,
		b: decoder
	};
}

function _Json_mapMany(f, decoders)
{
	return {
		$: 9,
		f: f,
		g: decoders
	};
}

var _Json_andThen = F2(function(callback, decoder)
{
	return {
		$: 10,
		b: decoder,
		h: callback
	};
});

function _Json_oneOf(decoders)
{
	return {
		$: 11,
		g: decoders
	};
}


// DECODING OBJECTS

var _Json_map1 = F2(function(f, d1)
{
	return _Json_mapMany(f, [d1]);
});

var _Json_map2 = F3(function(f, d1, d2)
{
	return _Json_mapMany(f, [d1, d2]);
});

var _Json_map3 = F4(function(f, d1, d2, d3)
{
	return _Json_mapMany(f, [d1, d2, d3]);
});

var _Json_map4 = F5(function(f, d1, d2, d3, d4)
{
	return _Json_mapMany(f, [d1, d2, d3, d4]);
});

var _Json_map5 = F6(function(f, d1, d2, d3, d4, d5)
{
	return _Json_mapMany(f, [d1, d2, d3, d4, d5]);
});

var _Json_map6 = F7(function(f, d1, d2, d3, d4, d5, d6)
{
	return _Json_mapMany(f, [d1, d2, d3, d4, d5, d6]);
});

var _Json_map7 = F8(function(f, d1, d2, d3, d4, d5, d6, d7)
{
	return _Json_mapMany(f, [d1, d2, d3, d4, d5, d6, d7]);
});

var _Json_map8 = F9(function(f, d1, d2, d3, d4, d5, d6, d7, d8)
{
	return _Json_mapMany(f, [d1, d2, d3, d4, d5, d6, d7, d8]);
});


// DECODE

var _Json_runOnString = F2(function(decoder, string)
{
	try
	{
		var value = JSON.parse(string);
		return _Json_runHelp(decoder, value);
	}
	catch (e)
	{
		return $elm$core$Result$Err(A2($elm$json$Json$Decode$Failure, 'This is not valid JSON! ' + e.message, _Json_wrap(string)));
	}
});

var _Json_run = F2(function(decoder, value)
{
	return _Json_runHelp(decoder, _Json_unwrap(value));
});

function _Json_runHelp(decoder, value)
{
	switch (decoder.$)
	{
		case 2:
			return decoder.b(value);

		case 5:
			return (value === null)
				? $elm$core$Result$Ok(decoder.c)
				: _Json_expecting('null', value);

		case 3:
			if (!_Json_isArray(value))
			{
				return _Json_expecting('a LIST', value);
			}
			return _Json_runArrayDecoder(decoder.b, value, _List_fromArray);

		case 4:
			if (!_Json_isArray(value))
			{
				return _Json_expecting('an ARRAY', value);
			}
			return _Json_runArrayDecoder(decoder.b, value, _Json_toElmArray);

		case 6:
			var field = decoder.d;
			if (typeof value !== 'object' || value === null || !(field in value))
			{
				return _Json_expecting('an OBJECT with a field named `' + field + '`', value);
			}
			var result = _Json_runHelp(decoder.b, value[field]);
			return ($elm$core$Result$isOk(result)) ? result : $elm$core$Result$Err(A2($elm$json$Json$Decode$Field, field, result.a));

		case 7:
			var index = decoder.e;
			if (!_Json_isArray(value))
			{
				return _Json_expecting('an ARRAY', value);
			}
			if (index >= value.length)
			{
				return _Json_expecting('a LONGER array. Need index ' + index + ' but only see ' + value.length + ' entries', value);
			}
			var result = _Json_runHelp(decoder.b, value[index]);
			return ($elm$core$Result$isOk(result)) ? result : $elm$core$Result$Err(A2($elm$json$Json$Decode$Index, index, result.a));

		case 8:
			if (typeof value !== 'object' || value === null || _Json_isArray(value))
			{
				return _Json_expecting('an OBJECT', value);
			}

			var keyValuePairs = _List_Nil;
			// TODO test perf of Object.keys and switch when support is good enough
			for (var key in value)
			{
				if (value.hasOwnProperty(key))
				{
					var result = _Json_runHelp(decoder.b, value[key]);
					if (!$elm$core$Result$isOk(result))
					{
						return $elm$core$Result$Err(A2($elm$json$Json$Decode$Field, key, result.a));
					}
					keyValuePairs = _List_Cons(_Utils_Tuple2(key, result.a), keyValuePairs);
				}
			}
			return $elm$core$Result$Ok($elm$core$List$reverse(keyValuePairs));

		case 9:
			var answer = decoder.f;
			var decoders = decoder.g;
			for (var i = 0; i < decoders.length; i++)
			{
				var result = _Json_runHelp(decoders[i], value);
				if (!$elm$core$Result$isOk(result))
				{
					return result;
				}
				answer = answer(result.a);
			}
			return $elm$core$Result$Ok(answer);

		case 10:
			var result = _Json_runHelp(decoder.b, value);
			return (!$elm$core$Result$isOk(result))
				? result
				: _Json_runHelp(decoder.h(result.a), value);

		case 11:
			var errors = _List_Nil;
			for (var temp = decoder.g; temp.b; temp = temp.b) // WHILE_CONS
			{
				var result = _Json_runHelp(temp.a, value);
				if ($elm$core$Result$isOk(result))
				{
					return result;
				}
				errors = _List_Cons(result.a, errors);
			}
			return $elm$core$Result$Err($elm$json$Json$Decode$OneOf($elm$core$List$reverse(errors)));

		case 1:
			return $elm$core$Result$Err(A2($elm$json$Json$Decode$Failure, decoder.a, _Json_wrap(value)));

		case 0:
			return $elm$core$Result$Ok(decoder.a);
	}
}

function _Json_runArrayDecoder(decoder, value, toElmValue)
{
	var len = value.length;
	var array = new Array(len);
	for (var i = 0; i < len; i++)
	{
		var result = _Json_runHelp(decoder, value[i]);
		if (!$elm$core$Result$isOk(result))
		{
			return $elm$core$Result$Err(A2($elm$json$Json$Decode$Index, i, result.a));
		}
		array[i] = result.a;
	}
	return $elm$core$Result$Ok(toElmValue(array));
}

function _Json_isArray(value)
{
	return Array.isArray(value) || (typeof FileList !== 'undefined' && value instanceof FileList);
}

function _Json_toElmArray(array)
{
	return A2($elm$core$Array$initialize, array.length, function(i) { return array[i]; });
}

function _Json_expecting(type, value)
{
	return $elm$core$Result$Err(A2($elm$json$Json$Decode$Failure, 'Expecting ' + type, _Json_wrap(value)));
}


// EQUALITY

function _Json_equality(x, y)
{
	if (x === y)
	{
		return true;
	}

	if (x.$ !== y.$)
	{
		return false;
	}

	switch (x.$)
	{
		case 0:
		case 1:
			return x.a === y.a;

		case 2:
			return x.b === y.b;

		case 5:
			return x.c === y.c;

		case 3:
		case 4:
		case 8:
			return _Json_equality(x.b, y.b);

		case 6:
			return x.d === y.d && _Json_equality(x.b, y.b);

		case 7:
			return x.e === y.e && _Json_equality(x.b, y.b);

		case 9:
			return x.f === y.f && _Json_listEquality(x.g, y.g);

		case 10:
			return x.h === y.h && _Json_equality(x.b, y.b);

		case 11:
			return _Json_listEquality(x.g, y.g);
	}
}

function _Json_listEquality(aDecoders, bDecoders)
{
	var len = aDecoders.length;
	if (len !== bDecoders.length)
	{
		return false;
	}
	for (var i = 0; i < len; i++)
	{
		if (!_Json_equality(aDecoders[i], bDecoders[i]))
		{
			return false;
		}
	}
	return true;
}


// ENCODE

var _Json_encode = F2(function(indentLevel, value)
{
	return JSON.stringify(_Json_unwrap(value), null, indentLevel) + '';
});

function _Json_wrap(value) { return { $: 0, a: value }; }
function _Json_unwrap(value) { return value.a; }

function _Json_wrap_UNUSED(value) { return value; }
function _Json_unwrap_UNUSED(value) { return value; }

function _Json_emptyArray() { return []; }
function _Json_emptyObject() { return {}; }

var _Json_addField = F3(function(key, value, object)
{
	object[key] = _Json_unwrap(value);
	return object;
});

function _Json_addEntry(func)
{
	return F2(function(entry, array)
	{
		array.push(_Json_unwrap(func(entry)));
		return array;
	});
}

var _Json_encodeNull = _Json_wrap(null);

// End Kernel $elm$kernel$Json$$ deps: $elm$kernel$List$$, $elm$kernel$Utils$$, $elm$core$Result$Err, $elm$json$Json$Decode$Failure, $elm$json$Json$Decode$Field, $elm$json$Json$Decode$Index, $elm$core$Result$Ok, $elm$json$Json$Decode$OneOf, $elm$json$Json$Decode$errorToString, $elm$core$Array$initialize, $elm$core$Result$isOk, $elm$core$List$reverse

// Begin Kernel $elm$kernel$Scheduler$$ deps: $elm$kernel$Utils$$



// TASKS

function _Scheduler_succeed(value)
{
	return {
		$: 0,
		a: value
	};
}

function _Scheduler_fail(error)
{
	return {
		$: 1,
		a: error
	};
}

function _Scheduler_binding(callback)
{
	return {
		$: 2,
		b: callback,
		c: null
	};
}

var _Scheduler_andThen = F2(function(callback, task)
{
	return {
		$: 3,
		b: callback,
		d: task
	};
});

var _Scheduler_onError = F2(function(callback, task)
{
	return {
		$: 4,
		b: callback,
		d: task
	};
});

function _Scheduler_receive(callback)
{
	return {
		$: 5,
		b: callback
	};
}


// PROCESSES

var _Scheduler_guid = 0;

function _Scheduler_rawSpawn(task)
{
	var proc = {
		$: 0,
		e: _Scheduler_guid++,
		f: task,
		g: null,
		h: []
	};

  console.log('Spawned process', proc)

	_Scheduler_enqueue(proc);

	return proc;
}

function _Scheduler_spawn(task)
{
	return _Scheduler_binding(function(callback) {
		callback(_Scheduler_succeed(_Scheduler_rawSpawn(task)));
	});
}

function _Scheduler_rawSend(proc, msg)
{
	proc.h.push(msg);
	_Scheduler_enqueue(proc);
}

var _Scheduler_send = F2(function(proc, msg)
{
	return _Scheduler_binding(function(callback) {
		_Scheduler_rawSend(proc, msg);
		callback(_Scheduler_succeed(_Utils_Tuple0));
	});
});

function _Scheduler_kill(proc)
{
	return _Scheduler_binding(function(callback) {
		var task = proc.f;
		if (task.$ === 2 && task.c)
		{
			task.c();
		}

		proc.f = null;

		callback(_Scheduler_succeed(_Utils_Tuple0));
	});
}


/* STEP PROCESSES

type alias Process =
  { $ : tag
  , id : unique_id
  , root : Task
  , stack : null | { $: SUCCEED | FAIL, a: callback, b: stack }
  , mailbox : [msg]
  }

*/


var _Scheduler_working = false;
var _Scheduler_queue = [];


function _Scheduler_enqueue(proc)
{
	_Scheduler_queue.push(proc);
	if (_Scheduler_working)
	{
		return;
	}
	_Scheduler_working = true;
	while (proc = _Scheduler_queue.shift())
	{
		_Scheduler_step(proc);
	}
	_Scheduler_working = false;
}

function debug_step(proc) {
  const value = 'a';
  const callback = 'b';
  const kill = 'c';
  const task = 'd';
  const id = 'e';
  const root_ = 'f';
  const stack = 'g';
  const mailbox = 'h';

  const task_ctors = [
    'SUCCEED',
    'FAIL',
    'BINDING',
    'AND_THEN',
    'ON_ERROR',
    'RECEIVE'
  ];
  const root = proc[root_];

  const procRoot = {
    $: task_ctors[root.$],
    value: root[value],
    callback: root[callback],
    kill: root[kill],
    task: root[task]
  };
  console.log(`Stepping Process ${proc[id]}: root=`, procRoot);
}


function _Scheduler_step(proc)
{
	while (proc.f)
	{
    debug_step(proc);

		var rootTag = proc.f.$;
		if (rootTag === 0 || rootTag === 1)
		{
			while (proc.g && proc.g.$ !== rootTag)
			{
				proc.g = proc.g.i;
			}
			if (!proc.g)
			{
				return;
			}
			proc.f = proc.g.b(proc.f.a);
			proc.g = proc.g.i;
		}
		else if (rootTag === 2)
		{
			proc.f.c = proc.f.b(function(newRoot) {
				proc.f = newRoot;
				_Scheduler_enqueue(proc);
			});
			return;
		}
		else if (rootTag === 5)
		{
			if (proc.h.length === 0)
			{
				return;
			}
			proc.f = proc.f.b(proc.h.shift());
		}
		else // if (rootTag === 3 || rootTag === 4)
		{
			proc.g = {
				$: rootTag === 3 ? 0 : 1,
				b: proc.f.b,
				i: proc.g
			};
			proc.f = proc.f.d;
		}
	}
}

// End Kernel $elm$kernel$Scheduler$$ deps: $elm$kernel$Utils$$

// Begin Kernel $elm$kernel$Process$$ deps: $elm$kernel$Scheduler$$, $elm$kernel$Utils$$



function _Process_sleep(time)
{
	return _Scheduler_binding(function(callback) {
		var id = setTimeout(function() {
			callback(_Scheduler_succeed(_Utils_Tuple0));
		}, time);

		return function() { clearTimeout(id); };
	});
}

// End Kernel $elm$kernel$Process$$ deps: $elm$kernel$Scheduler$$, $elm$kernel$Utils$$

// Begin Kernel $elm$kernel$Platform$$ deps: $elm$kernel$Debug$$, $elm$kernel$Json$$, $elm$kernel$List$$, $elm$kernel$Process$$, $elm$kernel$Scheduler$$, $elm$kernel$Utils$$, $elm$core$Result$isOk




// PROGRAMS


var _Platform_worker = F4(function(impl, flagDecoder, debugMetadata, args)
{
	return _Platform_initialize(
		flagDecoder,
		args,
		impl.init,
		impl.update,
		impl.subscriptions,
		function() { return function() {} }
	);
});



// INITIALIZE A PROGRAM


function _Platform_initialize(flagDecoder, args, init, update, subscriptions, stepperBuilder)
{
	var result = A2(_Json_run, flagDecoder, _Json_wrap(args ? args['flags'] : undefined));
	$elm$core$Result$isOk(result) || _Debug_crash(2 /**/, _Json_errorToString(result.a) /**/);
	var managers = {};
	var initPair = init(result.a);
	var model = initPair.a;
	var stepper = stepperBuilder(sendToApp, model);
	var ports = _Platform_setupEffects(managers, sendToApp);

	function sendToApp(msg, viewMetadata)
	{
    console.log('sendToApp', msg);
		var pair = A2(update, msg, model);
		stepper(model = pair.a, viewMetadata);
		_Platform_enqueueEffects(managers, pair.b, subscriptions(model));
	}

	_Platform_enqueueEffects(managers, initPair.b, subscriptions(model));

	return ports ? { ports: ports } : {};
}



// TRACK PRELOADS
//
// This is used by code in elm/browser and elm/http
// to register any HTTP requests that are triggered by init.
//


var _Platform_preload;


function _Platform_registerPreload(url)
{
	_Platform_preload.add(url);
}



// EFFECT MANAGERS


var _Platform_effectManagers = {};


function _Platform_setupEffects(managers, sendToApp)
{
	var ports;

	// setup all necessary effect managers
	for (var key in _Platform_effectManagers)
	{
		var manager = _Platform_effectManagers[key];

		if (manager.a)
		{
			ports = ports || {};
			ports[key] = manager.a(key, sendToApp);
		}

		managers[key] = _Platform_instantiateManager(manager, sendToApp);
	}

	return ports;
}


function _Platform_createManager(init, onEffects, onSelfMsg, cmdMap, subMap)
{
	return {
		b: init,
		c: onEffects,
		d: onSelfMsg,
		e: cmdMap,
		f: subMap
	};
}


function _Platform_instantiateManager(info, sendToApp)
{
	var router = {
		g: sendToApp,
		h: undefined
	};

	var onEffects = info.c;
	var onSelfMsg = info.d;
	var cmdMap = info.e;
	var subMap = info.f;

	function loop(state)
	{
		return A2(_Scheduler_andThen, loop, _Scheduler_receive(function(msg)
		{
			var value = msg.a;

			if (msg.$ === 0)
			{
				return A3(onSelfMsg, router, value, state);
			}

			return cmdMap && subMap
				? A4(onEffects, router, value.i, value.j, state)
				: A3(onEffects, router, cmdMap ? value.i : value.j, state);
		}));
	}

	return router.h = _Scheduler_rawSpawn(A2(_Scheduler_andThen, loop, info.b));
}



// ROUTING


var _Platform_sendToApp = F2(function(router, msg)
{
	return _Scheduler_binding(function(callback)
	{
		router.g(msg);
		callback(_Scheduler_succeed(_Utils_Tuple0));
	});
});


var _Platform_sendToSelf = F2(function(router, msg)
{
	return A2(_Scheduler_send, router.h, {
		$: 0,
		a: msg
	});
});



// BAGS


function _Platform_leaf(home)
{
	return function(value)
	{
		return {
			$: 1,
			k: home,
			l: value
		};
	};
}


function _Platform_batch(list)
{
	return {
		$: 2,
		m: list
	};
}


var _Platform_map = F2(function(tagger, bag)
{
	return {
		$: 3,
		n: tagger,
		o: bag
	}
});



// PIPE BAGS INTO EFFECT MANAGERS
//
// Effects must be queued!
//
// Say your init contains a synchronous command, like Time.now or Time.here
//
//   - This will produce a batch of effects (FX_1)
//   - The synchronous task triggers the subsequent `update` call
//   - This will produce a batch of effects (FX_2)
//
// If we just start dispatching FX_2, subscriptions from FX_2 can be processed
// before subscriptions from FX_1. No good! Earlier versions of this code had
// this problem, leading to these reports:
//
//   https://github.com/elm/core/issues/980
//   https://github.com/elm/core/pull/981
//   https://github.com/elm/compiler/issues/1776
//
// The queue is necessary to avoid ordering issues for synchronous commands.


// Why use true/false here? Why not just check the length of the queue?
// The goal is to detect "are we currently dispatching effects?" If we
// are, we need to bail and let the ongoing while loop handle things.
//
// Now say the queue has 1 element. When we dequeue the final element,
// the queue will be empty, but we are still actively dispatching effects.
// So you could get queue jumping in a really tricky category of cases.
//
var _Platform_effectsQueue = [];
var _Platform_effectsActive = false;


function _Platform_enqueueEffects(managers, cmdBag, subBag)
{
	_Platform_effectsQueue.push({ p: managers, q: cmdBag, r: subBag });

	if (_Platform_effectsActive) return;

	_Platform_effectsActive = true;
	for (var fx; fx = _Platform_effectsQueue.shift(); )
	{
		_Platform_dispatchEffects(fx.p, fx.q, fx.r);
	}
	_Platform_effectsActive = false;
}


function _Platform_dispatchEffects(managers, cmdBag, subBag)
{
	var effectsDict = {};
	_Platform_gatherEffects(true, cmdBag, effectsDict, null);
	_Platform_gatherEffects(false, subBag, effectsDict, null);

	for (var home in managers)
	{
		_Scheduler_rawSend(managers[home], {
			$: 'fx',
			a: effectsDict[home] || { i: _List_Nil, j: _List_Nil }
		});
	}
}


function _Platform_gatherEffects(isCmd, bag, effectsDict, taggers)
{
	switch (bag.$)
	{
		case 1:
			var home = bag.k;
			var effect = _Platform_toEffect(isCmd, home, taggers, bag.l);
			effectsDict[home] = _Platform_insert(isCmd, effect, effectsDict[home]);
			return;

		case 2:
			for (var list = bag.m; list.b; list = list.b) // WHILE_CONS
			{
				_Platform_gatherEffects(isCmd, list.a, effectsDict, taggers);
			}
			return;

		case 3:
			_Platform_gatherEffects(isCmd, bag.o, effectsDict, {
				s: bag.n,
				t: taggers
			});
			return;
	}
}


function _Platform_toEffect(isCmd, home, taggers, value)
{
	function applyTaggers(x)
	{
		for (var temp = taggers; temp; temp = temp.t)
		{
			x = temp.s(x);
		}
		return x;
	}

	var map = isCmd
		? _Platform_effectManagers[home].e
		: _Platform_effectManagers[home].f;

	return A2(map, applyTaggers, value)
}


function _Platform_insert(isCmd, newEffect, effects)
{
	effects = effects || { i: _List_Nil, j: _List_Nil };

	isCmd
		? (effects.i = _List_Cons(newEffect, effects.i))
		: (effects.j = _List_Cons(newEffect, effects.j));

	return effects;
}



// PORTS


function _Platform_checkPortName(name)
{
	if (_Platform_effectManagers[name])
	{
		_Debug_crash(3, name)
	}
}



// OUTGOING PORTS


function _Platform_outgoingPort(name, converter)
{
	_Platform_checkPortName(name);
	_Platform_effectManagers[name] = {
		e: _Platform_outgoingPortMap,
		u: converter,
		a: _Platform_setupOutgoingPort
	};
	return _Platform_leaf(name);
}


var _Platform_outgoingPortMap = F2(function(tagger, value) { return value; });


function _Platform_setupOutgoingPort(name)
{
	var subs = [];
	var converter = _Platform_effectManagers[name].u;

	// CREATE MANAGER

	var init = _Process_sleep(0);

	_Platform_effectManagers[name].b = init;
	_Platform_effectManagers[name].c = F3(function(router, cmdList, state)
	{
		for ( ; cmdList.b; cmdList = cmdList.b) // WHILE_CONS
		{
			// grab a separate reference to subs in case unsubscribe is called
			var currentSubs = subs;
			var value = _Json_unwrap(converter(cmdList.a));
			for (var i = 0; i < currentSubs.length; i++)
			{
				currentSubs[i](value);
			}
		}
		return init;
	});

	// PUBLIC API

	function subscribe(callback)
	{
		subs.push(callback);
	}

	function unsubscribe(callback)
	{
		// copy subs into a new array in case unsubscribe is called within a
		// subscribed callback
		subs = subs.slice();
		var index = subs.indexOf(callback);
		if (index >= 0)
		{
			subs.splice(index, 1);
		}
	}

	return {
		subscribe: subscribe,
		unsubscribe: unsubscribe
	};
}



// INCOMING PORTS


function _Platform_incomingPort(name, converter)
{
	_Platform_checkPortName(name);
	_Platform_effectManagers[name] = {
		f: _Platform_incomingPortMap,
		u: converter,
		a: _Platform_setupIncomingPort
	};
	return _Platform_leaf(name);
}


var _Platform_incomingPortMap = F2(function(tagger, finalTagger)
{
	return function(value)
	{
		return tagger(finalTagger(value));
	};
});


function _Platform_setupIncomingPort(name, sendToApp)
{
	var subs = _List_Nil;
	var converter = _Platform_effectManagers[name].u;

	// CREATE MANAGER

	var init = _Scheduler_succeed(null);

	_Platform_effectManagers[name].b = init;
	_Platform_effectManagers[name].c = F3(function(router, subList, state)
	{
		subs = subList;
		return init;
	});

	// PUBLIC API

	function send(incomingValue)
	{
		var result = A2(_Json_run, converter, _Json_wrap(incomingValue));

		$elm$core$Result$isOk(result) || _Debug_crash(4, name, result.a);

		var value = result.a;
		for (var temp = subs; temp.b; temp = temp.b) // WHILE_CONS
		{
			sendToApp(temp.a(value));
		}
	}

	return { send: send };
}



// EXPORT ELM MODULES
//
// Have DEBUG and PROD versions so that we can (1) give nicer errors in
// debug mode and (2) not pay for the bits needed for that in prod mode.
//


function _Platform_export_UNUSED(exports)
{
	scope['Elm']
		? _Platform_mergeExportsProd(scope['Elm'], exports)
		: scope['Elm'] = exports;
}


function _Platform_mergeExportsProd(obj, exports)
{
	for (var name in exports)
	{
		(name in obj)
			? (name == 'init')
				? _Debug_crash(6)
				: _Platform_mergeExportsProd(obj[name], exports[name])
			: (obj[name] = exports[name]);
	}
}


function _Platform_export(exports)
{
	scope['Elm']
		? _Platform_mergeExportsDebug('Elm', scope['Elm'], exports)
		: scope['Elm'] = exports;
}


function _Platform_mergeExportsDebug(moduleName, obj, exports)
{
	for (var name in exports)
	{
		(name in obj)
			? (name == 'init')
				? _Debug_crash(6, moduleName)
				: _Platform_mergeExportsDebug(moduleName + '.' + name, obj[name], exports[name])
			: (obj[name] = exports[name]);
	}
}

// End Kernel $elm$kernel$Platform$$ deps: $elm$kernel$Debug$$, $elm$kernel$Json$$, $elm$kernel$List$$, $elm$kernel$Process$$, $elm$kernel$Scheduler$$, $elm$kernel$Utils$$, $elm$core$Result$isOk

// Begin Kernel $elm$kernel$VirtualDom$$ deps: $elm$kernel$Debug$$, $elm$kernel$Json$$, $elm$kernel$List$$, $elm$kernel$Platform$$, $elm$kernel$Utils$$, $elm$core$Result$isOk, $elm$json$Json$Decode$map, $elm$json$Json$Decode$map2, $elm$json$Json$Decode$succeed, $elm$virtual_dom$VirtualDom$toHandlerInt




// HELPERS


var _VirtualDom_divertHrefToApp;

var _VirtualDom_doc = typeof document !== 'undefined' ? document : {};


function _VirtualDom_appendChild(parent, child)
{
	parent.appendChild(child);
}

var _VirtualDom_init = F4(function(virtualNode, flagDecoder, debugMetadata, args)
{
	// NOTE: this function needs _Platform_export available to work

	/**_UNUSED/
	var node = args['node'];
	//*/
	/**/
	var node = args && args['node'] ? args['node'] : _Debug_crash(0);
	//*/

	node.parentNode.replaceChild(
		_VirtualDom_render(virtualNode, function() {}),
		node
	);

	return {};
});



// TEXT


function _VirtualDom_text(string)
{
	return {
		$: 0,
		a: string
	};
}



// NODE


var _VirtualDom_nodeNS = F2(function(namespace, tag)
{
	return F2(function(factList, kidList)
	{
		for (var kids = [], descendantsCount = 0; kidList.b; kidList = kidList.b) // WHILE_CONS
		{
			var kid = kidList.a;
			descendantsCount += (kid.b || 0);
			kids.push(kid);
		}
		descendantsCount += kids.length;

		return {
			$: 1,
			c: tag,
			d: _VirtualDom_organizeFacts(factList),
			e: kids,
			f: namespace,
			b: descendantsCount
		};
	});
});


var _VirtualDom_node = _VirtualDom_nodeNS(undefined);



// KEYED NODE


var _VirtualDom_keyedNodeNS = F2(function(namespace, tag)
{
	return F2(function(factList, kidList)
	{
		for (var kids = [], descendantsCount = 0; kidList.b; kidList = kidList.b) // WHILE_CONS
		{
			var kid = kidList.a;
			descendantsCount += (kid.b.b || 0);
			kids.push(kid);
		}
		descendantsCount += kids.length;

		return {
			$: 2,
			c: tag,
			d: _VirtualDom_organizeFacts(factList),
			e: kids,
			f: namespace,
			b: descendantsCount
		};
	});
});


var _VirtualDom_keyedNode = _VirtualDom_keyedNodeNS(undefined);



// CUSTOM


function _VirtualDom_custom(factList, model, render, diff)
{
	return {
		$: 3,
		d: _VirtualDom_organizeFacts(factList),
		g: model,
		h: render,
		i: diff
	};
}



// MAP


var _VirtualDom_map = F2(function(tagger, node)
{
	return {
		$: 4,
		j: tagger,
		k: node,
		b: 1 + (node.b || 0)
	};
});



// LAZY


function _VirtualDom_thunk(refs, thunk)
{
	return {
		$: 5,
		l: refs,
		m: thunk,
		k: undefined
	};
}

var _VirtualDom_lazy = F2(function(func, a)
{
	return _VirtualDom_thunk([func, a], function() {
		return func(a);
	});
});

var _VirtualDom_lazy2 = F3(function(func, a, b)
{
	return _VirtualDom_thunk([func, a, b], function() {
		return A2(func, a, b);
	});
});

var _VirtualDom_lazy3 = F4(function(func, a, b, c)
{
	return _VirtualDom_thunk([func, a, b, c], function() {
		return A3(func, a, b, c);
	});
});

var _VirtualDom_lazy4 = F5(function(func, a, b, c, d)
{
	return _VirtualDom_thunk([func, a, b, c, d], function() {
		return A4(func, a, b, c, d);
	});
});

var _VirtualDom_lazy5 = F6(function(func, a, b, c, d, e)
{
	return _VirtualDom_thunk([func, a, b, c, d, e], function() {
		return A5(func, a, b, c, d, e);
	});
});

var _VirtualDom_lazy6 = F7(function(func, a, b, c, d, e, f)
{
	return _VirtualDom_thunk([func, a, b, c, d, e, f], function() {
		return A6(func, a, b, c, d, e, f);
	});
});

var _VirtualDom_lazy7 = F8(function(func, a, b, c, d, e, f, g)
{
	return _VirtualDom_thunk([func, a, b, c, d, e, f, g], function() {
		return A7(func, a, b, c, d, e, f, g);
	});
});

var _VirtualDom_lazy8 = F9(function(func, a, b, c, d, e, f, g, h)
{
	return _VirtualDom_thunk([func, a, b, c, d, e, f, g, h], function() {
		return A8(func, a, b, c, d, e, f, g, h);
	});
});



// FACTS


var _VirtualDom_on = F2(function(key, handler)
{
	return {
		$: 'a0',
		n: key,
		o: handler
	};
});
var _VirtualDom_style = F2(function(key, value)
{
	return {
		$: 'a1',
		n: key,
		o: value
	};
});
var _VirtualDom_property = F2(function(key, value)
{
	return {
		$: 'a2',
		n: key,
		o: value
	};
});
var _VirtualDom_attribute = F2(function(key, value)
{
	return {
		$: 'a3',
		n: key,
		o: value
	};
});
var _VirtualDom_attributeNS = F3(function(namespace, key, value)
{
	return {
		$: 'a4',
		n: key,
		o: { f: namespace, o: value }
	};
});



// XSS ATTACK VECTOR CHECKS


function _VirtualDom_noScript(tag)
{
	return tag == 'script' ? 'p' : tag;
}

function _VirtualDom_noOnOrFormAction(key)
{
	return /^(on|formAction$)/i.test(key) ? 'data-' + key : key;
}

function _VirtualDom_noInnerHtmlOrFormAction(key)
{
	return key == 'innerHTML' || key == 'formAction' ? 'data-' + key : key;
}

function _VirtualDom_noJavaScriptUri_UNUSED(value)
{
	return /^javascript:/i.test(value.replace(/\s/g,'')) ? '' : value;
}

function _VirtualDom_noJavaScriptUri(value)
{
	return /^javascript:/i.test(value.replace(/\s/g,''))
		? 'javascript:alert("This is an XSS vector. Please use ports or web components instead.")'
		: value;
}

function _VirtualDom_noJavaScriptOrHtmlUri_UNUSED(value)
{
	return /^\s*(javascript:|data:text\/html)/i.test(value) ? '' : value;
}

function _VirtualDom_noJavaScriptOrHtmlUri(value)
{
	return /^\s*(javascript:|data:text\/html)/i.test(value)
		? 'javascript:alert("This is an XSS vector. Please use ports or web components instead.")'
		: value;
}



// MAP FACTS


var _VirtualDom_mapAttribute = F2(function(func, attr)
{
	return (attr.$ === 'a0')
		? A2(_VirtualDom_on, attr.n, _VirtualDom_mapHandler(func, attr.o))
		: attr;
});

function _VirtualDom_mapHandler(func, handler)
{
	var tag = $elm$virtual_dom$VirtualDom$toHandlerInt(handler);

	// 0 = Normal
	// 1 = MayStopPropagation
	// 2 = MayPreventDefault
	// 3 = Custom

	return {
		$: handler.$,
		a:
			!tag
				? A2($elm$json$Json$Decode$map, func, handler.a)
				:
			A3($elm$json$Json$Decode$map2,
				tag < 3
					? _VirtualDom_mapEventTuple
					: _VirtualDom_mapEventRecord,
				$elm$json$Json$Decode$succeed(func),
				handler.a
			)
	};
}

var _VirtualDom_mapEventTuple = F2(function(func, tuple)
{
	return _Utils_Tuple2(func(tuple.a), tuple.b);
});

var _VirtualDom_mapEventRecord = F2(function(func, record)
{
	return {
		message: func(record.message),
		stopPropagation: record.stopPropagation,
		preventDefault: record.preventDefault
	}
});



// ORGANIZE FACTS


function _VirtualDom_organizeFacts(factList)
{
	for (var facts = {}; factList.b; factList = factList.b) // WHILE_CONS
	{
		var entry = factList.a;

		var tag = entry.$;
		var key = entry.n;
		var value = entry.o;

		if (tag === 'a2')
		{
			(key === 'className')
				? _VirtualDom_addClass(facts, key, _Json_unwrap(value))
				: facts[key] = _Json_unwrap(value);

			continue;
		}

		var subFacts = facts[tag] || (facts[tag] = {});
		(tag === 'a3' && key === 'class')
			? _VirtualDom_addClass(subFacts, key, value)
			: subFacts[key] = value;
	}

	return facts;
}

function _VirtualDom_addClass(object, key, newClass)
{
	var classes = object[key];
	object[key] = classes ? classes + ' ' + newClass : newClass;
}



// RENDER


function _VirtualDom_render(vNode, eventNode)
{
	var tag = vNode.$;

	if (tag === 5)
	{
		return _VirtualDom_render(vNode.k || (vNode.k = vNode.m()), eventNode);
	}

	if (tag === 0)
	{
		return _VirtualDom_doc.createTextNode(vNode.a);
	}

	if (tag === 4)
	{
		var subNode = vNode.k;
		var tagger = vNode.j;

		while (subNode.$ === 4)
		{
			typeof tagger !== 'object'
				? tagger = [tagger, subNode.j]
				: tagger.push(subNode.j);

			subNode = subNode.k;
		}

		var subEventRoot = { j: tagger, p: eventNode };
		var domNode = _VirtualDom_render(subNode, subEventRoot);
		domNode.elm_event_node_ref = subEventRoot;
		return domNode;
	}

	if (tag === 3)
	{
		var domNode = vNode.h(vNode.g);
		_VirtualDom_applyFacts(domNode, eventNode, vNode.d);
		return domNode;
	}

	// at this point `tag` must be 1 or 2

	var domNode = vNode.f
		? _VirtualDom_doc.createElementNS(vNode.f, vNode.c)
		: _VirtualDom_doc.createElement(vNode.c);

	if (_VirtualDom_divertHrefToApp && vNode.c == 'a')
	{
		domNode.addEventListener('click', _VirtualDom_divertHrefToApp(domNode));
	}

	_VirtualDom_applyFacts(domNode, eventNode, vNode.d);

	for (var kids = vNode.e, i = 0; i < kids.length; i++)
	{
		_VirtualDom_appendChild(domNode, _VirtualDom_render(tag === 1 ? kids[i] : kids[i].b, eventNode));
	}

	return domNode;
}



// APPLY FACTS


function _VirtualDom_applyFacts(domNode, eventNode, facts)
{
	for (var key in facts)
	{
		var value = facts[key];

		key === 'a1'
			? _VirtualDom_applyStyles(domNode, value)
			:
		key === 'a0'
			? _VirtualDom_applyEvents(domNode, eventNode, value)
			:
		key === 'a3'
			? _VirtualDom_applyAttrs(domNode, value)
			:
		key === 'a4'
			? _VirtualDom_applyAttrsNS(domNode, value)
			:
		((key !== 'value' && key !== 'checked') || domNode[key] !== value) && (domNode[key] = value);
	}
}



// APPLY STYLES


function _VirtualDom_applyStyles(domNode, styles)
{
	var domNodeStyle = domNode.style;

	for (var key in styles)
	{
		domNodeStyle[key] = styles[key];
	}
}



// APPLY ATTRS


function _VirtualDom_applyAttrs(domNode, attrs)
{
	for (var key in attrs)
	{
		var value = attrs[key];
		typeof value !== 'undefined'
			? domNode.setAttribute(key, value)
			: domNode.removeAttribute(key);
	}
}



// APPLY NAMESPACED ATTRS


function _VirtualDom_applyAttrsNS(domNode, nsAttrs)
{
	for (var key in nsAttrs)
	{
		var pair = nsAttrs[key];
		var namespace = pair.f;
		var value = pair.o;

		typeof value !== 'undefined'
			? domNode.setAttributeNS(namespace, key, value)
			: domNode.removeAttributeNS(namespace, key);
	}
}



// APPLY EVENTS


function _VirtualDom_applyEvents(domNode, eventNode, events)
{
	var allCallbacks = domNode.elmFs || (domNode.elmFs = {});

	for (var key in events)
	{
		var newHandler = events[key];
		var oldCallback = allCallbacks[key];

		if (!newHandler)
		{
			domNode.removeEventListener(key, oldCallback);
			allCallbacks[key] = undefined;
			continue;
		}

		if (oldCallback)
		{
			var oldHandler = oldCallback.q;
			if (oldHandler.$ === newHandler.$)
			{
				oldCallback.q = newHandler;
				continue;
			}
			domNode.removeEventListener(key, oldCallback);
		}

		oldCallback = _VirtualDom_makeCallback(eventNode, newHandler);
		domNode.addEventListener(key, oldCallback,
			_VirtualDom_passiveSupported
			&& { passive: $elm$virtual_dom$VirtualDom$toHandlerInt(newHandler) < 2 }
		);
		allCallbacks[key] = oldCallback;
	}
}



// PASSIVE EVENTS


var _VirtualDom_passiveSupported;

try
{
	window.addEventListener('t', null, Object.defineProperty({}, 'passive', {
		get: function() { _VirtualDom_passiveSupported = true; }
	}));
}
catch(e) {}



// EVENT HANDLERS


function _VirtualDom_makeCallback(eventNode, initialHandler)
{
	function callback(event)
	{
		var handler = callback.q;
		var result = _Json_runHelp(handler.a, event);

		if (!$elm$core$Result$isOk(result))
		{
			return;
		}

		var tag = $elm$virtual_dom$VirtualDom$toHandlerInt(handler);

		// 0 = Normal
		// 1 = MayStopPropagation
		// 2 = MayPreventDefault
		// 3 = Custom

		var value = result.a;
		var message = !tag ? value : tag < 3 ? value.a : value.message;
		var stopPropagation = tag == 1 ? value.b : tag == 3 && value.stopPropagation;
		var currentEventNode = (
			stopPropagation && event.stopPropagation(),
			(tag == 2 ? value.b : tag == 3 && value.preventDefault) && event.preventDefault(),
			eventNode
		);
		var tagger;
		var i;
		while (tagger = currentEventNode.j)
		{
			if (typeof tagger == 'function')
			{
				message = tagger(message);
			}
			else
			{
				for (var i = tagger.length; i--; )
				{
					message = tagger[i](message);
				}
			}
			currentEventNode = currentEventNode.p;
		}
		currentEventNode(message, stopPropagation); // stopPropagation implies isSync
	}

	callback.q = initialHandler;

	return callback;
}

function _VirtualDom_equalEvents(x, y)
{
	return x.$ == y.$ && _Json_equality(x.a, y.a);
}



// DIFF


// TODO: Should we do patches like in iOS?
//
// type Patch
//   = At Int Patch
//   | Batch (List Patch)
//   | Change ...
//
// How could it not be better?
//
function _VirtualDom_diff(x, y)
{
	var patches = [];
	_VirtualDom_diffHelp(x, y, patches, 0);
	return patches;
}


function _VirtualDom_pushPatch(patches, type, index, data)
{
	var patch = {
		$: type,
		r: index,
		s: data,
		t: undefined,
		u: undefined
	};
	patches.push(patch);
	return patch;
}


function _VirtualDom_diffHelp(x, y, patches, index)
{
	if (x === y)
	{
		return;
	}

	var xType = x.$;
	var yType = y.$;

	// Bail if you run into different types of nodes. Implies that the
	// structure has changed significantly and it's not worth a diff.
	if (xType !== yType)
	{
		if (xType === 1 && yType === 2)
		{
			y = _VirtualDom_dekey(y);
			yType = 1;
		}
		else
		{
			_VirtualDom_pushPatch(patches, 0, index, y);
			return;
		}
	}

	// Now we know that both nodes are the same $.
	switch (yType)
	{
		case 5:
			var xRefs = x.l;
			var yRefs = y.l;
			var i = xRefs.length;
			var same = i === yRefs.length;
			while (same && i--)
			{
				same = xRefs[i] === yRefs[i];
			}
			if (same)
			{
				y.k = x.k;
				return;
			}
			y.k = y.m();
			var subPatches = [];
			_VirtualDom_diffHelp(x.k, y.k, subPatches, 0);
			subPatches.length > 0 && _VirtualDom_pushPatch(patches, 1, index, subPatches);
			return;

		case 4:
			// gather nested taggers
			var xTaggers = x.j;
			var yTaggers = y.j;
			var nesting = false;

			var xSubNode = x.k;
			while (xSubNode.$ === 4)
			{
				nesting = true;

				typeof xTaggers !== 'object'
					? xTaggers = [xTaggers, xSubNode.j]
					: xTaggers.push(xSubNode.j);

				xSubNode = xSubNode.k;
			}

			var ySubNode = y.k;
			while (ySubNode.$ === 4)
			{
				nesting = true;

				typeof yTaggers !== 'object'
					? yTaggers = [yTaggers, ySubNode.j]
					: yTaggers.push(ySubNode.j);

				ySubNode = ySubNode.k;
			}

			// Just bail if different numbers of taggers. This implies the
			// structure of the virtual DOM has changed.
			if (nesting && xTaggers.length !== yTaggers.length)
			{
				_VirtualDom_pushPatch(patches, 0, index, y);
				return;
			}

			// check if taggers are "the same"
			if (nesting ? !_VirtualDom_pairwiseRefEqual(xTaggers, yTaggers) : xTaggers !== yTaggers)
			{
				_VirtualDom_pushPatch(patches, 2, index, yTaggers);
			}

			// diff everything below the taggers
			_VirtualDom_diffHelp(xSubNode, ySubNode, patches, index + 1);
			return;

		case 0:
			if (x.a !== y.a)
			{
				_VirtualDom_pushPatch(patches, 3, index, y.a);
			}
			return;

		case 1:
			_VirtualDom_diffNodes(x, y, patches, index, _VirtualDom_diffKids);
			return;

		case 2:
			_VirtualDom_diffNodes(x, y, patches, index, _VirtualDom_diffKeyedKids);
			return;

		case 3:
			if (x.h !== y.h)
			{
				_VirtualDom_pushPatch(patches, 0, index, y);
				return;
			}

			var factsDiff = _VirtualDom_diffFacts(x.d, y.d);
			factsDiff && _VirtualDom_pushPatch(patches, 4, index, factsDiff);

			var patch = y.i(x.g, y.g);
			patch && _VirtualDom_pushPatch(patches, 5, index, patch);

			return;
	}
}

// assumes the incoming arrays are the same length
function _VirtualDom_pairwiseRefEqual(as, bs)
{
	for (var i = 0; i < as.length; i++)
	{
		if (as[i] !== bs[i])
		{
			return false;
		}
	}

	return true;
}

function _VirtualDom_diffNodes(x, y, patches, index, diffKids)
{
	// Bail if obvious indicators have changed. Implies more serious
	// structural changes such that it's not worth it to diff.
	if (x.c !== y.c || x.f !== y.f)
	{
		_VirtualDom_pushPatch(patches, 0, index, y);
		return;
	}

	var factsDiff = _VirtualDom_diffFacts(x.d, y.d);
	factsDiff && _VirtualDom_pushPatch(patches, 4, index, factsDiff);

	diffKids(x, y, patches, index);
}



// DIFF FACTS


// TODO Instead of creating a new diff object, it's possible to just test if
// there *is* a diff. During the actual patch, do the diff again and make the
// modifications directly. This way, there's no new allocations. Worth it?
function _VirtualDom_diffFacts(x, y, category)
{
	var diff;

	// look for changes and removals
	for (var xKey in x)
	{
		if (xKey === 'a1' || xKey === 'a0' || xKey === 'a3' || xKey === 'a4')
		{
			var subDiff = _VirtualDom_diffFacts(x[xKey], y[xKey] || {}, xKey);
			if (subDiff)
			{
				diff = diff || {};
				diff[xKey] = subDiff;
			}
			continue;
		}

		// remove if not in the new facts
		if (!(xKey in y))
		{
			diff = diff || {};
			diff[xKey] =
				!category
					? (typeof x[xKey] === 'string' ? '' : null)
					:
				(category === 'a1')
					? ''
					:
				(category === 'a0' || category === 'a3')
					? undefined
					:
				{ f: x[xKey].f, o: undefined };

			continue;
		}

		var xValue = x[xKey];
		var yValue = y[xKey];

		// reference equal, so don't worry about it
		if (xValue === yValue && xKey !== 'value' && xKey !== 'checked'
			|| category === 'a0' && _VirtualDom_equalEvents(xValue, yValue))
		{
			continue;
		}

		diff = diff || {};
		diff[xKey] = yValue;
	}

	// add new stuff
	for (var yKey in y)
	{
		if (!(yKey in x))
		{
			diff = diff || {};
			diff[yKey] = y[yKey];
		}
	}

	return diff;
}



// DIFF KIDS


function _VirtualDom_diffKids(xParent, yParent, patches, index)
{
	var xKids = xParent.e;
	var yKids = yParent.e;

	var xLen = xKids.length;
	var yLen = yKids.length;

	// FIGURE OUT IF THERE ARE INSERTS OR REMOVALS

	if (xLen > yLen)
	{
		_VirtualDom_pushPatch(patches, 6, index, {
			v: yLen,
			i: xLen - yLen
		});
	}
	else if (xLen < yLen)
	{
		_VirtualDom_pushPatch(patches, 7, index, {
			v: xLen,
			e: yKids
		});
	}

	// PAIRWISE DIFF EVERYTHING ELSE

	for (var minLen = xLen < yLen ? xLen : yLen, i = 0; i < minLen; i++)
	{
		var xKid = xKids[i];
		_VirtualDom_diffHelp(xKid, yKids[i], patches, ++index);
		index += xKid.b || 0;
	}
}



// KEYED DIFF


function _VirtualDom_diffKeyedKids(xParent, yParent, patches, rootIndex)
{
	var localPatches = [];

	var changes = {}; // Dict String Entry
	var inserts = []; // Array { index : Int, entry : Entry }
	// type Entry = { tag : String, vnode : VNode, index : Int, data : _ }

	var xKids = xParent.e;
	var yKids = yParent.e;
	var xLen = xKids.length;
	var yLen = yKids.length;
	var xIndex = 0;
	var yIndex = 0;

	var index = rootIndex;

	while (xIndex < xLen && yIndex < yLen)
	{
		var x = xKids[xIndex];
		var y = yKids[yIndex];

		var xKey = x.a;
		var yKey = y.a;
		var xNode = x.b;
		var yNode = y.b;

		var newMatch = undefined;
		var oldMatch = undefined;

		// check if keys match

		if (xKey === yKey)
		{
			index++;
			_VirtualDom_diffHelp(xNode, yNode, localPatches, index);
			index += xNode.b || 0;

			xIndex++;
			yIndex++;
			continue;
		}

		// look ahead 1 to detect insertions and removals.

		var xNext = xKids[xIndex + 1];
		var yNext = yKids[yIndex + 1];

		if (xNext)
		{
			var xNextKey = xNext.a;
			var xNextNode = xNext.b;
			oldMatch = yKey === xNextKey;
		}

		if (yNext)
		{
			var yNextKey = yNext.a;
			var yNextNode = yNext.b;
			newMatch = xKey === yNextKey;
		}


		// swap x and y
		if (newMatch && oldMatch)
		{
			index++;
			_VirtualDom_diffHelp(xNode, yNextNode, localPatches, index);
			_VirtualDom_insertNode(changes, localPatches, xKey, yNode, yIndex, inserts);
			index += xNode.b || 0;

			index++;
			_VirtualDom_removeNode(changes, localPatches, xKey, xNextNode, index);
			index += xNextNode.b || 0;

			xIndex += 2;
			yIndex += 2;
			continue;
		}

		// insert y
		if (newMatch)
		{
			index++;
			_VirtualDom_insertNode(changes, localPatches, yKey, yNode, yIndex, inserts);
			_VirtualDom_diffHelp(xNode, yNextNode, localPatches, index);
			index += xNode.b || 0;

			xIndex += 1;
			yIndex += 2;
			continue;
		}

		// remove x
		if (oldMatch)
		{
			index++;
			_VirtualDom_removeNode(changes, localPatches, xKey, xNode, index);
			index += xNode.b || 0;

			index++;
			_VirtualDom_diffHelp(xNextNode, yNode, localPatches, index);
			index += xNextNode.b || 0;

			xIndex += 2;
			yIndex += 1;
			continue;
		}

		// remove x, insert y
		if (xNext && xNextKey === yNextKey)
		{
			index++;
			_VirtualDom_removeNode(changes, localPatches, xKey, xNode, index);
			_VirtualDom_insertNode(changes, localPatches, yKey, yNode, yIndex, inserts);
			index += xNode.b || 0;

			index++;
			_VirtualDom_diffHelp(xNextNode, yNextNode, localPatches, index);
			index += xNextNode.b || 0;

			xIndex += 2;
			yIndex += 2;
			continue;
		}

		break;
	}

	// eat up any remaining nodes with removeNode and insertNode

	while (xIndex < xLen)
	{
		index++;
		var x = xKids[xIndex];
		var xNode = x.b;
		_VirtualDom_removeNode(changes, localPatches, x.a, xNode, index);
		index += xNode.b || 0;
		xIndex++;
	}

	while (yIndex < yLen)
	{
		var endInserts = endInserts || [];
		var y = yKids[yIndex];
		_VirtualDom_insertNode(changes, localPatches, y.a, y.b, undefined, endInserts);
		yIndex++;
	}

	if (localPatches.length > 0 || inserts.length > 0 || endInserts)
	{
		_VirtualDom_pushPatch(patches, 8, rootIndex, {
			w: localPatches,
			x: inserts,
			y: endInserts
		});
	}
}



// CHANGES FROM KEYED DIFF


var _VirtualDom_POSTFIX = '_elmW6BL';


function _VirtualDom_insertNode(changes, localPatches, key, vnode, yIndex, inserts)
{
	var entry = changes[key];

	// never seen this key before
	if (!entry)
	{
		entry = {
			c: 0,
			z: vnode,
			r: yIndex,
			s: undefined
		};

		inserts.push({ r: yIndex, A: entry });
		changes[key] = entry;

		return;
	}

	// this key was removed earlier, a match!
	if (entry.c === 1)
	{
		inserts.push({ r: yIndex, A: entry });

		entry.c = 2;
		var subPatches = [];
		_VirtualDom_diffHelp(entry.z, vnode, subPatches, entry.r);
		entry.r = yIndex;
		entry.s.s = {
			w: subPatches,
			A: entry
		};

		return;
	}

	// this key has already been inserted or moved, a duplicate!
	_VirtualDom_insertNode(changes, localPatches, key + _VirtualDom_POSTFIX, vnode, yIndex, inserts);
}


function _VirtualDom_removeNode(changes, localPatches, key, vnode, index)
{
	var entry = changes[key];

	// never seen this key before
	if (!entry)
	{
		var patch = _VirtualDom_pushPatch(localPatches, 9, index, undefined);

		changes[key] = {
			c: 1,
			z: vnode,
			r: index,
			s: patch
		};

		return;
	}

	// this key was inserted earlier, a match!
	if (entry.c === 0)
	{
		entry.c = 2;
		var subPatches = [];
		_VirtualDom_diffHelp(vnode, entry.z, subPatches, index);

		_VirtualDom_pushPatch(localPatches, 9, index, {
			w: subPatches,
			A: entry
		});

		return;
	}

	// this key has already been removed or moved, a duplicate!
	_VirtualDom_removeNode(changes, localPatches, key + _VirtualDom_POSTFIX, vnode, index);
}



// ADD DOM NODES
//
// Each DOM node has an "index" assigned in order of traversal. It is important
// to minimize our crawl over the actual DOM, so these indexes (along with the
// descendantsCount of virtual nodes) let us skip touching entire subtrees of
// the DOM if we know there are no patches there.


function _VirtualDom_addDomNodes(domNode, vNode, patches, eventNode)
{
	_VirtualDom_addDomNodesHelp(domNode, vNode, patches, 0, 0, vNode.b, eventNode);
}


// assumes `patches` is non-empty and indexes increase monotonically.
function _VirtualDom_addDomNodesHelp(domNode, vNode, patches, i, low, high, eventNode)
{
	var patch = patches[i];
	var index = patch.r;

	while (index === low)
	{
		var patchType = patch.$;

		if (patchType === 1)
		{
			_VirtualDom_addDomNodes(domNode, vNode.k, patch.s, eventNode);
		}
		else if (patchType === 8)
		{
			patch.t = domNode;
			patch.u = eventNode;

			var subPatches = patch.s.w;
			if (subPatches.length > 0)
			{
				_VirtualDom_addDomNodesHelp(domNode, vNode, subPatches, 0, low, high, eventNode);
			}
		}
		else if (patchType === 9)
		{
			patch.t = domNode;
			patch.u = eventNode;

			var data = patch.s;
			if (data)
			{
				data.A.s = domNode;
				var subPatches = data.w;
				if (subPatches.length > 0)
				{
					_VirtualDom_addDomNodesHelp(domNode, vNode, subPatches, 0, low, high, eventNode);
				}
			}
		}
		else
		{
			patch.t = domNode;
			patch.u = eventNode;
		}

		i++;

		if (!(patch = patches[i]) || (index = patch.r) > high)
		{
			return i;
		}
	}

	var tag = vNode.$;

	if (tag === 4)
	{
		var subNode = vNode.k;

		while (subNode.$ === 4)
		{
			subNode = subNode.k;
		}

		return _VirtualDom_addDomNodesHelp(domNode, subNode, patches, i, low + 1, high, domNode.elm_event_node_ref);
	}

	// tag must be 1 or 2 at this point

	var vKids = vNode.e;
	var childNodes = domNode.childNodes;
	for (var j = 0; j < vKids.length; j++)
	{
		low++;
		var vKid = tag === 1 ? vKids[j] : vKids[j].b;
		var nextLow = low + (vKid.b || 0);
		if (low <= index && index <= nextLow)
		{
			i = _VirtualDom_addDomNodesHelp(childNodes[j], vKid, patches, i, low, nextLow, eventNode);
			if (!(patch = patches[i]) || (index = patch.r) > high)
			{
				return i;
			}
		}
		low = nextLow;
	}
	return i;
}



// APPLY PATCHES


function _VirtualDom_applyPatches(rootDomNode, oldVirtualNode, patches, eventNode)
{
	if (patches.length === 0)
	{
		return rootDomNode;
	}

	_VirtualDom_addDomNodes(rootDomNode, oldVirtualNode, patches, eventNode);
	return _VirtualDom_applyPatchesHelp(rootDomNode, patches);
}

function _VirtualDom_applyPatchesHelp(rootDomNode, patches)
{
	for (var i = 0; i < patches.length; i++)
	{
		var patch = patches[i];
		var localDomNode = patch.t
		var newNode = _VirtualDom_applyPatch(localDomNode, patch);
		if (localDomNode === rootDomNode)
		{
			rootDomNode = newNode;
		}
	}
	return rootDomNode;
}

function _VirtualDom_applyPatch(domNode, patch)
{
	switch (patch.$)
	{
		case 0:
			return _VirtualDom_applyPatchRedraw(domNode, patch.s, patch.u);

		case 4:
			_VirtualDom_applyFacts(domNode, patch.u, patch.s);
			return domNode;

		case 3:
			domNode.replaceData(0, domNode.length, patch.s);
			return domNode;

		case 1:
			return _VirtualDom_applyPatchesHelp(domNode, patch.s);

		case 2:
			if (domNode.elm_event_node_ref)
			{
				domNode.elm_event_node_ref.j = patch.s;
			}
			else
			{
				domNode.elm_event_node_ref = { j: patch.s, p: patch.u };
			}
			return domNode;

		case 6:
			var data = patch.s;
			for (var i = 0; i < data.i; i++)
			{
				domNode.removeChild(domNode.childNodes[data.v]);
			}
			return domNode;

		case 7:
			var data = patch.s;
			var kids = data.e;
			var i = data.v;
			var theEnd = domNode.childNodes[i];
			for (; i < kids.length; i++)
			{
				domNode.insertBefore(_VirtualDom_render(kids[i], patch.u), theEnd);
			}
			return domNode;

		case 9:
			var data = patch.s;
			if (!data)
			{
				domNode.parentNode.removeChild(domNode);
				return domNode;
			}
			var entry = data.A;
			if (typeof entry.r !== 'undefined')
			{
				domNode.parentNode.removeChild(domNode);
			}
			entry.s = _VirtualDom_applyPatchesHelp(domNode, data.w);
			return domNode;

		case 8:
			return _VirtualDom_applyPatchReorder(domNode, patch);

		case 5:
			return patch.s(domNode);

		default:
			_Debug_crash(10); // 'Ran into an unknown patch!'
	}
}


function _VirtualDom_applyPatchRedraw(domNode, vNode, eventNode)
{
	var parentNode = domNode.parentNode;
	var newNode = _VirtualDom_render(vNode, eventNode);

	if (!newNode.elm_event_node_ref)
	{
		newNode.elm_event_node_ref = domNode.elm_event_node_ref;
	}

	if (parentNode && newNode !== domNode)
	{
		parentNode.replaceChild(newNode, domNode);
	}
	return newNode;
}


function _VirtualDom_applyPatchReorder(domNode, patch)
{
	var data = patch.s;

	// remove end inserts
	var frag = _VirtualDom_applyPatchReorderEndInsertsHelp(data.y, patch);

	// removals
	domNode = _VirtualDom_applyPatchesHelp(domNode, data.w);

	// inserts
	var inserts = data.x;
	for (var i = 0; i < inserts.length; i++)
	{
		var insert = inserts[i];
		var entry = insert.A;
		var node = entry.c === 2
			? entry.s
			: _VirtualDom_render(entry.z, patch.u);
		domNode.insertBefore(node, domNode.childNodes[insert.r]);
	}

	// add end inserts
	if (frag)
	{
		_VirtualDom_appendChild(domNode, frag);
	}

	return domNode;
}


function _VirtualDom_applyPatchReorderEndInsertsHelp(endInserts, patch)
{
	if (!endInserts)
	{
		return;
	}

	var frag = _VirtualDom_doc.createDocumentFragment();
	for (var i = 0; i < endInserts.length; i++)
	{
		var insert = endInserts[i];
		var entry = insert.A;
		_VirtualDom_appendChild(frag, entry.c === 2
			? entry.s
			: _VirtualDom_render(entry.z, patch.u)
		);
	}
	return frag;
}


function _VirtualDom_virtualize(node)
{
	// TEXT NODES

	if (node.nodeType === 3)
	{
		return _VirtualDom_text(node.textContent);
	}


	// WEIRD NODES

	if (node.nodeType !== 1)
	{
		return _VirtualDom_text('');
	}


	// ELEMENT NODES

	var attrList = _List_Nil;
	var attrs = node.attributes;
	for (var i = attrs.length; i--; )
	{
		var attr = attrs[i];
		var name = attr.name;
		var value = attr.value;
		attrList = _List_Cons( A2(_VirtualDom_attribute, name, value), attrList );
	}

	var tag = node.tagName.toLowerCase();
	var kidList = _List_Nil;
	var kids = node.childNodes;

	for (var i = kids.length; i--; )
	{
		kidList = _List_Cons(_VirtualDom_virtualize(kids[i]), kidList);
	}
	return A3(_VirtualDom_node, tag, attrList, kidList);
}

function _VirtualDom_dekey(keyedNode)
{
	var keyedKids = keyedNode.e;
	var len = keyedKids.length;
	var kids = new Array(len);
	for (var i = 0; i < len; i++)
	{
		kids[i] = keyedKids[i].b;
	}

	return {
		$: 1,
		c: keyedNode.c,
		d: keyedNode.d,
		e: kids,
		f: keyedNode.f,
		b: keyedNode.b
	};
}

// End Kernel $elm$kernel$VirtualDom$$ deps: $elm$kernel$Debug$$, $elm$kernel$Json$$, $elm$kernel$List$$, $elm$kernel$Platform$$, $elm$kernel$Utils$$, $elm$core$Result$isOk, $elm$json$Json$Decode$map, $elm$json$Json$Decode$map2, $elm$json$Json$Decode$succeed, $elm$virtual_dom$VirtualDom$toHandlerInt

// Begin Kernel $elm$kernel$Browser$$ deps: $elm$kernel$Debug$$, $elm$kernel$Debugger$$, $elm$kernel$Json$$, $elm$kernel$List$$, $elm$kernel$Platform$$, $elm$kernel$Scheduler$$, $elm$kernel$Utils$$, $elm$kernel$VirtualDom$$, $elm$browser$Browser$External, $elm$browser$Browser$Internal, $elm$core$Maybe$Just, $elm$browser$Browser$Dom$NotFound, $elm$core$Maybe$Nothing, $elm$url$Url$fromString, $elm$core$Result$isOk, $elm$core$Basics$never, $elm$core$Task$perform




// ELEMENT


var _Debugger_element;

var _Browser_element = _Debugger_element || F4(function(impl, flagDecoder, debugMetadata, args)
{
	return _Platform_initialize(
		flagDecoder,
		args,
		impl.init,
		impl.update,
		impl.subscriptions,
		function(sendToApp, initialModel) {
			var view = impl.view;
			/**_UNUSED/
			var domNode = args['node'];
			//*/
			/**/
			var domNode = args && args['node'] ? args['node'] : _Debug_crash(0);
			//*/
			var currNode = _VirtualDom_virtualize(domNode);

			return _Browser_makeAnimator(initialModel, function(model)
			{
				var nextNode = view(model);
				var patches = _VirtualDom_diff(currNode, nextNode);
				domNode = _VirtualDom_applyPatches(domNode, currNode, patches, sendToApp);
				currNode = nextNode;
			});
		}
	);
});



// DOCUMENT


var _Debugger_document;

var _Browser_document = _Debugger_document || F4(function(impl, flagDecoder, debugMetadata, args)
{
	return _Platform_initialize(
		flagDecoder,
		args,
		impl.init,
		impl.update,
		impl.subscriptions,
		function(sendToApp, initialModel) {
			var divertHrefToApp = impl.setup && impl.setup(sendToApp)
			var view = impl.view;
			var title = _VirtualDom_doc.title;
			var bodyNode = _VirtualDom_doc.body;
			var currNode = _VirtualDom_virtualize(bodyNode);
			return _Browser_makeAnimator(initialModel, function(model)
			{
				_VirtualDom_divertHrefToApp = divertHrefToApp;
				var doc = view(model);
				var nextNode = _VirtualDom_node('body')(_List_Nil)(doc.body);
				var patches = _VirtualDom_diff(currNode, nextNode);
				bodyNode = _VirtualDom_applyPatches(bodyNode, currNode, patches, sendToApp);
				currNode = nextNode;
				_VirtualDom_divertHrefToApp = 0;
				(title !== doc.title) && (_VirtualDom_doc.title = title = doc.title);
			});
		}
	);
});



// ANIMATION


var _Browser_cancelAnimationFrame =
	typeof cancelAnimationFrame !== 'undefined'
		? cancelAnimationFrame
		: function(id) { clearTimeout(id); };

var _Browser_requestAnimationFrame =
	typeof requestAnimationFrame !== 'undefined'
		? requestAnimationFrame
		: function(callback) { return setTimeout(callback, 1000 / 60); };


function _Browser_makeAnimator(model, draw)
{
	draw(model);

	var state = 0;

	function updateIfNeeded()
	{
		state = state === 1
			? 0
			: ( _Browser_requestAnimationFrame(updateIfNeeded), draw(model), 1 );
	}

	return function(nextModel, isSync)
	{
		model = nextModel;

		isSync
			? ( draw(model),
				state === 2 && (state = 1)
				)
			: ( state === 0 && _Browser_requestAnimationFrame(updateIfNeeded),
				state = 2
				);
	};
}



// APPLICATION


function _Browser_application(impl)
{
	var onUrlChange = impl.onUrlChange;
	var onUrlRequest = impl.onUrlRequest;
	var key = function() { key.a(onUrlChange(_Browser_getUrl())); };

	return _Browser_document({
		setup: function(sendToApp)
		{
			key.a = sendToApp;
			_Browser_window.addEventListener('popstate', key);
			_Browser_window.navigator.userAgent.indexOf('Trident') < 0 || _Browser_window.addEventListener('hashchange', key);

			return F2(function(domNode, event)
			{
				if (!event.ctrlKey && !event.metaKey && !event.shiftKey && event.button < 1 && !domNode.target && !domNode.hasAttribute('download'))
				{
					event.preventDefault();
					var href = domNode.href;
					var curr = _Browser_getUrl();
					var next = $elm$url$Url$fromString(href).a;
					sendToApp(onUrlRequest(
						(next
							&& curr.protocol === next.protocol
							&& curr.host === next.host
							&& curr.port_.a === next.port_.a
						)
							? $elm$browser$Browser$Internal(next)
							: $elm$browser$Browser$External(href)
					));
				}
			});
		},
		init: function(flags)
		{
			return A3(impl.init, flags, _Browser_getUrl(), key);
		},
		view: impl.view,
		update: impl.update,
		subscriptions: impl.subscriptions
	});
}

function _Browser_getUrl()
{
	return $elm$url$Url$fromString(_VirtualDom_doc.location.href).a || _Debug_crash(1);
}

var _Browser_go = F2(function(key, n)
{
	return A2($elm$core$Task$perform, $elm$core$Basics$never, _Scheduler_binding(function() {
		n && history.go(n);
		key();
	}));
});

var _Browser_pushUrl = F2(function(key, url)
{
	return A2($elm$core$Task$perform, $elm$core$Basics$never, _Scheduler_binding(function() {
		history.pushState({}, '', url);
		key();
	}));
});

var _Browser_replaceUrl = F2(function(key, url)
{
	return A2($elm$core$Task$perform, $elm$core$Basics$never, _Scheduler_binding(function() {
		history.replaceState({}, '', url);
		key();
	}));
});



// GLOBAL EVENTS


var _Browser_fakeNode = { addEventListener: function() {}, removeEventListener: function() {} };
var _Browser_doc = typeof document !== 'undefined' ? document : _Browser_fakeNode;
var _Browser_window = typeof window !== 'undefined' ? window : _Browser_fakeNode;

var _Browser_on = F3(function(node, eventName, sendToSelf)
{
	return _Scheduler_spawn(_Scheduler_binding(function(callback)
	{
		function handler(event)	{ _Scheduler_rawSpawn(sendToSelf(event)); }
		node.addEventListener(eventName, handler, _VirtualDom_passiveSupported && { passive: true });
		return function() { node.removeEventListener(eventName, handler); };
	}));
});

var _Browser_decodeEvent = F2(function(decoder, event)
{
	var result = _Json_runHelp(decoder, event);
	return $elm$core$Result$isOk(result) ? $elm$core$Maybe$Just(result.a) : $elm$core$Maybe$Nothing;
});



// PAGE VISIBILITY


function _Browser_visibilityInfo()
{
	return (typeof _VirtualDom_doc.hidden !== 'undefined')
		? { hidden: 'hidden', change: 'visibilitychange' }
		:
	(typeof _VirtualDom_doc.mozHidden !== 'undefined')
		? { hidden: 'mozHidden', change: 'mozvisibilitychange' }
		:
	(typeof _VirtualDom_doc.msHidden !== 'undefined')
		? { hidden: 'msHidden', change: 'msvisibilitychange' }
		:
	(typeof _VirtualDom_doc.webkitHidden !== 'undefined')
		? { hidden: 'webkitHidden', change: 'webkitvisibilitychange' }
		: { hidden: 'hidden', change: 'visibilitychange' };
}



// ANIMATION FRAMES


function _Browser_rAF()
{
	return _Scheduler_binding(function(callback)
	{
		var id = _Browser_requestAnimationFrame(function() {
			callback(_Scheduler_succeed(Date.now()));
		});

		return function() {
			_Browser_cancelAnimationFrame(id);
		};
	});
}


function _Browser_now()
{
	return _Scheduler_binding(function(callback)
	{
		callback(_Scheduler_succeed(Date.now()));
	});
}



// DOM STUFF


function _Browser_withNode(id, doStuff)
{
	return _Scheduler_binding(function(callback)
	{
		_Browser_requestAnimationFrame(function() {
			var node = document.getElementById(id);
			callback(node
				? _Scheduler_succeed(doStuff(node))
				: _Scheduler_fail($elm$browser$Browser$Dom$NotFound(id))
			);
		});
	});
}


function _Browser_withWindow(doStuff)
{
	return _Scheduler_binding(function(callback)
	{
		_Browser_requestAnimationFrame(function() {
			callback(_Scheduler_succeed(doStuff()));
		});
	});
}


// FOCUS and BLUR


var _Browser_call = F2(function(functionName, id)
{
	return _Browser_withNode(id, function(node) {
		node[functionName]();
		return _Utils_Tuple0;
	});
});



// WINDOW VIEWPORT


function _Browser_getViewport()
{
	return {
		scene: _Browser_getScene(),
		viewport: {
			x: _Browser_window.pageXOffset,
			y: _Browser_window.pageYOffset,
			width: _Browser_doc.documentElement.clientWidth,
			height: _Browser_doc.documentElement.clientHeight
		}
	};
}

function _Browser_getScene()
{
	var body = _Browser_doc.body;
	var elem = _Browser_doc.documentElement;
	return {
		width: Math.max(body.scrollWidth, body.offsetWidth, elem.scrollWidth, elem.offsetWidth, elem.clientWidth),
		height: Math.max(body.scrollHeight, body.offsetHeight, elem.scrollHeight, elem.offsetHeight, elem.clientHeight)
	};
}

var _Browser_setViewport = F2(function(x, y)
{
	return _Browser_withWindow(function()
	{
		_Browser_window.scroll(x, y);
		return _Utils_Tuple0;
	});
});



// ELEMENT VIEWPORT


function _Browser_getViewportOf(id)
{
	return _Browser_withNode(id, function(node)
	{
		return {
			scene: {
				width: node.scrollWidth,
				height: node.scrollHeight
			},
			viewport: {
				x: node.scrollLeft,
				y: node.scrollTop,
				width: node.clientWidth,
				height: node.clientHeight
			}
		};
	});
}


var _Browser_setViewportOf = F3(function(id, x, y)
{
	return _Browser_withNode(id, function(node)
	{
		node.scrollLeft = x;
		node.scrollTop = y;
		return _Utils_Tuple0;
	});
});



// ELEMENT


function _Browser_getElement(id)
{
	return _Browser_withNode(id, function(node)
	{
		var rect = node.getBoundingClientRect();
		var x = _Browser_window.pageXOffset;
		var y = _Browser_window.pageYOffset;
		return {
			scene: _Browser_getScene(),
			viewport: {
				x: x,
				y: y,
				width: _Browser_doc.documentElement.clientWidth,
				height: _Browser_doc.documentElement.clientHeight
			},
			element: {
				x: x + rect.left,
				y: y + rect.top,
				width: rect.width,
				height: rect.height
			}
		};
	});
}



// LOAD and RELOAD


function _Browser_reload(skipCache)
{
	return A2($elm$core$Task$perform, $elm$core$Basics$never, _Scheduler_binding(function(callback)
	{
		_VirtualDom_doc.location.reload(skipCache);
	}));
}

function _Browser_load(url)
{
	return A2($elm$core$Task$perform, $elm$core$Basics$never, _Scheduler_binding(function(callback)
	{
		try
		{
			_Browser_window.location = url;
		}
		catch(err)
		{
			// Only Firefox can throw a NS_ERROR_MALFORMED_URI exception here.
			// Other browsers reload the page, so let's be consistent about that.
			_VirtualDom_doc.location.reload(false);
		}
	}));
}

// End Kernel $elm$kernel$Browser$$ deps: $elm$kernel$Debug$$, $elm$kernel$Debugger$$, $elm$kernel$Json$$, $elm$kernel$List$$, $elm$kernel$Platform$$, $elm$kernel$Scheduler$$, $elm$kernel$Utils$$, $elm$kernel$VirtualDom$$, $elm$browser$Browser$External, $elm$browser$Browser$Internal, $elm$core$Maybe$Just, $elm$browser$Browser$Dom$NotFound, $elm$core$Maybe$Nothing, $elm$url$Url$fromString, $elm$core$Result$isOk, $elm$core$Basics$never, $elm$core$Task$perform

// Begin Kernel $elm$kernel$Http$$ deps: $elm$kernel$Scheduler$$, $elm$kernel$Utils$$, $elm$http$Http$BadStatus_, $elm$http$Http$BadUrl_, $elm$http$Http$GoodStatus_, $elm$core$Maybe$Just, $elm$http$Http$NetworkError_, $elm$core$Maybe$Nothing, $elm$http$Http$Receiving, $elm$http$Http$Sending, $elm$http$Http$Timeout_, $elm$core$Dict$empty, $elm$core$Maybe$isJust, $elm$core$Platform$sendToSelf, $elm$core$Dict$update



// SEND REQUEST

var _Http_toTask = F3(function(router, toTask, request)
{
	return _Scheduler_binding(function(callback)
	{
		function done(response) {
			callback(toTask(request.expect.a(response)));
		}

		var xhr = new XMLHttpRequest();
		xhr.addEventListener('error', function() { done($elm$http$Http$NetworkError_); });
		xhr.addEventListener('timeout', function() { done($elm$http$Http$Timeout_); });
		xhr.addEventListener('load', function() { done(_Http_toResponse(request.expect.b, xhr)); });
		$elm$core$Maybe$isJust(request.tracker) && _Http_track(router, xhr, request.tracker.a);

		try {
			xhr.open(request.method, request.url, true);
		} catch (e) {
			return done($elm$http$Http$BadUrl_(request.url));
		}

		_Http_configureRequest(xhr, request);

		request.body.a && xhr.setRequestHeader('Content-Type', request.body.a);
		xhr.send(request.body.b);

		return function() { xhr.c = true; xhr.abort(); };
	});
});


// CONFIGURE

function _Http_configureRequest(xhr, request)
{
	for (var headers = request.headers; headers.b; headers = headers.b) // WHILE_CONS
	{
		xhr.setRequestHeader(headers.a.a, headers.a.b);
	}
	xhr.timeout = request.timeout.a || 0;
	xhr.responseType = request.expect.d;
	xhr.withCredentials = request.allowCookiesFromOtherDomains;
}


// RESPONSES

function _Http_toResponse(toBody, xhr)
{
	return A2(
		200 <= xhr.status && xhr.status < 300 ? $elm$http$Http$GoodStatus_ : $elm$http$Http$BadStatus_,
		_Http_toMetadata(xhr),
		toBody(xhr.response)
	);
}


// METADATA

function _Http_toMetadata(xhr)
{
	return {
		url: xhr.responseURL,
		statusCode: xhr.status,
		statusText: xhr.statusText,
		headers: _Http_parseHeaders(xhr.getAllResponseHeaders())
	};
}


// HEADERS

function _Http_parseHeaders(rawHeaders)
{
	if (!rawHeaders)
	{
		return $elm$core$Dict$empty;
	}

	var headers = $elm$core$Dict$empty;
	var headerPairs = rawHeaders.split('\r\n');
	for (var i = headerPairs.length; i--; )
	{
		var headerPair = headerPairs[i];
		var index = headerPair.indexOf(': ');
		if (index > 0)
		{
			var key = headerPair.substring(0, index);
			var value = headerPair.substring(index + 2);

			headers = A3($elm$core$Dict$update, key, function(oldValue) {
				return $elm$core$Maybe$Just($elm$core$Maybe$isJust(oldValue)
					? value + ', ' + oldValue.a
					: value
				);
			}, headers);
		}
	}
	return headers;
}


// EXPECT

var _Http_expect = F3(function(type, toBody, toValue)
{
	return {
		$: 0,
		d: type,
		b: toBody,
		a: toValue
	};
});

var _Http_mapExpect = F2(function(func, expect)
{
	return {
		$: 0,
		d: expect.d,
		b: expect.b,
		a: function(x) { return func(expect.a(x)); }
	};
});

function _Http_toDataView(arrayBuffer)
{
	return new DataView(arrayBuffer);
}


// BODY and PARTS

var _Http_emptyBody = { $: 0 };
var _Http_pair = F2(function(a, b) { return { $: 0, a: a, b: b }; });

function _Http_toFormData(parts)
{
	for (var formData = new FormData(); parts.b; parts = parts.b) // WHILE_CONS
	{
		var part = parts.a;
		formData.append(part.a, part.b);
	}
	return formData;
}

var _Http_bytesToBlob = F2(function(mime, bytes)
{
	return new Blob([bytes], { type: mime });
});


// PROGRESS

function _Http_track(router, xhr, tracker)
{
	// TODO check out lengthComputable on loadstart event

	xhr.upload.addEventListener('progress', function(event) {
		if (xhr.c) { return; }
		_Scheduler_rawSpawn(A2($elm$core$Platform$sendToSelf, router, _Utils_Tuple2(tracker, $elm$http$Http$Sending({
			sent: event.loaded,
			size: event.total
		}))));
	});
	xhr.addEventListener('progress', function(event) {
		if (xhr.c) { return; }
		_Scheduler_rawSpawn(A2($elm$core$Platform$sendToSelf, router, _Utils_Tuple2(tracker, $elm$http$Http$Receiving({
			received: event.loaded,
			size: event.lengthComputable ? $elm$core$Maybe$Just(event.total) : $elm$core$Maybe$Nothing
		}))));
	});
}
// End Kernel $elm$kernel$Http$$ deps: $elm$kernel$Scheduler$$, $elm$kernel$Utils$$, $elm$http$Http$BadStatus_, $elm$http$Http$BadUrl_, $elm$http$Http$GoodStatus_, $elm$core$Maybe$Just, $elm$http$Http$NetworkError_, $elm$core$Maybe$Nothing, $elm$http$Http$Receiving, $elm$http$Http$Sending, $elm$http$Http$Timeout_, $elm$core$Dict$empty, $elm$core$Maybe$isJust, $elm$core$Platform$sendToSelf, $elm$core$Dict$update
// Enum
var $elm$core$Basics$EQ = {$: 'EQ'};
// Enum
var $elm$core$Basics$LT = {$: 'LT'};
// Define, deps=$elm$kernel$List$$
var $elm$core$List$cons = /* VarKernel */_List_cons;
// Define, deps=$elm$kernel$JsArray$$
var $elm$core$Elm$JsArray$foldr = /* VarKernel */_JsArray_foldr;
// Define, deps=$elm$core$Elm$JsArray$foldr
var $elm$core$Array$foldr = /* Function */F3(
	function (func, baseCase, _v0) {
		// Destruct
		var tree = _v0.c;
		// Destruct
		var tail = _v0.d;
		// Let
		var helper = /* Function */F2(
			function (node, acc) {
				// Case
				// Decider Chain
				if (node.$ === 'SubTree') {
					// Decider Leaf Inline
					// Destruct
					var subTree = node.a;
					return /* Call */A3($elm$core$Elm$JsArray$foldr, /* VarLocal */helper, /* VarLocal */acc, /* VarLocal */subTree);
				} else {
					// Decider Leaf Inline
					// Destruct
					var values = node.a;
					return /* Call */A3($elm$core$Elm$JsArray$foldr, /* VarLocal */func, /* VarLocal */acc, /* VarLocal */values);
				}
			});
		return /* Call */A3(
			$elm$core$Elm$JsArray$foldr,
			/* VarLocal */helper,
			/* Call */A3($elm$core$Elm$JsArray$foldr, /* VarLocal */func, /* VarLocal */baseCase, /* VarLocal */tail),
			/* VarLocal */tree);
	});
// Define, deps=$elm$kernel$List$$, $elm$core$List$cons, $elm$core$Array$foldr
var $elm$core$Array$toList = /* Function */function (array) {
	return /* Call */A3($elm$core$Array$foldr, /* VarGlobal */$elm$core$List$cons, /* List */_List_Nil, /* VarLocal */array);
};
// Link $elm$core$Dict$_M$foldr
// Cycle foldr
var $elm$core$Dict$foldr = F3(
	function (func, acc, t) {
		foldr:
		while (true) {
			// Case
			// Decider Chain
			if (t.$ === 'RBEmpty_elm_builtin') {
				// Decider Leaf Inline
				return /* VarLocal */acc;
			} else {
				// Decider Leaf Inline
				// Destruct
				var key = t.b;
				// Destruct
				var value = t.c;
				// Destruct
				var left = t.d;
				// Destruct
				var right = t.e;
				// TailCall
				var $temp$func = /* VarLocal */func,
					$temp$acc = /* Call */A3(
					/* VarLocal */func,
					/* VarLocal */key,
					/* VarLocal */value,
					/* Call */A3($elm$core$Dict$foldr, /* VarLocal */func, /* VarLocal */acc, /* VarLocal */right)),
					$temp$t = /* VarLocal */left;
				func = $temp$func;
				acc = $temp$acc;
				t = $temp$t;
				continue foldr;
			}
		}
	});
// end Cycle foldr
// Define, deps=$elm$kernel$List$$, $elm$kernel$Utils$$, $elm$core$List$cons, $elm$core$Dict$foldr
var $elm$core$Dict$toList = /* Function */function (dict) {
	return /* Call */A3(
		$elm$core$Dict$foldr,
		/* Function */F3(
			function (key, value, list) {
				return /* Call */A2(
					$elm$core$List$cons,
					/* Tuple */_Utils_Tuple2(/* VarLocal */key, /* VarLocal */value),
					/* VarLocal */list);
			}),
		/* List */_List_Nil,
		/* VarLocal */dict);
};
// Define, deps=$elm$kernel$List$$, $elm$core$List$cons, $elm$core$Dict$foldr
var $elm$core$Dict$keys = /* Function */function (dict) {
	return /* Call */A3(
		$elm$core$Dict$foldr,
		/* Function */F3(
			function (key, value, keyList) {
				return /* Call */A2($elm$core$List$cons, /* VarLocal */key, /* VarLocal */keyList);
			}),
		/* List */_List_Nil,
		/* VarLocal */dict);
};
// Define, deps=$elm$core$Dict$keys
var $elm$core$Set$toList = /* Function */function (_v0) {
	// Destruct
	var dict = _v0.a;
	return /* Call */$elm$core$Dict$keys(/* VarLocal */dict);
};
// Enum
var $elm$core$Basics$GT = {$: 'GT'};
// Ctor
var $author$project$Main$KeyPressed = function (a) {
	return {$: 'KeyPressed', a: a};
};
// Define, deps=
var $elm$core$Basics$apL = /* Function */F2(
	function (f, x) {
		return /* Call *//* VarLocal */f(/* VarLocal */x);
	});
// Ctor
var $elm$core$Result$Err = function (a) {
	return {$: 'Err', a: a};
};
// Ctor
var $elm$json$Json$Decode$Failure = F2(
	function (a, b) {
		return {$: 'Failure', a: a, b: b};
	});
// Ctor
var $elm$json$Json$Decode$Field = F2(
	function (a, b) {
		return {$: 'Field', a: a, b: b};
	});
// Ctor
var $elm$json$Json$Decode$Index = F2(
	function (a, b) {
		return {$: 'Index', a: a, b: b};
	});
// Ctor
var $elm$core$Result$Ok = function (a) {
	return {$: 'Ok', a: a};
};
// Ctor
var $elm$json$Json$Decode$OneOf = function (a) {
	return {$: 'OneOf', a: a};
};
// Link $elm$json$Json$Decode$_M$errorOneOf
// Enum
var $elm$core$Basics$False = {$: 'False'};
// Define, deps=$elm$kernel$Basics$$
var $elm$core$Basics$add = /* VarKernel */_Basics_add;
// Ctor
var $elm$core$Maybe$Just = function (a) {
	return {$: 'Just', a: a};
};
// Ctor
var $elm$core$Maybe$Nothing = {$: 'Nothing'};
// Define, deps=$elm$kernel$String$$
var $elm$core$String$all = /* VarKernel */_String_all;
// Define, deps=$elm$kernel$Basics$$
var $elm$core$Basics$and = /* VarKernel */_Basics_and;
// Define, deps=$elm$kernel$Utils$$
var $elm$core$Basics$append = /* VarKernel */_Utils_append;
// Define, deps=$elm$kernel$Json$$
var $elm$json$Json$Encode$encode = /* VarKernel */_Json_encode;
// Link $elm$json$Json$Decode$_M$errorOneOf
// Link $elm$json$Json$Decode$_M$errorOneOf
// Define, deps=$elm$kernel$String$$
var $elm$core$String$fromInt = /* VarKernel */_String_fromNumber;
// Define, deps=$elm$kernel$List$$, $elm$kernel$String$$
var $elm$core$String$join = /* Function */F2(
	function (sep, chunks) {
		return /* Call */A2(
			/* VarKernel */_String_join,
			/* VarLocal */sep,
			/* Call *//* VarKernel */_List_toArray(/* VarLocal */chunks));
	});
// Define, deps=$elm$kernel$List$$, $elm$kernel$String$$
var $elm$core$String$split = /* Function */F2(
	function (sep, string) {
		return /* Call *//* VarKernel */_List_fromArray(
			/* Call */A2(/* VarKernel */_String_split, /* VarLocal */sep, /* VarLocal */string));
	});
// Define, deps=$elm$core$String$join, $elm$core$String$split
var $elm$json$Json$Decode$indent = /* Function */function (str) {
	return /* Call */A2(
		$elm$core$String$join,
		/* Str */'\n    ',
		/* Call */A2($elm$core$String$split, /* Str */'\n', /* VarLocal */str));
};
// Link $elm$core$List$_M$foldl
// Cycle foldl
var $elm$core$List$foldl = F3(
	function (func, acc, list) {
		foldl:
		while (true) {
			// Case
			// Decider Chain
			if (!list.b) {
				// Decider Leaf Inline
				return /* VarLocal */acc;
			} else {
				// Decider Leaf Inline
				// Destruct
				var x = list.a;
				// Destruct
				var xs = list.b;
				// TailCall
				var $temp$func = /* VarLocal */func,
					$temp$acc = /* Call */A2(/* VarLocal */func, /* VarLocal */x, /* VarLocal */acc),
					$temp$list = /* VarLocal */xs;
				func = $temp$func;
				acc = $temp$acc;
				list = $temp$list;
				continue foldl;
			}
		}
	});
// end Cycle foldl
// Define, deps=$elm$core$Basics$add, $elm$core$List$foldl
var $elm$core$List$length = /* Function */function (xs) {
	return /* Call */A3(
		$elm$core$List$foldl,
		/* Function */F2(
			function (_v0, i) {
				return /* Call *//* VarLocal */i + /* Int */1;
			}),
		/* Int */0,
		/* VarLocal */xs);
};
// Define, deps=$elm$kernel$List$$
var $elm$core$List$map2 = /* VarKernel */_List_map2;
// Link $elm$core$List$_M$rangeHelp
// Define, deps=$elm$kernel$Utils$$
var $elm$core$Basics$le = /* VarKernel */_Utils_le;
// Define, deps=$elm$kernel$Basics$$
var $elm$core$Basics$sub = /* VarKernel */_Basics_sub;
// Cycle rangeHelp
var $elm$core$List$rangeHelp = F3(
	function (lo, hi, list) {
		rangeHelp:
		while (true) {
			// If
			if (/* Call */_Utils_cmp(/* VarLocal */lo, /* VarLocal */hi) < 1) {
				// TailCall
				var $temp$lo = /* VarLocal */lo,
					$temp$hi = /* Call *//* VarLocal */hi - /* Int */1,
					$temp$list = /* Call */A2($elm$core$List$cons, /* VarLocal */hi, /* VarLocal */list);
				lo = $temp$lo;
				hi = $temp$hi;
				list = $temp$list;
				continue rangeHelp;
			} else {
				return /* VarLocal */list;
			}
		}
	});
// end Cycle rangeHelp
// Define, deps=$elm$kernel$List$$, $elm$core$List$rangeHelp
var $elm$core$List$range = /* Function */F2(
	function (lo, hi) {
		return /* Call */A3($elm$core$List$rangeHelp, /* VarLocal */lo, /* VarLocal */hi, /* List */_List_Nil);
	});
// Define, deps=$elm$core$List$length, $elm$core$List$map2, $elm$core$List$range, $elm$core$Basics$sub
var $elm$core$List$indexedMap = /* Function */F2(
	function (f, xs) {
		return /* Call */A3(
			$elm$core$List$map2,
			/* VarLocal */f,
			/* Call */A2(
				$elm$core$List$range,
				/* Int */0,
				/* Call *//* Call */$elm$core$List$length(/* VarLocal */xs) - /* Int */1),
			/* VarLocal */xs);
	});
// Define, deps=$elm$kernel$Char$$
var $elm$core$Char$toCode = /* VarKernel */_Char_toCode;
// Define, deps=$elm$core$Basics$and, $elm$core$Basics$le, $elm$core$Char$toCode
var $elm$core$Char$isLower = /* Function */function (_char) {
	// Let
	var code = /* Call */$elm$core$Char$toCode(/* VarLocal */_char);
	return /* Call *//* Call */(_Utils_cmp(/* Int */97, /* VarLocal */code) < 1) && /* Call */(_Utils_cmp(/* VarLocal */code, /* Int */122) < 1);
};
// Define, deps=$elm$core$Basics$and, $elm$core$Basics$le, $elm$core$Char$toCode
var $elm$core$Char$isUpper = /* Function */function (_char) {
	// Let
	var code = /* Call */$elm$core$Char$toCode(/* VarLocal */_char);
	return /* Call *//* Call */(_Utils_cmp(/* VarLocal */code, /* Int */90) < 1) && /* Call */(_Utils_cmp(/* Int */65, /* VarLocal */code) < 1);
};
// Define, deps=$elm$kernel$Basics$$
var $elm$core$Basics$or = /* VarKernel */_Basics_or;
// Define, deps=$elm$core$Char$isLower, $elm$core$Char$isUpper, $elm$core$Basics$or
var $elm$core$Char$isAlpha = /* Function */function (_char) {
	return /* Call *//* Call */$elm$core$Char$isLower(/* VarLocal */_char) || /* Call */$elm$core$Char$isUpper(/* VarLocal */_char);
};
// Define, deps=$elm$core$Basics$and, $elm$core$Basics$le, $elm$core$Char$toCode
var $elm$core$Char$isDigit = /* Function */function (_char) {
	// Let
	var code = /* Call */$elm$core$Char$toCode(/* VarLocal */_char);
	return /* Call *//* Call */(_Utils_cmp(/* VarLocal */code, /* Int */57) < 1) && /* Call */(_Utils_cmp(/* Int */48, /* VarLocal */code) < 1);
};
// Define, deps=$elm$core$Char$isDigit, $elm$core$Char$isLower, $elm$core$Char$isUpper, $elm$core$Basics$or
var $elm$core$Char$isAlphaNum = /* Function */function (_char) {
	return /* Call *//* Call */$elm$core$Char$isLower(/* VarLocal */_char) || /* Call */(/* Call */$elm$core$Char$isUpper(/* VarLocal */_char) || /* Call */$elm$core$Char$isDigit(/* VarLocal */_char));
};
// Define, deps=$elm$kernel$List$$, $elm$core$List$cons, $elm$core$List$foldl
var $elm$core$List$reverse = /* Function */function (list) {
	return /* Call */A3($elm$core$List$foldl, /* VarGlobal */$elm$core$List$cons, /* List */_List_Nil, /* VarLocal */list);
};
// Define, deps=$elm$kernel$String$$
var $elm$core$String$uncons = /* VarKernel */_String_uncons;
// Cycle errorOneOf, errorToString, errorToStringHelp
var $elm$json$Json$Decode$errorOneOf = /* Function */F2(
	function (i, error) {
		return /* Call */_Utils_ap(
			/* Str */'\n\n(',
			_Utils_ap(
				/* Call */$elm$core$String$fromInt(/* Call *//* VarLocal */i + /* Int */1),
				_Utils_ap(
					/* Str */') ',
					/* Call *//* VarGlobal */$elm$json$Json$Decode$indent(
						/* Call *//* VarGlobal */$elm$json$Json$Decode$errorToString(/* VarLocal */error)))));
	});
var $elm$json$Json$Decode$errorToString = /* Function */function (error) {
	return /* Call */A2(/* VarGlobal */$elm$json$Json$Decode$errorToStringHelp, /* VarLocal */error, /* List */_List_Nil);
};
var $elm$json$Json$Decode$errorToStringHelp = F2(
	function (error, context) {
		errorToStringHelp:
		while (true) {
			// Case
			// Decider Fanout
			switch (error.$) {
				case 'Field':
					// Decider Leaf Inline
					// Destruct
					var f = error.a;
					// Destruct
					var err = error.b;
					// Let
					var isSimple = function () {
						// Let
						var _v1 = /* Call */$elm$core$String$uncons(/* VarLocal */f);
						// Case
						// Decider Chain
						if (_v1.$ === 'Nothing') {
							// Decider Leaf Inline
							return /* Bool */false;
						} else {
							// Decider Leaf Inline
							// Destruct
							var _v2 = _v1.a;
							// Destruct
							var _char = _v2.a;
							// Destruct
							var rest = _v2.b;
							return /* Call *//* Call */$elm$core$Char$isAlpha(/* VarLocal */_char) && /* Call */A2($elm$core$String$all, /* VarGlobal */$elm$core$Char$isAlphaNum, /* VarLocal */rest);
						}
					}();
					// Let
					var fieldName = /* If *//* VarLocal */isSimple ? /* Call */_Utils_ap(/* Str */'.', /* VarLocal */f) : /* Call */_Utils_ap(
						/* Str */'[\'',
						_Utils_ap(/* VarLocal */f, /* Str */'\']'));
					// TailCall
					var $temp$error = /* VarLocal */err,
						$temp$context = /* Call */A2($elm$core$List$cons, /* VarLocal */fieldName, /* VarLocal */context);
					error = $temp$error;
					context = $temp$context;
					continue errorToStringHelp;
				case 'Index':
					// Decider Leaf Inline
					// Destruct
					var i = error.a;
					// Destruct
					var err = error.b;
					// Let
					var indexName = /* Call */_Utils_ap(
						/* Str */'[',
						_Utils_ap(
							/* Call */$elm$core$String$fromInt(/* VarLocal */i),
							/* Str */']'));
					// TailCall
					var $temp$error = /* VarLocal */err,
						$temp$context = /* Call */A2($elm$core$List$cons, /* VarLocal */indexName, /* VarLocal */context);
					error = $temp$error;
					context = $temp$context;
					continue errorToStringHelp;
				case 'OneOf':
					// Decider Leaf Inline
					// Destruct
					var errors = error.a;
					// Case
					// Decider Chain
					if (!errors.b) {
						// Decider Leaf Inline
						return /* Call */_Utils_ap(
							/* Str */'Ran into a Json.Decode.oneOf with no possibilities',
							function () {
								// Case
								// Decider Chain
								if (!context.b) {
									// Decider Leaf Inline
									return /* Str */'!';
								} else {
									// Decider Leaf Inline
									return /* Call */_Utils_ap(
										/* Str */' at json',
										/* Call */A2(
											$elm$core$String$join,
											/* Str */'',
											/* Call */$elm$core$List$reverse(/* VarLocal */context)));
								}
							}());
					} else {
						// Decider Chain
						if (!errors.b.b) {
							// Decider Leaf Inline
							// Destruct
							var err = errors.a;
							// TailCall
							var $temp$error = /* VarLocal */err,
								$temp$context = /* VarLocal */context;
							error = $temp$error;
							context = $temp$context;
							continue errorToStringHelp;
						} else {
							// Decider Leaf Inline
							// Let
							var starter = function () {
								// Case
								// Decider Chain
								if (!context.b) {
									// Decider Leaf Inline
									return /* Str */'Json.Decode.oneOf';
								} else {
									// Decider Leaf Inline
									return /* Call */_Utils_ap(
										/* Str */'The Json.Decode.oneOf at json',
										/* Call */A2(
											$elm$core$String$join,
											/* Str */'',
											/* Call */$elm$core$List$reverse(/* VarLocal */context)));
								}
							}();
							// Let
							var introduction = /* Call */_Utils_ap(
								/* VarLocal */starter,
								_Utils_ap(
									/* Str */' failed in the following ',
									_Utils_ap(
										/* Call */$elm$core$String$fromInt(
											/* Call */$elm$core$List$length(/* VarLocal */errors)),
										/* Str */' ways:')));
							return /* Call */A2(
								$elm$core$String$join,
								/* Str */'\n\n',
								/* Call */A2(
									$elm$core$List$cons,
									/* VarLocal */introduction,
									/* Call */A2($elm$core$List$indexedMap, /* VarGlobal */$elm$json$Json$Decode$errorOneOf, /* VarLocal */errors)));
						}
					}
				default:
					// Decider Leaf Inline
					// Destruct
					var msg = error.a;
					// Destruct
					var json = error.b;
					// Let
					var introduction = function () {
						// Case
						// Decider Chain
						if (!context.b) {
							// Decider Leaf Inline
							return /* Str */'Problem with the given value:\n\n';
						} else {
							// Decider Leaf Inline
							return /* Call */_Utils_ap(
								/* Str */'Problem with the value at json',
								_Utils_ap(
									/* Call */A2(
										$elm$core$String$join,
										/* Str */'',
										/* Call */$elm$core$List$reverse(/* VarLocal */context)),
									/* Str */':\n\n    '));
						}
					}();
					return /* Call */_Utils_ap(
						/* VarLocal */introduction,
						_Utils_ap(
							/* Call *//* VarGlobal */$elm$json$Json$Decode$indent(
								/* Call */A2(/* VarGlobal */$elm$json$Json$Encode$encode, /* Int */4, /* VarLocal */json)),
							_Utils_ap(/* Str */'\n\n', /* VarLocal */msg)));
			}
		}
	});
// end Cycle errorOneOf, errorToString, errorToStringHelp
// Define, deps=
var $elm$core$Array$branchFactor = /* Int */32;
// Ctor
var $elm$core$Array$Array_elm_builtin = F4(
	function (a, b, c, d) {
		return {$: 'Array_elm_builtin', a: a, b: b, c: c, d: d};
	});
// Define, deps=$elm$kernel$JsArray$$
var $elm$core$Elm$JsArray$empty = /* VarKernel */_JsArray_empty;
// Define, deps=$elm$kernel$Basics$$
var $elm$core$Basics$ceiling = /* VarKernel */_Basics_ceiling;
// Define, deps=$elm$kernel$Basics$$
var $elm$core$Basics$fdiv = /* VarKernel */_Basics_fdiv;
// Define, deps=$elm$kernel$Basics$$, $elm$core$Basics$fdiv
var $elm$core$Basics$logBase = /* Function */F2(
	function (base, number) {
		return /* Call *//* Call *//* VarKernel */_Basics_log(/* VarLocal */number) / /* Call *//* VarKernel */_Basics_log(/* VarLocal */base);
	});
// Define, deps=$elm$kernel$Basics$$
var $elm$core$Basics$toFloat = /* VarKernel */_Basics_toFloat;
// Define, deps=$elm$core$Array$branchFactor, $elm$core$Basics$ceiling, $elm$core$Basics$logBase, $elm$core$Basics$toFloat
var $elm$core$Array$shiftStep = /* Call */$elm$core$Basics$ceiling(
	/* Call */A2($elm$core$Basics$logBase, /* Int */2, /* Call *//* VarGlobal */$elm$core$Array$branchFactor));
// Define, deps=$elm$core$Array$Array_elm_builtin, $elm$core$Elm$JsArray$empty, $elm$core$Array$shiftStep
var $elm$core$Array$empty = /* Call */A4($elm$core$Array$Array_elm_builtin, /* Int */0, /* VarGlobal */$elm$core$Array$shiftStep, /* VarGlobal */$elm$core$Elm$JsArray$empty, /* VarGlobal */$elm$core$Elm$JsArray$empty);
// Define, deps=$elm$kernel$JsArray$$
var $elm$core$Elm$JsArray$initialize = /* VarKernel */_JsArray_initialize;
// Link $elm$core$Array$_M$initializeHelp
// Ctor
var $elm$core$Array$Leaf = function (a) {
	return {$: 'Leaf', a: a};
};
// Define, deps=
var $elm$core$Basics$apR = /* Function */F2(
	function (x, f) {
		return /* Call *//* VarLocal */f(/* VarLocal */x);
	});
// Define, deps=$elm$kernel$Utils$$
var $elm$core$Basics$eq = /* VarKernel */_Utils_equal;
// Define, deps=$elm$kernel$Basics$$
var $elm$core$Basics$floor = /* VarKernel */_Basics_floor;
// Define, deps=$elm$kernel$JsArray$$
var $elm$core$Elm$JsArray$length = /* VarKernel */_JsArray_length;
// Define, deps=$elm$kernel$Utils$$
var $elm$core$Basics$gt = /* VarKernel */_Utils_gt;
// Define, deps=$elm$core$Basics$gt
var $elm$core$Basics$max = /* Function */F2(
	function (x, y) {
		return /* If *//* Call */(_Utils_cmp(/* VarLocal */x, /* VarLocal */y) > 0) ? /* VarLocal */x : /* VarLocal */y;
	});
// Define, deps=$elm$kernel$Basics$$
var $elm$core$Basics$mul = /* VarKernel */_Basics_mul;
// Link $elm$core$Array$_M$treeFromBuilder
// Link $elm$core$Array$_M$compressNodes
// Ctor
var $elm$core$Array$SubTree = function (a) {
	return {$: 'SubTree', a: a};
};
// Define, deps=$elm$kernel$JsArray$$
var $elm$core$Elm$JsArray$initializeFromList = /* VarKernel */_JsArray_initializeFromList;
// Cycle compressNodes
var $elm$core$Array$compressNodes = F2(
	function (nodes, acc) {
		compressNodes:
		while (true) {
			// Let
			var _v0 = /* Call */A2($elm$core$Elm$JsArray$initializeFromList, /* VarGlobal */$elm$core$Array$branchFactor, /* VarLocal */nodes);
			// Destruct
			var node = _v0.a;
			// Destruct
			var remainingNodes = _v0.b;
			// Let
			var newAcc = /* Call */A2(
				$elm$core$List$cons,
				/* Call */$elm$core$Array$SubTree(/* VarLocal */node),
				/* VarLocal */acc);
			// Case
			// Decider Chain
			if (!remainingNodes.b) {
				// Decider Leaf Inline
				return /* Call */$elm$core$List$reverse(/* VarLocal */newAcc);
			} else {
				// Decider Leaf Inline
				// TailCall
				var $temp$nodes = /* VarLocal */remainingNodes,
					$temp$acc = /* VarLocal */newAcc;
				nodes = $temp$nodes;
				acc = $temp$acc;
				continue compressNodes;
			}
		}
	});
// end Cycle compressNodes
// Define, deps=
var $elm$core$Tuple$first = /* Function */function (_v0) {
	// Destruct
	var x = _v0.a;
	return /* VarLocal */x;
};
// Cycle treeFromBuilder
var $elm$core$Array$treeFromBuilder = F2(
	function (nodeList, nodeListSize) {
		treeFromBuilder:
		while (true) {
			// Let
			var newNodeSize = /* Call *//* Call */$elm$core$Basics$ceiling(/* Call *//* Call *//* VarLocal */nodeListSize / /* Call *//* VarGlobal */$elm$core$Array$branchFactor);
			// If
			if (/* Call */_Utils_eq(/* VarLocal */newNodeSize, /* Int */1)) {
				return /* Call *//* Call *//* Call */A2($elm$core$Elm$JsArray$initializeFromList, /* VarGlobal */$elm$core$Array$branchFactor, /* VarLocal */nodeList).a;
			} else {
				// TailCall
				var $temp$nodeList = /* Call */A2($elm$core$Array$compressNodes, /* VarLocal */nodeList, /* List */_List_Nil),
					$temp$nodeListSize = /* VarLocal */newNodeSize;
				nodeList = $temp$nodeList;
				nodeListSize = $temp$nodeListSize;
				continue treeFromBuilder;
			}
		}
	});
// end Cycle treeFromBuilder
// Define, deps=$elm$core$Array$Array_elm_builtin, $elm$core$Basics$add, $elm$core$Basics$apL, $elm$core$Basics$apR, $elm$core$Array$branchFactor, $elm$core$Elm$JsArray$empty, $elm$core$Basics$eq, $elm$core$Basics$floor, $elm$core$Elm$JsArray$length, $elm$core$Basics$logBase, $elm$core$Basics$max, $elm$core$Basics$mul, $elm$core$List$reverse, $elm$core$Array$shiftStep, $elm$core$Basics$sub, $elm$core$Basics$toFloat, $elm$core$Array$treeFromBuilder
var $elm$core$Array$builderToArray = /* Function */F2(
	function (reverseNodeList, builder) {
		// If
		if (/* Call */_Utils_eq(/* Access *//* VarLocal */builder.nodeListSize, /* Int */0)) {
			return /* Call */A4(
				$elm$core$Array$Array_elm_builtin,
				/* Call */$elm$core$Elm$JsArray$length(/* Access *//* VarLocal */builder.tail),
				/* VarGlobal */$elm$core$Array$shiftStep,
				/* VarGlobal */$elm$core$Elm$JsArray$empty,
				/* Access *//* VarLocal */builder.tail);
		} else {
			// Let
			var treeLen = /* Call *//* Access *//* VarLocal */builder.nodeListSize * /* VarGlobal */$elm$core$Array$branchFactor;
			// Let
			var depth = /* Call *//* Call */$elm$core$Basics$floor(
				/* Call *//* Call */A2($elm$core$Basics$logBase, /* Call *//* VarGlobal */$elm$core$Array$branchFactor, /* Call *//* Call *//* Call *//* VarLocal */treeLen - /* Int */1));
			// Let
			var correctNodeList = /* If *//* VarLocal */reverseNodeList ? /* Call */$elm$core$List$reverse(/* Access *//* VarLocal */builder.nodeList) : /* Access *//* VarLocal */builder.nodeList;
			// Let
			var tree = /* Call */A2($elm$core$Array$treeFromBuilder, /* VarLocal */correctNodeList, /* Access *//* VarLocal */builder.nodeListSize);
			return /* Call */A4(
				$elm$core$Array$Array_elm_builtin,
				/* Call *//* Call */$elm$core$Elm$JsArray$length(/* Access *//* VarLocal */builder.tail) + /* VarLocal */treeLen,
				/* Call *//* Call */A2($elm$core$Basics$max, /* Int */5, /* Call *//* VarLocal */depth * /* VarGlobal */$elm$core$Array$shiftStep),
				/* VarLocal */tree,
				/* Access *//* VarLocal */builder.tail);
		}
	});
// Define, deps=$elm$kernel$Basics$$
var $elm$core$Basics$idiv = /* VarKernel */_Basics_idiv;
// Define, deps=$elm$kernel$Utils$$
var $elm$core$Basics$lt = /* VarKernel */_Utils_lt;
// Cycle initializeHelp
var $elm$core$Array$initializeHelp = F5(
	function (fn, fromIndex, len, nodeList, tail) {
		initializeHelp:
		while (true) {
			// If
			if (/* Call */_Utils_cmp(/* VarLocal */fromIndex, /* Int */0) < 0) {
				return /* Call */A2(
					$elm$core$Array$builderToArray,
					/* Bool */false,
					/* Record */{nodeList: /* VarLocal */nodeList, nodeListSize: /* Call */(/* VarLocal */len / /* VarGlobal */$elm$core$Array$branchFactor) | 0, tail: /* VarLocal */tail});
			} else {
				// Let
				var leaf = /* Call *//* Call */$elm$core$Array$Leaf(
					/* Call */A3($elm$core$Elm$JsArray$initialize, /* VarGlobal */$elm$core$Array$branchFactor, /* VarLocal */fromIndex, /* VarLocal */fn));
				// TailCall
				var $temp$fn = /* VarLocal */fn,
					$temp$fromIndex = /* Call *//* VarLocal */fromIndex - /* VarGlobal */$elm$core$Array$branchFactor,
					$temp$len = /* VarLocal */len,
					$temp$nodeList = /* Call */A2($elm$core$List$cons, /* VarLocal */leaf, /* VarLocal */nodeList),
					$temp$tail = /* VarLocal */tail;
				fn = $temp$fn;
				fromIndex = $temp$fromIndex;
				len = $temp$len;
				nodeList = $temp$nodeList;
				tail = $temp$tail;
				continue initializeHelp;
			}
		}
	});
// end Cycle initializeHelp
// Define, deps=$elm$kernel$Basics$$
var $elm$core$Basics$remainderBy = /* VarKernel */_Basics_remainderBy;
// Define, deps=$elm$kernel$List$$, $elm$core$Array$branchFactor, $elm$core$Array$empty, $elm$core$Elm$JsArray$initialize, $elm$core$Array$initializeHelp, $elm$core$Basics$le, $elm$core$Basics$remainderBy, $elm$core$Basics$sub
var $elm$core$Array$initialize = /* Function */F2(
	function (len, fn) {
		// If
		if (/* Call */_Utils_cmp(/* VarLocal */len, /* Int */0) < 1) {
			return /* VarGlobal */$elm$core$Array$empty;
		} else {
			// Let
			var tailLen = /* Call *//* VarLocal */len % /* VarGlobal */$elm$core$Array$branchFactor;
			// Let
			var tail = /* Call */A3($elm$core$Elm$JsArray$initialize, /* VarLocal */tailLen, /* Call *//* VarLocal */len - /* VarLocal */tailLen, /* VarLocal */fn);
			// Let
			var initialFromIndex = /* Call *//* Call */(/* VarLocal */len - /* VarLocal */tailLen) - /* VarGlobal */$elm$core$Array$branchFactor;
			return /* Call */A5($elm$core$Array$initializeHelp, /* VarLocal */fn, /* VarLocal */initialFromIndex, /* VarLocal */len, /* List */_List_Nil, /* VarLocal */tail);
		}
	});
// Enum
var $elm$core$Basics$True = {$: 'True'};
// Define, deps=$elm$core$Basics$False, $elm$core$Basics$True
var $elm$core$Result$isOk = /* Function */function (result) {
	// Case
	// Decider Chain
	if (result.$ === 'Ok') {
		// Decider Leaf Inline
		return /* Bool */true;
	} else {
		// Decider Leaf Inline
		return /* Bool */false;
	}
};
// Define, deps=$elm$kernel$Json$$
var $elm$json$Json$Decode$map = /* VarKernel */_Json_map1;
// Define, deps=$elm$kernel$Json$$
var $elm$json$Json$Decode$map2 = /* VarKernel */_Json_map2;
// Define, deps=$elm$kernel$Json$$
var $elm$json$Json$Decode$succeed = /* VarKernel */_Json_succeed;
// Define, deps=
var $elm$virtual_dom$VirtualDom$toHandlerInt = /* Function */function (handler) {
	// Case
	// Decider Fanout
	switch (handler.$) {
		case 'Normal':
			// Decider Leaf Inline
			return /* Int */0;
		case 'MayStopPropagation':
			// Decider Leaf Inline
			return /* Int */1;
		case 'MayPreventDefault':
			// Decider Leaf Inline
			return /* Int */2;
		default:
			// Decider Leaf Inline
			return /* Int */3;
	}
};
// Ctor
var $elm$browser$Browser$External = function (a) {
	return {$: 'External', a: a};
};
// Ctor
var $elm$browser$Browser$Internal = function (a) {
	return {$: 'Internal', a: a};
};
// Define, deps=
var $elm$core$Basics$identity = /* Function */function (x) {
	return /* VarLocal */x;
};
// Box
var $elm$browser$Browser$Dom$NotFound = function (a) {
	return {$: 'NotFound', a: a};
};
// Enum
var $elm$url$Url$Http = {$: 'Http'};
// Enum
var $elm$url$Url$Https = {$: 'Https'};
// Define, deps=
var $elm$url$Url$Url = /* Function */F6(
	function (protocol, host, port_, path, query, fragment) {
		return /* Record */{fragment: /* VarLocal */fragment, host: /* VarLocal */host, path: /* VarLocal */path, port_: /* VarLocal */port_, protocol: /* VarLocal */protocol, query: /* VarLocal */query};
	});
// Define, deps=$elm$kernel$String$$
var $elm$core$String$contains = /* VarKernel */_String_contains;
// Define, deps=$elm$kernel$String$$
var $elm$core$String$length = /* VarKernel */_String_length;
// Define, deps=$elm$kernel$String$$
var $elm$core$String$slice = /* VarKernel */_String_slice;
// Define, deps=$elm$core$String$length, $elm$core$Basics$lt, $elm$core$String$slice
var $elm$core$String$dropLeft = /* Function */F2(
	function (n, string) {
		return /* If *//* Call */(_Utils_cmp(/* VarLocal */n, /* Int */1) < 0) ? /* VarLocal */string : /* Call */A3(
			$elm$core$String$slice,
			/* VarLocal */n,
			/* Call */$elm$core$String$length(/* VarLocal */string),
			/* VarLocal */string);
	});
// Define, deps=$elm$kernel$String$$
var $elm$core$String$indexes = /* VarKernel */_String_indexes;
// Define, deps=$elm$core$Basics$eq
var $elm$core$String$isEmpty = /* Function */function (string) {
	return /* Call */_Utils_eq(/* VarLocal */string, /* Str */'');
};
// Define, deps=$elm$core$Basics$lt, $elm$core$String$slice
var $elm$core$String$left = /* Function */F2(
	function (n, string) {
		return /* If *//* Call */(_Utils_cmp(/* VarLocal */n, /* Int */1) < 0) ? /* Str */'' : /* Call */A3($elm$core$String$slice, /* Int */0, /* VarLocal */n, /* VarLocal */string);
	});
// Define, deps=$elm$kernel$String$$
var $elm$core$String$toInt = /* VarKernel */_String_toInt;
// Define, deps=$elm$core$Maybe$Just, $elm$core$Maybe$Nothing, $elm$url$Url$Url, $elm$core$Basics$add, $elm$core$Basics$apL, $elm$core$String$contains, $elm$core$String$dropLeft, $elm$core$String$indexes, $elm$core$String$isEmpty, $elm$core$String$left, $elm$core$Basics$or, $elm$core$String$toInt
var $elm$url$Url$chompBeforePath = /* Function */F5(
	function (protocol, path, params, frag, str) {
		// If
		if (/* Call *//* Call */$elm$core$String$isEmpty(/* VarLocal */str) || /* Call */A2($elm$core$String$contains, /* Str */'@', /* VarLocal */str)) {
			return /* VarGlobal */$elm$core$Maybe$Nothing;
		} else {
			// Let
			var _v0 = /* Call */A2($elm$core$String$indexes, /* Str */':', /* VarLocal */str);
			// Case
			// Decider Chain
			if (!_v0.b) {
				// Decider Leaf Inline
				return /* Call *//* Call */$elm$core$Maybe$Just(
					/* Call */A6(/* VarGlobal */$elm$url$Url$Url, /* VarLocal */protocol, /* VarLocal */str, /* VarGlobal */$elm$core$Maybe$Nothing, /* VarLocal */path, /* VarLocal */params, /* VarLocal */frag));
			} else {
				// Decider Chain
				if (!_v0.b.b) {
					// Decider Leaf Inline
					// Destruct
					var i = _v0.a;
					// Let
					var _v1 = /* Call */$elm$core$String$toInt(
						/* Call */A2($elm$core$String$dropLeft, /* Call *//* VarLocal */i + /* Int */1, /* VarLocal */str));
					// Case
					// Decider Chain
					if (_v1.$ === 'Nothing') {
						// Decider Leaf Inline
						return /* VarGlobal */$elm$core$Maybe$Nothing;
					} else {
						// Decider Leaf Inline
						// Destruct
						var port_ = _v1;
						return /* Call *//* Call */$elm$core$Maybe$Just(
							/* Call */A6(
								/* VarGlobal */$elm$url$Url$Url,
								/* VarLocal */protocol,
								/* Call */A2($elm$core$String$left, /* VarLocal */i, /* VarLocal */str),
								/* VarLocal */port_,
								/* VarLocal */path,
								/* VarLocal */params,
								/* VarLocal */frag));
					}
				} else {
					// Decider Leaf Inline
					return /* VarGlobal */$elm$core$Maybe$Nothing;
				}
			}
		}
	});
// Define, deps=$elm$core$Maybe$Nothing, $elm$url$Url$chompBeforePath, $elm$core$String$dropLeft, $elm$core$String$indexes, $elm$core$String$isEmpty, $elm$core$String$left
var $elm$url$Url$chompBeforeQuery = /* Function */F4(
	function (protocol, params, frag, str) {
		// If
		if (/* Call */$elm$core$String$isEmpty(/* VarLocal */str)) {
			return /* VarGlobal */$elm$core$Maybe$Nothing;
		} else {
			// Let
			var _v0 = /* Call */A2($elm$core$String$indexes, /* Str */'/', /* VarLocal */str);
			// Case
			// Decider Chain
			if (!_v0.b) {
				// Decider Leaf Inline
				return /* Call */A5(/* VarGlobal */$elm$url$Url$chompBeforePath, /* VarLocal */protocol, /* Str */'/', /* VarLocal */params, /* VarLocal */frag, /* VarLocal */str);
			} else {
				// Decider Leaf Inline
				// Destruct
				var i = _v0.a;
				return /* Call */A5(
					/* VarGlobal */$elm$url$Url$chompBeforePath,
					/* VarLocal */protocol,
					/* Call */A2($elm$core$String$dropLeft, /* VarLocal */i, /* VarLocal */str),
					/* VarLocal */params,
					/* VarLocal */frag,
					/* Call */A2($elm$core$String$left, /* VarLocal */i, /* VarLocal */str));
			}
		}
	});
// Define, deps=$elm$core$Maybe$Just, $elm$core$Maybe$Nothing, $elm$core$Basics$add, $elm$url$Url$chompBeforeQuery, $elm$core$String$dropLeft, $elm$core$String$indexes, $elm$core$String$isEmpty, $elm$core$String$left
var $elm$url$Url$chompBeforeFragment = /* Function */F3(
	function (protocol, frag, str) {
		// If
		if (/* Call */$elm$core$String$isEmpty(/* VarLocal */str)) {
			return /* VarGlobal */$elm$core$Maybe$Nothing;
		} else {
			// Let
			var _v0 = /* Call */A2($elm$core$String$indexes, /* Str */'?', /* VarLocal */str);
			// Case
			// Decider Chain
			if (!_v0.b) {
				// Decider Leaf Inline
				return /* Call */A4(/* VarGlobal */$elm$url$Url$chompBeforeQuery, /* VarLocal */protocol, /* VarGlobal */$elm$core$Maybe$Nothing, /* VarLocal */frag, /* VarLocal */str);
			} else {
				// Decider Leaf Inline
				// Destruct
				var i = _v0.a;
				return /* Call */A4(
					/* VarGlobal */$elm$url$Url$chompBeforeQuery,
					/* VarLocal */protocol,
					/* Call */$elm$core$Maybe$Just(
						/* Call */A2($elm$core$String$dropLeft, /* Call *//* VarLocal */i + /* Int */1, /* VarLocal */str)),
					/* VarLocal */frag,
					/* Call */A2($elm$core$String$left, /* VarLocal */i, /* VarLocal */str));
			}
		}
	});
// Define, deps=$elm$core$Maybe$Just, $elm$core$Maybe$Nothing, $elm$core$Basics$add, $elm$url$Url$chompBeforeFragment, $elm$core$String$dropLeft, $elm$core$String$indexes, $elm$core$String$isEmpty, $elm$core$String$left
var $elm$url$Url$chompAfterProtocol = /* Function */F2(
	function (protocol, str) {
		// If
		if (/* Call */$elm$core$String$isEmpty(/* VarLocal */str)) {
			return /* VarGlobal */$elm$core$Maybe$Nothing;
		} else {
			// Let
			var _v0 = /* Call */A2($elm$core$String$indexes, /* Str */'#', /* VarLocal */str);
			// Case
			// Decider Chain
			if (!_v0.b) {
				// Decider Leaf Inline
				return /* Call */A3(/* VarGlobal */$elm$url$Url$chompBeforeFragment, /* VarLocal */protocol, /* VarGlobal */$elm$core$Maybe$Nothing, /* VarLocal */str);
			} else {
				// Decider Leaf Inline
				// Destruct
				var i = _v0.a;
				return /* Call */A3(
					/* VarGlobal */$elm$url$Url$chompBeforeFragment,
					/* VarLocal */protocol,
					/* Call */$elm$core$Maybe$Just(
						/* Call */A2($elm$core$String$dropLeft, /* Call *//* VarLocal */i + /* Int */1, /* VarLocal */str)),
					/* Call */A2($elm$core$String$left, /* VarLocal */i, /* VarLocal */str));
			}
		}
	});
// Define, deps=$elm$kernel$String$$
var $elm$core$String$startsWith = /* VarKernel */_String_startsWith;
// Define, deps=$elm$url$Url$Http, $elm$url$Url$Https, $elm$core$Maybe$Nothing, $elm$url$Url$chompAfterProtocol, $elm$core$String$dropLeft, $elm$core$String$startsWith
var $elm$url$Url$fromString = /* Function */function (str) {
	return /* If *//* Call */A2($elm$core$String$startsWith, /* Str */'http://', /* VarLocal */str) ? /* Call */A2(
		/* VarGlobal */$elm$url$Url$chompAfterProtocol,
		/* VarEnum */$elm$url$Url$Http,
		/* Call */A2($elm$core$String$dropLeft, /* Int */7, /* VarLocal */str)) : (/* Call */A2($elm$core$String$startsWith, /* Str */'https://', /* VarLocal */str) ? /* Call */A2(
		/* VarGlobal */$elm$url$Url$chompAfterProtocol,
		/* VarEnum */$elm$url$Url$Https,
		/* Call */A2($elm$core$String$dropLeft, /* Int */8, /* VarLocal */str)) : /* VarGlobal */$elm$core$Maybe$Nothing);
};
// Link $elm$core$Basics$_M$never
// Cycle never
var $elm$core$Basics$never = function (_v0) {
	never:
	while (true) {
		// Destruct
		var nvr = _v0.a;
		// TailCall
		var $temp$_v0 = /* VarLocal */nvr;
		_v0 = $temp$_v0;
		continue never;
	}
};
// end Cycle never
// Box
var $elm$core$Task$Perform = function (a) {
	return {$: 'Perform', a: a};
};
// Link $elm$core$Task$$fx$
// Manager $elm$core$Task$$fx$
// Define, deps=$elm$kernel$Scheduler$$
var $elm$core$Task$succeed = /* VarKernel */_Scheduler_succeed;
// Define, deps=$elm$kernel$Utils$$, $elm$core$Task$succeed
var $elm$core$Task$init = /* Call */$elm$core$Task$succeed(/* Unit */_Utils_Tuple0);
// Link $elm$core$List$_M$foldrHelper
// Cycle foldrHelper
var $elm$core$List$foldrHelper = /* Function */F4(
	function (fn, acc, ctr, ls) {
		// Case
		// Decider Chain
		if (!ls.b) {
			// Decider Leaf Inline
			return /* VarLocal */acc;
		} else {
			// Decider Leaf Inline
			// Destruct
			var a = ls.a;
			// Destruct
			var r1 = ls.b;
			// Case
			// Decider Chain
			if (!r1.b) {
				// Decider Leaf Inline
				return /* Call */A2(/* VarLocal */fn, /* VarLocal */a, /* VarLocal */acc);
			} else {
				// Decider Leaf Inline
				// Destruct
				var b = r1.a;
				// Destruct
				var r2 = r1.b;
				// Case
				// Decider Chain
				if (!r2.b) {
					// Decider Leaf Inline
					return /* Call */A2(
						/* VarLocal */fn,
						/* VarLocal */a,
						/* Call */A2(/* VarLocal */fn, /* VarLocal */b, /* VarLocal */acc));
				} else {
					// Decider Leaf Inline
					// Destruct
					var c = r2.a;
					// Destruct
					var r3 = r2.b;
					// Case
					// Decider Chain
					if (!r3.b) {
						// Decider Leaf Inline
						return /* Call */A2(
							/* VarLocal */fn,
							/* VarLocal */a,
							/* Call */A2(
								/* VarLocal */fn,
								/* VarLocal */b,
								/* Call */A2(/* VarLocal */fn, /* VarLocal */c, /* VarLocal */acc)));
					} else {
						// Decider Leaf Inline
						// Destruct
						var d = r3.a;
						// Destruct
						var r4 = r3.b;
						// Let
						var res = /* If *//* Call */(_Utils_cmp(/* VarLocal */ctr, /* Int */500) > 0) ? /* Call */A3(
							$elm$core$List$foldl,
							/* VarLocal */fn,
							/* VarLocal */acc,
							/* Call */$elm$core$List$reverse(/* VarLocal */r4)) : /* Call */A4($elm$core$List$foldrHelper, /* VarLocal */fn, /* VarLocal */acc, /* Call *//* VarLocal */ctr + /* Int */1, /* VarLocal */r4);
						return /* Call */A2(
							/* VarLocal */fn,
							/* VarLocal */a,
							/* Call */A2(
								/* VarLocal */fn,
								/* VarLocal */b,
								/* Call */A2(
									/* VarLocal */fn,
									/* VarLocal */c,
									/* Call */A2(/* VarLocal */fn, /* VarLocal */d, /* VarLocal */res))));
					}
				}
			}
		}
	});
// end Cycle foldrHelper
// Define, deps=$elm$core$List$foldrHelper
var $elm$core$List$foldr = /* Function */F3(
	function (fn, acc, ls) {
		return /* Call */A4($elm$core$List$foldrHelper, /* VarLocal */fn, /* VarLocal */acc, /* Int */0, /* VarLocal */ls);
	});
// Define, deps=$elm$kernel$List$$, $elm$core$List$cons, $elm$core$List$foldr
var $elm$core$List$map = /* Function */F2(
	function (f, xs) {
		return /* Call */A3(
			$elm$core$List$foldr,
			/* Function */F2(
				function (x, acc) {
					return /* Call */A2(
						$elm$core$List$cons,
						/* Call *//* VarLocal */f(/* VarLocal */x),
						/* VarLocal */acc);
				}),
			/* List */_List_Nil,
			/* VarLocal */xs);
	});
// Define, deps=$elm$kernel$Scheduler$$
var $elm$core$Task$andThen = /* VarKernel */_Scheduler_andThen;
// Define, deps=$elm$core$Task$andThen, $elm$core$Basics$apR, $elm$core$Task$succeed
var $elm$core$Task$map = /* Function */F2(
	function (func, taskA) {
		return /* Call *//* Call */A2(
			$elm$core$Task$andThen,
			/* Function */function (a) {
				return /* Call */$elm$core$Task$succeed(
					/* Call *//* VarLocal */func(/* VarLocal */a));
			},
			/* VarLocal */taskA);
	});
// Define, deps=$elm$core$Task$andThen, $elm$core$Basics$apR, $elm$core$Task$succeed
var $elm$core$Task$map2 = /* Function */F3(
	function (func, taskA, taskB) {
		return /* Call *//* Call */A2(
			$elm$core$Task$andThen,
			/* Function */function (a) {
				return /* Call *//* Call */A2(
					$elm$core$Task$andThen,
					/* Function */function (b) {
						return /* Call */$elm$core$Task$succeed(
							/* Call */A2(/* VarLocal */func, /* VarLocal */a, /* VarLocal */b));
					},
					/* VarLocal */taskB);
			},
			/* VarLocal */taskA);
	});
// Define, deps=$elm$kernel$List$$, $elm$core$List$cons, $elm$core$List$foldr, $elm$core$Task$map2, $elm$core$Task$succeed
var $elm$core$Task$sequence = /* Function */function (tasks) {
	return /* Call */A3(
		$elm$core$List$foldr,
		/* Call */$elm$core$Task$map2(/* VarGlobal */$elm$core$List$cons),
		/* Call */$elm$core$Task$succeed(/* List */_List_Nil),
		/* VarLocal */tasks);
};
// Define, deps=$elm$kernel$Platform$$
var $elm$core$Platform$sendToApp = /* VarKernel */_Platform_sendToApp;
// Define, deps=$elm$kernel$Scheduler$$, $elm$core$Task$andThen, $elm$core$Basics$apR, $elm$core$Platform$sendToApp
var $elm$core$Task$spawnCmd = /* Function */F2(
	function (router, _v0) {
		// Destruct
		var task = _v0.a;
		return /* Call *//* VarKernel */_Scheduler_spawn(
			/* Call *//* Call */A2(
				$elm$core$Task$andThen,
				/* Call */$elm$core$Platform$sendToApp(/* VarLocal */router),
				/* VarLocal */task));
	});
// Define, deps=$elm$kernel$Utils$$, $elm$core$List$map, $elm$core$Task$map, $elm$core$Task$sequence, $elm$core$Task$spawnCmd
var $elm$core$Task$onEffects = /* Function */F3(
	function (router, commands, state) {
		return /* Call */A2(
			$elm$core$Task$map,
			/* Function */function (_v0) {
				return /* Unit */_Utils_Tuple0;
			},
			/* Call */$elm$core$Task$sequence(
				/* Call */A2(
					$elm$core$List$map,
					/* Call */$elm$core$Task$spawnCmd(/* VarLocal */router),
					/* VarLocal */commands)));
	});
// Define, deps=$elm$kernel$Utils$$, $elm$core$Task$succeed
var $elm$core$Task$onSelfMsg = /* Function */F3(
	function (_v0, _v1, _v2) {
		return /* Call */$elm$core$Task$succeed(/* Unit */_Utils_Tuple0);
	});
// Define, deps=$elm$core$Task$Perform, $elm$core$Basics$identity, $elm$core$Task$map
var $elm$core$Task$cmdMap = /* Function */F2(
	function (tagger, _v0) {
		// Destruct
		var task = _v0.a;
		return /* Call *//* VarBox */$elm$core$Task$Perform(
			/* Call */A2($elm$core$Task$map, /* VarLocal */tagger, /* VarLocal */task));
	});
_Platform_effectManagers['Task'] = _Platform_createManager($elm$core$Task$init, $elm$core$Task$onEffects, $elm$core$Task$onSelfMsg, $elm$core$Task$cmdMap);
var $elm$core$Task$command = _Platform_leaf('Task');
// End Manager $elm$core$Task$$fx$
// Define, deps=$elm$core$Task$Perform, $elm$core$Task$command, $elm$core$Basics$identity, $elm$core$Task$map
var $elm$core$Task$perform = /* Function */F2(
	function (toMessage, task) {
		return /* Call */$elm$core$Task$command(
			/* Call *//* VarBox */$elm$core$Task$Perform(
				/* Call */A2($elm$core$Task$map, /* VarLocal */toMessage, /* VarLocal */task)));
	});
// Define, deps=$elm$kernel$Browser$$
var $elm$browser$Browser$element = /* VarKernel */_Browser_element;
// Define, deps=$elm$kernel$Json$$
var $elm$json$Json$Decode$field = /* VarKernel */_Json_decodeField;
// Define, deps=
var $author$project$Main$initialModel = /* Record */{text: /* Str */'Initialised'};
// Define, deps=
var $author$project$WebAssembly$intercept = /* Function */function (teaRecord) {
	return /* VarLocal */teaRecord;
};
// Define, deps=$elm$kernel$Platform$$
var $elm$core$Platform$Cmd$batch = /* VarKernel */_Platform_batch;
// Define, deps=$elm$kernel$List$$, $elm$core$Platform$Cmd$batch
var $elm$core$Platform$Cmd$none = /* Call */$elm$core$Platform$Cmd$batch(/* List */_List_Nil);
// Enum
var $elm$browser$Browser$Events$Document = {$: 'Document'};
// Ctor
var $elm$browser$Browser$Events$MySub = F3(
	function (a, b, c) {
		return {$: 'MySub', a: a, b: b, c: c};
	});
// Link $elm$browser$Browser$Events$$fx$
// Manager $elm$browser$Browser$Events$$fx$
// Define, deps=
var $elm$browser$Browser$Events$State = /* Function */F2(
	function (subs, pids) {
		return /* Record */{pids: /* VarLocal */pids, subs: /* VarLocal */subs};
	});
// Ctor
var $elm$core$Dict$RBEmpty_elm_builtin = {$: 'RBEmpty_elm_builtin'};
// Define, deps=$elm$core$Dict$RBEmpty_elm_builtin
var $elm$core$Dict$empty = /* VarGlobal */$elm$core$Dict$RBEmpty_elm_builtin;
// Define, deps=$elm$kernel$List$$, $elm$browser$Browser$Events$State, $elm$core$Dict$empty, $elm$core$Task$succeed
var $elm$browser$Browser$Events$init = /* Call */$elm$core$Task$succeed(
	/* Call */A2(/* VarGlobal */$elm$browser$Browser$Events$State, /* List */_List_Nil, /* VarGlobal */$elm$core$Dict$empty));
// Define, deps=
var $elm$browser$Browser$Events$nodeToKey = /* Function */function (node) {
	// Case
	// Decider Chain
	if (node.$ === 'Document') {
		// Decider Leaf Inline
		return /* Str */'d_';
	} else {
		// Decider Leaf Inline
		return /* Str */'w_';
	}
};
// Define, deps=$elm$kernel$Utils$$, $elm$core$Basics$append, $elm$browser$Browser$Events$nodeToKey
var $elm$browser$Browser$Events$addKey = /* Function */function (sub) {
	// Destruct
	var node = sub.a;
	// Destruct
	var name = sub.b;
	return /* Tuple */_Utils_Tuple2(
		/* Call */_Utils_ap(
			/* Call *//* VarGlobal */$elm$browser$Browser$Events$nodeToKey(/* VarLocal */node),
			/* VarLocal */name),
		/* VarLocal */sub);
};
// Enum
var $elm$core$Dict$Black = {$: 'Black'};
// Ctor
var $elm$core$Dict$RBNode_elm_builtin = F5(
	function (a, b, c, d, e) {
		return {$: 'RBNode_elm_builtin', a: a, b: b, c: c, d: d, e: e};
	});
// Link $elm$core$Dict$_M$insertHelp
// Enum
var $elm$core$Dict$Red = {$: 'Red'};
// Define, deps=$elm$core$Dict$Black, $elm$core$Dict$RBNode_elm_builtin, $elm$core$Dict$Red
var $elm$core$Dict$balance = /* Function */F5(
	function (color, key, value, left, right) {
		// Case
		// Decider Chain
		if ((right.$ === 'RBNode_elm_builtin') && (right.a.$ === 'Red')) {
			// Decider Leaf Inline
			// Destruct
			var _v1 = right.a;
			// Destruct
			var rK = right.b;
			// Destruct
			var rV = right.c;
			// Destruct
			var rLeft = right.d;
			// Destruct
			var rRight = right.e;
			// Case
			// Decider Chain
			if ((left.$ === 'RBNode_elm_builtin') && (left.a.$ === 'Red')) {
				// Decider Leaf Inline
				// Destruct
				var _v3 = left.a;
				// Destruct
				var lK = left.b;
				// Destruct
				var lV = left.c;
				// Destruct
				var lLeft = left.d;
				// Destruct
				var lRight = left.e;
				return /* Call */A5(
					$elm$core$Dict$RBNode_elm_builtin,
					/* VarEnum */$elm$core$Dict$Red,
					/* VarLocal */key,
					/* VarLocal */value,
					/* Call */A5($elm$core$Dict$RBNode_elm_builtin, /* VarEnum */$elm$core$Dict$Black, /* VarLocal */lK, /* VarLocal */lV, /* VarLocal */lLeft, /* VarLocal */lRight),
					/* Call */A5($elm$core$Dict$RBNode_elm_builtin, /* VarEnum */$elm$core$Dict$Black, /* VarLocal */rK, /* VarLocal */rV, /* VarLocal */rLeft, /* VarLocal */rRight));
			} else {
				// Decider Leaf Inline
				return /* Call */A5(
					$elm$core$Dict$RBNode_elm_builtin,
					/* VarLocal */color,
					/* VarLocal */rK,
					/* VarLocal */rV,
					/* Call */A5($elm$core$Dict$RBNode_elm_builtin, /* VarEnum */$elm$core$Dict$Red, /* VarLocal */key, /* VarLocal */value, /* VarLocal */left, /* VarLocal */rLeft),
					/* VarLocal */rRight);
			}
		} else {
			// Decider Leaf Inline
			// Case
			// Decider Chain
			if ((((left.$ === 'RBNode_elm_builtin') && (left.a.$ === 'Red')) && (left.d.$ === 'RBNode_elm_builtin')) && (left.d.a.$ === 'Red')) {
				// Decider Leaf Inline
				// Destruct
				var _v5 = left.a;
				// Destruct
				var lK = left.b;
				// Destruct
				var lV = left.c;
				// Destruct
				var _v6 = left.d;
				// Destruct
				var _v7 = _v6.a;
				// Destruct
				var llK = _v6.b;
				// Destruct
				var llV = _v6.c;
				// Destruct
				var llLeft = _v6.d;
				// Destruct
				var llRight = _v6.e;
				// Destruct
				var lRight = left.e;
				return /* Call */A5(
					$elm$core$Dict$RBNode_elm_builtin,
					/* VarEnum */$elm$core$Dict$Red,
					/* VarLocal */lK,
					/* VarLocal */lV,
					/* Call */A5($elm$core$Dict$RBNode_elm_builtin, /* VarEnum */$elm$core$Dict$Black, /* VarLocal */llK, /* VarLocal */llV, /* VarLocal */llLeft, /* VarLocal */llRight),
					/* Call */A5($elm$core$Dict$RBNode_elm_builtin, /* VarEnum */$elm$core$Dict$Black, /* VarLocal */key, /* VarLocal */value, /* VarLocal */lRight, /* VarLocal */right));
			} else {
				// Decider Leaf Inline
				return /* Call */A5($elm$core$Dict$RBNode_elm_builtin, /* VarLocal */color, /* VarLocal */key, /* VarLocal */value, /* VarLocal */left, /* VarLocal */right);
			}
		}
	});
// Define, deps=$elm$kernel$Utils$$
var $elm$core$Basics$compare = /* VarKernel */_Utils_compare;
// Cycle insertHelp
var $elm$core$Dict$insertHelp = /* Function */F3(
	function (key, value, dict) {
		// Case
		// Decider Chain
		if (dict.$ === 'RBEmpty_elm_builtin') {
			// Decider Leaf Inline
			return /* Call */A5($elm$core$Dict$RBNode_elm_builtin, /* VarEnum */$elm$core$Dict$Red, /* VarLocal */key, /* VarLocal */value, /* VarGlobal */$elm$core$Dict$RBEmpty_elm_builtin, /* VarGlobal */$elm$core$Dict$RBEmpty_elm_builtin);
		} else {
			// Decider Leaf Inline
			// Destruct
			var nColor = dict.a;
			// Destruct
			var nKey = dict.b;
			// Destruct
			var nValue = dict.c;
			// Destruct
			var nLeft = dict.d;
			// Destruct
			var nRight = dict.e;
			// Let
			var _v1 = /* Call */A2($elm$core$Basics$compare, /* VarLocal */key, /* VarLocal */nKey);
			// Case
			// Decider Fanout
			switch (_v1.$) {
				case 'LT':
					// Decider Leaf Inline
					return /* Call */A5(
						$elm$core$Dict$balance,
						/* VarLocal */nColor,
						/* VarLocal */nKey,
						/* VarLocal */nValue,
						/* Call */A3($elm$core$Dict$insertHelp, /* VarLocal */key, /* VarLocal */value, /* VarLocal */nLeft),
						/* VarLocal */nRight);
				case 'EQ':
					// Decider Leaf Inline
					return /* Call */A5($elm$core$Dict$RBNode_elm_builtin, /* VarLocal */nColor, /* VarLocal */nKey, /* VarLocal */value, /* VarLocal */nLeft, /* VarLocal */nRight);
				default:
					// Decider Leaf Inline
					return /* Call */A5(
						$elm$core$Dict$balance,
						/* VarLocal */nColor,
						/* VarLocal */nKey,
						/* VarLocal */nValue,
						/* VarLocal */nLeft,
						/* Call */A3($elm$core$Dict$insertHelp, /* VarLocal */key, /* VarLocal */value, /* VarLocal */nRight));
			}
		}
	});
// end Cycle insertHelp
// Define, deps=$elm$core$Dict$Black, $elm$core$Dict$RBNode_elm_builtin, $elm$core$Dict$insertHelp
var $elm$core$Dict$insert = /* Function */F3(
	function (key, value, dict) {
		// Let
		var _v0 = /* Call */A3($elm$core$Dict$insertHelp, /* VarLocal */key, /* VarLocal */value, /* VarLocal */dict);
		// Case
		// Decider Chain
		if ((_v0.$ === 'RBNode_elm_builtin') && (_v0.a.$ === 'Red')) {
			// Decider Leaf Inline
			// Destruct
			var _v1 = _v0.a;
			// Destruct
			var k = _v0.b;
			// Destruct
			var v = _v0.c;
			// Destruct
			var l = _v0.d;
			// Destruct
			var r = _v0.e;
			return /* Call */A5($elm$core$Dict$RBNode_elm_builtin, /* VarEnum */$elm$core$Dict$Black, /* VarLocal */k, /* VarLocal */v, /* VarLocal */l, /* VarLocal */r);
		} else {
			// Decider Leaf Inline
			// Destruct
			var x = _v0;
			return /* VarLocal */x;
		}
	});
// Define, deps=$elm$core$Dict$empty, $elm$core$List$foldl, $elm$core$Dict$insert
var $elm$core$Dict$fromList = /* Function */function (assocs) {
	return /* Call */A3(
		$elm$core$List$foldl,
		/* Function */F2(
			function (_v0, dict) {
				// Destruct
				var key = _v0.a;
				// Destruct
				var value = _v0.b;
				return /* Call */A3($elm$core$Dict$insert, /* VarLocal */key, /* VarLocal */value, /* VarLocal */dict);
			}),
		/* VarGlobal */$elm$core$Dict$empty,
		/* VarLocal */assocs);
};
// Define, deps=$elm$kernel$Scheduler$$
var $elm$core$Process$kill = /* VarKernel */_Scheduler_kill;
// Link $elm$core$Dict$_M$foldl
// Cycle foldl
var $elm$core$Dict$foldl = F3(
	function (func, acc, dict) {
		foldl:
		while (true) {
			// Case
			// Decider Chain
			if (dict.$ === 'RBEmpty_elm_builtin') {
				// Decider Leaf Inline
				return /* VarLocal */acc;
			} else {
				// Decider Leaf Inline
				// Destruct
				var key = dict.b;
				// Destruct
				var value = dict.c;
				// Destruct
				var left = dict.d;
				// Destruct
				var right = dict.e;
				// TailCall
				var $temp$func = /* VarLocal */func,
					$temp$acc = /* Call */A3(
					/* VarLocal */func,
					/* VarLocal */key,
					/* VarLocal */value,
					/* Call */A3($elm$core$Dict$foldl, /* VarLocal */func, /* VarLocal */acc, /* VarLocal */left)),
					$temp$dict = /* VarLocal */right;
				func = $temp$func;
				acc = $temp$acc;
				dict = $temp$dict;
				continue foldl;
			}
		}
	});
// end Cycle foldl
// Define, deps=$elm$kernel$Utils$$, $elm$core$Dict$foldl, $elm$core$List$foldl, $elm$core$Basics$gt, $elm$core$Basics$lt, $elm$core$Dict$toList
var $elm$core$Dict$merge = /* Function */F6(
	function (leftStep, bothStep, rightStep, leftDict, rightDict, initialResult) {
		// Let
		var stepState = F3(
			function (rKey, rValue, _v0) {
				stepState:
				while (true) {
					// Destruct
					var list = _v0.a;
					// Destruct
					var result = _v0.b;
					// Case
					// Decider Chain
					if (!list.b) {
						// Decider Leaf Inline
						return /* Tuple */_Utils_Tuple2(
							/* VarLocal */list,
							/* Call */A3(/* VarLocal */rightStep, /* VarLocal */rKey, /* VarLocal */rValue, /* VarLocal */result));
					} else {
						// Decider Leaf Inline
						// Destruct
						var _v2 = list.a;
						// Destruct
						var lKey = _v2.a;
						// Destruct
						var lValue = _v2.b;
						// Destruct
						var rest = list.b;
						// If
						if (/* Call */_Utils_cmp(/* VarLocal */lKey, /* VarLocal */rKey) < 0) {
							// TailCall
							var $temp$rKey = /* VarLocal */rKey,
								$temp$rValue = /* VarLocal */rValue,
								$temp$_v0 = /* Tuple */_Utils_Tuple2(
								/* VarLocal */rest,
								/* Call */A3(/* VarLocal */leftStep, /* VarLocal */lKey, /* VarLocal */lValue, /* VarLocal */result));
							rKey = $temp$rKey;
							rValue = $temp$rValue;
							_v0 = $temp$_v0;
							continue stepState;
						} else {
							if (/* Call */_Utils_cmp(/* VarLocal */lKey, /* VarLocal */rKey) > 0) {
								return /* Tuple */_Utils_Tuple2(
									/* VarLocal */list,
									/* Call */A3(/* VarLocal */rightStep, /* VarLocal */rKey, /* VarLocal */rValue, /* VarLocal */result));
							} else {
								return /* Tuple */_Utils_Tuple2(
									/* VarLocal */rest,
									/* Call */A4(/* VarLocal */bothStep, /* VarLocal */lKey, /* VarLocal */lValue, /* VarLocal */rValue, /* VarLocal */result));
							}
						}
					}
				}
			});
		// Let
		var _v3 = /* Call */A3(
			$elm$core$Dict$foldl,
			/* VarLocal */stepState,
			/* Tuple */_Utils_Tuple2(
				/* Call */$elm$core$Dict$toList(/* VarLocal */leftDict),
				/* VarLocal */initialResult),
			/* VarLocal */rightDict);
		// Destruct
		var leftovers = _v3.a;
		// Destruct
		var intermediateResult = _v3.b;
		return /* Call */A3(
			$elm$core$List$foldl,
			/* Function */F2(
				function (_v4, result) {
					// Destruct
					var k = _v4.a;
					// Destruct
					var v = _v4.b;
					return /* Call */A3(/* VarLocal */leftStep, /* VarLocal */k, /* VarLocal */v, /* VarLocal */result);
				}),
			/* VarLocal */intermediateResult,
			/* VarLocal */leftovers);
	});
// Define, deps=
var $elm$browser$Browser$Events$Event = /* Function */F2(
	function (key, event) {
		return /* Record */{event: /* VarLocal */event, key: /* VarLocal */key};
	});
// Define, deps=$elm$kernel$Platform$$
var $elm$core$Platform$sendToSelf = /* VarKernel */_Platform_sendToSelf;
// Define, deps=$elm$kernel$Browser$$, $elm$kernel$Utils$$, $elm$browser$Browser$Events$Event, $elm$core$Basics$apL, $elm$core$Task$map, $elm$core$Platform$sendToSelf
var $elm$browser$Browser$Events$spawn = /* Function */F3(
	function (router, key, _v0) {
		// Destruct
		var node = _v0.a;
		// Destruct
		var name = _v0.b;
		// Let
		var actualNode = function () {
			// Case
			// Decider Chain
			if (node.$ === 'Document') {
				// Decider Leaf Inline
				return /* VarKernel */_Browser_doc;
			} else {
				// Decider Leaf Inline
				return /* VarKernel */_Browser_window;
			}
		}();
		return /* Call *//* Call */A2(
			$elm$core$Task$map,
			/* Function */function (value) {
				return /* Tuple */_Utils_Tuple2(/* VarLocal */key, /* VarLocal */value);
			},
			/* Call *//* Call */A3(
				/* VarKernel */_Browser_on,
				/* VarLocal */actualNode,
				/* VarLocal */name,
				/* Function */function (event) {
					return /* Call */A2(
						$elm$core$Platform$sendToSelf,
						/* VarLocal */router,
						/* Call */A2(/* VarGlobal */$elm$browser$Browser$Events$Event, /* VarLocal */key, /* VarLocal */event));
				}));
	});
// Define, deps=$elm$core$Dict$foldl, $elm$core$Dict$insert
var $elm$core$Dict$union = /* Function */F2(
	function (t1, t2) {
		return /* Call */A3($elm$core$Dict$foldl, /* VarGlobal */$elm$core$Dict$insert, /* VarLocal */t2, /* VarLocal */t1);
	});
// Define, deps=$elm$kernel$List$$, $elm$kernel$Utils$$, $elm$browser$Browser$Events$State, $elm$browser$Browser$Events$addKey, $elm$core$Task$andThen, $elm$core$Basics$apR, $elm$core$List$cons, $elm$core$Dict$empty, $elm$core$Dict$fromList, $elm$core$Dict$insert, $elm$core$Process$kill, $elm$core$List$map, $elm$core$Dict$merge, $elm$core$Task$sequence, $elm$browser$Browser$Events$spawn, $elm$core$Task$succeed, $elm$core$Dict$union
var $elm$browser$Browser$Events$onEffects = /* Function */F3(
	function (router, subs, state) {
		// Let
		var stepRight = /* Function */F3(
			function (key, sub, _v6) {
				// Destruct
				var deads = _v6.a;
				// Destruct
				var lives = _v6.b;
				// Destruct
				var news = _v6.c;
				return /* Tuple */_Utils_Tuple3(
					/* VarLocal */deads,
					/* VarLocal */lives,
					/* Call */A2(
						$elm$core$List$cons,
						/* Call */A3(/* VarGlobal */$elm$browser$Browser$Events$spawn, /* VarLocal */router, /* VarLocal */key, /* VarLocal */sub),
						/* VarLocal */news));
			});
		// Let
		var stepLeft = /* Function */F3(
			function (_v4, pid, _v5) {
				// Destruct
				var deads = _v5.a;
				// Destruct
				var lives = _v5.b;
				// Destruct
				var news = _v5.c;
				return /* Tuple */_Utils_Tuple3(
					/* Call */A2($elm$core$List$cons, /* VarLocal */pid, /* VarLocal */deads),
					/* VarLocal */lives,
					/* VarLocal */news);
			});
		// Let
		var stepBoth = /* Function */F4(
			function (key, pid, _v2, _v3) {
				// Destruct
				var deads = _v3.a;
				// Destruct
				var lives = _v3.b;
				// Destruct
				var news = _v3.c;
				return /* Tuple */_Utils_Tuple3(
					/* VarLocal */deads,
					/* Call */A3($elm$core$Dict$insert, /* VarLocal */key, /* VarLocal */pid, /* VarLocal */lives),
					/* VarLocal */news);
			});
		// Let
		var newSubs = /* Call */A2($elm$core$List$map, /* VarGlobal */$elm$browser$Browser$Events$addKey, /* VarLocal */subs);
		// Let
		var _v0 = /* Call */A6(
			$elm$core$Dict$merge,
			/* VarLocal */stepLeft,
			/* VarLocal */stepBoth,
			/* VarLocal */stepRight,
			/* Access *//* VarLocal */state.pids,
			/* Call */$elm$core$Dict$fromList(/* VarLocal */newSubs),
			/* Tuple */_Utils_Tuple3(/* List */_List_Nil, /* VarGlobal */$elm$core$Dict$empty, /* List */_List_Nil));
		// Destruct
		var deadPids = _v0.a;
		// Destruct
		var livePids = _v0.b;
		// Destruct
		var makeNewPids = _v0.c;
		return /* Call *//* Call */A2(
			$elm$core$Task$andThen,
			/* Function */function (pids) {
				return /* Call */$elm$core$Task$succeed(
					/* Call */A2(
						/* VarGlobal */$elm$browser$Browser$Events$State,
						/* VarLocal */newSubs,
						/* Call */A2(
							$elm$core$Dict$union,
							/* VarLocal */livePids,
							/* Call */$elm$core$Dict$fromList(/* VarLocal */pids))));
			},
			/* Call *//* Call */A2(
				$elm$core$Task$andThen,
				/* Function */function (_v1) {
					return /* Call */$elm$core$Task$sequence(/* VarLocal */makeNewPids);
				},
				/* Call */$elm$core$Task$sequence(
					/* Call */A2($elm$core$List$map, /* VarGlobal */$elm$core$Process$kill, /* VarLocal */deadPids))));
	});
// Define, deps=$elm$core$List$cons
var $elm$core$List$maybeCons = /* Function */F3(
	function (f, mx, xs) {
		// Let
		var _v0 = /* Call *//* VarLocal */f(/* VarLocal */mx);
		// Case
		// Decider Chain
		if (_v0.$ === 'Just') {
			// Decider Leaf Inline
			// Destruct
			var x = _v0.a;
			return /* Call */A2($elm$core$List$cons, /* VarLocal */x, /* VarLocal */xs);
		} else {
			// Decider Leaf Inline
			return /* VarLocal */xs;
		}
	});
// Define, deps=$elm$kernel$List$$, $elm$core$List$foldr, $elm$core$List$maybeCons
var $elm$core$List$filterMap = /* Function */F2(
	function (f, xs) {
		return /* Call */A3(
			$elm$core$List$foldr,
			/* Call */$elm$core$List$maybeCons(/* VarLocal */f),
			/* List */_List_Nil,
			/* VarLocal */xs);
	});
// Define, deps=$elm$kernel$Browser$$, $elm$core$Maybe$Nothing, $elm$core$Task$andThen, $elm$core$Basics$apR, $elm$core$Basics$eq, $elm$core$List$filterMap, $elm$core$List$map, $elm$core$Platform$sendToApp, $elm$core$Task$sequence, $elm$core$Task$succeed
var $elm$browser$Browser$Events$onSelfMsg = /* Function */F3(
	function (router, _v0, state) {
		// Destruct
		var event = _v0.event;
		// Destruct
		var key = _v0.key;
		// Let
		var toMessage = /* Function */function (_v2) {
			// Destruct
			var subKey = _v2.a;
			// Destruct
			var _v3 = _v2.b;
			// Destruct
			var node = _v3.a;
			// Destruct
			var name = _v3.b;
			// Destruct
			var decoder = _v3.c;
			return /* If *//* Call */_Utils_eq(/* VarLocal */subKey, /* VarLocal */key) ? /* Call */A2(/* VarKernel */_Browser_decodeEvent, /* VarLocal */decoder, /* VarLocal */event) : /* VarGlobal */$elm$core$Maybe$Nothing;
		};
		// Let
		var messages = /* Call */A2($elm$core$List$filterMap, /* VarLocal */toMessage, /* Access *//* VarLocal */state.subs);
		return /* Call *//* Call */A2(
			$elm$core$Task$andThen,
			/* Function */function (_v1) {
				return /* Call */$elm$core$Task$succeed(/* VarLocal */state);
			},
			/* Call */$elm$core$Task$sequence(
				/* Call */A2(
					$elm$core$List$map,
					/* Call */$elm$core$Platform$sendToApp(/* VarLocal */router),
					/* VarLocal */messages)));
	});
// Define, deps=$elm$browser$Browser$Events$MySub, $elm$json$Json$Decode$map
var $elm$browser$Browser$Events$subMap = /* Function */F2(
	function (func, _v0) {
		// Destruct
		var node = _v0.a;
		// Destruct
		var name = _v0.b;
		// Destruct
		var decoder = _v0.c;
		return /* Call */A3(
			/* VarGlobal */$elm$browser$Browser$Events$MySub,
			/* VarLocal */node,
			/* VarLocal */name,
			/* Call */A2(/* VarGlobal */$elm$json$Json$Decode$map, /* VarLocal */func, /* VarLocal */decoder));
	});
_Platform_effectManagers['Browser.Events'] = _Platform_createManager($elm$browser$Browser$Events$init, $elm$browser$Browser$Events$onEffects, $elm$browser$Browser$Events$onSelfMsg, 0, $elm$browser$Browser$Events$subMap);
var $elm$browser$Browser$Events$subscription = _Platform_leaf('Browser.Events');
// End Manager $elm$browser$Browser$Events$$fx$
// Define, deps=$elm$browser$Browser$Events$MySub, $elm$browser$Browser$Events$subscription
var $elm$browser$Browser$Events$on = /* Function */F3(
	function (node, name, decoder) {
		return /* Call *//* VarGlobal */$elm$browser$Browser$Events$subscription(
			/* Call */A3(/* VarGlobal */$elm$browser$Browser$Events$MySub, /* VarLocal */node, /* VarLocal */name, /* VarLocal */decoder));
	});
// Define, deps=$elm$browser$Browser$Events$Document, $elm$browser$Browser$Events$on
var $elm$browser$Browser$Events$onKeyDown = /* Call */A2(/* VarGlobal */$elm$browser$Browser$Events$on, /* VarEnum */$elm$browser$Browser$Events$Document, /* Str */'keydown');
// Define, deps=$elm$kernel$Json$$
var $elm$json$Json$Decode$string = /* VarKernel */_Json_decodeString;
// Ctor
var $author$project$Main$JsonLoaded = function (a) {
	return {$: 'JsonLoaded', a: a};
};
// Define, deps=$elm$kernel$Json$$
var $elm$json$Json$Decode$decodeString = /* VarKernel */_Json_runOnString;
// Ctor
var $elm$http$Http$BadStatus_ = F2(
	function (a, b) {
		return {$: 'BadStatus_', a: a, b: b};
	});
// Ctor
var $elm$http$Http$BadUrl_ = function (a) {
	return {$: 'BadUrl_', a: a};
};
// Ctor
var $elm$http$Http$GoodStatus_ = F2(
	function (a, b) {
		return {$: 'GoodStatus_', a: a, b: b};
	});
// Ctor
var $elm$http$Http$NetworkError_ = {$: 'NetworkError_'};
// Ctor
var $elm$http$Http$Receiving = function (a) {
	return {$: 'Receiving', a: a};
};
// Ctor
var $elm$http$Http$Sending = function (a) {
	return {$: 'Sending', a: a};
};
// Ctor
var $elm$http$Http$Timeout_ = {$: 'Timeout_'};
// Define, deps=$elm$core$Basics$False, $elm$core$Basics$True
var $elm$core$Maybe$isJust = /* Function */function (maybe) {
	// Case
	// Decider Chain
	if (maybe.$ === 'Just') {
		// Decider Leaf Inline
		return /* Bool */true;
	} else {
		// Decider Leaf Inline
		return /* Bool */false;
	}
};
// Link $elm$core$Dict$_M$get
// Cycle get
var $elm$core$Dict$get = F2(
	function (targetKey, dict) {
		get:
		while (true) {
			// Case
			// Decider Chain
			if (dict.$ === 'RBEmpty_elm_builtin') {
				// Decider Leaf Inline
				return /* VarGlobal */$elm$core$Maybe$Nothing;
			} else {
				// Decider Leaf Inline
				// Destruct
				var key = dict.b;
				// Destruct
				var value = dict.c;
				// Destruct
				var left = dict.d;
				// Destruct
				var right = dict.e;
				// Let
				var _v1 = /* Call */A2($elm$core$Basics$compare, /* VarLocal */targetKey, /* VarLocal */key);
				// Case
				// Decider Fanout
				switch (_v1.$) {
					case 'LT':
						// Decider Leaf Inline
						// TailCall
						var $temp$targetKey = /* VarLocal */targetKey,
							$temp$dict = /* VarLocal */left;
						targetKey = $temp$targetKey;
						dict = $temp$dict;
						continue get;
					case 'EQ':
						// Decider Leaf Inline
						return /* Call */$elm$core$Maybe$Just(/* VarLocal */value);
					default:
						// Decider Leaf Inline
						// TailCall
						var $temp$targetKey = /* VarLocal */targetKey,
							$temp$dict = /* VarLocal */right;
						targetKey = $temp$targetKey;
						dict = $temp$dict;
						continue get;
				}
			}
		}
	});
// end Cycle get
// Link $elm$core$Dict$_M$removeHelp
// Link $elm$core$Dict$_M$getMin
// Cycle getMin
var $elm$core$Dict$getMin = function (dict) {
	getMin:
	while (true) {
		// Case
		// Decider Chain
		if ((dict.$ === 'RBNode_elm_builtin') && (dict.d.$ === 'RBNode_elm_builtin')) {
			// Decider Leaf Inline
			// Destruct
			var left = dict.d;
			// TailCall
			var $temp$dict = /* VarLocal */left;
			dict = $temp$dict;
			continue getMin;
		} else {
			// Decider Leaf Inline
			return /* VarLocal */dict;
		}
	}
};
// end Cycle getMin
// Define, deps=$elm$core$Dict$Black, $elm$core$Dict$RBNode_elm_builtin, $elm$core$Dict$Red
var $elm$core$Dict$moveRedLeft = /* Function */function (dict) {
	// Case
	// Decider Chain
	if (((dict.$ === 'RBNode_elm_builtin') && (dict.d.$ === 'RBNode_elm_builtin')) && (dict.e.$ === 'RBNode_elm_builtin')) {
		// Decider Chain
		if ((dict.e.d.$ === 'RBNode_elm_builtin') && (dict.e.d.a.$ === 'Red')) {
			// Decider Leaf Inline
			// Destruct
			var clr = dict.a;
			// Destruct
			var k = dict.b;
			// Destruct
			var v = dict.c;
			// Destruct
			var _v1 = dict.d;
			// Destruct
			var lClr = _v1.a;
			// Destruct
			var lK = _v1.b;
			// Destruct
			var lV = _v1.c;
			// Destruct
			var lLeft = _v1.d;
			// Destruct
			var lRight = _v1.e;
			// Destruct
			var _v2 = dict.e;
			// Destruct
			var rClr = _v2.a;
			// Destruct
			var rK = _v2.b;
			// Destruct
			var rV = _v2.c;
			// Destruct
			var rLeft = _v2.d;
			// Destruct
			var _v3 = rLeft.a;
			// Destruct
			var rlK = rLeft.b;
			// Destruct
			var rlV = rLeft.c;
			// Destruct
			var rlL = rLeft.d;
			// Destruct
			var rlR = rLeft.e;
			// Destruct
			var rRight = _v2.e;
			return /* Call */A5(
				$elm$core$Dict$RBNode_elm_builtin,
				/* VarEnum */$elm$core$Dict$Red,
				/* VarLocal */rlK,
				/* VarLocal */rlV,
				/* Call */A5(
					$elm$core$Dict$RBNode_elm_builtin,
					/* VarEnum */$elm$core$Dict$Black,
					/* VarLocal */k,
					/* VarLocal */v,
					/* Call */A5($elm$core$Dict$RBNode_elm_builtin, /* VarEnum */$elm$core$Dict$Red, /* VarLocal */lK, /* VarLocal */lV, /* VarLocal */lLeft, /* VarLocal */lRight),
					/* VarLocal */rlL),
				/* Call */A5($elm$core$Dict$RBNode_elm_builtin, /* VarEnum */$elm$core$Dict$Black, /* VarLocal */rK, /* VarLocal */rV, /* VarLocal */rlR, /* VarLocal */rRight));
		} else {
			// Decider Leaf Inline
			// Destruct
			var clr = dict.a;
			// Destruct
			var k = dict.b;
			// Destruct
			var v = dict.c;
			// Destruct
			var _v4 = dict.d;
			// Destruct
			var lClr = _v4.a;
			// Destruct
			var lK = _v4.b;
			// Destruct
			var lV = _v4.c;
			// Destruct
			var lLeft = _v4.d;
			// Destruct
			var lRight = _v4.e;
			// Destruct
			var _v5 = dict.e;
			// Destruct
			var rClr = _v5.a;
			// Destruct
			var rK = _v5.b;
			// Destruct
			var rV = _v5.c;
			// Destruct
			var rLeft = _v5.d;
			// Destruct
			var rRight = _v5.e;
			// Case
			// Decider Chain
			if (clr.$ === 'Black') {
				// Decider Leaf Inline
				return /* Call */A5(
					$elm$core$Dict$RBNode_elm_builtin,
					/* VarEnum */$elm$core$Dict$Black,
					/* VarLocal */k,
					/* VarLocal */v,
					/* Call */A5($elm$core$Dict$RBNode_elm_builtin, /* VarEnum */$elm$core$Dict$Red, /* VarLocal */lK, /* VarLocal */lV, /* VarLocal */lLeft, /* VarLocal */lRight),
					/* Call */A5($elm$core$Dict$RBNode_elm_builtin, /* VarEnum */$elm$core$Dict$Red, /* VarLocal */rK, /* VarLocal */rV, /* VarLocal */rLeft, /* VarLocal */rRight));
			} else {
				// Decider Leaf Inline
				return /* Call */A5(
					$elm$core$Dict$RBNode_elm_builtin,
					/* VarEnum */$elm$core$Dict$Black,
					/* VarLocal */k,
					/* VarLocal */v,
					/* Call */A5($elm$core$Dict$RBNode_elm_builtin, /* VarEnum */$elm$core$Dict$Red, /* VarLocal */lK, /* VarLocal */lV, /* VarLocal */lLeft, /* VarLocal */lRight),
					/* Call */A5($elm$core$Dict$RBNode_elm_builtin, /* VarEnum */$elm$core$Dict$Red, /* VarLocal */rK, /* VarLocal */rV, /* VarLocal */rLeft, /* VarLocal */rRight));
			}
		}
	} else {
		// Decider Leaf Inline
		return /* VarLocal */dict;
	}
};
// Link $elm$core$Dict$_M$removeHelp
// Define, deps=$elm$core$Dict$Black, $elm$core$Dict$RBNode_elm_builtin, $elm$core$Dict$Red
var $elm$core$Dict$moveRedRight = /* Function */function (dict) {
	// Case
	// Decider Chain
	if (((dict.$ === 'RBNode_elm_builtin') && (dict.d.$ === 'RBNode_elm_builtin')) && (dict.e.$ === 'RBNode_elm_builtin')) {
		// Decider Chain
		if ((dict.d.d.$ === 'RBNode_elm_builtin') && (dict.d.d.a.$ === 'Red')) {
			// Decider Leaf Inline
			// Destruct
			var clr = dict.a;
			// Destruct
			var k = dict.b;
			// Destruct
			var v = dict.c;
			// Destruct
			var _v1 = dict.d;
			// Destruct
			var lClr = _v1.a;
			// Destruct
			var lK = _v1.b;
			// Destruct
			var lV = _v1.c;
			// Destruct
			var _v2 = _v1.d;
			// Destruct
			var _v3 = _v2.a;
			// Destruct
			var llK = _v2.b;
			// Destruct
			var llV = _v2.c;
			// Destruct
			var llLeft = _v2.d;
			// Destruct
			var llRight = _v2.e;
			// Destruct
			var lRight = _v1.e;
			// Destruct
			var _v4 = dict.e;
			// Destruct
			var rClr = _v4.a;
			// Destruct
			var rK = _v4.b;
			// Destruct
			var rV = _v4.c;
			// Destruct
			var rLeft = _v4.d;
			// Destruct
			var rRight = _v4.e;
			return /* Call */A5(
				$elm$core$Dict$RBNode_elm_builtin,
				/* VarEnum */$elm$core$Dict$Red,
				/* VarLocal */lK,
				/* VarLocal */lV,
				/* Call */A5($elm$core$Dict$RBNode_elm_builtin, /* VarEnum */$elm$core$Dict$Black, /* VarLocal */llK, /* VarLocal */llV, /* VarLocal */llLeft, /* VarLocal */llRight),
				/* Call */A5(
					$elm$core$Dict$RBNode_elm_builtin,
					/* VarEnum */$elm$core$Dict$Black,
					/* VarLocal */k,
					/* VarLocal */v,
					/* VarLocal */lRight,
					/* Call */A5($elm$core$Dict$RBNode_elm_builtin, /* VarEnum */$elm$core$Dict$Red, /* VarLocal */rK, /* VarLocal */rV, /* VarLocal */rLeft, /* VarLocal */rRight)));
		} else {
			// Decider Leaf Inline
			// Destruct
			var clr = dict.a;
			// Destruct
			var k = dict.b;
			// Destruct
			var v = dict.c;
			// Destruct
			var _v5 = dict.d;
			// Destruct
			var lClr = _v5.a;
			// Destruct
			var lK = _v5.b;
			// Destruct
			var lV = _v5.c;
			// Destruct
			var lLeft = _v5.d;
			// Destruct
			var lRight = _v5.e;
			// Destruct
			var _v6 = dict.e;
			// Destruct
			var rClr = _v6.a;
			// Destruct
			var rK = _v6.b;
			// Destruct
			var rV = _v6.c;
			// Destruct
			var rLeft = _v6.d;
			// Destruct
			var rRight = _v6.e;
			// Case
			// Decider Chain
			if (clr.$ === 'Black') {
				// Decider Leaf Inline
				return /* Call */A5(
					$elm$core$Dict$RBNode_elm_builtin,
					/* VarEnum */$elm$core$Dict$Black,
					/* VarLocal */k,
					/* VarLocal */v,
					/* Call */A5($elm$core$Dict$RBNode_elm_builtin, /* VarEnum */$elm$core$Dict$Red, /* VarLocal */lK, /* VarLocal */lV, /* VarLocal */lLeft, /* VarLocal */lRight),
					/* Call */A5($elm$core$Dict$RBNode_elm_builtin, /* VarEnum */$elm$core$Dict$Red, /* VarLocal */rK, /* VarLocal */rV, /* VarLocal */rLeft, /* VarLocal */rRight));
			} else {
				// Decider Leaf Inline
				return /* Call */A5(
					$elm$core$Dict$RBNode_elm_builtin,
					/* VarEnum */$elm$core$Dict$Black,
					/* VarLocal */k,
					/* VarLocal */v,
					/* Call */A5($elm$core$Dict$RBNode_elm_builtin, /* VarEnum */$elm$core$Dict$Red, /* VarLocal */lK, /* VarLocal */lV, /* VarLocal */lLeft, /* VarLocal */lRight),
					/* Call */A5($elm$core$Dict$RBNode_elm_builtin, /* VarEnum */$elm$core$Dict$Red, /* VarLocal */rK, /* VarLocal */rV, /* VarLocal */rLeft, /* VarLocal */rRight));
			}
		}
	} else {
		// Decider Leaf Inline
		return /* VarLocal */dict;
	}
};
// Define, deps=$elm$core$Dict$RBNode_elm_builtin, $elm$core$Dict$Red, $elm$core$Dict$moveRedRight
var $elm$core$Dict$removeHelpPrepEQGT = /* Function */F7(
	function (targetKey, dict, color, key, value, left, right) {
		// Case
		// Decider Chain
		if ((left.$ === 'RBNode_elm_builtin') && (left.a.$ === 'Red')) {
			// Decider Leaf Inline
			// Destruct
			var _v1 = left.a;
			// Destruct
			var lK = left.b;
			// Destruct
			var lV = left.c;
			// Destruct
			var lLeft = left.d;
			// Destruct
			var lRight = left.e;
			return /* Call */A5(
				$elm$core$Dict$RBNode_elm_builtin,
				/* VarLocal */color,
				/* VarLocal */lK,
				/* VarLocal */lV,
				/* VarLocal */lLeft,
				/* Call */A5($elm$core$Dict$RBNode_elm_builtin, /* VarEnum */$elm$core$Dict$Red, /* VarLocal */key, /* VarLocal */value, /* VarLocal */lRight, /* VarLocal */right));
		} else {
			// Decider Leaf Inline
			// Case
			// label:_v2
			_v2$2:
			while (true) {
				// Decider Chain
				if ((right.$ === 'RBNode_elm_builtin') && (right.a.$ === 'Black')) {
					// Decider Chain
					if (right.d.$ === 'RBNode_elm_builtin') {
						// Decider Chain
						if (right.d.a.$ === 'Black') {
							// Decider Leaf Inline
							// Destruct
							var _v3 = right.a;
							// Destruct
							var _v4 = right.d;
							// Destruct
							var _v5 = _v4.a;
							return /* Call */$elm$core$Dict$moveRedRight(/* VarLocal */dict);
						} else {
							// Decider Leaf Jump
							break _v2$2;
						}
					} else {
						// Decider Leaf Inline
						// Destruct
						var _v6 = right.a;
						// Destruct
						var _v7 = right.d;
						return /* Call */$elm$core$Dict$moveRedRight(/* VarLocal */dict);
					}
				} else {
					// Decider Leaf Jump
					break _v2$2;
				}
			}
			return /* VarLocal */dict;
		}
	});
// Link $elm$core$Dict$_M$removeMin
// Cycle removeMin
var $elm$core$Dict$removeMin = /* Function */function (dict) {
	// Case
	// Decider Chain
	if ((dict.$ === 'RBNode_elm_builtin') && (dict.d.$ === 'RBNode_elm_builtin')) {
		// Decider Leaf Inline
		// Destruct
		var color = dict.a;
		// Destruct
		var key = dict.b;
		// Destruct
		var value = dict.c;
		// Destruct
		var left = dict.d;
		// Destruct
		var lColor = left.a;
		// Destruct
		var lLeft = left.d;
		// Destruct
		var right = dict.e;
		// Case
		// Decider Chain
		if (lColor.$ === 'Black') {
			// Decider Leaf Inline
			// Case
			// Decider Chain
			if ((lLeft.$ === 'RBNode_elm_builtin') && (lLeft.a.$ === 'Red')) {
				// Decider Leaf Inline
				// Destruct
				var _v3 = lLeft.a;
				return /* Call */A5(
					$elm$core$Dict$RBNode_elm_builtin,
					/* VarLocal */color,
					/* VarLocal */key,
					/* VarLocal */value,
					/* Call */$elm$core$Dict$removeMin(/* VarLocal */left),
					/* VarLocal */right);
			} else {
				// Decider Leaf Inline
				// Let
				var _v4 = /* Call */$elm$core$Dict$moveRedLeft(/* VarLocal */dict);
				// Case
				// Decider Chain
				if (_v4.$ === 'RBNode_elm_builtin') {
					// Decider Leaf Inline
					// Destruct
					var nColor = _v4.a;
					// Destruct
					var nKey = _v4.b;
					// Destruct
					var nValue = _v4.c;
					// Destruct
					var nLeft = _v4.d;
					// Destruct
					var nRight = _v4.e;
					return /* Call */A5(
						$elm$core$Dict$balance,
						/* VarLocal */nColor,
						/* VarLocal */nKey,
						/* VarLocal */nValue,
						/* Call */$elm$core$Dict$removeMin(/* VarLocal */nLeft),
						/* VarLocal */nRight);
				} else {
					// Decider Leaf Inline
					return /* VarGlobal */$elm$core$Dict$RBEmpty_elm_builtin;
				}
			}
		} else {
			// Decider Leaf Inline
			return /* Call */A5(
				$elm$core$Dict$RBNode_elm_builtin,
				/* VarLocal */color,
				/* VarLocal */key,
				/* VarLocal */value,
				/* Call */$elm$core$Dict$removeMin(/* VarLocal */left),
				/* VarLocal */right);
		}
	} else {
		// Decider Leaf Inline
		return /* VarGlobal */$elm$core$Dict$RBEmpty_elm_builtin;
	}
};
// end Cycle removeMin
// Cycle removeHelp, removeHelpEQGT
var $elm$core$Dict$removeHelp = /* Function */F2(
	function (targetKey, dict) {
		// Case
		// Decider Chain
		if (dict.$ === 'RBEmpty_elm_builtin') {
			// Decider Leaf Inline
			return /* VarGlobal */$elm$core$Dict$RBEmpty_elm_builtin;
		} else {
			// Decider Leaf Inline
			// Destruct
			var color = dict.a;
			// Destruct
			var key = dict.b;
			// Destruct
			var value = dict.c;
			// Destruct
			var left = dict.d;
			// Destruct
			var right = dict.e;
			// If
			if (/* Call */_Utils_cmp(/* VarLocal */targetKey, /* VarLocal */key) < 0) {
				// Case
				// Decider Chain
				if ((left.$ === 'RBNode_elm_builtin') && (left.a.$ === 'Black')) {
					// Decider Leaf Inline
					// Destruct
					var _v4 = left.a;
					// Destruct
					var lLeft = left.d;
					// Case
					// Decider Chain
					if ((lLeft.$ === 'RBNode_elm_builtin') && (lLeft.a.$ === 'Red')) {
						// Decider Leaf Inline
						// Destruct
						var _v6 = lLeft.a;
						return /* Call */A5(
							$elm$core$Dict$RBNode_elm_builtin,
							/* VarLocal */color,
							/* VarLocal */key,
							/* VarLocal */value,
							/* Call */A2($elm$core$Dict$removeHelp, /* VarLocal */targetKey, /* VarLocal */left),
							/* VarLocal */right);
					} else {
						// Decider Leaf Inline
						// Let
						var _v7 = /* Call */$elm$core$Dict$moveRedLeft(/* VarLocal */dict);
						// Case
						// Decider Chain
						if (_v7.$ === 'RBNode_elm_builtin') {
							// Decider Leaf Inline
							// Destruct
							var nColor = _v7.a;
							// Destruct
							var nKey = _v7.b;
							// Destruct
							var nValue = _v7.c;
							// Destruct
							var nLeft = _v7.d;
							// Destruct
							var nRight = _v7.e;
							return /* Call */A5(
								$elm$core$Dict$balance,
								/* VarLocal */nColor,
								/* VarLocal */nKey,
								/* VarLocal */nValue,
								/* Call */A2($elm$core$Dict$removeHelp, /* VarLocal */targetKey, /* VarLocal */nLeft),
								/* VarLocal */nRight);
						} else {
							// Decider Leaf Inline
							return /* VarGlobal */$elm$core$Dict$RBEmpty_elm_builtin;
						}
					}
				} else {
					// Decider Leaf Inline
					return /* Call */A5(
						$elm$core$Dict$RBNode_elm_builtin,
						/* VarLocal */color,
						/* VarLocal */key,
						/* VarLocal */value,
						/* Call */A2($elm$core$Dict$removeHelp, /* VarLocal */targetKey, /* VarLocal */left),
						/* VarLocal */right);
				}
			} else {
				return /* Call */A2(
					$elm$core$Dict$removeHelpEQGT,
					/* VarLocal */targetKey,
					/* Call */A7($elm$core$Dict$removeHelpPrepEQGT, /* VarLocal */targetKey, /* VarLocal */dict, /* VarLocal */color, /* VarLocal */key, /* VarLocal */value, /* VarLocal */left, /* VarLocal */right));
			}
		}
	});
var $elm$core$Dict$removeHelpEQGT = /* Function */F2(
	function (targetKey, dict) {
		// Case
		// Decider Chain
		if (dict.$ === 'RBNode_elm_builtin') {
			// Decider Leaf Inline
			// Destruct
			var color = dict.a;
			// Destruct
			var key = dict.b;
			// Destruct
			var value = dict.c;
			// Destruct
			var left = dict.d;
			// Destruct
			var right = dict.e;
			// If
			if (/* Call */_Utils_eq(/* VarLocal */targetKey, /* VarLocal */key)) {
				// Let
				var _v1 = /* Call */$elm$core$Dict$getMin(/* VarLocal */right);
				// Case
				// Decider Chain
				if (_v1.$ === 'RBNode_elm_builtin') {
					// Decider Leaf Inline
					// Destruct
					var minKey = _v1.b;
					// Destruct
					var minValue = _v1.c;
					return /* Call */A5(
						$elm$core$Dict$balance,
						/* VarLocal */color,
						/* VarLocal */minKey,
						/* VarLocal */minValue,
						/* VarLocal */left,
						/* Call */$elm$core$Dict$removeMin(/* VarLocal */right));
				} else {
					// Decider Leaf Inline
					return /* VarGlobal */$elm$core$Dict$RBEmpty_elm_builtin;
				}
			} else {
				return /* Call */A5(
					$elm$core$Dict$balance,
					/* VarLocal */color,
					/* VarLocal */key,
					/* VarLocal */value,
					/* VarLocal */left,
					/* Call */A2($elm$core$Dict$removeHelp, /* VarLocal */targetKey, /* VarLocal */right));
			}
		} else {
			// Decider Leaf Inline
			return /* VarGlobal */$elm$core$Dict$RBEmpty_elm_builtin;
		}
	});
// end Cycle removeHelp, removeHelpEQGT
// Define, deps=$elm$core$Dict$Black, $elm$core$Dict$RBNode_elm_builtin, $elm$core$Dict$removeHelp
var $elm$core$Dict$remove = /* Function */F2(
	function (key, dict) {
		// Let
		var _v0 = /* Call */A2($elm$core$Dict$removeHelp, /* VarLocal */key, /* VarLocal */dict);
		// Case
		// Decider Chain
		if ((_v0.$ === 'RBNode_elm_builtin') && (_v0.a.$ === 'Red')) {
			// Decider Leaf Inline
			// Destruct
			var _v1 = _v0.a;
			// Destruct
			var k = _v0.b;
			// Destruct
			var v = _v0.c;
			// Destruct
			var l = _v0.d;
			// Destruct
			var r = _v0.e;
			return /* Call */A5($elm$core$Dict$RBNode_elm_builtin, /* VarEnum */$elm$core$Dict$Black, /* VarLocal */k, /* VarLocal */v, /* VarLocal */l, /* VarLocal */r);
		} else {
			// Decider Leaf Inline
			// Destruct
			var x = _v0;
			return /* VarLocal */x;
		}
	});
// Define, deps=$elm$core$Dict$get, $elm$core$Dict$insert, $elm$core$Dict$remove
var $elm$core$Dict$update = /* Function */F3(
	function (targetKey, alter, dictionary) {
		// Let
		var _v0 = /* Call *//* VarLocal */alter(
			/* Call */A2($elm$core$Dict$get, /* VarLocal */targetKey, /* VarLocal */dictionary));
		// Case
		// Decider Chain
		if (_v0.$ === 'Just') {
			// Decider Leaf Inline
			// Destruct
			var value = _v0.a;
			return /* Call */A3($elm$core$Dict$insert, /* VarLocal */targetKey, /* VarLocal */value, /* VarLocal */dictionary);
		} else {
			// Decider Leaf Inline
			return /* Call */A2($elm$core$Dict$remove, /* VarLocal */targetKey, /* VarLocal */dictionary);
		}
	});
// Define, deps=
var $elm$core$Basics$composeR = /* Function */F3(
	function (f, g, x) {
		return /* Call *//* VarLocal */g(
			/* Call *//* VarLocal */f(/* VarLocal */x));
	});
// Define, deps=$elm$kernel$Http$$, $elm$core$Basics$composeR, $elm$core$Basics$identity
var $elm$http$Http$expectStringResponse = /* Function */F2(
	function (toMsg, toResult) {
		return /* Call */A3(
			/* VarKernel */_Http_expect,
			/* Str */'',
			/* VarGlobal */$elm$core$Basics$identity,
			/* Call */A2($elm$core$Basics$composeR, /* VarLocal */toResult, /* VarLocal */toMsg));
	});
// Define, deps=$elm$core$Result$Err, $elm$core$Result$Ok
var $elm$core$Result$mapError = /* Function */F2(
	function (f, result) {
		// Case
		// Decider Chain
		if (result.$ === 'Ok') {
			// Decider Leaf Inline
			// Destruct
			var v = result.a;
			return /* Call */$elm$core$Result$Ok(/* VarLocal */v);
		} else {
			// Decider Leaf Inline
			// Destruct
			var e = result.a;
			return /* Call */$elm$core$Result$Err(
				/* Call *//* VarLocal */f(/* VarLocal */e));
		}
	});
// Ctor
var $elm$http$Http$BadBody = function (a) {
	return {$: 'BadBody', a: a};
};
// Ctor
var $elm$http$Http$BadStatus = function (a) {
	return {$: 'BadStatus', a: a};
};
// Ctor
var $elm$http$Http$BadUrl = function (a) {
	return {$: 'BadUrl', a: a};
};
// Ctor
var $elm$http$Http$NetworkError = {$: 'NetworkError'};
// Ctor
var $elm$http$Http$Timeout = {$: 'Timeout'};
// Define, deps=$elm$http$Http$BadBody, $elm$http$Http$BadStatus, $elm$http$Http$BadUrl, $elm$core$Result$Err, $elm$http$Http$NetworkError, $elm$http$Http$Timeout, $elm$core$Result$mapError
var $elm$http$Http$resolve = /* Function */F2(
	function (toResult, response) {
		// Case
		// Decider Fanout
		switch (response.$) {
			case 'BadUrl_':
				// Decider Leaf Inline
				// Destruct
				var url = response.a;
				return /* Call */$elm$core$Result$Err(
					/* Call *//* VarGlobal */$elm$http$Http$BadUrl(/* VarLocal */url));
			case 'Timeout_':
				// Decider Leaf Inline
				return /* Call */$elm$core$Result$Err(/* VarGlobal */$elm$http$Http$Timeout);
			case 'NetworkError_':
				// Decider Leaf Inline
				return /* Call */$elm$core$Result$Err(/* VarGlobal */$elm$http$Http$NetworkError);
			case 'BadStatus_':
				// Decider Leaf Inline
				// Destruct
				var metadata = response.a;
				return /* Call */$elm$core$Result$Err(
					/* Call *//* VarGlobal */$elm$http$Http$BadStatus(/* Access *//* VarLocal */metadata.statusCode));
			default:
				// Decider Leaf Inline
				// Destruct
				var body = response.b;
				return /* Call */A2(
					$elm$core$Result$mapError,
					/* VarGlobal */$elm$http$Http$BadBody,
					/* Call *//* VarLocal */toResult(/* VarLocal */body));
		}
	});
// Define, deps=$elm$core$Basics$apL, $elm$json$Json$Decode$decodeString, $elm$json$Json$Decode$errorToString, $elm$http$Http$expectStringResponse, $elm$core$Result$mapError, $elm$http$Http$resolve
var $elm$http$Http$expectJson = /* Function */F2(
	function (toMsg, decoder) {
		return /* Call *//* Call */A2(
			/* VarGlobal */$elm$http$Http$expectStringResponse,
			/* VarLocal */toMsg,
			/* Call *//* Call *//* VarGlobal */$elm$http$Http$resolve(
				/* Function */function (string) {
					return /* Call */A2(
						$elm$core$Result$mapError,
						/* VarGlobal */$elm$json$Json$Decode$errorToString,
						/* Call */A2(/* VarGlobal */$elm$json$Json$Decode$decodeString, /* VarLocal */decoder, /* VarLocal */string));
				}));
	});
// Define, deps=$elm$kernel$Http$$
var $elm$http$Http$emptyBody = /* VarKernel */_Http_emptyBody;
// Ctor
var $elm$http$Http$Request = function (a) {
	return {$: 'Request', a: a};
};
// Link $elm$http$Http$$fx$
// Manager $elm$http$Http$$fx$
// Define, deps=
var $elm$http$Http$State = /* Function */F2(
	function (reqs, subs) {
		return /* Record */{reqs: /* VarLocal */reqs, subs: /* VarLocal */subs};
	});
// Define, deps=$elm$kernel$List$$, $elm$http$Http$State, $elm$core$Dict$empty, $elm$core$Task$succeed
var $elm$http$Http$init = /* Call */$elm$core$Task$succeed(
	/* Call */A2(/* VarGlobal */$elm$http$Http$State, /* VarGlobal */$elm$core$Dict$empty, /* List */_List_Nil));
// Link $elm$http$Http$_M$updateReqs
// Define, deps=$elm$kernel$Scheduler$$
var $elm$core$Process$spawn = /* VarKernel */_Scheduler_spawn;
// Cycle updateReqs
var $elm$http$Http$updateReqs = F3(
	function (router, cmds, reqs) {
		updateReqs:
		while (true) {
			// Case
			// Decider Chain
			if (!cmds.b) {
				// Decider Leaf Inline
				return /* Call */$elm$core$Task$succeed(/* VarLocal */reqs);
			} else {
				// Decider Leaf Inline
				// Destruct
				var cmd = cmds.a;
				// Destruct
				var otherCmds = cmds.b;
				// Case
				// Decider Chain
				if (cmd.$ === 'Cancel') {
					// Decider Leaf Inline
					// Destruct
					var tracker = cmd.a;
					// Let
					var _v2 = /* Call */A2($elm$core$Dict$get, /* VarLocal */tracker, /* VarLocal */reqs);
					// Case
					// Decider Chain
					if (_v2.$ === 'Nothing') {
						// Decider Leaf Inline
						// TailCall
						var $temp$router = /* VarLocal */router,
							$temp$cmds = /* VarLocal */otherCmds,
							$temp$reqs = /* VarLocal */reqs;
						router = $temp$router;
						cmds = $temp$cmds;
						reqs = $temp$reqs;
						continue updateReqs;
					} else {
						// Decider Leaf Inline
						// Destruct
						var pid = _v2.a;
						return /* Call *//* Call */A2(
							$elm$core$Task$andThen,
							/* Function */function (_v3) {
								return /* Call */A3(
									/* VarGlobal */$elm$http$Http$updateReqs,
									/* VarLocal */router,
									/* VarLocal */otherCmds,
									/* Call */A2($elm$core$Dict$remove, /* VarLocal */tracker, /* VarLocal */reqs));
							},
							/* Call */$elm$core$Process$kill(/* VarLocal */pid));
					}
				} else {
					// Decider Leaf Inline
					// Destruct
					var req = cmd.a;
					return /* Call *//* Call */A2(
						$elm$core$Task$andThen,
						/* Function */function (pid) {
							// Let
							var _v4 = /* Access *//* VarLocal */req.tracker;
							// Case
							// Decider Chain
							if (_v4.$ === 'Nothing') {
								// Decider Leaf Inline
								return /* Call */A3(/* VarGlobal */$elm$http$Http$updateReqs, /* VarLocal */router, /* VarLocal */otherCmds, /* VarLocal */reqs);
							} else {
								// Decider Leaf Inline
								// Destruct
								var tracker = _v4.a;
								return /* Call */A3(
									/* VarGlobal */$elm$http$Http$updateReqs,
									/* VarLocal */router,
									/* VarLocal */otherCmds,
									/* Call */A3($elm$core$Dict$insert, /* VarLocal */tracker, /* VarLocal */pid, /* VarLocal */reqs));
							}
						},
						/* Call */$elm$core$Process$spawn(
							/* Call */A3(
								/* VarKernel */_Http_toTask,
								/* VarLocal */router,
								/* Call */$elm$core$Platform$sendToApp(/* VarLocal */router),
								/* VarLocal */req)));
				}
			}
		}
	});
// end Cycle updateReqs
// Define, deps=$elm$http$Http$State, $elm$core$Task$andThen, $elm$core$Basics$apR, $elm$core$Task$succeed, $elm$http$Http$updateReqs
var $elm$http$Http$onEffects = /* Function */F4(
	function (router, cmds, subs, state) {
		return /* Call *//* Call */A2(
			$elm$core$Task$andThen,
			/* Function */function (reqs) {
				return /* Call */$elm$core$Task$succeed(
					/* Call */A2(/* VarGlobal */$elm$http$Http$State, /* VarLocal */reqs, /* VarLocal */subs));
			},
			/* Call */A3(/* VarGlobal */$elm$http$Http$updateReqs, /* VarLocal */router, /* VarLocal */cmds, /* Access *//* VarLocal */state.reqs));
	});
// Define, deps=$elm$core$Maybe$Just, $elm$core$Maybe$Nothing, $elm$core$Basics$eq, $elm$core$Platform$sendToApp
var $elm$http$Http$maybeSend = /* Function */F4(
	function (router, desiredTracker, progress, _v0) {
		// Destruct
		var actualTracker = _v0.a;
		// Destruct
		var toMsg = _v0.b;
		return /* If *//* Call */_Utils_eq(/* VarLocal */desiredTracker, /* VarLocal */actualTracker) ? /* Call */$elm$core$Maybe$Just(
			/* Call */A2(
				$elm$core$Platform$sendToApp,
				/* VarLocal */router,
				/* Call *//* VarLocal */toMsg(/* VarLocal */progress))) : /* VarGlobal */$elm$core$Maybe$Nothing;
	});
// Define, deps=$elm$core$Task$andThen, $elm$core$Basics$apR, $elm$core$List$filterMap, $elm$http$Http$maybeSend, $elm$core$Task$sequence, $elm$core$Task$succeed
var $elm$http$Http$onSelfMsg = /* Function */F3(
	function (router, _v0, state) {
		// Destruct
		var tracker = _v0.a;
		// Destruct
		var progress = _v0.b;
		return /* Call *//* Call */A2(
			$elm$core$Task$andThen,
			/* Function */function (_v1) {
				return /* Call */$elm$core$Task$succeed(/* VarLocal */state);
			},
			/* Call */$elm$core$Task$sequence(
				/* Call */A2(
					$elm$core$List$filterMap,
					/* Call */A3(/* VarGlobal */$elm$http$Http$maybeSend, /* VarLocal */router, /* VarLocal */tracker, /* VarLocal */progress),
					/* Access *//* VarLocal */state.subs)));
	});
// Ctor
var $elm$http$Http$Cancel = function (a) {
	return {$: 'Cancel', a: a};
};
// Define, deps=$elm$kernel$Http$$, $elm$http$Http$Cancel, $elm$http$Http$Request
var $elm$http$Http$cmdMap = /* Function */F2(
	function (func, cmd) {
		// Case
		// Decider Chain
		if (cmd.$ === 'Cancel') {
			// Decider Leaf Inline
			// Destruct
			var tracker = cmd.a;
			return /* Call *//* VarGlobal */$elm$http$Http$Cancel(/* VarLocal */tracker);
		} else {
			// Decider Leaf Inline
			// Destruct
			var r = cmd.a;
			return /* Call *//* VarGlobal */$elm$http$Http$Request(
				/* Record */{
					allowCookiesFromOtherDomains: /* Access *//* VarLocal */r.allowCookiesFromOtherDomains,
					body: /* Access *//* VarLocal */r.body,
					expect: /* Call */A2(/* VarKernel */_Http_mapExpect, /* VarLocal */func, /* Access *//* VarLocal */r.expect),
					headers: /* Access *//* VarLocal */r.headers,
					method: /* Access *//* VarLocal */r.method,
					timeout: /* Access *//* VarLocal */r.timeout,
					tracker: /* Access *//* VarLocal */r.tracker,
					url: /* Access *//* VarLocal */r.url
				});
		}
	});
// Ctor
var $elm$http$Http$MySub = F2(
	function (a, b) {
		return {$: 'MySub', a: a, b: b};
	});
// Define, deps=$elm$http$Http$MySub, $elm$core$Basics$composeR
var $elm$http$Http$subMap = /* Function */F2(
	function (func, _v0) {
		// Destruct
		var tracker = _v0.a;
		// Destruct
		var toMsg = _v0.b;
		return /* Call */A2(
			/* VarGlobal */$elm$http$Http$MySub,
			/* VarLocal */tracker,
			/* Call */A2($elm$core$Basics$composeR, /* VarLocal */toMsg, /* VarLocal */func));
	});
_Platform_effectManagers['Http'] = _Platform_createManager($elm$http$Http$init, $elm$http$Http$onEffects, $elm$http$Http$onSelfMsg, $elm$http$Http$cmdMap, $elm$http$Http$subMap);
var $elm$http$Http$command = _Platform_leaf('Http');
var $elm$http$Http$subscription = _Platform_leaf('Http');
// End Manager $elm$http$Http$$fx$
// Define, deps=$elm$core$Basics$False, $elm$http$Http$Request, $elm$core$Basics$apL, $elm$http$Http$command
var $elm$http$Http$request = /* Function */function (r) {
	return /* Call *//* Call *//* VarGlobal */$elm$http$Http$command(
		/* Call *//* Call *//* VarGlobal */$elm$http$Http$Request(
			/* Record */{allowCookiesFromOtherDomains: /* Bool */false, body: /* Access *//* VarLocal */r.body, expect: /* Access *//* VarLocal */r.expect, headers: /* Access *//* VarLocal */r.headers, method: /* Access *//* VarLocal */r.method, timeout: /* Access *//* VarLocal */r.timeout, tracker: /* Access *//* VarLocal */r.tracker, url: /* Access *//* VarLocal */r.url}));
};
// Define, deps=$elm$kernel$List$$, $elm$core$Maybe$Nothing, $elm$http$Http$emptyBody, $elm$http$Http$request
var $elm$http$Http$get = /* Function */function (r) {
	return /* Call *//* VarGlobal */$elm$http$Http$request(
		/* Record */{body: /* VarGlobal */$elm$http$Http$emptyBody, expect: /* Access *//* VarLocal */r.expect, headers: /* List */_List_Nil, method: /* Str */'GET', timeout: /* VarGlobal */$elm$core$Maybe$Nothing, tracker: /* VarGlobal */$elm$core$Maybe$Nothing, url: /* Access *//* VarLocal */r.url});
};
// Define, deps=$author$project$Main$JsonLoaded, $elm$http$Http$expectJson, $elm$http$Http$get, $elm$json$Json$Decode$string
var $author$project$Main$getJson = /* Call *//* VarGlobal */$elm$http$Http$get(
	/* Record */{
		expect: /* Call */A2(/* VarGlobal */$elm$http$Http$expectJson, /* VarGlobal */$author$project$Main$JsonLoaded, /* VarGlobal */$elm$json$Json$Decode$string),
		url: /* Str */'./assets/data.json'
	});
// Define, deps=$elm$kernel$Utils$$, $author$project$Main$getJson, $elm$core$Platform$Cmd$none
var $author$project$Main$update = /* Function */F2(
	function (msg, model) {
		// Case
		// Decider Fanout
		switch (msg.$) {
			case 'ButtonClicked':
				// Decider Leaf Inline
				return /* Tuple */_Utils_Tuple2(
					/* Update */_Utils_update(
						/* VarLocal */model,
						{text: /* Str */'Loading...'}),
					/* VarGlobal */$author$project$Main$getJson);
			case 'KeyPressed':
				// Decider Leaf Inline
				// Destruct
				var key = msg.a;
				return /* Tuple */_Utils_Tuple2(
					/* Update */_Utils_update(
						/* VarLocal */model,
						{text: /* VarLocal */key}),
					/* VarGlobal */$elm$core$Platform$Cmd$none);
			default:
				// Decider Chain
				if (msg.a.$ === 'Ok') {
					// Decider Leaf Inline
					// Destruct
					var jsonString = msg.a.a;
					return /* Tuple */_Utils_Tuple2(
						/* Update */_Utils_update(
							/* VarLocal */model,
							{text: /* VarLocal */jsonString}),
						/* VarGlobal */$elm$core$Platform$Cmd$none);
				} else {
					// Decider Leaf Inline
					return /* Tuple */_Utils_Tuple2(
						/* Update */_Utils_update(
							/* VarLocal */model,
							{text: /* Str */'Error!'}),
						/* VarGlobal */$elm$core$Platform$Cmd$none);
				}
		}
	});
// Ctor
var $author$project$Main$ButtonClicked = {$: 'ButtonClicked'};
// Define, deps=$elm$kernel$VirtualDom$$
var $elm$html$Html$button = /* Call *//* VarKernel */_VirtualDom_node(/* Str */'button');
// Define, deps=$elm$kernel$Json$$
var $elm$json$Json$Encode$string = /* VarKernel */_Json_wrap;
// Define, deps=$elm$kernel$VirtualDom$$, $elm$json$Json$Encode$string
var $elm$html$Html$Attributes$stringProperty = /* Function */F2(
	function (key, string) {
		return /* Call */A2(
			/* VarKernel */_VirtualDom_property,
			/* VarLocal */key,
			/* Call *//* VarGlobal */$elm$json$Json$Encode$string(/* VarLocal */string));
	});
// Define, deps=$elm$html$Html$Attributes$stringProperty
var $elm$html$Html$Attributes$class = /* Call *//* VarGlobal */$elm$html$Html$Attributes$stringProperty(/* Str */'className');
// Define, deps=$elm$kernel$VirtualDom$$
var $elm$html$Html$div = /* Call *//* VarKernel */_VirtualDom_node(/* Str */'div');
// Define, deps=$elm$kernel$VirtualDom$$
var $elm$html$Html$li = /* Call *//* VarKernel */_VirtualDom_node(/* Str */'li');
// Ctor
var $elm$virtual_dom$VirtualDom$Normal = function (a) {
	return {$: 'Normal', a: a};
};
// Define, deps=$elm$kernel$VirtualDom$$
var $elm$virtual_dom$VirtualDom$on = /* VarKernel */_VirtualDom_on;
// Define, deps=$elm$virtual_dom$VirtualDom$Normal, $elm$virtual_dom$VirtualDom$on
var $elm$html$Html$Events$on = /* Function */F2(
	function (event, decoder) {
		return /* Call */A2(
			/* VarGlobal */$elm$virtual_dom$VirtualDom$on,
			/* VarLocal */event,
			/* Call *//* VarGlobal */$elm$virtual_dom$VirtualDom$Normal(/* VarLocal */decoder));
	});
// Define, deps=$elm$html$Html$Events$on, $elm$json$Json$Decode$succeed
var $elm$html$Html$Events$onClick = /* Function */function (msg) {
	return /* Call */A2(
		/* VarGlobal */$elm$html$Html$Events$on,
		/* Str */'click',
		/* Call *//* VarGlobal */$elm$json$Json$Decode$succeed(/* VarLocal */msg));
};
// Define, deps=$elm$kernel$VirtualDom$$
var $elm$virtual_dom$VirtualDom$text = /* VarKernel */_VirtualDom_text;
// Define, deps=$elm$virtual_dom$VirtualDom$text
var $elm$html$Html$text = /* VarGlobal */$elm$virtual_dom$VirtualDom$text;
// Define, deps=$elm$kernel$VirtualDom$$
var $elm$html$Html$ul = /* Call *//* VarKernel */_VirtualDom_node(/* Str */'ul');
// Define, deps=$elm$kernel$List$$, $author$project$Main$ButtonClicked, $elm$html$Html$button, $elm$html$Html$Attributes$class, $elm$html$Html$div, $elm$html$Html$li, $elm$html$Html$Events$onClick, $elm$html$Html$text, $elm$html$Html$ul
var $author$project$Main$view = /* Function */function (model) {
	return /* Call */A2(
		/* VarGlobal */$elm$html$Html$div,
		/* List */_List_Nil,
		/* List */_List_fromArray(
			[
				/* Call */A2(
				/* VarGlobal */$elm$html$Html$ul,
				/* List */_List_Nil,
				/* List */_List_fromArray(
					[
						/* Call */A2(
						/* VarGlobal */$elm$html$Html$li,
						/* List */_List_Nil,
						/* List */_List_fromArray(
							[
								/* Call *//* VarGlobal */$elm$html$Html$text(/* Str */'Load a local JSON file over HTTP by either clicking the button or pressing any key')
							])),
						/* Call */A2(
						/* VarGlobal */$elm$html$Html$li,
						/* List */_List_Nil,
						/* List */_List_fromArray(
							[
								/* Call *//* VarGlobal */$elm$html$Html$text(/* Str */'This app has 3 effect managers: Http (Cmd & Sub), Task (Cmd only), and Browser.Events (Sub only)')
							]))
					])),
				/* Call */A2(
				/* VarGlobal */$elm$html$Html$div,
				/* List */_List_fromArray(
					[
						/* Call *//* VarGlobal */$elm$html$Html$Attributes$class(/* Str */'effects-test')
					]),
				/* List */_List_fromArray(
					[
						/* Call */A2(
						/* VarGlobal */$elm$html$Html$button,
						/* List */_List_fromArray(
							[
								/* Call *//* VarGlobal */$elm$html$Html$Events$onClick(/* VarGlobal */$author$project$Main$ButtonClicked)
							]),
						/* List */_List_fromArray(
							[
								/* Call *//* VarGlobal */$elm$html$Html$text(/* Str */'LOAD')
							])),
						/* Call */A2(
						/* VarGlobal */$elm$html$Html$div,
						/* List */_List_Nil,
						/* List */_List_fromArray(
							[
								/* Call *//* VarGlobal */$elm$html$Html$text(/* Access *//* VarLocal */model.text)
							]))
					]))
			]));
};
// Define, deps=$elm$kernel$Utils$$, $author$project$Main$KeyPressed, $elm$core$Basics$apL, $elm$browser$Browser$element, $elm$json$Json$Decode$field, $author$project$Main$initialModel, $author$project$WebAssembly$intercept, $elm$json$Json$Decode$map, $elm$core$Platform$Cmd$none, $elm$browser$Browser$Events$onKeyDown, $elm$json$Json$Decode$string, $elm$json$Json$Decode$succeed, $author$project$Main$update, $author$project$Main$view
var $author$project$Main$main = /* Call *//* Call *//* VarGlobal */$elm$browser$Browser$element(
	/* Call *//* VarGlobal */$author$project$WebAssembly$intercept(
		/* Record */{
			init: /* Function */function (_v0) {
				return /* Tuple */_Utils_Tuple2(/* VarGlobal */$author$project$Main$initialModel, /* VarGlobal */$elm$core$Platform$Cmd$none);
			},
			subscriptions: /* Function */function (_v1) {
				return /* Call *//* VarGlobal */$elm$browser$Browser$Events$onKeyDown(
					/* Call */A2(
						/* VarGlobal */$elm$json$Json$Decode$map,
						/* VarGlobal */$author$project$Main$KeyPressed,
						/* Call */A2(/* VarGlobal */$elm$json$Json$Decode$field, /* Str */'key', /* VarGlobal */$elm$json$Json$Decode$string)));
			},
			update: /* VarGlobal */$author$project$Main$update,
			view: /* VarGlobal */$author$project$Main$view
		}));
_Platform_export({'Main':{'init':$author$project$Main$main(
	/* Call *//* VarGlobal */$elm$json$Json$Decode$succeed(/* Unit */_Utils_Tuple0))(0)}});}(this));