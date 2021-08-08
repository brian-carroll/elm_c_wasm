/*
 * WebAssembly-compatible version of Json.js
 * Provides an interface to other JS runtime modules
 *
 * App initialisation flags (Platform.js, Browser.server.js)
 *    __Json_wrap
 *    __Json_run
 *
 * DOM event handlers (VirtualDom.js, Browser.js, Browser.server.js)
 *    __Json_equality
 *    __Json_map
 *    __Json_map2
 *    __Json_runHelp
 *    __Json_succeed
 *    __Json_unwrap
 *
 * Json decoders for non-core Elm types (File.js)
 *    __Json_decodePrim
 *    __Json_expecting
 */

var wasmWrapper;
var EmscriptenModule;

function __Json_wrap(value) {
  const JSON_VALUE_WRAP = 0x2000 * 10000;
  return { $: JSON_VALUE_WRAP, a: value };
}
function __Json_unwrap(value) {
  return value.a;
}
var __Json_run = F2(function (decoder, value) {
  return wasmWrapper.call(wasmWrapper.Json_run, [decoder, value]);
});
function __Json_runHelp(decoder, value) {
  // Wrap it before sending to Wasm, so it gets serialised as JSON, not Elm
  return __Json_run(decoder, __Json_wrap(value));
}

/**
 * DOM event handlers
 */
var __Json_equality; // needs an immediate answer
var __Json_map;
var __Json_map2;
var __Json_succeed;

/**
 * Non-core decoders
 */
var __Json_decodePrim;
var __Json_expecting;
