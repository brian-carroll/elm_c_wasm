

var createEmscriptenModule = (function() {
  var _scriptDir = typeof document !== 'undefined' && document.currentScript ? document.currentScript.src : undefined;
  return (
function(createEmscriptenModule) {
  createEmscriptenModule = createEmscriptenModule || {};

var Module=typeof createEmscriptenModule!=="undefined"?createEmscriptenModule:{};var moduleOverrides={};var key;for(key in Module){if(Module.hasOwnProperty(key)){moduleOverrides[key]=Module[key]}}Module["arguments"]=[];Module["thisProgram"]="./this.program";Module["quit"]=(function(status,toThrow){throw toThrow});Module["preRun"]=[];Module["postRun"]=[];var ENVIRONMENT_IS_WEB=false;var ENVIRONMENT_IS_WORKER=false;var ENVIRONMENT_IS_NODE=false;var ENVIRONMENT_IS_SHELL=false;ENVIRONMENT_IS_WEB=typeof window==="object";ENVIRONMENT_IS_WORKER=typeof importScripts==="function";ENVIRONMENT_IS_NODE=typeof process==="object"&&typeof require==="function"&&!ENVIRONMENT_IS_WEB&&!ENVIRONMENT_IS_WORKER;ENVIRONMENT_IS_SHELL=!ENVIRONMENT_IS_WEB&&!ENVIRONMENT_IS_NODE&&!ENVIRONMENT_IS_WORKER;var scriptDirectory="";function locateFile(path){if(Module["locateFile"]){return Module["locateFile"](path,scriptDirectory)}else{return scriptDirectory+path}}if(ENVIRONMENT_IS_NODE){scriptDirectory=__dirname+"/";var nodeFS;var nodePath;Module["read"]=function shell_read(filename,binary){var ret;if(!nodeFS)nodeFS=require("fs");if(!nodePath)nodePath=require("path");filename=nodePath["normalize"](filename);ret=nodeFS["readFileSync"](filename);return binary?ret:ret.toString()};Module["readBinary"]=function readBinary(filename){var ret=Module["read"](filename,true);if(!ret.buffer){ret=new Uint8Array(ret)}assert(ret.buffer);return ret};if(process["argv"].length>1){Module["thisProgram"]=process["argv"][1].replace(/\\/g,"/")}Module["arguments"]=process["argv"].slice(2);process["on"]("uncaughtException",(function(ex){if(!(ex instanceof ExitStatus)){throw ex}}));process["on"]("unhandledRejection",(function(reason,p){process["exit"](1)}));Module["quit"]=(function(status){process["exit"](status)});Module["inspect"]=(function(){return"[Emscripten Module object]"})}else if(ENVIRONMENT_IS_SHELL){if(typeof read!="undefined"){Module["read"]=function shell_read(f){return read(f)}}Module["readBinary"]=function readBinary(f){var data;if(typeof readbuffer==="function"){return new Uint8Array(readbuffer(f))}data=read(f,"binary");assert(typeof data==="object");return data};if(typeof scriptArgs!="undefined"){Module["arguments"]=scriptArgs}else if(typeof arguments!="undefined"){Module["arguments"]=arguments}if(typeof quit==="function"){Module["quit"]=(function(status){quit(status)})}}else if(ENVIRONMENT_IS_WEB||ENVIRONMENT_IS_WORKER){if(ENVIRONMENT_IS_WEB){if(document.currentScript){scriptDirectory=document.currentScript.src}}else{scriptDirectory=self.location.href}if(_scriptDir){scriptDirectory=_scriptDir}if(scriptDirectory.indexOf("blob:")!==0){scriptDirectory=scriptDirectory.substr(0,scriptDirectory.lastIndexOf("/")+1)}else{scriptDirectory=""}Module["read"]=function shell_read(url){var xhr=new XMLHttpRequest;xhr.open("GET",url,false);xhr.send(null);return xhr.responseText};if(ENVIRONMENT_IS_WORKER){Module["readBinary"]=function readBinary(url){var xhr=new XMLHttpRequest;xhr.open("GET",url,false);xhr.responseType="arraybuffer";xhr.send(null);return new Uint8Array(xhr.response)}}Module["readAsync"]=function readAsync(url,onload,onerror){var xhr=new XMLHttpRequest;xhr.open("GET",url,true);xhr.responseType="arraybuffer";xhr.onload=function xhr_onload(){if(xhr.status==200||xhr.status==0&&xhr.response){onload(xhr.response);return}onerror()};xhr.onerror=onerror;xhr.send(null)};Module["setWindowTitle"]=(function(title){document.title=title})}else{}var out=Module["print"]||(typeof console!=="undefined"?console.log.bind(console):typeof print!=="undefined"?print:null);var err=Module["printErr"]||(typeof printErr!=="undefined"?printErr:typeof console!=="undefined"&&console.warn.bind(console)||out);for(key in moduleOverrides){if(moduleOverrides.hasOwnProperty(key)){Module[key]=moduleOverrides[key]}}moduleOverrides=undefined;var STACK_ALIGN=16;function staticAlloc(size){var ret=STATICTOP;STATICTOP=STATICTOP+size+15&-16;return ret}function alignMemory(size,factor){if(!factor)factor=STACK_ALIGN;var ret=size=Math.ceil(size/factor)*factor;return ret}var asm2wasmImports={"f64-rem":(function(x,y){return x%y}),"debugger":(function(){debugger})};var functionPointers=new Array(0);var GLOBAL_BASE=1024;var ABORT=false;var EXITSTATUS=0;function assert(condition,text){if(!condition){abort("Assertion failed: "+text)}}function getCFunc(ident){var func=Module["_"+ident];assert(func,"Cannot call unknown function "+ident+", make sure it is exported");return func}var JSfuncs={"stackSave":(function(){stackSave()}),"stackRestore":(function(){stackRestore()}),"arrayToC":(function(arr){var ret=stackAlloc(arr.length);writeArrayToMemory(arr,ret);return ret}),"stringToC":(function(str){var ret=0;if(str!==null&&str!==undefined&&str!==0){var len=(str.length<<2)+1;ret=stackAlloc(len);stringToUTF8(str,ret,len)}return ret})};var toC={"string":JSfuncs["stringToC"],"array":JSfuncs["arrayToC"]};function ccall(ident,returnType,argTypes,args,opts){function convertReturnValue(ret){if(returnType==="string")return Pointer_stringify(ret);if(returnType==="boolean")return Boolean(ret);return ret}var func=getCFunc(ident);var cArgs=[];var stack=0;if(args){for(var i=0;i<args.length;i++){var converter=toC[argTypes[i]];if(converter){if(stack===0)stack=stackSave();cArgs[i]=converter(args[i])}else{cArgs[i]=args[i]}}}var ret=func.apply(null,cArgs);ret=convertReturnValue(ret);if(stack!==0)stackRestore(stack);return ret}function cwrap(ident,returnType,argTypes,opts){argTypes=argTypes||[];var numericArgs=argTypes.every((function(type){return type==="number"}));var numericRet=returnType!=="string";if(numericRet&&numericArgs&&!opts){return getCFunc(ident)}return(function(){return ccall(ident,returnType,argTypes,arguments,opts)})}function Pointer_stringify(ptr,length){if(length===0||!ptr)return"";var hasUtf=0;var t;var i=0;while(1){t=HEAPU8[ptr+i>>0];hasUtf|=t;if(t==0&&!length)break;i++;if(length&&i==length)break}if(!length)length=i;var ret="";if(hasUtf<128){var MAX_CHUNK=1024;var curr;while(length>0){curr=String.fromCharCode.apply(String,HEAPU8.subarray(ptr,ptr+Math.min(length,MAX_CHUNK)));ret=ret?ret+curr:curr;ptr+=MAX_CHUNK;length-=MAX_CHUNK}return ret}return UTF8ToString(ptr)}var UTF8Decoder=typeof TextDecoder!=="undefined"?new TextDecoder("utf8"):undefined;function UTF8ArrayToString(u8Array,idx){var endPtr=idx;while(u8Array[endPtr])++endPtr;if(endPtr-idx>16&&u8Array.subarray&&UTF8Decoder){return UTF8Decoder.decode(u8Array.subarray(idx,endPtr))}else{var u0,u1,u2,u3,u4,u5;var str="";while(1){u0=u8Array[idx++];if(!u0)return str;if(!(u0&128)){str+=String.fromCharCode(u0);continue}u1=u8Array[idx++]&63;if((u0&224)==192){str+=String.fromCharCode((u0&31)<<6|u1);continue}u2=u8Array[idx++]&63;if((u0&240)==224){u0=(u0&15)<<12|u1<<6|u2}else{u3=u8Array[idx++]&63;if((u0&248)==240){u0=(u0&7)<<18|u1<<12|u2<<6|u3}else{u4=u8Array[idx++]&63;if((u0&252)==248){u0=(u0&3)<<24|u1<<18|u2<<12|u3<<6|u4}else{u5=u8Array[idx++]&63;u0=(u0&1)<<30|u1<<24|u2<<18|u3<<12|u4<<6|u5}}}if(u0<65536){str+=String.fromCharCode(u0)}else{var ch=u0-65536;str+=String.fromCharCode(55296|ch>>10,56320|ch&1023)}}}}function UTF8ToString(ptr){return UTF8ArrayToString(HEAPU8,ptr)}function stringToUTF8Array(str,outU8Array,outIdx,maxBytesToWrite){if(!(maxBytesToWrite>0))return 0;var startIdx=outIdx;var endIdx=outIdx+maxBytesToWrite-1;for(var i=0;i<str.length;++i){var u=str.charCodeAt(i);if(u>=55296&&u<=57343){var u1=str.charCodeAt(++i);u=65536+((u&1023)<<10)|u1&1023}if(u<=127){if(outIdx>=endIdx)break;outU8Array[outIdx++]=u}else if(u<=2047){if(outIdx+1>=endIdx)break;outU8Array[outIdx++]=192|u>>6;outU8Array[outIdx++]=128|u&63}else if(u<=65535){if(outIdx+2>=endIdx)break;outU8Array[outIdx++]=224|u>>12;outU8Array[outIdx++]=128|u>>6&63;outU8Array[outIdx++]=128|u&63}else if(u<=2097151){if(outIdx+3>=endIdx)break;outU8Array[outIdx++]=240|u>>18;outU8Array[outIdx++]=128|u>>12&63;outU8Array[outIdx++]=128|u>>6&63;outU8Array[outIdx++]=128|u&63}else if(u<=67108863){if(outIdx+4>=endIdx)break;outU8Array[outIdx++]=248|u>>24;outU8Array[outIdx++]=128|u>>18&63;outU8Array[outIdx++]=128|u>>12&63;outU8Array[outIdx++]=128|u>>6&63;outU8Array[outIdx++]=128|u&63}else{if(outIdx+5>=endIdx)break;outU8Array[outIdx++]=252|u>>30;outU8Array[outIdx++]=128|u>>24&63;outU8Array[outIdx++]=128|u>>18&63;outU8Array[outIdx++]=128|u>>12&63;outU8Array[outIdx++]=128|u>>6&63;outU8Array[outIdx++]=128|u&63}}outU8Array[outIdx]=0;return outIdx-startIdx}function stringToUTF8(str,outPtr,maxBytesToWrite){return stringToUTF8Array(str,HEAPU8,outPtr,maxBytesToWrite)}function lengthBytesUTF8(str){var len=0;for(var i=0;i<str.length;++i){var u=str.charCodeAt(i);if(u>=55296&&u<=57343)u=65536+((u&1023)<<10)|str.charCodeAt(++i)&1023;if(u<=127){++len}else if(u<=2047){len+=2}else if(u<=65535){len+=3}else if(u<=2097151){len+=4}else if(u<=67108863){len+=5}else{len+=6}}return len}var UTF16Decoder=typeof TextDecoder!=="undefined"?new TextDecoder("utf-16le"):undefined;function allocateUTF8OnStack(str){var size=lengthBytesUTF8(str)+1;var ret=stackAlloc(size);stringToUTF8Array(str,HEAP8,ret,size);return ret}var WASM_PAGE_SIZE=65536;var ASMJS_PAGE_SIZE=16777216;function alignUp(x,multiple){if(x%multiple>0){x+=multiple-x%multiple}return x}var buffer,HEAP8,HEAPU8,HEAP16,HEAPU16,HEAP32,HEAPU32,HEAPF32,HEAPF64;function updateGlobalBuffer(buf){Module["buffer"]=buffer=buf}function updateGlobalBufferViews(){Module["HEAP8"]=HEAP8=new Int8Array(buffer);Module["HEAP16"]=HEAP16=new Int16Array(buffer);Module["HEAP32"]=HEAP32=new Int32Array(buffer);Module["HEAPU8"]=HEAPU8=new Uint8Array(buffer);Module["HEAPU16"]=HEAPU16=new Uint16Array(buffer);Module["HEAPU32"]=HEAPU32=new Uint32Array(buffer);Module["HEAPF32"]=HEAPF32=new Float32Array(buffer);Module["HEAPF64"]=HEAPF64=new Float64Array(buffer)}var STATIC_BASE,STATICTOP,staticSealed;var STACK_BASE,STACKTOP,STACK_MAX;var DYNAMIC_BASE,DYNAMICTOP_PTR;STATIC_BASE=STATICTOP=STACK_BASE=STACKTOP=STACK_MAX=DYNAMIC_BASE=DYNAMICTOP_PTR=0;staticSealed=false;function abortOnCannotGrowMemory(){abort("Cannot enlarge memory arrays. Either (1) compile with  -s TOTAL_MEMORY=X  with X higher than the current value "+TOTAL_MEMORY+", (2) compile with  -s ALLOW_MEMORY_GROWTH=1  which allows increasing the size at runtime, or (3) if you want malloc to return NULL (0) instead of this abort, compile with  -s ABORTING_MALLOC=0 ")}function enlargeMemory(){abortOnCannotGrowMemory()}var TOTAL_STACK=Module["TOTAL_STACK"]||5242880;var TOTAL_MEMORY=Module["TOTAL_MEMORY"]||16777216;if(TOTAL_MEMORY<TOTAL_STACK)err("TOTAL_MEMORY should be larger than TOTAL_STACK, was "+TOTAL_MEMORY+"! (TOTAL_STACK="+TOTAL_STACK+")");if(Module["buffer"]){buffer=Module["buffer"]}else{if(typeof WebAssembly==="object"&&typeof WebAssembly.Memory==="function"){Module["wasmMemory"]=new WebAssembly.Memory({"initial":TOTAL_MEMORY/WASM_PAGE_SIZE,"maximum":TOTAL_MEMORY/WASM_PAGE_SIZE});buffer=Module["wasmMemory"].buffer}else{buffer=new ArrayBuffer(TOTAL_MEMORY)}Module["buffer"]=buffer}updateGlobalBufferViews();function getTotalMemory(){return TOTAL_MEMORY}function callRuntimeCallbacks(callbacks){while(callbacks.length>0){var callback=callbacks.shift();if(typeof callback=="function"){callback();continue}var func=callback.func;if(typeof func==="number"){if(callback.arg===undefined){Module["dynCall_v"](func)}else{Module["dynCall_vi"](func,callback.arg)}}else{func(callback.arg===undefined?null:callback.arg)}}}var __ATPRERUN__=[];var __ATINIT__=[];var __ATMAIN__=[];var __ATEXIT__=[];var __ATPOSTRUN__=[];var runtimeInitialized=false;var runtimeExited=false;function preRun(){if(Module["preRun"]){if(typeof Module["preRun"]=="function")Module["preRun"]=[Module["preRun"]];while(Module["preRun"].length){addOnPreRun(Module["preRun"].shift())}}callRuntimeCallbacks(__ATPRERUN__)}function ensureInitRuntime(){if(runtimeInitialized)return;runtimeInitialized=true;callRuntimeCallbacks(__ATINIT__)}function preMain(){callRuntimeCallbacks(__ATMAIN__)}function exitRuntime(){callRuntimeCallbacks(__ATEXIT__);runtimeExited=true}function postRun(){if(Module["postRun"]){if(typeof Module["postRun"]=="function")Module["postRun"]=[Module["postRun"]];while(Module["postRun"].length){addOnPostRun(Module["postRun"].shift())}}callRuntimeCallbacks(__ATPOSTRUN__)}function addOnPreRun(cb){__ATPRERUN__.unshift(cb)}function addOnPostRun(cb){__ATPOSTRUN__.unshift(cb)}function writeArrayToMemory(array,buffer){HEAP8.set(array,buffer)}var runDependencies=0;var runDependencyWatcher=null;var dependenciesFulfilled=null;function addRunDependency(id){runDependencies++;if(Module["monitorRunDependencies"]){Module["monitorRunDependencies"](runDependencies)}}function removeRunDependency(id){runDependencies--;if(Module["monitorRunDependencies"]){Module["monitorRunDependencies"](runDependencies)}if(runDependencies==0){if(runDependencyWatcher!==null){clearInterval(runDependencyWatcher);runDependencyWatcher=null}if(dependenciesFulfilled){var callback=dependenciesFulfilled;dependenciesFulfilled=null;callback()}}}Module["preloadedImages"]={};Module["preloadedAudios"]={};var dataURIPrefix="data:application/octet-stream;base64,";function isDataURI(filename){return String.prototype.startsWith?filename.startsWith(dataURIPrefix):filename.indexOf(dataURIPrefix)===0}function integrateWasmJS(){var wasmTextFile="benchmark.wast";var wasmBinaryFile="benchmark.wasm";var asmjsCodeFile="benchmark.temp.asm.js";if(!isDataURI(wasmTextFile)){wasmTextFile=locateFile(wasmTextFile)}if(!isDataURI(wasmBinaryFile)){wasmBinaryFile=locateFile(wasmBinaryFile)}if(!isDataURI(asmjsCodeFile)){asmjsCodeFile=locateFile(asmjsCodeFile)}var wasmPageSize=64*1024;var info={"global":null,"env":null,"asm2wasm":asm2wasmImports,"parent":Module};var exports=null;function mergeMemory(newBuffer){var oldBuffer=Module["buffer"];if(newBuffer.byteLength<oldBuffer.byteLength){err("the new buffer in mergeMemory is smaller than the previous one. in native wasm, we should grow memory here")}var oldView=new Int8Array(oldBuffer);var newView=new Int8Array(newBuffer);newView.set(oldView);updateGlobalBuffer(newBuffer);updateGlobalBufferViews()}function fixImports(imports){return imports}function getBinary(){try{if(Module["wasmBinary"]){return new Uint8Array(Module["wasmBinary"])}if(Module["readBinary"]){return Module["readBinary"](wasmBinaryFile)}else{throw"both async and sync fetching of the wasm failed"}}catch(err){abort(err)}}function getBinaryPromise(){if(!Module["wasmBinary"]&&(ENVIRONMENT_IS_WEB||ENVIRONMENT_IS_WORKER)&&typeof fetch==="function"){return fetch(wasmBinaryFile,{credentials:"same-origin"}).then((function(response){if(!response["ok"]){throw"failed to load wasm binary file at '"+wasmBinaryFile+"'"}return response["arrayBuffer"]()})).catch((function(){return getBinary()}))}return new Promise((function(resolve,reject){resolve(getBinary())}))}function doNativeWasm(global,env,providedBuffer){if(typeof WebAssembly!=="object"){err("no native wasm support detected");return false}if(!(Module["wasmMemory"]instanceof WebAssembly.Memory)){err("no native wasm Memory in use");return false}env["memory"]=Module["wasmMemory"];info["global"]={"NaN":NaN,"Infinity":Infinity};info["global.Math"]=Math;info["env"]=env;function receiveInstance(instance,module){exports=instance.exports;if(exports.memory)mergeMemory(exports.memory);Module["asm"]=exports;Module["usingWasm"]=true;removeRunDependency("wasm-instantiate")}addRunDependency("wasm-instantiate");if(Module["instantiateWasm"]){try{return Module["instantiateWasm"](info,receiveInstance)}catch(e){err("Module.instantiateWasm callback failed with error: "+e);return false}}function receiveInstantiatedSource(output){receiveInstance(output["instance"],output["module"])}function instantiateArrayBuffer(receiver){getBinaryPromise().then((function(binary){return WebAssembly.instantiate(binary,info)})).then(receiver).catch((function(reason){err("failed to asynchronously prepare wasm: "+reason);abort(reason)}))}if(!Module["wasmBinary"]&&typeof WebAssembly.instantiateStreaming==="function"&&!isDataURI(wasmBinaryFile)&&typeof fetch==="function"){WebAssembly.instantiateStreaming(fetch(wasmBinaryFile,{credentials:"same-origin"}),info).then(receiveInstantiatedSource).catch((function(reason){err("wasm streaming compile failed: "+reason);err("falling back to ArrayBuffer instantiation");instantiateArrayBuffer(receiveInstantiatedSource)}))}else{instantiateArrayBuffer(receiveInstantiatedSource)}return{}}Module["asmPreload"]=Module["asm"];var asmjsReallocBuffer=Module["reallocBuffer"];var wasmReallocBuffer=(function(size){var PAGE_MULTIPLE=Module["usingWasm"]?WASM_PAGE_SIZE:ASMJS_PAGE_SIZE;size=alignUp(size,PAGE_MULTIPLE);var old=Module["buffer"];var oldSize=old.byteLength;if(Module["usingWasm"]){try{var result=Module["wasmMemory"].grow((size-oldSize)/wasmPageSize);if(result!==(-1|0)){return Module["buffer"]=Module["wasmMemory"].buffer}else{return null}}catch(e){return null}}});Module["reallocBuffer"]=(function(size){if(finalMethod==="asmjs"){return asmjsReallocBuffer(size)}else{return wasmReallocBuffer(size)}});var finalMethod="";Module["asm"]=(function(global,env,providedBuffer){env=fixImports(env);if(!env["table"]){var TABLE_SIZE=Module["wasmTableSize"];if(TABLE_SIZE===undefined)TABLE_SIZE=1024;var MAX_TABLE_SIZE=Module["wasmMaxTableSize"];if(typeof WebAssembly==="object"&&typeof WebAssembly.Table==="function"){if(MAX_TABLE_SIZE!==undefined){env["table"]=new WebAssembly.Table({"initial":TABLE_SIZE,"maximum":MAX_TABLE_SIZE,"element":"anyfunc"})}else{env["table"]=new WebAssembly.Table({"initial":TABLE_SIZE,element:"anyfunc"})}}else{env["table"]=new Array(TABLE_SIZE)}Module["wasmTable"]=env["table"]}if(!env["memoryBase"]){env["memoryBase"]=Module["STATIC_BASE"]}if(!env["tableBase"]){env["tableBase"]=0}var exports;exports=doNativeWasm(global,env,providedBuffer);assert(exports,"no binaryen method succeeded.");return exports})}integrateWasmJS();STATIC_BASE=GLOBAL_BASE;STATICTOP=STATIC_BASE+6528;__ATINIT__.push();var STATIC_BUMP=6528;Module["STATIC_BASE"]=STATIC_BASE;Module["STATIC_BUMP"]=STATIC_BUMP;STATICTOP+=16;function ___assert_fail(condition,filename,line,func){abort("Assertion failed: "+Pointer_stringify(condition)+", at: "+[filename?Pointer_stringify(filename):"unknown filename",line,func?Pointer_stringify(func):"unknown function"])}var SYSCALLS={varargs:0,get:(function(varargs){SYSCALLS.varargs+=4;var ret=HEAP32[SYSCALLS.varargs-4>>2];return ret}),getStr:(function(){var ret=Pointer_stringify(SYSCALLS.get());return ret}),get64:(function(){var low=SYSCALLS.get(),high=SYSCALLS.get();if(low>=0)assert(high===0);else assert(high===-1);return low}),getZero:(function(){assert(SYSCALLS.get()===0)})};function ___syscall140(which,varargs){SYSCALLS.varargs=varargs;try{var stream=SYSCALLS.getStreamFromFD(),offset_high=SYSCALLS.get(),offset_low=SYSCALLS.get(),result=SYSCALLS.get(),whence=SYSCALLS.get();var offset=offset_low;FS.llseek(stream,offset,whence);HEAP32[result>>2]=stream.position;if(stream.getdents&&offset===0&&whence===0)stream.getdents=null;return 0}catch(e){if(typeof FS==="undefined"||!(e instanceof FS.ErrnoError))abort(e);return-e.errno}}function ___syscall146(which,varargs){SYSCALLS.varargs=varargs;try{var stream=SYSCALLS.get(),iov=SYSCALLS.get(),iovcnt=SYSCALLS.get();var ret=0;if(!___syscall146.buffers){___syscall146.buffers=[null,[],[]];___syscall146.printChar=(function(stream,curr){var buffer=___syscall146.buffers[stream];assert(buffer);if(curr===0||curr===10){(stream===1?out:err)(UTF8ArrayToString(buffer,0));buffer.length=0}else{buffer.push(curr)}})}for(var i=0;i<iovcnt;i++){var ptr=HEAP32[iov+i*8>>2];var len=HEAP32[iov+(i*8+4)>>2];for(var j=0;j<len;j++){___syscall146.printChar(stream,HEAPU8[ptr+j])}ret+=len}return ret}catch(e){if(typeof FS==="undefined"||!(e instanceof FS.ErrnoError))abort(e);return-e.errno}}function ___syscall54(which,varargs){SYSCALLS.varargs=varargs;try{return 0}catch(e){if(typeof FS==="undefined"||!(e instanceof FS.ErrnoError))abort(e);return-e.errno}}function ___syscall6(which,varargs){SYSCALLS.varargs=varargs;try{var stream=SYSCALLS.getStreamFromFD();FS.close(stream);return 0}catch(e){if(typeof FS==="undefined"||!(e instanceof FS.ErrnoError))abort(e);return-e.errno}}function _emscripten_memcpy_big(dest,src,num){HEAPU8.set(HEAPU8.subarray(src,src+num),dest);return dest}function ___setErrNo(value){if(Module["___errno_location"])HEAP32[Module["___errno_location"]()>>2]=value;return value}DYNAMICTOP_PTR=staticAlloc(4);STACK_BASE=STACKTOP=alignMemory(STATICTOP);STACK_MAX=STACK_BASE+TOTAL_STACK;DYNAMIC_BASE=alignMemory(STACK_MAX);HEAP32[DYNAMICTOP_PTR>>2]=DYNAMIC_BASE;staticSealed=true;Module["wasmTableSize"]=14;Module["wasmMaxTableSize"]=14;Module.asmGlobalArg={};Module.asmLibraryArg={"abort":abort,"enlargeMemory":enlargeMemory,"getTotalMemory":getTotalMemory,"abortOnCannotGrowMemory":abortOnCannotGrowMemory,"___assert_fail":___assert_fail,"___setErrNo":___setErrNo,"___syscall140":___syscall140,"___syscall146":___syscall146,"___syscall54":___syscall54,"___syscall6":___syscall6,"_emscripten_memcpy_big":_emscripten_memcpy_big,"DYNAMICTOP_PTR":DYNAMICTOP_PTR,"STACKTOP":STACKTOP};var asm=Module["asm"](Module.asmGlobalArg,Module.asmLibraryArg,buffer);Module["asm"]=asm;var ___errno_location=Module["___errno_location"]=(function(){return Module["asm"]["___errno_location"].apply(null,arguments)});var _collectGarbage=Module["_collectGarbage"]=(function(){return Module["asm"]["_collectGarbage"].apply(null,arguments)});var _debugHeapState=Module["_debugHeapState"]=(function(){return Module["asm"]["_debugHeapState"].apply(null,arguments)});var _dump=Module["_dump"]=(function(){return Module["asm"]["_dump"].apply(null,arguments)});var _evalClosure=Module["_evalClosure"]=(function(){return Module["asm"]["_evalClosure"].apply(null,arguments)});var _export_add=Module["_export_add"]=(function(){return Module["asm"]["_export_add"].apply(null,arguments)});var _export_add_unboxed=Module["_export_add_unboxed"]=(function(){return Module["asm"]["_export_add_unboxed"].apply(null,arguments)});var _export_count=Module["_export_count"]=(function(){return Module["asm"]["_export_count"].apply(null,arguments)});var _export_count_no_tce=Module["_export_count_no_tce"]=(function(){return Module["asm"]["_export_count_no_tce"].apply(null,arguments)});var _finishWritingAt=Module["_finishWritingAt"]=(function(){return Module["asm"]["_finishWritingAt"].apply(null,arguments)});var _getFalse=Module["_getFalse"]=(function(){return Module["asm"]["_getFalse"].apply(null,arguments)});var _getMaxWriteAddr=Module["_getMaxWriteAddr"]=(function(){return Module["asm"]["_getMaxWriteAddr"].apply(null,arguments)});var _getNextFieldGroup=Module["_getNextFieldGroup"]=(function(){return Module["asm"]["_getNextFieldGroup"].apply(null,arguments)});var _getNextMain=Module["_getNextMain"]=(function(){return Module["asm"]["_getNextMain"].apply(null,arguments)});var _getNil=Module["_getNil"]=(function(){return Module["asm"]["_getNil"].apply(null,arguments)});var _getTrue=Module["_getTrue"]=(function(){return Module["asm"]["_getTrue"].apply(null,arguments)});var _getUnit=Module["_getUnit"]=(function(){return Module["asm"]["_getUnit"].apply(null,arguments)});var _getWriteAddr=Module["_getWriteAddr"]=(function(){return Module["asm"]["_getWriteAddr"].apply(null,arguments)});var _main=Module["_main"]=(function(){return Module["asm"]["_main"].apply(null,arguments)});var _readF64=Module["_readF64"]=(function(){return Module["asm"]["_readF64"].apply(null,arguments)});var _writeF64=Module["_writeF64"]=(function(){return Module["asm"]["_writeF64"].apply(null,arguments)});var stackAlloc=Module["stackAlloc"]=(function(){return Module["asm"]["stackAlloc"].apply(null,arguments)});var stackRestore=Module["stackRestore"]=(function(){return Module["asm"]["stackRestore"].apply(null,arguments)});var stackSave=Module["stackSave"]=(function(){return Module["asm"]["stackSave"].apply(null,arguments)});Module["asm"]=asm;Module["ccall"]=ccall;Module["cwrap"]=cwrap;Module["then"]=(function(func){if(Module["calledRun"]){func(Module)}else{var old=Module["onRuntimeInitialized"];Module["onRuntimeInitialized"]=(function(){if(old)old();func(Module)})}return Module});function ExitStatus(status){this.name="ExitStatus";this.message="Program terminated with exit("+status+")";this.status=status}ExitStatus.prototype=new Error;ExitStatus.prototype.constructor=ExitStatus;var initialStackTop;var calledMain=false;dependenciesFulfilled=function runCaller(){if(!Module["calledRun"])run();if(!Module["calledRun"])dependenciesFulfilled=runCaller};Module["callMain"]=function callMain(args){args=args||[];ensureInitRuntime();var argc=args.length+1;var argv=stackAlloc((argc+1)*4);HEAP32[argv>>2]=allocateUTF8OnStack(Module["thisProgram"]);for(var i=1;i<argc;i++){HEAP32[(argv>>2)+i]=allocateUTF8OnStack(args[i-1])}HEAP32[(argv>>2)+argc]=0;try{var ret=Module["_main"](argc,argv,0);exit(ret,true)}catch(e){if(e instanceof ExitStatus){return}else if(e=="SimulateInfiniteLoop"){Module["noExitRuntime"]=true;return}else{var toLog=e;if(e&&typeof e==="object"&&e.stack){toLog=[e,e.stack]}err("exception thrown: "+toLog);Module["quit"](1,e)}}finally{calledMain=true}};function run(args){args=args||Module["arguments"];if(runDependencies>0){return}preRun();if(runDependencies>0)return;if(Module["calledRun"])return;function doRun(){if(Module["calledRun"])return;Module["calledRun"]=true;if(ABORT)return;ensureInitRuntime();preMain();if(Module["onRuntimeInitialized"])Module["onRuntimeInitialized"]();if(Module["_main"]&&shouldRunNow)Module["callMain"](args);postRun()}if(Module["setStatus"]){Module["setStatus"]("Running...");setTimeout((function(){setTimeout((function(){Module["setStatus"]("")}),1);doRun()}),1)}else{doRun()}}Module["run"]=run;function exit(status,implicit){if(implicit&&Module["noExitRuntime"]&&status===0){return}if(Module["noExitRuntime"]){}else{ABORT=true;EXITSTATUS=status;STACKTOP=initialStackTop;exitRuntime();if(Module["onExit"])Module["onExit"](status)}Module["quit"](status,new ExitStatus(status))}function abort(what){if(Module["onAbort"]){Module["onAbort"](what)}if(what!==undefined){out(what);err(what);what=JSON.stringify(what)}else{what=""}ABORT=true;EXITSTATUS=1;throw"abort("+what+"). Build with -s ASSERTIONS=1 for more info."}Module["abort"]=abort;if(Module["preInit"]){if(typeof Module["preInit"]=="function")Module["preInit"]=[Module["preInit"]];while(Module["preInit"].length>0){Module["preInit"].pop()()}}var shouldRunNow=true;if(Module["noInitialRun"]){shouldRunNow=false}Module["noExitRuntime"]=true;run()





  return createEmscriptenModule;
}
);
})();
if (typeof exports === 'object' && typeof module === 'object')
    module.exports = createEmscriptenModule;
  else if (typeof define === 'function' && define['amd'])
    define([], function() { return createEmscriptenModule; });
  else if (typeof exports === 'object')
    exports["createEmscriptenModule"] = createEmscriptenModule;
  
createEmscriptenModule().then(function(Module) {

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

console.warn('Compiled in DEV mode. Follow the advice at https://elm-lang.org/0.19.0/optimize for better performance and smaller assets.');


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
		return ord === elm$core$Basics$EQ ? 0 : ord === elm$core$Basics$LT ? -1 : 1;
	}));
});



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
	if (depth > 100)
	{
		stack.push(_Utils_Tuple2(x,y));
		return true;
	}

	if (x === y)
	{
		return true;
	}

	if (typeof x !== 'object' || x === null || y === null)
	{
		typeof x === 'function' && _Debug_crash(5);
		return false;
	}

	/**/
	if (x.$ === 'Set_elm_builtin')
	{
		x = elm$core$Set$toList(x);
		y = elm$core$Set$toList(y);
	}
	if (x.$ === 'RBNode_elm_builtin' || x.$ === 'RBEmpty_elm_builtin')
	{
		x = elm$core$Dict$toList(x);
		y = elm$core$Dict$toList(y);
	}
	//*/

	/**_UNUSED/
	if (x.$ < 0)
	{
		x = elm$core$Dict$toList(x);
		y = elm$core$Dict$toList(y);
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
	return n < 0 ? elm$core$Basics$LT : n ? elm$core$Basics$GT : elm$core$Basics$EQ;
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
				+ _Debug_toAnsiString(ansi, elm$core$Set$toList(value));
		}

		if (tag === 'RBNode_elm_builtin' || tag === 'RBEmpty_elm_builtin')
		{
			return _Debug_ctorColor(ansi, 'Dict')
				+ _Debug_fadeColor(ansi, '.fromList') + ' '
				+ _Debug_toAnsiString(ansi, elm$core$Dict$toList(value));
		}

		if (tag === 'Array_elm_builtin')
		{
			return _Debug_ctorColor(ansi, 'Array')
				+ _Debug_fadeColor(ansi, '.fromList') + ' '
				+ _Debug_toAnsiString(ansi, elm$core$Array$toList(value));
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

	if (typeof File === 'function' && value instanceof File)
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
	return ansi ? '\x1b[94m' + string + '\x1b[0m' : string;
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


function _Scheduler_step(proc)
{
	while (proc.f)
	{
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


var _Benchmark_getTimestamp =
  typeof performance !== "undefined"
    ? performance.now.bind(performance)
    : Date.now;

// sample : Int -> Operation -> Task Error Float
var _Benchmark_sample = F2(function(n, fn) {
  return _Scheduler_binding(function(callback) {
    var start = _Benchmark_getTimestamp();

    try {
      for (var i = 0; i < n; i++) {
        fn();
      }
    } catch (error) {
      if (error instanceof RangeError) {
        callback(_Scheduler_fail(elm_explorations$benchmark$Benchmark$LowLevel$StackOverflow));
      } else {
        callback(_Scheduler_fail(elm_explorations$benchmark$Benchmark$LowLevel$UnknownError(error.message)));
      }
      return;
    }

    var end = _Benchmark_getTimestamp();

    callback(_Scheduler_succeed(end - start));
  });
});

// operation : (() -> a) -> Operation
function _Benchmark_operation(thunk) {
  return thunk;
}



var _String_cons = F2(function(chr, str)
{
	return chr + str;
});

function _String_uncons(string)
{
	var word = string.charCodeAt(0);
	return word
		? elm$core$Maybe$Just(
			0xD800 <= word && word <= 0xDBFF
				? _Utils_Tuple2(_Utils_chr(string[0] + string[1]), string.slice(2))
				: _Utils_Tuple2(_Utils_chr(string[0]), string.slice(1))
		)
		: elm$core$Maybe$Nothing;
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
			return elm$core$Maybe$Nothing;
		}
		total = 10 * total + code - 0x30;
	}

	return i == start
		? elm$core$Maybe$Nothing
		: elm$core$Maybe$Just(code0 == 0x2D ? -total : total);
}


// FLOAT CONVERSIONS

function _String_toFloat(s)
{
	// check if it is a hex, octal, or binary number
	if (s.length === 0 || /[\sxbo]/.test(s))
	{
		return elm$core$Maybe$Nothing;
	}
	var n = +s;
	// faster isNaN check
	return n === n ? elm$core$Maybe$Just(n) : elm$core$Maybe$Nothing;
}

function _String_fromList(chars)
{
	return _List_toArray(chars).join('');
}




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



/**/
function _Json_errorToString(error)
{
	return elm$json$Json$Decode$errorToString(error);
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
		? elm$core$Result$Ok(value)
		:
	(isFinite(value) && !(value % 1))
		? elm$core$Result$Ok(value)
		: _Json_expecting('an INT', value);
});

var _Json_decodeBool = _Json_decodePrim(function(value) {
	return (typeof value === 'boolean')
		? elm$core$Result$Ok(value)
		: _Json_expecting('a BOOL', value);
});

var _Json_decodeFloat = _Json_decodePrim(function(value) {
	return (typeof value === 'number')
		? elm$core$Result$Ok(value)
		: _Json_expecting('a FLOAT', value);
});

var _Json_decodeValue = _Json_decodePrim(function(value) {
	return elm$core$Result$Ok(_Json_wrap(value));
});

var _Json_decodeString = _Json_decodePrim(function(value) {
	return (typeof value === 'string')
		? elm$core$Result$Ok(value)
		: (value instanceof String)
			? elm$core$Result$Ok(value + '')
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
		return elm$core$Result$Err(A2(elm$json$Json$Decode$Failure, 'This is not valid JSON! ' + e.message, _Json_wrap(string)));
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
				? elm$core$Result$Ok(decoder.c)
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
			return (elm$core$Result$isOk(result)) ? result : elm$core$Result$Err(A2(elm$json$Json$Decode$Field, field, result.a));

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
			return (elm$core$Result$isOk(result)) ? result : elm$core$Result$Err(A2(elm$json$Json$Decode$Index, index, result.a));

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
					if (!elm$core$Result$isOk(result))
					{
						return elm$core$Result$Err(A2(elm$json$Json$Decode$Field, key, result.a));
					}
					keyValuePairs = _List_Cons(_Utils_Tuple2(key, result.a), keyValuePairs);
				}
			}
			return elm$core$Result$Ok(elm$core$List$reverse(keyValuePairs));

		case 9:
			var answer = decoder.f;
			var decoders = decoder.g;
			for (var i = 0; i < decoders.length; i++)
			{
				var result = _Json_runHelp(decoders[i], value);
				if (!elm$core$Result$isOk(result))
				{
					return result;
				}
				answer = answer(result.a);
			}
			return elm$core$Result$Ok(answer);

		case 10:
			var result = _Json_runHelp(decoder.b, value);
			return (!elm$core$Result$isOk(result))
				? result
				: _Json_runHelp(decoder.h(result.a), value);

		case 11:
			var errors = _List_Nil;
			for (var temp = decoder.g; temp.b; temp = temp.b) // WHILE_CONS
			{
				var result = _Json_runHelp(temp.a, value);
				if (elm$core$Result$isOk(result))
				{
					return result;
				}
				errors = _List_Cons(result.a, errors);
			}
			return elm$core$Result$Err(elm$json$Json$Decode$OneOf(elm$core$List$reverse(errors)));

		case 1:
			return elm$core$Result$Err(A2(elm$json$Json$Decode$Failure, decoder.a, _Json_wrap(value)));

		case 0:
			return elm$core$Result$Ok(decoder.a);
	}
}

function _Json_runArrayDecoder(decoder, value, toElmValue)
{
	var len = value.length;
	var array = new Array(len);
	for (var i = 0; i < len; i++)
	{
		var result = _Json_runHelp(decoder, value[i]);
		if (!elm$core$Result$isOk(result))
		{
			return elm$core$Result$Err(A2(elm$json$Json$Decode$Index, i, result.a));
		}
		array[i] = result.a;
	}
	return elm$core$Result$Ok(toElmValue(array));
}

function _Json_isArray(value)
{
	return Array.isArray(value) || (typeof FileList !== 'undefined' && value instanceof FileList);
}

function _Json_toElmArray(array)
{
	return A2(elm$core$Array$initialize, array.length, function(i) { return array[i]; });
}

function _Json_expecting(type, value)
{
	return elm$core$Result$Err(A2(elm$json$Json$Decode$Failure, 'Expecting ' + type, _Json_wrap(value)));
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



function _Process_sleep(time)
{
	return _Scheduler_binding(function(callback) {
		var id = setTimeout(function() {
			callback(_Scheduler_succeed(_Utils_Tuple0));
		}, time);

		return function() { clearTimeout(id); };
	});
}




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
	elm$core$Result$isOk(result) || _Debug_crash(2 /**/, _Json_errorToString(result.a) /**/);
	var managers = {};
	result = init(result.a);
	var model = result.a;
	var stepper = stepperBuilder(sendToApp, model);
	var ports = _Platform_setupEffects(managers, sendToApp);

	function sendToApp(msg, viewMetadata)
	{
		result = A2(update, msg, model);
		stepper(model = result.a, viewMetadata);
		_Platform_dispatchEffects(managers, result.b, subscriptions(model));
	}

	_Platform_dispatchEffects(managers, result.b, subscriptions(model));

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
				p: bag.n,
				q: taggers
			});
			return;
	}
}


function _Platform_toEffect(isCmd, home, taggers, value)
{
	function applyTaggers(x)
	{
		for (var temp = taggers; temp; temp = temp.q)
		{
			x = temp.p(x);
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
		r: converter,
		a: _Platform_setupOutgoingPort
	};
	return _Platform_leaf(name);
}


var _Platform_outgoingPortMap = F2(function(tagger, value) { return value; });


function _Platform_setupOutgoingPort(name)
{
	var subs = [];
	var converter = _Platform_effectManagers[name].r;

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
		r: converter,
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
	var converter = _Platform_effectManagers[name].r;

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

		elm$core$Result$isOk(result) || _Debug_crash(4, name, result.a);

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
	var tag = elm$virtual_dom$VirtualDom$toHandlerInt(handler);

	// 0 = Normal
	// 1 = MayStopPropagation
	// 2 = MayPreventDefault
	// 3 = Custom

	return {
		$: handler.$,
		a:
			!tag
				? A2(elm$json$Json$Decode$map, func, handler.a)
				:
			A3(elm$json$Json$Decode$map2,
				tag < 3
					? _VirtualDom_mapEventTuple
					: _VirtualDom_mapEventRecord,
				elm$json$Json$Decode$succeed(func),
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
			&& { passive: elm$virtual_dom$VirtualDom$toHandlerInt(newHandler) < 2 }
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

		if (!elm$core$Result$isOk(result))
		{
			return;
		}

		var tag = elm$virtual_dom$VirtualDom$toHandlerInt(handler);

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
					var next = elm$url$Url$fromString(href).a;
					sendToApp(onUrlRequest(
						(next
							&& curr.protocol === next.protocol
							&& curr.host === next.host
							&& curr.port_.a === next.port_.a
						)
							? elm$browser$Browser$Internal(next)
							: elm$browser$Browser$External(href)
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
	return elm$url$Url$fromString(_VirtualDom_doc.location.href).a || _Debug_crash(1);
}

var _Browser_go = F2(function(key, n)
{
	return A2(elm$core$Task$perform, elm$core$Basics$never, _Scheduler_binding(function() {
		n && history.go(n);
		key();
	}));
});

var _Browser_pushUrl = F2(function(key, url)
{
	return A2(elm$core$Task$perform, elm$core$Basics$never, _Scheduler_binding(function() {
		history.pushState({}, '', url);
		key();
	}));
});

var _Browser_replaceUrl = F2(function(key, url)
{
	return A2(elm$core$Task$perform, elm$core$Basics$never, _Scheduler_binding(function() {
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
	return elm$core$Result$isOk(result) ? elm$core$Maybe$Just(result.a) : elm$core$Maybe$Nothing;
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
				: _Scheduler_fail(elm$browser$Browser$Dom$NotFound(id))
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
	return A2(elm$core$Task$perform, elm$core$Basics$never, _Scheduler_binding(function(callback)
	{
		_VirtualDom_doc.location.reload(skipCache);
	}));
}

function _Browser_load(url)
{
	return A2(elm$core$Task$perform, elm$core$Basics$never, _Scheduler_binding(function(callback)
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



var _Bitwise_and = F2(function(a, b)
{
	return a & b;
});

var _Bitwise_or = F2(function(a, b)
{
	return a | b;
});

var _Bitwise_xor = F2(function(a, b)
{
	return a ^ b;
});

function _Bitwise_complement(a)
{
	return ~a;
};

var _Bitwise_shiftLeftBy = F2(function(offset, a)
{
	return a << offset;
});

var _Bitwise_shiftRightBy = F2(function(offset, a)
{
	return a >> offset;
});

var _Bitwise_shiftRightZfBy = F2(function(offset, a)
{
	return a >>> offset;
});


// CREATE

var _Regex_never = /.^/;

var _Regex_fromStringWith = F2(function(options, string)
{
	var flags = 'g';
	if (options.multiline) { flags += 'm'; }
	if (options.caseInsensitive) { flags += 'i'; }

	try
	{
		return elm$core$Maybe$Just(new RegExp(string, flags));
	}
	catch(error)
	{
		return elm$core$Maybe$Nothing;
	}
});


// USE

var _Regex_contains = F2(function(re, string)
{
	return string.match(re) !== null;
});


var _Regex_findAtMost = F3(function(n, re, str)
{
	var out = [];
	var number = 0;
	var string = str;
	var lastIndex = re.lastIndex;
	var prevLastIndex = -1;
	var result;
	while (number++ < n && (result = re.exec(string)))
	{
		if (prevLastIndex == re.lastIndex) break;
		var i = result.length - 1;
		var subs = new Array(i);
		while (i > 0)
		{
			var submatch = result[i];
			subs[--i] = submatch
				? elm$core$Maybe$Just(submatch)
				: elm$core$Maybe$Nothing;
		}
		out.push(A4(elm$regex$Regex$Match, result[0], result.index, number, _List_fromArray(subs)));
		prevLastIndex = re.lastIndex;
	}
	re.lastIndex = lastIndex;
	return _List_fromArray(out);
});


var _Regex_replaceAtMost = F4(function(n, re, replacer, string)
{
	var count = 0;
	function jsReplacer(match)
	{
		if (count++ >= n)
		{
			return match;
		}
		var i = arguments.length - 3;
		var submatches = new Array(i);
		while (i > 0)
		{
			var submatch = arguments[i];
			submatches[--i] = submatch
				? elm$core$Maybe$Just(submatch)
				: elm$core$Maybe$Nothing;
		}
		return replacer(A4(elm$regex$Regex$Match, match, arguments[arguments.length - 2], count, _List_fromArray(submatches)));
	}
	return string.replace(re, jsReplacer);
});

var _Regex_splitAtMost = F3(function(n, re, str)
{
	var string = str;
	var out = [];
	var start = re.lastIndex;
	var restoreLastIndex = re.lastIndex;
	while (n--)
	{
		var result = re.exec(string);
		if (!result) break;
		out.push(string.slice(start, result.index));
		start = re.lastIndex;
	}
	out.push(string.slice(start));
	re.lastIndex = restoreLastIndex;
	return _List_fromArray(out);
});

var _Regex_infinity = Infinity;
var author$project$Main$Box = function (a) {
	return {$: 'Box', a: a};
};
var elm$core$Basics$EQ = {$: 'EQ'};
var elm$core$Basics$GT = {$: 'GT'};
var elm$core$Basics$LT = {$: 'LT'};
var elm$core$Dict$foldr = F3(
	function (func, acc, t) {
		foldr:
		while (true) {
			if (t.$ === 'RBEmpty_elm_builtin') {
				return acc;
			} else {
				var key = t.b;
				var value = t.c;
				var left = t.d;
				var right = t.e;
				var $temp$func = func,
					$temp$acc = A3(
					func,
					key,
					value,
					A3(elm$core$Dict$foldr, func, acc, right)),
					$temp$t = left;
				func = $temp$func;
				acc = $temp$acc;
				t = $temp$t;
				continue foldr;
			}
		}
	});
var elm$core$List$cons = _List_cons;
var elm$core$Dict$toList = function (dict) {
	return A3(
		elm$core$Dict$foldr,
		F3(
			function (key, value, list) {
				return A2(
					elm$core$List$cons,
					_Utils_Tuple2(key, value),
					list);
			}),
		_List_Nil,
		dict);
};
var elm$core$Dict$keys = function (dict) {
	return A3(
		elm$core$Dict$foldr,
		F3(
			function (key, value, keyList) {
				return A2(elm$core$List$cons, key, keyList);
			}),
		_List_Nil,
		dict);
};
var elm$core$Set$toList = function (_n0) {
	var dict = _n0.a;
	return elm$core$Dict$keys(dict);
};
var elm$core$Elm$JsArray$foldr = _JsArray_foldr;
var elm$core$Array$foldr = F3(
	function (func, baseCase, _n0) {
		var tree = _n0.c;
		var tail = _n0.d;
		var helper = F2(
			function (node, acc) {
				if (node.$ === 'SubTree') {
					var subTree = node.a;
					return A3(elm$core$Elm$JsArray$foldr, helper, acc, subTree);
				} else {
					var values = node.a;
					return A3(elm$core$Elm$JsArray$foldr, func, acc, values);
				}
			});
		return A3(
			elm$core$Elm$JsArray$foldr,
			helper,
			A3(elm$core$Elm$JsArray$foldr, func, baseCase, tail),
			tree);
	});
var elm$core$Array$toList = function (array) {
	return A3(elm$core$Array$foldr, elm$core$List$cons, _List_Nil, array);
};
var elm$core$Basics$add = _Basics_add;
var elm$core$Basics$identity = function (x) {
	return x;
};
var author$project$Main$jsAddBoxed = F2(
	function (_n0, _n1) {
		var a = _n0.a;
		var b = _n1.a;
		return author$project$Main$Box(a + b);
	});
var author$project$Main$jsAddUnboxed = F2(
	function (a, b) {
		return a + b;
	});
var elm_explorations$benchmark$Benchmark$Benchmark$Series = F2(
	function (a, b) {
		return {$: 'Series', a: a, b: b};
	});
var elm_explorations$benchmark$Benchmark$LowLevel$StackOverflow = {$: 'StackOverflow'};
var elm_explorations$benchmark$Benchmark$LowLevel$UnknownError = function (a) {
	return {$: 'UnknownError', a: a};
};
var elm_explorations$benchmark$Benchmark$LowLevel$operation = function (fn) {
	return _Benchmark_operation(fn);
};
var elm_explorations$benchmark$Benchmark$Status$Cold = {$: 'Cold'};
var elm_explorations$benchmark$Benchmark$compare = F5(
	function (name, name1, fn1, name2, fn2) {
		return A2(
			elm_explorations$benchmark$Benchmark$Benchmark$Series,
			name,
			_List_fromArray(
				[
					_Utils_Tuple3(
					name1,
					elm_explorations$benchmark$Benchmark$LowLevel$operation(fn1),
					elm_explorations$benchmark$Benchmark$Status$Cold),
					_Utils_Tuple3(
					name2,
					elm_explorations$benchmark$Benchmark$LowLevel$operation(fn2),
					elm_explorations$benchmark$Benchmark$Status$Cold)
				]));
	});
var author$project$Main$addBoxedVsUnboxed = A5(
	elm_explorations$benchmark$Benchmark$compare,
	'Add',
	'JS Boxed',
	function (_n0) {
		return A2(
			author$project$Main$jsAddBoxed,
			author$project$Main$Box(123),
			author$project$Main$Box(456));
	},
	'JS Unboxed',
	function (_n1) {
		return A2(author$project$Main$jsAddUnboxed, 123, 456);
	});
var author$project$Main$wasmAdd = author$project$Main$jsAddUnboxed;
var author$project$Main$addJsVsWasm = A5(
	elm_explorations$benchmark$Benchmark$compare,
	'Add boxed',
	'Wasm',
	function (_n0) {
		return A2(author$project$Main$wasmAdd, 123, 456);
	},
	'JS',
	function (_n1) {
		return A2(
			author$project$Main$jsAddBoxed,
			author$project$Main$Box(123),
			author$project$Main$Box(456));
	});
var author$project$Main$wasmAddUnboxed = author$project$Main$jsAddUnboxed;
var author$project$Main$addJsVsWasmUnboxed = A5(
	elm_explorations$benchmark$Benchmark$compare,
	'Add unboxed',
	'Wasm',
	function (_n0) {
		return A2(author$project$Main$wasmAddUnboxed, 123, 456);
	},
	'JS',
	function (_n1) {
		return A2(author$project$Main$jsAddUnboxed, 123, 456);
	});
var author$project$Main$jsAddUnboxedNoA2 = author$project$Main$jsAddUnboxed;
var author$project$Main$wasmAddUnboxedNoA2 = author$project$Main$wasmAddUnboxed;
var author$project$Main$addJsVsWasmWithoutA2 = A5(
	elm_explorations$benchmark$Benchmark$compare,
	'Add unboxed without A2',
	'Wasm',
	function (_n0) {
		return author$project$Main$wasmAddUnboxedNoA2(123, 456);
	},
	'JS',
	function (_n1) {
		return author$project$Main$jsAddUnboxedNoA2(123, 456);
	});
var author$project$Main$count = 100;
var elm$core$Basics$eq = _Utils_equal;
var elm$core$Basics$sub = _Basics_sub;
var author$project$Main$jsCountBoxed = function (_n0) {
	jsCountBoxed:
	while (true) {
		var remaining = _n0.a;
		if (!remaining) {
			return _Utils_Tuple0;
		} else {
			var $temp$_n0 = author$project$Main$Box(remaining - 1);
			_n0 = $temp$_n0;
			continue jsCountBoxed;
		}
	}
};
var author$project$Main$jsCountUnboxed = function (remaining) {
	jsCountUnboxed:
	while (true) {
		if (!remaining) {
			return _Utils_Tuple0;
		} else {
			var $temp$remaining = remaining - 1;
			remaining = $temp$remaining;
			continue jsCountUnboxed;
		}
	}
};
var elm$core$Basics$append = _Utils_append;
var elm$core$Maybe$Just = function (a) {
	return {$: 'Just', a: a};
};
var elm$core$Maybe$Nothing = {$: 'Nothing'};
var elm$core$String$fromInt = _String_fromNumber;
var author$project$Main$countBoxedVsUnboxed = A5(
	elm_explorations$benchmark$Benchmark$compare,
	'Count to ' + (elm$core$String$fromInt(author$project$Main$count) + ' (JS only)'),
	'JS Boxed',
	function (_n0) {
		return author$project$Main$jsCountBoxed(
			author$project$Main$Box(author$project$Main$count));
	},
	'JS Unboxed',
	function (_n1) {
		return author$project$Main$jsCountUnboxed(author$project$Main$count);
	});
var author$project$Main$wasmCount = author$project$Main$jsCountUnboxed;
var author$project$Main$countJsVsWasm = A5(
	elm_explorations$benchmark$Benchmark$compare,
	'Count to ' + (elm$core$String$fromInt(author$project$Main$count) + ' (Wasm vs JS)'),
	'Wasm Boxed',
	function (_n0) {
		return author$project$Main$wasmCount(author$project$Main$count);
	},
	'JS Boxed',
	function (_n1) {
		return author$project$Main$jsCountBoxed(
			author$project$Main$Box(author$project$Main$count));
	});
var author$project$Main$wasmCountNoTCE = author$project$Main$jsCountUnboxed;
var author$project$Main$countTCE = A5(
	elm_explorations$benchmark$Benchmark$compare,
	'Wasm tail call elimination (count)',
	'Tail call eliminated',
	function (_n0) {
		return author$project$Main$wasmCount(author$project$Main$count);
	},
	'Not eliminated',
	function (_n1) {
		return author$project$Main$wasmCountNoTCE(author$project$Main$count);
	});
var elm$core$Basics$apL = F2(
	function (f, x) {
		return f(x);
	});
var elm_explorations$benchmark$Benchmark$Benchmark$Group = F2(
	function (a, b) {
		return {$: 'Group', a: a, b: b};
	});
var elm_explorations$benchmark$Benchmark$describe = elm_explorations$benchmark$Benchmark$Benchmark$Group;
var author$project$Main$suite = A2(
	elm_explorations$benchmark$Benchmark$describe,
	'',
	_Utils_ap(
		_List_Nil,
		_Utils_ap(
			_List_fromArray(
				[author$project$Main$addJsVsWasm]),
			_Utils_ap(
				_List_fromArray(
					[author$project$Main$addJsVsWasmUnboxed]),
				_Utils_ap(
					_List_fromArray(
						[author$project$Main$addJsVsWasmWithoutA2]),
					_Utils_ap(
						_List_fromArray(
							[author$project$Main$addBoxedVsUnboxed]),
						_Utils_ap(
							_List_fromArray(
								[author$project$Main$countJsVsWasm]),
							_Utils_ap(
								_List_fromArray(
									[author$project$Main$countBoxedVsUnboxed]),
								_Utils_ap(
									_List_fromArray(
										[author$project$Main$countTCE]),
									_List_Nil)))))))));
var elm$core$Array$branchFactor = 32;
var elm$core$Array$Array_elm_builtin = F4(
	function (a, b, c, d) {
		return {$: 'Array_elm_builtin', a: a, b: b, c: c, d: d};
	});
var elm$core$Basics$ceiling = _Basics_ceiling;
var elm$core$Basics$fdiv = _Basics_fdiv;
var elm$core$Basics$logBase = F2(
	function (base, number) {
		return _Basics_log(number) / _Basics_log(base);
	});
var elm$core$Basics$toFloat = _Basics_toFloat;
var elm$core$Array$shiftStep = elm$core$Basics$ceiling(
	A2(elm$core$Basics$logBase, 2, elm$core$Array$branchFactor));
var elm$core$Elm$JsArray$empty = _JsArray_empty;
var elm$core$Array$empty = A4(elm$core$Array$Array_elm_builtin, 0, elm$core$Array$shiftStep, elm$core$Elm$JsArray$empty, elm$core$Elm$JsArray$empty);
var elm$core$Array$Leaf = function (a) {
	return {$: 'Leaf', a: a};
};
var elm$core$Array$SubTree = function (a) {
	return {$: 'SubTree', a: a};
};
var elm$core$Elm$JsArray$initializeFromList = _JsArray_initializeFromList;
var elm$core$List$foldl = F3(
	function (func, acc, list) {
		foldl:
		while (true) {
			if (!list.b) {
				return acc;
			} else {
				var x = list.a;
				var xs = list.b;
				var $temp$func = func,
					$temp$acc = A2(func, x, acc),
					$temp$list = xs;
				func = $temp$func;
				acc = $temp$acc;
				list = $temp$list;
				continue foldl;
			}
		}
	});
var elm$core$List$reverse = function (list) {
	return A3(elm$core$List$foldl, elm$core$List$cons, _List_Nil, list);
};
var elm$core$Array$compressNodes = F2(
	function (nodes, acc) {
		compressNodes:
		while (true) {
			var _n0 = A2(elm$core$Elm$JsArray$initializeFromList, elm$core$Array$branchFactor, nodes);
			var node = _n0.a;
			var remainingNodes = _n0.b;
			var newAcc = A2(
				elm$core$List$cons,
				elm$core$Array$SubTree(node),
				acc);
			if (!remainingNodes.b) {
				return elm$core$List$reverse(newAcc);
			} else {
				var $temp$nodes = remainingNodes,
					$temp$acc = newAcc;
				nodes = $temp$nodes;
				acc = $temp$acc;
				continue compressNodes;
			}
		}
	});
var elm$core$Basics$apR = F2(
	function (x, f) {
		return f(x);
	});
var elm$core$Tuple$first = function (_n0) {
	var x = _n0.a;
	return x;
};
var elm$core$Array$treeFromBuilder = F2(
	function (nodeList, nodeListSize) {
		treeFromBuilder:
		while (true) {
			var newNodeSize = elm$core$Basics$ceiling(nodeListSize / elm$core$Array$branchFactor);
			if (newNodeSize === 1) {
				return A2(elm$core$Elm$JsArray$initializeFromList, elm$core$Array$branchFactor, nodeList).a;
			} else {
				var $temp$nodeList = A2(elm$core$Array$compressNodes, nodeList, _List_Nil),
					$temp$nodeListSize = newNodeSize;
				nodeList = $temp$nodeList;
				nodeListSize = $temp$nodeListSize;
				continue treeFromBuilder;
			}
		}
	});
var elm$core$Basics$floor = _Basics_floor;
var elm$core$Basics$gt = _Utils_gt;
var elm$core$Basics$max = F2(
	function (x, y) {
		return (_Utils_cmp(x, y) > 0) ? x : y;
	});
var elm$core$Basics$mul = _Basics_mul;
var elm$core$Elm$JsArray$length = _JsArray_length;
var elm$core$Array$builderToArray = F2(
	function (reverseNodeList, builder) {
		if (!builder.nodeListSize) {
			return A4(
				elm$core$Array$Array_elm_builtin,
				elm$core$Elm$JsArray$length(builder.tail),
				elm$core$Array$shiftStep,
				elm$core$Elm$JsArray$empty,
				builder.tail);
		} else {
			var treeLen = builder.nodeListSize * elm$core$Array$branchFactor;
			var depth = elm$core$Basics$floor(
				A2(elm$core$Basics$logBase, elm$core$Array$branchFactor, treeLen - 1));
			var correctNodeList = reverseNodeList ? elm$core$List$reverse(builder.nodeList) : builder.nodeList;
			var tree = A2(elm$core$Array$treeFromBuilder, correctNodeList, builder.nodeListSize);
			return A4(
				elm$core$Array$Array_elm_builtin,
				elm$core$Elm$JsArray$length(builder.tail) + treeLen,
				A2(elm$core$Basics$max, 5, depth * elm$core$Array$shiftStep),
				tree,
				builder.tail);
		}
	});
var elm$core$Basics$False = {$: 'False'};
var elm$core$Basics$idiv = _Basics_idiv;
var elm$core$Basics$lt = _Utils_lt;
var elm$core$Elm$JsArray$initialize = _JsArray_initialize;
var elm$core$Array$initializeHelp = F5(
	function (fn, fromIndex, len, nodeList, tail) {
		initializeHelp:
		while (true) {
			if (fromIndex < 0) {
				return A2(
					elm$core$Array$builderToArray,
					false,
					{nodeList: nodeList, nodeListSize: (len / elm$core$Array$branchFactor) | 0, tail: tail});
			} else {
				var leaf = elm$core$Array$Leaf(
					A3(elm$core$Elm$JsArray$initialize, elm$core$Array$branchFactor, fromIndex, fn));
				var $temp$fn = fn,
					$temp$fromIndex = fromIndex - elm$core$Array$branchFactor,
					$temp$len = len,
					$temp$nodeList = A2(elm$core$List$cons, leaf, nodeList),
					$temp$tail = tail;
				fn = $temp$fn;
				fromIndex = $temp$fromIndex;
				len = $temp$len;
				nodeList = $temp$nodeList;
				tail = $temp$tail;
				continue initializeHelp;
			}
		}
	});
var elm$core$Basics$le = _Utils_le;
var elm$core$Basics$remainderBy = _Basics_remainderBy;
var elm$core$Array$initialize = F2(
	function (len, fn) {
		if (len <= 0) {
			return elm$core$Array$empty;
		} else {
			var tailLen = len % elm$core$Array$branchFactor;
			var tail = A3(elm$core$Elm$JsArray$initialize, tailLen, len - tailLen, fn);
			var initialFromIndex = (len - tailLen) - elm$core$Array$branchFactor;
			return A5(elm$core$Array$initializeHelp, fn, initialFromIndex, len, _List_Nil, tail);
		}
	});
var elm$core$Result$Err = function (a) {
	return {$: 'Err', a: a};
};
var elm$core$Result$Ok = function (a) {
	return {$: 'Ok', a: a};
};
var elm$core$Basics$True = {$: 'True'};
var elm$core$Result$isOk = function (result) {
	if (result.$ === 'Ok') {
		return true;
	} else {
		return false;
	}
};
var elm$json$Json$Decode$Failure = F2(
	function (a, b) {
		return {$: 'Failure', a: a, b: b};
	});
var elm$json$Json$Decode$Field = F2(
	function (a, b) {
		return {$: 'Field', a: a, b: b};
	});
var elm$json$Json$Decode$Index = F2(
	function (a, b) {
		return {$: 'Index', a: a, b: b};
	});
var elm$json$Json$Decode$OneOf = function (a) {
	return {$: 'OneOf', a: a};
};
var elm$core$Basics$and = _Basics_and;
var elm$core$Basics$or = _Basics_or;
var elm$core$Char$toCode = _Char_toCode;
var elm$core$Char$isLower = function (_char) {
	var code = elm$core$Char$toCode(_char);
	return (97 <= code) && (code <= 122);
};
var elm$core$Char$isUpper = function (_char) {
	var code = elm$core$Char$toCode(_char);
	return (code <= 90) && (65 <= code);
};
var elm$core$Char$isAlpha = function (_char) {
	return elm$core$Char$isLower(_char) || elm$core$Char$isUpper(_char);
};
var elm$core$Char$isDigit = function (_char) {
	var code = elm$core$Char$toCode(_char);
	return (code <= 57) && (48 <= code);
};
var elm$core$Char$isAlphaNum = function (_char) {
	return elm$core$Char$isLower(_char) || (elm$core$Char$isUpper(_char) || elm$core$Char$isDigit(_char));
};
var elm$core$List$length = function (xs) {
	return A3(
		elm$core$List$foldl,
		F2(
			function (_n0, i) {
				return i + 1;
			}),
		0,
		xs);
};
var elm$core$List$map2 = _List_map2;
var elm$core$List$rangeHelp = F3(
	function (lo, hi, list) {
		rangeHelp:
		while (true) {
			if (_Utils_cmp(lo, hi) < 1) {
				var $temp$lo = lo,
					$temp$hi = hi - 1,
					$temp$list = A2(elm$core$List$cons, hi, list);
				lo = $temp$lo;
				hi = $temp$hi;
				list = $temp$list;
				continue rangeHelp;
			} else {
				return list;
			}
		}
	});
var elm$core$List$range = F2(
	function (lo, hi) {
		return A3(elm$core$List$rangeHelp, lo, hi, _List_Nil);
	});
var elm$core$List$indexedMap = F2(
	function (f, xs) {
		return A3(
			elm$core$List$map2,
			f,
			A2(
				elm$core$List$range,
				0,
				elm$core$List$length(xs) - 1),
			xs);
	});
var elm$core$String$all = _String_all;
var elm$core$String$join = F2(
	function (sep, chunks) {
		return A2(
			_String_join,
			sep,
			_List_toArray(chunks));
	});
var elm$core$String$uncons = _String_uncons;
var elm$core$String$split = F2(
	function (sep, string) {
		return _List_fromArray(
			A2(_String_split, sep, string));
	});
var elm$json$Json$Decode$indent = function (str) {
	return A2(
		elm$core$String$join,
		'\n    ',
		A2(elm$core$String$split, '\n', str));
};
var elm$json$Json$Encode$encode = _Json_encode;
var elm$json$Json$Decode$errorOneOf = F2(
	function (i, error) {
		return '\n\n(' + (elm$core$String$fromInt(i + 1) + (') ' + elm$json$Json$Decode$indent(
			elm$json$Json$Decode$errorToString(error))));
	});
var elm$json$Json$Decode$errorToString = function (error) {
	return A2(elm$json$Json$Decode$errorToStringHelp, error, _List_Nil);
};
var elm$json$Json$Decode$errorToStringHelp = F2(
	function (error, context) {
		errorToStringHelp:
		while (true) {
			switch (error.$) {
				case 'Field':
					var f = error.a;
					var err = error.b;
					var isSimple = function () {
						var _n1 = elm$core$String$uncons(f);
						if (_n1.$ === 'Nothing') {
							return false;
						} else {
							var _n2 = _n1.a;
							var _char = _n2.a;
							var rest = _n2.b;
							return elm$core$Char$isAlpha(_char) && A2(elm$core$String$all, elm$core$Char$isAlphaNum, rest);
						}
					}();
					var fieldName = isSimple ? ('.' + f) : ('[\'' + (f + '\']'));
					var $temp$error = err,
						$temp$context = A2(elm$core$List$cons, fieldName, context);
					error = $temp$error;
					context = $temp$context;
					continue errorToStringHelp;
				case 'Index':
					var i = error.a;
					var err = error.b;
					var indexName = '[' + (elm$core$String$fromInt(i) + ']');
					var $temp$error = err,
						$temp$context = A2(elm$core$List$cons, indexName, context);
					error = $temp$error;
					context = $temp$context;
					continue errorToStringHelp;
				case 'OneOf':
					var errors = error.a;
					if (!errors.b) {
						return 'Ran into a Json.Decode.oneOf with no possibilities' + function () {
							if (!context.b) {
								return '!';
							} else {
								return ' at json' + A2(
									elm$core$String$join,
									'',
									elm$core$List$reverse(context));
							}
						}();
					} else {
						if (!errors.b.b) {
							var err = errors.a;
							var $temp$error = err,
								$temp$context = context;
							error = $temp$error;
							context = $temp$context;
							continue errorToStringHelp;
						} else {
							var starter = function () {
								if (!context.b) {
									return 'Json.Decode.oneOf';
								} else {
									return 'The Json.Decode.oneOf at json' + A2(
										elm$core$String$join,
										'',
										elm$core$List$reverse(context));
								}
							}();
							var introduction = starter + (' failed in the following ' + (elm$core$String$fromInt(
								elm$core$List$length(errors)) + ' ways:'));
							return A2(
								elm$core$String$join,
								'\n\n',
								A2(
									elm$core$List$cons,
									introduction,
									A2(elm$core$List$indexedMap, elm$json$Json$Decode$errorOneOf, errors)));
						}
					}
				default:
					var msg = error.a;
					var json = error.b;
					var introduction = function () {
						if (!context.b) {
							return 'Problem with the given value:\n\n';
						} else {
							return 'Problem with the value at json' + (A2(
								elm$core$String$join,
								'',
								elm$core$List$reverse(context)) + ':\n\n    ');
						}
					}();
					return introduction + (elm$json$Json$Decode$indent(
						A2(elm$json$Json$Encode$encode, 4, json)) + ('\n\n' + msg));
			}
		}
	});
var elm$json$Json$Decode$succeed = _Json_succeed;
var elm$browser$Browser$External = function (a) {
	return {$: 'External', a: a};
};
var elm$browser$Browser$Internal = function (a) {
	return {$: 'Internal', a: a};
};
var elm$browser$Browser$Dom$NotFound = function (a) {
	return {$: 'NotFound', a: a};
};
var elm$core$Basics$never = function (_n0) {
	never:
	while (true) {
		var nvr = _n0.a;
		var $temp$_n0 = nvr;
		_n0 = $temp$_n0;
		continue never;
	}
};
var elm$core$Task$Perform = function (a) {
	return {$: 'Perform', a: a};
};
var elm$core$Task$succeed = _Scheduler_succeed;
var elm$core$Task$init = elm$core$Task$succeed(_Utils_Tuple0);
var elm$core$List$foldrHelper = F4(
	function (fn, acc, ctr, ls) {
		if (!ls.b) {
			return acc;
		} else {
			var a = ls.a;
			var r1 = ls.b;
			if (!r1.b) {
				return A2(fn, a, acc);
			} else {
				var b = r1.a;
				var r2 = r1.b;
				if (!r2.b) {
					return A2(
						fn,
						a,
						A2(fn, b, acc));
				} else {
					var c = r2.a;
					var r3 = r2.b;
					if (!r3.b) {
						return A2(
							fn,
							a,
							A2(
								fn,
								b,
								A2(fn, c, acc)));
					} else {
						var d = r3.a;
						var r4 = r3.b;
						var res = (ctr > 500) ? A3(
							elm$core$List$foldl,
							fn,
							acc,
							elm$core$List$reverse(r4)) : A4(elm$core$List$foldrHelper, fn, acc, ctr + 1, r4);
						return A2(
							fn,
							a,
							A2(
								fn,
								b,
								A2(
									fn,
									c,
									A2(fn, d, res))));
					}
				}
			}
		}
	});
var elm$core$List$foldr = F3(
	function (fn, acc, ls) {
		return A4(elm$core$List$foldrHelper, fn, acc, 0, ls);
	});
var elm$core$List$map = F2(
	function (f, xs) {
		return A3(
			elm$core$List$foldr,
			F2(
				function (x, acc) {
					return A2(
						elm$core$List$cons,
						f(x),
						acc);
				}),
			_List_Nil,
			xs);
	});
var elm$core$Task$andThen = _Scheduler_andThen;
var elm$core$Task$map = F2(
	function (func, taskA) {
		return A2(
			elm$core$Task$andThen,
			function (a) {
				return elm$core$Task$succeed(
					func(a));
			},
			taskA);
	});
var elm$core$Task$map2 = F3(
	function (func, taskA, taskB) {
		return A2(
			elm$core$Task$andThen,
			function (a) {
				return A2(
					elm$core$Task$andThen,
					function (b) {
						return elm$core$Task$succeed(
							A2(func, a, b));
					},
					taskB);
			},
			taskA);
	});
var elm$core$Task$sequence = function (tasks) {
	return A3(
		elm$core$List$foldr,
		elm$core$Task$map2(elm$core$List$cons),
		elm$core$Task$succeed(_List_Nil),
		tasks);
};
var elm$core$Platform$sendToApp = _Platform_sendToApp;
var elm$core$Task$spawnCmd = F2(
	function (router, _n0) {
		var task = _n0.a;
		return _Scheduler_spawn(
			A2(
				elm$core$Task$andThen,
				elm$core$Platform$sendToApp(router),
				task));
	});
var elm$core$Task$onEffects = F3(
	function (router, commands, state) {
		return A2(
			elm$core$Task$map,
			function (_n0) {
				return _Utils_Tuple0;
			},
			elm$core$Task$sequence(
				A2(
					elm$core$List$map,
					elm$core$Task$spawnCmd(router),
					commands)));
	});
var elm$core$Task$onSelfMsg = F3(
	function (_n0, _n1, _n2) {
		return elm$core$Task$succeed(_Utils_Tuple0);
	});
var elm$core$Task$cmdMap = F2(
	function (tagger, _n0) {
		var task = _n0.a;
		return elm$core$Task$Perform(
			A2(elm$core$Task$map, tagger, task));
	});
_Platform_effectManagers['Task'] = _Platform_createManager(elm$core$Task$init, elm$core$Task$onEffects, elm$core$Task$onSelfMsg, elm$core$Task$cmdMap);
var elm$core$Task$command = _Platform_leaf('Task');
var elm$core$Task$perform = F2(
	function (toMessage, task) {
		return elm$core$Task$command(
			elm$core$Task$Perform(
				A2(elm$core$Task$map, toMessage, task)));
	});
var elm$json$Json$Decode$map = _Json_map1;
var elm$json$Json$Decode$map2 = _Json_map2;
var elm$virtual_dom$VirtualDom$toHandlerInt = function (handler) {
	switch (handler.$) {
		case 'Normal':
			return 0;
		case 'MayStopPropagation':
			return 1;
		case 'MayPreventDefault':
			return 2;
		default:
			return 3;
	}
};
var elm$core$String$length = _String_length;
var elm$core$String$slice = _String_slice;
var elm$core$String$dropLeft = F2(
	function (n, string) {
		return (n < 1) ? string : A3(
			elm$core$String$slice,
			n,
			elm$core$String$length(string),
			string);
	});
var elm$core$String$startsWith = _String_startsWith;
var elm$url$Url$Http = {$: 'Http'};
var elm$url$Url$Https = {$: 'Https'};
var elm$core$String$indexes = _String_indexes;
var elm$core$String$isEmpty = function (string) {
	return string === '';
};
var elm$core$String$left = F2(
	function (n, string) {
		return (n < 1) ? '' : A3(elm$core$String$slice, 0, n, string);
	});
var elm$core$String$contains = _String_contains;
var elm$core$String$toInt = _String_toInt;
var elm$url$Url$Url = F6(
	function (protocol, host, port_, path, query, fragment) {
		return {fragment: fragment, host: host, path: path, port_: port_, protocol: protocol, query: query};
	});
var elm$url$Url$chompBeforePath = F5(
	function (protocol, path, params, frag, str) {
		if (elm$core$String$isEmpty(str) || A2(elm$core$String$contains, '@', str)) {
			return elm$core$Maybe$Nothing;
		} else {
			var _n0 = A2(elm$core$String$indexes, ':', str);
			if (!_n0.b) {
				return elm$core$Maybe$Just(
					A6(elm$url$Url$Url, protocol, str, elm$core$Maybe$Nothing, path, params, frag));
			} else {
				if (!_n0.b.b) {
					var i = _n0.a;
					var _n1 = elm$core$String$toInt(
						A2(elm$core$String$dropLeft, i + 1, str));
					if (_n1.$ === 'Nothing') {
						return elm$core$Maybe$Nothing;
					} else {
						var port_ = _n1;
						return elm$core$Maybe$Just(
							A6(
								elm$url$Url$Url,
								protocol,
								A2(elm$core$String$left, i, str),
								port_,
								path,
								params,
								frag));
					}
				} else {
					return elm$core$Maybe$Nothing;
				}
			}
		}
	});
var elm$url$Url$chompBeforeQuery = F4(
	function (protocol, params, frag, str) {
		if (elm$core$String$isEmpty(str)) {
			return elm$core$Maybe$Nothing;
		} else {
			var _n0 = A2(elm$core$String$indexes, '/', str);
			if (!_n0.b) {
				return A5(elm$url$Url$chompBeforePath, protocol, '/', params, frag, str);
			} else {
				var i = _n0.a;
				return A5(
					elm$url$Url$chompBeforePath,
					protocol,
					A2(elm$core$String$dropLeft, i, str),
					params,
					frag,
					A2(elm$core$String$left, i, str));
			}
		}
	});
var elm$url$Url$chompBeforeFragment = F3(
	function (protocol, frag, str) {
		if (elm$core$String$isEmpty(str)) {
			return elm$core$Maybe$Nothing;
		} else {
			var _n0 = A2(elm$core$String$indexes, '?', str);
			if (!_n0.b) {
				return A4(elm$url$Url$chompBeforeQuery, protocol, elm$core$Maybe$Nothing, frag, str);
			} else {
				var i = _n0.a;
				return A4(
					elm$url$Url$chompBeforeQuery,
					protocol,
					elm$core$Maybe$Just(
						A2(elm$core$String$dropLeft, i + 1, str)),
					frag,
					A2(elm$core$String$left, i, str));
			}
		}
	});
var elm$url$Url$chompAfterProtocol = F2(
	function (protocol, str) {
		if (elm$core$String$isEmpty(str)) {
			return elm$core$Maybe$Nothing;
		} else {
			var _n0 = A2(elm$core$String$indexes, '#', str);
			if (!_n0.b) {
				return A3(elm$url$Url$chompBeforeFragment, protocol, elm$core$Maybe$Nothing, str);
			} else {
				var i = _n0.a;
				return A3(
					elm$url$Url$chompBeforeFragment,
					protocol,
					elm$core$Maybe$Just(
						A2(elm$core$String$dropLeft, i + 1, str)),
					A2(elm$core$String$left, i, str));
			}
		}
	});
var elm$url$Url$fromString = function (str) {
	return A2(elm$core$String$startsWith, 'http://', str) ? A2(
		elm$url$Url$chompAfterProtocol,
		elm$url$Url$Http,
		A2(elm$core$String$dropLeft, 7, str)) : (A2(elm$core$String$startsWith, 'https://', str) ? A2(
		elm$url$Url$chompAfterProtocol,
		elm$url$Url$Https,
		A2(elm$core$String$dropLeft, 8, str)) : elm$core$Maybe$Nothing);
};
var elm$browser$Browser$element = _Browser_element;
var elm$core$Basics$always = F2(
	function (a, _n0) {
		return a;
	});
var elm$core$Platform$Sub$batch = _Platform_batch;
var elm$core$Platform$Sub$none = elm$core$Platform$Sub$batch(_List_Nil);
var elm$core$Platform$Cmd$batch = _Platform_batch;
var elm$core$Platform$Cmd$none = elm$core$Platform$Cmd$batch(_List_Nil);
var elm$core$Basics$composeL = F3(
	function (g, f, x) {
		return g(
			f(x));
	});
var elm$core$Basics$not = _Basics_not;
var elm$core$List$any = F2(
	function (isOkay, list) {
		any:
		while (true) {
			if (!list.b) {
				return false;
			} else {
				var x = list.a;
				var xs = list.b;
				if (isOkay(x)) {
					return true;
				} else {
					var $temp$isOkay = isOkay,
						$temp$list = xs;
					isOkay = $temp$isOkay;
					list = $temp$list;
					continue any;
				}
			}
		}
	});
var elm$core$List$all = F2(
	function (isOkay, list) {
		return !A2(
			elm$core$List$any,
			A2(elm$core$Basics$composeL, elm$core$Basics$not, isOkay),
			list);
	});
var elm$core$Basics$clamp = F3(
	function (low, high, number) {
		return (_Utils_cmp(number, low) < 0) ? low : ((_Utils_cmp(number, high) > 0) ? high : number);
	});
var elm$core$Dict$foldl = F3(
	function (func, acc, dict) {
		foldl:
		while (true) {
			if (dict.$ === 'RBEmpty_elm_builtin') {
				return acc;
			} else {
				var key = dict.b;
				var value = dict.c;
				var left = dict.d;
				var right = dict.e;
				var $temp$func = func,
					$temp$acc = A3(
					func,
					key,
					value,
					A3(elm$core$Dict$foldl, func, acc, left)),
					$temp$dict = right;
				func = $temp$func;
				acc = $temp$acc;
				dict = $temp$dict;
				continue foldl;
			}
		}
	});
var elm_explorations$benchmark$Benchmark$Samples$count = function (_n0) {
	var samples = _n0.a;
	return A3(
		elm$core$Dict$foldl,
		F3(
			function (_n1, times, acc) {
				return elm$core$List$length(times) + acc;
			}),
		0,
		samples);
};
var elm_explorations$benchmark$Benchmark$Status$numBuckets = 25;
var elm_explorations$benchmark$Benchmark$Status$samplesPerBucket = 5;
var elm_explorations$benchmark$Benchmark$Status$progress = function (status) {
	switch (status.$) {
		case 'Cold':
			return 0;
		case 'Unsized':
			return 0;
		case 'Pending':
			var samples = status.b;
			return A3(
				elm$core$Basics$clamp,
				0,
				1,
				elm_explorations$benchmark$Benchmark$Samples$count(samples) / (elm_explorations$benchmark$Benchmark$Status$numBuckets * elm_explorations$benchmark$Benchmark$Status$samplesPerBucket));
		case 'Failure':
			return 1;
		default:
			return 1;
	}
};
var elm_explorations$benchmark$Benchmark$done = function (benchmark_) {
	switch (benchmark_.$) {
		case 'Single':
			var status = benchmark_.c;
			return elm_explorations$benchmark$Benchmark$Status$progress(status) === 1;
		case 'Series':
			var benchmarks = benchmark_.b;
			return A2(
				elm$core$List$all,
				elm$core$Basics$eq(1),
				A2(
					elm$core$List$map,
					elm_explorations$benchmark$Benchmark$Status$progress,
					A2(
						elm$core$List$map,
						function (_n1) {
							var status = _n1.c;
							return status;
						},
						benchmarks)));
		default:
			var benchmarks = benchmark_.b;
			return A2(elm$core$List$all, elm_explorations$benchmark$Benchmark$done, benchmarks);
	}
};
var elm$core$Basics$ge = _Utils_ge;
var elm$core$Basics$modBy = _Basics_modBy;
var elm$core$Task$onError = _Scheduler_onError;
var BrianHicks$elm_trend$Trend$Linear$Line = F2(
	function (slope, intercept) {
		return {intercept: intercept, slope: slope};
	});
var BrianHicks$elm_trend$Trend$Linear$Quick = function (a) {
	return {$: 'Quick', a: a};
};
var BrianHicks$elm_trend$Trend$Linear$Trend = F2(
	function (a, b) {
		return {$: 'Trend', a: a, b: b};
	});
var BrianHicks$elm_trend$Trend$Math$NeedMoreValues = function (a) {
	return {$: 'NeedMoreValues', a: a};
};
var BrianHicks$elm_trend$Trend$Math$AllZeros = {$: 'AllZeros'};
var elm$core$List$sum = function (numbers) {
	return A3(elm$core$List$foldl, elm$core$Basics$add, 0, numbers);
};
var BrianHicks$elm_trend$Trend$Math$mean = function (numbers) {
	if (!numbers.b) {
		return elm$core$Result$Err(
			BrianHicks$elm_trend$Trend$Math$NeedMoreValues(1));
	} else {
		return elm$core$Result$Ok(
			elm$core$List$sum(numbers) / elm$core$List$length(numbers));
	}
};
var elm$core$Basics$pow = _Basics_pow;
var elm$core$Basics$sqrt = _Basics_sqrt;
var elm$core$Result$andThen = F2(
	function (callback, result) {
		if (result.$ === 'Ok') {
			var value = result.a;
			return callback(value);
		} else {
			var msg = result.a;
			return elm$core$Result$Err(msg);
		}
	});
var elm$core$Result$map = F2(
	function (func, ra) {
		if (ra.$ === 'Ok') {
			var a = ra.a;
			return elm$core$Result$Ok(
				func(a));
		} else {
			var e = ra.a;
			return elm$core$Result$Err(e);
		}
	});
var BrianHicks$elm_trend$Trend$Math$stddev = function (numbers) {
	var helper = function (seriesMean) {
		return A2(
			elm$core$Result$map,
			elm$core$Basics$sqrt,
			BrianHicks$elm_trend$Trend$Math$mean(
				A2(
					elm$core$List$map,
					function (n) {
						return A2(elm$core$Basics$pow, n - seriesMean, 2);
					},
					numbers)));
	};
	return A2(
		elm$core$Result$andThen,
		helper,
		BrianHicks$elm_trend$Trend$Math$mean(numbers));
};
var elm$core$Basics$isNaN = _Basics_isNaN;
var elm$core$List$unzip = function (pairs) {
	var step = F2(
		function (_n0, _n1) {
			var x = _n0.a;
			var y = _n0.b;
			var xs = _n1.a;
			var ys = _n1.b;
			return _Utils_Tuple2(
				A2(elm$core$List$cons, x, xs),
				A2(elm$core$List$cons, y, ys));
		});
	return A3(
		elm$core$List$foldr,
		step,
		_Utils_Tuple2(_List_Nil, _List_Nil),
		pairs);
};
var elm$core$Result$map2 = F3(
	function (func, ra, rb) {
		if (ra.$ === 'Err') {
			var x = ra.a;
			return elm$core$Result$Err(x);
		} else {
			var a = ra.a;
			if (rb.$ === 'Err') {
				var x = rb.a;
				return elm$core$Result$Err(x);
			} else {
				var b = rb.a;
				return elm$core$Result$Ok(
					A2(func, a, b));
			}
		}
	});
var BrianHicks$elm_trend$Trend$Math$correlation = function (values) {
	if (!values.b) {
		return elm$core$Result$Err(
			BrianHicks$elm_trend$Trend$Math$NeedMoreValues(2));
	} else {
		if (!values.b.b) {
			return elm$core$Result$Err(
				BrianHicks$elm_trend$Trend$Math$NeedMoreValues(2));
		} else {
			var standardize = F3(
				function (meanResult, stddevResult, series) {
					return A3(
						elm$core$Result$map2,
						F2(
							function (meanValue, stddevValue) {
								return A2(
									elm$core$List$map,
									function (point) {
										return (point - meanValue) / stddevValue;
									},
									series);
							}),
						meanResult,
						stddevResult);
				});
			var _n1 = elm$core$List$unzip(values);
			var xs = _n1.a;
			var ys = _n1.b;
			var summedProduct = A2(
				elm$core$Result$map,
				elm$core$List$sum,
				A3(
					elm$core$Result$map2,
					F2(
						function (stdX, stdY) {
							return A3(elm$core$List$map2, elm$core$Basics$mul, stdX, stdY);
						}),
					A3(
						standardize,
						BrianHicks$elm_trend$Trend$Math$mean(xs),
						BrianHicks$elm_trend$Trend$Math$stddev(xs),
						xs),
					A3(
						standardize,
						BrianHicks$elm_trend$Trend$Math$mean(ys),
						BrianHicks$elm_trend$Trend$Math$stddev(ys),
						ys)));
			return A2(
				elm$core$Result$andThen,
				function (val) {
					return elm$core$Basics$isNaN(val) ? elm$core$Result$Err(BrianHicks$elm_trend$Trend$Math$AllZeros) : elm$core$Result$Ok(val);
				},
				A2(
					elm$core$Result$map,
					function (sum) {
						return sum / elm$core$List$length(values);
					},
					summedProduct));
		}
	}
};
var elm$core$Result$map3 = F4(
	function (func, ra, rb, rc) {
		if (ra.$ === 'Err') {
			var x = ra.a;
			return elm$core$Result$Err(x);
		} else {
			var a = ra.a;
			if (rb.$ === 'Err') {
				var x = rb.a;
				return elm$core$Result$Err(x);
			} else {
				var b = rb.a;
				if (rc.$ === 'Err') {
					var x = rc.a;
					return elm$core$Result$Err(x);
				} else {
					var c = rc.a;
					return elm$core$Result$Ok(
						A3(func, a, b, c));
				}
			}
		}
	});
var BrianHicks$elm_trend$Trend$Linear$quick = function (values) {
	if (!values.b) {
		return elm$core$Result$Err(
			BrianHicks$elm_trend$Trend$Math$NeedMoreValues(2));
	} else {
		if (!values.b.b) {
			return elm$core$Result$Err(
				BrianHicks$elm_trend$Trend$Math$NeedMoreValues(2));
		} else {
			var _n1 = elm$core$List$unzip(values);
			var xs = _n1.a;
			var ys = _n1.b;
			var slopeResult = A4(
				elm$core$Result$map3,
				F3(
					function (correl, stddevY, stddevX) {
						return (correl * stddevY) / stddevX;
					}),
				BrianHicks$elm_trend$Trend$Math$correlation(values),
				BrianHicks$elm_trend$Trend$Math$stddev(ys),
				BrianHicks$elm_trend$Trend$Math$stddev(xs));
			var intercept = A4(
				elm$core$Result$map3,
				F3(
					function (meanY, slope, meanX) {
						return meanY - (slope * meanX);
					}),
				BrianHicks$elm_trend$Trend$Math$mean(ys),
				slopeResult,
				BrianHicks$elm_trend$Trend$Math$mean(xs));
			return A2(
				elm$core$Result$map,
				function (trendLine) {
					return A2(
						BrianHicks$elm_trend$Trend$Linear$Trend,
						trendLine,
						BrianHicks$elm_trend$Trend$Linear$Quick(values));
				},
				A3(elm$core$Result$map2, BrianHicks$elm_trend$Trend$Linear$Line, slopeResult, intercept));
		}
	}
};
var elm$core$Tuple$mapFirst = F2(
	function (func, _n0) {
		var x = _n0.a;
		var y = _n0.b;
		return _Utils_Tuple2(
			func(x),
			y);
	});
var elm$core$Tuple$mapSecond = F2(
	function (func, _n0) {
		var x = _n0.a;
		var y = _n0.b;
		return _Utils_Tuple2(
			x,
			func(y));
	});
var BrianHicks$elm_trend$Trend$Linear$line = function (_n0) {
	var precalculated = _n0.a;
	return precalculated;
};
var BrianHicks$elm_trend$Trend$Linear$predictY = F2(
	function (_n0, x) {
		var slope = _n0.slope;
		var intercept = _n0.intercept;
		return (slope * x) + intercept;
	});
var BrianHicks$elm_trend$Trend$Linear$Robust = F2(
	function (a, b) {
		return {$: 'Robust', a: a, b: b};
	});
var elm$core$List$drop = F2(
	function (n, list) {
		drop:
		while (true) {
			if (n <= 0) {
				return list;
			} else {
				if (!list.b) {
					return list;
				} else {
					var x = list.a;
					var xs = list.b;
					var $temp$n = n - 1,
						$temp$list = xs;
					n = $temp$n;
					list = $temp$list;
					continue drop;
				}
			}
		}
	});
var elm$core$List$head = function (list) {
	if (list.b) {
		var x = list.a;
		var xs = list.b;
		return elm$core$Maybe$Just(x);
	} else {
		return elm$core$Maybe$Nothing;
	}
};
var elm$core$List$takeReverse = F3(
	function (n, list, kept) {
		takeReverse:
		while (true) {
			if (n <= 0) {
				return kept;
			} else {
				if (!list.b) {
					return kept;
				} else {
					var x = list.a;
					var xs = list.b;
					var $temp$n = n - 1,
						$temp$list = xs,
						$temp$kept = A2(elm$core$List$cons, x, kept);
					n = $temp$n;
					list = $temp$list;
					kept = $temp$kept;
					continue takeReverse;
				}
			}
		}
	});
var elm$core$List$takeTailRec = F2(
	function (n, list) {
		return elm$core$List$reverse(
			A3(elm$core$List$takeReverse, n, list, _List_Nil));
	});
var elm$core$List$takeFast = F3(
	function (ctr, n, list) {
		if (n <= 0) {
			return _List_Nil;
		} else {
			var _n0 = _Utils_Tuple2(n, list);
			_n0$1:
			while (true) {
				_n0$5:
				while (true) {
					if (!_n0.b.b) {
						return list;
					} else {
						if (_n0.b.b.b) {
							switch (_n0.a) {
								case 1:
									break _n0$1;
								case 2:
									var _n2 = _n0.b;
									var x = _n2.a;
									var _n3 = _n2.b;
									var y = _n3.a;
									return _List_fromArray(
										[x, y]);
								case 3:
									if (_n0.b.b.b.b) {
										var _n4 = _n0.b;
										var x = _n4.a;
										var _n5 = _n4.b;
										var y = _n5.a;
										var _n6 = _n5.b;
										var z = _n6.a;
										return _List_fromArray(
											[x, y, z]);
									} else {
										break _n0$5;
									}
								default:
									if (_n0.b.b.b.b && _n0.b.b.b.b.b) {
										var _n7 = _n0.b;
										var x = _n7.a;
										var _n8 = _n7.b;
										var y = _n8.a;
										var _n9 = _n8.b;
										var z = _n9.a;
										var _n10 = _n9.b;
										var w = _n10.a;
										var tl = _n10.b;
										return (ctr > 1000) ? A2(
											elm$core$List$cons,
											x,
											A2(
												elm$core$List$cons,
												y,
												A2(
													elm$core$List$cons,
													z,
													A2(
														elm$core$List$cons,
														w,
														A2(elm$core$List$takeTailRec, n - 4, tl))))) : A2(
											elm$core$List$cons,
											x,
											A2(
												elm$core$List$cons,
												y,
												A2(
													elm$core$List$cons,
													z,
													A2(
														elm$core$List$cons,
														w,
														A3(elm$core$List$takeFast, ctr + 1, n - 4, tl)))));
									} else {
										break _n0$5;
									}
							}
						} else {
							if (_n0.a === 1) {
								break _n0$1;
							} else {
								break _n0$5;
							}
						}
					}
				}
				return list;
			}
			var _n1 = _n0.b;
			var x = _n1.a;
			return _List_fromArray(
				[x]);
		}
	});
var elm$core$List$take = F2(
	function (n, list) {
		return A3(elm$core$List$takeFast, 0, n, list);
	});
var elm$core$Result$toMaybe = function (result) {
	if (result.$ === 'Ok') {
		var v = result.a;
		return elm$core$Maybe$Just(v);
	} else {
		return elm$core$Maybe$Nothing;
	}
};
var BrianHicks$elm_trend$Trend$Linear$percentile = F2(
	function (k, xs) {
		var index = elm$core$List$length(xs) * k;
		return (!(index - elm$core$Basics$floor(index))) ? elm$core$List$head(
			A2(
				elm$core$List$drop,
				elm$core$Basics$ceiling(index) - 1,
				xs)) : elm$core$Result$toMaybe(
			BrianHicks$elm_trend$Trend$Math$mean(
				A2(
					elm$core$List$take,
					2,
					A2(
						elm$core$List$drop,
						elm$core$Basics$floor(index) - 1,
						xs))));
	});
var elm$core$List$sortBy = _List_sortBy;
var elm$core$List$sort = function (xs) {
	return A2(elm$core$List$sortBy, elm$core$Basics$identity, xs);
};
var elm$core$Maybe$andThen = F2(
	function (callback, maybeValue) {
		if (maybeValue.$ === 'Just') {
			var value = maybeValue.a;
			return callback(value);
		} else {
			return elm$core$Maybe$Nothing;
		}
	});
var elm$core$Maybe$map = F2(
	function (f, maybe) {
		if (maybe.$ === 'Just') {
			var value = maybe.a;
			return elm$core$Maybe$Just(
				f(value));
		} else {
			return elm$core$Maybe$Nothing;
		}
	});
var elm$core$Maybe$map2 = F3(
	function (func, ma, mb) {
		if (ma.$ === 'Nothing') {
			return elm$core$Maybe$Nothing;
		} else {
			var a = ma.a;
			if (mb.$ === 'Nothing') {
				return elm$core$Maybe$Nothing;
			} else {
				var b = mb.a;
				return elm$core$Maybe$Just(
					A2(func, a, b));
			}
		}
	});
var BrianHicks$elm_trend$Trend$Linear$theilSenLine = F3(
	function (pct, slopes, points) {
		var slope = A2(BrianHicks$elm_trend$Trend$Linear$percentile, pct, slopes);
		var intercept = A2(
			elm$core$Maybe$andThen,
			BrianHicks$elm_trend$Trend$Linear$percentile(pct),
			A2(
				elm$core$Maybe$map,
				elm$core$List$sort,
				A2(
					elm$core$Maybe$map,
					function (m) {
						return A2(
							elm$core$List$map,
							function (_n0) {
								var x = _n0.a;
								var y = _n0.b;
								return y - (m * x);
							},
							points);
					},
					slope)));
		return A3(elm$core$Maybe$map2, BrianHicks$elm_trend$Trend$Linear$Line, slope, intercept);
	});
var elm$core$Basics$isInfinite = _Basics_isInfinite;
var elm$core$List$filter = F2(
	function (isGood, list) {
		return A3(
			elm$core$List$foldr,
			F2(
				function (x, xs) {
					return isGood(x) ? A2(elm$core$List$cons, x, xs) : xs;
				}),
			_List_Nil,
			list);
	});
var elm$core$Maybe$map3 = F4(
	function (func, ma, mb, mc) {
		if (ma.$ === 'Nothing') {
			return elm$core$Maybe$Nothing;
		} else {
			var a = ma.a;
			if (mb.$ === 'Nothing') {
				return elm$core$Maybe$Nothing;
			} else {
				var b = mb.a;
				if (mc.$ === 'Nothing') {
					return elm$core$Maybe$Nothing;
				} else {
					var c = mc.a;
					return elm$core$Maybe$Just(
						A3(func, a, b, c));
				}
			}
		}
	});
var elm$core$Result$fromMaybe = F2(
	function (err, maybe) {
		if (maybe.$ === 'Just') {
			var v = maybe.a;
			return elm$core$Result$Ok(v);
		} else {
			return elm$core$Result$Err(err);
		}
	});
var BrianHicks$elm_trend$Trend$Linear$robust = function (values) {
	if (!values.b) {
		return elm$core$Result$Err(
			BrianHicks$elm_trend$Trend$Math$NeedMoreValues(2));
	} else {
		if (!values.b.b) {
			return elm$core$Result$Err(
				BrianHicks$elm_trend$Trend$Math$NeedMoreValues(2));
		} else {
			var slopes = elm$core$List$sort(
				A3(
					elm$core$List$foldl,
					F2(
						function (_n1, acc1) {
							var x = _n1.a;
							var y = _n1.b;
							return A3(
								elm$core$List$foldl,
								F2(
									function (_n2, acc2) {
										var x1 = _n2.a;
										var y1 = _n2.b;
										var res = (y - y1) / (x - x1);
										return elm$core$Basics$isNaN(res) ? acc2 : A2(elm$core$List$cons, res, acc2);
									}),
								acc1,
								values);
						}),
					_List_Nil,
					values));
			var finiteSlopes = A2(
				elm$core$List$filter,
				A2(elm$core$Basics$composeL, elm$core$Basics$not, elm$core$Basics$isInfinite),
				slopes);
			return A2(
				elm$core$Result$fromMaybe,
				BrianHicks$elm_trend$Trend$Math$AllZeros,
				A4(
					elm$core$Maybe$map3,
					F3(
						function (trendLine, lower, upper) {
							return A2(
								BrianHicks$elm_trend$Trend$Linear$Trend,
								trendLine,
								A2(BrianHicks$elm_trend$Trend$Linear$Robust, lower, upper));
						}),
					A3(BrianHicks$elm_trend$Trend$Linear$theilSenLine, 0.5, finiteSlopes, values),
					A3(BrianHicks$elm_trend$Trend$Linear$theilSenLine, 0.975, slopes, values),
					A3(BrianHicks$elm_trend$Trend$Linear$theilSenLine, 2.5e-2, slopes, values)));
		}
	}
};
var elm$core$Dict$RBEmpty_elm_builtin = {$: 'RBEmpty_elm_builtin'};
var elm$core$Dict$empty = elm$core$Dict$RBEmpty_elm_builtin;
var elm$core$Dict$Black = {$: 'Black'};
var elm$core$Dict$RBNode_elm_builtin = F5(
	function (a, b, c, d, e) {
		return {$: 'RBNode_elm_builtin', a: a, b: b, c: c, d: d, e: e};
	});
var elm$core$Basics$compare = _Utils_compare;
var elm$core$Dict$Red = {$: 'Red'};
var elm$core$Dict$balance = F5(
	function (color, key, value, left, right) {
		if ((right.$ === 'RBNode_elm_builtin') && (right.a.$ === 'Red')) {
			var _n1 = right.a;
			var rK = right.b;
			var rV = right.c;
			var rLeft = right.d;
			var rRight = right.e;
			if ((left.$ === 'RBNode_elm_builtin') && (left.a.$ === 'Red')) {
				var _n3 = left.a;
				var lK = left.b;
				var lV = left.c;
				var lLeft = left.d;
				var lRight = left.e;
				return A5(
					elm$core$Dict$RBNode_elm_builtin,
					elm$core$Dict$Red,
					key,
					value,
					A5(elm$core$Dict$RBNode_elm_builtin, elm$core$Dict$Black, lK, lV, lLeft, lRight),
					A5(elm$core$Dict$RBNode_elm_builtin, elm$core$Dict$Black, rK, rV, rLeft, rRight));
			} else {
				return A5(
					elm$core$Dict$RBNode_elm_builtin,
					color,
					rK,
					rV,
					A5(elm$core$Dict$RBNode_elm_builtin, elm$core$Dict$Red, key, value, left, rLeft),
					rRight);
			}
		} else {
			if ((((left.$ === 'RBNode_elm_builtin') && (left.a.$ === 'Red')) && (left.d.$ === 'RBNode_elm_builtin')) && (left.d.a.$ === 'Red')) {
				var _n5 = left.a;
				var lK = left.b;
				var lV = left.c;
				var _n6 = left.d;
				var _n7 = _n6.a;
				var llK = _n6.b;
				var llV = _n6.c;
				var llLeft = _n6.d;
				var llRight = _n6.e;
				var lRight = left.e;
				return A5(
					elm$core$Dict$RBNode_elm_builtin,
					elm$core$Dict$Red,
					lK,
					lV,
					A5(elm$core$Dict$RBNode_elm_builtin, elm$core$Dict$Black, llK, llV, llLeft, llRight),
					A5(elm$core$Dict$RBNode_elm_builtin, elm$core$Dict$Black, key, value, lRight, right));
			} else {
				return A5(elm$core$Dict$RBNode_elm_builtin, color, key, value, left, right);
			}
		}
	});
var elm$core$Dict$insertHelp = F3(
	function (key, value, dict) {
		if (dict.$ === 'RBEmpty_elm_builtin') {
			return A5(elm$core$Dict$RBNode_elm_builtin, elm$core$Dict$Red, key, value, elm$core$Dict$RBEmpty_elm_builtin, elm$core$Dict$RBEmpty_elm_builtin);
		} else {
			var nColor = dict.a;
			var nKey = dict.b;
			var nValue = dict.c;
			var nLeft = dict.d;
			var nRight = dict.e;
			var _n1 = A2(elm$core$Basics$compare, key, nKey);
			switch (_n1.$) {
				case 'LT':
					return A5(
						elm$core$Dict$balance,
						nColor,
						nKey,
						nValue,
						A3(elm$core$Dict$insertHelp, key, value, nLeft),
						nRight);
				case 'EQ':
					return A5(elm$core$Dict$RBNode_elm_builtin, nColor, nKey, value, nLeft, nRight);
				default:
					return A5(
						elm$core$Dict$balance,
						nColor,
						nKey,
						nValue,
						nLeft,
						A3(elm$core$Dict$insertHelp, key, value, nRight));
			}
		}
	});
var elm$core$Dict$insert = F3(
	function (key, value, dict) {
		var _n0 = A3(elm$core$Dict$insertHelp, key, value, dict);
		if ((_n0.$ === 'RBNode_elm_builtin') && (_n0.a.$ === 'Red')) {
			var _n1 = _n0.a;
			var k = _n0.b;
			var v = _n0.c;
			var l = _n0.d;
			var r = _n0.e;
			return A5(elm$core$Dict$RBNode_elm_builtin, elm$core$Dict$Black, k, v, l, r);
		} else {
			var x = _n0;
			return x;
		}
	});
var elm$core$Dict$map = F2(
	function (func, dict) {
		if (dict.$ === 'RBEmpty_elm_builtin') {
			return elm$core$Dict$RBEmpty_elm_builtin;
		} else {
			var color = dict.a;
			var key = dict.b;
			var value = dict.c;
			var left = dict.d;
			var right = dict.e;
			return A5(
				elm$core$Dict$RBNode_elm_builtin,
				color,
				key,
				A2(func, key, value),
				A2(elm$core$Dict$map, func, left),
				A2(elm$core$Dict$map, func, right));
		}
	});
var elm$core$List$partition = F2(
	function (pred, list) {
		var step = F2(
			function (x, _n0) {
				var trues = _n0.a;
				var falses = _n0.b;
				return pred(x) ? _Utils_Tuple2(
					A2(elm$core$List$cons, x, trues),
					falses) : _Utils_Tuple2(
					trues,
					A2(elm$core$List$cons, x, falses));
			});
		return A3(
			elm$core$List$foldr,
			step,
			_Utils_Tuple2(_List_Nil, _List_Nil),
			list);
	});
var elm$core$Result$withDefault = F2(
	function (def, result) {
		if (result.$ === 'Ok') {
			var a = result.a;
			return a;
		} else {
			return def;
		}
	});
var elm_explorations$benchmark$Benchmark$Samples$pointify = function (samples) {
	return A3(
		elm$core$Dict$foldr,
		F3(
			function (sampleSize, values, acc) {
				return _Utils_ap(
					A2(
						elm$core$List$map,
						function (b) {
							return _Utils_Tuple2(sampleSize, b);
						},
						values),
					acc);
			}),
		_List_Nil,
		samples);
};
var elm_explorations$benchmark$Benchmark$Samples$groups = function (_n0) {
	var samples = _n0.a;
	return A2(
		elm$core$Result$withDefault,
		_Utils_Tuple2(samples, elm$core$Dict$empty),
		A2(
			elm$core$Result$map,
			A2(
				elm$core$Dict$foldl,
				F3(
					function (key, _n1, _n2) {
						var good = _n1.a;
						var outliers = _n1.b;
						var accGood = _n2.a;
						var accOutliers = _n2.b;
						return _Utils_Tuple2(
							A3(elm$core$Dict$insert, key, good, accGood),
							A3(elm$core$Dict$insert, key, outliers, accOutliers));
					}),
				_Utils_Tuple2(elm$core$Dict$empty, elm$core$Dict$empty)),
			A2(
				elm$core$Result$map,
				function (line) {
					return A2(
						elm$core$Dict$map,
						F2(
							function (sampleSize, values) {
								var predicted = A2(BrianHicks$elm_trend$Trend$Linear$predictY, line, sampleSize);
								var upperBound = predicted * 1.1;
								var lowerBound = predicted / 1.1;
								return A2(
									elm$core$List$partition,
									function (v) {
										return (_Utils_cmp(lowerBound, v) < 0) && (_Utils_cmp(v, upperBound) < 0);
									},
									values);
							}),
						samples);
				},
				A2(
					elm$core$Result$map,
					BrianHicks$elm_trend$Trend$Linear$line,
					BrianHicks$elm_trend$Trend$Linear$robust(
						elm_explorations$benchmark$Benchmark$Samples$pointify(samples))))));
};
var elm_explorations$benchmark$Benchmark$Samples$points = function (samples) {
	return A2(
		elm$core$Tuple$mapSecond,
		elm_explorations$benchmark$Benchmark$Samples$pointify,
		A2(
			elm$core$Tuple$mapFirst,
			elm_explorations$benchmark$Benchmark$Samples$pointify,
			elm_explorations$benchmark$Benchmark$Samples$groups(samples)));
};
var elm_explorations$benchmark$Benchmark$Samples$trend = function (samples) {
	return BrianHicks$elm_trend$Trend$Linear$quick(
		elm_explorations$benchmark$Benchmark$Samples$points(samples).a);
};
var elm_explorations$benchmark$Benchmark$Status$AnalysisError = function (a) {
	return {$: 'AnalysisError', a: a};
};
var elm_explorations$benchmark$Benchmark$Status$Failure = function (a) {
	return {$: 'Failure', a: a};
};
var elm_explorations$benchmark$Benchmark$Status$Success = F2(
	function (a, b) {
		return {$: 'Success', a: a, b: b};
	});
var elm_explorations$benchmark$Benchmark$finalize = function (samples) {
	var _n0 = elm_explorations$benchmark$Benchmark$Samples$trend(samples);
	if (_n0.$ === 'Ok') {
		var trend = _n0.a;
		return A2(elm_explorations$benchmark$Benchmark$Status$Success, samples, trend);
	} else {
		var err = _n0.a;
		return elm_explorations$benchmark$Benchmark$Status$Failure(
			elm_explorations$benchmark$Benchmark$Status$AnalysisError(err));
	}
};
var elm_explorations$benchmark$Benchmark$LowLevel$defaultMinimum = 1;
var elm$core$Basics$composeR = F3(
	function (f, g, x) {
		return g(
			f(x));
	});
var elm$core$Basics$min = F2(
	function (x, y) {
		return (_Utils_cmp(x, y) < 0) ? x : y;
	});
var elm$core$List$minimum = function (list) {
	if (list.b) {
		var x = list.a;
		var xs = list.b;
		return elm$core$Maybe$Just(
			A3(elm$core$List$foldl, elm$core$Basics$min, x, xs));
	} else {
		return elm$core$Maybe$Nothing;
	}
};
var elm$core$List$repeatHelp = F3(
	function (result, n, value) {
		repeatHelp:
		while (true) {
			if (n <= 0) {
				return result;
			} else {
				var $temp$result = A2(elm$core$List$cons, value, result),
					$temp$n = n - 1,
					$temp$value = value;
				result = $temp$result;
				n = $temp$n;
				value = $temp$value;
				continue repeatHelp;
			}
		}
	});
var elm$core$List$repeat = F2(
	function (n, value) {
		return A3(elm$core$List$repeatHelp, _List_Nil, n, value);
	});
var elm$core$Maybe$withDefault = F2(
	function (_default, maybe) {
		if (maybe.$ === 'Just') {
			var value = maybe.a;
			return value;
		} else {
			return _default;
		}
	});
var elm_explorations$benchmark$Benchmark$LowLevel$sample = F2(
	function (n, operation_) {
		return A2(_Benchmark_sample, n, operation_);
	});
var elm$core$Basics$round = _Basics_round;
var elm_explorations$benchmark$Benchmark$LowLevel$standardizeSampleSize = function (sampleSize) {
	var helper = F2(
		function (rough, magnitude) {
			helper:
			while (true) {
				if (rough > 10) {
					var $temp$rough = elm$core$Basics$round(rough / 10),
						$temp$magnitude = magnitude * 10;
					rough = $temp$rough;
					magnitude = $temp$magnitude;
					continue helper;
				} else {
					return rough * magnitude;
				}
			}
		});
	return A2(helper, sampleSize, 1);
};
var elm_explorations$benchmark$Benchmark$LowLevel$findSampleSizeWithMinimum = F2(
	function (minimumRuntime, operation_) {
		var sampleSize = function (i) {
			return i * 10;
		};
		var resample = F2(
			function (iteration, total) {
				return (_Utils_cmp(total, minimumRuntime) < 0) ? A2(
					elm$core$Task$andThen,
					resample(iteration + 1),
					A2(
						elm$core$Task$map,
						A2(
							elm$core$Basics$composeR,
							elm$core$List$minimum,
							elm$core$Maybe$withDefault(0)),
						elm$core$Task$sequence(
							A2(
								elm$core$List$repeat,
								3,
								A2(
									elm_explorations$benchmark$Benchmark$LowLevel$sample,
									sampleSize(iteration),
									operation_))))) : elm$core$Task$succeed(
					sampleSize(iteration));
			});
		return A2(
			elm$core$Task$map,
			elm_explorations$benchmark$Benchmark$LowLevel$standardizeSampleSize,
			A2(resample, 1, 0));
	});
var elm_explorations$benchmark$Benchmark$LowLevel$findSampleSize = elm_explorations$benchmark$Benchmark$LowLevel$findSampleSizeWithMinimum(elm_explorations$benchmark$Benchmark$LowLevel$defaultMinimum);
var elm_explorations$benchmark$Benchmark$LowLevel$warmup = function (operation_) {
	var toCollect = 1000;
	var sampleSize = 10000;
	var helper = function (soFar) {
		return (_Utils_cmp(soFar, toCollect) > -1) ? elm$core$Task$succeed(_Utils_Tuple0) : A2(
			elm$core$Task$andThen,
			helper,
			A2(
				elm$core$Task$map,
				elm$core$Basics$add(soFar),
				A2(elm_explorations$benchmark$Benchmark$LowLevel$sample, sampleSize, operation_)));
	};
	return helper(0);
};
var elm_explorations$benchmark$Benchmark$Samples$Samples = function (a) {
	return {$: 'Samples', a: a};
};
var elm_explorations$benchmark$Benchmark$Samples$empty = elm_explorations$benchmark$Benchmark$Samples$Samples(elm$core$Dict$empty);
var elm$core$Dict$get = F2(
	function (targetKey, dict) {
		get:
		while (true) {
			if (dict.$ === 'RBEmpty_elm_builtin') {
				return elm$core$Maybe$Nothing;
			} else {
				var key = dict.b;
				var value = dict.c;
				var left = dict.d;
				var right = dict.e;
				var _n1 = A2(elm$core$Basics$compare, targetKey, key);
				switch (_n1.$) {
					case 'LT':
						var $temp$targetKey = targetKey,
							$temp$dict = left;
						targetKey = $temp$targetKey;
						dict = $temp$dict;
						continue get;
					case 'EQ':
						return elm$core$Maybe$Just(value);
					default:
						var $temp$targetKey = targetKey,
							$temp$dict = right;
						targetKey = $temp$targetKey;
						dict = $temp$dict;
						continue get;
				}
			}
		}
	});
var elm$core$Dict$getMin = function (dict) {
	getMin:
	while (true) {
		if ((dict.$ === 'RBNode_elm_builtin') && (dict.d.$ === 'RBNode_elm_builtin')) {
			var left = dict.d;
			var $temp$dict = left;
			dict = $temp$dict;
			continue getMin;
		} else {
			return dict;
		}
	}
};
var elm$core$Dict$moveRedLeft = function (dict) {
	if (((dict.$ === 'RBNode_elm_builtin') && (dict.d.$ === 'RBNode_elm_builtin')) && (dict.e.$ === 'RBNode_elm_builtin')) {
		if ((dict.e.d.$ === 'RBNode_elm_builtin') && (dict.e.d.a.$ === 'Red')) {
			var clr = dict.a;
			var k = dict.b;
			var v = dict.c;
			var _n1 = dict.d;
			var lClr = _n1.a;
			var lK = _n1.b;
			var lV = _n1.c;
			var lLeft = _n1.d;
			var lRight = _n1.e;
			var _n2 = dict.e;
			var rClr = _n2.a;
			var rK = _n2.b;
			var rV = _n2.c;
			var rLeft = _n2.d;
			var _n3 = rLeft.a;
			var rlK = rLeft.b;
			var rlV = rLeft.c;
			var rlL = rLeft.d;
			var rlR = rLeft.e;
			var rRight = _n2.e;
			return A5(
				elm$core$Dict$RBNode_elm_builtin,
				elm$core$Dict$Red,
				rlK,
				rlV,
				A5(
					elm$core$Dict$RBNode_elm_builtin,
					elm$core$Dict$Black,
					k,
					v,
					A5(elm$core$Dict$RBNode_elm_builtin, elm$core$Dict$Red, lK, lV, lLeft, lRight),
					rlL),
				A5(elm$core$Dict$RBNode_elm_builtin, elm$core$Dict$Black, rK, rV, rlR, rRight));
		} else {
			var clr = dict.a;
			var k = dict.b;
			var v = dict.c;
			var _n4 = dict.d;
			var lClr = _n4.a;
			var lK = _n4.b;
			var lV = _n4.c;
			var lLeft = _n4.d;
			var lRight = _n4.e;
			var _n5 = dict.e;
			var rClr = _n5.a;
			var rK = _n5.b;
			var rV = _n5.c;
			var rLeft = _n5.d;
			var rRight = _n5.e;
			if (clr.$ === 'Black') {
				return A5(
					elm$core$Dict$RBNode_elm_builtin,
					elm$core$Dict$Black,
					k,
					v,
					A5(elm$core$Dict$RBNode_elm_builtin, elm$core$Dict$Red, lK, lV, lLeft, lRight),
					A5(elm$core$Dict$RBNode_elm_builtin, elm$core$Dict$Red, rK, rV, rLeft, rRight));
			} else {
				return A5(
					elm$core$Dict$RBNode_elm_builtin,
					elm$core$Dict$Black,
					k,
					v,
					A5(elm$core$Dict$RBNode_elm_builtin, elm$core$Dict$Red, lK, lV, lLeft, lRight),
					A5(elm$core$Dict$RBNode_elm_builtin, elm$core$Dict$Red, rK, rV, rLeft, rRight));
			}
		}
	} else {
		return dict;
	}
};
var elm$core$Dict$moveRedRight = function (dict) {
	if (((dict.$ === 'RBNode_elm_builtin') && (dict.d.$ === 'RBNode_elm_builtin')) && (dict.e.$ === 'RBNode_elm_builtin')) {
		if ((dict.d.d.$ === 'RBNode_elm_builtin') && (dict.d.d.a.$ === 'Red')) {
			var clr = dict.a;
			var k = dict.b;
			var v = dict.c;
			var _n1 = dict.d;
			var lClr = _n1.a;
			var lK = _n1.b;
			var lV = _n1.c;
			var _n2 = _n1.d;
			var _n3 = _n2.a;
			var llK = _n2.b;
			var llV = _n2.c;
			var llLeft = _n2.d;
			var llRight = _n2.e;
			var lRight = _n1.e;
			var _n4 = dict.e;
			var rClr = _n4.a;
			var rK = _n4.b;
			var rV = _n4.c;
			var rLeft = _n4.d;
			var rRight = _n4.e;
			return A5(
				elm$core$Dict$RBNode_elm_builtin,
				elm$core$Dict$Red,
				lK,
				lV,
				A5(elm$core$Dict$RBNode_elm_builtin, elm$core$Dict$Black, llK, llV, llLeft, llRight),
				A5(
					elm$core$Dict$RBNode_elm_builtin,
					elm$core$Dict$Black,
					k,
					v,
					lRight,
					A5(elm$core$Dict$RBNode_elm_builtin, elm$core$Dict$Red, rK, rV, rLeft, rRight)));
		} else {
			var clr = dict.a;
			var k = dict.b;
			var v = dict.c;
			var _n5 = dict.d;
			var lClr = _n5.a;
			var lK = _n5.b;
			var lV = _n5.c;
			var lLeft = _n5.d;
			var lRight = _n5.e;
			var _n6 = dict.e;
			var rClr = _n6.a;
			var rK = _n6.b;
			var rV = _n6.c;
			var rLeft = _n6.d;
			var rRight = _n6.e;
			if (clr.$ === 'Black') {
				return A5(
					elm$core$Dict$RBNode_elm_builtin,
					elm$core$Dict$Black,
					k,
					v,
					A5(elm$core$Dict$RBNode_elm_builtin, elm$core$Dict$Red, lK, lV, lLeft, lRight),
					A5(elm$core$Dict$RBNode_elm_builtin, elm$core$Dict$Red, rK, rV, rLeft, rRight));
			} else {
				return A5(
					elm$core$Dict$RBNode_elm_builtin,
					elm$core$Dict$Black,
					k,
					v,
					A5(elm$core$Dict$RBNode_elm_builtin, elm$core$Dict$Red, lK, lV, lLeft, lRight),
					A5(elm$core$Dict$RBNode_elm_builtin, elm$core$Dict$Red, rK, rV, rLeft, rRight));
			}
		}
	} else {
		return dict;
	}
};
var elm$core$Dict$removeHelpPrepEQGT = F7(
	function (targetKey, dict, color, key, value, left, right) {
		if ((left.$ === 'RBNode_elm_builtin') && (left.a.$ === 'Red')) {
			var _n1 = left.a;
			var lK = left.b;
			var lV = left.c;
			var lLeft = left.d;
			var lRight = left.e;
			return A5(
				elm$core$Dict$RBNode_elm_builtin,
				color,
				lK,
				lV,
				lLeft,
				A5(elm$core$Dict$RBNode_elm_builtin, elm$core$Dict$Red, key, value, lRight, right));
		} else {
			_n2$2:
			while (true) {
				if ((right.$ === 'RBNode_elm_builtin') && (right.a.$ === 'Black')) {
					if (right.d.$ === 'RBNode_elm_builtin') {
						if (right.d.a.$ === 'Black') {
							var _n3 = right.a;
							var _n4 = right.d;
							var _n5 = _n4.a;
							return elm$core$Dict$moveRedRight(dict);
						} else {
							break _n2$2;
						}
					} else {
						var _n6 = right.a;
						var _n7 = right.d;
						return elm$core$Dict$moveRedRight(dict);
					}
				} else {
					break _n2$2;
				}
			}
			return dict;
		}
	});
var elm$core$Dict$removeMin = function (dict) {
	if ((dict.$ === 'RBNode_elm_builtin') && (dict.d.$ === 'RBNode_elm_builtin')) {
		var color = dict.a;
		var key = dict.b;
		var value = dict.c;
		var left = dict.d;
		var lColor = left.a;
		var lLeft = left.d;
		var right = dict.e;
		if (lColor.$ === 'Black') {
			if ((lLeft.$ === 'RBNode_elm_builtin') && (lLeft.a.$ === 'Red')) {
				var _n3 = lLeft.a;
				return A5(
					elm$core$Dict$RBNode_elm_builtin,
					color,
					key,
					value,
					elm$core$Dict$removeMin(left),
					right);
			} else {
				var _n4 = elm$core$Dict$moveRedLeft(dict);
				if (_n4.$ === 'RBNode_elm_builtin') {
					var nColor = _n4.a;
					var nKey = _n4.b;
					var nValue = _n4.c;
					var nLeft = _n4.d;
					var nRight = _n4.e;
					return A5(
						elm$core$Dict$balance,
						nColor,
						nKey,
						nValue,
						elm$core$Dict$removeMin(nLeft),
						nRight);
				} else {
					return elm$core$Dict$RBEmpty_elm_builtin;
				}
			}
		} else {
			return A5(
				elm$core$Dict$RBNode_elm_builtin,
				color,
				key,
				value,
				elm$core$Dict$removeMin(left),
				right);
		}
	} else {
		return elm$core$Dict$RBEmpty_elm_builtin;
	}
};
var elm$core$Dict$removeHelp = F2(
	function (targetKey, dict) {
		if (dict.$ === 'RBEmpty_elm_builtin') {
			return elm$core$Dict$RBEmpty_elm_builtin;
		} else {
			var color = dict.a;
			var key = dict.b;
			var value = dict.c;
			var left = dict.d;
			var right = dict.e;
			if (_Utils_cmp(targetKey, key) < 0) {
				if ((left.$ === 'RBNode_elm_builtin') && (left.a.$ === 'Black')) {
					var _n4 = left.a;
					var lLeft = left.d;
					if ((lLeft.$ === 'RBNode_elm_builtin') && (lLeft.a.$ === 'Red')) {
						var _n6 = lLeft.a;
						return A5(
							elm$core$Dict$RBNode_elm_builtin,
							color,
							key,
							value,
							A2(elm$core$Dict$removeHelp, targetKey, left),
							right);
					} else {
						var _n7 = elm$core$Dict$moveRedLeft(dict);
						if (_n7.$ === 'RBNode_elm_builtin') {
							var nColor = _n7.a;
							var nKey = _n7.b;
							var nValue = _n7.c;
							var nLeft = _n7.d;
							var nRight = _n7.e;
							return A5(
								elm$core$Dict$balance,
								nColor,
								nKey,
								nValue,
								A2(elm$core$Dict$removeHelp, targetKey, nLeft),
								nRight);
						} else {
							return elm$core$Dict$RBEmpty_elm_builtin;
						}
					}
				} else {
					return A5(
						elm$core$Dict$RBNode_elm_builtin,
						color,
						key,
						value,
						A2(elm$core$Dict$removeHelp, targetKey, left),
						right);
				}
			} else {
				return A2(
					elm$core$Dict$removeHelpEQGT,
					targetKey,
					A7(elm$core$Dict$removeHelpPrepEQGT, targetKey, dict, color, key, value, left, right));
			}
		}
	});
var elm$core$Dict$removeHelpEQGT = F2(
	function (targetKey, dict) {
		if (dict.$ === 'RBNode_elm_builtin') {
			var color = dict.a;
			var key = dict.b;
			var value = dict.c;
			var left = dict.d;
			var right = dict.e;
			if (_Utils_eq(targetKey, key)) {
				var _n1 = elm$core$Dict$getMin(right);
				if (_n1.$ === 'RBNode_elm_builtin') {
					var minKey = _n1.b;
					var minValue = _n1.c;
					return A5(
						elm$core$Dict$balance,
						color,
						minKey,
						minValue,
						left,
						elm$core$Dict$removeMin(right));
				} else {
					return elm$core$Dict$RBEmpty_elm_builtin;
				}
			} else {
				return A5(
					elm$core$Dict$balance,
					color,
					key,
					value,
					left,
					A2(elm$core$Dict$removeHelp, targetKey, right));
			}
		} else {
			return elm$core$Dict$RBEmpty_elm_builtin;
		}
	});
var elm$core$Dict$remove = F2(
	function (key, dict) {
		var _n0 = A2(elm$core$Dict$removeHelp, key, dict);
		if ((_n0.$ === 'RBNode_elm_builtin') && (_n0.a.$ === 'Red')) {
			var _n1 = _n0.a;
			var k = _n0.b;
			var v = _n0.c;
			var l = _n0.d;
			var r = _n0.e;
			return A5(elm$core$Dict$RBNode_elm_builtin, elm$core$Dict$Black, k, v, l, r);
		} else {
			var x = _n0;
			return x;
		}
	});
var elm$core$Dict$update = F3(
	function (targetKey, alter, dictionary) {
		var _n0 = alter(
			A2(elm$core$Dict$get, targetKey, dictionary));
		if (_n0.$ === 'Just') {
			var value = _n0.a;
			return A3(elm$core$Dict$insert, targetKey, value, dictionary);
		} else {
			return A2(elm$core$Dict$remove, targetKey, dictionary);
		}
	});
var elm_explorations$benchmark$Benchmark$Samples$record = F3(
	function (sampleSize, sample, _n0) {
		var samplesDict = _n0.a;
		return elm_explorations$benchmark$Benchmark$Samples$Samples(
			A3(
				elm$core$Dict$update,
				sampleSize,
				function (value) {
					if (value.$ === 'Nothing') {
						return elm$core$Maybe$Just(
							_List_fromArray(
								[sample]));
					} else {
						var samples_ = value.a;
						return elm$core$Maybe$Just(
							A2(elm$core$List$cons, sample, samples_));
					}
				},
				samplesDict));
	});
var elm_explorations$benchmark$Benchmark$Status$MeasurementError = function (a) {
	return {$: 'MeasurementError', a: a};
};
var elm_explorations$benchmark$Benchmark$Status$Pending = F2(
	function (a, b) {
		return {$: 'Pending', a: a, b: b};
	});
var elm_explorations$benchmark$Benchmark$Status$Unsized = {$: 'Unsized'};
var elm_explorations$benchmark$Benchmark$Status$bucketSpacingRatio = 2;
var elm_explorations$benchmark$Benchmark$stepLowLevel = F2(
	function (operation, status) {
		switch (status.$) {
			case 'Cold':
				return A2(
					elm$core$Task$onError,
					A2(
						elm$core$Basics$composeL,
						A2(elm$core$Basics$composeL, elm$core$Task$succeed, elm_explorations$benchmark$Benchmark$Status$Failure),
						elm_explorations$benchmark$Benchmark$Status$MeasurementError),
					A2(
						elm$core$Task$map,
						function (_n1) {
							return elm_explorations$benchmark$Benchmark$Status$Unsized;
						},
						elm_explorations$benchmark$Benchmark$LowLevel$warmup(operation)));
			case 'Unsized':
				return A2(
					elm$core$Task$onError,
					A2(
						elm$core$Basics$composeL,
						A2(elm$core$Basics$composeL, elm$core$Task$succeed, elm_explorations$benchmark$Benchmark$Status$Failure),
						elm_explorations$benchmark$Benchmark$Status$MeasurementError),
					A2(
						elm$core$Task$map,
						function (sampleSize) {
							return A2(elm_explorations$benchmark$Benchmark$Status$Pending, sampleSize, elm_explorations$benchmark$Benchmark$Samples$empty);
						},
						elm_explorations$benchmark$Benchmark$LowLevel$findSampleSize(operation)));
			case 'Pending':
				var baseSampleSize = status.a;
				var samples = status.b;
				var sampleSize = baseSampleSize * ((elm_explorations$benchmark$Benchmark$Status$bucketSpacingRatio * A2(
					elm$core$Basics$modBy,
					elm_explorations$benchmark$Benchmark$Status$numBuckets,
					elm_explorations$benchmark$Benchmark$Samples$count(samples))) + 1);
				return A2(
					elm$core$Task$onError,
					A2(
						elm$core$Basics$composeL,
						A2(elm$core$Basics$composeL, elm$core$Task$succeed, elm_explorations$benchmark$Benchmark$Status$Failure),
						elm_explorations$benchmark$Benchmark$Status$MeasurementError),
					A2(
						elm$core$Task$map,
						function (newSample) {
							var newSamples = A3(elm_explorations$benchmark$Benchmark$Samples$record, sampleSize, newSample, samples);
							return (_Utils_cmp(
								elm_explorations$benchmark$Benchmark$Samples$count(newSamples),
								elm_explorations$benchmark$Benchmark$Status$numBuckets * elm_explorations$benchmark$Benchmark$Status$samplesPerBucket) > -1) ? elm_explorations$benchmark$Benchmark$finalize(newSamples) : A2(elm_explorations$benchmark$Benchmark$Status$Pending, baseSampleSize, newSamples);
						},
						A2(elm_explorations$benchmark$Benchmark$LowLevel$sample, sampleSize, operation)));
			default:
				return elm$core$Task$succeed(status);
		}
	});
var elm_explorations$benchmark$Benchmark$Benchmark$Single = F3(
	function (a, b, c) {
		return {$: 'Single', a: a, b: b, c: c};
	});
var elm_explorations$benchmark$Benchmark$step = function (benchmark_) {
	switch (benchmark_.$) {
		case 'Single':
			var name = benchmark_.a;
			var inner = benchmark_.b;
			var status = benchmark_.c;
			return A2(
				elm$core$Task$map,
				A2(elm_explorations$benchmark$Benchmark$Benchmark$Single, name, inner),
				A2(elm_explorations$benchmark$Benchmark$stepLowLevel, inner, status));
		case 'Series':
			var name = benchmark_.a;
			var benchmarks = benchmark_.b;
			return A2(
				elm$core$Task$map,
				elm_explorations$benchmark$Benchmark$Benchmark$Series(name),
				elm$core$Task$sequence(
					A2(
						elm$core$List$map,
						function (_n1) {
							var name_ = _n1.a;
							var inner = _n1.b;
							var status = _n1.c;
							return A2(
								elm$core$Task$map,
								function (status_) {
									return _Utils_Tuple3(name_, inner, status_);
								},
								A2(elm_explorations$benchmark$Benchmark$stepLowLevel, inner, status));
						},
						benchmarks)));
		default:
			var name = benchmark_.a;
			var benchmarks = benchmark_.b;
			return A2(
				elm$core$Task$map,
				elm_explorations$benchmark$Benchmark$Benchmark$Group(name),
				elm$core$Task$sequence(
					A2(elm$core$List$map, elm_explorations$benchmark$Benchmark$step, benchmarks)));
	}
};
var elm_explorations$benchmark$Benchmark$Runner$App$Update = function (a) {
	return {$: 'Update', a: a};
};
var elm$core$Process$sleep = _Process_sleep;
var elm_explorations$benchmark$Benchmark$Runner$App$breakForRender = function (task) {
	return A2(
		elm$core$Task$andThen,
		function (_n0) {
			return task;
		},
		elm$core$Process$sleep(0));
};
var elm_explorations$benchmark$Benchmark$Runner$App$next = function (benchmark) {
	return elm_explorations$benchmark$Benchmark$done(benchmark) ? elm$core$Platform$Cmd$none : A2(
		elm$core$Task$perform,
		elm_explorations$benchmark$Benchmark$Runner$App$Update,
		elm_explorations$benchmark$Benchmark$Runner$App$breakForRender(
			elm_explorations$benchmark$Benchmark$step(benchmark)));
};
var elm_explorations$benchmark$Benchmark$Runner$App$init = F2(
	function (benchmark, _n0) {
		return _Utils_Tuple2(
			benchmark,
			elm_explorations$benchmark$Benchmark$Runner$App$next(benchmark));
	});
var elm_explorations$benchmark$Benchmark$Runner$App$update = F2(
	function (msg, model) {
		var benchmark = msg.a;
		return _Utils_Tuple2(
			benchmark,
			elm_explorations$benchmark$Benchmark$Runner$App$next(benchmark));
	});
var elm_explorations$benchmark$Benchmark$Reporting$Group = F2(
	function (a, b) {
		return {$: 'Group', a: a, b: b};
	});
var elm_explorations$benchmark$Benchmark$Reporting$Series = F2(
	function (a, b) {
		return {$: 'Series', a: a, b: b};
	});
var elm_explorations$benchmark$Benchmark$Reporting$Single = F2(
	function (a, b) {
		return {$: 'Single', a: a, b: b};
	});
var elm_explorations$benchmark$Benchmark$Reporting$fromBenchmark = function (internal) {
	switch (internal.$) {
		case 'Single':
			var name = internal.a;
			var status = internal.c;
			return A2(elm_explorations$benchmark$Benchmark$Reporting$Single, name, status);
		case 'Series':
			var name = internal.a;
			var benchmarks = internal.b;
			return A2(
				elm_explorations$benchmark$Benchmark$Reporting$Series,
				name,
				A2(
					elm$core$List$map,
					function (_n1) {
						var childName = _n1.a;
						var status = _n1.c;
						return _Utils_Tuple2(childName, status);
					},
					benchmarks));
		default:
			var name = internal.a;
			var benchmarks = internal.b;
			return A2(
				elm_explorations$benchmark$Benchmark$Reporting$Group,
				name,
				A2(elm$core$List$map, elm_explorations$benchmark$Benchmark$Reporting$fromBenchmark, benchmarks));
	}
};
var elm_explorations$benchmark$Benchmark$Runner$App$InProgressClass = function (a) {
	return {$: 'InProgressClass', a: a};
};
var elm_explorations$benchmark$Benchmark$Runner$App$Page = {$: 'Page'};
var elm_explorations$benchmark$Benchmark$Runner$App$ReportClass = function (a) {
	return {$: 'ReportClass', a: a};
};
var elm_explorations$benchmark$Benchmark$Runner$App$ReportVariation = function (a) {
	return {$: 'ReportVariation', a: a};
};
var elm_explorations$benchmark$Benchmark$Runner$App$Wrapper = {$: 'Wrapper'};
var mdgriffith$style_elements$Style$Internal$Model$RGBA = F4(
	function (a, b, c, d) {
		return {$: 'RGBA', a: a, b: b, c: c, d: d};
	});
var mdgriffith$style_elements$Style$rgb = F3(
	function (r, g, b) {
		return A4(mdgriffith$style_elements$Style$Internal$Model$RGBA, r, g, b, 1);
	});
var mdgriffith$style_elements$Style$rgba = mdgriffith$style_elements$Style$Internal$Model$RGBA;
var mdgriffith$style_elements$Style$Internal$Model$Exact = F2(
	function (a, b) {
		return {$: 'Exact', a: a, b: b};
	});
var elm$core$String$fromFloat = _String_fromNumber;
var mdgriffith$style_elements$Style$Internal$Render$Value$color = function (_n0) {
	var red = _n0.a;
	var green = _n0.b;
	var blue = _n0.c;
	var alpha = _n0.d;
	return ('rgba(' + elm$core$String$fromInt(
		elm$core$Basics$round(red * 255))) + ((',' + elm$core$String$fromInt(
		elm$core$Basics$round(green * 255))) + ((',' + elm$core$String$fromInt(
		elm$core$Basics$round(blue * 255))) + (',' + (elm$core$String$fromFloat(alpha) + ')'))));
};
var mdgriffith$style_elements$Style$Color$background = function (clr) {
	return A2(
		mdgriffith$style_elements$Style$Internal$Model$Exact,
		'background-color',
		mdgriffith$style_elements$Style$Internal$Render$Value$color(clr));
};
var mdgriffith$style_elements$Style$Internal$Model$Font = F2(
	function (a, b) {
		return {$: 'Font', a: a, b: b};
	});
var mdgriffith$style_elements$Style$Font$size = function (i) {
	return A2(
		mdgriffith$style_elements$Style$Internal$Model$Font,
		'font-size',
		elm$core$String$fromFloat(i) + 'px');
};
var mdgriffith$style_elements$Style$Internal$Model$Shadows = function (a) {
	return {$: 'Shadows', a: a};
};
var mdgriffith$style_elements$Style$Internal$Model$ShadowModel = function (a) {
	return {$: 'ShadowModel', a: a};
};
var mdgriffith$style_elements$Style$Shadow$boxHelper = function (_n0) {
	var offset = _n0.offset;
	var size = _n0.size;
	var blur = _n0.blur;
	var color = _n0.color;
	return mdgriffith$style_elements$Style$Internal$Model$ShadowModel(
		{blur: blur, color: color, kind: 'box', offset: offset, size: size});
};
var mdgriffith$style_elements$Style$Shadow$box = function (shadow) {
	return mdgriffith$style_elements$Style$Internal$Model$Shadows(
		_List_fromArray(
			[
				mdgriffith$style_elements$Style$Shadow$boxHelper(shadow)
			]));
};
var elm_explorations$benchmark$Benchmark$Runner$Box$style = _List_fromArray(
	[
		mdgriffith$style_elements$Style$Color$background(
		A3(mdgriffith$style_elements$Style$rgb, 248 / 255, 248 / 255, 248 / 255)),
		mdgriffith$style_elements$Style$Shadow$box(
		{
			blur: 2,
			color: A4(mdgriffith$style_elements$Style$rgba, 15 / 255, 30 / 255, 45 / 255, 0.1),
			offset: _Utils_Tuple2(0, 1),
			size: 0
		}),
		mdgriffith$style_elements$Style$Font$size(24)
	]);
var elm_explorations$benchmark$Benchmark$Runner$InProgress$Box = {$: 'Box'};
var elm_explorations$benchmark$Benchmark$Runner$InProgress$Progress = {$: 'Progress'};
var elm_explorations$benchmark$Benchmark$Runner$InProgress$Status = {$: 'Status'};
var elm_explorations$benchmark$Benchmark$Runner$InProgress$TextClass = function (a) {
	return {$: 'TextClass', a: a};
};
var elm_explorations$benchmark$Benchmark$Runner$InProgress$Unstyled = {$: 'Unstyled'};
var elm_explorations$benchmark$Benchmark$Runner$Text$Hero = {$: 'Hero'};
var elm_explorations$benchmark$Benchmark$Runner$Text$Path = {$: 'Path'};
var mdgriffith$style_elements$Style$prop = F2(
	function (name, val) {
		return A2(mdgriffith$style_elements$Style$Internal$Model$Exact, name, val);
	});
var mdgriffith$style_elements$Style$Internal$Batchable$One = function (a) {
	return {$: 'One', a: a};
};
var mdgriffith$style_elements$Style$Internal$Batchable$one = mdgriffith$style_elements$Style$Internal$Batchable$One;
var mdgriffith$style_elements$Style$Internal$Model$Style = F2(
	function (a, b) {
		return {$: 'Style', a: a, b: b};
	});
var mdgriffith$style_elements$Style$style = F2(
	function (cls, props) {
		return mdgriffith$style_elements$Style$Internal$Batchable$one(
			A2(
				mdgriffith$style_elements$Style$Internal$Model$Style,
				cls,
				A2(
					elm$core$List$cons,
					A2(mdgriffith$style_elements$Style$prop, 'border-style', 'solid'),
					props)));
	});
var mdgriffith$style_elements$Style$Font$alignLeft = A2(mdgriffith$style_elements$Style$Internal$Model$Font, 'text-align', 'left');
var mdgriffith$style_elements$Style$Font$center = A2(mdgriffith$style_elements$Style$Internal$Model$Font, 'text-align', 'center');
var elm_explorations$benchmark$Benchmark$Runner$Text$styles = _List_fromArray(
	[
		A2(
		mdgriffith$style_elements$Style$style,
		elm_explorations$benchmark$Benchmark$Runner$Text$Hero,
		_List_fromArray(
			[
				mdgriffith$style_elements$Style$Font$center,
				mdgriffith$style_elements$Style$Font$size(48)
			])),
		A2(
		mdgriffith$style_elements$Style$style,
		elm_explorations$benchmark$Benchmark$Runner$Text$Path,
		_List_fromArray(
			[
				mdgriffith$style_elements$Style$Font$alignLeft,
				mdgriffith$style_elements$Style$Font$size(18)
			]))
	]);
var mdgriffith$style_elements$Style$Internal$Model$TextColor = function (a) {
	return {$: 'TextColor', a: a};
};
var mdgriffith$style_elements$Style$Color$text = mdgriffith$style_elements$Style$Internal$Model$TextColor;
var mdgriffith$style_elements$Style$Internal$Batchable$Batch = function (a) {
	return {$: 'Batch', a: a};
};
var mdgriffith$style_elements$Style$Internal$Batchable$Many = function (a) {
	return {$: 'Many', a: a};
};
var mdgriffith$style_elements$Style$Internal$Batchable$map = F2(
	function (fn, batchable) {
		switch (batchable.$) {
			case 'One':
				var internal = batchable.a;
				return mdgriffith$style_elements$Style$Internal$Batchable$One(
					fn(internal));
			case 'Many':
				var elems = batchable.a;
				return mdgriffith$style_elements$Style$Internal$Batchable$Many(
					A2(elm$core$List$map, fn, elems));
			default:
				var embedded = batchable.a;
				return mdgriffith$style_elements$Style$Internal$Batchable$Batch(
					A2(
						elm$core$List$map,
						mdgriffith$style_elements$Style$Internal$Batchable$map(fn),
						embedded));
		}
	});
var mdgriffith$style_elements$Style$Internal$Model$Import = function (a) {
	return {$: 'Import', a: a};
};
var mdgriffith$style_elements$Style$Internal$Model$RawStyle = F2(
	function (a, b) {
		return {$: 'RawStyle', a: a, b: b};
	});
var mdgriffith$style_elements$Style$Internal$Model$Reset = function (a) {
	return {$: 'Reset', a: a};
};
var mdgriffith$style_elements$Style$Internal$Model$Background = function (a) {
	return {$: 'Background', a: a};
};
var mdgriffith$style_elements$Style$Internal$Model$Child = F2(
	function (a, b) {
		return {$: 'Child', a: a, b: b};
	});
var mdgriffith$style_elements$Style$Internal$Model$Filters = function (a) {
	return {$: 'Filters', a: a};
};
var mdgriffith$style_elements$Style$Internal$Model$FontFamily = function (a) {
	return {$: 'FontFamily', a: a};
};
var mdgriffith$style_elements$Style$Internal$Model$Layout = function (a) {
	return {$: 'Layout', a: a};
};
var mdgriffith$style_elements$Style$Internal$Model$MediaQuery = F2(
	function (a, b) {
		return {$: 'MediaQuery', a: a, b: b};
	});
var mdgriffith$style_elements$Style$Internal$Model$Position = function (a) {
	return {$: 'Position', a: a};
};
var mdgriffith$style_elements$Style$Internal$Model$PseudoElement = F2(
	function (a, b) {
		return {$: 'PseudoElement', a: a, b: b};
	});
var mdgriffith$style_elements$Style$Internal$Model$SelectionColor = function (a) {
	return {$: 'SelectionColor', a: a};
};
var mdgriffith$style_elements$Style$Internal$Model$Transform = function (a) {
	return {$: 'Transform', a: a};
};
var mdgriffith$style_elements$Style$Internal$Model$Transitions = function (a) {
	return {$: 'Transitions', a: a};
};
var mdgriffith$style_elements$Style$Internal$Model$Variation = F2(
	function (a, b) {
		return {$: 'Variation', a: a, b: b};
	});
var mdgriffith$style_elements$Style$Internal$Model$Visibility = function (a) {
	return {$: 'Visibility', a: a};
};
var mdgriffith$style_elements$Style$Internal$Model$mapPropClass = F2(
	function (fn, prop) {
		switch (prop.$) {
			case 'Child':
				var _class = prop.a;
				var props = prop.b;
				return A2(
					mdgriffith$style_elements$Style$Internal$Model$Child,
					fn(_class),
					A2(
						elm$core$List$map,
						mdgriffith$style_elements$Style$Internal$Model$mapPropClass(fn),
						props));
			case 'Variation':
				var _var = prop.a;
				var props = prop.b;
				return A2(
					mdgriffith$style_elements$Style$Internal$Model$Variation,
					_var,
					A2(
						elm$core$List$map,
						A2(mdgriffith$style_elements$Style$Internal$Model$mapPropClassAndVar, fn, elm$core$Basics$identity),
						props));
			case 'Exact':
				var name = prop.a;
				var val = prop.b;
				return A2(mdgriffith$style_elements$Style$Internal$Model$Exact, name, val);
			case 'Position':
				var props = prop.a;
				return mdgriffith$style_elements$Style$Internal$Model$Position(props);
			case 'Font':
				var name = prop.a;
				var val = prop.b;
				return A2(mdgriffith$style_elements$Style$Internal$Model$Font, name, val);
			case 'Layout':
				var props = prop.a;
				return mdgriffith$style_elements$Style$Internal$Model$Layout(props);
			case 'Background':
				var props = prop.a;
				return mdgriffith$style_elements$Style$Internal$Model$Background(props);
			case 'MediaQuery':
				var name = prop.a;
				var props = prop.b;
				return A2(
					mdgriffith$style_elements$Style$Internal$Model$MediaQuery,
					name,
					A2(
						elm$core$List$map,
						mdgriffith$style_elements$Style$Internal$Model$mapPropClass(fn),
						props));
			case 'PseudoElement':
				var name = prop.a;
				var props = prop.b;
				return A2(
					mdgriffith$style_elements$Style$Internal$Model$PseudoElement,
					name,
					A2(
						elm$core$List$map,
						mdgriffith$style_elements$Style$Internal$Model$mapPropClass(fn),
						props));
			case 'Shadows':
				var shadows = prop.a;
				return mdgriffith$style_elements$Style$Internal$Model$Shadows(shadows);
			case 'Transform':
				var transforms = prop.a;
				return mdgriffith$style_elements$Style$Internal$Model$Transform(transforms);
			case 'Filters':
				var filters = prop.a;
				return mdgriffith$style_elements$Style$Internal$Model$Filters(filters);
			case 'Visibility':
				var v = prop.a;
				return mdgriffith$style_elements$Style$Internal$Model$Visibility(v);
			case 'TextColor':
				var color = prop.a;
				return mdgriffith$style_elements$Style$Internal$Model$TextColor(color);
			case 'Transitions':
				var t = prop.a;
				return mdgriffith$style_elements$Style$Internal$Model$Transitions(t);
			case 'SelectionColor':
				var clr = prop.a;
				return mdgriffith$style_elements$Style$Internal$Model$SelectionColor(clr);
			default:
				var fam = prop.a;
				return mdgriffith$style_elements$Style$Internal$Model$FontFamily(fam);
		}
	});
var mdgriffith$style_elements$Style$Internal$Model$mapPropClassAndVar = F3(
	function (fn, fnVar, prop) {
		switch (prop.$) {
			case 'Child':
				var _class = prop.a;
				var props = prop.b;
				return A2(
					mdgriffith$style_elements$Style$Internal$Model$Child,
					fn(_class),
					A2(
						elm$core$List$map,
						A2(mdgriffith$style_elements$Style$Internal$Model$mapPropClassAndVar, fn, fnVar),
						props));
			case 'Variation':
				var _var = prop.a;
				var props = prop.b;
				return A2(
					mdgriffith$style_elements$Style$Internal$Model$Variation,
					fnVar(_var),
					A2(
						elm$core$List$map,
						mdgriffith$style_elements$Style$Internal$Model$mapPropClass(fn),
						props));
			case 'Exact':
				var name = prop.a;
				var val = prop.b;
				return A2(mdgriffith$style_elements$Style$Internal$Model$Exact, name, val);
			case 'Position':
				var props = prop.a;
				return mdgriffith$style_elements$Style$Internal$Model$Position(props);
			case 'Font':
				var name = prop.a;
				var val = prop.b;
				return A2(mdgriffith$style_elements$Style$Internal$Model$Font, name, val);
			case 'Layout':
				var props = prop.a;
				return mdgriffith$style_elements$Style$Internal$Model$Layout(props);
			case 'Background':
				var props = prop.a;
				return mdgriffith$style_elements$Style$Internal$Model$Background(props);
			case 'MediaQuery':
				var name = prop.a;
				var props = prop.b;
				return A2(
					mdgriffith$style_elements$Style$Internal$Model$MediaQuery,
					name,
					A2(
						elm$core$List$map,
						A2(mdgriffith$style_elements$Style$Internal$Model$mapPropClassAndVar, fn, fnVar),
						props));
			case 'PseudoElement':
				var name = prop.a;
				var props = prop.b;
				return A2(
					mdgriffith$style_elements$Style$Internal$Model$PseudoElement,
					name,
					A2(
						elm$core$List$map,
						A2(mdgriffith$style_elements$Style$Internal$Model$mapPropClassAndVar, fn, fnVar),
						props));
			case 'Shadows':
				var shadows = prop.a;
				return mdgriffith$style_elements$Style$Internal$Model$Shadows(shadows);
			case 'Transform':
				var transforms = prop.a;
				return mdgriffith$style_elements$Style$Internal$Model$Transform(transforms);
			case 'Filters':
				var filters = prop.a;
				return mdgriffith$style_elements$Style$Internal$Model$Filters(filters);
			case 'Visibility':
				var v = prop.a;
				return mdgriffith$style_elements$Style$Internal$Model$Visibility(v);
			case 'TextColor':
				var color = prop.a;
				return mdgriffith$style_elements$Style$Internal$Model$TextColor(color);
			case 'Transitions':
				var t = prop.a;
				return mdgriffith$style_elements$Style$Internal$Model$Transitions(t);
			case 'SelectionColor':
				var clr = prop.a;
				return mdgriffith$style_elements$Style$Internal$Model$SelectionColor(clr);
			default:
				var fam = prop.a;
				return mdgriffith$style_elements$Style$Internal$Model$FontFamily(fam);
		}
	});
var mdgriffith$style_elements$Style$Internal$Model$mapClassAndVar = F3(
	function (fn, fnVariation, style) {
		switch (style.$) {
			case 'Style':
				var _class = style.a;
				var props = style.b;
				return A2(
					mdgriffith$style_elements$Style$Internal$Model$Style,
					fn(_class),
					A2(
						elm$core$List$map,
						A2(mdgriffith$style_elements$Style$Internal$Model$mapPropClassAndVar, fn, fnVariation),
						props));
			case 'Import':
				var str = style.a;
				return mdgriffith$style_elements$Style$Internal$Model$Import(str);
			case 'RawStyle':
				var str = style.a;
				var props = style.b;
				return A2(mdgriffith$style_elements$Style$Internal$Model$RawStyle, str, props);
			default:
				var r = style.a;
				return mdgriffith$style_elements$Style$Internal$Model$Reset(r);
		}
	});
var mdgriffith$style_elements$Style$Sheet$ChildSheet = function (a) {
	return {$: 'ChildSheet', a: a};
};
var mdgriffith$style_elements$Style$Sheet$map = F3(
	function (toParent, toParentVariation, styles) {
		return mdgriffith$style_elements$Style$Sheet$ChildSheet(
			A2(
				elm$core$List$map,
				mdgriffith$style_elements$Style$Internal$Batchable$map(
					A2(mdgriffith$style_elements$Style$Internal$Model$mapClassAndVar, toParent, toParentVariation)),
				styles));
	});
var mdgriffith$style_elements$Style$Internal$Batchable$many = mdgriffith$style_elements$Style$Internal$Batchable$Many;
var elm$core$List$append = F2(
	function (xs, ys) {
		if (!ys.b) {
			return xs;
		} else {
			return A3(elm$core$List$foldr, elm$core$List$cons, ys, xs);
		}
	});
var elm$core$List$concat = function (lists) {
	return A3(elm$core$List$foldr, elm$core$List$append, _List_Nil, lists);
};
var elm$core$List$concatMap = F2(
	function (f, list) {
		return elm$core$List$concat(
			A2(elm$core$List$map, f, list));
	});
var mdgriffith$style_elements$Style$Internal$Batchable$toList = function (batchables) {
	var flatten = function (batchToFlatten) {
		switch (batchToFlatten.$) {
			case 'One':
				var thing = batchToFlatten.a;
				return _List_fromArray(
					[thing]);
			case 'Many':
				var things = batchToFlatten.a;
				return things;
			default:
				var embedded = batchToFlatten.a;
				return mdgriffith$style_elements$Style$Internal$Batchable$toList(embedded);
		}
	};
	return A2(elm$core$List$concatMap, flatten, batchables);
};
var mdgriffith$style_elements$Style$Sheet$merge = function (_n0) {
	var styles = _n0.a;
	return mdgriffith$style_elements$Style$Internal$Batchable$many(
		mdgriffith$style_elements$Style$Internal$Batchable$toList(styles));
};
var elm_explorations$benchmark$Benchmark$Runner$InProgress$styles = _List_fromArray(
	[
		A2(mdgriffith$style_elements$Style$style, elm_explorations$benchmark$Benchmark$Runner$InProgress$Unstyled, _List_Nil),
		A2(mdgriffith$style_elements$Style$style, elm_explorations$benchmark$Benchmark$Runner$InProgress$Box, elm_explorations$benchmark$Benchmark$Runner$Box$style),
		A2(
		mdgriffith$style_elements$Style$style,
		elm_explorations$benchmark$Benchmark$Runner$InProgress$Progress,
		_List_fromArray(
			[
				mdgriffith$style_elements$Style$Color$text(
				A3(mdgriffith$style_elements$Style$rgb, 248 / 255, 248 / 255, 248 / 255)),
				mdgriffith$style_elements$Style$Color$background(
				A3(mdgriffith$style_elements$Style$rgb, 87 / 255, 171 / 255, 226 / 255))
			])),
		A2(
		mdgriffith$style_elements$Style$style,
		elm_explorations$benchmark$Benchmark$Runner$InProgress$Status,
		_List_fromArray(
			[
				mdgriffith$style_elements$Style$Font$size(14)
			])),
		mdgriffith$style_elements$Style$Sheet$merge(
		A3(mdgriffith$style_elements$Style$Sheet$map, elm_explorations$benchmark$Benchmark$Runner$InProgress$TextClass, elm$core$Basics$identity, elm_explorations$benchmark$Benchmark$Runner$Text$styles))
	]);
var elm_explorations$benchmark$Benchmark$Runner$Report$Box = {$: 'Box'};
var elm_explorations$benchmark$Benchmark$Runner$Report$Cell = {$: 'Cell'};
var elm_explorations$benchmark$Benchmark$Runner$Report$Header = {$: 'Header'};
var elm_explorations$benchmark$Benchmark$Runner$Report$Numeric = {$: 'Numeric'};
var elm_explorations$benchmark$Benchmark$Runner$Report$Table = {$: 'Table'};
var elm_explorations$benchmark$Benchmark$Runner$Report$Text = {$: 'Text'};
var elm_explorations$benchmark$Benchmark$Runner$Report$TextClass = function (a) {
	return {$: 'TextClass', a: a};
};
var elm_explorations$benchmark$Benchmark$Runner$Report$Unstyled = {$: 'Unstyled'};
var mdgriffith$style_elements$Style$variation = F2(
	function (v, variationProps) {
		return A2(mdgriffith$style_elements$Style$Internal$Model$Variation, v, variationProps);
	});
var mdgriffith$style_elements$Style$Font$alignRight = A2(mdgriffith$style_elements$Style$Internal$Model$Font, 'text-align', 'right');
var mdgriffith$style_elements$Style$Font$bold = A2(mdgriffith$style_elements$Style$Internal$Model$Font, 'font-weight', '700');
var elm_explorations$benchmark$Benchmark$Runner$Report$styles = _List_fromArray(
	[
		A2(mdgriffith$style_elements$Style$style, elm_explorations$benchmark$Benchmark$Runner$Report$Unstyled, _List_Nil),
		A2(mdgriffith$style_elements$Style$style, elm_explorations$benchmark$Benchmark$Runner$Report$Box, elm_explorations$benchmark$Benchmark$Runner$Box$style),
		A2(
		mdgriffith$style_elements$Style$style,
		elm_explorations$benchmark$Benchmark$Runner$Report$Table,
		_List_fromArray(
			[
				A2(mdgriffith$style_elements$Style$prop, 'font-feature-settings', '\'tnum\'')
			])),
		A2(
		mdgriffith$style_elements$Style$style,
		elm_explorations$benchmark$Benchmark$Runner$Report$Header,
		_List_fromArray(
			[
				mdgriffith$style_elements$Style$Font$bold,
				mdgriffith$style_elements$Style$Font$size(12),
				A2(
				mdgriffith$style_elements$Style$variation,
				elm_explorations$benchmark$Benchmark$Runner$Report$Numeric,
				_List_fromArray(
					[mdgriffith$style_elements$Style$Font$alignRight])),
				A2(
				mdgriffith$style_elements$Style$variation,
				elm_explorations$benchmark$Benchmark$Runner$Report$Text,
				_List_fromArray(
					[mdgriffith$style_elements$Style$Font$alignLeft]))
			])),
		A2(
		mdgriffith$style_elements$Style$style,
		elm_explorations$benchmark$Benchmark$Runner$Report$Cell,
		_List_fromArray(
			[
				mdgriffith$style_elements$Style$Font$size(18),
				A2(
				mdgriffith$style_elements$Style$variation,
				elm_explorations$benchmark$Benchmark$Runner$Report$Numeric,
				_List_fromArray(
					[mdgriffith$style_elements$Style$Font$alignRight])),
				A2(
				mdgriffith$style_elements$Style$variation,
				elm_explorations$benchmark$Benchmark$Runner$Report$Text,
				_List_fromArray(
					[mdgriffith$style_elements$Style$Font$alignLeft]))
			])),
		mdgriffith$style_elements$Style$Sheet$merge(
		A3(mdgriffith$style_elements$Style$Sheet$map, elm_explorations$benchmark$Benchmark$Runner$Report$TextClass, elm$core$Basics$identity, elm_explorations$benchmark$Benchmark$Runner$Text$styles))
	]);
var mdgriffith$style_elements$Style$Internal$Model$FontName = function (a) {
	return {$: 'FontName', a: a};
};
var mdgriffith$style_elements$Style$Font$font = mdgriffith$style_elements$Style$Internal$Model$FontName;
var mdgriffith$style_elements$Style$Internal$Model$SansSerif = {$: 'SansSerif'};
var mdgriffith$style_elements$Style$Font$sansSerif = mdgriffith$style_elements$Style$Internal$Model$SansSerif;
var mdgriffith$style_elements$Style$Font$typeface = function (families) {
	return mdgriffith$style_elements$Style$Internal$Model$FontFamily(families);
};
var elm_explorations$benchmark$Benchmark$Runner$Text$body = _List_fromArray(
	[
		mdgriffith$style_elements$Style$Font$typeface(
		_List_fromArray(
			[
				mdgriffith$style_elements$Style$Font$font('Helvetica Neue'),
				mdgriffith$style_elements$Style$Font$font('Helvetica'),
				mdgriffith$style_elements$Style$Font$font('Arial'),
				mdgriffith$style_elements$Style$Font$sansSerif
			])),
		mdgriffith$style_elements$Style$Color$text(
		A3(mdgriffith$style_elements$Style$rgb, 15 / 255, 30 / 255, 45 / 255))
	]);
var elm_explorations$benchmark$Benchmark$Runner$App$styles = _List_fromArray(
	[
		A2(
		mdgriffith$style_elements$Style$style,
		elm_explorations$benchmark$Benchmark$Runner$App$Page,
		_Utils_ap(
			elm_explorations$benchmark$Benchmark$Runner$Text$body,
			_List_fromArray(
				[
					mdgriffith$style_elements$Style$Color$background(
					A3(mdgriffith$style_elements$Style$rgb, 242 / 255, 242 / 255, 242 / 255))
				]))),
		A2(mdgriffith$style_elements$Style$style, elm_explorations$benchmark$Benchmark$Runner$App$Wrapper, _List_Nil),
		mdgriffith$style_elements$Style$Sheet$merge(
		A3(mdgriffith$style_elements$Style$Sheet$map, elm_explorations$benchmark$Benchmark$Runner$App$InProgressClass, elm$core$Basics$identity, elm_explorations$benchmark$Benchmark$Runner$InProgress$styles)),
		mdgriffith$style_elements$Style$Sheet$merge(
		A3(mdgriffith$style_elements$Style$Sheet$map, elm_explorations$benchmark$Benchmark$Runner$App$ReportClass, elm_explorations$benchmark$Benchmark$Runner$App$ReportVariation, elm_explorations$benchmark$Benchmark$Runner$Report$styles))
	]);
var elm_explorations$benchmark$Benchmark$Runner$Box$spaceBetweenSections = 25;
var elm_explorations$benchmark$Benchmark$Runner$Box$barPaddingX = 15;
var elm_explorations$benchmark$Benchmark$Runner$Box$barPaddingY = 7;
var mdgriffith$style_elements$Element$Internal$Model$Element = function (a) {
	return {$: 'Element', a: a};
};
var mdgriffith$style_elements$Element$el = F3(
	function (style, attrs, child) {
		return mdgriffith$style_elements$Element$Internal$Model$Element(
			{
				absolutelyPositioned: elm$core$Maybe$Nothing,
				attrs: attrs,
				child: child,
				node: 'div',
				style: elm$core$Maybe$Just(style)
			});
	});
var mdgriffith$style_elements$Element$Internal$Model$Layout = function (a) {
	return {$: 'Layout', a: a};
};
var mdgriffith$style_elements$Element$Internal$Model$Normal = function (a) {
	return {$: 'Normal', a: a};
};
var mdgriffith$style_elements$Style$Internal$Model$FlexLayout = F2(
	function (a, b) {
		return {$: 'FlexLayout', a: a, b: b};
	});
var mdgriffith$style_elements$Style$Internal$Model$GoRight = {$: 'GoRight'};
var mdgriffith$style_elements$Element$row = F3(
	function (style, attrs, children) {
		return mdgriffith$style_elements$Element$Internal$Model$Layout(
			{
				absolutelyPositioned: elm$core$Maybe$Nothing,
				attrs: attrs,
				children: mdgriffith$style_elements$Element$Internal$Model$Normal(children),
				layout: A2(mdgriffith$style_elements$Style$Internal$Model$FlexLayout, mdgriffith$style_elements$Style$Internal$Model$GoRight, _List_Nil),
				node: 'div',
				style: elm$core$Maybe$Just(style)
			});
	});
var mdgriffith$style_elements$Element$Internal$Model$NoDecoration = {$: 'NoDecoration'};
var mdgriffith$style_elements$Element$Internal$Model$Text = F2(
	function (a, b) {
		return {$: 'Text', a: a, b: b};
	});
var mdgriffith$style_elements$Element$text = mdgriffith$style_elements$Element$Internal$Model$Text(
	{decoration: mdgriffith$style_elements$Element$Internal$Model$NoDecoration, inline: false});
var mdgriffith$style_elements$Style$Internal$Model$Px = function (a) {
	return {$: 'Px', a: a};
};
var mdgriffith$style_elements$Element$Attributes$px = mdgriffith$style_elements$Style$Internal$Model$Px;
var mdgriffith$style_elements$Element$Internal$Model$HAlign = function (a) {
	return {$: 'HAlign', a: a};
};
var mdgriffith$style_elements$Element$Internal$Model$Justify = {$: 'Justify'};
var mdgriffith$style_elements$Element$Attributes$spread = mdgriffith$style_elements$Element$Internal$Model$HAlign(mdgriffith$style_elements$Element$Internal$Model$Justify);
var mdgriffith$style_elements$Element$Internal$Model$VAlign = function (a) {
	return {$: 'VAlign', a: a};
};
var mdgriffith$style_elements$Element$Internal$Model$VerticalCenter = {$: 'VerticalCenter'};
var mdgriffith$style_elements$Element$Attributes$verticalCenter = mdgriffith$style_elements$Element$Internal$Model$VAlign(mdgriffith$style_elements$Element$Internal$Model$VerticalCenter);
var mdgriffith$style_elements$Element$Internal$Model$Width = function (a) {
	return {$: 'Width', a: a};
};
var mdgriffith$style_elements$Element$Attributes$width = mdgriffith$style_elements$Element$Internal$Model$Width;
var elm_explorations$benchmark$Benchmark$Runner$InProgress$caption = F2(
	function (name, status) {
		var informativeStatus = function () {
			switch (status.$) {
				case 'Cold':
					return 'Warming JIT';
				case 'Unsized':
					return 'Finding sample size';
				case 'Pending':
					return 'Collecting samples';
				case 'Failure':
					return 'Failed!';
				default:
					return 'Finished';
			}
		}();
		return A3(
			mdgriffith$style_elements$Element$row,
			elm_explorations$benchmark$Benchmark$Runner$InProgress$Unstyled,
			_List_fromArray(
				[
					mdgriffith$style_elements$Element$Attributes$width(
					mdgriffith$style_elements$Element$Attributes$px(500)),
					mdgriffith$style_elements$Element$Attributes$spread,
					mdgriffith$style_elements$Element$Attributes$verticalCenter
				]),
			_List_fromArray(
				[
					mdgriffith$style_elements$Element$text(name),
					A3(
					mdgriffith$style_elements$Element$el,
					elm_explorations$benchmark$Benchmark$Runner$InProgress$Status,
					_List_Nil,
					mdgriffith$style_elements$Element$text(informativeStatus))
				]));
	});
var mdgriffith$style_elements$Element$Internal$Model$Empty = {$: 'Empty'};
var mdgriffith$style_elements$Element$empty = mdgriffith$style_elements$Element$Internal$Model$Empty;
var elm$virtual_dom$VirtualDom$attribute = F2(
	function (key, value) {
		return A2(
			_VirtualDom_attribute,
			_VirtualDom_noOnOrFormAction(key),
			_VirtualDom_noJavaScriptOrHtmlUri(value));
	});
var elm$html$Html$Attributes$attribute = elm$virtual_dom$VirtualDom$attribute;
var mdgriffith$style_elements$Element$Internal$Model$Attr = function (a) {
	return {$: 'Attr', a: a};
};
var mdgriffith$style_elements$Element$Attributes$attribute = F2(
	function (name, val) {
		return mdgriffith$style_elements$Element$Internal$Model$Attr(
			A2(elm$html$Html$Attributes$attribute, name, val));
	});
var elm$virtual_dom$VirtualDom$style = _VirtualDom_style;
var mdgriffith$style_elements$Element$Attributes$clip = mdgriffith$style_elements$Element$Internal$Model$Attr(
	A2(elm$virtual_dom$VirtualDom$style, 'overflow', 'hidden'));
var mdgriffith$style_elements$Element$Internal$Model$Padding = F4(
	function (a, b, c, d) {
		return {$: 'Padding', a: a, b: b, c: c, d: d};
	});
var mdgriffith$style_elements$Element$Attributes$paddingBottom = function (x) {
	return A4(
		mdgriffith$style_elements$Element$Internal$Model$Padding,
		elm$core$Maybe$Nothing,
		elm$core$Maybe$Nothing,
		elm$core$Maybe$Just(x),
		elm$core$Maybe$Nothing);
};
var mdgriffith$style_elements$Element$Attributes$paddingLeft = function (x) {
	return A4(
		mdgriffith$style_elements$Element$Internal$Model$Padding,
		elm$core$Maybe$Nothing,
		elm$core$Maybe$Nothing,
		elm$core$Maybe$Nothing,
		elm$core$Maybe$Just(x));
};
var mdgriffith$style_elements$Element$Attributes$paddingTop = function (x) {
	return A4(
		mdgriffith$style_elements$Element$Internal$Model$Padding,
		elm$core$Maybe$Just(x),
		elm$core$Maybe$Nothing,
		elm$core$Maybe$Nothing,
		elm$core$Maybe$Nothing);
};
var mdgriffith$style_elements$Style$Internal$Model$Percent = function (a) {
	return {$: 'Percent', a: a};
};
var mdgriffith$style_elements$Element$Attributes$percent = mdgriffith$style_elements$Style$Internal$Model$Percent;
var elm_explorations$benchmark$Benchmark$Runner$InProgress$filledPortion = F2(
	function (name, status) {
		return (elm_explorations$benchmark$Benchmark$Status$progress(status) > 0) ? A3(
			mdgriffith$style_elements$Element$el,
			elm_explorations$benchmark$Benchmark$Runner$InProgress$Progress,
			_List_fromArray(
				[
					mdgriffith$style_elements$Element$Attributes$paddingTop(elm_explorations$benchmark$Benchmark$Runner$Box$barPaddingY),
					mdgriffith$style_elements$Element$Attributes$paddingBottom(elm_explorations$benchmark$Benchmark$Runner$Box$barPaddingY),
					mdgriffith$style_elements$Element$Attributes$paddingLeft(elm_explorations$benchmark$Benchmark$Runner$Box$barPaddingX),
					mdgriffith$style_elements$Element$Attributes$clip,
					mdgriffith$style_elements$Element$Attributes$width(
					mdgriffith$style_elements$Element$Attributes$percent(
						100 * elm_explorations$benchmark$Benchmark$Status$progress(status))),
					A2(mdgriffith$style_elements$Element$Attributes$attribute, 'role', 'progressbar'),
					A2(
					mdgriffith$style_elements$Element$Attributes$attribute,
					'aria-valuenow',
					elm$core$String$fromInt(
						elm$core$Basics$floor(
							100 * elm_explorations$benchmark$Benchmark$Status$progress(status)))),
					A2(mdgriffith$style_elements$Element$Attributes$attribute, 'aria-valuemin', '0'),
					A2(mdgriffith$style_elements$Element$Attributes$attribute, 'aria-valuemax', '100')
				]),
			A2(elm_explorations$benchmark$Benchmark$Runner$InProgress$caption, name, status)) : mdgriffith$style_elements$Element$empty;
	});
var mdgriffith$style_elements$Element$Internal$Model$Nearby = function (a) {
	return {$: 'Nearby', a: a};
};
var mdgriffith$style_elements$Element$Internal$Model$PositionFrame = function (a) {
	return {$: 'PositionFrame', a: a};
};
var mdgriffith$style_elements$Element$Internal$Model$Within = {$: 'Within'};
var mdgriffith$style_elements$Element$Internal$Model$Raw = function (a) {
	return {$: 'Raw', a: a};
};
var mdgriffith$style_elements$Element$Internal$Model$Spacer = function (a) {
	return {$: 'Spacer', a: a};
};
var mdgriffith$style_elements$Element$Internal$Modify$addAttr = F2(
	function (prop, el) {
		switch (el.$) {
			case 'Empty':
				return mdgriffith$style_elements$Element$Internal$Model$Empty;
			case 'Raw':
				var h = el.a;
				return mdgriffith$style_elements$Element$Internal$Model$Raw(h);
			case 'Spacer':
				var x = el.a;
				return mdgriffith$style_elements$Element$Internal$Model$Spacer(x);
			case 'Layout':
				var elm = el.a;
				return mdgriffith$style_elements$Element$Internal$Model$Layout(
					_Utils_update(
						elm,
						{
							attrs: A2(elm$core$List$cons, prop, elm.attrs)
						}));
			case 'Element':
				var elm = el.a;
				return mdgriffith$style_elements$Element$Internal$Model$Element(
					_Utils_update(
						elm,
						{
							attrs: A2(elm$core$List$cons, prop, elm.attrs)
						}));
			default:
				var dec = el.a;
				var content = el.b;
				return mdgriffith$style_elements$Element$Internal$Model$Element(
					{
						absolutelyPositioned: elm$core$Maybe$Nothing,
						attrs: _List_fromArray(
							[prop]),
						child: A2(mdgriffith$style_elements$Element$Internal$Model$Text, dec, content),
						node: 'div',
						style: elm$core$Maybe$Nothing
					});
		}
	});
var mdgriffith$style_elements$Element$Internal$Modify$addChild = F2(
	function (parent, el) {
		switch (parent.$) {
			case 'Empty':
				return mdgriffith$style_elements$Element$Internal$Model$Element(
					{
						absolutelyPositioned: elm$core$Maybe$Just(
							_List_fromArray(
								[el])),
						attrs: _List_Nil,
						child: mdgriffith$style_elements$Element$Internal$Model$Empty,
						node: 'div',
						style: elm$core$Maybe$Nothing
					});
			case 'Spacer':
				var x = parent.a;
				return mdgriffith$style_elements$Element$Internal$Model$Spacer(x);
			case 'Raw':
				var h = parent.a;
				return mdgriffith$style_elements$Element$Internal$Model$Raw(h);
			case 'Layout':
				var elm = parent.a;
				var absolutelyPositioned = elm.absolutelyPositioned;
				if (absolutelyPositioned.$ === 'Nothing') {
					return mdgriffith$style_elements$Element$Internal$Model$Layout(
						_Utils_update(
							elm,
							{
								absolutelyPositioned: elm$core$Maybe$Just(
									_List_fromArray(
										[el]))
							}));
				} else {
					var others = absolutelyPositioned.a;
					return mdgriffith$style_elements$Element$Internal$Model$Layout(
						_Utils_update(
							elm,
							{
								absolutelyPositioned: elm$core$Maybe$Just(
									A2(elm$core$List$cons, el, others))
							}));
				}
			case 'Element':
				var elm = parent.a;
				var absolutelyPositioned = elm.absolutelyPositioned;
				if (absolutelyPositioned.$ === 'Nothing') {
					return mdgriffith$style_elements$Element$Internal$Model$Element(
						_Utils_update(
							elm,
							{
								absolutelyPositioned: elm$core$Maybe$Just(
									_List_fromArray(
										[el]))
							}));
				} else {
					var others = absolutelyPositioned.a;
					return mdgriffith$style_elements$Element$Internal$Model$Element(
						_Utils_update(
							elm,
							{
								absolutelyPositioned: elm$core$Maybe$Just(
									A2(elm$core$List$cons, el, others))
							}));
				}
			default:
				var dec = parent.a;
				var content = parent.b;
				return mdgriffith$style_elements$Element$Internal$Model$Element(
					{
						absolutelyPositioned: elm$core$Maybe$Just(
							_List_fromArray(
								[el])),
						attrs: _List_Nil,
						child: A2(mdgriffith$style_elements$Element$Internal$Model$Text, dec, content),
						node: 'div',
						style: elm$core$Maybe$Nothing
					});
		}
	});
var mdgriffith$style_elements$Element$Internal$Modify$wrapHtml = function (el) {
	if (el.$ === 'Raw') {
		var h = el.a;
		return mdgriffith$style_elements$Element$Internal$Model$Element(
			{
				absolutelyPositioned: elm$core$Maybe$Nothing,
				attrs: _List_Nil,
				child: mdgriffith$style_elements$Element$Internal$Model$Raw(h),
				node: 'div',
				style: elm$core$Maybe$Nothing
			});
	} else {
		var x = el;
		return x;
	}
};
var mdgriffith$style_elements$Element$within = F2(
	function (nearbys, parent) {
		var position = F2(
			function (elem, p) {
				return A2(
					mdgriffith$style_elements$Element$Internal$Modify$addChild,
					p,
					A2(
						mdgriffith$style_elements$Element$Internal$Modify$addAttr,
						mdgriffith$style_elements$Element$Internal$Model$PositionFrame(
							mdgriffith$style_elements$Element$Internal$Model$Nearby(mdgriffith$style_elements$Element$Internal$Model$Within)),
						mdgriffith$style_elements$Element$Internal$Modify$wrapHtml(elem)));
			});
		return A3(elm$core$List$foldr, position, parent, nearbys);
	});
var mdgriffith$style_elements$Element$Attributes$paddingXY = F2(
	function (x, y) {
		return A4(
			mdgriffith$style_elements$Element$Internal$Model$Padding,
			elm$core$Maybe$Just(y),
			elm$core$Maybe$Just(x),
			elm$core$Maybe$Just(y),
			elm$core$Maybe$Just(x));
	});
var elm_explorations$benchmark$Benchmark$Runner$InProgress$progressBar = F2(
	function (name, status) {
		return A2(
			mdgriffith$style_elements$Element$within,
			_List_fromArray(
				[
					A2(elm_explorations$benchmark$Benchmark$Runner$InProgress$filledPortion, name, status)
				]),
			A3(
				mdgriffith$style_elements$Element$row,
				elm_explorations$benchmark$Benchmark$Runner$InProgress$Box,
				_List_fromArray(
					[
						A2(mdgriffith$style_elements$Element$Attributes$paddingXY, elm_explorations$benchmark$Benchmark$Runner$Box$barPaddingX, elm_explorations$benchmark$Benchmark$Runner$Box$barPaddingY),
						mdgriffith$style_elements$Element$Attributes$width(
						mdgriffith$style_elements$Element$Attributes$percent(100))
					]),
				_List_fromArray(
					[
						A2(elm_explorations$benchmark$Benchmark$Runner$InProgress$caption, name, status)
					])));
	});
var elm_explorations$benchmark$Benchmark$Runner$Text$path = F2(
	function (_class, parts) {
		return A3(
			mdgriffith$style_elements$Element$el,
			_class(elm_explorations$benchmark$Benchmark$Runner$Text$Path),
			_List_fromArray(
				[
					mdgriffith$style_elements$Element$Attributes$paddingBottom(3)
				]),
			mdgriffith$style_elements$Element$text(
				A2(elm$core$String$join, ' / ', parts)));
	});
var mdgriffith$style_elements$Style$Internal$Model$Down = {$: 'Down'};
var mdgriffith$style_elements$Element$column = F3(
	function (style, attrs, children) {
		return mdgriffith$style_elements$Element$Internal$Model$Layout(
			{
				absolutelyPositioned: elm$core$Maybe$Nothing,
				attrs: attrs,
				children: mdgriffith$style_elements$Element$Internal$Model$Normal(children),
				layout: A2(mdgriffith$style_elements$Style$Internal$Model$FlexLayout, mdgriffith$style_elements$Style$Internal$Model$Down, _List_Nil),
				node: 'div',
				style: elm$core$Maybe$Just(style)
			});
	});
var elm_explorations$benchmark$Benchmark$Runner$InProgress$barsWithPath = F2(
	function (parents, children) {
		return A3(
			mdgriffith$style_elements$Element$column,
			elm_explorations$benchmark$Benchmark$Runner$InProgress$Unstyled,
			_List_fromArray(
				[
					mdgriffith$style_elements$Element$Attributes$paddingTop(elm_explorations$benchmark$Benchmark$Runner$Box$spaceBetweenSections)
				]),
			A2(
				elm$core$List$cons,
				A2(elm_explorations$benchmark$Benchmark$Runner$Text$path, elm_explorations$benchmark$Benchmark$Runner$InProgress$TextClass, parents),
				A2(
					elm$core$List$map,
					function (_n0) {
						var a = _n0.a;
						var b = _n0.b;
						return A2(elm_explorations$benchmark$Benchmark$Runner$InProgress$progressBar, a, b);
					},
					children)));
	});
var elm_explorations$benchmark$Benchmark$Runner$InProgress$progressBars = F2(
	function (reversedParents, report) {
		switch (report.$) {
			case 'Single':
				var name = report.a;
				var status = report.b;
				return _List_fromArray(
					[
						A2(
						elm_explorations$benchmark$Benchmark$Runner$InProgress$barsWithPath,
						elm$core$List$reverse(reversedParents),
						_List_fromArray(
							[
								_Utils_Tuple2(name, status)
							]))
					]);
			case 'Series':
				var name = report.a;
				var statuses = report.b;
				return _List_fromArray(
					[
						A2(
						elm_explorations$benchmark$Benchmark$Runner$InProgress$barsWithPath,
						elm$core$List$reverse(
							A2(elm$core$List$cons, name, reversedParents)),
						statuses)
					]);
			default:
				var name = report.a;
				var reports = report.b;
				return elm$core$List$concat(
					A2(
						elm$core$List$map,
						elm_explorations$benchmark$Benchmark$Runner$InProgress$progressBars(
							A2(elm$core$List$cons, name, reversedParents)),
						reports));
		}
	});
var mdgriffith$style_elements$Element$Internal$Modify$setNode = F2(
	function (node, el) {
		switch (el.$) {
			case 'Empty':
				return mdgriffith$style_elements$Element$Internal$Model$Empty;
			case 'Raw':
				var h = el.a;
				return mdgriffith$style_elements$Element$Internal$Model$Raw(h);
			case 'Spacer':
				var x = el.a;
				return mdgriffith$style_elements$Element$Internal$Model$Spacer(x);
			case 'Layout':
				var elm = el.a;
				return mdgriffith$style_elements$Element$Internal$Model$Layout(
					_Utils_update(
						elm,
						{node: node}));
			case 'Element':
				var elm = el.a;
				return mdgriffith$style_elements$Element$Internal$Model$Element(
					_Utils_update(
						elm,
						{node: node}));
			default:
				var dec = el.a;
				var content = el.b;
				return mdgriffith$style_elements$Element$Internal$Model$Element(
					{
						absolutelyPositioned: elm$core$Maybe$Nothing,
						attrs: _List_Nil,
						child: A2(mdgriffith$style_elements$Element$Internal$Model$Text, dec, content),
						node: node,
						style: elm$core$Maybe$Nothing
					});
		}
	});
var mdgriffith$style_elements$Element$node = function (str) {
	return mdgriffith$style_elements$Element$Internal$Modify$setNode(str);
};
var elm_explorations$benchmark$Benchmark$Runner$Text$hero = F2(
	function (_class, caption) {
		return A2(
			mdgriffith$style_elements$Element$node,
			'h1',
			A3(
				mdgriffith$style_elements$Element$el,
				_class(elm_explorations$benchmark$Benchmark$Runner$Text$Hero),
				_List_fromArray(
					[
						mdgriffith$style_elements$Element$Attributes$width(
						mdgriffith$style_elements$Element$Attributes$percent(100)),
						mdgriffith$style_elements$Element$Attributes$paddingBottom(20)
					]),
				mdgriffith$style_elements$Element$text(caption)));
	});
var elm_explorations$benchmark$Benchmark$Runner$InProgress$view = function (report) {
	return A3(
		mdgriffith$style_elements$Element$column,
		elm_explorations$benchmark$Benchmark$Runner$InProgress$Unstyled,
		_List_Nil,
		A2(
			elm$core$List$cons,
			A2(elm_explorations$benchmark$Benchmark$Runner$Text$hero, elm_explorations$benchmark$Benchmark$Runner$InProgress$TextClass, 'Benchmarks Running'),
			A2(elm_explorations$benchmark$Benchmark$Runner$InProgress$progressBars, _List_Nil, report)));
};
var mdgriffith$style_elements$Element$Internal$Model$Vary = F2(
	function (a, b) {
		return {$: 'Vary', a: a, b: b};
	});
var mdgriffith$style_elements$Element$Attributes$vary = mdgriffith$style_elements$Element$Internal$Model$Vary;
var elm_explorations$benchmark$Benchmark$Runner$Report$cell = F2(
	function (variation, contents) {
		return A3(
			mdgriffith$style_elements$Element$el,
			elm_explorations$benchmark$Benchmark$Runner$Report$Cell,
			_List_fromArray(
				[
					A2(mdgriffith$style_elements$Element$Attributes$vary, variation, true),
					mdgriffith$style_elements$Element$Attributes$paddingTop(5)
				]),
			contents);
	});
var BrianHicks$elm_trend$Trend$Linear$goodnessOfFit = function (_n0) {
	var fit = _n0.a;
	var values = _n0.b.a;
	var _n1 = elm$core$List$unzip(values);
	var xs = _n1.a;
	var ys = _n1.b;
	var predictions = A2(
		elm$core$List$map,
		BrianHicks$elm_trend$Trend$Linear$predictY(fit),
		xs);
	var meanY = A2(
		elm$core$Result$withDefault,
		0,
		BrianHicks$elm_trend$Trend$Math$mean(ys));
	var sumSquareResiduals = elm$core$List$sum(
		A3(
			elm$core$List$map2,
			F2(
				function (actual, prediction) {
					return A2(elm$core$Basics$pow, actual - prediction, 2);
				}),
			ys,
			predictions));
	var sumSquareTotal = elm$core$List$sum(
		A2(
			elm$core$List$map,
			function (y) {
				return A2(elm$core$Basics$pow, y - meanY, 2);
			},
			ys));
	return 1 - (sumSquareResiduals / sumSquareTotal);
};
var elm_explorations$benchmark$Benchmark$Runner$Humanize$percent = A2(
	elm$core$Basics$composeR,
	elm$core$Basics$mul(10000),
	A2(
		elm$core$Basics$composeR,
		elm$core$Basics$round,
		A2(
			elm$core$Basics$composeR,
			elm$core$Basics$toFloat,
			A2(
				elm$core$Basics$composeR,
				function (a) {
					return a / 100;
				},
				A2(
					elm$core$Basics$composeR,
					elm$core$String$fromFloat,
					function (a) {
						return a + '%';
					})))));
var elm_explorations$benchmark$Benchmark$Runner$Report$goodnessOfFit = A2(
	elm$core$Basics$composeR,
	BrianHicks$elm_trend$Trend$Linear$goodnessOfFit,
	A2(
		elm$core$Basics$composeR,
		elm_explorations$benchmark$Benchmark$Runner$Humanize$percent,
		A2(
			elm$core$Basics$composeR,
			mdgriffith$style_elements$Element$text,
			elm_explorations$benchmark$Benchmark$Runner$Report$cell(elm_explorations$benchmark$Benchmark$Runner$Report$Numeric))));
var elm_explorations$benchmark$Benchmark$Runner$Report$header = F2(
	function (variation, caption) {
		return A3(
			mdgriffith$style_elements$Element$el,
			elm_explorations$benchmark$Benchmark$Runner$Report$Header,
			_List_fromArray(
				[
					A2(mdgriffith$style_elements$Element$Attributes$vary, variation, true)
				]),
			mdgriffith$style_elements$Element$text(caption));
	});
var BrianHicks$elm_trend$Trend$Linear$predictX = F2(
	function (_n0, y) {
		var slope = _n0.slope;
		var intercept = _n0.intercept;
		return (y - intercept) / slope;
	});
var elm_explorations$benchmark$Benchmark$Runner$Report$percentChange = F2(
	function (old, _new) {
		var rps = A2(
			elm$core$Basics$composeR,
			BrianHicks$elm_trend$Trend$Linear$line,
			function (a) {
				return A2(BrianHicks$elm_trend$Trend$Linear$predictX, a, 1000);
			});
		var change = (rps(_new) - rps(old)) / rps(old);
		var sign = (change > 0) ? '+' : '';
		return _Utils_eq(old, _new) ? A2(
			elm_explorations$benchmark$Benchmark$Runner$Report$cell,
			elm_explorations$benchmark$Benchmark$Runner$Report$Numeric,
			mdgriffith$style_elements$Element$text('-')) : A2(
			elm_explorations$benchmark$Benchmark$Runner$Report$cell,
			elm_explorations$benchmark$Benchmark$Runner$Report$Numeric,
			mdgriffith$style_elements$Element$text(
				_Utils_ap(
					sign,
					elm_explorations$benchmark$Benchmark$Runner$Humanize$percent(change))));
	});
var elm_explorations$benchmark$Benchmark$Runner$Report$pointsFromStatus = function (status) {
	if (status.$ === 'Success') {
		var samples = status.a;
		return elm$core$Maybe$Just(
			elm_explorations$benchmark$Benchmark$Samples$points(samples));
	} else {
		return elm$core$Maybe$Nothing;
	}
};
var mdgriffith$style_elements$Element$Internal$Model$GridCoords = function (a) {
	return {$: 'GridCoords', a: a};
};
var mdgriffith$style_elements$Element$Internal$Model$OnGrid = function (a) {
	return {$: 'OnGrid', a: a};
};
var mdgriffith$style_elements$Style$Internal$Model$GridPosition = function (a) {
	return {$: 'GridPosition', a: a};
};
var mdgriffith$style_elements$Element$cell = function (box) {
	var coords = {height: box.height, start: box.start, width: box.width};
	return mdgriffith$style_elements$Element$Internal$Model$OnGrid(
		A2(
			mdgriffith$style_elements$Element$Internal$Modify$addAttr,
			mdgriffith$style_elements$Element$Internal$Model$GridCoords(
				mdgriffith$style_elements$Style$Internal$Model$GridPosition(coords)),
			box.content));
};
var mdgriffith$style_elements$Style$Internal$Model$Grid = F2(
	function (a, b) {
		return {$: 'Grid', a: a, b: b};
	});
var mdgriffith$style_elements$Style$Internal$Model$GridGap = F2(
	function (a, b) {
		return {$: 'GridGap', a: a, b: b};
	});
var mdgriffith$style_elements$Style$Internal$Model$GridTemplate = function (a) {
	return {$: 'GridTemplate', a: a};
};
var mdgriffith$style_elements$Element$grid = F3(
	function (style, attrs, config) {
		var prepare = function (elem) {
			return mdgriffith$style_elements$Element$Internal$Model$Normal(
				A2(
					elm$core$List$map,
					function (_n4) {
						var x = _n4.a;
						return x;
					},
					elem));
		};
		var forSpacing = function (attr) {
			if (attr.$ === 'Spacing') {
				return true;
			} else {
				return false;
			}
		};
		var _n0 = A2(elm$core$List$partition, forSpacing, attrs);
		var spacing = _n0.a;
		var notSpacingAttrs = _n0.b;
		var gridAttributes = function () {
			var _n1 = elm$core$List$head(
				elm$core$List$reverse(spacing));
			if (_n1.$ === 'Nothing') {
				return _List_Nil;
			} else {
				if (_n1.a.$ === 'Spacing') {
					var _n2 = _n1.a;
					var x = _n2.a;
					var y = _n2.b;
					return _List_fromArray(
						[
							A2(mdgriffith$style_elements$Style$Internal$Model$GridGap, x, y)
						]);
				} else {
					return _List_Nil;
				}
			}
		}();
		return mdgriffith$style_elements$Element$Internal$Model$Layout(
			{
				absolutelyPositioned: elm$core$Maybe$Nothing,
				attrs: notSpacingAttrs,
				children: prepare(config.cells),
				layout: A2(
					mdgriffith$style_elements$Style$Internal$Model$Grid,
					mdgriffith$style_elements$Style$Internal$Model$GridTemplate(
						{columns: config.columns, rows: config.rows}),
					gridAttributes),
				node: 'div',
				style: elm$core$Maybe$Just(style)
			});
	});
var mdgriffith$style_elements$Element$table = F3(
	function (style, attrs, rows) {
		var children = elm$core$List$concat(
			A2(
				elm$core$List$indexedMap,
				F2(
					function (rowIndex, columns) {
						return A2(
							elm$core$List$indexedMap,
							F2(
								function (col, content) {
									return mdgriffith$style_elements$Element$cell(
										{
											content: content,
											height: 1,
											start: _Utils_Tuple2(rowIndex, col),
											width: 1
										});
								}),
							columns);
					}),
				rows));
		return A3(
			mdgriffith$style_elements$Element$grid,
			style,
			attrs,
			{cells: children, columns: _List_Nil, rows: _List_Nil});
	});
var elm_explorations$benchmark$Benchmark$Runner$Report$report = F4(
	function (parents, name, points, tableContents) {
		return A3(
			mdgriffith$style_elements$Element$column,
			elm_explorations$benchmark$Benchmark$Runner$Report$Unstyled,
			_List_fromArray(
				[
					mdgriffith$style_elements$Element$Attributes$paddingTop(elm_explorations$benchmark$Benchmark$Runner$Box$spaceBetweenSections)
				]),
			_List_fromArray(
				[
					A2(elm_explorations$benchmark$Benchmark$Runner$Text$path, elm_explorations$benchmark$Benchmark$Runner$Report$TextClass, parents),
					A3(
					mdgriffith$style_elements$Element$column,
					elm_explorations$benchmark$Benchmark$Runner$Report$Box,
					_List_fromArray(
						[
							A2(mdgriffith$style_elements$Element$Attributes$paddingXY, elm_explorations$benchmark$Benchmark$Runner$Box$barPaddingX, elm_explorations$benchmark$Benchmark$Runner$Box$barPaddingY),
							mdgriffith$style_elements$Element$Attributes$width(
							mdgriffith$style_elements$Element$Attributes$px(500))
						]),
					_List_fromArray(
						[
							mdgriffith$style_elements$Element$text(name),
							A3(
							mdgriffith$style_elements$Element$table,
							elm_explorations$benchmark$Benchmark$Runner$Report$Table,
							_List_fromArray(
								[
									mdgriffith$style_elements$Element$Attributes$width(
									mdgriffith$style_elements$Element$Attributes$percent(100)),
									mdgriffith$style_elements$Element$Attributes$paddingTop(10)
								]),
							tableContents)
						]))
				]));
	});
var elm$core$String$fromList = _String_fromList;
var elm$core$String$foldr = _String_foldr;
var elm$core$String$toList = function (string) {
	return A3(elm$core$String$foldr, elm$core$List$cons, _List_Nil, string);
};
var elm_explorations$benchmark$Benchmark$Runner$Humanize$groupsOf = F2(
	function (howMany, items) {
		var _n0 = A2(elm$core$List$take, howMany, items);
		if (!_n0.b) {
			return _List_Nil;
		} else {
			var xs = _n0;
			return A2(
				elm$core$List$cons,
				xs,
				A2(
					elm_explorations$benchmark$Benchmark$Runner$Humanize$groupsOf,
					howMany,
					A2(elm$core$List$drop, howMany, items)));
		}
	});
var elm_explorations$benchmark$Benchmark$Runner$Humanize$int = A2(
	elm$core$Basics$composeR,
	elm$core$String$fromInt,
	A2(
		elm$core$Basics$composeR,
		elm$core$String$toList,
		A2(
			elm$core$Basics$composeR,
			elm$core$List$reverse,
			A2(
				elm$core$Basics$composeR,
				elm_explorations$benchmark$Benchmark$Runner$Humanize$groupsOf(3),
				A2(
					elm$core$Basics$composeR,
					elm$core$List$reverse,
					A2(
						elm$core$Basics$composeR,
						elm$core$List$map(
							A2(elm$core$Basics$composeR, elm$core$List$reverse, elm$core$String$fromList)),
						elm$core$String$join(',')))))));
var elm_explorations$benchmark$Benchmark$Runner$Report$runsPerSecond = function (variation) {
	return A2(
		elm$core$Basics$composeR,
		BrianHicks$elm_trend$Trend$Linear$line,
		A2(
			elm$core$Basics$composeR,
			function (a) {
				return A2(BrianHicks$elm_trend$Trend$Linear$predictX, a, 1000);
			},
			A2(
				elm$core$Basics$composeR,
				elm$core$Basics$floor,
				A2(
					elm$core$Basics$composeR,
					elm_explorations$benchmark$Benchmark$Runner$Humanize$int,
					A2(
						elm$core$Basics$composeR,
						mdgriffith$style_elements$Element$text,
						elm_explorations$benchmark$Benchmark$Runner$Report$cell(variation))))));
};
var elm_explorations$benchmark$Benchmark$Runner$Report$trendFromStatus = function (status) {
	if (status.$ === 'Success') {
		var trend = status.b;
		return elm$core$Maybe$Just(trend);
	} else {
		return elm$core$Maybe$Nothing;
	}
};
var elm_explorations$benchmark$Benchmark$Runner$Report$trendsFromStatuses = A2(
	elm$core$List$foldr,
	F2(
		function (_this, acc) {
			return A3(
				elm$core$Maybe$map2,
				elm$core$List$cons,
				elm_explorations$benchmark$Benchmark$Runner$Report$trendFromStatus(_this),
				acc);
		}),
	elm$core$Maybe$Just(_List_Nil));
var elm_explorations$benchmark$Benchmark$Runner$Report$multiReport = F3(
	function (parents, name, children) {
		var _n0 = elm$core$List$unzip(children);
		var names = _n0.a;
		var statuses = _n0.b;
		var allPoints = A3(
			elm$core$List$foldr,
			elm$core$Maybe$map2(elm$core$List$cons),
			elm$core$Maybe$Just(_List_Nil),
			A2(elm$core$List$map, elm_explorations$benchmark$Benchmark$Runner$Report$pointsFromStatus, statuses));
		var contents = A2(
			elm$core$Maybe$map,
			function (trends) {
				return _List_fromArray(
					[
						A2(
						elm$core$List$cons,
						A2(elm_explorations$benchmark$Benchmark$Runner$Report$header, elm_explorations$benchmark$Benchmark$Runner$Report$Text, 'name'),
						A2(
							elm$core$List$map,
							A2(
								elm$core$Basics$composeR,
								mdgriffith$style_elements$Element$text,
								elm_explorations$benchmark$Benchmark$Runner$Report$cell(elm_explorations$benchmark$Benchmark$Runner$Report$Text)),
							names)),
						A2(
						elm$core$List$cons,
						A2(elm_explorations$benchmark$Benchmark$Runner$Report$header, elm_explorations$benchmark$Benchmark$Runner$Report$Numeric, 'runs / second'),
						A2(
							elm$core$List$map,
							elm_explorations$benchmark$Benchmark$Runner$Report$runsPerSecond(elm_explorations$benchmark$Benchmark$Runner$Report$Numeric),
							trends)),
						A2(
						elm$core$List$cons,
						A2(elm_explorations$benchmark$Benchmark$Runner$Report$header, elm_explorations$benchmark$Benchmark$Runner$Report$Numeric, '% change'),
						A2(
							elm$core$List$cons,
							A2(
								elm_explorations$benchmark$Benchmark$Runner$Report$cell,
								elm_explorations$benchmark$Benchmark$Runner$Report$Numeric,
								mdgriffith$style_elements$Element$text('-')),
							A3(
								elm$core$List$map2,
								elm_explorations$benchmark$Benchmark$Runner$Report$percentChange,
								trends,
								A2(elm$core$List$drop, 1, trends)))),
						A2(
						elm$core$List$cons,
						A2(elm_explorations$benchmark$Benchmark$Runner$Report$header, elm_explorations$benchmark$Benchmark$Runner$Report$Numeric, 'goodness of fit'),
						A2(elm$core$List$map, elm_explorations$benchmark$Benchmark$Runner$Report$goodnessOfFit, trends))
					]);
			},
			elm_explorations$benchmark$Benchmark$Runner$Report$trendsFromStatuses(statuses));
		return A2(
			elm$core$Maybe$withDefault,
			mdgriffith$style_elements$Element$empty,
			A3(
				elm$core$Maybe$map2,
				A2(elm_explorations$benchmark$Benchmark$Runner$Report$report, parents, name),
				allPoints,
				contents));
	});
var elm$core$List$singleton = function (value) {
	return _List_fromArray(
		[value]);
};
var elm_explorations$benchmark$Benchmark$Runner$Report$singleReport = F3(
	function (parents, name, status) {
		var contents = A2(
			elm$core$Maybe$map,
			function (trend) {
				return _List_fromArray(
					[
						_List_fromArray(
						[
							A2(elm_explorations$benchmark$Benchmark$Runner$Report$header, elm_explorations$benchmark$Benchmark$Runner$Report$Text, 'runs / second'),
							A2(elm_explorations$benchmark$Benchmark$Runner$Report$runsPerSecond, elm_explorations$benchmark$Benchmark$Runner$Report$Text, trend)
						]),
						_List_fromArray(
						[
							A2(elm_explorations$benchmark$Benchmark$Runner$Report$header, elm_explorations$benchmark$Benchmark$Runner$Report$Numeric, 'goodness of fit'),
							elm_explorations$benchmark$Benchmark$Runner$Report$goodnessOfFit(trend)
						])
					]);
			},
			elm_explorations$benchmark$Benchmark$Runner$Report$trendFromStatus(status));
		return A2(
			elm$core$Maybe$withDefault,
			mdgriffith$style_elements$Element$empty,
			A3(
				elm$core$Maybe$map2,
				A2(elm_explorations$benchmark$Benchmark$Runner$Report$report, parents, name),
				A2(
					elm$core$Maybe$map,
					elm$core$List$singleton,
					elm_explorations$benchmark$Benchmark$Runner$Report$pointsFromStatus(status)),
				contents));
	});
var elm_explorations$benchmark$Benchmark$Runner$Report$reports = F2(
	function (reversedParents, report_) {
		switch (report_.$) {
			case 'Single':
				var name = report_.a;
				var status = report_.b;
				return _List_fromArray(
					[
						A3(
						elm_explorations$benchmark$Benchmark$Runner$Report$singleReport,
						elm$core$List$reverse(reversedParents),
						name,
						status)
					]);
			case 'Series':
				var name = report_.a;
				var statuses = report_.b;
				return _List_fromArray(
					[
						A3(
						elm_explorations$benchmark$Benchmark$Runner$Report$multiReport,
						elm$core$List$reverse(reversedParents),
						name,
						statuses)
					]);
			default:
				var name = report_.a;
				var children = report_.b;
				return elm$core$List$concat(
					A2(
						elm$core$List$map,
						elm_explorations$benchmark$Benchmark$Runner$Report$reports(
							A2(elm$core$List$cons, name, reversedParents)),
						children));
		}
	});
var elm_explorations$benchmark$Benchmark$Runner$Report$view = function (report_) {
	return A3(
		mdgriffith$style_elements$Element$column,
		elm_explorations$benchmark$Benchmark$Runner$Report$Unstyled,
		_List_Nil,
		A2(
			elm$core$List$cons,
			A2(elm_explorations$benchmark$Benchmark$Runner$Text$hero, elm_explorations$benchmark$Benchmark$Runner$Report$TextClass, 'Benchmark Report'),
			A2(elm_explorations$benchmark$Benchmark$Runner$Report$reports, _List_Nil, report_)));
};
var elm$virtual_dom$VirtualDom$map = _VirtualDom_map;
var elm$html$Html$map = elm$virtual_dom$VirtualDom$map;
var elm$virtual_dom$VirtualDom$mapAttribute = _VirtualDom_mapAttribute;
var elm$html$Html$Attributes$map = elm$virtual_dom$VirtualDom$mapAttribute;
var mdgriffith$style_elements$Element$Internal$Model$Event = function (a) {
	return {$: 'Event', a: a};
};
var mdgriffith$style_elements$Element$Internal$Model$Expand = {$: 'Expand'};
var mdgriffith$style_elements$Element$Internal$Model$GridArea = function (a) {
	return {$: 'GridArea', a: a};
};
var mdgriffith$style_elements$Element$Internal$Model$Height = function (a) {
	return {$: 'Height', a: a};
};
var mdgriffith$style_elements$Element$Internal$Model$Hidden = {$: 'Hidden'};
var mdgriffith$style_elements$Element$Internal$Model$Inline = {$: 'Inline'};
var mdgriffith$style_elements$Element$Internal$Model$InputEvent = function (a) {
	return {$: 'InputEvent', a: a};
};
var mdgriffith$style_elements$Element$Internal$Model$Margin = F4(
	function (a, b, c, d) {
		return {$: 'Margin', a: a, b: b, c: c, d: d};
	});
var mdgriffith$style_elements$Element$Internal$Model$Opacity = function (a) {
	return {$: 'Opacity', a: a};
};
var mdgriffith$style_elements$Element$Internal$Model$Overflow = function (a) {
	return {$: 'Overflow', a: a};
};
var mdgriffith$style_elements$Element$Internal$Model$PhantomPadding = F4(
	function (a, b, c, d) {
		return {$: 'PhantomPadding', a: a, b: b, c: c, d: d};
	});
var mdgriffith$style_elements$Element$Internal$Model$PointerEvents = function (a) {
	return {$: 'PointerEvents', a: a};
};
var mdgriffith$style_elements$Element$Internal$Model$Position = F3(
	function (a, b, c) {
		return {$: 'Position', a: a, b: b, c: c};
	});
var mdgriffith$style_elements$Element$Internal$Model$Shrink = function (a) {
	return {$: 'Shrink', a: a};
};
var mdgriffith$style_elements$Element$Internal$Model$Spacing = F2(
	function (a, b) {
		return {$: 'Spacing', a: a, b: b};
	});
var mdgriffith$style_elements$Element$Internal$Model$mapAllAttr = F3(
	function (fnMsg, fnVar, attr) {
		switch (attr.$) {
			case 'Event':
				var htmlAttr = attr.a;
				return mdgriffith$style_elements$Element$Internal$Model$Event(
					A2(elm$html$Html$Attributes$map, fnMsg, htmlAttr));
			case 'InputEvent':
				var htmlAttr = attr.a;
				return mdgriffith$style_elements$Element$Internal$Model$InputEvent(
					A2(elm$html$Html$Attributes$map, fnMsg, htmlAttr));
			case 'Attr':
				var htmlAttr = attr.a;
				return mdgriffith$style_elements$Element$Internal$Model$Attr(
					A2(elm$html$Html$Attributes$map, fnMsg, htmlAttr));
			case 'Vary':
				var v = attr.a;
				var b = attr.b;
				return A2(
					mdgriffith$style_elements$Element$Internal$Model$Vary,
					fnVar(v),
					b);
			case 'Height':
				var len = attr.a;
				return mdgriffith$style_elements$Element$Internal$Model$Height(len);
			case 'Width':
				var len = attr.a;
				return mdgriffith$style_elements$Element$Internal$Model$Width(len);
			case 'Inline':
				return mdgriffith$style_elements$Element$Internal$Model$Inline;
			case 'HAlign':
				var align = attr.a;
				return mdgriffith$style_elements$Element$Internal$Model$HAlign(align);
			case 'VAlign':
				var align = attr.a;
				return mdgriffith$style_elements$Element$Internal$Model$VAlign(align);
			case 'Position':
				var x = attr.a;
				var y = attr.b;
				var z = attr.c;
				return A3(mdgriffith$style_elements$Element$Internal$Model$Position, x, y, z);
			case 'PositionFrame':
				var fr = attr.a;
				return mdgriffith$style_elements$Element$Internal$Model$PositionFrame(fr);
			case 'Hidden':
				return mdgriffith$style_elements$Element$Internal$Model$Hidden;
			case 'Opacity':
				var o = attr.a;
				return mdgriffith$style_elements$Element$Internal$Model$Opacity(o);
			case 'Spacing':
				var x = attr.a;
				var y = attr.b;
				return A2(mdgriffith$style_elements$Element$Internal$Model$Spacing, x, y);
			case 'Margin':
				var t = attr.a;
				var r = attr.b;
				var b = attr.c;
				var l = attr.d;
				return A4(mdgriffith$style_elements$Element$Internal$Model$Margin, t, r, b, l);
			case 'Expand':
				return mdgriffith$style_elements$Element$Internal$Model$Expand;
			case 'Padding':
				var t = attr.a;
				var r = attr.b;
				var b = attr.c;
				var l = attr.d;
				return A4(mdgriffith$style_elements$Element$Internal$Model$Padding, t, r, b, l);
			case 'PhantomPadding':
				var t = attr.a;
				var r = attr.b;
				var b = attr.c;
				var l = attr.d;
				return A4(mdgriffith$style_elements$Element$Internal$Model$PhantomPadding, t, r, b, l);
			case 'GridArea':
				var str = attr.a;
				return mdgriffith$style_elements$Element$Internal$Model$GridArea(str);
			case 'GridCoords':
				var pos = attr.a;
				return mdgriffith$style_elements$Element$Internal$Model$GridCoords(pos);
			case 'PointerEvents':
				var on = attr.a;
				return mdgriffith$style_elements$Element$Internal$Model$PointerEvents(on);
			case 'Shrink':
				var i = attr.a;
				return mdgriffith$style_elements$Element$Internal$Model$Shrink(i);
			default:
				var x = attr.a;
				return mdgriffith$style_elements$Element$Internal$Model$Overflow(x);
		}
	});
var mdgriffith$style_elements$Element$Internal$Model$Keyed = function (a) {
	return {$: 'Keyed', a: a};
};
var mdgriffith$style_elements$Element$Internal$Model$mapChildren = F2(
	function (fn, children) {
		if (children.$ === 'Normal') {
			var c = children.a;
			return mdgriffith$style_elements$Element$Internal$Model$Normal(
				A2(elm$core$List$map, fn, c));
		} else {
			var keyed = children.a;
			return mdgriffith$style_elements$Element$Internal$Model$Keyed(
				A2(
					elm$core$List$map,
					elm$core$Tuple$mapSecond(fn),
					keyed));
		}
	});
var mdgriffith$style_elements$Element$Internal$Model$mapAll = F4(
	function (onMsg, onStyle, onVariation, el) {
		switch (el.$) {
			case 'Empty':
				return mdgriffith$style_elements$Element$Internal$Model$Empty;
			case 'Spacer':
				var f = el.a;
				return mdgriffith$style_elements$Element$Internal$Model$Spacer(f);
			case 'Text':
				var dec = el.a;
				var str = el.b;
				return A2(mdgriffith$style_elements$Element$Internal$Model$Text, dec, str);
			case 'Element':
				var elm = el.a;
				var attrs = elm.attrs;
				var child = elm.child;
				var absolutelyPositioned = elm.absolutelyPositioned;
				var style = elm.style;
				return mdgriffith$style_elements$Element$Internal$Model$Element(
					{
						absolutelyPositioned: A2(
							elm$core$Maybe$map,
							elm$core$List$map(
								function (childEl) {
									return A4(mdgriffith$style_elements$Element$Internal$Model$mapAll, onMsg, onStyle, onVariation, childEl);
								}),
							absolutelyPositioned),
						attrs: A2(
							elm$core$List$map,
							A2(mdgriffith$style_elements$Element$Internal$Model$mapAllAttr, onMsg, onVariation),
							attrs),
						child: A4(mdgriffith$style_elements$Element$Internal$Model$mapAll, onMsg, onStyle, onVariation, child),
						node: elm.node,
						style: A2(elm$core$Maybe$map, onStyle, style)
					});
			case 'Layout':
				var elm = el.a;
				var attrs = elm.attrs;
				var children = elm.children;
				var absolutelyPositioned = elm.absolutelyPositioned;
				var style = elm.style;
				return mdgriffith$style_elements$Element$Internal$Model$Layout(
					{
						absolutelyPositioned: A2(
							elm$core$Maybe$map,
							elm$core$List$map(
								function (child) {
									return A4(mdgriffith$style_elements$Element$Internal$Model$mapAll, onMsg, onStyle, onVariation, child);
								}),
							absolutelyPositioned),
						attrs: A2(
							elm$core$List$map,
							A2(mdgriffith$style_elements$Element$Internal$Model$mapAllAttr, onMsg, onVariation),
							attrs),
						children: A2(
							mdgriffith$style_elements$Element$Internal$Model$mapChildren,
							function (child) {
								return A4(mdgriffith$style_elements$Element$Internal$Model$mapAll, onMsg, onStyle, onVariation, child);
							},
							children),
						layout: elm.layout,
						node: elm.node,
						style: A2(elm$core$Maybe$map, onStyle, style)
					});
			default:
				var html = el.a;
				return mdgriffith$style_elements$Element$Internal$Model$Raw(
					A2(elm$html$Html$map, onMsg, html));
		}
	});
var mdgriffith$style_elements$Element$mapAll = mdgriffith$style_elements$Element$Internal$Model$mapAll;
var elm$html$Html$div = _VirtualDom_node('div');
var elm$json$Json$Encode$string = _Json_wrap;
var elm$html$Html$Attributes$stringProperty = F2(
	function (key, string) {
		return A2(
			_VirtualDom_property,
			key,
			elm$json$Json$Encode$string(string));
	});
var elm$html$Html$Attributes$class = elm$html$Html$Attributes$stringProperty('className');
var elm$html$Html$Attributes$style = elm$virtual_dom$VirtualDom$style;
var elm$virtual_dom$VirtualDom$node = function (tag) {
	return _VirtualDom_node(
		_VirtualDom_noScript(tag));
};
var elm$html$Html$node = elm$virtual_dom$VirtualDom$node;
var elm$virtual_dom$VirtualDom$text = _VirtualDom_text;
var elm$html$Html$text = elm$virtual_dom$VirtualDom$text;
var mdgriffith$style_elements$Element$Internal$Render$withFocus = '\n\n.style-elements em.el {\n    padding: 0;\n    padding-left: 0.2em;\n}\n\n.style-elements button.button-focus:focus {\n   outline: none;\n   box-shadow: 0 0 3px 3px rgba(155,203,255,1.0);\n   border-color: rgba(155,203,255,1.0);\n}\n\n.style-elements textarea:focus, .style-elements input:focus {\n   outline: none;\n   box-shadow: 0 0 2px 2px rgba(155,203,255,1.0);\n   border-color: rgba(155,203,255,1.0);\n}\n.style-elements input[type=\'checkbox\'] {\n    border-radius: 3px;\n}\n.style-elements input[type=\'radio\'] {\n    border-radius: 7px;\n}\n.style-elements input[type=\'radio\']:focus {\n    border-radius: 7px;\n    box-shadow: 0 0 4px 4px rgba(155,203,255,1.0);\n}\n\n.style-elements select.focus-override:focus, .style-elements input.focus-override:focus {\n    outline: none;\n    box-shadow: none;\n    border-color:transparent;\n}\n.style-elements input.focus-override:focus ~ .alt-icon {\n    box-shadow: 0 0 3px 3px rgba(155,203,255,1.0);\n    border-color: rgba(155,203,255,1.0);\n}\n.style-elements select.focus-override:focus ~ .alt-icon {\n    box-shadow: 0 0 3px 3px rgba(155,203,255,1.0);\n    border-color: rgba(155,203,255,1.0);\n}\n.style-elements .arrows {\n    display:block;\n    position: relative;\n    height: 10px;\n    width: 10px;\n}\n/*\n.style-elements .arrows::after {\n    content: " ";\n    position:absolute;\n    top:-2px;\n    left:0;\n    width: 0;\n    height: 0;\n    border-left: 5px solid transparent;\n    border-right: 5px solid transparent;\n    border-bottom: 5px solid black;\n}\n*/\n\n.style-elements .arrows::before {\n    content: " ";\n    position:absolute;\n    top:2px;\n    left:0;\n    width: 0;\n    height: 0;\n    border-left: 5px solid transparent;\n    border-right: 5px solid transparent;\n    border-top: 5px solid black;\n}\n\n\n';
var mdgriffith$style_elements$Element$Internal$Render$miniNormalize = 'html{-ms-text-size-adjust:100%;-webkit-text-size-adjust:100%;margin:0;padding:0;border:0}body{margin:0}.style-elements{display:block;position:relative;margin:0;padding:0;border:0;font-size:100%;font:inherit;box-sizing:border-box;line-height:1.2}.el{display:block;position:relative;margin:0;padding:0;border:0;border-style:solid;font-size:100%;font:inherit;box-sizing:border-box}em.el{font-style:italic}b.el,strong.el{font-weight:bolder}strike.el{text-decoration:line-through}u.el{text-decoration:underline}a.el{text-decoration:none;color:inherit}img.el{border-style:none}sub.el,sup.el{font-size:75%;line-height:0;position:relative;vertical-align:baseline}sub.el{bottom:-0.25em}sup.el{top:-0.5em}' + mdgriffith$style_elements$Element$Internal$Render$withFocus;
var mdgriffith$style_elements$Element$Internal$Render$embed = F2(
	function (full, stylesheet) {
		return _List_fromArray(
			[
				A3(
				elm$html$Html$node,
				'style',
				_List_Nil,
				_List_fromArray(
					[
						elm$html$Html$text(
						full ? ('html,body{width:100%;height:100%;}' + mdgriffith$style_elements$Element$Internal$Render$miniNormalize) : mdgriffith$style_elements$Element$Internal$Render$miniNormalize)
					])),
				A3(
				elm$html$Html$node,
				'style',
				_List_Nil,
				_List_fromArray(
					[
						elm$html$Html$text(stylesheet.css)
					]))
			]);
	});
var elm$core$List$isEmpty = function (xs) {
	if (!xs.b) {
		return true;
	} else {
		return false;
	}
};
var mdgriffith$style_elements$Element$Internal$Adjustments$tagIntermediates = false;
var mdgriffith$style_elements$Element$Internal$Adjustments$tag = function (str) {
	return mdgriffith$style_elements$Element$Internal$Adjustments$tagIntermediates ? mdgriffith$style_elements$Element$Internal$Model$Attr(
		elm$html$Html$Attributes$class(str)) : mdgriffith$style_elements$Element$Internal$Model$Attr(
		elm$html$Html$Attributes$class(''));
};
var mdgriffith$style_elements$Element$Internal$Model$Center = {$: 'Center'};
var mdgriffith$style_elements$Element$Internal$Adjustments$centerTextLayout = function (elm) {
	if (elm.$ === 'Layout') {
		var layoutEl = elm.a;
		var attrs = layoutEl.attrs;
		var layout = layoutEl.layout;
		var _n1 = A2(
			elm$core$List$partition,
			function (attr) {
				return _Utils_eq(
					attr,
					mdgriffith$style_elements$Element$Internal$Model$HAlign(mdgriffith$style_elements$Element$Internal$Model$Center)) || _Utils_eq(
					attr,
					mdgriffith$style_elements$Element$Internal$Model$VAlign(mdgriffith$style_elements$Element$Internal$Model$VerticalCenter));
			},
			attrs);
		var centeredProps = _n1.a;
		var others = _n1.b;
		if (layout.$ === 'TextLayout') {
			return (!elm$core$List$isEmpty(centeredProps)) ? mdgriffith$style_elements$Element$Internal$Model$Layout(
				{
					absolutelyPositioned: elm$core$Maybe$Nothing,
					attrs: A2(
						elm$core$List$cons,
						mdgriffith$style_elements$Element$Internal$Adjustments$tag('center-text'),
						A2(
							elm$core$List$cons,
							mdgriffith$style_elements$Element$Internal$Model$PointerEvents(false),
							centeredProps)),
					children: mdgriffith$style_elements$Element$Internal$Model$Normal(
						_List_fromArray(
							[
								mdgriffith$style_elements$Element$Internal$Model$Layout(
								_Utils_update(
									layoutEl,
									{
										attrs: A2(
											elm$core$List$cons,
											mdgriffith$style_elements$Element$Internal$Model$PointerEvents(true),
											others)
									}))
							])),
					layout: A2(mdgriffith$style_elements$Style$Internal$Model$FlexLayout, mdgriffith$style_elements$Style$Internal$Model$GoRight, _List_Nil),
					node: 'div',
					style: elm$core$Maybe$Nothing
				}) : elm;
		} else {
			return elm;
		}
	} else {
		return elm;
	}
};
var mdgriffith$style_elements$Element$Internal$Model$Screen = {$: 'Screen'};
var mdgriffith$style_elements$Element$Internal$Adjustments$hoistFixedScreenElements = function (el) {
	var elementIsOnScreen = function (attrs) {
		return A2(
			elm$core$List$any,
			function (attr) {
				return _Utils_eq(
					attr,
					mdgriffith$style_elements$Element$Internal$Model$PositionFrame(mdgriffith$style_elements$Element$Internal$Model$Screen));
			},
			attrs);
	};
	switch (el.$) {
		case 'Element':
			var attrs = el.a.attrs;
			return elementIsOnScreen(attrs) ? _Utils_Tuple2(
				mdgriffith$style_elements$Element$Internal$Model$Empty,
				elm$core$Maybe$Just(
					_List_fromArray(
						[el]))) : _Utils_Tuple2(el, elm$core$Maybe$Nothing);
		case 'Layout':
			var attrs = el.a.attrs;
			return elementIsOnScreen(attrs) ? _Utils_Tuple2(
				mdgriffith$style_elements$Element$Internal$Model$Empty,
				elm$core$Maybe$Just(
					_List_fromArray(
						[el]))) : _Utils_Tuple2(el, elm$core$Maybe$Nothing);
		default:
			return _Utils_Tuple2(el, elm$core$Maybe$Nothing);
	}
};
var elm$core$List$maybeCons = F3(
	function (f, mx, xs) {
		var _n0 = f(mx);
		if (_n0.$ === 'Just') {
			var x = _n0.a;
			return A2(elm$core$List$cons, x, xs);
		} else {
			return xs;
		}
	});
var elm$core$List$filterMap = F2(
	function (f, xs) {
		return A3(
			elm$core$List$foldr,
			elm$core$List$maybeCons(f),
			_List_Nil,
			xs);
	});
var elm$core$Basics$negate = function (n) {
	return -n;
};
var mdgriffith$style_elements$Style$Internal$Model$Calc = F2(
	function (a, b) {
		return {$: 'Calc', a: a, b: b};
	});
var mdgriffith$style_elements$Element$Internal$Adjustments$counterSpacing = function (elm) {
	if (elm.$ === 'Layout') {
		var layoutEl = elm.a;
		var node = layoutEl.node;
		var layout = layoutEl.layout;
		var style = layoutEl.style;
		var attrs = layoutEl.attrs;
		var children = layoutEl.children;
		var absolutelyPositioned = layoutEl.absolutelyPositioned;
		var forSpacing = function (posAttr) {
			if (posAttr.$ === 'Spacing') {
				var x = posAttr.a;
				var y = posAttr.b;
				return elm$core$Maybe$Just(
					_Utils_Tuple2(x, y));
			} else {
				return elm$core$Maybe$Nothing;
			}
		};
		var spacing = elm$core$List$head(
			elm$core$List$reverse(
				A2(elm$core$List$filterMap, forSpacing, attrs)));
		var hasSpacing = function () {
			if (spacing.$ === 'Nothing') {
				return false;
			} else {
				return true;
			}
		}();
		var forPhantomPadding = function (posAttr) {
			if (posAttr.$ === 'Padding') {
				var t = posAttr.a;
				var r = posAttr.b;
				var b = posAttr.c;
				var l = posAttr.d;
				return elm$core$Maybe$Just(
					A4(
						mdgriffith$style_elements$Element$Internal$Model$PhantomPadding,
						A2(elm$core$Maybe$withDefault, 0, t),
						A2(elm$core$Maybe$withDefault, 0, r),
						A2(elm$core$Maybe$withDefault, 0, b),
						A2(elm$core$Maybe$withDefault, 0, l)));
			} else {
				return elm$core$Maybe$Nothing;
			}
		};
		var phantomPadding = A2(
			elm$core$Maybe$withDefault,
			A4(mdgriffith$style_elements$Element$Internal$Model$PhantomPadding, 0, 0, 0, 0),
			elm$core$List$head(
				elm$core$List$reverse(
					A2(elm$core$List$filterMap, forPhantomPadding, attrs))));
		var _n1 = A2(
			elm$core$List$partition,
			function (attr) {
				return _Utils_eq(
					attr,
					mdgriffith$style_elements$Element$Internal$Model$HAlign(mdgriffith$style_elements$Element$Internal$Model$Center)) || _Utils_eq(
					attr,
					mdgriffith$style_elements$Element$Internal$Model$VAlign(mdgriffith$style_elements$Element$Internal$Model$VerticalCenter));
			},
			attrs);
		var centeredProps = _n1.a;
		var others = _n1.b;
		if (layout.$ === 'FlexLayout') {
			if (hasSpacing) {
				var forAlignment = function (attr) {
					switch (attr.$) {
						case 'HAlign':
							return true;
						case 'VAlign':
							return true;
						default:
							return false;
					}
				};
				var _n3 = A2(elm$core$List$partition, forAlignment, attrs);
				var aligned = _n3.a;
				var unaligned = _n3.b;
				var _n4 = function () {
					if (spacing.$ === 'Nothing') {
						return _Utils_Tuple3(
							A4(mdgriffith$style_elements$Element$Internal$Model$Margin, 0, 0, 0, 0),
							A2(mdgriffith$style_elements$Element$Internal$Model$Spacing, 0, 0),
							0);
					} else {
						var _n6 = spacing.a;
						var x = _n6.a;
						var y = _n6.b;
						return _Utils_Tuple3(
							A4(mdgriffith$style_elements$Element$Internal$Model$Margin, (-1) * y, (-1) * x, (-1) * y, (-1) * x),
							A2(mdgriffith$style_elements$Element$Internal$Model$Spacing, x, y),
							x);
					}
				}();
				var negativeMargin = _n4.a;
				var spacingAttr = _n4.b;
				var totalHSpacing = _n4.c;
				return mdgriffith$style_elements$Element$Internal$Model$Layout(
					{
						absolutelyPositioned: absolutelyPositioned,
						attrs: A2(
							elm$core$List$cons,
							mdgriffith$style_elements$Element$Internal$Adjustments$tag('counter-spacing-container'),
							A2(
								elm$core$List$cons,
								mdgriffith$style_elements$Element$Internal$Model$PointerEvents(true),
								unaligned)),
						children: mdgriffith$style_elements$Element$Internal$Model$Normal(
							_List_fromArray(
								[
									mdgriffith$style_elements$Element$Internal$Model$Layout(
									{
										absolutelyPositioned: elm$core$Maybe$Nothing,
										attrs: A2(
											elm$core$List$cons,
											mdgriffith$style_elements$Element$Internal$Adjustments$tag('counter-spacing'),
											A2(
												elm$core$List$cons,
												mdgriffith$style_elements$Element$Internal$Model$PointerEvents(false),
												A2(
													elm$core$List$cons,
													phantomPadding,
													A2(
														elm$core$List$cons,
														negativeMargin,
														A2(
															elm$core$List$cons,
															spacingAttr,
															A2(
																elm$core$List$cons,
																mdgriffith$style_elements$Element$Internal$Model$Width(
																	A2(mdgriffith$style_elements$Style$Internal$Model$Calc, 100, totalHSpacing)),
																A2(
																	elm$core$List$cons,
																	mdgriffith$style_elements$Element$Internal$Model$Shrink(1),
																	aligned))))))),
										children: function () {
											if (children.$ === 'Normal') {
												var childs = children.a;
												return mdgriffith$style_elements$Element$Internal$Model$Normal(
													A2(
														elm$core$List$map,
														mdgriffith$style_elements$Element$Internal$Modify$addAttr(
															mdgriffith$style_elements$Element$Internal$Model$PointerEvents(true)),
														childs));
											} else {
												var childs = children.a;
												return mdgriffith$style_elements$Element$Internal$Model$Keyed(
													A2(
														elm$core$List$map,
														elm$core$Tuple$mapSecond(
															mdgriffith$style_elements$Element$Internal$Modify$addAttr(
																mdgriffith$style_elements$Element$Internal$Model$PointerEvents(true))),
														childs));
											}
										}(),
										layout: layout,
										node: 'div',
										style: elm$core$Maybe$Nothing
									})
								])),
						layout: A2(mdgriffith$style_elements$Style$Internal$Model$FlexLayout, mdgriffith$style_elements$Style$Internal$Model$GoRight, _List_Nil),
						node: node,
						style: style
					});
			} else {
				return mdgriffith$style_elements$Element$Internal$Model$Layout(
					_Utils_update(
						layoutEl,
						{
							attrs: A2(
								elm$core$List$cons,
								mdgriffith$style_elements$Element$Internal$Model$PointerEvents(true),
								attrs)
						}));
			}
		} else {
			return elm;
		}
	} else {
		return elm;
	}
};
var mdgriffith$style_elements$Element$Internal$Model$Above = {$: 'Above'};
var mdgriffith$style_elements$Element$Internal$Model$Absolute = function (a) {
	return {$: 'Absolute', a: a};
};
var mdgriffith$style_elements$Element$Internal$Model$Below = {$: 'Below'};
var mdgriffith$style_elements$Element$Internal$Model$Bottom = {$: 'Bottom'};
var mdgriffith$style_elements$Element$Internal$Model$BottomLeft = {$: 'BottomLeft'};
var mdgriffith$style_elements$Element$Internal$Model$Left = {$: 'Left'};
var mdgriffith$style_elements$Element$Internal$Model$Relative = {$: 'Relative'};
var mdgriffith$style_elements$Element$Internal$Model$Right = {$: 'Right'};
var mdgriffith$style_elements$Element$Internal$Model$Top = {$: 'Top'};
var mdgriffith$style_elements$Element$Internal$Model$TopLeft = {$: 'TopLeft'};
var mdgriffith$style_elements$Element$Internal$Modify$addAttrList = F2(
	function (props, el) {
		switch (el.$) {
			case 'Empty':
				return mdgriffith$style_elements$Element$Internal$Model$Empty;
			case 'Spacer':
				var x = el.a;
				return mdgriffith$style_elements$Element$Internal$Model$Spacer(x);
			case 'Raw':
				var h = el.a;
				return mdgriffith$style_elements$Element$Internal$Model$Raw(h);
			case 'Layout':
				var elm = el.a;
				return mdgriffith$style_elements$Element$Internal$Model$Layout(
					_Utils_update(
						elm,
						{
							attrs: _Utils_ap(props, elm.attrs)
						}));
			case 'Element':
				var elm = el.a;
				return mdgriffith$style_elements$Element$Internal$Model$Element(
					_Utils_update(
						elm,
						{
							attrs: _Utils_ap(props, elm.attrs)
						}));
			default:
				var dec = el.a;
				var content = el.b;
				return mdgriffith$style_elements$Element$Internal$Model$Element(
					{
						absolutelyPositioned: elm$core$Maybe$Nothing,
						attrs: props,
						child: A2(mdgriffith$style_elements$Element$Internal$Model$Text, dec, content),
						node: 'div',
						style: elm$core$Maybe$Nothing
					});
		}
	});
var mdgriffith$style_elements$Element$Internal$Modify$addAttrPriority = F2(
	function (prop, el) {
		switch (el.$) {
			case 'Empty':
				return mdgriffith$style_elements$Element$Internal$Model$Empty;
			case 'Raw':
				var h = el.a;
				return mdgriffith$style_elements$Element$Internal$Model$Raw(h);
			case 'Spacer':
				var x = el.a;
				return mdgriffith$style_elements$Element$Internal$Model$Spacer(x);
			case 'Layout':
				var elm = el.a;
				return mdgriffith$style_elements$Element$Internal$Model$Layout(
					_Utils_update(
						elm,
						{
							attrs: _Utils_ap(
								elm.attrs,
								_List_fromArray(
									[prop]))
						}));
			case 'Element':
				var elm = el.a;
				return mdgriffith$style_elements$Element$Internal$Model$Element(
					_Utils_update(
						elm,
						{
							attrs: _Utils_ap(
								elm.attrs,
								_List_fromArray(
									[prop]))
						}));
			default:
				var dec = el.a;
				var content = el.b;
				return mdgriffith$style_elements$Element$Internal$Model$Element(
					{
						absolutelyPositioned: elm$core$Maybe$Nothing,
						attrs: _List_fromArray(
							[prop]),
						child: A2(mdgriffith$style_elements$Element$Internal$Model$Text, dec, content),
						node: 'div',
						style: elm$core$Maybe$Nothing
					});
		}
	});
var mdgriffith$style_elements$Element$Internal$Modify$setAttrs = F2(
	function (props, el) {
		switch (el.$) {
			case 'Empty':
				return mdgriffith$style_elements$Element$Internal$Model$Empty;
			case 'Spacer':
				var x = el.a;
				return mdgriffith$style_elements$Element$Internal$Model$Spacer(x);
			case 'Raw':
				var h = el.a;
				return mdgriffith$style_elements$Element$Internal$Model$Raw(h);
			case 'Layout':
				var elm = el.a;
				return mdgriffith$style_elements$Element$Internal$Model$Layout(
					_Utils_update(
						elm,
						{attrs: props}));
			case 'Element':
				var elm = el.a;
				return mdgriffith$style_elements$Element$Internal$Model$Element(
					_Utils_update(
						elm,
						{attrs: props}));
			default:
				var dec = el.a;
				var content = el.b;
				return A2(mdgriffith$style_elements$Element$Internal$Model$Text, dec, content);
		}
	});
var mdgriffith$style_elements$Element$Internal$Adjustments$positionNearby = F2(
	function (parent, elm) {
		var setPosition = F3(
			function (nearbyPosition, _n24, el) {
				var aligned = _n24.a;
				var unaligned = _n24.b;
				var nearbyAlignment = function () {
					_n19$4:
					while (true) {
						if ((nearbyPosition.$ === 'Just') && (nearbyPosition.a.$ === 'Nearby')) {
							switch (nearbyPosition.a.a.$) {
								case 'Above':
									var _n20 = nearbyPosition.a.a;
									return _List_fromArray(
										[
											mdgriffith$style_elements$Element$Internal$Model$VAlign(mdgriffith$style_elements$Element$Internal$Model$Top)
										]);
								case 'Below':
									var _n21 = nearbyPosition.a.a;
									return _List_fromArray(
										[
											mdgriffith$style_elements$Element$Internal$Model$VAlign(mdgriffith$style_elements$Element$Internal$Model$Bottom)
										]);
								case 'OnRight':
									var _n22 = nearbyPosition.a.a;
									return _List_fromArray(
										[
											mdgriffith$style_elements$Element$Internal$Model$HAlign(mdgriffith$style_elements$Element$Internal$Model$Right)
										]);
								case 'OnLeft':
									var _n23 = nearbyPosition.a.a;
									return _List_fromArray(
										[
											mdgriffith$style_elements$Element$Internal$Model$HAlign(mdgriffith$style_elements$Element$Internal$Model$Left)
										]);
								default:
									break _n19$4;
							}
						} else {
							break _n19$4;
						}
					}
					return _List_Nil;
				}();
				var isLayout = function () {
					if (elm.$ === 'Layout') {
						return true;
					} else {
						return false;
					}
				}();
				var framed = function () {
					if (nearbyPosition.$ === 'Nothing') {
						return false;
					} else {
						return true;
					}
				}();
				var forWidth = function (prop) {
					if (prop.$ === 'Width') {
						return true;
					} else {
						return false;
					}
				};
				var width = elm$core$List$head(
					elm$core$List$reverse(
						A2(elm$core$List$filter, forWidth, unaligned)));
				var forHeight = function (prop) {
					if (prop.$ === 'Height') {
						return true;
					} else {
						return false;
					}
				};
				var height = elm$core$List$head(
					elm$core$List$reverse(
						A2(elm$core$List$filter, forHeight, unaligned)));
				var adjustWidthHeight = function (elem) {
					var adjustWidth = function (element) {
						if (width.$ === 'Nothing') {
							return element;
						} else {
							if ((width.a.$ === 'Width') && (width.a.a.$ === 'Percent')) {
								var percent = width.a.a.a;
								return A2(
									mdgriffith$style_elements$Element$Internal$Modify$addAttrPriority,
									mdgriffith$style_elements$Element$Internal$Model$Width(
										mdgriffith$style_elements$Style$Internal$Model$Percent(100)),
									element);
							} else {
								var x = width.a;
								return element;
							}
						}
					};
					var adjustHeight = function (element) {
						if (height.$ === 'Nothing') {
							return element;
						} else {
							if ((height.a.$ === 'Height') && (height.a.a.$ === 'Percent')) {
								var percent = height.a.a.a;
								return A2(
									mdgriffith$style_elements$Element$Internal$Modify$addAttrPriority,
									mdgriffith$style_elements$Element$Internal$Model$Width(
										mdgriffith$style_elements$Style$Internal$Model$Percent(100)),
									element);
							} else {
								var x = height.a;
								return element;
							}
						}
					};
					return adjustHeight(
						adjustWidth(elem));
				};
				var addWidthHeight = function (attrs) {
					var _n8 = _Utils_Tuple2(width, height);
					if (_n8.a.$ === 'Nothing') {
						if (_n8.b.$ === 'Nothing') {
							var _n9 = _n8.a;
							var _n10 = _n8.b;
							return attrs;
						} else {
							var _n11 = _n8.a;
							var h = _n8.b.a;
							return A2(elm$core$List$cons, h, attrs);
						}
					} else {
						if (_n8.b.$ === 'Just') {
							var w = _n8.a.a;
							var h = _n8.b.a;
							return A2(
								elm$core$List$cons,
								w,
								A2(elm$core$List$cons, h, attrs));
						} else {
							var w = _n8.a.a;
							var _n12 = _n8.b;
							return A2(elm$core$List$cons, w, attrs);
						}
					}
				};
				return (_Utils_eq(
					nearbyPosition,
					elm$core$Maybe$Just(
						mdgriffith$style_elements$Element$Internal$Model$Nearby(mdgriffith$style_elements$Element$Internal$Model$Above))) || _Utils_eq(
					nearbyPosition,
					elm$core$Maybe$Just(
						mdgriffith$style_elements$Element$Internal$Model$Nearby(mdgriffith$style_elements$Element$Internal$Model$Below)))) ? mdgriffith$style_elements$Element$Internal$Model$Layout(
					{
						absolutelyPositioned: elm$core$Maybe$Nothing,
						attrs: A2(
							elm$core$List$cons,
							mdgriffith$style_elements$Element$Internal$Adjustments$tag('above-below-intermediate-parent'),
							A2(
								elm$core$List$cons,
								mdgriffith$style_elements$Element$Internal$Model$PointerEvents(false),
								A2(
									elm$core$List$cons,
									mdgriffith$style_elements$Element$Internal$Model$Height(
										mdgriffith$style_elements$Style$Internal$Model$Px(0)),
									A2(
										elm$core$List$cons,
										mdgriffith$style_elements$Element$Internal$Model$Width(
											mdgriffith$style_elements$Style$Internal$Model$Percent(100)),
										A2(
											elm$core$List$cons,
											mdgriffith$style_elements$Element$Internal$Model$PositionFrame(
												mdgriffith$style_elements$Element$Internal$Model$Absolute(
													_Utils_eq(
														nearbyPosition,
														elm$core$Maybe$Just(
															mdgriffith$style_elements$Element$Internal$Model$Nearby(mdgriffith$style_elements$Element$Internal$Model$Above))) ? mdgriffith$style_elements$Element$Internal$Model$TopLeft : mdgriffith$style_elements$Element$Internal$Model$BottomLeft)),
											A2(
												elm$core$List$cons,
												A3(
													mdgriffith$style_elements$Element$Internal$Model$Position,
													elm$core$Maybe$Just(0),
													elm$core$Maybe$Just(0),
													elm$core$Maybe$Nothing),
												isLayout ? nearbyAlignment : _Utils_ap(nearbyAlignment, aligned))))))),
						children: mdgriffith$style_elements$Element$Internal$Model$Normal(
							_List_fromArray(
								[
									mdgriffith$style_elements$Element$Internal$Model$Element(
									{
										absolutelyPositioned: elm$core$Maybe$Nothing,
										attrs: function () {
											var addWidth = function (attrs) {
												return isLayout ? A2(
													elm$core$List$cons,
													mdgriffith$style_elements$Element$Internal$Model$Width(
														mdgriffith$style_elements$Style$Internal$Model$Percent(100)),
													attrs) : attrs;
											};
											return addWidth(
												_List_fromArray(
													[
														mdgriffith$style_elements$Element$Internal$Adjustments$tag('above-below-intermediate'),
														mdgriffith$style_elements$Element$Internal$Model$PointerEvents(false),
														mdgriffith$style_elements$Element$Internal$Model$PositionFrame(
														mdgriffith$style_elements$Element$Internal$Model$Absolute(
															_Utils_eq(
																nearbyPosition,
																elm$core$Maybe$Just(
																	mdgriffith$style_elements$Element$Internal$Model$Nearby(mdgriffith$style_elements$Element$Internal$Model$Above))) ? mdgriffith$style_elements$Element$Internal$Model$BottomLeft : mdgriffith$style_elements$Element$Internal$Model$TopLeft)),
														A3(
														mdgriffith$style_elements$Element$Internal$Model$Position,
														elm$core$Maybe$Nothing,
														elm$core$Maybe$Just(0),
														elm$core$Maybe$Nothing),
														mdgriffith$style_elements$Element$Internal$Model$VAlign(mdgriffith$style_elements$Element$Internal$Model$Bottom),
														mdgriffith$style_elements$Element$Internal$Model$Attr(
														A2(elm$html$Html$Attributes$style, 'z-index', '10'))
													]));
										}(),
										child: mdgriffith$style_elements$Element$Internal$Adjustments$counterSpacing(
											A2(
												mdgriffith$style_elements$Element$Internal$Modify$setAttrs,
												A2(
													elm$core$List$cons,
													mdgriffith$style_elements$Element$Internal$Model$PointerEvents(true),
													A2(
														elm$core$List$cons,
														mdgriffith$style_elements$Element$Internal$Model$PositionFrame(
															mdgriffith$style_elements$Element$Internal$Model$Absolute(mdgriffith$style_elements$Element$Internal$Model$TopLeft)),
														A2(
															elm$core$List$cons,
															A3(
																mdgriffith$style_elements$Element$Internal$Model$Position,
																elm$core$Maybe$Just(0),
																elm$core$Maybe$Just(0),
																elm$core$Maybe$Nothing),
															unaligned))),
												el)),
										node: 'div',
										style: elm$core$Maybe$Nothing
									})
								])),
						layout: A2(mdgriffith$style_elements$Style$Internal$Model$FlexLayout, mdgriffith$style_elements$Style$Internal$Model$GoRight, _List_Nil),
						node: 'div',
						style: elm$core$Maybe$Nothing
					}) : (framed ? mdgriffith$style_elements$Element$Internal$Model$Layout(
					{
						absolutelyPositioned: elm$core$Maybe$Nothing,
						attrs: A2(
							elm$core$List$cons,
							mdgriffith$style_elements$Element$Internal$Adjustments$tag('nearby-intermediate-parent'),
							A2(
								elm$core$List$cons,
								mdgriffith$style_elements$Element$Internal$Model$PointerEvents(false),
								A2(
									elm$core$List$cons,
									mdgriffith$style_elements$Element$Internal$Model$Height(
										mdgriffith$style_elements$Style$Internal$Model$Percent(100)),
									A2(
										elm$core$List$cons,
										mdgriffith$style_elements$Element$Internal$Model$Width(
											mdgriffith$style_elements$Style$Internal$Model$Percent(100)),
										A2(
											elm$core$List$cons,
											mdgriffith$style_elements$Element$Internal$Model$PositionFrame(
												mdgriffith$style_elements$Element$Internal$Model$Absolute(mdgriffith$style_elements$Element$Internal$Model$TopLeft)),
											A2(
												elm$core$List$cons,
												A3(
													mdgriffith$style_elements$Element$Internal$Model$Position,
													elm$core$Maybe$Just(0),
													elm$core$Maybe$Just(0),
													elm$core$Maybe$Nothing),
												isLayout ? nearbyAlignment : _Utils_ap(nearbyAlignment, aligned))))))),
						children: mdgriffith$style_elements$Element$Internal$Model$Normal(
							_List_fromArray(
								[
									mdgriffith$style_elements$Element$Internal$Model$Element(
									{
										absolutelyPositioned: elm$core$Maybe$Nothing,
										attrs: addWidthHeight(
											_List_fromArray(
												[
													mdgriffith$style_elements$Element$Internal$Model$PointerEvents(false),
													mdgriffith$style_elements$Element$Internal$Model$PositionFrame(mdgriffith$style_elements$Element$Internal$Model$Relative),
													A3(
													mdgriffith$style_elements$Element$Internal$Model$Position,
													elm$core$Maybe$Just(0),
													elm$core$Maybe$Just(0),
													elm$core$Maybe$Nothing),
													A4(
													mdgriffith$style_elements$Element$Internal$Model$Padding,
													elm$core$Maybe$Just(0),
													elm$core$Maybe$Just(0),
													elm$core$Maybe$Just(0),
													elm$core$Maybe$Just(0)),
													mdgriffith$style_elements$Element$Internal$Model$Attr(
													A2(elm$html$Html$Attributes$style, 'z-index', '10')),
													mdgriffith$style_elements$Element$Internal$Adjustments$tag('nearby-intermediate')
												])),
										child: mdgriffith$style_elements$Element$Internal$Adjustments$counterSpacing(
											adjustWidthHeight(
												A2(
													mdgriffith$style_elements$Element$Internal$Modify$addAttrList,
													A2(
														elm$core$List$cons,
														mdgriffith$style_elements$Element$Internal$Model$PointerEvents(true),
														A2(
															elm$core$List$cons,
															mdgriffith$style_elements$Element$Internal$Model$PositionFrame(
																mdgriffith$style_elements$Element$Internal$Model$Absolute(mdgriffith$style_elements$Element$Internal$Model$TopLeft)),
															A2(
																elm$core$List$cons,
																A3(
																	mdgriffith$style_elements$Element$Internal$Model$Position,
																	elm$core$Maybe$Just(0),
																	elm$core$Maybe$Just(0),
																	elm$core$Maybe$Nothing),
																_List_Nil))),
													el))),
										node: 'div',
										style: elm$core$Maybe$Nothing
									})
								])),
						layout: A2(mdgriffith$style_elements$Style$Internal$Model$FlexLayout, mdgriffith$style_elements$Style$Internal$Model$GoRight, _List_Nil),
						node: 'div',
						style: elm$core$Maybe$Nothing
					}) : ((!elm$core$List$isEmpty(aligned)) ? mdgriffith$style_elements$Element$Internal$Model$Layout(
					{
						absolutelyPositioned: elm$core$Maybe$Nothing,
						attrs: A2(
							elm$core$List$cons,
							mdgriffith$style_elements$Element$Internal$Adjustments$tag('nearby-aligned-intermediate-parent'),
							A2(
								elm$core$List$cons,
								mdgriffith$style_elements$Element$Internal$Model$PointerEvents(false),
								A2(
									elm$core$List$cons,
									mdgriffith$style_elements$Element$Internal$Model$Height(
										mdgriffith$style_elements$Style$Internal$Model$Percent(100)),
									A2(
										elm$core$List$cons,
										mdgriffith$style_elements$Element$Internal$Model$Width(
											mdgriffith$style_elements$Style$Internal$Model$Percent(100)),
										A2(
											elm$core$List$cons,
											mdgriffith$style_elements$Element$Internal$Model$PositionFrame(mdgriffith$style_elements$Element$Internal$Model$Relative),
											A2(
												elm$core$List$cons,
												A3(
													mdgriffith$style_elements$Element$Internal$Model$Position,
													elm$core$Maybe$Just(0),
													elm$core$Maybe$Just(0),
													elm$core$Maybe$Nothing),
												isLayout ? nearbyAlignment : _Utils_ap(nearbyAlignment, aligned))))))),
						children: mdgriffith$style_elements$Element$Internal$Model$Normal(
							_List_fromArray(
								[
									mdgriffith$style_elements$Element$Internal$Model$Element(
									{
										absolutelyPositioned: elm$core$Maybe$Nothing,
										attrs: addWidthHeight(
											_List_fromArray(
												[
													mdgriffith$style_elements$Element$Internal$Model$PointerEvents(false),
													mdgriffith$style_elements$Element$Internal$Model$PositionFrame(mdgriffith$style_elements$Element$Internal$Model$Relative),
													A3(
													mdgriffith$style_elements$Element$Internal$Model$Position,
													elm$core$Maybe$Just(0),
													elm$core$Maybe$Just(0),
													elm$core$Maybe$Nothing),
													A4(
													mdgriffith$style_elements$Element$Internal$Model$Padding,
													elm$core$Maybe$Just(0),
													elm$core$Maybe$Just(0),
													elm$core$Maybe$Just(0),
													elm$core$Maybe$Just(0)),
													mdgriffith$style_elements$Element$Internal$Adjustments$tag('nearby-aligned-intermediate')
												])),
										child: mdgriffith$style_elements$Element$Internal$Adjustments$counterSpacing(
											adjustWidthHeight(
												A2(
													mdgriffith$style_elements$Element$Internal$Modify$addAttrList,
													A2(
														elm$core$List$cons,
														mdgriffith$style_elements$Element$Internal$Model$PointerEvents(true),
														A2(
															elm$core$List$cons,
															mdgriffith$style_elements$Element$Internal$Model$PositionFrame(mdgriffith$style_elements$Element$Internal$Model$Relative),
															A2(
																elm$core$List$cons,
																A3(
																	mdgriffith$style_elements$Element$Internal$Model$Position,
																	elm$core$Maybe$Just(0),
																	elm$core$Maybe$Just(0),
																	elm$core$Maybe$Nothing),
																_List_Nil))),
													el))),
										node: 'div',
										style: elm$core$Maybe$Nothing
									})
								])),
						layout: A2(mdgriffith$style_elements$Style$Internal$Model$FlexLayout, mdgriffith$style_elements$Style$Internal$Model$GoRight, _List_Nil),
						node: 'div',
						style: elm$core$Maybe$Nothing
					}) : mdgriffith$style_elements$Element$Internal$Adjustments$counterSpacing(elm)));
			});
		var forAlignment = function (attr) {
			switch (attr.$) {
				case 'HAlign':
					return true;
				case 'VAlign':
					return true;
				default:
					return false;
			}
		};
		var separateAlignment = function (attrs) {
			return A2(elm$core$List$partition, forAlignment, attrs);
		};
		switch (elm.$) {
			case 'Element':
				var attrs = elm.a.attrs;
				var isFrame = function (attr) {
					if (attr.$ === 'PositionFrame') {
						var x = attr.a;
						return elm$core$Maybe$Just(x);
					} else {
						return elm$core$Maybe$Nothing;
					}
				};
				var frame = elm$core$List$head(
					A2(elm$core$List$filterMap, isFrame, attrs));
				var _n1 = separateAlignment(attrs);
				var aligned = _n1.a;
				var unaligned = _n1.b;
				if (parent.$ === 'Nothing') {
					return A3(
						setPosition,
						frame,
						_Utils_Tuple2(aligned, unaligned),
						elm);
				} else {
					return elm;
				}
			case 'Layout':
				var attrs = elm.a.attrs;
				var isFrame = function (attr) {
					if (attr.$ === 'PositionFrame') {
						var x = attr.a;
						return elm$core$Maybe$Just(x);
					} else {
						return elm$core$Maybe$Nothing;
					}
				};
				var frame = elm$core$List$head(
					A2(elm$core$List$filterMap, isFrame, attrs));
				var _n4 = separateAlignment(attrs);
				var aligned = _n4.a;
				var unaligned = _n4.b;
				if (parent.$ === 'Nothing') {
					return A3(
						setPosition,
						frame,
						_Utils_Tuple2(aligned, unaligned),
						elm);
				} else {
					return mdgriffith$style_elements$Element$Internal$Adjustments$counterSpacing(elm);
				}
			default:
				return mdgriffith$style_elements$Element$Internal$Adjustments$counterSpacing(elm);
		}
	});
var mdgriffith$style_elements$Element$Internal$Model$adjust = F3(
	function (fn, parent, elemen) {
		var merge = F2(
			function (el, current) {
				if (el.$ === 'Nothing') {
					return current;
				} else {
					var something = el.a;
					if (current.$ === 'Nothing') {
						return el;
					} else {
						var cur = current.a;
						return elm$core$Maybe$Just(
							_Utils_ap(something, cur));
					}
				}
			});
		var maybeOnEmptyList = function (list) {
			return elm$core$List$isEmpty(list) ? elm$core$Maybe$Nothing : elm$core$Maybe$Just(list);
		};
		switch (elemen.$) {
			case 'Element':
				var elm = elemen.a;
				var child = elm.child;
				var absolutelyPositioned = elm.absolutelyPositioned;
				var adjustAndMerge = F2(
					function (el, _n8) {
						var adjustedAggregate = _n8.a;
						var dataAggregate = _n8.b;
						var _n6 = A3(mdgriffith$style_elements$Element$Internal$Model$adjust, fn, elm$core$Maybe$Nothing, el);
						var adjusted = _n6.a;
						var data = _n6.b;
						if (data.$ === 'Nothing') {
							return _Utils_Tuple2(
								A2(elm$core$List$cons, adjusted, adjustedAggregate),
								dataAggregate);
						} else {
							var d = data.a;
							return _Utils_Tuple2(
								A2(elm$core$List$cons, adjusted, adjustedAggregate),
								_Utils_ap(d, dataAggregate));
						}
					});
				var _n1 = function () {
					if (absolutelyPositioned.$ === 'Nothing') {
						return _Utils_Tuple2(elm$core$Maybe$Nothing, elm$core$Maybe$Nothing);
					} else {
						var others = absolutelyPositioned.a;
						return function (_n3) {
							var children = _n3.a;
							var onScreen = _n3.b;
							return _Utils_Tuple2(
								maybeOnEmptyList(children),
								maybeOnEmptyList(onScreen));
						}(
							A3(
								elm$core$List$foldr,
								adjustAndMerge,
								_Utils_Tuple2(_List_Nil, _List_Nil),
								others));
					}
				}();
				var adjustedOthers = _n1.a;
				var otherChildrenData = _n1.b;
				var _n4 = A3(mdgriffith$style_elements$Element$Internal$Model$adjust, fn, elm$core$Maybe$Nothing, child);
				var adjustedChild = _n4.a;
				var childData = _n4.b;
				var _n5 = A2(
					fn,
					parent,
					mdgriffith$style_elements$Element$Internal$Model$Element(
						_Utils_update(
							elm,
							{absolutelyPositioned: adjustedOthers, child: adjustedChild})));
				var adjustedEl = _n5.a;
				var elData = _n5.b;
				return _Utils_Tuple2(
					adjustedEl,
					A3(
						elm$core$List$foldr,
						merge,
						elm$core$Maybe$Nothing,
						_List_fromArray(
							[childData, otherChildrenData, elData])));
			case 'Layout':
				var elm = elemen.a;
				var adjustAndMergeKeyed = F3(
					function (usingParent, _n22, _n23) {
						var key = _n22.a;
						var el = _n22.b;
						var adjustedAggregate = _n23.a;
						var dataAggregate = _n23.b;
						var _n20 = A3(mdgriffith$style_elements$Element$Internal$Model$adjust, fn, usingParent, el);
						var adjusted = _n20.a;
						var data = _n20.b;
						if (data.$ === 'Nothing') {
							return _Utils_Tuple2(
								A2(
									elm$core$List$cons,
									_Utils_Tuple2(key, adjusted),
									adjustedAggregate),
								dataAggregate);
						} else {
							var d = data.a;
							return _Utils_Tuple2(
								A2(
									elm$core$List$cons,
									_Utils_Tuple2(key, adjusted),
									adjustedAggregate),
								_Utils_ap(d, dataAggregate));
						}
					});
				var adjustAndMerge = F3(
					function (usingParent, el, _n19) {
						var adjustedAggregate = _n19.a;
						var dataAggregate = _n19.b;
						var _n17 = A3(mdgriffith$style_elements$Element$Internal$Model$adjust, fn, usingParent, el);
						var adjusted = _n17.a;
						var data = _n17.b;
						if (data.$ === 'Nothing') {
							return _Utils_Tuple2(
								A2(elm$core$List$cons, adjusted, adjustedAggregate),
								dataAggregate);
						} else {
							var d = data.a;
							return _Utils_Tuple2(
								A2(elm$core$List$cons, adjusted, adjustedAggregate),
								_Utils_ap(d, dataAggregate));
						}
					});
				var _n9 = function () {
					var _n10 = elm.absolutelyPositioned;
					if (_n10.$ === 'Nothing') {
						return _Utils_Tuple2(elm$core$Maybe$Nothing, elm$core$Maybe$Nothing);
					} else {
						var others = _n10.a;
						return function (_n11) {
							var children = _n11.a;
							var onScreen = _n11.b;
							return _Utils_Tuple2(
								maybeOnEmptyList(children),
								maybeOnEmptyList(onScreen));
						}(
							A3(
								elm$core$List$foldr,
								adjustAndMerge(elm$core$Maybe$Nothing),
								_Utils_Tuple2(_List_Nil, _List_Nil),
								others));
					}
				}();
				var adjustedOthers = _n9.a;
				var otherChildrenData = _n9.b;
				var _n12 = function () {
					var _n13 = elm.children;
					if (_n13.$ === 'Normal') {
						var normalChildren = _n13.a;
						var _n14 = A3(
							elm$core$List$foldr,
							adjustAndMerge(
								elm$core$Maybe$Just(elm.layout)),
							_Utils_Tuple2(_List_Nil, _List_Nil),
							normalChildren);
						var adjusted = _n14.a;
						var data = _n14.b;
						return _Utils_Tuple2(
							mdgriffith$style_elements$Element$Internal$Model$Normal(adjusted),
							maybeOnEmptyList(data));
					} else {
						var keyedChildren = _n13.a;
						var _n15 = A3(
							elm$core$List$foldr,
							adjustAndMergeKeyed(
								elm$core$Maybe$Just(elm.layout)),
							_Utils_Tuple2(_List_Nil, _List_Nil),
							keyedChildren);
						var adjusted = _n15.a;
						var data = _n15.b;
						return _Utils_Tuple2(
							mdgriffith$style_elements$Element$Internal$Model$Keyed(adjusted),
							maybeOnEmptyList(data));
					}
				}();
				var adjustedChildren = _n12.a;
				var childrenData = _n12.b;
				var _n16 = A2(
					fn,
					parent,
					mdgriffith$style_elements$Element$Internal$Model$Layout(
						_Utils_update(
							elm,
							{absolutelyPositioned: adjustedOthers, children: adjustedChildren})));
				var adjustedLayout = _n16.a;
				var layoutData = _n16.b;
				return _Utils_Tuple2(
					adjustedLayout,
					A3(
						elm$core$List$foldr,
						merge,
						elm$core$Maybe$Nothing,
						_List_fromArray(
							[layoutData, childrenData, otherChildrenData])));
			default:
				return A2(fn, elm$core$Maybe$Nothing, elemen);
		}
	});
var mdgriffith$style_elements$Element$Internal$Adjustments$apply = function (root) {
	var stack = F2(
		function (parent, el) {
			return A3(
				elm$core$Basics$composeR,
				mdgriffith$style_elements$Element$Internal$Adjustments$centerTextLayout,
				A2(
					elm$core$Basics$composeR,
					mdgriffith$style_elements$Element$Internal$Adjustments$positionNearby(parent),
					mdgriffith$style_elements$Element$Internal$Adjustments$hoistFixedScreenElements),
				el);
		});
	return A3(mdgriffith$style_elements$Element$Internal$Model$adjust, stack, elm$core$Maybe$Nothing, root);
};
var mdgriffith$style_elements$Element$Internal$Render$FirstAndLast = {$: 'FirstAndLast'};
var elm$core$Basics$neq = _Utils_notEqual;
var elm$html$Html$em = _VirtualDom_node('em');
var elm$html$Html$s = _VirtualDom_node('s');
var elm$html$Html$span = _VirtualDom_node('span');
var elm$html$Html$strong = _VirtualDom_node('strong');
var elm$html$Html$sub = _VirtualDom_node('sub');
var elm$html$Html$sup = _VirtualDom_node('sup');
var elm$html$Html$u = _VirtualDom_node('u');
var elm$virtual_dom$VirtualDom$keyedNode = function (tag) {
	return _VirtualDom_keyedNode(
		_VirtualDom_noScript(tag));
};
var elm$html$Html$Keyed$node = elm$virtual_dom$VirtualDom$keyedNode;
var mdgriffith$style_elements$Element$Internal$Render$LayoutElement = function (a) {
	return {$: 'LayoutElement', a: a};
};
var mdgriffith$style_elements$Element$Internal$Render$Single = {$: 'Single'};
var mdgriffith$style_elements$Style$Internal$Model$Box = F4(
	function (a, b, c, d) {
		return {$: 'Box', a: a, b: b, c: c, d: d};
	});
var mdgriffith$style_elements$Element$Internal$Render$defaultPadding = F2(
	function (_n0, _n1) {
		var mW = _n0.a;
		var mX = _n0.b;
		var mY = _n0.c;
		var mZ = _n0.d;
		var w = _n1.a;
		var x = _n1.b;
		var y = _n1.c;
		var z = _n1.d;
		return A4(
			mdgriffith$style_elements$Style$Internal$Model$Box,
			A2(elm$core$Maybe$withDefault, w, mW),
			A2(elm$core$Maybe$withDefault, x, mX),
			A2(elm$core$Maybe$withDefault, y, mY),
			A2(elm$core$Maybe$withDefault, z, mZ));
	});
var mdgriffith$style_elements$Element$Internal$Render$First = {$: 'First'};
var mdgriffith$style_elements$Element$Internal$Render$Last = {$: 'Last'};
var mdgriffith$style_elements$Element$Internal$Render$Middle = function (a) {
	return {$: 'Middle', a: a};
};
var mdgriffith$style_elements$Element$Internal$Render$detectOrder = F2(
	function (list, i) {
		var len = elm$core$List$length(list);
		return ((!i) && (len === 1)) ? mdgriffith$style_elements$Element$Internal$Render$FirstAndLast : ((!i) ? mdgriffith$style_elements$Element$Internal$Render$First : (_Utils_eq(i, len - 1) ? mdgriffith$style_elements$Element$Internal$Render$Last : mdgriffith$style_elements$Element$Internal$Render$Middle(i)));
	});
var mdgriffith$style_elements$Element$Internal$Render$emptyPositionable = {
	attrs: _List_Nil,
	expand: false,
	frame: elm$core$Maybe$Nothing,
	gridPosition: elm$core$Maybe$Nothing,
	height: elm$core$Maybe$Nothing,
	hidden: false,
	horizontal: elm$core$Maybe$Nothing,
	inline: false,
	margin: elm$core$Maybe$Nothing,
	opacity: elm$core$Maybe$Nothing,
	overflow: elm$core$Maybe$Nothing,
	padding: A4(mdgriffith$style_elements$Style$Internal$Model$Box, elm$core$Maybe$Nothing, elm$core$Maybe$Nothing, elm$core$Maybe$Nothing, elm$core$Maybe$Nothing),
	pointerevents: elm$core$Maybe$Nothing,
	positioned: _Utils_Tuple3(elm$core$Maybe$Nothing, elm$core$Maybe$Nothing, elm$core$Maybe$Nothing),
	shrink: elm$core$Maybe$Nothing,
	variations: _List_Nil,
	vertical: elm$core$Maybe$Nothing,
	width: elm$core$Maybe$Nothing
};
var mdgriffith$style_elements$Style$Internal$Render$Value$gridPosition = function (_n0) {
	var start = _n0.a.start;
	var width = _n0.a.width;
	var height = _n0.a.height;
	var _n1 = start;
	var x = _n1.a;
	var y = _n1.b;
	var _n2 = _Utils_Tuple2(y + 1, (y + 1) + height);
	var rowStart = _n2.a;
	var rowEnd = _n2.b;
	var _n3 = _Utils_Tuple2(x + 1, (x + 1) + width);
	var colStart = _n3.a;
	var colEnd = _n3.b;
	return ((!width) || (!height)) ? elm$core$Maybe$Nothing : elm$core$Maybe$Just(
		A2(
			elm$core$String$join,
			' / ',
			_List_fromArray(
				[
					elm$core$String$fromInt(rowStart),
					elm$core$String$fromInt(colStart),
					elm$core$String$fromInt(rowEnd),
					elm$core$String$fromInt(colEnd)
				])));
};
var mdgriffith$style_elements$Element$Internal$Render$makePositionable = F2(
	function (attribute, pos) {
		switch (attribute.$) {
			case 'Overflow':
				var x = attribute.a;
				return _Utils_update(
					pos,
					{
						overflow: elm$core$Maybe$Just(x)
					});
			case 'Shrink':
				var i = attribute.a;
				return _Utils_update(
					pos,
					{
						shrink: elm$core$Maybe$Just(i)
					});
			case 'Inline':
				return _Utils_update(
					pos,
					{inline: true});
			case 'Expand':
				return _Utils_update(
					pos,
					{expand: true});
			case 'Vary':
				var vary = attribute.a;
				var on = attribute.b;
				return _Utils_update(
					pos,
					{
						variations: A2(
							elm$core$List$cons,
							_Utils_Tuple2(vary, on),
							pos.variations)
					});
			case 'Height':
				var len = attribute.a;
				return _Utils_update(
					pos,
					{
						height: elm$core$Maybe$Just(len)
					});
			case 'Width':
				var len = attribute.a;
				return _Utils_update(
					pos,
					{
						width: elm$core$Maybe$Just(len)
					});
			case 'Position':
				var x = attribute.a;
				var y = attribute.b;
				var z = attribute.c;
				var _n1 = pos.positioned;
				var currentX = _n1.a;
				var currentY = _n1.b;
				var currentZ = _n1.c;
				var newX = function () {
					if (x.$ === 'Nothing') {
						return currentX;
					} else {
						var a = x.a;
						return elm$core$Maybe$Just(a);
					}
				}();
				var newY = function () {
					if (y.$ === 'Nothing') {
						return currentY;
					} else {
						var a = y.a;
						return elm$core$Maybe$Just(a);
					}
				}();
				var newZ = function () {
					if (z.$ === 'Nothing') {
						return currentZ;
					} else {
						var a = z.a;
						return elm$core$Maybe$Just(a);
					}
				}();
				return _Utils_update(
					pos,
					{
						positioned: _Utils_Tuple3(newX, newY, newZ)
					});
			case 'PositionFrame':
				var frame = attribute.a;
				return _Utils_update(
					pos,
					{
						frame: elm$core$Maybe$Just(frame)
					});
			case 'HAlign':
				var alignment = attribute.a;
				return _Utils_update(
					pos,
					{
						horizontal: elm$core$Maybe$Just(alignment)
					});
			case 'VAlign':
				var alignment = attribute.a;
				return _Utils_update(
					pos,
					{
						vertical: elm$core$Maybe$Just(alignment)
					});
			case 'Spacing':
				var spaceX = attribute.a;
				var spaceY = attribute.b;
				return pos;
			case 'Margin':
				var t = attribute.a;
				var r = attribute.b;
				var b = attribute.c;
				var l = attribute.d;
				return _Utils_update(
					pos,
					{
						margin: elm$core$Maybe$Just(
							A4(mdgriffith$style_elements$Style$Internal$Model$Box, t, r, b, l))
					});
			case 'PhantomPadding':
				return pos;
			case 'Padding':
				var top = attribute.a;
				var right = attribute.b;
				var bottom = attribute.c;
				var left = attribute.d;
				var _n5 = pos.padding;
				var currentTop = _n5.a;
				var currentRight = _n5.b;
				var currentBottom = _n5.c;
				var currentLeft = _n5.d;
				var newBottom = function () {
					if (bottom.$ === 'Nothing') {
						return currentBottom;
					} else {
						var a = bottom.a;
						return elm$core$Maybe$Just(a);
					}
				}();
				var newLeft = function () {
					if (left.$ === 'Nothing') {
						return currentLeft;
					} else {
						var a = left.a;
						return elm$core$Maybe$Just(a);
					}
				}();
				var newRight = function () {
					if (right.$ === 'Nothing') {
						return currentRight;
					} else {
						var a = right.a;
						return elm$core$Maybe$Just(a);
					}
				}();
				var newTop = function () {
					if (top.$ === 'Nothing') {
						return currentTop;
					} else {
						var a = top.a;
						return elm$core$Maybe$Just(a);
					}
				}();
				return _Utils_update(
					pos,
					{
						padding: A4(mdgriffith$style_elements$Style$Internal$Model$Box, newTop, newRight, newBottom, newLeft)
					});
			case 'Hidden':
				return _Utils_update(
					pos,
					{hidden: true});
			case 'Opacity':
				var t = attribute.a;
				return _Utils_update(
					pos,
					{
						opacity: elm$core$Maybe$Just(t)
					});
			case 'Event':
				var ev = attribute.a;
				return _Utils_update(
					pos,
					{
						attrs: A2(elm$core$List$cons, ev, pos.attrs)
					});
			case 'InputEvent':
				var ev = attribute.a;
				return _Utils_update(
					pos,
					{
						attrs: A2(elm$core$List$cons, ev, pos.attrs)
					});
			case 'Attr':
				var attr = attribute.a;
				return _Utils_update(
					pos,
					{
						attrs: A2(elm$core$List$cons, attr, pos.attrs)
					});
			case 'PointerEvents':
				var on = attribute.a;
				return _Utils_update(
					pos,
					{
						pointerevents: elm$core$Maybe$Just(on)
					});
			case 'GridArea':
				var name = attribute.a;
				return _Utils_update(
					pos,
					{
						gridPosition: elm$core$Maybe$Just(name)
					});
			default:
				var coords = attribute.a;
				var _n10 = mdgriffith$style_elements$Style$Internal$Render$Value$gridPosition(coords);
				if (_n10.$ === 'Nothing') {
					return _Utils_update(
						pos,
						{hidden: true});
				} else {
					var xy = _n10.a;
					return _Utils_update(
						pos,
						{
							gridPosition: elm$core$Maybe$Just(xy)
						});
				}
		}
	});
var mdgriffith$style_elements$Element$Internal$Render$gather = function (attrs) {
	return A3(elm$core$List$foldl, mdgriffith$style_elements$Element$Internal$Render$makePositionable, mdgriffith$style_elements$Element$Internal$Render$emptyPositionable, attrs);
};
var elm$core$Tuple$second = function (_n0) {
	var y = _n0.b;
	return y;
};
var elm$html$Html$Attributes$classList = function (classes) {
	return elm$html$Html$Attributes$class(
		A2(
			elm$core$String$join,
			' ',
			A2(
				elm$core$List$map,
				elm$core$Tuple$first,
				A2(elm$core$List$filter, elm$core$Tuple$second, classes))));
};
var mdgriffith$style_elements$Style$Internal$Model$Bottom = {$: 'Bottom'};
var mdgriffith$style_elements$Style$Internal$Model$Center = {$: 'Center'};
var mdgriffith$style_elements$Style$Internal$Model$GridH = function (a) {
	return {$: 'GridH', a: a};
};
var mdgriffith$style_elements$Style$Internal$Model$GridV = function (a) {
	return {$: 'GridV', a: a};
};
var mdgriffith$style_elements$Style$Internal$Model$Horz = function (a) {
	return {$: 'Horz', a: a};
};
var mdgriffith$style_elements$Style$Internal$Model$Justify = {$: 'Justify'};
var mdgriffith$style_elements$Style$Internal$Model$Left = {$: 'Left'};
var mdgriffith$style_elements$Style$Internal$Model$Other = function (a) {
	return {$: 'Other', a: a};
};
var mdgriffith$style_elements$Style$Internal$Model$Right = {$: 'Right'};
var mdgriffith$style_elements$Style$Internal$Model$TextLayout = function (a) {
	return {$: 'TextLayout', a: a};
};
var mdgriffith$style_elements$Style$Internal$Model$Top = {$: 'Top'};
var mdgriffith$style_elements$Style$Internal$Model$Vert = function (a) {
	return {$: 'Vert', a: a};
};
var mdgriffith$style_elements$Element$Internal$Render$alignLayout = F3(
	function (maybeHorizontal, maybeVertical, layout) {
		var alignGridVertical = function (align) {
			switch (align.$) {
				case 'Top':
					return mdgriffith$style_elements$Style$Internal$Model$GridV(
						mdgriffith$style_elements$Style$Internal$Model$Other(mdgriffith$style_elements$Style$Internal$Model$Top));
				case 'Bottom':
					return mdgriffith$style_elements$Style$Internal$Model$GridV(
						mdgriffith$style_elements$Style$Internal$Model$Other(mdgriffith$style_elements$Style$Internal$Model$Bottom));
				case 'VerticalCenter':
					return mdgriffith$style_elements$Style$Internal$Model$GridV(mdgriffith$style_elements$Style$Internal$Model$Center);
				default:
					return mdgriffith$style_elements$Style$Internal$Model$GridV(mdgriffith$style_elements$Style$Internal$Model$Justify);
			}
		};
		var alignGridHorizontal = function (align) {
			switch (align.$) {
				case 'Left':
					return mdgriffith$style_elements$Style$Internal$Model$GridH(
						mdgriffith$style_elements$Style$Internal$Model$Other(mdgriffith$style_elements$Style$Internal$Model$Left));
				case 'Right':
					return mdgriffith$style_elements$Style$Internal$Model$GridH(
						mdgriffith$style_elements$Style$Internal$Model$Other(mdgriffith$style_elements$Style$Internal$Model$Right));
				case 'Center':
					return mdgriffith$style_elements$Style$Internal$Model$GridH(mdgriffith$style_elements$Style$Internal$Model$Center);
				default:
					return mdgriffith$style_elements$Style$Internal$Model$GridH(mdgriffith$style_elements$Style$Internal$Model$Justify);
			}
		};
		var alignFlexboxVertical = function (align) {
			switch (align.$) {
				case 'Top':
					return mdgriffith$style_elements$Style$Internal$Model$Vert(
						mdgriffith$style_elements$Style$Internal$Model$Other(mdgriffith$style_elements$Style$Internal$Model$Top));
				case 'Bottom':
					return mdgriffith$style_elements$Style$Internal$Model$Vert(
						mdgriffith$style_elements$Style$Internal$Model$Other(mdgriffith$style_elements$Style$Internal$Model$Bottom));
				case 'VerticalCenter':
					return mdgriffith$style_elements$Style$Internal$Model$Vert(mdgriffith$style_elements$Style$Internal$Model$Center);
				default:
					return mdgriffith$style_elements$Style$Internal$Model$Vert(mdgriffith$style_elements$Style$Internal$Model$Justify);
			}
		};
		var alignFlexboxHorizontal = function (align) {
			switch (align.$) {
				case 'Left':
					return mdgriffith$style_elements$Style$Internal$Model$Horz(
						mdgriffith$style_elements$Style$Internal$Model$Other(mdgriffith$style_elements$Style$Internal$Model$Left));
				case 'Right':
					return mdgriffith$style_elements$Style$Internal$Model$Horz(
						mdgriffith$style_elements$Style$Internal$Model$Other(mdgriffith$style_elements$Style$Internal$Model$Right));
				case 'Center':
					return mdgriffith$style_elements$Style$Internal$Model$Horz(mdgriffith$style_elements$Style$Internal$Model$Center);
				default:
					return mdgriffith$style_elements$Style$Internal$Model$Horz(mdgriffith$style_elements$Style$Internal$Model$Justify);
			}
		};
		switch (layout.$) {
			case 'TextLayout':
				var clearfix = layout.a;
				return mdgriffith$style_elements$Style$Internal$Model$TextLayout(clearfix);
			case 'FlexLayout':
				var dir = layout.a;
				var els = layout.b;
				var _n1 = _Utils_Tuple2(maybeHorizontal, maybeVertical);
				if (_n1.a.$ === 'Nothing') {
					if (_n1.b.$ === 'Nothing') {
						var _n2 = _n1.a;
						var _n3 = _n1.b;
						return A2(mdgriffith$style_elements$Style$Internal$Model$FlexLayout, dir, els);
					} else {
						var _n5 = _n1.a;
						var v = _n1.b.a;
						return A2(
							mdgriffith$style_elements$Style$Internal$Model$FlexLayout,
							dir,
							A2(
								elm$core$List$cons,
								alignFlexboxVertical(v),
								els));
					}
				} else {
					if (_n1.b.$ === 'Nothing') {
						var h = _n1.a.a;
						var _n4 = _n1.b;
						return A2(
							mdgriffith$style_elements$Style$Internal$Model$FlexLayout,
							dir,
							A2(
								elm$core$List$cons,
								alignFlexboxHorizontal(h),
								els));
					} else {
						var h = _n1.a.a;
						var v = _n1.b.a;
						return A2(
							mdgriffith$style_elements$Style$Internal$Model$FlexLayout,
							dir,
							A2(
								elm$core$List$cons,
								alignFlexboxHorizontal(h),
								A2(
									elm$core$List$cons,
									alignFlexboxVertical(v),
									els)));
					}
				}
			default:
				var template = layout.a;
				var els = layout.b;
				var _n6 = _Utils_Tuple2(maybeHorizontal, maybeVertical);
				if (_n6.a.$ === 'Nothing') {
					if (_n6.b.$ === 'Nothing') {
						var _n7 = _n6.a;
						var _n8 = _n6.b;
						return A2(mdgriffith$style_elements$Style$Internal$Model$Grid, template, els);
					} else {
						var _n10 = _n6.a;
						var v = _n6.b.a;
						return A2(
							mdgriffith$style_elements$Style$Internal$Model$Grid,
							template,
							A2(
								elm$core$List$cons,
								alignGridVertical(v),
								els));
					}
				} else {
					if (_n6.b.$ === 'Nothing') {
						var h = _n6.a.a;
						var _n9 = _n6.b;
						return A2(
							mdgriffith$style_elements$Style$Internal$Model$Grid,
							template,
							A2(
								elm$core$List$cons,
								alignGridHorizontal(h),
								els));
					} else {
						var h = _n6.a.a;
						var v = _n6.b.a;
						return A2(
							mdgriffith$style_elements$Style$Internal$Model$Grid,
							template,
							A2(
								elm$core$List$cons,
								alignGridHorizontal(h),
								A2(
									elm$core$List$cons,
									alignGridVertical(v),
									els)));
					}
				}
		}
	});
var mdgriffith$style_elements$Element$Internal$Render$calcPosition = F2(
	function (frame, _n0) {
		var mx = _n0.a;
		var my = _n0.b;
		var mz = _n0.c;
		var z = A2(elm$core$Maybe$withDefault, 0, mz);
		var y = A2(elm$core$Maybe$withDefault, 0, my);
		var x = A2(elm$core$Maybe$withDefault, 0, mx);
		switch (frame.$) {
			case 'Relative':
				return _List_fromArray(
					[
						_Utils_Tuple2('position', 'relative'),
						_Utils_Tuple2(
						'left',
						elm$core$String$fromFloat(x) + 'px'),
						_Utils_Tuple2(
						'top',
						elm$core$String$fromFloat(y) + 'px')
					]);
			case 'Screen':
				return _List_fromArray(
					[
						_Utils_Tuple2('position', 'fixed'),
						_Utils_Tuple2(
						'left',
						elm$core$String$fromFloat(x) + 'px'),
						_Utils_Tuple2(
						'top',
						elm$core$String$fromFloat(y) + 'px'),
						_Utils_Tuple2('z-index', '1000')
					]);
			case 'Absolute':
				if (frame.a.$ === 'TopLeft') {
					var _n2 = frame.a;
					return A2(
						elm$core$List$filterMap,
						elm$core$Basics$identity,
						_List_fromArray(
							[
								elm$core$Maybe$Just(
								_Utils_Tuple2('position', 'absolute')),
								function () {
								if (mx.$ === 'Just') {
									var xVal = mx.a;
									return elm$core$Maybe$Just(
										_Utils_Tuple2(
											'left',
											elm$core$String$fromFloat(xVal) + 'px'));
								} else {
									return elm$core$Maybe$Nothing;
								}
							}(),
								function () {
								if (my.$ === 'Just') {
									var yVal = my.a;
									return elm$core$Maybe$Just(
										_Utils_Tuple2(
											'top',
											elm$core$String$fromFloat(yVal) + 'px'));
								} else {
									return elm$core$Maybe$Nothing;
								}
							}()
							]));
				} else {
					var _n5 = frame.a;
					return A2(
						elm$core$List$filterMap,
						elm$core$Basics$identity,
						_List_fromArray(
							[
								elm$core$Maybe$Just(
								_Utils_Tuple2('position', 'absolute')),
								function () {
								if (mx.$ === 'Just') {
									var xVal = mx.a;
									return elm$core$Maybe$Just(
										_Utils_Tuple2(
											'left',
											elm$core$String$fromFloat(xVal) + 'px'));
								} else {
									return elm$core$Maybe$Nothing;
								}
							}(),
								function () {
								if (my.$ === 'Just') {
									var yVal = my.a;
									return elm$core$Maybe$Just(
										_Utils_Tuple2(
											'bottom',
											elm$core$String$fromFloat(yVal) + 'px'));
								} else {
									return elm$core$Maybe$Nothing;
								}
							}()
							]));
				}
			default:
				switch (frame.a.$) {
					case 'Within':
						var _n8 = frame.a;
						return _List_fromArray(
							[
								_Utils_Tuple2('position', 'relative'),
								_Utils_Tuple2(
								'top',
								elm$core$String$fromFloat(y) + 'px'),
								_Utils_Tuple2(
								'left',
								elm$core$String$fromFloat(x) + 'px')
							]);
					case 'Above':
						var _n9 = frame.a;
						return _List_fromArray(
							[
								_Utils_Tuple2('position', 'relative'),
								_Utils_Tuple2(
								'top',
								elm$core$String$fromFloat(y) + 'px'),
								_Utils_Tuple2(
								'left',
								elm$core$String$fromFloat(x) + 'px')
							]);
					case 'Below':
						var _n10 = frame.a;
						return _List_fromArray(
							[
								_Utils_Tuple2('position', 'relative'),
								_Utils_Tuple2(
								'top',
								'calc(100% + ' + (elm$core$String$fromFloat(y) + 'px)')),
								_Utils_Tuple2(
								'left',
								elm$core$String$fromFloat(x) + 'px')
							]);
					case 'OnLeft':
						var _n11 = frame.a;
						return _List_fromArray(
							[
								_Utils_Tuple2('position', 'relative'),
								_Utils_Tuple2(
								'right',
								'calc(100% - ' + (elm$core$String$fromFloat(x) + 'px)')),
								_Utils_Tuple2(
								'top',
								elm$core$String$fromFloat(y) + 'px')
							]);
					default:
						var _n12 = frame.a;
						return _List_fromArray(
							[
								_Utils_Tuple2('position', 'relative'),
								_Utils_Tuple2(
								'left',
								'calc(100% + ' + (elm$core$String$fromFloat(x) + 'px)')),
								_Utils_Tuple2(
								'top',
								elm$core$String$fromFloat(y) + 'px')
							]);
				}
		}
	});
var mdgriffith$style_elements$Element$Internal$Render$flexboxHorizontalIndividualAlignment = F2(
	function (direction, alignment) {
		switch (direction.$) {
			case 'GoRight':
				switch (alignment.$) {
					case 'Left':
						return elm$core$Maybe$Nothing;
					case 'Right':
						return elm$core$Maybe$Nothing;
					case 'Center':
						return elm$core$Maybe$Nothing;
					default:
						return elm$core$Maybe$Nothing;
				}
			case 'GoLeft':
				switch (alignment.$) {
					case 'Left':
						return elm$core$Maybe$Nothing;
					case 'Right':
						return elm$core$Maybe$Nothing;
					case 'Center':
						return elm$core$Maybe$Nothing;
					default:
						return elm$core$Maybe$Nothing;
				}
			case 'Down':
				switch (alignment.$) {
					case 'Left':
						return elm$core$Maybe$Just(
							_Utils_Tuple2('align-self', 'flex-start'));
					case 'Right':
						return elm$core$Maybe$Just(
							_Utils_Tuple2('align-self', 'flex-end'));
					case 'Center':
						return elm$core$Maybe$Just(
							_Utils_Tuple2('align-self', 'center'));
					default:
						return elm$core$Maybe$Just(
							_Utils_Tuple2('align-self', 'stretch'));
				}
			default:
				switch (alignment.$) {
					case 'Left':
						return elm$core$Maybe$Just(
							_Utils_Tuple2('align-self', 'flex-start'));
					case 'Right':
						return elm$core$Maybe$Just(
							_Utils_Tuple2('align-self', 'flex-end'));
					case 'Center':
						return elm$core$Maybe$Just(
							_Utils_Tuple2('align-self', 'center'));
					default:
						return elm$core$Maybe$Just(
							_Utils_Tuple2('align-self', 'stretch'));
				}
		}
	});
var mdgriffith$style_elements$Element$Internal$Render$flexboxVerticalIndividualAlignment = F2(
	function (direction, alignment) {
		switch (direction.$) {
			case 'GoRight':
				switch (alignment.$) {
					case 'Top':
						return elm$core$Maybe$Just(
							_Utils_Tuple2('align-self', 'flex-start'));
					case 'Bottom':
						return elm$core$Maybe$Just(
							_Utils_Tuple2('align-self', 'flex-end'));
					case 'VerticalCenter':
						return elm$core$Maybe$Just(
							_Utils_Tuple2('align-self', 'center'));
					default:
						return elm$core$Maybe$Just(
							_Utils_Tuple2('align-self', 'center'));
				}
			case 'GoLeft':
				switch (alignment.$) {
					case 'Top':
						return elm$core$Maybe$Just(
							_Utils_Tuple2('align-self', 'flex-start'));
					case 'Bottom':
						return elm$core$Maybe$Just(
							_Utils_Tuple2('align-self', 'flex-end'));
					case 'VerticalCenter':
						return elm$core$Maybe$Just(
							_Utils_Tuple2('align-self', 'center'));
					default:
						return elm$core$Maybe$Just(
							_Utils_Tuple2('align-self', 'center'));
				}
			case 'Down':
				switch (alignment.$) {
					case 'Top':
						return elm$core$Maybe$Nothing;
					case 'Bottom':
						return elm$core$Maybe$Nothing;
					case 'VerticalCenter':
						return elm$core$Maybe$Nothing;
					default:
						return elm$core$Maybe$Nothing;
				}
			default:
				switch (alignment.$) {
					case 'Top':
						return elm$core$Maybe$Nothing;
					case 'Bottom':
						return elm$core$Maybe$Nothing;
					case 'VerticalCenter':
						return elm$core$Maybe$Nothing;
					default:
						return elm$core$Maybe$Nothing;
				}
		}
	});
var mdgriffith$style_elements$Element$Internal$Render$renderPadding = function (_n0) {
	var top = _n0.a;
	var right = _n0.b;
	var bottom = _n0.c;
	var left = _n0.d;
	var format = F2(
		function (name, x) {
			return _Utils_Tuple2(
				name,
				elm$core$String$fromFloat(x) + 'px');
		});
	return A2(
		elm$core$List$filterMap,
		elm$core$Basics$identity,
		_List_fromArray(
			[
				A2(
				elm$core$Maybe$map,
				format('padding-top'),
				top),
				A2(
				elm$core$Maybe$map,
				format('padding-bottom'),
				bottom),
				A2(
				elm$core$Maybe$map,
				format('padding-left'),
				left),
				A2(
				elm$core$Maybe$map,
				format('padding-right'),
				right)
			]));
};
var mdgriffith$style_elements$Style$Internal$Render$Property$flexHeight = function (l) {
	switch (l.$) {
		case 'Px':
			var x = l.a;
			return _List_fromArray(
				[
					_Utils_Tuple2(
					'height',
					elm$core$String$fromFloat(x) + 'px')
				]);
		case 'Percent':
			var x = l.a;
			return _List_fromArray(
				[
					_Utils_Tuple2(
					'height',
					elm$core$String$fromFloat(x) + '%')
				]);
		case 'Auto':
			return _List_fromArray(
				[
					_Utils_Tuple2('height', 'auto')
				]);
		case 'Fill':
			var i = l.a;
			return _List_fromArray(
				[
					_Utils_Tuple2(
					'flex-grow',
					elm$core$String$fromFloat(i)),
					_Utils_Tuple2('flex-basis', '0')
				]);
		default:
			var perc = l.a;
			var px = l.b;
			return _List_fromArray(
				[
					_Utils_Tuple2(
					'height',
					'calc(' + (elm$core$String$fromFloat(perc) + ('% + ' + (elm$core$String$fromFloat(px) + 'px)'))))
				]);
	}
};
var mdgriffith$style_elements$Style$Internal$Render$Property$flexWidth = F2(
	function (len, adjustment) {
		switch (len.$) {
			case 'Px':
				var x = len.a;
				return _List_fromArray(
					[
						_Utils_Tuple2(
						'width',
						elm$core$String$fromFloat(x) + 'px')
					]);
			case 'Percent':
				var x = len.a;
				return _List_fromArray(
					[
						_Utils_Tuple2(
						'width',
						'calc(' + (elm$core$String$fromFloat(x) + ('% - ' + (elm$core$String$fromFloat(adjustment) + 'px)'))))
					]);
			case 'Auto':
				return _List_fromArray(
					[
						_Utils_Tuple2('width', 'auto')
					]);
			case 'Fill':
				var i = len.a;
				return _List_fromArray(
					[
						_Utils_Tuple2(
						'flex-grow',
						elm$core$String$fromFloat(i)),
						_Utils_Tuple2('flex-basis', '0')
					]);
			default:
				var perc = len.a;
				var px = len.b;
				return _List_fromArray(
					[
						_Utils_Tuple2(
						'width',
						'calc(' + (elm$core$String$fromFloat(perc) + ('% + ' + (elm$core$String$fromFloat(px) + 'px)'))))
					]);
		}
	});
var mdgriffith$style_elements$Style$Internal$Render$Property$direction = function (dir) {
	switch (dir.$) {
		case 'GoRight':
			return _Utils_Tuple2('flex-direction', 'row');
		case 'GoLeft':
			return _Utils_Tuple2('flex-direction', 'row-reverse');
		case 'Down':
			return _Utils_Tuple2('flex-direction', 'column');
		default:
			return _Utils_Tuple2('flex-direction', 'column-reverse');
	}
};
var mdgriffith$style_elements$Style$Internal$Render$Property$flexbox = F2(
	function (dir, el) {
		switch (el.$) {
			case 'Wrap':
				var wrap = el.a;
				return wrap ? _Utils_Tuple2('flex-wrap', 'wrap') : _Utils_Tuple2('flex-wrap', 'nowrap');
			case 'Horz':
				var horizontal = el.a;
				switch (dir.$) {
					case 'GoRight':
						switch (horizontal.$) {
							case 'Other':
								if (horizontal.a.$ === 'Left') {
									var _n3 = horizontal.a;
									return _Utils_Tuple2('justify-content', 'flex-start');
								} else {
									var _n4 = horizontal.a;
									return _Utils_Tuple2('justify-content', 'flex-end');
								}
							case 'Center':
								return _Utils_Tuple2('justify-content', 'center');
							case 'Justify':
								return _Utils_Tuple2('justify-content', 'space-between');
							default:
								return _Utils_Tuple2('justify-content', 'space-between');
						}
					case 'GoLeft':
						switch (horizontal.$) {
							case 'Other':
								if (horizontal.a.$ === 'Left') {
									var _n6 = horizontal.a;
									return _Utils_Tuple2('justify-content', 'flex-end');
								} else {
									var _n7 = horizontal.a;
									return _Utils_Tuple2('justify-content', 'flex-start');
								}
							case 'Center':
								return _Utils_Tuple2('justify-content', 'center');
							case 'Justify':
								return _Utils_Tuple2('justify-content', 'space-between');
							default:
								return _Utils_Tuple2('justify-content', 'space-between');
						}
					case 'Down':
						switch (horizontal.$) {
							case 'Other':
								if (horizontal.a.$ === 'Left') {
									var _n9 = horizontal.a;
									return _Utils_Tuple2('align-items', 'flex-start');
								} else {
									var _n10 = horizontal.a;
									return _Utils_Tuple2('align-items', 'flex-end');
								}
							case 'Center':
								return _Utils_Tuple2('align-items', 'center');
							case 'Justify':
								return _Utils_Tuple2('align-items', 'Justify');
							default:
								return _Utils_Tuple2('align-items', 'Justify');
						}
					default:
						switch (horizontal.$) {
							case 'Other':
								if (horizontal.a.$ === 'Left') {
									var _n12 = horizontal.a;
									return _Utils_Tuple2('align-items', 'flex-start');
								} else {
									var _n13 = horizontal.a;
									return _Utils_Tuple2('align-items', 'flex-end');
								}
							case 'Center':
								return _Utils_Tuple2('align-items', 'center');
							case 'Justify':
								return _Utils_Tuple2('align-items', 'Justify');
							default:
								return _Utils_Tuple2('align-items', 'Justify');
						}
				}
			default:
				var vertical = el.a;
				switch (dir.$) {
					case 'GoRight':
						switch (vertical.$) {
							case 'Other':
								if (vertical.a.$ === 'Top') {
									var _n16 = vertical.a;
									return _Utils_Tuple2('align-items', 'flex-start');
								} else {
									var _n17 = vertical.a;
									return _Utils_Tuple2('align-items', 'flex-end');
								}
							case 'Center':
								return _Utils_Tuple2('align-items', 'center');
							case 'Justify':
								return _Utils_Tuple2('align-items', 'Justify');
							default:
								return _Utils_Tuple2('align-items', 'Justify');
						}
					case 'GoLeft':
						switch (vertical.$) {
							case 'Other':
								if (vertical.a.$ === 'Top') {
									var _n19 = vertical.a;
									return _Utils_Tuple2('align-items', 'flex-start');
								} else {
									var _n20 = vertical.a;
									return _Utils_Tuple2('align-items', 'flex-end');
								}
							case 'Center':
								return _Utils_Tuple2('align-items', 'center');
							case 'Justify':
								return _Utils_Tuple2('align-items', 'Justify');
							default:
								return _Utils_Tuple2('align-items', 'Justify');
						}
					case 'Down':
						switch (vertical.$) {
							case 'Other':
								if (vertical.a.$ === 'Top') {
									var _n22 = vertical.a;
									return _Utils_Tuple2('justify-content', 'flex-start');
								} else {
									var _n23 = vertical.a;
									return _Utils_Tuple2('justify-content', 'flex-end');
								}
							case 'Center':
								return _Utils_Tuple2('justify-content', 'center');
							case 'Justify':
								return _Utils_Tuple2('justify-content', 'space-between');
							default:
								return _Utils_Tuple2('align-items', 'Justify');
						}
					default:
						switch (vertical.$) {
							case 'Other':
								if (vertical.a.$ === 'Top') {
									var _n25 = vertical.a;
									return _Utils_Tuple2('justify-content', 'flex-end');
								} else {
									var _n26 = vertical.a;
									return _Utils_Tuple2('justify-content', 'flex-start');
								}
							case 'Center':
								return _Utils_Tuple2('justify-content', 'center');
							case 'Justify':
								return _Utils_Tuple2('justify-content', 'space-between');
							default:
								return _Utils_Tuple2('align-items', 'Justify');
						}
				}
		}
	});
var mdgriffith$style_elements$Style$Internal$Render$Property$gridAlignment = function (align) {
	switch (align.$) {
		case 'GridGap':
			var row = align.a;
			var column = align.b;
			return _Utils_Tuple2(
				'grid-gap',
				elm$core$String$fromFloat(row) + ('px ' + (elm$core$String$fromFloat(column) + 'px')));
		case 'GridH':
			var horizontal = align.a;
			switch (horizontal.$) {
				case 'Other':
					if (horizontal.a.$ === 'Left') {
						var _n2 = horizontal.a;
						return _Utils_Tuple2('justify-content', 'start');
					} else {
						var _n3 = horizontal.a;
						return _Utils_Tuple2('justify-content', 'end');
					}
				case 'Center':
					return _Utils_Tuple2('justify-content', 'center');
				case 'Justify':
					return _Utils_Tuple2('justify-content', 'space-between');
				default:
					return _Utils_Tuple2('justify-content', 'space-between');
			}
		default:
			var vertical = align.a;
			switch (vertical.$) {
				case 'Other':
					if (vertical.a.$ === 'Top') {
						var _n5 = vertical.a;
						return _Utils_Tuple2('align-content', 'start');
					} else {
						var _n6 = vertical.a;
						return _Utils_Tuple2('align-content', 'end');
					}
				case 'Center':
					return _Utils_Tuple2('align-content', 'center');
				case 'Justify':
					return _Utils_Tuple2('align-content', 'space-between');
				default:
					return _Utils_Tuple2('align-content', 'space-between');
			}
	}
};
var mdgriffith$style_elements$Style$Internal$Render$Property$layout = F2(
	function (inline, lay) {
		switch (lay.$) {
			case 'TextLayout':
				return _List_fromArray(
					[
						_Utils_Tuple2(
						'display',
						inline ? 'inline-block' : 'block')
					]);
			case 'FlexLayout':
				var dir = lay.a;
				var flexProps = lay.b;
				return A2(
					elm$core$List$cons,
					_Utils_Tuple2(
						'display',
						inline ? 'inline-flex' : 'flex'),
					A2(
						elm$core$List$cons,
						mdgriffith$style_elements$Style$Internal$Render$Property$direction(dir),
						A2(
							elm$core$List$map,
							mdgriffith$style_elements$Style$Internal$Render$Property$flexbox(dir),
							flexProps)));
			default:
				if (lay.a.$ === 'NamedGridTemplate') {
					var rows = lay.a.a.rows;
					var columns = lay.a.a.columns;
					var options = lay.b;
					var renderLen = function (len) {
						switch (len.$) {
							case 'Px':
								var x = len.a;
								return elm$core$String$fromFloat(x) + 'px';
							case 'Percent':
								var x = len.a;
								return elm$core$String$fromFloat(x) + '%';
							case 'Auto':
								return 'auto';
							case 'Fill':
								var i = len.a;
								return elm$core$String$fromFloat(i) + 'fr';
							default:
								var perc = len.a;
								var px = len.b;
								return 'calc(' + (elm$core$String$fromFloat(perc) + ('% + ' + (elm$core$String$fromFloat(px) + 'px)')));
						}
					};
					var grid = inline ? _Utils_Tuple2('display', 'inline-grid') : _Utils_Tuple2('display', 'grid');
					var areaSpan = function (_n3) {
						var span = _n3.a;
						var maybeName = _n3.b;
						var name = function () {
							if (maybeName.$ === 'Nothing') {
								return '.';
							} else {
								var str = maybeName.a;
								return str;
							}
						}();
						if (span.$ === 'SpanAll') {
							return A2(
								elm$core$List$repeat,
								elm$core$List$length(columns),
								name);
						} else {
							var i = span.a;
							return A2(elm$core$List$repeat, i, name);
						}
					};
					var areasInRow = function (areas) {
						var quote = function (str) {
							return '\"' + (str + '\"');
						};
						var areaStrs = A2(elm$core$List$concatMap, areaSpan, areas);
						return (_Utils_cmp(
							elm$core$List$length(areaStrs),
							elm$core$List$length(columns)) > 0) ? quote(
							A2(elm$core$String$join, ' ', areaStrs)) : ((_Utils_cmp(
							elm$core$List$length(areaStrs),
							elm$core$List$length(columns)) < 0) ? quote(
							A2(elm$core$String$join, ' ', areaStrs)) : quote(
							A2(elm$core$String$join, ' ', areaStrs)));
					};
					var alignment = A2(elm$core$List$map, mdgriffith$style_elements$Style$Internal$Render$Property$gridAlignment, options);
					return A2(
						elm$core$List$cons,
						grid,
						A2(
							elm$core$List$cons,
							_Utils_Tuple2(
								'grid-template-rows',
								A2(
									elm$core$String$join,
									' ',
									A2(
										elm$core$List$map,
										A2(elm$core$Basics$composeL, renderLen, elm$core$Tuple$first),
										rows))),
							A2(
								elm$core$List$cons,
								_Utils_Tuple2(
									'grid-template-columns',
									A2(
										elm$core$String$join,
										' ',
										A2(elm$core$List$map, renderLen, columns))),
								A2(
									elm$core$List$cons,
									_Utils_Tuple2(
										'grid-template-areas',
										A2(
											elm$core$String$join,
											'\n',
											A2(
												elm$core$List$map,
												A2(elm$core$Basics$composeL, areasInRow, elm$core$Tuple$second),
												rows))),
									alignment))));
				} else {
					var rows = lay.a.a.rows;
					var columns = lay.a.a.columns;
					var options = lay.b;
					var renderLen = function (len) {
						switch (len.$) {
							case 'Px':
								var x = len.a;
								return elm$core$String$fromFloat(x) + 'px';
							case 'Percent':
								var x = len.a;
								return elm$core$String$fromFloat(x) + '%';
							case 'Auto':
								return 'auto';
							case 'Fill':
								var i = len.a;
								return elm$core$String$fromFloat(i) + 'fr';
							default:
								var perc = len.a;
								var px = len.b;
								return 'calc(' + (elm$core$String$fromFloat(perc) + ('% + ' + (elm$core$String$fromFloat(px) + 'px)')));
						}
					};
					var grid = inline ? _Utils_Tuple2('display', 'inline-grid') : _Utils_Tuple2('display', 'grid');
					var alignment = A2(elm$core$List$map, mdgriffith$style_elements$Style$Internal$Render$Property$gridAlignment, options);
					return A2(
						elm$core$List$cons,
						grid,
						A2(
							elm$core$List$cons,
							_Utils_Tuple2(
								'grid-template-rows',
								A2(
									elm$core$String$join,
									' ',
									A2(elm$core$List$map, renderLen, rows))),
							A2(
								elm$core$List$cons,
								_Utils_Tuple2(
									'grid-template-columns',
									A2(
										elm$core$String$join,
										' ',
										A2(elm$core$List$map, renderLen, columns))),
								alignment)));
				}
		}
	});
var mdgriffith$style_elements$Style$Internal$Render$Value$box = function (_n0) {
	var a = _n0.a;
	var b = _n0.b;
	var c = _n0.c;
	var d = _n0.d;
	return elm$core$String$fromFloat(a) + ('px ' + (elm$core$String$fromFloat(b) + ('px ' + (elm$core$String$fromFloat(c) + ('px ' + (elm$core$String$fromFloat(d) + 'px'))))));
};
var mdgriffith$style_elements$Style$Internal$Render$Value$length = function (l) {
	switch (l.$) {
		case 'Px':
			var x = l.a;
			return elm$core$String$fromFloat(x) + 'px';
		case 'Percent':
			var x = l.a;
			return elm$core$String$fromFloat(x) + '%';
		case 'Auto':
			return 'auto';
		case 'Fill':
			var i = l.a;
			return '100%';
		default:
			var perc = l.a;
			var px = l.b;
			return 'calc(' + (elm$core$String$fromFloat(perc) + ('% + ' + (elm$core$String$fromFloat(px) + 'px)')));
	}
};
var mdgriffith$style_elements$Style$Internal$Render$Value$parentAdjustedLength = F2(
	function (len, adjustment) {
		switch (len.$) {
			case 'Px':
				var x = len.a;
				return elm$core$String$fromFloat(x) + 'px';
			case 'Percent':
				var x = len.a;
				return 'calc(' + (elm$core$String$fromFloat(x) + ('% - ' + (elm$core$String$fromFloat(adjustment) + 'px)')));
			case 'Auto':
				return 'auto';
			case 'Fill':
				var i = len.a;
				return 'calc(100% - ' + (elm$core$String$fromFloat(adjustment) + 'px)');
			default:
				var perc = len.a;
				var px = len.b;
				return 'calc(' + (elm$core$String$fromFloat(perc) + ('% + ' + (elm$core$String$fromFloat(px) + 'px)')));
		}
	});
var mdgriffith$style_elements$Element$Internal$Render$renderAttributes = F6(
	function (elType, order, maybeElemID, parent, stylesheet, elem) {
		var width = function (attrs) {
			var _n62 = elem.width;
			if (_n62.$ === 'Nothing') {
				return attrs;
			} else {
				var len = _n62.a;
				if (parent.$ === 'Just') {
					var parentEl = parent.a;
					var _n64 = A2(
						elm$core$Maybe$withDefault,
						A4(mdgriffith$style_elements$Style$Internal$Model$Box, 0, 0, 0, 0),
						parentEl.parentSpecifiedSpacing);
					var rightPad = _n64.b;
					var leftPad = _n64.d;
					var paddingAdjustment = (rightPad + leftPad) / 2;
					var _n65 = parentEl.layout;
					_n65$2:
					while (true) {
						if (_n65.$ === 'FlexLayout') {
							switch (_n65.a.$) {
								case 'GoRight':
									var _n66 = _n65.a;
									return _Utils_ap(
										A2(mdgriffith$style_elements$Style$Internal$Render$Property$flexWidth, len, paddingAdjustment),
										attrs);
								case 'GoLeft':
									var _n67 = _n65.a;
									return _Utils_ap(
										A2(mdgriffith$style_elements$Style$Internal$Render$Property$flexWidth, len, paddingAdjustment),
										attrs);
								default:
									break _n65$2;
							}
						} else {
							break _n65$2;
						}
					}
					return A2(
						elm$core$List$cons,
						_Utils_Tuple2(
							'width',
							A2(mdgriffith$style_elements$Style$Internal$Render$Value$parentAdjustedLength, len, paddingAdjustment)),
						attrs);
				} else {
					return A2(
						elm$core$List$cons,
						_Utils_Tuple2(
							'width',
							mdgriffith$style_elements$Style$Internal$Render$Value$length(len)),
						attrs);
				}
			}
		};
		var vertical = function (attrs) {
			var _n57 = elem.vertical;
			if (_n57.$ === 'Nothing') {
				return attrs;
			} else {
				var align = _n57.a;
				if (elem.inline && _Utils_eq(elType, mdgriffith$style_elements$Element$Internal$Render$Single)) {
					return attrs;
				} else {
					if (elem.inline) {
						return attrs;
					} else {
						if (!_Utils_eq(elem.frame, elm$core$Maybe$Nothing)) {
							switch (align.$) {
								case 'Top':
									return A2(
										elm$core$List$cons,
										_Utils_Tuple2('top', '0'),
										attrs);
								case 'Bottom':
									return A2(
										elm$core$List$cons,
										_Utils_Tuple2('bottom', '0'),
										attrs);
								case 'VerticalCenter':
									return attrs;
								default:
									return attrs;
							}
						} else {
							if (parent.$ === 'Nothing') {
								return attrs;
							} else {
								var parentEl = parent.a;
								var _n60 = parentEl.layout;
								if (_n60.$ === 'FlexLayout') {
									var dir = _n60.a;
									var _n61 = A2(mdgriffith$style_elements$Element$Internal$Render$flexboxVerticalIndividualAlignment, dir, align);
									if (_n61.$ === 'Nothing') {
										return attrs;
									} else {
										var a = _n61.a;
										return A2(elm$core$List$cons, a, attrs);
									}
								} else {
									return attrs;
								}
							}
						}
					}
				}
			}
		};
		var shrink = function (attrs) {
			var _n41 = elem.shrink;
			if (_n41.$ === 'Just') {
				var i = _n41.a;
				return A2(
					elm$core$List$cons,
					_Utils_Tuple2(
						'flex-shrink',
						elm$core$String$fromInt(i)),
					attrs);
			} else {
				if (parent.$ === 'Nothing') {
					return attrs;
				} else {
					var parentEl = parent.a;
					var verticalOverflow = function () {
						var _n53 = elem.overflow;
						if (_n53.$ === 'Just') {
							switch (_n53.a.$) {
								case 'XAxis':
									var _n54 = _n53.a;
									return false;
								case 'YAxis':
									var _n55 = _n53.a;
									return true;
								default:
									var _n56 = _n53.a;
									return true;
							}
						} else {
							return false;
						}
					}();
					var isVertical = function (dir) {
						switch (dir.$) {
							case 'Up':
								return true;
							case 'Down':
								return true;
							default:
								return false;
						}
					};
					var isPx = function (x) {
						if ((x.$ === 'Just') && (x.a.$ === 'Px')) {
							return true;
						} else {
							return false;
						}
					};
					var isPercent = function (x) {
						if ((x.$ === 'Just') && (x.a.$ === 'Percent')) {
							return true;
						} else {
							return false;
						}
					};
					var isHorizontal = function (dir) {
						switch (dir.$) {
							case 'GoRight':
								return true;
							case 'GoLeft':
								return true;
							default:
								return false;
						}
					};
					var horizontalOverflow = function () {
						var _n45 = elem.overflow;
						if (_n45.$ === 'Just') {
							switch (_n45.a.$) {
								case 'XAxis':
									var _n46 = _n45.a;
									return true;
								case 'YAxis':
									var _n47 = _n45.a;
									return false;
								default:
									var _n48 = _n45.a;
									return true;
							}
						} else {
							return false;
						}
					}();
					var _n43 = parentEl.layout;
					if (_n43.$ === 'FlexLayout') {
						var dir = _n43.a;
						if (isHorizontal(dir) && isPx(elem.width)) {
							return A2(
								elm$core$List$cons,
								_Utils_Tuple2('flex-shrink', '0'),
								attrs);
						} else {
							if (isHorizontal(dir) && isPercent(elem.width)) {
								return A2(
									elm$core$List$cons,
									_Utils_Tuple2('flex-shrink', '0'),
									attrs);
							} else {
								if (isHorizontal(dir) && (!_Utils_eq(elem.width, elm$core$Maybe$Nothing))) {
									return A2(
										elm$core$List$cons,
										_Utils_Tuple2('flex-shrink', '1'),
										attrs);
								} else {
									if (isHorizontal(dir) && horizontalOverflow) {
										return A2(
											elm$core$List$cons,
											_Utils_Tuple2('flex-shrink', '1'),
											attrs);
									} else {
										if (isVertical(dir) && isPx(elem.height)) {
											return A2(
												elm$core$List$cons,
												_Utils_Tuple2('flex-shrink', '0'),
												attrs);
										} else {
											if (isVertical(dir) && isPercent(elem.height)) {
												return A2(
													elm$core$List$cons,
													_Utils_Tuple2('flex-shrink', '0'),
													attrs);
											} else {
												if (isVertical(dir) && (!_Utils_eq(elem.height, elm$core$Maybe$Nothing))) {
													return A2(
														elm$core$List$cons,
														_Utils_Tuple2('flex-shrink', '1'),
														attrs);
												} else {
													if (isVertical(dir) && verticalOverflow) {
														return A2(
															elm$core$List$cons,
															_Utils_Tuple2('flex-shrink', '1'),
															attrs);
													} else {
														if (isHorizontal(dir) && _Utils_eq(elem.width, elm$core$Maybe$Nothing)) {
															return A2(
																elm$core$List$cons,
																_Utils_Tuple2('flex-shrink', '1'),
																attrs);
														} else {
															if (isVertical(dir) && _Utils_eq(elem.height, elm$core$Maybe$Nothing)) {
																if (elType.$ === 'Single') {
																	return A2(
																		elm$core$List$cons,
																		_Utils_Tuple2('flex-shrink', '1'),
																		attrs);
																} else {
																	var elLayout = elType.a;
																	return A2(
																		elm$core$List$cons,
																		_Utils_Tuple2('flex-shrink', '0'),
																		attrs);
																}
															} else {
																return A2(
																	elm$core$List$cons,
																	_Utils_Tuple2('flex-shrink', '0'),
																	attrs);
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}
					} else {
						return attrs;
					}
				}
			}
		};
		var position = function (attrs) {
			return _Utils_ap(
				A2(
					mdgriffith$style_elements$Element$Internal$Render$calcPosition,
					A2(elm$core$Maybe$withDefault, mdgriffith$style_elements$Element$Internal$Model$Relative, elem.frame),
					elem.positioned),
				attrs);
		};
		var passthrough = function (attrs) {
			var _n40 = elem.pointerevents;
			if (_n40.$ === 'Nothing') {
				return attrs;
			} else {
				if (!_n40.a) {
					return A2(
						elm$core$List$cons,
						_Utils_Tuple2('pointer-events', 'none'),
						attrs);
				} else {
					return A2(
						elm$core$List$cons,
						_Utils_Tuple2('pointer-events', 'auto'),
						attrs);
				}
			}
		};
		var padding = function (attrs) {
			var paddings = mdgriffith$style_elements$Element$Internal$Render$renderPadding(elem.padding);
			return (elm$core$List$length(paddings) > 0) ? _Utils_ap(paddings, attrs) : attrs;
		};
		var overflow = function (attrs) {
			var _n38 = elem.overflow;
			if (_n38.$ === 'Nothing') {
				return attrs;
			} else {
				var o = _n38.a;
				switch (o.$) {
					case 'XAxis':
						return A2(
							elm$core$List$cons,
							_Utils_Tuple2('overflow-x', 'auto'),
							attrs);
					case 'YAxis':
						return A2(
							elm$core$List$cons,
							_Utils_Tuple2('overflow-y', 'auto'),
							attrs);
					default:
						return A2(
							elm$core$List$cons,
							_Utils_Tuple2('overflow', 'auto'),
							attrs);
				}
			}
		};
		var opacity = function (attrs) {
			var _n37 = elem.opacity;
			if (_n37.$ === 'Nothing') {
				return attrs;
			} else {
				var o = _n37.a;
				return A2(
					elm$core$List$cons,
					_Utils_Tuple2(
						'opacity',
						elm$core$String$fromFloat(o)),
					attrs);
			}
		};
		var layout = function (attrs) {
			if (elType.$ === 'Single') {
				return elem.inline ? A2(
					elm$core$List$cons,
					_Utils_Tuple2('display', 'inline'),
					attrs) : A2(
					elm$core$List$cons,
					_Utils_Tuple2('display', 'block'),
					attrs);
			} else {
				var lay = elType.a;
				return _Utils_ap(
					A2(
						mdgriffith$style_elements$Style$Internal$Render$Property$layout,
						elem.inline,
						A3(mdgriffith$style_elements$Element$Internal$Render$alignLayout, elem.horizontal, elem.vertical, lay)),
					attrs);
			}
		};
		var horizontal = function (attrs) {
			var _n28 = elem.horizontal;
			if (_n28.$ === 'Nothing') {
				return attrs;
			} else {
				var align = _n28.a;
				if (elem.inline && _Utils_eq(elType, mdgriffith$style_elements$Element$Internal$Render$Single)) {
					switch (align.$) {
						case 'Left':
							return A2(
								elm$core$List$cons,
								_Utils_Tuple2('z-index', '1'),
								A2(
									elm$core$List$cons,
									_Utils_Tuple2('float', 'left'),
									attrs));
						case 'Right':
							return A2(
								elm$core$List$cons,
								_Utils_Tuple2('z-index', '1'),
								A2(
									elm$core$List$cons,
									_Utils_Tuple2('float', 'right'),
									attrs));
						case 'Center':
							return attrs;
						default:
							return attrs;
					}
				} else {
					if (elem.inline) {
						return attrs;
					} else {
						if (!_Utils_eq(elem.frame, elm$core$Maybe$Nothing)) {
							switch (align.$) {
								case 'Left':
									return A2(
										elm$core$List$cons,
										_Utils_Tuple2('left', '0'),
										attrs);
								case 'Right':
									return A2(
										elm$core$List$cons,
										_Utils_Tuple2('right', '0'),
										attrs);
								case 'Center':
									return attrs;
								default:
									return attrs;
							}
						} else {
							if (elType.$ === 'LayoutElement') {
								return attrs;
							} else {
								if (parent.$ === 'Nothing') {
									return attrs;
								} else {
									var parentEl = parent.a;
									var _n33 = parentEl.layout;
									switch (_n33.$) {
										case 'TextLayout':
											switch (align.$) {
												case 'Left':
													return A2(
														elm$core$List$cons,
														_Utils_Tuple2('z-index', '1'),
														A2(
															elm$core$List$cons,
															_Utils_Tuple2('float', 'left'),
															attrs));
												case 'Right':
													return A2(
														elm$core$List$cons,
														_Utils_Tuple2('z-index', '1'),
														A2(
															elm$core$List$cons,
															_Utils_Tuple2('float', 'right'),
															attrs));
												case 'Center':
													return attrs;
												default:
													return attrs;
											}
										case 'FlexLayout':
											var dir = _n33.a;
											var _n35 = A2(mdgriffith$style_elements$Element$Internal$Render$flexboxHorizontalIndividualAlignment, dir, align);
											if (_n35.$ === 'Nothing') {
												return attrs;
											} else {
												var a = _n35.a;
												return A2(elm$core$List$cons, a, attrs);
											}
										default:
											return attrs;
									}
								}
							}
						}
					}
				}
			}
		};
		var height = function (attrs) {
			var _n19 = elem.height;
			if (_n19.$ === 'Nothing') {
				return attrs;
			} else {
				var len = _n19.a;
				if (parent.$ === 'Just') {
					var parentEl = parent.a;
					var hundredPercentOrFill = function (x) {
						switch (x.$) {
							case 'Percent':
								var p = x.a;
								return p === 100;
							case 'Fill':
								return true;
							case 'Calc':
								var perc = x.a;
								return perc === 100;
							default:
								return false;
						}
					};
					var _n21 = A2(
						elm$core$Maybe$withDefault,
						A4(mdgriffith$style_elements$Style$Internal$Model$Box, 0, 0, 0, 0),
						parentEl.parentSpecifiedSpacing);
					var topPad = _n21.a;
					var bottomPad = _n21.c;
					var paddingAdjustment = (topPad + bottomPad) / 2;
					var _n22 = parentEl.layout;
					if (_n22.$ === 'FlexLayout') {
						switch (_n22.a.$) {
							case 'Down':
								var _n23 = _n22.a;
								return _Utils_ap(
									mdgriffith$style_elements$Style$Internal$Render$Property$flexHeight(len),
									attrs);
							case 'Up':
								var _n24 = _n22.a;
								return _Utils_ap(
									mdgriffith$style_elements$Style$Internal$Render$Property$flexHeight(len),
									attrs);
							case 'GoRight':
								var _n25 = _n22.a;
								return hundredPercentOrFill(len) ? A2(
									elm$core$List$cons,
									_Utils_Tuple2('height', 'auto'),
									attrs) : A2(
									elm$core$List$cons,
									_Utils_Tuple2(
										'height',
										A2(mdgriffith$style_elements$Style$Internal$Render$Value$parentAdjustedLength, len, paddingAdjustment)),
									attrs);
							default:
								var _n26 = _n22.a;
								return hundredPercentOrFill(len) ? A2(
									elm$core$List$cons,
									_Utils_Tuple2('height', 'auto'),
									attrs) : A2(
									elm$core$List$cons,
									_Utils_Tuple2(
										'height',
										A2(mdgriffith$style_elements$Style$Internal$Render$Value$parentAdjustedLength, len, paddingAdjustment)),
									attrs);
						}
					} else {
						return A2(
							elm$core$List$cons,
							_Utils_Tuple2(
								'height',
								A2(mdgriffith$style_elements$Style$Internal$Render$Value$parentAdjustedLength, len, paddingAdjustment)),
							attrs);
					}
				} else {
					return A2(
						elm$core$List$cons,
						_Utils_Tuple2(
							'height',
							mdgriffith$style_elements$Style$Internal$Render$Value$length(len)),
						attrs);
				}
			}
		};
		var gridPos = function (attrs) {
			var _n18 = elem.gridPosition;
			if (_n18.$ === 'Nothing') {
				return attrs;
			} else {
				var area = _n18.a;
				return A2(
					elm$core$List$cons,
					_Utils_Tuple2('grid-area', area),
					attrs);
			}
		};
		var defaults = _List_fromArray(
			[
				_Utils_Tuple2('box-sizing', 'border-box')
			]);
		var attributes = function () {
			if (maybeElemID.$ === 'Nothing') {
				return elem.attrs;
			} else {
				var elemID = maybeElemID.a;
				return (elm$core$List$length(elem.variations) > 0) ? A2(
					elm$core$List$cons,
					elm$html$Html$Attributes$classList(
						A2(stylesheet.variations, elemID, elem.variations)),
					elem.attrs) : A2(
					elm$core$List$cons,
					elm$html$Html$Attributes$class(
						stylesheet.style(elemID)),
					elem.attrs);
			}
		}();
		var adjustspacing = function (_n16) {
			var top = _n16.a;
			var right = _n16.b;
			var bottom = _n16.c;
			var left = _n16.d;
			var onScreen = function () {
				var _n14 = elem.frame;
				if ((_n14.$ === 'Just') && (_n14.a.$ === 'Screen')) {
					var _n15 = _n14.a;
					return true;
				} else {
					return false;
				}
			}();
			var halved = A4(mdgriffith$style_elements$Style$Internal$Model$Box, top / 2, right / 2, bottom / 2, left / 2);
			if (onScreen) {
				return A4(mdgriffith$style_elements$Style$Internal$Model$Box, 0, 0, 0, 0);
			} else {
				if (parent.$ === 'Nothing') {
					return A4(mdgriffith$style_elements$Style$Internal$Model$Box, top, right, bottom, left);
				} else {
					var parentEl = parent.a;
					var _n11 = parentEl.layout;
					if (_n11.$ === 'TextLayout') {
						var _n12 = elem.horizontal;
						if (_n12.$ === 'Nothing') {
							return (_Utils_eq(order, mdgriffith$style_elements$Element$Internal$Render$Last) || _Utils_eq(order, mdgriffith$style_elements$Element$Internal$Render$FirstAndLast)) ? A4(mdgriffith$style_elements$Style$Internal$Model$Box, 0, 0, 0, 0) : (elem.inline ? A4(mdgriffith$style_elements$Style$Internal$Model$Box, 0, right, 0, 0) : A4(mdgriffith$style_elements$Style$Internal$Model$Box, 0, 0, bottom, 0));
						} else {
							var align = _n12.a;
							if ((!elem.inline) && _Utils_eq(elem.frame, elm$core$Maybe$Nothing)) {
								switch (align.$) {
									case 'Left':
										return _Utils_eq(order, mdgriffith$style_elements$Element$Internal$Render$First) ? A4(mdgriffith$style_elements$Style$Internal$Model$Box, 0, right, bottom, 0) : (_Utils_eq(order, mdgriffith$style_elements$Element$Internal$Render$FirstAndLast) ? A4(mdgriffith$style_elements$Style$Internal$Model$Box, 0, right, 0, 0) : (_Utils_eq(order, mdgriffith$style_elements$Element$Internal$Render$Last) ? A4(mdgriffith$style_elements$Style$Internal$Model$Box, 0, right, 0, 0) : A4(mdgriffith$style_elements$Style$Internal$Model$Box, 0, right, bottom, 0)));
									case 'Right':
										return _Utils_eq(order, mdgriffith$style_elements$Element$Internal$Render$First) ? A4(mdgriffith$style_elements$Style$Internal$Model$Box, 0, 0, bottom, left) : (_Utils_eq(order, mdgriffith$style_elements$Element$Internal$Render$FirstAndLast) ? A4(mdgriffith$style_elements$Style$Internal$Model$Box, 0, 0, 0, left) : (_Utils_eq(order, mdgriffith$style_elements$Element$Internal$Render$Last) ? A4(mdgriffith$style_elements$Style$Internal$Model$Box, 0, 0, 0, left) : A4(mdgriffith$style_elements$Style$Internal$Model$Box, 0, 0, bottom, left)));
									default:
										return (_Utils_eq(order, mdgriffith$style_elements$Element$Internal$Render$Last) || _Utils_eq(order, mdgriffith$style_elements$Element$Internal$Render$FirstAndLast)) ? A4(mdgriffith$style_elements$Style$Internal$Model$Box, 0, 0, 0, 0) : A4(mdgriffith$style_elements$Style$Internal$Model$Box, 0, 0, bottom, 0);
								}
							} else {
								return A4(mdgriffith$style_elements$Style$Internal$Model$Box, top, right, bottom, left);
							}
						}
					} else {
						return halved;
					}
				}
			}
		};
		var spacing = function (attrs) {
			var _n9 = elem.margin;
			if (_n9.$ === 'Nothing') {
				return attrs;
			} else {
				var space = _n9.a;
				return A2(
					elm$core$List$cons,
					_Utils_Tuple2(
						'margin',
						mdgriffith$style_elements$Style$Internal$Render$Value$box(
							adjustspacing(space))),
					attrs);
			}
		};
		if (elem.hidden) {
			return A2(
				elm$core$List$cons,
				A2(elm$html$Html$Attributes$style, 'display', 'none'),
				attributes);
		} else {
			if (elem.expand) {
				var expandedProps = function () {
					if (parent.$ === 'Nothing') {
						return _List_fromArray(
							[
								_Utils_Tuple2('width', '100%'),
								_Utils_Tuple2('height', '100%'),
								_Utils_Tuple2('margin', '0')
							]);
					} else {
						var parentEl = parent.a;
						var _n2 = parentEl.layout;
						switch (_n2.$) {
							case 'TextLayout':
								var borders = elm$core$List$concat(
									_List_fromArray(
										[
											_Utils_eq(order, mdgriffith$style_elements$Element$Internal$Render$Last) ? _List_fromArray(
											[
												_Utils_Tuple2('border-top-right-radius', '0'),
												_Utils_Tuple2('border-top-left-radius', '0')
											]) : (_Utils_eq(order, mdgriffith$style_elements$Element$Internal$Render$First) ? _List_fromArray(
											[
												_Utils_Tuple2('border-bottom-right-radius', '0'),
												_Utils_Tuple2('border-bottom-left-radius', '0')
											]) : (_Utils_eq(order, mdgriffith$style_elements$Element$Internal$Render$FirstAndLast) ? _List_fromArray(
											[
												_Utils_Tuple2('border-top-right-radius', '0'),
												_Utils_Tuple2('border-top-left-radius', '0'),
												_Utils_Tuple2('border-bottom-right-radius', '0'),
												_Utils_Tuple2('border-bottom-left-radius', '0')
											]) : _List_Nil))
										]));
								var _n3 = parentEl.parentPadding;
								var top = _n3.a;
								var right = _n3.b;
								var bottom = _n3.c;
								var left = _n3.d;
								return _Utils_ap(
									_List_fromArray(
										[
											_Utils_Tuple2(
											'width',
											'calc(100% + ' + (elm$core$String$fromFloat(right + left) + 'px')),
											_Utils_Tuple2('margin', '0'),
											_Utils_Tuple2(
											'margin-left',
											elm$core$String$fromFloat((-1) * left) + 'px'),
											(_Utils_eq(order, mdgriffith$style_elements$Element$Internal$Render$First) || _Utils_eq(order, mdgriffith$style_elements$Element$Internal$Render$FirstAndLast)) ? _Utils_Tuple2(
											'margin-top',
											elm$core$String$fromFloat((-1) * top) + 'px') : _Utils_Tuple2('margin-top', '0'),
											(_Utils_eq(order, mdgriffith$style_elements$Element$Internal$Render$Last) || _Utils_eq(order, mdgriffith$style_elements$Element$Internal$Render$FirstAndLast)) ? _Utils_Tuple2(
											'margin-bottom',
											elm$core$String$fromFloat((-1) * bottom) + 'px') : _Utils_Tuple2('margin-bottom', '0'),
											_Utils_Tuple2(
											'padding',
											mdgriffith$style_elements$Style$Internal$Render$Value$box(
												A2(mdgriffith$style_elements$Element$Internal$Render$defaultPadding, elem.padding, parentEl.parentPadding)))
										]),
									borders);
							case 'FlexLayout':
								var dir = _n2.a;
								var flex = _n2.b;
								var _n4 = function () {
									var _n5 = parentEl.parentSpecifiedSpacing;
									if (_n5.$ === 'Nothing') {
										return A4(mdgriffith$style_elements$Style$Internal$Model$Box, 0, 0, 0, 0);
									} else {
										var p = _n5.a;
										return p;
									}
								}();
								var parentSpaceTop = _n4.a;
								var parentSpaceRight = _n4.b;
								var parentSpaceBottom = _n4.c;
								var parentSpaceLeft = _n4.d;
								var _n6 = parentEl.parentPadding;
								var top = _n6.a;
								var right = _n6.b;
								var bottom = _n6.c;
								var left = _n6.d;
								switch (dir.$) {
									case 'GoRight':
										return width(
											_List_fromArray(
												[
													_Utils_Tuple2(
													'height',
													'calc(100% + ' + (elm$core$String$fromFloat((top + bottom) - ((parentSpaceTop + parentSpaceBottom) / 2)) + 'px')),
													_Utils_Tuple2('margin', '0'),
													_Utils_Tuple2(
													'margin-top',
													elm$core$String$fromFloat(((-1) * top) + (parentSpaceTop / 2)) + 'px'),
													(_Utils_eq(order, mdgriffith$style_elements$Element$Internal$Render$First) || _Utils_eq(order, mdgriffith$style_elements$Element$Internal$Render$FirstAndLast)) ? _Utils_Tuple2(
													'margin-left',
													elm$core$String$fromFloat((-1) * left) + 'px') : _Utils_Tuple2(
													'margin-left',
													elm$core$String$fromFloat(parentSpaceLeft / 2) + 'px'),
													(_Utils_eq(order, mdgriffith$style_elements$Element$Internal$Render$Last) || _Utils_eq(order, mdgriffith$style_elements$Element$Internal$Render$FirstAndLast)) ? _Utils_Tuple2(
													'margin-right',
													elm$core$String$fromFloat((-1) * right) + 'px') : _Utils_Tuple2(
													'margin-right',
													elm$core$String$fromFloat(parentSpaceRight / 2) + 'px')
												]));
									case 'GoLeft':
										return width(
											_List_fromArray(
												[
													_Utils_Tuple2(
													'height',
													'calc(100% + ' + (elm$core$String$fromFloat((top + bottom) - ((parentSpaceTop + parentSpaceBottom) / 2)) + 'px')),
													_Utils_Tuple2('margin', '0'),
													_Utils_Tuple2(
													'margin-top',
													elm$core$String$fromFloat(((-1) * top) + (parentSpaceTop / 2)) + 'px'),
													(_Utils_eq(order, mdgriffith$style_elements$Element$Internal$Render$First) || _Utils_eq(order, mdgriffith$style_elements$Element$Internal$Render$FirstAndLast)) ? _Utils_Tuple2(
													'margin-right',
													elm$core$String$fromFloat((-1) * right) + 'px') : _Utils_Tuple2(
													'margin-right',
													elm$core$String$fromFloat(parentSpaceRight / 2) + 'px'),
													(_Utils_eq(order, mdgriffith$style_elements$Element$Internal$Render$Last) || _Utils_eq(order, mdgriffith$style_elements$Element$Internal$Render$FirstAndLast)) ? _Utils_Tuple2(
													'margin-left',
													elm$core$String$fromFloat((-1) * left) + 'px') : _Utils_Tuple2(
													'margin-left',
													elm$core$String$fromFloat(parentSpaceLeft / 2) + 'px')
												]));
									case 'Up':
										return height(
											_List_fromArray(
												[
													_Utils_Tuple2(
													'width',
													'calc(100% + ' + (elm$core$String$fromFloat((left + right) - ((parentSpaceLeft + parentSpaceRight) / 2)) + 'px')),
													_Utils_Tuple2('margin', '0'),
													_Utils_Tuple2(
													'margin-left',
													elm$core$String$fromFloat(((-1) * left) + (parentSpaceLeft / 2)) + 'px'),
													(_Utils_eq(order, mdgriffith$style_elements$Element$Internal$Render$First) || _Utils_eq(order, mdgriffith$style_elements$Element$Internal$Render$FirstAndLast)) ? _Utils_Tuple2(
													'margin-bottom',
													elm$core$String$fromFloat((-1) * top) + 'px') : _Utils_Tuple2(
													'margin-bottom',
													elm$core$String$fromFloat(parentSpaceBottom / 2) + 'px'),
													(_Utils_eq(order, mdgriffith$style_elements$Element$Internal$Render$Last) || _Utils_eq(order, mdgriffith$style_elements$Element$Internal$Render$FirstAndLast)) ? _Utils_Tuple2(
													'margin-top',
													elm$core$String$fromFloat((-1) * bottom) + 'px') : _Utils_Tuple2(
													'margin-top',
													elm$core$String$fromFloat(parentSpaceTop / 2) + 'px')
												]));
									default:
										return height(
											_List_fromArray(
												[
													_Utils_Tuple2(
													'width',
													'calc(100% + ' + (elm$core$String$fromFloat((left + right) - ((parentSpaceLeft + parentSpaceRight) / 2)) + 'px')),
													_Utils_Tuple2('margin', '0'),
													_Utils_Tuple2(
													'margin-left',
													elm$core$String$fromFloat(((-1) * left) + (parentSpaceLeft / 2)) + 'px'),
													(_Utils_eq(order, mdgriffith$style_elements$Element$Internal$Render$First) || _Utils_eq(order, mdgriffith$style_elements$Element$Internal$Render$FirstAndLast)) ? _Utils_Tuple2(
													'margin-top',
													elm$core$String$fromFloat((-1) * top) + 'px') : _Utils_Tuple2(
													'margin-top',
													elm$core$String$fromFloat(parentSpaceTop / 2) + 'px'),
													(_Utils_eq(order, mdgriffith$style_elements$Element$Internal$Render$Last) || _Utils_eq(order, mdgriffith$style_elements$Element$Internal$Render$FirstAndLast)) ? _Utils_Tuple2(
													'margin-bottom',
													elm$core$String$fromFloat((-1) * bottom) + 'px') : _Utils_Tuple2(
													'margin-bottom',
													elm$core$String$fromFloat(parentSpaceBottom / 2) + 'px')
												]));
								}
							default:
								return _List_Nil;
						}
					}
				}();
				return _Utils_ap(
					A2(
						elm$core$List$map,
						function (_n0) {
							var name = _n0.a;
							var val = _n0.b;
							return A2(elm$html$Html$Attributes$style, name, val);
						},
						_Utils_ap(
							defaults,
							A3(
								elm$core$Basics$composeL,
								A2(
									elm$core$Basics$composeL,
									A2(
										elm$core$Basics$composeL,
										A2(
											elm$core$Basics$composeL,
											A2(
												elm$core$Basics$composeL,
												A2(
													elm$core$Basics$composeL,
													A2(
														elm$core$Basics$composeL,
														A2(elm$core$Basics$composeL, passthrough, gridPos),
														layout),
													spacing),
												opacity),
											shrink),
										padding),
									position),
								overflow,
								expandedProps))),
					attributes);
			} else {
				return _Utils_ap(
					A2(
						elm$core$List$map,
						function (_n8) {
							var name = _n8.a;
							var val = _n8.b;
							return A2(elm$html$Html$Attributes$style, name, val);
						},
						A3(
							elm$core$Basics$composeL,
							A2(
								elm$core$Basics$composeL,
								A2(
									elm$core$Basics$composeL,
									A2(
										elm$core$Basics$composeL,
										A2(
											elm$core$Basics$composeL,
											A2(
												elm$core$Basics$composeL,
												A2(
													elm$core$Basics$composeL,
													A2(
														elm$core$Basics$composeL,
														A2(
															elm$core$Basics$composeL,
															A2(
																elm$core$Basics$composeL,
																A2(
																	elm$core$Basics$composeL,
																	A2(elm$core$Basics$composeL, passthrough, gridPos),
																	layout),
																spacing),
															opacity),
														shrink),
													width),
												height),
											padding),
										horizontal),
									vertical),
								position),
							overflow,
							defaults)),
					attributes);
			}
		}
	});
var mdgriffith$style_elements$Element$Internal$Render$renderSyleAttributes = function (styles) {
	return A2(
		elm$core$List$map,
		function (_n0) {
			var name = _n0.a;
			var val = _n0.b;
			return A2(elm$html$Html$Attributes$style, name, val);
		},
		styles);
};
var mdgriffith$style_elements$Element$Internal$Render$spacingToMargin = function (attrs) {
	var spaceToMarg = function (a) {
		if (a.$ === 'Spacing') {
			var x = a.a;
			var y = a.b;
			return A4(mdgriffith$style_elements$Element$Internal$Model$Margin, y, x, y, x);
		} else {
			var other = a;
			return other;
		}
	};
	return A2(elm$core$List$map, spaceToMarg, attrs);
};
var mdgriffith$style_elements$Element$Internal$Render$renderElement = F4(
	function (parent, stylesheet, order, elm) {
		switch (elm.$) {
			case 'Empty':
				return elm$html$Html$text('');
			case 'Raw':
				var html = elm.a;
				return html;
			case 'Spacer':
				var x = elm.a;
				var forSpacing = function (posAttr) {
					if (posAttr.$ === 'Spacing') {
						var spaceX = posAttr.a;
						var spaceY = posAttr.b;
						return elm$core$Maybe$Just(
							_Utils_Tuple2(spaceX, spaceY));
					} else {
						return elm$core$Maybe$Nothing;
					}
				};
				var _n1 = function () {
					if (parent.$ === 'Just') {
						var ctxt = parent.a;
						return A2(
							elm$core$Maybe$withDefault,
							A4(mdgriffith$style_elements$Style$Internal$Model$Box, 0, 0, 0, 0),
							ctxt.parentSpecifiedSpacing);
					} else {
						return A4(mdgriffith$style_elements$Style$Internal$Model$Box, 0, 0, 0, 0);
					}
				}();
				var spacingX = _n1.a;
				var spacingY = _n1.b;
				var inline = _List_fromArray(
					[
						_Utils_Tuple2(
						'width',
						elm$core$String$fromFloat(x * spacingX) + 'px'),
						_Utils_Tuple2(
						'height',
						elm$core$String$fromFloat(x * spacingY) + 'px'),
						_Utils_Tuple2('visibility', 'hidden')
					]);
				return A2(
					elm$html$Html$div,
					mdgriffith$style_elements$Element$Internal$Render$renderSyleAttributes(inline),
					_List_Nil);
			case 'Text':
				var decoration = elm.a.decoration;
				var inline = elm.a.inline;
				var str = elm.b;
				var attrs = inline ? mdgriffith$style_elements$Element$Internal$Render$renderSyleAttributes(
					_List_fromArray(
						[
							_Utils_Tuple2('display', 'inline')
						])) : mdgriffith$style_elements$Element$Internal$Render$renderSyleAttributes(
					_List_fromArray(
						[
							_Utils_Tuple2('white-space', 'pre'),
							_Utils_Tuple2('text-overflow', 'ellipsis'),
							_Utils_Tuple2('overflow', 'hidden'),
							_Utils_Tuple2('display', 'block')
						]));
				switch (decoration.$) {
					case 'NoDecoration':
						return A2(
							elm$html$Html$span,
							A2(
								elm$core$List$cons,
								elm$html$Html$Attributes$class('el'),
								attrs),
							_List_fromArray(
								[
									elm$html$Html$text(str)
								]));
					case 'RawText':
						return elm$html$Html$text(str);
					case 'Bold':
						return A2(
							elm$html$Html$strong,
							A2(
								elm$core$List$cons,
								elm$html$Html$Attributes$class('el'),
								attrs),
							_List_fromArray(
								[
									elm$html$Html$text(str)
								]));
					case 'Italic':
						return A2(
							elm$html$Html$em,
							A2(
								elm$core$List$cons,
								elm$html$Html$Attributes$class('el'),
								attrs),
							_List_fromArray(
								[
									elm$html$Html$text(str)
								]));
					case 'Underline':
						return A2(
							elm$html$Html$u,
							A2(
								elm$core$List$cons,
								elm$html$Html$Attributes$class('el'),
								attrs),
							_List_fromArray(
								[
									elm$html$Html$text(str)
								]));
					case 'Strike':
						return A2(
							elm$html$Html$s,
							A2(
								elm$core$List$cons,
								elm$html$Html$Attributes$class('el'),
								attrs),
							_List_fromArray(
								[
									elm$html$Html$text(str)
								]));
					case 'Super':
						return A2(
							elm$html$Html$sup,
							A2(
								elm$core$List$cons,
								elm$html$Html$Attributes$class('el'),
								attrs),
							_List_fromArray(
								[
									elm$html$Html$text(str)
								]));
					default:
						return A2(
							elm$html$Html$sub,
							A2(
								elm$core$List$cons,
								elm$html$Html$Attributes$class('el'),
								attrs),
							_List_fromArray(
								[
									elm$html$Html$text(str)
								]));
				}
			case 'Element':
				var node = elm.a.node;
				var style = elm.a.style;
				var attrs = elm.a.attrs;
				var child = elm.a.child;
				var absolutelyPositioned = elm.a.absolutelyPositioned;
				var parentTextLayout = function (layout) {
					if (layout.$ === 'TextLayout') {
						return true;
					} else {
						return false;
					}
				};
				var childHtml = function () {
					if (absolutelyPositioned.$ === 'Nothing') {
						return _List_fromArray(
							[
								A4(mdgriffith$style_elements$Element$Internal$Render$renderElement, elm$core$Maybe$Nothing, stylesheet, mdgriffith$style_elements$Element$Internal$Render$FirstAndLast, child)
							]);
					} else {
						var absol = absolutelyPositioned.a;
						return A2(
							elm$core$List$map,
							A3(mdgriffith$style_elements$Element$Internal$Render$renderElement, elm$core$Maybe$Nothing, stylesheet, mdgriffith$style_elements$Element$Internal$Render$FirstAndLast),
							A2(elm$core$List$cons, child, absol));
					}
				}();
				var attributes = function () {
					if (parent.$ === 'Nothing') {
						return mdgriffith$style_elements$Element$Internal$Render$spacingToMargin(attrs);
					} else {
						var ctxt = parent.a;
						var _n6 = ctxt.parentSpecifiedSpacing;
						if (_n6.$ === 'Nothing') {
							return (parentTextLayout(ctxt.layout) || A2(
								elm$core$List$any,
								elm$core$Basics$eq(mdgriffith$style_elements$Element$Internal$Model$Inline),
								attrs)) ? mdgriffith$style_elements$Element$Internal$Render$spacingToMargin(attrs) : attrs;
						} else {
							var _n7 = _n6.a;
							var top = _n7.a;
							var right = _n7.b;
							var bottom = _n7.c;
							var left = _n7.d;
							return (parentTextLayout(ctxt.layout) || A2(
								elm$core$List$any,
								elm$core$Basics$eq(mdgriffith$style_elements$Element$Internal$Model$Inline),
								attrs)) ? A2(
								elm$core$List$cons,
								A4(mdgriffith$style_elements$Element$Internal$Model$Margin, top, right, bottom, left),
								mdgriffith$style_elements$Element$Internal$Render$spacingToMargin(attrs)) : A2(
								elm$core$List$cons,
								A4(mdgriffith$style_elements$Element$Internal$Model$Margin, top, right, bottom, left),
								attrs);
						}
					}
				}();
				var htmlAttrs = A6(
					mdgriffith$style_elements$Element$Internal$Render$renderAttributes,
					mdgriffith$style_elements$Element$Internal$Render$Single,
					order,
					style,
					parent,
					stylesheet,
					mdgriffith$style_elements$Element$Internal$Render$gather(attributes));
				return A3(
					elm$html$Html$node,
					node,
					A2(
						elm$core$List$cons,
						elm$html$Html$Attributes$class('el'),
						htmlAttrs),
					childHtml);
			default:
				var node = elm.a.node;
				var layout = elm.a.layout;
				var style = elm.a.style;
				var attrs = elm.a.attrs;
				var children = elm.a.children;
				var absolutelyPositioned = elm.a.absolutelyPositioned;
				var isFlexbox = function (layoutType) {
					if (layoutType.$ === 'FlexLayout') {
						return true;
					} else {
						return false;
					}
				};
				var forPadding = function (posAttr) {
					switch (posAttr.$) {
						case 'Padding':
							var t = posAttr.a;
							var r = posAttr.b;
							var b = posAttr.c;
							var l = posAttr.d;
							return elm$core$Maybe$Just(
								A2(
									mdgriffith$style_elements$Element$Internal$Render$defaultPadding,
									A4(mdgriffith$style_elements$Style$Internal$Model$Box, t, r, b, l),
									A4(mdgriffith$style_elements$Style$Internal$Model$Box, 0, 0, 0, 0)));
						case 'PhantomPadding':
							var t = posAttr.a;
							var r = posAttr.b;
							var b = posAttr.c;
							var l = posAttr.d;
							return elm$core$Maybe$Just(
								A4(mdgriffith$style_elements$Style$Internal$Model$Box, t, r, b, l));
						default:
							return elm$core$Maybe$Nothing;
					}
				};
				var findSpacing = function (posAttr) {
					if (posAttr.$ === 'Spacing') {
						var x = posAttr.a;
						var y = posAttr.b;
						return elm$core$Maybe$Just(
							A4(mdgriffith$style_elements$Style$Internal$Model$Box, y, x, y, x));
					} else {
						return elm$core$Maybe$Nothing;
					}
				};
				var forSpacing = A2(
					elm$core$Basics$composeL,
					function (x) {
						return !_Utils_eq(x, elm$core$Maybe$Nothing);
					},
					findSpacing);
				var clearfix = function (allAttrs) {
					if (layout.$ === 'TextLayout') {
						var fix = layout.a;
						return fix ? A2(
							elm$core$List$cons,
							elm$html$Html$Attributes$class('clearfix'),
							allAttrs) : allAttrs;
					} else {
						return allAttrs;
					}
				};
				var attributes = function () {
					if (parent.$ === 'Nothing') {
						return attrs;
					} else {
						var ctxt = parent.a;
						var _n18 = ctxt.parentSpecifiedSpacing;
						if (_n18.$ === 'Nothing') {
							return attrs;
						} else {
							var _n19 = _n18.a;
							var t = _n19.a;
							var r = _n19.b;
							var b = _n19.c;
							var l = _n19.d;
							return A2(
								elm$core$List$cons,
								A4(mdgriffith$style_elements$Element$Internal$Model$Margin, t, r, b, l),
								attrs);
						}
					}
				}();
				var htmlAttrs = clearfix(
					A6(
						mdgriffith$style_elements$Element$Internal$Render$renderAttributes,
						mdgriffith$style_elements$Element$Internal$Render$LayoutElement(layout),
						order,
						style,
						parent,
						stylesheet,
						mdgriffith$style_elements$Element$Internal$Render$gather(attributes)));
				var padding = function () {
					var _n16 = elm$core$List$head(
						A2(elm$core$List$filterMap, forPadding, attributes));
					if (_n16.$ === 'Nothing') {
						return A4(mdgriffith$style_elements$Style$Internal$Model$Box, 0, 0, 0, 0);
					} else {
						var pad = _n16.a;
						return pad;
					}
				}();
				var inherit = {
					layout: layout,
					parentPadding: padding,
					parentSpecifiedSpacing: elm$core$List$head(
						A2(elm$core$List$filterMap, findSpacing, attrs))
				};
				var adjacentFlexboxCorrection = function (htmlNode) {
					if (parent.$ === 'Nothing') {
						return htmlNode;
					} else {
						var p = parent.a;
						return (isFlexbox(p.layout) && isFlexbox(layout)) ? htmlNode : htmlNode;
					}
				};
				var _n10 = A2(elm$core$List$partition, forSpacing, attrs);
				var spacingAttr = _n10.a;
				var _n11 = A2(
					elm$core$List$partition,
					function (attr) {
						return _Utils_eq(
							attr,
							mdgriffith$style_elements$Element$Internal$Model$HAlign(mdgriffith$style_elements$Element$Internal$Model$Center)) || _Utils_eq(
							attr,
							mdgriffith$style_elements$Element$Internal$Model$VAlign(mdgriffith$style_elements$Element$Internal$Model$VerticalCenter));
					},
					attrs);
				var centeredProps = _n11.a;
				var others = _n11.b;
				if (children.$ === 'Normal') {
					var childList = children.a;
					var childHtml = A2(
						elm$core$List$indexedMap,
						F2(
							function (i, child) {
								return A4(
									mdgriffith$style_elements$Element$Internal$Render$renderElement,
									elm$core$Maybe$Just(inherit),
									stylesheet,
									A2(mdgriffith$style_elements$Element$Internal$Render$detectOrder, childList, i),
									child);
							}),
						childList);
					var allChildren = function () {
						if (absolutelyPositioned.$ === 'Nothing') {
							return childHtml;
						} else {
							var absol = absolutelyPositioned.a;
							return _Utils_ap(
								childHtml,
								A2(
									elm$core$List$map,
									A3(mdgriffith$style_elements$Element$Internal$Render$renderElement, elm$core$Maybe$Nothing, stylesheet, mdgriffith$style_elements$Element$Internal$Render$FirstAndLast),
									absol));
						}
					}();
					return adjacentFlexboxCorrection(
						A3(
							elm$html$Html$node,
							node,
							A2(
								elm$core$List$cons,
								elm$html$Html$Attributes$class('el'),
								htmlAttrs),
							allChildren));
				} else {
					var keyed = children.a;
					var childHtml = A2(
						elm$core$List$indexedMap,
						F2(
							function (i, _n14) {
								var key = _n14.a;
								var child = _n14.b;
								return _Utils_Tuple2(
									key,
									A4(
										mdgriffith$style_elements$Element$Internal$Render$renderElement,
										elm$core$Maybe$Just(inherit),
										stylesheet,
										A2(mdgriffith$style_elements$Element$Internal$Render$detectOrder, keyed, i),
										child));
							}),
						keyed);
					return adjacentFlexboxCorrection(
						A3(
							elm$html$Html$Keyed$node,
							node,
							A2(
								elm$core$List$cons,
								elm$html$Html$Attributes$class('el'),
								htmlAttrs),
							childHtml));
				}
		}
	});
var mdgriffith$style_elements$Element$Internal$Render$render = F2(
	function (stylesheet, elm) {
		var _n0 = mdgriffith$style_elements$Element$Internal$Adjustments$apply(elm);
		var adjusted = _n0.a;
		var onScreen = _n0.b;
		var fixedScreenElements = function () {
			if (onScreen.$ === 'Nothing') {
				return _List_Nil;
			} else {
				var screenEls = onScreen.a;
				return A2(
					elm$core$List$map,
					A3(mdgriffith$style_elements$Element$Internal$Render$renderElement, elm$core$Maybe$Nothing, stylesheet, mdgriffith$style_elements$Element$Internal$Render$FirstAndLast),
					screenEls);
			}
		}();
		return A2(
			elm$core$List$cons,
			A4(mdgriffith$style_elements$Element$Internal$Render$renderElement, elm$core$Maybe$Nothing, stylesheet, mdgriffith$style_elements$Element$Internal$Render$FirstAndLast, adjusted),
			fixedScreenElements);
	});
var mdgriffith$style_elements$Element$Internal$Render$viewport = F2(
	function (stylesheet, elm) {
		return A2(
			elm$html$Html$div,
			_List_fromArray(
				[
					elm$html$Html$Attributes$class('style-elements'),
					A2(elm$html$Html$Attributes$style, 'width', '100%'),
					A2(elm$html$Html$Attributes$style, 'height', '100%')
				]),
			_Utils_ap(
				A2(mdgriffith$style_elements$Element$Internal$Render$embed, true, stylesheet),
				A2(mdgriffith$style_elements$Element$Internal$Render$render, stylesheet, elm)));
	});
var mdgriffith$style_elements$Element$viewport = mdgriffith$style_elements$Element$Internal$Render$viewport;
var mdgriffith$style_elements$Element$Attributes$center = mdgriffith$style_elements$Element$Internal$Model$HAlign(mdgriffith$style_elements$Element$Internal$Model$Center);
var mdgriffith$style_elements$Style$Internal$Model$Fill = function (a) {
	return {$: 'Fill', a: a};
};
var mdgriffith$style_elements$Element$Attributes$fill = mdgriffith$style_elements$Style$Internal$Model$Fill(1);
var mdgriffith$style_elements$Element$Attributes$maxWidth = function (len) {
	return mdgriffith$style_elements$Element$Internal$Model$Attr(
		A2(
			elm$html$Html$Attributes$style,
			'max-width',
			mdgriffith$style_elements$Style$Internal$Render$Value$length(len)));
};
var mdgriffith$style_elements$Element$Attributes$minHeight = function (len) {
	return mdgriffith$style_elements$Element$Internal$Model$Attr(
		A2(
			elm$html$Html$Attributes$style,
			'min-height',
			mdgriffith$style_elements$Style$Internal$Render$Value$length(len)));
};
var mdgriffith$style_elements$Element$Attributes$padding = function (x) {
	return A4(
		mdgriffith$style_elements$Element$Internal$Model$Padding,
		elm$core$Maybe$Just(x),
		elm$core$Maybe$Just(x),
		elm$core$Maybe$Just(x),
		elm$core$Maybe$Just(x));
};
var mdgriffith$style_elements$Style$Unguarded = {$: 'Unguarded'};
var mdgriffith$style_elements$Style$Internal$Find$style = F2(
	function (_class, elements) {
		var find = function (el) {
			if (el.$ === 'Style') {
				var cls = el.a;
				var name = el.b;
				return _Utils_eq(cls, _class) ? elm$core$Maybe$Just(name) : elm$core$Maybe$Nothing;
			} else {
				return elm$core$Maybe$Nothing;
			}
		};
		var found = elm$core$List$head(
			A2(elm$core$List$filterMap, find, elements));
		if (found.$ === 'Nothing') {
			return '';
		} else {
			var cls = found.a;
			return cls;
		}
	});
var mdgriffith$style_elements$Style$Internal$Find$variation = F3(
	function (_class, vary, elements) {
		var find = function (el) {
			if (el.$ === 'Variation') {
				var cls = el.a;
				var _var = el.b;
				var name = el.c;
				return (_Utils_eq(_class, cls) && _Utils_eq(_var, vary)) ? elm$core$Maybe$Just(name) : elm$core$Maybe$Nothing;
			} else {
				return elm$core$Maybe$Nothing;
			}
		};
		var found = elm$core$List$head(
			A2(elm$core$List$filterMap, find, elements));
		if (found.$ === 'Nothing') {
			return '';
		} else {
			var cls = found.a;
			return cls;
		}
	});
var mdgriffith$style_elements$Style$prepareSheet = function (_n0) {
	var css = _n0.a.css;
	var findable = _n0.a.findable;
	var variations = F2(
		function (_class, vs) {
			var varys = A2(
				elm$core$List$map,
				function (cls) {
					return _Utils_Tuple2(cls, true);
				},
				A2(
					elm$core$List$map,
					A2(
						elm$core$Basics$composeL,
						function (vary) {
							return A3(mdgriffith$style_elements$Style$Internal$Find$variation, _class, vary, findable);
						},
						elm$core$Tuple$first),
					A2(elm$core$List$filter, elm$core$Tuple$second, vs)));
			var parent = A2(mdgriffith$style_elements$Style$Internal$Find$style, _class, findable);
			return A2(
				elm$core$List$cons,
				_Utils_Tuple2(parent, true),
				varys);
		});
	return {
		css: css,
		style: function (_class) {
			return A2(mdgriffith$style_elements$Style$Internal$Find$style, _class, findable);
		},
		variations: F2(
			function (_class, varys) {
				return A2(variations, _class, varys);
			})
	};
};
var mdgriffith$style_elements$Style$Internal$Intermediate$Rendered = function (a) {
	return {$: 'Rendered', a: a};
};
var mdgriffith$style_elements$Style$Internal$Selector$getFindable = function (find) {
	getFindable:
	while (true) {
		switch (find.$) {
			case 'Select':
				var findable = find.b;
				return _List_fromArray(
					[findable]);
			case 'SelectChild':
				var selector = find.a;
				var $temp$find = selector;
				find = $temp$find;
				continue getFindable;
			case 'Stack':
				var selectors = find.a;
				return A2(
					elm$core$Maybe$withDefault,
					_List_Nil,
					A2(
						elm$core$Maybe$map,
						function (x) {
							return _List_fromArray(
								[x]);
						},
						elm$core$List$head(
							elm$core$List$reverse(
								A2(elm$core$List$concatMap, mdgriffith$style_elements$Style$Internal$Selector$getFindable, selectors)))));
			default:
				return _List_Nil;
		}
	}
};
var mdgriffith$style_elements$Style$Internal$Intermediate$asFindable = function (intermediate) {
	var findableProp = function (prop) {
		switch (prop.$) {
			case 'SubClass':
				var cls = prop.a;
				return mdgriffith$style_elements$Style$Internal$Intermediate$asFindable(cls);
			case 'PropsAndSub':
				var cls = prop.b;
				return mdgriffith$style_elements$Style$Internal$Intermediate$asFindable(cls);
			default:
				return _List_Nil;
		}
	};
	if (intermediate.$ === 'Class') {
		var classRule = intermediate.a;
		return _Utils_ap(
			mdgriffith$style_elements$Style$Internal$Selector$getFindable(classRule.selector),
			A2(elm$core$List$concatMap, findableProp, classRule.props));
	} else {
		return _List_Nil;
	}
};
var mdgriffith$style_elements$Style$Internal$Intermediate$RenderableClass = F2(
	function (a, b) {
		return {$: 'RenderableClass', a: a, b: b};
	});
var mdgriffith$style_elements$Style$Internal$Intermediate$RenderableFree = function (a) {
	return {$: 'RenderableFree', a: a};
};
var mdgriffith$style_elements$Style$Internal$Intermediate$RenderableMedia = F3(
	function (a, b, c) {
		return {$: 'RenderableMedia', a: a, b: b, c: c};
	});
var elm$core$String$concat = function (strings) {
	return A2(elm$core$String$join, '', strings);
};
var mdgriffith$style_elements$Style$Internal$Selector$render = F2(
	function (maybeGuard, selector) {
		var spacer = function (sel) {
			if (sel.$ === 'Pseudo') {
				return '';
			} else {
				return ' ';
			}
		};
		var renderAndSpace = F2(
			function (i, sel) {
				return (!i) ? A2(mdgriffith$style_elements$Style$Internal$Selector$render, maybeGuard, sel) : _Utils_ap(
					spacer(sel),
					A2(mdgriffith$style_elements$Style$Internal$Selector$render, maybeGuard, sel));
			});
		var applyGuard = function (str) {
			if (maybeGuard.$ === 'Nothing') {
				return str;
			} else {
				var g = maybeGuard.a;
				return str + ('--' + g);
			}
		};
		switch (selector.$) {
			case 'Select':
				var single = selector.a;
				return '.style-elements .' + applyGuard(single);
			case 'SelectChild':
				var selectChild = selector.a;
				return '> ' + A2(mdgriffith$style_elements$Style$Internal$Selector$render, maybeGuard, selectChild);
			case 'Free':
				var single = selector.a;
				return single;
			case 'Pseudo':
				var psu = selector.a;
				return psu;
			default:
				var sels = selector.a;
				return elm$core$String$concat(
					A2(elm$core$List$indexedMap, renderAndSpace, sels));
		}
	});
var mdgriffith$style_elements$Style$Internal$Intermediate$makeRenderable = function (cls) {
	var renderableProps = F2(
		function (prop, _n4) {
			var rendered = _n4.a;
			var subEls = _n4.b;
			switch (prop.$) {
				case 'Props':
					var ps = prop.a;
					return _Utils_Tuple2(
						_Utils_ap(rendered, ps),
						subEls);
				case 'SubClass':
					var embedded = prop.a;
					return _Utils_Tuple2(
						rendered,
						_Utils_ap(
							subEls,
							mdgriffith$style_elements$Style$Internal$Intermediate$makeRenderable(embedded)));
				case 'PropsAndSub':
					var ps = prop.a;
					var embedded = prop.b;
					return _Utils_Tuple2(
						_Utils_ap(rendered, ps),
						_Utils_ap(
							subEls,
							mdgriffith$style_elements$Style$Internal$Intermediate$makeRenderable(embedded)));
				default:
					return _Utils_Tuple2(rendered, subEls);
			}
		});
	switch (cls.$) {
		case 'Class':
			var classRule = cls.a;
			var _n1 = A3(
				elm$core$List$foldl,
				renderableProps,
				_Utils_Tuple2(_List_Nil, _List_Nil),
				classRule.props);
			var rendered = _n1.a;
			var subelements = _n1.b;
			return A2(
				elm$core$List$cons,
				A2(
					mdgriffith$style_elements$Style$Internal$Intermediate$RenderableClass,
					A2(mdgriffith$style_elements$Style$Internal$Selector$render, elm$core$Maybe$Nothing, classRule.selector),
					rendered),
				subelements);
		case 'Media':
			var mediaRule = cls.a;
			var _n2 = A3(
				elm$core$List$foldl,
				renderableProps,
				_Utils_Tuple2(_List_Nil, _List_Nil),
				mediaRule.props);
			var rendered = _n2.a;
			var subelements = _n2.b;
			return A2(
				elm$core$List$cons,
				A3(
					mdgriffith$style_elements$Style$Internal$Intermediate$RenderableMedia,
					mediaRule.query,
					A2(mdgriffith$style_elements$Style$Internal$Selector$render, elm$core$Maybe$Nothing, mediaRule.selector),
					rendered),
				subelements);
		default:
			var str = cls.a;
			return _List_fromArray(
				[
					mdgriffith$style_elements$Style$Internal$Intermediate$RenderableFree(str)
				]);
	}
};
var elm$core$Bitwise$and = _Bitwise_and;
var elm$core$Bitwise$shiftRightBy = _Bitwise_shiftRightBy;
var elm$core$String$repeatHelp = F3(
	function (n, chunk, result) {
		return (n <= 0) ? result : A3(
			elm$core$String$repeatHelp,
			n >> 1,
			_Utils_ap(chunk, chunk),
			(!(n & 1)) ? result : _Utils_ap(result, chunk));
	});
var elm$core$String$repeat = F2(
	function (n, chunk) {
		return A3(elm$core$String$repeatHelp, n, chunk, '');
	});
var mdgriffith$style_elements$Style$Internal$Render$Css$brace = F2(
	function (i, str) {
		return ' {\n' + (str + ('\n' + (A2(elm$core$String$repeat, i, ' ') + '}')));
	});
var mdgriffith$style_elements$Style$Internal$Render$Css$prop = F2(
	function (i, _n0) {
		var name = _n0.a;
		var value = _n0.b;
		return A2(elm$core$String$repeat, i, ' ') + (name + (': ' + (value + ';')));
	});
var mdgriffith$style_elements$Style$Internal$Intermediate$render = function (renderable) {
	switch (renderable.$) {
		case 'RenderableClass':
			var selector = renderable.a;
			var styleProps = renderable.b;
			return selector + (A2(
				mdgriffith$style_elements$Style$Internal$Render$Css$brace,
				0,
				A2(
					elm$core$String$join,
					'\n',
					A2(
						elm$core$List$map,
						mdgriffith$style_elements$Style$Internal$Render$Css$prop(2),
						styleProps))) + '\n');
		case 'RenderableMedia':
			var query = renderable.a;
			var selector = renderable.b;
			var styleProps = renderable.c;
			return _Utils_ap(
				query,
				A2(
					mdgriffith$style_elements$Style$Internal$Render$Css$brace,
					0,
					'  ' + (selector + A2(
						mdgriffith$style_elements$Style$Internal$Render$Css$brace,
						2,
						A2(
							elm$core$String$join,
							'\n',
							A2(
								elm$core$List$map,
								mdgriffith$style_elements$Style$Internal$Render$Css$prop(4),
								styleProps))))));
		default:
			var str = renderable.a;
			return str;
	}
};
var mdgriffith$style_elements$Style$Internal$Intermediate$finalize = function (intermediates) {
	var finalizeCss = function (cls) {
		return A2(
			elm$core$String$join,
			'\n',
			A2(
				elm$core$List$map,
				mdgriffith$style_elements$Style$Internal$Intermediate$render,
				mdgriffith$style_elements$Style$Internal$Intermediate$makeRenderable(cls)));
	};
	return mdgriffith$style_elements$Style$Internal$Intermediate$Rendered(
		{
			css: A2(
				elm$core$String$join,
				'\n',
				A2(elm$core$List$map, finalizeCss, intermediates)),
			findable: A2(elm$core$List$concatMap, mdgriffith$style_elements$Style$Internal$Intermediate$asFindable, intermediates)
		});
};
var mdgriffith$style_elements$Style$Internal$Model$Rotate = function (a) {
	return {$: 'Rotate', a: a};
};
var mdgriffith$style_elements$Style$Internal$Model$RotateAround = F4(
	function (a, b, c, d) {
		return {$: 'RotateAround', a: a, b: b, c: c, d: d};
	});
var mdgriffith$style_elements$Style$Internal$Model$Scale = F3(
	function (a, b, c) {
		return {$: 'Scale', a: a, b: b, c: c};
	});
var mdgriffith$style_elements$Style$Internal$Model$Translate = F3(
	function (a, b, c) {
		return {$: 'Translate', a: a, b: b, c: c};
	});
var mdgriffith$style_elements$Style$Internal$Render$preprocess = function (style) {
	if (style.$ === 'Style') {
		var className = style.a;
		var styleProps = style.b;
		var visible = function (prop) {
			if (prop.$ === 'Visibility') {
				return true;
			} else {
				return false;
			}
		};
		var shadows = function (prop) {
			if (prop.$ === 'Shadows') {
				return true;
			} else {
				return false;
			}
		};
		var prioritize = F2(
			function (isPriority, priorityProps) {
				var _n9 = A2(elm$core$List$partition, isPriority, priorityProps);
				var high = _n9.a;
				var low = _n9.b;
				return _Utils_ap(low, high);
			});
		var overridePrevious = F2(
			function (overridable, overrideProps) {
				var eliminatePrevious = F2(
					function (prop, _n8) {
						var existing = _n8.a;
						var overridden = _n8.b;
						return (overridable(prop) && overridden) ? _Utils_Tuple2(existing, overridden) : ((overridable(prop) && (!overridden)) ? _Utils_Tuple2(
							A2(elm$core$List$cons, prop, existing),
							true) : _Utils_Tuple2(
							A2(elm$core$List$cons, prop, existing),
							overridden));
					});
				return A3(
					elm$core$List$foldr,
					eliminatePrevious,
					_Utils_Tuple2(_List_Nil, false),
					overrideProps).a;
			});
		var mergeTransforms = function (mergeableProps) {
			var setIfNothing = F2(
				function (x, maybeX) {
					if (maybeX.$ === 'Nothing') {
						return elm$core$Maybe$Just(x);
					} else {
						var a = maybeX;
						return a;
					}
				});
			var gatherTransformStack = F2(
				function (transformation, gathered) {
					switch (transformation.$) {
						case 'Translate':
							var x = transformation.a;
							var y = transformation.b;
							var z = transformation.c;
							return _Utils_update(
								gathered,
								{
									translate: A2(
										setIfNothing,
										A3(mdgriffith$style_elements$Style$Internal$Model$Translate, x, y, z),
										gathered.translate)
								});
						case 'Rotate':
							var a = transformation.a;
							return _Utils_update(
								gathered,
								{
									rotate: A2(
										setIfNothing,
										mdgriffith$style_elements$Style$Internal$Model$Rotate(a),
										gathered.rotate)
								});
						case 'RotateAround':
							var x = transformation.a;
							var y = transformation.b;
							var z = transformation.c;
							var angle = transformation.d;
							return _Utils_update(
								gathered,
								{
									rotate: A2(
										setIfNothing,
										A4(mdgriffith$style_elements$Style$Internal$Model$RotateAround, x, y, z, angle),
										gathered.rotate)
								});
						default:
							var x = transformation.a;
							var y = transformation.b;
							var z = transformation.c;
							return _Utils_update(
								gathered,
								{
									scale: A2(
										setIfNothing,
										A3(mdgriffith$style_elements$Style$Internal$Model$Scale, x, y, z),
										gathered.scale)
								});
					}
				});
			var gatherTransforms = F2(
				function (prop, _n5) {
					var transforms = _n5.a;
					var gatheredProps = _n5.b;
					if (prop.$ === 'Transform') {
						var stack = prop.a;
						return _Utils_Tuple2(
							A3(elm$core$List$foldr, gatherTransformStack, transforms, stack),
							gatheredProps);
					} else {
						return _Utils_Tuple2(
							transforms,
							A2(elm$core$List$cons, prop, gatheredProps));
					}
				});
			var applyTransforms = function (_n3) {
				var rotate = _n3.a.rotate;
				var scale = _n3.a.scale;
				var translate = _n3.a.translate;
				var gathered = _n3.b;
				var transformations = A2(
					elm$core$List$filterMap,
					elm$core$Basics$identity,
					_List_fromArray(
						[translate, rotate, scale]));
				return elm$core$List$isEmpty(transformations) ? gathered : A2(
					elm$core$List$cons,
					mdgriffith$style_elements$Style$Internal$Model$Transform(transformations),
					gathered);
			};
			return applyTransforms(
				A3(
					elm$core$List$foldr,
					gatherTransforms,
					_Utils_Tuple2(
						{rotate: elm$core$Maybe$Nothing, scale: elm$core$Maybe$Nothing, translate: elm$core$Maybe$Nothing},
						_List_Nil),
					mergeableProps));
		};
		var mergeShadowsAndFilters = function (shadowsAndFilters) {
			var gather = F2(
				function (prop, existing) {
					switch (prop.$) {
						case 'Filters':
							var fs = prop.a;
							return _Utils_update(
								existing,
								{
									filters: _Utils_ap(fs, existing.filters)
								});
						case 'Shadows':
							var ss = prop.a;
							return _Utils_update(
								existing,
								{
									shadows: _Utils_ap(ss, existing.shadows)
								});
						default:
							return _Utils_update(
								existing,
								{
									others: A2(elm$core$List$cons, prop, existing.others)
								});
					}
				});
			var combine = function (combineable) {
				return A2(
					elm$core$List$cons,
					mdgriffith$style_elements$Style$Internal$Model$Filters(combineable.filters),
					A2(
						elm$core$List$cons,
						mdgriffith$style_elements$Style$Internal$Model$Shadows(combineable.shadows),
						combineable.others));
			};
			return combine(
				A3(
					elm$core$List$foldr,
					gather,
					{filters: _List_Nil, others: _List_Nil, shadows: _List_Nil},
					shadowsAndFilters));
		};
		var processed = mergeTransforms(
			mergeShadowsAndFilters(
				A2(
					overridePrevious,
					shadows,
					A2(
						prioritize,
						shadows,
						A2(
							overridePrevious,
							visible,
							A2(prioritize, visible, styleProps))))));
		var dropShadow = function (_n1) {
			var shade = _n1.a;
			return shade.kind === 'drop';
		};
		return A2(mdgriffith$style_elements$Style$Internal$Model$Style, className, processed);
	} else {
		return style;
	}
};
var mdgriffith$style_elements$Style$Internal$Intermediate$Class = function (a) {
	return {$: 'Class', a: a};
};
var mdgriffith$style_elements$Style$Internal$Intermediate$Free = function (a) {
	return {$: 'Free', a: a};
};
var mdgriffith$style_elements$Style$Internal$Intermediate$Media = function (a) {
	return {$: 'Media', a: a};
};
var mdgriffith$style_elements$Style$Internal$Intermediate$SubClass = function (a) {
	return {$: 'SubClass', a: a};
};
var mdgriffith$style_elements$Style$Internal$Find$Style = F2(
	function (a, b) {
		return {$: 'Style', a: a, b: b};
	});
var mdgriffith$style_elements$Style$Internal$Find$Variation = F3(
	function (a, b, c) {
		return {$: 'Variation', a: a, b: b, c: c};
	});
var mdgriffith$style_elements$Style$Internal$Selector$Select = F2(
	function (a, b) {
		return {$: 'Select', a: a, b: b};
	});
var mdgriffith$style_elements$Style$Internal$Selector$SelectChild = function (a) {
	return {$: 'SelectChild', a: a};
};
var mdgriffith$style_elements$Style$Internal$Selector$Stack = function (a) {
	return {$: 'Stack', a: a};
};
var mdgriffith$style_elements$Style$Internal$Selector$guard = F2(
	function (guardingString, selector) {
		var addGuard = function (str) {
			return str + ('g' + guardingString);
		};
		var onFindable = function (findable) {
			if (findable.$ === 'Style') {
				var _class = findable.a;
				var name = findable.b;
				return A2(
					mdgriffith$style_elements$Style$Internal$Find$Style,
					_class,
					addGuard(name));
			} else {
				var _class = findable.a;
				var variation = findable.b;
				var name = findable.c;
				return A3(
					mdgriffith$style_elements$Style$Internal$Find$Variation,
					_class,
					variation,
					addGuard(name));
			}
		};
		var onSelector = function (sel) {
			switch (sel.$) {
				case 'Select':
					var rendered = sel.a;
					var findable = sel.b;
					return A2(
						mdgriffith$style_elements$Style$Internal$Selector$Select,
						addGuard(rendered),
						onFindable(findable));
				case 'SelectChild':
					var selectChild = sel.a;
					return mdgriffith$style_elements$Style$Internal$Selector$SelectChild(
						onSelector(selectChild));
				case 'Stack':
					var selectors = sel.a;
					return mdgriffith$style_elements$Style$Internal$Selector$Stack(
						A2(elm$core$List$map, onSelector, selectors));
				default:
					var x = sel;
					return x;
			}
		};
		return onSelector(selector);
	});
var mdgriffith$style_elements$Style$Internal$Intermediate$applyGuard = F2(
	function (guardString, _class) {
		var guardProp = function (prop) {
			if (prop.$ === 'SubClass') {
				var sc = prop.a;
				return mdgriffith$style_elements$Style$Internal$Intermediate$SubClass(
					A2(mdgriffith$style_elements$Style$Internal$Intermediate$applyGuard, guardString, sc));
			} else {
				var x = prop;
				return x;
			}
		};
		switch (_class.$) {
			case 'Class':
				var cls = _class.a;
				return mdgriffith$style_elements$Style$Internal$Intermediate$Class(
					{
						props: A2(elm$core$List$map, guardProp, cls.props),
						selector: A2(mdgriffith$style_elements$Style$Internal$Selector$guard, guardString, cls.selector)
					});
			case 'Media':
				var media = _class.a;
				return mdgriffith$style_elements$Style$Internal$Intermediate$Media(
					{
						props: A2(elm$core$List$map, guardProp, media.props),
						query: media.query,
						selector: A2(mdgriffith$style_elements$Style$Internal$Selector$guard, guardString, media.selector)
					});
			default:
				var x = _class;
				return x;
		}
	});
var mdgriffith$style_elements$Style$Internal$Intermediate$calculateGuard = function (_class) {
	var propToString = function (_n2) {
		var x = _n2.a;
		var y = _n2.b;
		return _Utils_ap(x, y);
	};
	var asString = function (prop) {
		switch (prop.$) {
			case 'Props':
				var ps = prop.a;
				return elm$core$String$concat(
					A2(elm$core$List$map, propToString, ps));
			case 'SubClass':
				var embedded = prop.a;
				return mdgriffith$style_elements$Style$Internal$Intermediate$calculateGuard(embedded);
			case 'PropsAndSub':
				var ps = prop.a;
				var embedded = prop.b;
				return _Utils_ap(
					elm$core$String$concat(
						A2(elm$core$List$map, propToString, ps)),
					mdgriffith$style_elements$Style$Internal$Intermediate$calculateGuard(embedded));
			default:
				return '';
		}
	};
	switch (_class.$) {
		case 'Class':
			var classRule = _class.a;
			return elm$core$String$concat(
				A2(elm$core$List$map, asString, classRule.props));
		case 'Media':
			var mediaRule = _class.a;
			return elm$core$String$concat(
				A2(elm$core$List$map, asString, mediaRule.props));
		default:
			return '';
	}
};
var Skinney$murmur3$Murmur3$HashData = F4(
	function (shift, seed, hash, charsProcessed) {
		return {charsProcessed: charsProcessed, hash: hash, seed: seed, shift: shift};
	});
var Skinney$murmur3$Murmur3$c1 = 3432918353;
var Skinney$murmur3$Murmur3$c2 = 461845907;
var elm$core$Bitwise$shiftLeftBy = _Bitwise_shiftLeftBy;
var elm$core$Bitwise$shiftRightZfBy = _Bitwise_shiftRightZfBy;
var Skinney$murmur3$Murmur3$multiplyBy = F2(
	function (b, a) {
		return ((a & 65535) * b) + ((((a >>> 16) * b) & 65535) << 16);
	});
var elm$core$Bitwise$or = _Bitwise_or;
var Skinney$murmur3$Murmur3$rotlBy = F2(
	function (b, a) {
		return (a << b) | (a >>> (32 - b));
	});
var elm$core$Bitwise$xor = _Bitwise_xor;
var Skinney$murmur3$Murmur3$finalize = function (data) {
	var acc = data.hash ? (data.seed ^ A2(
		Skinney$murmur3$Murmur3$multiplyBy,
		Skinney$murmur3$Murmur3$c2,
		A2(
			Skinney$murmur3$Murmur3$rotlBy,
			15,
			A2(Skinney$murmur3$Murmur3$multiplyBy, Skinney$murmur3$Murmur3$c1, data.hash)))) : data.seed;
	var h0 = acc ^ data.charsProcessed;
	var h1 = A2(Skinney$murmur3$Murmur3$multiplyBy, 2246822507, h0 ^ (h0 >>> 16));
	var h2 = A2(Skinney$murmur3$Murmur3$multiplyBy, 3266489909, h1 ^ (h1 >>> 13));
	return (h2 ^ (h2 >>> 16)) >>> 0;
};
var Skinney$murmur3$Murmur3$mix = F2(
	function (h1, k1) {
		return A2(
			Skinney$murmur3$Murmur3$multiplyBy,
			5,
			A2(
				Skinney$murmur3$Murmur3$rotlBy,
				13,
				h1 ^ A2(
					Skinney$murmur3$Murmur3$multiplyBy,
					Skinney$murmur3$Murmur3$c2,
					A2(
						Skinney$murmur3$Murmur3$rotlBy,
						15,
						A2(Skinney$murmur3$Murmur3$multiplyBy, Skinney$murmur3$Murmur3$c1, k1))))) + 3864292196;
	});
var Skinney$murmur3$Murmur3$hashFold = F2(
	function (c, data) {
		var res = data.hash | ((255 & elm$core$Char$toCode(c)) << data.shift);
		var _n0 = data.shift;
		if (_n0 === 24) {
			return {
				charsProcessed: data.charsProcessed + 1,
				hash: 0,
				seed: A2(Skinney$murmur3$Murmur3$mix, data.seed, res),
				shift: 0
			};
		} else {
			return {charsProcessed: data.charsProcessed + 1, hash: res, seed: data.seed, shift: data.shift + 8};
		}
	});
var elm$core$String$foldl = _String_foldl;
var Skinney$murmur3$Murmur3$hashString = F2(
	function (seed, str) {
		return Skinney$murmur3$Murmur3$finalize(
			A3(
				elm$core$String$foldl,
				Skinney$murmur3$Murmur3$hashFold,
				A4(Skinney$murmur3$Murmur3$HashData, 0, seed, 0, 0),
				str));
	});
var mdgriffith$style_elements$Style$Internal$Intermediate$hash = function (value) {
	return elm$core$String$fromInt(
		A2(Skinney$murmur3$Murmur3$hashString, 8675309, value));
};
var mdgriffith$style_elements$Style$Internal$Intermediate$guard = function (_class) {
	return A2(
		mdgriffith$style_elements$Style$Internal$Intermediate$applyGuard,
		mdgriffith$style_elements$Style$Internal$Intermediate$hash(
			mdgriffith$style_elements$Style$Internal$Intermediate$calculateGuard(_class)),
		_class);
};
var mdgriffith$style_elements$Style$Internal$Render$class = F2(
	function (name, props) {
		var renderedProps = A2(
			elm$core$String$join,
			'\n',
			A2(
				elm$core$List$map,
				mdgriffith$style_elements$Style$Internal$Render$Css$prop(2),
				props));
		return '.' + (name + A2(mdgriffith$style_elements$Style$Internal$Render$Css$brace, 0, renderedProps));
	});
var mdgriffith$style_elements$Style$Internal$Intermediate$PropsAndSub = F2(
	function (a, b) {
		return {$: 'PropsAndSub', a: a, b: b};
	});
var mdgriffith$style_elements$Style$Internal$Intermediate$asMediaQuery = F2(
	function (query, prop) {
		var classAsMediaQuery = function (cls) {
			if (cls.$ === 'Class') {
				var classRule = cls.a;
				return mdgriffith$style_elements$Style$Internal$Intermediate$Media(
					{props: classRule.props, query: query, selector: classRule.selector});
			} else {
				var x = cls;
				return x;
			}
		};
		switch (prop.$) {
			case 'SubClass':
				var cls = prop.a;
				return mdgriffith$style_elements$Style$Internal$Intermediate$SubClass(
					classAsMediaQuery(cls));
			case 'PropsAndSub':
				var x = prop.a;
				var cls = prop.b;
				return A2(
					mdgriffith$style_elements$Style$Internal$Intermediate$PropsAndSub,
					x,
					classAsMediaQuery(cls));
			default:
				var x = prop;
				return x;
		}
	});
var mdgriffith$style_elements$Style$Internal$Intermediate$getProps = function (prop) {
	_n0$4:
	while (true) {
		switch (prop.$) {
			case 'Props':
				var rendered = prop.a;
				return rendered;
			case 'SubClass':
				if (prop.a.$ === 'Class') {
					var myClass = prop.a.a;
					return A2(elm$core$List$concatMap, mdgriffith$style_elements$Style$Internal$Intermediate$getProps, myClass.props);
				} else {
					break _n0$4;
				}
			case 'PropsAndSub':
				if (prop.b.$ === 'Class') {
					var rendered = prop.a;
					var myClass = prop.b.a;
					return _Utils_ap(
						rendered,
						A2(elm$core$List$concatMap, mdgriffith$style_elements$Style$Internal$Intermediate$getProps, myClass.props));
				} else {
					break _n0$4;
				}
			default:
				return _List_Nil;
		}
	}
	return _List_Nil;
};
var mdgriffith$style_elements$Style$Internal$Intermediate$Props = function (a) {
	return {$: 'Props', a: a};
};
var mdgriffith$style_elements$Style$Internal$Intermediate$props = mdgriffith$style_elements$Style$Internal$Intermediate$Props;
var mdgriffith$style_elements$Style$Internal$Render$Property$background = function (prop) {
	var renderStep = function (step) {
		switch (step.$) {
			case 'ColorStep':
				var color = step.a;
				return mdgriffith$style_elements$Style$Internal$Render$Value$color(color);
			case 'PercentStep':
				var color = step.a;
				var percent = step.b;
				return mdgriffith$style_elements$Style$Internal$Render$Value$color(color) + (' ' + (elm$core$String$fromFloat(percent) + '%'));
			default:
				var color = step.a;
				var percent = step.b;
				return mdgriffith$style_elements$Style$Internal$Render$Value$color(color) + (' ' + (elm$core$String$fromFloat(percent) + 'px'));
		}
	};
	var directionName = function (dir) {
		switch (dir.$) {
			case 'ToUp':
				return 'to top';
			case 'ToDown':
				return 'to bottom';
			case 'ToRight':
				return 'to right';
			case 'ToTopRight':
				return 'to top right';
			case 'ToBottomRight':
				return 'to bottom right';
			case 'ToLeft':
				return 'to left';
			case 'ToTopLeft':
				return 'to top left';
			case 'ToBottomLeft':
				return 'to bottom left';
			default:
				var angle = dir.a;
				return elm$core$String$fromFloat(angle) + 'rad';
		}
	};
	switch (prop.$) {
		case 'BackgroundElement':
			var name = prop.a;
			var val = prop.b;
			return _List_fromArray(
				[
					_Utils_Tuple2(name, val)
				]);
		case 'BackgroundImage':
			var image = prop.a;
			return _List_fromArray(
				[
					_Utils_Tuple2('background-image', 'url(' + (image.src + ')')),
					_Utils_Tuple2(
					'background-position',
					elm$core$String$fromFloat(image.position.a) + ('px ' + (elm$core$String$fromFloat(image.position.b) + 'px'))),
					_Utils_Tuple2(
					'background-repeat',
					function () {
						var _n1 = image.repeat;
						switch (_n1.$) {
							case 'RepeatX':
								return 'repeat-x';
							case 'RepeatY':
								return 'repeat-y';
							case 'Repeat':
								return 'repeat';
							case 'Space':
								return 'space';
							case 'Round':
								return 'round';
							default:
								return 'no-repeat';
						}
					}()),
					_Utils_Tuple2(
					'background-size',
					function () {
						var _n2 = image.size;
						switch (_n2.$) {
							case 'Contain':
								return 'contain';
							case 'Cover':
								return 'cover';
							case 'BackgroundWidth':
								var width = _n2.a;
								return mdgriffith$style_elements$Style$Internal$Render$Value$length(width) + ' auto';
							case 'BackgroundHeight':
								var height = _n2.a;
								return 'auto ' + mdgriffith$style_elements$Style$Internal$Render$Value$length(height);
							default:
								var width = _n2.a.width;
								var height = _n2.a.height;
								return mdgriffith$style_elements$Style$Internal$Render$Value$length(width) + (' ' + mdgriffith$style_elements$Style$Internal$Render$Value$length(height));
						}
					}())
				]);
		default:
			var dir = prop.a;
			var steps = prop.b;
			return _List_fromArray(
				[
					_Utils_Tuple2(
					'background-image',
					'linear-gradient(' + (A2(
						elm$core$String$join,
						', ',
						A2(
							elm$core$List$cons,
							directionName(dir),
							A2(elm$core$List$map, renderStep, steps))) + ')'))
				]);
	}
};
var mdgriffith$style_elements$Style$Internal$Render$Value$shadow = function (_n0) {
	var shadowModel = _n0.a;
	return A2(
		elm$core$String$join,
		' ',
		A2(
			elm$core$List$filterMap,
			elm$core$Basics$identity,
			_List_fromArray(
				[
					(shadowModel.kind === 'inset') ? elm$core$Maybe$Just('inset') : elm$core$Maybe$Nothing,
					elm$core$Maybe$Just(
					elm$core$String$fromFloat(shadowModel.offset.a) + 'px'),
					elm$core$Maybe$Just(
					elm$core$String$fromFloat(shadowModel.offset.b) + 'px'),
					elm$core$Maybe$Just(
					elm$core$String$fromFloat(shadowModel.blur) + 'px'),
					((shadowModel.kind === 'text') || (shadowModel.kind === 'drop')) ? elm$core$Maybe$Nothing : elm$core$Maybe$Just(
					elm$core$String$fromFloat(shadowModel.size) + 'px'),
					elm$core$Maybe$Just(
					mdgriffith$style_elements$Style$Internal$Render$Value$color(shadowModel.color))
				])));
};
var mdgriffith$style_elements$Style$Internal$Render$Property$filters = function (myFilters) {
	var filterName = function (filtr) {
		switch (filtr.$) {
			case 'FilterUrl':
				var url = filtr.a;
				return 'url(' + (url + ')');
			case 'Blur':
				var x = filtr.a;
				return 'blur(' + (elm$core$String$fromFloat(x) + 'px)');
			case 'Brightness':
				var x = filtr.a;
				return 'brightness(' + (elm$core$String$fromFloat(x) + '%)');
			case 'Contrast':
				var x = filtr.a;
				return 'contrast(' + (elm$core$String$fromFloat(x) + '%)');
			case 'Grayscale':
				var x = filtr.a;
				return 'grayscale(' + (elm$core$String$fromFloat(x) + '%)');
			case 'HueRotate':
				var x = filtr.a;
				return 'hueRotate(' + (elm$core$String$fromFloat(x) + 'deg)');
			case 'Invert':
				var x = filtr.a;
				return 'invert(' + (elm$core$String$fromFloat(x) + '%)');
			case 'OpacityFilter':
				var x = filtr.a;
				return 'opacity(' + (elm$core$String$fromFloat(x) + '%)');
			case 'Saturate':
				var x = filtr.a;
				return 'saturate(' + (elm$core$String$fromFloat(x) + '%)');
			case 'Sepia':
				var x = filtr.a;
				return 'sepia(' + (elm$core$String$fromFloat(x) + '%)');
			default:
				var dropShadow = filtr.a;
				var shadowModel = mdgriffith$style_elements$Style$Internal$Model$ShadowModel(
					{blur: dropShadow.blur, color: dropShadow.color, kind: 'drop', offset: dropShadow.offset, size: dropShadow.size});
				return 'drop-shadow(' + (mdgriffith$style_elements$Style$Internal$Render$Value$shadow(shadowModel) + ')');
		}
	};
	return (!elm$core$List$length(myFilters)) ? _List_Nil : _List_fromArray(
		[
			_Utils_Tuple2(
			'filter',
			A2(
				elm$core$String$join,
				' ',
				A2(elm$core$List$map, filterName, myFilters)))
		]);
};
var mdgriffith$style_elements$Style$Internal$Render$Property$position = function (posEls) {
	var renderPos = function (pos) {
		switch (pos.$) {
			case 'RelativeTo':
				switch (pos.a.$) {
					case 'Screen':
						var _n1 = pos.a;
						return _Utils_Tuple2('position', 'fixed');
					case 'Parent':
						var _n2 = pos.a;
						return _Utils_Tuple2('position', 'absolute');
					default:
						var _n3 = pos.a;
						return _Utils_Tuple2('position', 'relative');
				}
			case 'PosLeft':
				var x = pos.a;
				return _Utils_Tuple2(
					'left',
					elm$core$String$fromFloat(x) + 'px');
			case 'PosRight':
				var x = pos.a;
				return _Utils_Tuple2(
					'right',
					elm$core$String$fromFloat(x) + 'px');
			case 'PosTop':
				var x = pos.a;
				return _Utils_Tuple2(
					'top',
					elm$core$String$fromFloat(x) + 'px');
			case 'PosBottom':
				var x = pos.a;
				return _Utils_Tuple2(
					'bottom',
					elm$core$String$fromFloat(x) + 'px');
			case 'ZIndex':
				var i = pos.a;
				return _Utils_Tuple2(
					'z-index',
					elm$core$String$fromInt(i));
			case 'Inline':
				return _Utils_Tuple2('display', 'inline-block');
			default:
				switch (pos.a.$) {
					case 'FloatLeft':
						var _n4 = pos.a;
						return _Utils_Tuple2('float', 'left');
					case 'FloatRight':
						var _n5 = pos.a;
						return _Utils_Tuple2('float', 'right');
					case 'FloatTopLeft':
						var _n6 = pos.a;
						return _Utils_Tuple2('float', 'left');
					default:
						var _n7 = pos.a;
						return _Utils_Tuple2('float', 'right');
				}
		}
	};
	return A2(elm$core$List$map, renderPos, posEls);
};
var mdgriffith$style_elements$Style$Internal$Render$Property$shadow = function (shadows) {
	var _n0 = A2(
		elm$core$List$partition,
		function (_n1) {
			var s = _n1.a;
			return s.kind === 'text';
		},
		shadows);
	var text = _n0.a;
	var boxShadow = _n0.b;
	var renderedBox = A2(
		elm$core$String$join,
		', ',
		A2(elm$core$List$map, mdgriffith$style_elements$Style$Internal$Render$Value$shadow, boxShadow));
	var renderedText = A2(
		elm$core$String$join,
		', ',
		A2(elm$core$List$map, mdgriffith$style_elements$Style$Internal$Render$Value$shadow, text));
	return A2(
		elm$core$List$filterMap,
		elm$core$Basics$identity,
		_List_fromArray(
			[
				(renderedBox === '') ? elm$core$Maybe$Nothing : elm$core$Maybe$Just(
				_Utils_Tuple2('box-shadow', renderedBox)),
				(renderedText === '') ? elm$core$Maybe$Nothing : elm$core$Maybe$Just(
				_Utils_Tuple2('text-shadow', renderedText))
			]));
};
var mdgriffith$style_elements$Style$Internal$Render$Property$transformations = function (transforms) {
	var transformToString = function (transform) {
		switch (transform.$) {
			case 'Translate':
				var x = transform.a;
				var y = transform.b;
				var z = transform.c;
				return 'translate3d(' + (elm$core$String$fromFloat(x) + ('px, ' + (elm$core$String$fromFloat(y) + ('px, ' + (elm$core$String$fromFloat(z) + 'px)')))));
			case 'RotateAround':
				var x = transform.a;
				var y = transform.b;
				var z = transform.c;
				var angle = transform.d;
				return 'rotate3d(' + (elm$core$String$fromFloat(x) + (',' + (elm$core$String$fromFloat(y) + (',' + (elm$core$String$fromFloat(z) + (',' + (elm$core$String$fromFloat(angle) + 'rad)')))))));
			case 'Rotate':
				var x = transform.a;
				return 'rotate(' + (elm$core$String$fromFloat(x) + 'rad)');
			default:
				var x = transform.a;
				var y = transform.b;
				var z = transform.c;
				return 'scale3d(' + (elm$core$String$fromFloat(x) + (', ' + (elm$core$String$fromFloat(y) + (', ' + (elm$core$String$fromFloat(z) + ')')))));
		}
	};
	var transformString = A2(
		elm$core$String$join,
		' ',
		A2(elm$core$List$map, transformToString, transforms));
	var renderedTransforms = (elm$core$String$length(transformString) > 0) ? _List_fromArray(
		[
			_Utils_Tuple2('transform', transformString)
		]) : _List_Nil;
	return (!elm$core$List$length(transforms)) ? _List_Nil : renderedTransforms;
};
var mdgriffith$style_elements$Style$Internal$Render$Property$transition = function (_n0) {
	var delay = _n0.a.delay;
	var duration = _n0.a.duration;
	var easing = _n0.a.easing;
	var props = _n0.a.props;
	var formatTrans = function (prop) {
		return A2(
			elm$core$String$join,
			' ',
			_List_fromArray(
				[
					prop,
					elm$core$String$fromFloat(duration) + 'ms',
					easing,
					elm$core$String$fromFloat(delay) + 'ms'
				]));
	};
	return A2(
		elm$core$String$join,
		', ',
		A2(elm$core$List$map, formatTrans, props));
};
var mdgriffith$style_elements$Style$Internal$Render$Property$visibility = function (vis) {
	switch (vis.$) {
		case 'Hidden':
			return _List_fromArray(
				[
					_Utils_Tuple2('display', 'none')
				]);
		case 'Invisible':
			return _List_fromArray(
				[
					_Utils_Tuple2('visibility', 'hidden')
				]);
		default:
			var x = vis.a;
			return _List_fromArray(
				[
					_Utils_Tuple2(
					'opacity',
					elm$core$String$fromFloat(x))
				]);
	}
};
var mdgriffith$style_elements$Style$Internal$Render$Value$typeface = function (families) {
	var renderFont = function (font) {
		switch (font.$) {
			case 'Serif':
				return 'serif';
			case 'SansSerif':
				return 'sans-serif';
			case 'Cursive':
				return 'cursive';
			case 'Fantasy':
				return 'fantasy';
			case 'Monospace':
				return 'monospace';
			case 'FontName':
				var name = font.a;
				return '\"' + (name + '\"');
			default:
				var name = font.a;
				var url = font.b;
				return '\"' + (name + '\"');
		}
	};
	return A2(
		elm$core$String$join,
		', ',
		A2(elm$core$List$map, renderFont, families));
};
var mdgriffith$style_elements$Style$Internal$Selector$Free = function (a) {
	return {$: 'Free', a: a};
};
var mdgriffith$style_elements$Style$Internal$Selector$Pseudo = function (a) {
	return {$: 'Pseudo', a: a};
};
var mdgriffith$style_elements$Style$Internal$Selector$pseudo = F2(
	function (psu, sel) {
		switch (sel.$) {
			case 'Pseudo':
				var existing = sel.a;
				return mdgriffith$style_elements$Style$Internal$Selector$Pseudo(
					_Utils_ap(existing, psu));
			case 'Select':
				var single = sel.a;
				var findable = sel.b;
				return mdgriffith$style_elements$Style$Internal$Selector$Stack(
					_List_fromArray(
						[
							A2(mdgriffith$style_elements$Style$Internal$Selector$Select, single, findable),
							mdgriffith$style_elements$Style$Internal$Selector$Pseudo(psu)
						]));
			case 'SelectChild':
				var selectChild = sel.a;
				return mdgriffith$style_elements$Style$Internal$Selector$SelectChild(
					A2(mdgriffith$style_elements$Style$Internal$Selector$pseudo, psu, selectChild));
			case 'Free':
				var single = sel.a;
				return mdgriffith$style_elements$Style$Internal$Selector$Free(single);
			default:
				var sels = sel.a;
				var lastElem = elm$core$List$head(
					elm$core$List$reverse(sels));
				var init = elm$core$List$reverse(
					A2(
						elm$core$List$drop,
						1,
						elm$core$List$reverse(sels)));
				if (lastElem.$ === 'Nothing') {
					return mdgriffith$style_elements$Style$Internal$Selector$Stack(sels);
				} else {
					var last = lastElem.a;
					return mdgriffith$style_elements$Style$Internal$Selector$Stack(
						_Utils_ap(
							init,
							_List_fromArray(
								[
									A2(mdgriffith$style_elements$Style$Internal$Selector$pseudo, psu, last)
								])));
				}
		}
	});
var mdgriffith$style_elements$Style$Internal$Render$renderVariationProp = F2(
	function (parentClass, prop) {
		switch (prop.$) {
			case 'Child':
				return elm$core$Maybe$Nothing;
			case 'Variation':
				return elm$core$Maybe$Nothing;
			case 'PseudoElement':
				var cls = prop.a;
				var styleProps = prop.b;
				return A2(
					elm$core$Basics$composeL,
					A2(elm$core$Basics$composeL, elm$core$Maybe$Just, mdgriffith$style_elements$Style$Internal$Intermediate$SubClass),
					mdgriffith$style_elements$Style$Internal$Intermediate$Class)(
					{
						props: A2(
							elm$core$List$filterMap,
							mdgriffith$style_elements$Style$Internal$Render$renderVariationProp(parentClass),
							styleProps),
						selector: A2(mdgriffith$style_elements$Style$Internal$Selector$pseudo, cls, parentClass)
					});
			case 'MediaQuery':
				var query = prop.a;
				var styleProps = prop.b;
				return A2(
					elm$core$Basics$composeL,
					A2(elm$core$Basics$composeL, elm$core$Maybe$Just, mdgriffith$style_elements$Style$Internal$Intermediate$SubClass),
					mdgriffith$style_elements$Style$Internal$Intermediate$Media)(
					{
						props: A2(
							elm$core$List$map,
							mdgriffith$style_elements$Style$Internal$Intermediate$asMediaQuery(query),
							A2(
								elm$core$List$filterMap,
								mdgriffith$style_elements$Style$Internal$Render$renderVariationProp(parentClass),
								styleProps)),
						query: '@media ' + query,
						selector: parentClass
					});
			case 'Exact':
				var name = prop.a;
				var val = prop.b;
				return A2(elm$core$Basics$composeL, elm$core$Maybe$Just, mdgriffith$style_elements$Style$Internal$Intermediate$props)(
					_List_fromArray(
						[
							_Utils_Tuple2(name, val)
						]));
			case 'Visibility':
				var vis = prop.a;
				return A3(
					elm$core$Basics$composeL,
					elm$core$Maybe$Just,
					mdgriffith$style_elements$Style$Internal$Intermediate$props,
					mdgriffith$style_elements$Style$Internal$Render$Property$visibility(vis));
			case 'Position':
				var pos = prop.a;
				return A3(
					elm$core$Basics$composeL,
					elm$core$Maybe$Just,
					mdgriffith$style_elements$Style$Internal$Intermediate$props,
					mdgriffith$style_elements$Style$Internal$Render$Property$position(pos));
			case 'Font':
				var name = prop.a;
				var val = prop.b;
				return A3(
					elm$core$Basics$composeL,
					elm$core$Maybe$Just,
					mdgriffith$style_elements$Style$Internal$Intermediate$props,
					_List_fromArray(
						[
							_Utils_Tuple2(name, val)
						]));
			case 'FontFamily':
				var fam = prop.a;
				return A3(
					elm$core$Basics$composeL,
					elm$core$Maybe$Just,
					mdgriffith$style_elements$Style$Internal$Intermediate$props,
					_List_fromArray(
						[
							_Utils_Tuple2(
							'font-family',
							mdgriffith$style_elements$Style$Internal$Render$Value$typeface(fam))
						]));
			case 'Layout':
				var lay = prop.a;
				return A2(elm$core$Basics$composeL, elm$core$Maybe$Just, mdgriffith$style_elements$Style$Internal$Intermediate$props)(
					A2(mdgriffith$style_elements$Style$Internal$Render$Property$layout, false, lay));
			case 'Background':
				var props = prop.a;
				return A3(
					elm$core$Basics$composeL,
					elm$core$Maybe$Just,
					mdgriffith$style_elements$Style$Internal$Intermediate$props,
					mdgriffith$style_elements$Style$Internal$Render$Property$background(props));
			case 'Shadows':
				var shadows = prop.a;
				return A3(
					elm$core$Basics$composeL,
					elm$core$Maybe$Just,
					mdgriffith$style_elements$Style$Internal$Intermediate$props,
					mdgriffith$style_elements$Style$Internal$Render$Property$shadow(shadows));
			case 'Transform':
				var transformations = prop.a;
				return A3(
					elm$core$Basics$composeL,
					elm$core$Maybe$Just,
					mdgriffith$style_elements$Style$Internal$Intermediate$props,
					mdgriffith$style_elements$Style$Internal$Render$Property$transformations(transformations));
			case 'Filters':
				var filters = prop.a;
				return A3(
					elm$core$Basics$composeL,
					elm$core$Maybe$Just,
					mdgriffith$style_elements$Style$Internal$Intermediate$props,
					mdgriffith$style_elements$Style$Internal$Render$Property$filters(filters));
			case 'TextColor':
				var color = prop.a;
				return A3(
					elm$core$Basics$composeL,
					elm$core$Maybe$Just,
					mdgriffith$style_elements$Style$Internal$Intermediate$props,
					_List_fromArray(
						[
							_Utils_Tuple2(
							'color',
							mdgriffith$style_elements$Style$Internal$Render$Value$color(color))
						]));
			case 'SelectionColor':
				var color = prop.a;
				return A2(
					elm$core$Basics$composeL,
					A2(elm$core$Basics$composeL, elm$core$Maybe$Just, mdgriffith$style_elements$Style$Internal$Intermediate$SubClass),
					mdgriffith$style_elements$Style$Internal$Intermediate$Class)(
					{
						props: _List_fromArray(
							[
								mdgriffith$style_elements$Style$Internal$Intermediate$props(
								_List_fromArray(
									[
										_Utils_Tuple2(
										'background-color',
										mdgriffith$style_elements$Style$Internal$Render$Value$color(color))
									]))
							]),
						selector: A2(mdgriffith$style_elements$Style$Internal$Selector$pseudo, '::selection', parentClass)
					});
			default:
				var trans = prop.a;
				return elm$core$Maybe$Just(
					mdgriffith$style_elements$Style$Internal$Intermediate$props(
						_List_fromArray(
							[
								_Utils_Tuple2(
								'transition',
								A2(
									elm$core$String$join,
									', ',
									A2(elm$core$List$map, mdgriffith$style_elements$Style$Internal$Render$Property$transition, trans)))
							])));
		}
	});
var mdgriffith$style_elements$Style$Internal$Selector$child = F2(
	function (parent, selector) {
		return mdgriffith$style_elements$Style$Internal$Selector$Stack(
			_List_fromArray(
				[
					parent,
					mdgriffith$style_elements$Style$Internal$Selector$SelectChild(selector)
				]));
	});
var mdgriffith$style_elements$Style$Internal$Selector$select = function (_class) {
	return A2(
		mdgriffith$style_elements$Style$Internal$Selector$Select,
		'',
		A2(mdgriffith$style_elements$Style$Internal$Find$Style, _class, ''));
};
var mdgriffith$style_elements$Style$Internal$Find$toVariation = F3(
	function (_var, newName, element) {
		if (element.$ === 'Style') {
			var _class = element.a;
			var name = element.b;
			return A3(mdgriffith$style_elements$Style$Internal$Find$Variation, _class, _var, newName);
		} else {
			var _class = element.a;
			var name = element.c;
			return A3(mdgriffith$style_elements$Style$Internal$Find$Variation, _class, _var, newName);
		}
	});
var elm$core$String$toLower = _String_toLower;
var elm$regex$Regex$Match = F4(
	function (match, index, number, submatches) {
		return {index: index, match: match, number: number, submatches: submatches};
	});
var elm$regex$Regex$fromStringWith = _Regex_fromStringWith;
var elm$regex$Regex$fromString = function (string) {
	return A2(
		elm$regex$Regex$fromStringWith,
		{caseInsensitive: false, multiline: false},
		string);
};
var elm$regex$Regex$never = _Regex_never;
var elm$regex$Regex$replace = _Regex_replaceAtMost(_Regex_infinity);
var mdgriffith$style_elements$Style$Internal$Selector$uncapitalize = function (str) {
	var tail = A2(elm$core$String$dropLeft, 1, str);
	var head = elm$core$String$toLower(
		A2(elm$core$String$left, 1, str));
	return _Utils_ap(head, tail);
};
var mdgriffith$style_elements$Style$Internal$Selector$formatName = function (x) {
	return A3(
		elm$regex$Regex$replace,
		A2(
			elm$core$Maybe$withDefault,
			elm$regex$Regex$never,
			elm$regex$Regex$fromString('[\\s+]')),
		function (_n2) {
			return '-';
		},
		A3(
			elm$regex$Regex$replace,
			A2(
				elm$core$Maybe$withDefault,
				elm$regex$Regex$never,
				elm$regex$Regex$fromString('[A-Z0-9]+')),
			function (_n1) {
				var match = _n1.match;
				return ' ' + elm$core$String$toLower(match);
			},
			A3(
				elm$regex$Regex$replace,
				A2(
					elm$core$Maybe$withDefault,
					elm$regex$Regex$never,
					elm$regex$Regex$fromString('[^a-zA-Z0-9_-]')),
				function (_n0) {
					return '';
				},
				mdgriffith$style_elements$Style$Internal$Selector$uncapitalize(x))));
};
var mdgriffith$style_elements$Style$Internal$Selector$variant = F3(
	function (sel, _var, name) {
		switch (sel.$) {
			case 'Pseudo':
				var psu = sel.a;
				return mdgriffith$style_elements$Style$Internal$Selector$Pseudo(psu);
			case 'Select':
				var single = sel.a;
				var findable = sel.b;
				return A2(
					mdgriffith$style_elements$Style$Internal$Selector$Select,
					single + ('-' + mdgriffith$style_elements$Style$Internal$Selector$formatName(name)),
					A3(
						mdgriffith$style_elements$Style$Internal$Find$toVariation,
						_var,
						single + ('-' + mdgriffith$style_elements$Style$Internal$Selector$formatName(name)),
						findable));
			case 'SelectChild':
				var selectChild = sel.a;
				return mdgriffith$style_elements$Style$Internal$Selector$SelectChild(
					A3(mdgriffith$style_elements$Style$Internal$Selector$variant, selectChild, _var, name));
			case 'Free':
				var single = sel.a;
				return mdgriffith$style_elements$Style$Internal$Selector$Free(single);
			default:
				var sels = sel.a;
				var lastElem = elm$core$List$head(
					elm$core$List$reverse(sels));
				var init = elm$core$List$reverse(
					A2(
						elm$core$List$drop,
						1,
						elm$core$List$reverse(sels)));
				if (lastElem.$ === 'Nothing') {
					return mdgriffith$style_elements$Style$Internal$Selector$Stack(sels);
				} else {
					var last = lastElem.a;
					return mdgriffith$style_elements$Style$Internal$Selector$Stack(
						_Utils_ap(
							init,
							_List_fromArray(
								[
									A3(mdgriffith$style_elements$Style$Internal$Selector$variant, last, _var, name)
								])));
				}
		}
	});
var mdgriffith$style_elements$Style$Internal$Render$renderProp = F2(
	function (parentClass, prop) {
		switch (prop.$) {
			case 'Child':
				var cls = prop.a;
				var styleProps = prop.b;
				return A2(elm$core$Basics$composeL, mdgriffith$style_elements$Style$Internal$Intermediate$SubClass, mdgriffith$style_elements$Style$Internal$Intermediate$Class)(
					{
						props: A2(
							elm$core$List$map,
							mdgriffith$style_elements$Style$Internal$Render$renderProp(parentClass),
							styleProps),
						selector: A2(
							mdgriffith$style_elements$Style$Internal$Selector$child,
							parentClass,
							mdgriffith$style_elements$Style$Internal$Selector$select(cls))
					});
			case 'Variation':
				var _var = prop.a;
				var styleProps = prop.b;
				var variationName = function () {
					var _n1 = A2(
						elm$core$List$filterMap,
						mdgriffith$style_elements$Style$Internal$Render$renderVariationProp(parentClass),
						styleProps);
					if (!_n1.b) {
						return 'v';
					} else {
						var intermediates = _n1;
						return elm$core$String$concat(
							A2(
								elm$core$List$map,
								function (_n2) {
									var x = _n2.a;
									var y = _n2.b;
									return _Utils_ap(x, y);
								},
								A2(elm$core$List$concatMap, mdgriffith$style_elements$Style$Internal$Intermediate$getProps, intermediates)));
					}
				}();
				var selectVariation = A3(mdgriffith$style_elements$Style$Internal$Selector$variant, parentClass, _var, variationName);
				return A2(elm$core$Basics$composeL, mdgriffith$style_elements$Style$Internal$Intermediate$SubClass, mdgriffith$style_elements$Style$Internal$Intermediate$Class)(
					{
						props: A2(
							elm$core$List$filterMap,
							mdgriffith$style_elements$Style$Internal$Render$renderVariationProp(selectVariation),
							styleProps),
						selector: selectVariation
					});
			case 'PseudoElement':
				var cls = prop.a;
				var styleProps = prop.b;
				return A2(elm$core$Basics$composeL, mdgriffith$style_elements$Style$Internal$Intermediate$SubClass, mdgriffith$style_elements$Style$Internal$Intermediate$Class)(
					{
						props: A2(
							elm$core$List$map,
							mdgriffith$style_elements$Style$Internal$Render$renderProp(parentClass),
							styleProps),
						selector: A2(mdgriffith$style_elements$Style$Internal$Selector$pseudo, cls, parentClass)
					});
			case 'MediaQuery':
				var query = prop.a;
				var styleProps = prop.b;
				return A2(elm$core$Basics$composeL, mdgriffith$style_elements$Style$Internal$Intermediate$SubClass, mdgriffith$style_elements$Style$Internal$Intermediate$Media)(
					{
						props: A2(
							elm$core$List$map,
							mdgriffith$style_elements$Style$Internal$Intermediate$asMediaQuery(query),
							A2(
								elm$core$List$map,
								mdgriffith$style_elements$Style$Internal$Render$renderProp(parentClass),
								styleProps)),
						query: '@media ' + query,
						selector: parentClass
					});
			case 'Exact':
				var name = prop.a;
				var val = prop.b;
				return mdgriffith$style_elements$Style$Internal$Intermediate$props(
					_List_fromArray(
						[
							_Utils_Tuple2(name, val)
						]));
			case 'Visibility':
				var vis = prop.a;
				return mdgriffith$style_elements$Style$Internal$Intermediate$props(
					mdgriffith$style_elements$Style$Internal$Render$Property$visibility(vis));
			case 'Position':
				var pos = prop.a;
				return mdgriffith$style_elements$Style$Internal$Intermediate$props(
					mdgriffith$style_elements$Style$Internal$Render$Property$position(pos));
			case 'Font':
				var name = prop.a;
				var val = prop.b;
				return mdgriffith$style_elements$Style$Internal$Intermediate$props(
					_List_fromArray(
						[
							_Utils_Tuple2(name, val)
						]));
			case 'Layout':
				var lay = prop.a;
				return mdgriffith$style_elements$Style$Internal$Intermediate$props(
					A2(mdgriffith$style_elements$Style$Internal$Render$Property$layout, false, lay));
			case 'Background':
				var props = prop.a;
				return mdgriffith$style_elements$Style$Internal$Intermediate$props(
					mdgriffith$style_elements$Style$Internal$Render$Property$background(props));
			case 'Shadows':
				var shadows = prop.a;
				return mdgriffith$style_elements$Style$Internal$Intermediate$props(
					mdgriffith$style_elements$Style$Internal$Render$Property$shadow(shadows));
			case 'Transform':
				var transformations = prop.a;
				return mdgriffith$style_elements$Style$Internal$Intermediate$props(
					mdgriffith$style_elements$Style$Internal$Render$Property$transformations(transformations));
			case 'Filters':
				var filters = prop.a;
				return mdgriffith$style_elements$Style$Internal$Intermediate$props(
					mdgriffith$style_elements$Style$Internal$Render$Property$filters(filters));
			case 'SelectionColor':
				var color = prop.a;
				return A2(elm$core$Basics$composeL, mdgriffith$style_elements$Style$Internal$Intermediate$SubClass, mdgriffith$style_elements$Style$Internal$Intermediate$Class)(
					{
						props: _List_fromArray(
							[
								mdgriffith$style_elements$Style$Internal$Intermediate$props(
								_List_fromArray(
									[
										_Utils_Tuple2(
										'background-color',
										mdgriffith$style_elements$Style$Internal$Render$Value$color(color))
									]))
							]),
						selector: A2(mdgriffith$style_elements$Style$Internal$Selector$pseudo, '::selection', parentClass)
					});
			case 'TextColor':
				var color = prop.a;
				return mdgriffith$style_elements$Style$Internal$Intermediate$props(
					_List_fromArray(
						[
							_Utils_Tuple2(
							'color',
							mdgriffith$style_elements$Style$Internal$Render$Value$color(color))
						]));
			case 'Transitions':
				var trans = prop.a;
				return mdgriffith$style_elements$Style$Internal$Intermediate$props(
					_List_fromArray(
						[
							_Utils_Tuple2(
							'transition',
							A2(
								elm$core$String$join,
								', ',
								A2(elm$core$List$map, mdgriffith$style_elements$Style$Internal$Render$Property$transition, trans)))
						]));
			default:
				var fam = prop.a;
				return mdgriffith$style_elements$Style$Internal$Intermediate$props(
					_List_fromArray(
						[
							_Utils_Tuple2(
							'font-family',
							mdgriffith$style_elements$Style$Internal$Render$Value$typeface(fam))
						]));
		}
	});
var mdgriffith$style_elements$Style$Internal$Render$renderStyle = F2(
	function (guarded, style) {
		switch (style.$) {
			case 'Reset':
				var reset = style.a;
				return mdgriffith$style_elements$Style$Internal$Intermediate$Free(reset);
			case 'Import':
				var str = style.a;
				return mdgriffith$style_elements$Style$Internal$Intermediate$Free('@import ' + (str + ';'));
			case 'RawStyle':
				var cls = style.a;
				var styleProps = style.b;
				return mdgriffith$style_elements$Style$Internal$Intermediate$Free(
					A2(mdgriffith$style_elements$Style$Internal$Render$class, cls, styleProps));
			default:
				var cls = style.a;
				var styleProps = style.b;
				var selector = mdgriffith$style_elements$Style$Internal$Selector$select(cls);
				var inter = mdgriffith$style_elements$Style$Internal$Intermediate$Class(
					{
						props: A2(
							elm$core$List$map,
							mdgriffith$style_elements$Style$Internal$Render$renderProp(selector),
							styleProps),
						selector: selector
					});
				var guard = function (i) {
					return guarded ? mdgriffith$style_elements$Style$Internal$Intermediate$guard(i) : i;
				};
				return guard(inter);
		}
	});
var elm$core$Set$Set_elm_builtin = function (a) {
	return {$: 'Set_elm_builtin', a: a};
};
var elm$core$Set$empty = elm$core$Set$Set_elm_builtin(elm$core$Dict$empty);
var elm$core$Set$insert = F2(
	function (key, _n0) {
		var dict = _n0.a;
		return elm$core$Set$Set_elm_builtin(
			A3(elm$core$Dict$insert, key, _Utils_Tuple0, dict));
	});
var elm$core$Set$fromList = function (list) {
	return A3(elm$core$List$foldl, elm$core$Set$insert, elm$core$Set$empty, list);
};
var mdgriffith$style_elements$Style$Internal$Render$reorderImportAddReset = F2(
	function (reset, styles) {
		var reorder = F2(
			function (style, _n5) {
				var importStatements = _n5.a;
				var remainingStyles = _n5.b;
				if (style.$ === 'Import') {
					return _Utils_Tuple2(
						A2(elm$core$List$cons, style, importStatements),
						remainingStyles);
				} else {
					var x = style;
					return _Utils_Tuple2(
						importStatements,
						A2(elm$core$List$cons, style, remainingStyles));
				}
			});
		var getFontStyle = function (style) {
			if (style.$ === 'Style') {
				var props = style.b;
				var forFont = function (prop) {
					if (prop.$ === 'FontFamily') {
						var fams = prop.a;
						var forImport = function (font) {
							if (font.$ === 'ImportFont') {
								var url = font.b;
								return elm$core$Maybe$Just(url);
							} else {
								return elm$core$Maybe$Nothing;
							}
						};
						return A2(elm$core$List$filterMap, forImport, fams);
					} else {
						return _List_Nil;
					}
				};
				return A2(elm$core$List$concatMap, forFont, props);
			} else {
				return _List_Nil;
			}
		};
		var importedFonts = A2(
			elm$core$List$map,
			function (uri) {
				return mdgriffith$style_elements$Style$Internal$Model$Import('url(\'' + (uri + '\')'));
			},
			A3(
				elm$core$Basics$composeL,
				elm$core$Set$toList,
				elm$core$Set$fromList,
				A2(elm$core$List$concatMap, getFontStyle, styles)));
		var _n0 = A3(
			elm$core$List$foldr,
			reorder,
			_Utils_Tuple2(_List_Nil, _List_Nil),
			styles);
		var imports = _n0.a;
		var allStyles = _n0.b;
		return _Utils_ap(
			imports,
			_Utils_ap(
				importedFonts,
				_Utils_ap(
					_List_fromArray(
						[
							mdgriffith$style_elements$Style$Internal$Model$Reset(reset)
						]),
					allStyles)));
	});
var mdgriffith$style_elements$Style$Internal$Render$stylesheet = F3(
	function (reset, guard, batched) {
		return mdgriffith$style_elements$Style$Internal$Intermediate$finalize(
			A2(
				elm$core$List$map,
				A2(
					elm$core$Basics$composeL,
					mdgriffith$style_elements$Style$Internal$Render$renderStyle(guard),
					mdgriffith$style_elements$Style$Internal$Render$preprocess),
				A2(
					mdgriffith$style_elements$Style$Internal$Render$reorderImportAddReset,
					reset,
					mdgriffith$style_elements$Style$Internal$Batchable$toList(batched))));
	});
var mdgriffith$style_elements$Style$styleSheetWith = F2(
	function (options, styles) {
		var unguard = A2(
			elm$core$List$any,
			elm$core$Basics$eq(mdgriffith$style_elements$Style$Unguarded),
			options);
		return mdgriffith$style_elements$Style$prepareSheet(
			A3(mdgriffith$style_elements$Style$Internal$Render$stylesheet, '', !unguard, styles));
	});
var mdgriffith$style_elements$Style$styleSheet = function (styles) {
	return A2(mdgriffith$style_elements$Style$styleSheetWith, _List_Nil, styles);
};
var elm_explorations$benchmark$Benchmark$Runner$App$view = function (model) {
	var body = elm_explorations$benchmark$Benchmark$done(model) ? A4(
		mdgriffith$style_elements$Element$mapAll,
		elm$core$Basics$identity,
		elm_explorations$benchmark$Benchmark$Runner$App$ReportClass,
		elm_explorations$benchmark$Benchmark$Runner$App$ReportVariation,
		elm_explorations$benchmark$Benchmark$Runner$Report$view(
			elm_explorations$benchmark$Benchmark$Reporting$fromBenchmark(model))) : A4(
		mdgriffith$style_elements$Element$mapAll,
		elm$core$Basics$identity,
		elm_explorations$benchmark$Benchmark$Runner$App$InProgressClass,
		elm$core$Basics$identity,
		elm_explorations$benchmark$Benchmark$Runner$InProgress$view(
			elm_explorations$benchmark$Benchmark$Reporting$fromBenchmark(model)));
	return A2(
		mdgriffith$style_elements$Element$viewport,
		mdgriffith$style_elements$Style$styleSheet(elm_explorations$benchmark$Benchmark$Runner$App$styles),
		A3(
			mdgriffith$style_elements$Element$row,
			elm_explorations$benchmark$Benchmark$Runner$App$Page,
			_List_fromArray(
				[
					mdgriffith$style_elements$Element$Attributes$width(mdgriffith$style_elements$Element$Attributes$fill),
					mdgriffith$style_elements$Element$Attributes$minHeight(mdgriffith$style_elements$Element$Attributes$fill),
					mdgriffith$style_elements$Element$Attributes$center,
					mdgriffith$style_elements$Element$Attributes$verticalCenter
				]),
			_List_fromArray(
				[
					A3(
					mdgriffith$style_elements$Element$el,
					elm_explorations$benchmark$Benchmark$Runner$App$Wrapper,
					_List_fromArray(
						[
							mdgriffith$style_elements$Element$Attributes$maxWidth(
							mdgriffith$style_elements$Element$Attributes$px(800)),
							mdgriffith$style_elements$Element$Attributes$padding(60)
						]),
					body)
				])));
};
var elm_explorations$benchmark$Benchmark$Runner$program = function (benchmark) {
	return elm$browser$Browser$element(
		{
			init: elm_explorations$benchmark$Benchmark$Runner$App$init(benchmark),
			subscriptions: elm$core$Basics$always(elm$core$Platform$Sub$none),
			update: elm_explorations$benchmark$Benchmark$Runner$App$update,
			view: elm_explorations$benchmark$Benchmark$Runner$App$view
		});
};
//============ start patch.js ===================
var catch_wasm_exceptions = false; // checking for exceptions helps with debug but slows things down
if (catch_wasm_exceptions) {
  function wrap(exp) {
    return function() {
      try {
        return exp.apply(null, arguments);
      } catch (e) {
        Module.asm._dump(); // log the state of the Wasm heap & GC
        console.error(e);
        debugger;
      }
    };
  }
  author$project$Main$wasmCount = wrap(Module.asm._export_count);
  author$project$Main$wasmCountNoTCE = wrap(Module.asm._export_count_no_tce);
  author$project$Main$wasmAdd = F2(wrap(Module.asm._export_add));
  author$project$Main$wasmAddUnboxed = F2(wrap(Module.asm._export_add_unboxed));
} else {
  author$project$Main$wasmCount = Module.asm._export_count;
  author$project$Main$wasmCountNoTCE = Module.asm._export_count_no_tce;
  author$project$Main$wasmAdd = F2(Module.asm._export_add);
  author$project$Main$wasmAddUnboxed = F2(Module.asm._export_add_unboxed);
}
//============ end patch.js ===================
var author$project$Main$main = elm_explorations$benchmark$Benchmark$Runner$program(author$project$Main$suite);
_Platform_export({'Main':{'init':author$project$Main$main(
	elm$json$Json$Decode$succeed(_Utils_Tuple0))(0)}});}(this));
//============ start init.js ===================
debugger;
Elm.Main.init({ node: document.querySelector('#main') });
//============ end init.js ===================
});

