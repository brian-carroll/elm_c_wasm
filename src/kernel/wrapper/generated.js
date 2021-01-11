// Simulate compiler pre-processing of Kernel JavaScript
const __2_SELF = 0;
const __2_LEAF = 1;
const __2_NODE = 2;
const __2_MAP = 3;

const __home = 'k';
const __value = 'l';
const __bags = 'm';
const __func = 'n';
const __bag = 'o';

const TAG_MASK = 0xf0000000;
const TAG_SHIFT = 28;
const SIZE_MASK = 0x0fffffff;
const SIZE_SHIFT = 0;

function F(arity, fun, wrapper) {
  wrapper.a = arity;
  wrapper.f = fun;
  return wrapper;
}

function F2(fun) {
  return F(2, fun, function (a) {
    return function (b) {
      return fun(a, b);
    };
  });
}

// Called from within effects modules, exposed as Platform.sendToSelf
const _Platform_sendToSelf = F2(function (router, msg) {
  return A2(__Scheduler_send, router.__selfProcess, {
    $: __2_SELF,
    a: msg
  });
});

// Called from within effects modules
function _Platform_leaf(home) {
  return function (value) {
    return {
      $: __2_LEAF,
      [__home]: home,
      [__value]: value
    };
  };
}

// Called from user code as Platform.Cmd.batch or Sub.batch
function _Platform_batch(list) {
  return {
    $: __2_NODE,
    [__bags]: list
  };
}

// Called from user code as Platform.Cmd.map or Sub.map
const _Platform_map = F2(function (tagger, bag) {
  return {
    $: __2_MAP,
    [__func]: tagger,
    [__bag]: bag
  };
});

function _Wasm_decodePlatformEffects(addr8) {
  const index32 = addr8 >> 2;
  const ctor = _Wasm_mem32[index32 + 1];
  switch (ctor) {
    case 0:
      return _Wasm_decodePlatformSelf(addr8);
    case 1:
      return _Wasm_decodePlatformLeaf(addr8);
    case 2:
      return _Wasm_decodePlatformNode(addr8);
    case 3:
      return _Wasm_decodePlatformMap(addr8);
    default:
      throw new Error('Corrupt WebAssembly memory at 0x' + addr8.toString(16));
  }
}

function _Wasm_decodePlatformLeaf(addr8) {
  const index32 = addr8 >> 2;
  const effectTypeEnum = _Wasm_mem32[index32 + 2];
  const valueAddr = _Wasm_mem32[index32 + 3];
  switch (effectTypeEnum) {
    case 0:
      return $elm$core$Task$command(
        decodeCustom$elm$http$Task$MyCmd(valueAddr)
      );
    case 1:
      return $elm$http$Http$command(
        decodeCustom$elm$http$Http$MyCmd(valueAddr)
      );
    case 2:
      return $elm$http$Http$subscription(
        decodeCustom$elm$http$Http$MySub(valueAddr)
      );
    default:
      throw new Error('Corrupt WebAssembly memory at 0x' + addr8.toString(16));
  }
}

function _Wasm_decodeCustom$elm$http$Http$MyCmd(addr8) {
  const index32 = addr8 >> 2;
  const ctor = _Wasm_mem32[index32 + 1];
  switch (ctor) {
    case 0:
      return $elm$http$Http$Cancel(decodeString(_Wasm_mem32[index32 + 2]));
    case 1:
      return $elm$http$Http$Request(
        decodeRecord_allowCookiesFromOtherDomains_body_expect_headers_method_timeout_tracker_url(
          _Wasm_mem32[index32 + 2]
        )
      );
    default:
      throw new Error('Corrupt WebAssembly memory at 0x' + addr8.toString(16));
  }
}

// Can't assume a fieldgroup uniquely defines a decoder
// Add a numeric index on the end as a sort of hash of the field types
function _Wasm_decodeRecord_allowCookiesFromOtherDomains_body_expect_headers_method_timeout_tracker_url(
  addr8
) {
  const index32 = addr8 >> 2;
  return {
    allowCookiesFromOtherDomains: _Wasm_decodeBool(_Wasm_mem32[index32 + 3]),
    body: _Wasm_decodeBody(_Wasm_mem32[index32 + 4]),
    expect: _Wasm_decodeExpect(decodeAny, _Wasm_mem32[index32 + 5]),
    headers: _Wasm_decodeList(decodeHeader, _Wasm_mem32[index32 + 6]),
    method: _Wasm_decodeString(_Wasm_mem32[index32 + 7]),
    timeout: _Wasm_decodeCustom$elm$core$Maybe$Maybe(
      decodeFloat,
      _Wasm_mem32[index32 + 8]
    ),
    tracker: _Wasm_decodeCustom$elm$core$Maybe$Maybe(
      decodeString,
      _Wasm_mem32[index32 + 9]
    ),
    url: _Wasm_decodeString(_Wasm_mem32[index32 + 10])
  };
}

function _Wasm_decodeAny(addr8) {
  const decoder = _Wasm_decodeDetectType(addr8);
  return decoder(addr8);
}

function _Wasm_decodeDetectType(addr8) {
  const index32 = addr8 >> 2;
  const header = _Wasm_mem32[index32];
  const tag = (header & TAG_MASK) >> TAG_SHIFT;
  switch (tag) {
    case 0:
      return _Wasm_decodeInt;
    case 1:
      return _Wasm_decodeFloat;
    case 2:
      return _Wasm_decodeChar;
    case 3:
      return _Wasm_decodeString;
    case 4: {
      const headAddr8 = _Wasm_mem32[index32 + 1];
      const itemDecoder = _Wasm_decodeDetectType(headAddr8);
      return _Wasm_decodeList(itemDecoder);
    }
    case 5: {
      const aDecoder = _Wasm_decodeDetectType(_Wasm_mem32[index32 + 1]);
      const bDecoder = _Wasm_decodeDetectType(_Wasm_mem32[index32 + 2]);
      return _Wasm_decodeTuple2(aDecoder, bDecoder);
    }
    case 6: {
      const aDecoder = _Wasm_decodeDetectType(_Wasm_mem32[index32 + 1]);
      const bDecoder = _Wasm_decodeDetectType(_Wasm_mem32[index32 + 2]);
      const cDecoder = _Wasm_decodeDetectType(_Wasm_mem32[index32 + 3]);
      return _Wasm_decodeTuple3(aDecoder, bDecoder, cDecoder);
    }
    case 7:
      return _Wasm_detectCustomType(addr8); // generated for all app Custom types
    case 8:
      return _Wasm_detectRecordType(addr8); // generated for all app Record types
    case 9:
      return _Wasm_decodeFieldGroup;
    case 10:
      return _Wasm_decodeClosure;
    case 11:
      return _Wasm_decodeJsRef;
    default:
      throw new Error('Corrupt WebAssembly memory at 0x' + addr8.toString(16));
  }
}

function _Wasm_decodeInt(addr8) {
  const index32 = addr8 >> 2;
  return _Wasm_mem32[index32 + 1];
}

function _Wasm_decodeFloat(addr8) {
  return _Wasm_exports.readF64(addr8);
}

const textDecoder = new TextDecoder('utf-16le');

function _Wasm_decodeChar(addr8) {
  return _Utils_chr(decodeString(addr8));
}

function _Wasm_decodeString(addr8) {
  const index32 = addr8 >> 2;
  const size = _Wasm_mem32[index32] && SIZE_MASK;
  const index16 = (index32 + 1) << 1;
  let len16 = (size - 1) << 1;
  if (_Wasm_mem16[index16 + len16 - 1] === 0) len16--;
  const words16 = _Wasm_mem16.slice(index16, index16 + len16);
  return textDecoder.decode(words16);
}

function _Wasm_decodeTuple2(aDecoder, bDecoder) {
  return function (addr8) {
    const index32 = addr8 >> 2;
    const a = aDecoder(_Wasm_mem32[index32 + 1]);
    const b = bDecoder(_Wasm_mem32[index32 + 2]);
    return _Utils_Tuple2(a, b);
  };
}

function _Wasm_decodeTuple3(aDecoder, bDecoder, cDecoder) {
  return function (addr8) {
    const index32 = addr8 >> 2;
    const a = aDecoder(_Wasm_mem32[index32 + 1]);
    const b = bDecoder(_Wasm_mem32[index32 + 2]);
    const c = cDecoder(_Wasm_mem32[index32 + 3]);
    return _Utils_Tuple3(a, b, c);
  };
}

function _Wasm_decodeList(itemDecoder) {
  return function (listAddr8) {
    const nilAddr8 = _Wasm_exports.getNil();
    const tmp = _List_Cons(undefined, _List_Nil);
    let end = tmp;

    while (listAddr8 !== nilAddr8) {
      const listIndex32 = listAddr8 >> 2;
      const headAddr8 = _Wasm_mem32[listIndex32 + 1];
      const tailAddr8 = _Wasm_mem32[listIndex32 + 2];
      const jsValue = itemDecoder(headAddr8);
      end.b = _List_Cons(jsValue, _List_Nil);
      end = end.b;
      listAddr8 = tailAddr8;
    }

    return tmp.b;
  };
}

// isAsciiCode : Int -> Int -> String -> Bool
var _Parser_isAsciiCode = F3(function (code, offset, string) {
  return string.charCodeAt(offset) === code;
});

// isSubChar : (Char -> Bool) -> Int -> String -> Int
var _Parser_isSubChar = F3(function (predicate, offset, string) {
  return string.length <= offset
    ? -1
    : (string.charCodeAt(offset) & 0xf800) === 0xd800
    ? predicate(__Utils_chr(string.substr(offset, 2)))
      ? offset + 2
      : -1
    : predicate(__Utils_chr(string[offset]))
    ? string[offset] === '\n'
      ? -2
      : offset + 1
    : -1;
});

function _Wasm_callJsKernel(kernelFnIndex, argsAddr) {
  const argsIndex32 = argsAddr >> 2;
  switch (kernelFnIndex) {
    case 0:
      // isAsciiCode : Int -> Int -> String -> Bool
      return _Wasm_encode(
        _Wasm_encodeBool,
        _Parser_isAsciiCode(
          _Wasm_decodeInt(_Wasm_mem32[argsIndex32]),
          _Wasm_decodeInt(_Wasm_mem32[argsIndex32 + 1]),
          _Wasm_decodeString(_Wasm_mem32[argsIndex32 + 2])
        )
      );

    case 1:
      // isSubChar : (Char -> Bool) -> Int -> String -> Int
      return _Wasm_encode(
        _Wasm_encodeInt,
        _Parser_isSubChar(
          _Wasm_decodeClosure(
            [_Wasm_encodeChar],
            _Wasm_decodeBool
          )(_Wasm_mem32[argsIndex32]),
          _Wasm_decodeInt(_Wasm_mem32[argsIndex32 + 1]),
          _Wasm_decodeString(_Wasm_mem32[argsIndex32 + 2])
        )
      );

    default:
      throw new Error('Unknown JS kernel function ' + kernelFnIndex);
  }
}

let _Wasm_maxIndex32;
let _Wasm_maxIndex16;
const _Wasm_heapOverflow = new Error('Wasm heap overflow');

let _Wasm_writeIndex32 = 0;
function _Wasm_write32(words) {
  const addr8 = _Wasm_writeIndex32 >> 2;
  const nextIndex = _Wasm_writeIndex32 + words.length;
  if (nextIndex > _Wasm_maxIndex32) {
    throw _Wasm_heapOverflow;
  }
  for (; _Wasm_writeIndex32 < nextIndex; _Wasm_writeIndex32++) {
    _Wasm_mem32[_Wasm_writeIndex32] = word32;
  }
  return addr8;
}

function _Wasm_encode(encoder, jsValue) {
  for (let attempts = 0; attempts < 2; attempts++) {
    try {
      const startAddr = wasmExports.getWriteAddr();
      const maxAddr = wasmExports.getMaxWriteAddr();
      _Wasm_writeIndex32 = startAddr >> 2;
      _Wasm_maxIndex16 = maxAddr >> 1;
      _Wasm_maxIndex32 = maxAddr >> 2;
      const addr8 = encoder(jsValue);
      wasmExports.finishWritingAt(_Wasm_writeIndex32 << 2);
      return addr8;
    } catch (e) {
      if (e === heapOverflowError) {
        wasmExports.collectGarbage();
      } else {
        console.error(e);
        throw e;
      }
    }
  }
  throw new Error('Failed to write to Wasm');
}

function _Wasm_encodeBool(jsValue) {
  return jsValue ? _Wasm_exports.getTrue() : _Wasm_exports.getFalse();
}

function _Wasm_Header(tag, size) {
  return (tag << TAG_SHIFT) | (size << SIZE_SHIFT);
}

function _Wasm_encodeInt(jsValue) {
  return _Wasm_write32([_Wasm_Header(Tag.Int, 2), jsValue]);
}

function _Wasm_encodeList(itemEncoder) {
  return function (list) {
    const array = _List_toArray(list);
    const header = _Wasm_Header(Tag.List, 3);
    let head = _Wasm_exports.getNil();
    for (let i = array.length - 1; i >= 0; i--) {
      head = _Wasm_write32([header, itemEncoder(array[i]), head]);
    }
    return head;
  };
}
