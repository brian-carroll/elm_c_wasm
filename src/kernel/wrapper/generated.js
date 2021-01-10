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

function wasmDecodePlatformEffects(addr8) {
  const index32 = addr8 >> 2;
  const ctor = _Utils_mem32[index32 + 1];
  switch (ctor) {
    case 0:
      return decodePlatformSelf(addr8);
    case 1:
      return decodePlatformLeaf(addr8);
    case 2:
      return decodePlatformNode(addr8);
    case 3:
      return decodePlatformMap(addr8);
    default:
      throw new Error('Corrupt WebAssembly memory at 0x' + addr8.toString(16));
  }
}

function wasmDecodePlatformLeaf(addr8) {
  const index32 = addr8 >> 2;
  const effectTypeEnum = _Utils_mem32[index32 + 2];
  const valueAddr = _Utils_mem32[index32 + 3];
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

function wasmDecodeCustom$elm$http$Http$MyCmd(addr8) {
  const index32 = addr8 >> 2;
  const ctor = _Utils_mem32[index32 + 1];
  switch (ctor) {
    case 0:
      return $elm$http$Http$Cancel(decodeString(_Utils_mem32[index32 + 2]));
    case 1:
      return $elm$http$Http$Request(
        decodeRecord_allowCookiesFromOtherDomains_body_expect_headers_method_timeout_tracker_url(
          _Utils_mem32[index32 + 2]
        )
      );
    default:
      throw new Error('Corrupt WebAssembly memory at 0x' + addr8.toString(16));
  }
}

// Can't assume a fieldgroup uniquely defines a decoder
// Add a numeric index on the end as a sort of hash of the field types
function wasmDecodeRecord_allowCookiesFromOtherDomains_body_expect_headers_method_timeout_tracker_url(
  addr8
) {
  const index32 = addr8 >> 2;
  return {
    allowCookiesFromOtherDomains: decodeBool(_Utils_mem32[index32 + 3]),
    body: decodeBody(_Utils_mem32[index32 + 4]),
    expect: decodeExpect(decodeAny, _Utils_mem32[index32 + 5]),
    headers: decodeList(decodeHeader, _Utils_mem32[index32 + 6]),
    method: decodeString(_Utils_mem32[index32 + 7]),
    timeout: decodeCustom$elm$core$Maybe$Maybe(
      decodeFloat,
      _Utils_mem32[index32 + 8]
    ),
    tracker: decodeCustom$elm$core$Maybe$Maybe(
      decodeString,
      _Utils_mem32[index32 + 9]
    ),
    url: decodeString(_Utils_mem32[index32 + 10])
  };
}

function wasmDecodeAny(addr8) {
  const decoder = wasmDecodeDetectType(addr8);
  return decoder(addr8);
}

function wasmDecodeDetectType(addr8) {
  const index32 = addr8 >> 2;
  const header = _Utils_mem32[index32];
  const tag = (header & TAG_MASK) >> TAG_SHIFT;
  switch (tag) {
    case 0:
      return decodeInt;
    case 1:
      return decodeFloat;
    case 2:
      return decodeChar;
    case 3:
      return decodeString;
    case 4: {
      const headAddr8 = _Utils_mem32[index32 + 1];
      const itemDecoder = wasmDecodeDetectType(headAddr8);
      return decodeList(itemDecoder);
    }
    case 5: {
      const aDecoder = wasmDecodeDetectType(_Utils_mem32[index32 + 1]);
      const bDecoder = wasmDecodeDetectType(_Utils_mem32[index32 + 2]);
      return decodeTuple2(aDecoder, bDecoder);
    }
    case 6: {
      const aDecoder = wasmDecodeDetectType(_Utils_mem32[index32 + 1]);
      const bDecoder = wasmDecodeDetectType(_Utils_mem32[index32 + 2]);
      const cDecoder = wasmDecodeDetectType(_Utils_mem32[index32 + 3]);
      return decodeTuple3(aDecoder, bDecoder, cDecoder);
    }
    case 7:
      return detectCustomTypeDecoder(addr8); // generated for all app Custom types
    case 8:
      return detectRecordTypeDecoder(addr8); // generated for all app Record types
    case 9:
      return decodeFieldGroup;
    case 10:
      return decodeClosure;
    case 11:
      return decodeJsRef;
    default:
      throw new Error('Corrupt WebAssembly memory at 0x' + addr8.toString(16));
  }
}

function wasmDecodeInt(addr8) {
  const index32 = addr8 >> 2;
  return _Utils_mem32[index32 + 1];
}

function wasmDecodeFloat(addr8) {
  return wasmExports.readF64(addr8);
}

const textDecoder = new TextDecoder('utf-16le');

function wasmDecodeChar(addr8) {
  return _Utils_chr(decodeString(addr8));
}

function wasmDecodeString(addr8) {
  const index32 = addr8 >> 2;
  const size = _Utils_mem32[index32] && SIZE_MASK;
  const index16 = (index32 + 1) << 1;
  let len16 = (size - 1) << 1;
  if (_Utils_mem16[index16 + len16 - 1] === 0) len16--;
  const words16 = _Utils_mem16.slice(index16, index16 + len16);
  return textDecoder.decode(words16);
}

function wasmDecodeTuple2(aDecoder, bDecoder) {
  return function (addr8) {
    const index32 = addr8 >> 2;
    const a = aDecoder(_Utils_mem32[index32 + 1]);
    const b = bDecoder(_Utils_mem32[index32 + 2]);
    return _Utils_Tuple2(a, b);
  };
}

function wasmDecodeTuple3(aDecoder, bDecoder, cDecoder) {
  return function (addr8) {
    const index32 = addr8 >> 2;
    const a = aDecoder(_Utils_mem32[index32 + 1]);
    const b = bDecoder(_Utils_mem32[index32 + 2]);
    const c = cDecoder(_Utils_mem32[index32 + 3]);
    return _Utils_Tuple3(a, b, c);
  };
}

function wasmDecodeList(itemDecoder) {
  return function (listAddr8) {
    const nilAddr8 = wasmExports.getNil();
    const tmp = _List_Cons(undefined, _List_Nil);
    let end = tmp;

    while (listAddr8 !== nilAddr8) {
      const listIndex32 = listAddr8 >> 2;
      const headAddr8 = _Utils_mem32[listIndex32 + 1];
      const tailAddr8 = _Utils_mem32[listIndex32 + 2];
      const jsValue = itemDecoder(headAddr8);
      end.b = _List_Cons(jsValue, _List_Nil);
      end = end.b;
      listAddr8 = tailAddr8;
    }

    return tmp.b;
  };
}
