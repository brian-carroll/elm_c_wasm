import {
  _Wasm_mem_i32,
  _Wasm_mem_u16,
  _Wasm_decodeAny,
  _Wasm_decodeInt,
  _Wasm_decodeFloat,
  _Wasm_decodeBool,
  _Wasm_decodeString,
  _Wasm_decodeList,
  _Wasm_decodeClosure,
  _Wasm_handleWrite,
  _Wasm_encodeBool,
  _Wasm_encodeInt,
  _Wasm_encodeChar,
  WasmDecoder,
  ElmCurriedFunction
} from './Wasm';

/*
 * Generated by standard compiler
 */
declare function F3(fun: Function): ElmCurriedFunction;

declare var $elm$core$Task$command: (value: any) => object;

declare var $elm$http$Http$command: (value: any) => object;
declare var $elm$http$Http$subscription: (value: any) => object;
declare var $elm$http$Http$Cancel: (value: any) => any;
declare var $elm$http$Http$Request: (value: any) => any;

declare function __Utils_chr(s: string): String;

/*
 * Wasm not written yet
 */
export declare var _Wasm_detectCustomType: (
  addr: number
) => (addr: number) => object;
export declare var _Wasm_detectRecordType: (
  addr: number
) => (addr: number) => object;

/*
 * `Maybe` not written yet
 */
declare var _Wasm_decode$elm$core$Maybe$Maybe: (
  inner: WasmDecoder
) => WasmDecoder;

/*
 * `Parser` not written yet
 */

/*
 * Http not written yet
 */
declare function _Wasm_decode$elm$core$Task$MyCmd(addr: number): any;
declare function _Wasm_decode$elm$http$Http$MySub(addr: number): any;
declare function _Wasm_decode$elm$http$Http$Body(addr: number): any;
declare function _Wasm_decode$elm$http$Http$Expect(
  typeVarDecoder: Function,
  addr: number
): any;
declare function _Wasm_decode$elm$http$Http$Header(addr: number): any;

export var _Wasm_decodePlatformLeaf = function (addr8: number): object {
  const index32 = addr8 >> 2;
  const effectTypeEnum = _Wasm_mem_i32[index32 + 2];
  const valueAddr = _Wasm_mem_i32[index32 + 3];
  switch (effectTypeEnum) {
    case 0:
      return $elm$core$Task$command(
        _Wasm_decode$elm$core$Task$MyCmd(valueAddr)
      );
    case 1:
      return $elm$http$Http$command(
        _Wasm_decode$elm$http$Http$MyCmd(valueAddr)
      );
    case 2:
      return $elm$http$Http$subscription(
        _Wasm_decode$elm$http$Http$MySub(valueAddr)
      );
    default:
      throw new Error('Corrupt WebAssembly memory at 0x' + addr8.toString(16));
  }
};

function _Wasm_decode$elm$http$Http$MyCmd(addr8) {
  const index32 = addr8 >> 2;
  const ctor = _Wasm_mem_i32[index32 + 1];
  switch (ctor) {
    case 0:
      return $elm$http$Http$Cancel(
        _Wasm_decodeString(_Wasm_mem_i32[index32 + 2])
      );
    case 1:
      return $elm$http$Http$Request(
        _Wasm_decodeRecord_allowCookiesFromOtherDomains_body_expect_headers_method_timeout_tracker_url(
          _Wasm_mem_i32[index32 + 2]
        )
      );
    default:
      throw new Error('Corrupt WebAssembly memory at 0x' + addr8.toString(16));
  }
}

// Can't assume a fieldgroup uniquely defines a _Wasm_decoder
// Add a numeric index on the end as a sort of hash of the field types
function _Wasm_decodeRecord_allowCookiesFromOtherDomains_body_expect_headers_method_timeout_tracker_url(
  addr8
) {
  const index32 = addr8 >> 2;
  return {
    allowCookiesFromOtherDomains: _Wasm_decodeBool(_Wasm_mem_i32[index32 + 3]),
    body: _Wasm_decode$elm$http$Http$Body(_Wasm_mem_i32[index32 + 4]),
    expect: _Wasm_decode$elm$http$Http$Expect(
      _Wasm_decodeAny,
      _Wasm_mem_i32[index32 + 5]
    ),
    headers: _Wasm_decodeList(_Wasm_decode$elm$http$Http$Header)(
      _Wasm_mem_i32[index32 + 6]
    ),
    method: _Wasm_decodeString(_Wasm_mem_i32[index32 + 7]),
    timeout: _Wasm_decode$elm$core$Maybe$Maybe(_Wasm_decodeFloat)(
      _Wasm_mem_i32[index32 + 8]
    ),
    tracker: _Wasm_decode$elm$core$Maybe$Maybe(_Wasm_decodeString)(
      _Wasm_mem_i32[index32 + 9]
    ),
    url: _Wasm_decodeString(_Wasm_mem_i32[index32 + 10])
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

export function _Wasm_callJsKernel(kernelFnIndex: number, argsAddr: number) {
  const argsIndex32 = argsAddr >> 2;
  switch (kernelFnIndex) {
    case 0:
      // isAsciiCode : Int -> Int -> String -> Bool
      return _Wasm_handleWrite(function (state) {
        return _Wasm_encodeBool(
          state,
          _Parser_isAsciiCode(
            _Wasm_decodeInt(_Wasm_mem_i32[argsIndex32]),
            _Wasm_decodeInt(_Wasm_mem_i32[argsIndex32 + 1]),
            _Wasm_decodeString(_Wasm_mem_i32[argsIndex32 + 2])
          )
        );
      });

    case 1:
      // isSubChar : (Char -> Bool) -> Int -> String -> Int
      return _Wasm_handleWrite(function (state) {
        return _Wasm_encodeInt(
          state,
          _Parser_isSubChar(
            _Wasm_decodeClosure(
              [_Wasm_encodeChar],
              _Wasm_decodeBool
            )(_Wasm_mem_i32[argsIndex32]),
            _Wasm_decodeInt(_Wasm_mem_i32[argsIndex32 + 1]),
            _Wasm_decodeString(_Wasm_mem_i32[argsIndex32 + 2])
          )
        );
      });

    default:
      throw new Error('Unknown JS kernel function ' + kernelFnIndex);
  }
}
