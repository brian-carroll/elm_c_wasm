
var EmscriptenModule = (
function(EmscriptenModule) {
  EmscriptenModule = EmscriptenModule || {};

var Module=typeof EmscriptenModule!=="undefined"?EmscriptenModule:{};var moduleOverrides={};var key;for(key in Module){if(Module.hasOwnProperty(key)){moduleOverrides[key]=Module[key]}}Module["arguments"]=[];Module["thisProgram"]="./this.program";Module["quit"]=(function(status,toThrow){throw toThrow});Module["preRun"]=[];Module["postRun"]=[];var ENVIRONMENT_IS_WEB=false;var ENVIRONMENT_IS_WORKER=false;var ENVIRONMENT_IS_NODE=false;var ENVIRONMENT_IS_SHELL=false;ENVIRONMENT_IS_WEB=typeof window==="object";ENVIRONMENT_IS_WORKER=typeof importScripts==="function";ENVIRONMENT_IS_NODE=typeof process==="object"&&typeof require==="function"&&!ENVIRONMENT_IS_WEB&&!ENVIRONMENT_IS_WORKER;ENVIRONMENT_IS_SHELL=!ENVIRONMENT_IS_WEB&&!ENVIRONMENT_IS_NODE&&!ENVIRONMENT_IS_WORKER;var scriptDirectory="";function locateFile(path){if(Module["locateFile"]){return Module["locateFile"](path,scriptDirectory)}else{return scriptDirectory+path}}if(ENVIRONMENT_IS_NODE){scriptDirectory=__dirname+"/";var nodeFS;var nodePath;Module["read"]=function shell_read(filename,binary){var ret;if(!nodeFS)nodeFS=require("fs");if(!nodePath)nodePath=require("path");filename=nodePath["normalize"](filename);ret=nodeFS["readFileSync"](filename);return binary?ret:ret.toString()};Module["readBinary"]=function readBinary(filename){var ret=Module["read"](filename,true);if(!ret.buffer){ret=new Uint8Array(ret)}assert(ret.buffer);return ret};if(process["argv"].length>1){Module["thisProgram"]=process["argv"][1].replace(/\\/g,"/")}Module["arguments"]=process["argv"].slice(2);process["on"]("uncaughtException",(function(ex){if(!(ex instanceof ExitStatus)){throw ex}}));process["on"]("unhandledRejection",(function(reason,p){process["exit"](1)}));Module["quit"]=(function(status){process["exit"](status)});Module["inspect"]=(function(){return"[Emscripten Module object]"})}else if(ENVIRONMENT_IS_SHELL){if(typeof read!="undefined"){Module["read"]=function shell_read(f){return read(f)}}Module["readBinary"]=function readBinary(f){var data;if(typeof readbuffer==="function"){return new Uint8Array(readbuffer(f))}data=read(f,"binary");assert(typeof data==="object");return data};if(typeof scriptArgs!="undefined"){Module["arguments"]=scriptArgs}else if(typeof arguments!="undefined"){Module["arguments"]=arguments}if(typeof quit==="function"){Module["quit"]=(function(status){quit(status)})}}else if(ENVIRONMENT_IS_WEB||ENVIRONMENT_IS_WORKER){if(ENVIRONMENT_IS_WEB){if(document.currentScript){scriptDirectory=document.currentScript.src}}else{scriptDirectory=self.location.href}if(scriptDirectory.indexOf("blob:")!==0){scriptDirectory=scriptDirectory.substr(0,scriptDirectory.lastIndexOf("/")+1)}else{scriptDirectory=""}Module["read"]=function shell_read(url){var xhr=new XMLHttpRequest;xhr.open("GET",url,false);xhr.send(null);return xhr.responseText};if(ENVIRONMENT_IS_WORKER){Module["readBinary"]=function readBinary(url){var xhr=new XMLHttpRequest;xhr.open("GET",url,false);xhr.responseType="arraybuffer";xhr.send(null);return new Uint8Array(xhr.response)}}Module["readAsync"]=function readAsync(url,onload,onerror){var xhr=new XMLHttpRequest;xhr.open("GET",url,true);xhr.responseType="arraybuffer";xhr.onload=function xhr_onload(){if(xhr.status==200||xhr.status==0&&xhr.response){onload(xhr.response);return}onerror()};xhr.onerror=onerror;xhr.send(null)};Module["setWindowTitle"]=(function(title){document.title=title})}else{}var out=Module["print"]||(typeof console!=="undefined"?console.log.bind(console):typeof print!=="undefined"?print:null);var err=Module["printErr"]||(typeof printErr!=="undefined"?printErr:typeof console!=="undefined"&&console.warn.bind(console)||out);for(key in moduleOverrides){if(moduleOverrides.hasOwnProperty(key)){Module[key]=moduleOverrides[key]}}moduleOverrides=undefined;var STACK_ALIGN=16;function staticAlloc(size){var ret=STATICTOP;STATICTOP=STATICTOP+size+15&-16;return ret}function alignMemory(size,factor){if(!factor)factor=STACK_ALIGN;var ret=size=Math.ceil(size/factor)*factor;return ret}var asm2wasmImports={"f64-rem":(function(x,y){return x%y}),"debugger":(function(){debugger})};var functionPointers=new Array(0);var GLOBAL_BASE=1024;var ABORT=false;var EXITSTATUS=0;function assert(condition,text){if(!condition){abort("Assertion failed: "+text)}}function getCFunc(ident){var func=Module["_"+ident];assert(func,"Cannot call unknown function "+ident+", make sure it is exported");return func}var JSfuncs={"stackSave":(function(){stackSave()}),"stackRestore":(function(){stackRestore()}),"arrayToC":(function(arr){var ret=stackAlloc(arr.length);writeArrayToMemory(arr,ret);return ret}),"stringToC":(function(str){var ret=0;if(str!==null&&str!==undefined&&str!==0){var len=(str.length<<2)+1;ret=stackAlloc(len);stringToUTF8(str,ret,len)}return ret})};var toC={"string":JSfuncs["stringToC"],"array":JSfuncs["arrayToC"]};function ccall(ident,returnType,argTypes,args,opts){function convertReturnValue(ret){if(returnType==="string")return Pointer_stringify(ret);if(returnType==="boolean")return Boolean(ret);return ret}var func=getCFunc(ident);var cArgs=[];var stack=0;if(args){for(var i=0;i<args.length;i++){var converter=toC[argTypes[i]];if(converter){if(stack===0)stack=stackSave();cArgs[i]=converter(args[i])}else{cArgs[i]=args[i]}}}var ret=func.apply(null,cArgs);ret=convertReturnValue(ret);if(stack!==0)stackRestore(stack);return ret}function cwrap(ident,returnType,argTypes,opts){argTypes=argTypes||[];var numericArgs=argTypes.every((function(type){return type==="number"}));var numericRet=returnType!=="string";if(numericRet&&numericArgs&&!opts){return getCFunc(ident)}return(function(){return ccall(ident,returnType,argTypes,arguments,opts)})}function Pointer_stringify(ptr,length){if(length===0||!ptr)return"";var hasUtf=0;var t;var i=0;while(1){t=HEAPU8[ptr+i>>0];hasUtf|=t;if(t==0&&!length)break;i++;if(length&&i==length)break}if(!length)length=i;var ret="";if(hasUtf<128){var MAX_CHUNK=1024;var curr;while(length>0){curr=String.fromCharCode.apply(String,HEAPU8.subarray(ptr,ptr+Math.min(length,MAX_CHUNK)));ret=ret?ret+curr:curr;ptr+=MAX_CHUNK;length-=MAX_CHUNK}return ret}return UTF8ToString(ptr)}var UTF8Decoder=typeof TextDecoder!=="undefined"?new TextDecoder("utf8"):undefined;function UTF8ArrayToString(u8Array,idx){var endPtr=idx;while(u8Array[endPtr])++endPtr;if(endPtr-idx>16&&u8Array.subarray&&UTF8Decoder){return UTF8Decoder.decode(u8Array.subarray(idx,endPtr))}else{var u0,u1,u2,u3,u4,u5;var str="";while(1){u0=u8Array[idx++];if(!u0)return str;if(!(u0&128)){str+=String.fromCharCode(u0);continue}u1=u8Array[idx++]&63;if((u0&224)==192){str+=String.fromCharCode((u0&31)<<6|u1);continue}u2=u8Array[idx++]&63;if((u0&240)==224){u0=(u0&15)<<12|u1<<6|u2}else{u3=u8Array[idx++]&63;if((u0&248)==240){u0=(u0&7)<<18|u1<<12|u2<<6|u3}else{u4=u8Array[idx++]&63;if((u0&252)==248){u0=(u0&3)<<24|u1<<18|u2<<12|u3<<6|u4}else{u5=u8Array[idx++]&63;u0=(u0&1)<<30|u1<<24|u2<<18|u3<<12|u4<<6|u5}}}if(u0<65536){str+=String.fromCharCode(u0)}else{var ch=u0-65536;str+=String.fromCharCode(55296|ch>>10,56320|ch&1023)}}}}function UTF8ToString(ptr){return UTF8ArrayToString(HEAPU8,ptr)}function stringToUTF8Array(str,outU8Array,outIdx,maxBytesToWrite){if(!(maxBytesToWrite>0))return 0;var startIdx=outIdx;var endIdx=outIdx+maxBytesToWrite-1;for(var i=0;i<str.length;++i){var u=str.charCodeAt(i);if(u>=55296&&u<=57343){var u1=str.charCodeAt(++i);u=65536+((u&1023)<<10)|u1&1023}if(u<=127){if(outIdx>=endIdx)break;outU8Array[outIdx++]=u}else if(u<=2047){if(outIdx+1>=endIdx)break;outU8Array[outIdx++]=192|u>>6;outU8Array[outIdx++]=128|u&63}else if(u<=65535){if(outIdx+2>=endIdx)break;outU8Array[outIdx++]=224|u>>12;outU8Array[outIdx++]=128|u>>6&63;outU8Array[outIdx++]=128|u&63}else if(u<=2097151){if(outIdx+3>=endIdx)break;outU8Array[outIdx++]=240|u>>18;outU8Array[outIdx++]=128|u>>12&63;outU8Array[outIdx++]=128|u>>6&63;outU8Array[outIdx++]=128|u&63}else if(u<=67108863){if(outIdx+4>=endIdx)break;outU8Array[outIdx++]=248|u>>24;outU8Array[outIdx++]=128|u>>18&63;outU8Array[outIdx++]=128|u>>12&63;outU8Array[outIdx++]=128|u>>6&63;outU8Array[outIdx++]=128|u&63}else{if(outIdx+5>=endIdx)break;outU8Array[outIdx++]=252|u>>30;outU8Array[outIdx++]=128|u>>24&63;outU8Array[outIdx++]=128|u>>18&63;outU8Array[outIdx++]=128|u>>12&63;outU8Array[outIdx++]=128|u>>6&63;outU8Array[outIdx++]=128|u&63}}outU8Array[outIdx]=0;return outIdx-startIdx}function stringToUTF8(str,outPtr,maxBytesToWrite){return stringToUTF8Array(str,HEAPU8,outPtr,maxBytesToWrite)}function lengthBytesUTF8(str){var len=0;for(var i=0;i<str.length;++i){var u=str.charCodeAt(i);if(u>=55296&&u<=57343)u=65536+((u&1023)<<10)|str.charCodeAt(++i)&1023;if(u<=127){++len}else if(u<=2047){len+=2}else if(u<=65535){len+=3}else if(u<=2097151){len+=4}else if(u<=67108863){len+=5}else{len+=6}}return len}var UTF16Decoder=typeof TextDecoder!=="undefined"?new TextDecoder("utf-16le"):undefined;function allocateUTF8OnStack(str){var size=lengthBytesUTF8(str)+1;var ret=stackAlloc(size);stringToUTF8Array(str,HEAP8,ret,size);return ret}var WASM_PAGE_SIZE=65536;var ASMJS_PAGE_SIZE=16777216;function alignUp(x,multiple){if(x%multiple>0){x+=multiple-x%multiple}return x}var buffer,HEAP8,HEAPU8,HEAP16,HEAPU16,HEAP32,HEAPU32,HEAPF32,HEAPF64;function updateGlobalBuffer(buf){Module["buffer"]=buffer=buf}function updateGlobalBufferViews(){Module["HEAP8"]=HEAP8=new Int8Array(buffer);Module["HEAP16"]=HEAP16=new Int16Array(buffer);Module["HEAP32"]=HEAP32=new Int32Array(buffer);Module["HEAPU8"]=HEAPU8=new Uint8Array(buffer);Module["HEAPU16"]=HEAPU16=new Uint16Array(buffer);Module["HEAPU32"]=HEAPU32=new Uint32Array(buffer);Module["HEAPF32"]=HEAPF32=new Float32Array(buffer);Module["HEAPF64"]=HEAPF64=new Float64Array(buffer)}var STATIC_BASE,STATICTOP,staticSealed;var STACK_BASE,STACKTOP,STACK_MAX;var DYNAMIC_BASE,DYNAMICTOP_PTR;STATIC_BASE=STATICTOP=STACK_BASE=STACKTOP=STACK_MAX=DYNAMIC_BASE=DYNAMICTOP_PTR=0;staticSealed=false;function abortOnCannotGrowMemory(){abort("Cannot enlarge memory arrays. Either (1) compile with  -s TOTAL_MEMORY=X  with X higher than the current value "+TOTAL_MEMORY+", (2) compile with  -s ALLOW_MEMORY_GROWTH=1  which allows increasing the size at runtime, or (3) if you want malloc to return NULL (0) instead of this abort, compile with  -s ABORTING_MALLOC=0 ")}function enlargeMemory(){abortOnCannotGrowMemory()}var TOTAL_STACK=Module["TOTAL_STACK"]||5242880;var TOTAL_MEMORY=Module["TOTAL_MEMORY"]||16777216;if(TOTAL_MEMORY<TOTAL_STACK)err("TOTAL_MEMORY should be larger than TOTAL_STACK, was "+TOTAL_MEMORY+"! (TOTAL_STACK="+TOTAL_STACK+")");if(Module["buffer"]){buffer=Module["buffer"]}else{if(typeof WebAssembly==="object"&&typeof WebAssembly.Memory==="function"){Module["wasmMemory"]=new WebAssembly.Memory({"initial":TOTAL_MEMORY/WASM_PAGE_SIZE,"maximum":TOTAL_MEMORY/WASM_PAGE_SIZE});buffer=Module["wasmMemory"].buffer}else{buffer=new ArrayBuffer(TOTAL_MEMORY)}Module["buffer"]=buffer}updateGlobalBufferViews();function getTotalMemory(){return TOTAL_MEMORY}function callRuntimeCallbacks(callbacks){while(callbacks.length>0){var callback=callbacks.shift();if(typeof callback=="function"){callback();continue}var func=callback.func;if(typeof func==="number"){if(callback.arg===undefined){Module["dynCall_v"](func)}else{Module["dynCall_vi"](func,callback.arg)}}else{func(callback.arg===undefined?null:callback.arg)}}}var __ATPRERUN__=[];var __ATINIT__=[];var __ATMAIN__=[];var __ATEXIT__=[];var __ATPOSTRUN__=[];var runtimeInitialized=false;var runtimeExited=false;function preRun(){if(Module["preRun"]){if(typeof Module["preRun"]=="function")Module["preRun"]=[Module["preRun"]];while(Module["preRun"].length){addOnPreRun(Module["preRun"].shift())}}callRuntimeCallbacks(__ATPRERUN__)}function ensureInitRuntime(){if(runtimeInitialized)return;runtimeInitialized=true;callRuntimeCallbacks(__ATINIT__)}function preMain(){callRuntimeCallbacks(__ATMAIN__)}function exitRuntime(){callRuntimeCallbacks(__ATEXIT__);runtimeExited=true}function postRun(){if(Module["postRun"]){if(typeof Module["postRun"]=="function")Module["postRun"]=[Module["postRun"]];while(Module["postRun"].length){addOnPostRun(Module["postRun"].shift())}}callRuntimeCallbacks(__ATPOSTRUN__)}function addOnPreRun(cb){__ATPRERUN__.unshift(cb)}function addOnPostRun(cb){__ATPOSTRUN__.unshift(cb)}function writeArrayToMemory(array,buffer){HEAP8.set(array,buffer)}var runDependencies=0;var runDependencyWatcher=null;var dependenciesFulfilled=null;function addRunDependency(id){runDependencies++;if(Module["monitorRunDependencies"]){Module["monitorRunDependencies"](runDependencies)}}function removeRunDependency(id){runDependencies--;if(Module["monitorRunDependencies"]){Module["monitorRunDependencies"](runDependencies)}if(runDependencies==0){if(runDependencyWatcher!==null){clearInterval(runDependencyWatcher);runDependencyWatcher=null}if(dependenciesFulfilled){var callback=dependenciesFulfilled;dependenciesFulfilled=null;callback()}}}Module["preloadedImages"]={};Module["preloadedAudios"]={};var dataURIPrefix="data:application/octet-stream;base64,";function isDataURI(filename){return String.prototype.startsWith?filename.startsWith(dataURIPrefix):filename.indexOf(dataURIPrefix)===0}function integrateWasmJS(){var wasmTextFile="elm.wast";var wasmBinaryFile="elm.wasm";var asmjsCodeFile="elm.temp.asm.js";if(!isDataURI(wasmTextFile)){wasmTextFile=locateFile(wasmTextFile)}if(!isDataURI(wasmBinaryFile)){wasmBinaryFile=locateFile(wasmBinaryFile)}if(!isDataURI(asmjsCodeFile)){asmjsCodeFile=locateFile(asmjsCodeFile)}var wasmPageSize=64*1024;var info={"global":null,"env":null,"asm2wasm":asm2wasmImports,"parent":Module};var exports=null;function mergeMemory(newBuffer){var oldBuffer=Module["buffer"];if(newBuffer.byteLength<oldBuffer.byteLength){err("the new buffer in mergeMemory is smaller than the previous one. in native wasm, we should grow memory here")}var oldView=new Int8Array(oldBuffer);var newView=new Int8Array(newBuffer);newView.set(oldView);updateGlobalBuffer(newBuffer);updateGlobalBufferViews()}function fixImports(imports){return imports}function getBinary(){try{if(Module["wasmBinary"]){return new Uint8Array(Module["wasmBinary"])}if(Module["readBinary"]){return Module["readBinary"](wasmBinaryFile)}else{throw"both async and sync fetching of the wasm failed"}}catch(err){abort(err)}}function getBinaryPromise(){if(!Module["wasmBinary"]&&(ENVIRONMENT_IS_WEB||ENVIRONMENT_IS_WORKER)&&typeof fetch==="function"){return fetch(wasmBinaryFile,{credentials:"same-origin"}).then((function(response){if(!response["ok"]){throw"failed to load wasm binary file at '"+wasmBinaryFile+"'"}return response["arrayBuffer"]()})).catch((function(){return getBinary()}))}return new Promise((function(resolve,reject){resolve(getBinary())}))}function doNativeWasm(global,env,providedBuffer){if(typeof WebAssembly!=="object"){err("no native wasm support detected");return false}if(!(Module["wasmMemory"]instanceof WebAssembly.Memory)){err("no native wasm Memory in use");return false}env["memory"]=Module["wasmMemory"];info["global"]={"NaN":NaN,"Infinity":Infinity};info["global.Math"]=Math;info["env"]=env;function receiveInstance(instance,module){exports=instance.exports;if(exports.memory)mergeMemory(exports.memory);Module["asm"]=exports;Module["usingWasm"]=true;removeRunDependency("wasm-instantiate")}addRunDependency("wasm-instantiate");if(Module["instantiateWasm"]){try{return Module["instantiateWasm"](info,receiveInstance)}catch(e){err("Module.instantiateWasm callback failed with error: "+e);return false}}function receiveInstantiatedSource(output){receiveInstance(output["instance"],output["module"])}function instantiateArrayBuffer(receiver){getBinaryPromise().then((function(binary){return WebAssembly.instantiate(binary,info)})).then(receiver).catch((function(reason){err("failed to asynchronously prepare wasm: "+reason);abort(reason)}))}if(!Module["wasmBinary"]&&typeof WebAssembly.instantiateStreaming==="function"&&!isDataURI(wasmBinaryFile)&&typeof fetch==="function"){WebAssembly.instantiateStreaming(fetch(wasmBinaryFile,{credentials:"same-origin"}),info).then(receiveInstantiatedSource).catch((function(reason){err("wasm streaming compile failed: "+reason);err("falling back to ArrayBuffer instantiation");instantiateArrayBuffer(receiveInstantiatedSource)}))}else{instantiateArrayBuffer(receiveInstantiatedSource)}return{}}Module["asmPreload"]=Module["asm"];var asmjsReallocBuffer=Module["reallocBuffer"];var wasmReallocBuffer=(function(size){var PAGE_MULTIPLE=Module["usingWasm"]?WASM_PAGE_SIZE:ASMJS_PAGE_SIZE;size=alignUp(size,PAGE_MULTIPLE);var old=Module["buffer"];var oldSize=old.byteLength;if(Module["usingWasm"]){try{var result=Module["wasmMemory"].grow((size-oldSize)/wasmPageSize);if(result!==(-1|0)){return Module["buffer"]=Module["wasmMemory"].buffer}else{return null}}catch(e){return null}}});Module["reallocBuffer"]=(function(size){if(finalMethod==="asmjs"){return asmjsReallocBuffer(size)}else{return wasmReallocBuffer(size)}});var finalMethod="";Module["asm"]=(function(global,env,providedBuffer){env=fixImports(env);if(!env["table"]){var TABLE_SIZE=Module["wasmTableSize"];if(TABLE_SIZE===undefined)TABLE_SIZE=1024;var MAX_TABLE_SIZE=Module["wasmMaxTableSize"];if(typeof WebAssembly==="object"&&typeof WebAssembly.Table==="function"){if(MAX_TABLE_SIZE!==undefined){env["table"]=new WebAssembly.Table({"initial":TABLE_SIZE,"maximum":MAX_TABLE_SIZE,"element":"anyfunc"})}else{env["table"]=new WebAssembly.Table({"initial":TABLE_SIZE,element:"anyfunc"})}}else{env["table"]=new Array(TABLE_SIZE)}Module["wasmTable"]=env["table"]}if(!env["memoryBase"]){env["memoryBase"]=Module["STATIC_BASE"]}if(!env["tableBase"]){env["tableBase"]=0}var exports;exports=doNativeWasm(global,env,providedBuffer);assert(exports,"no binaryen method succeeded.");return exports})}integrateWasmJS();STATIC_BASE=GLOBAL_BASE;STATICTOP=STATIC_BASE+11808;__ATINIT__.push();var STATIC_BUMP=11808;Module["STATIC_BASE"]=STATIC_BASE;Module["STATIC_BUMP"]=STATIC_BUMP;STATICTOP+=16;function ___assert_fail(condition,filename,line,func){abort("Assertion failed: "+Pointer_stringify(condition)+", at: "+[filename?Pointer_stringify(filename):"unknown filename",line,func?Pointer_stringify(func):"unknown function"])}var SYSCALLS={varargs:0,get:(function(varargs){SYSCALLS.varargs+=4;var ret=HEAP32[SYSCALLS.varargs-4>>2];return ret}),getStr:(function(){var ret=Pointer_stringify(SYSCALLS.get());return ret}),get64:(function(){var low=SYSCALLS.get(),high=SYSCALLS.get();if(low>=0)assert(high===0);else assert(high===-1);return low}),getZero:(function(){assert(SYSCALLS.get()===0)})};function ___syscall140(which,varargs){SYSCALLS.varargs=varargs;try{var stream=SYSCALLS.getStreamFromFD(),offset_high=SYSCALLS.get(),offset_low=SYSCALLS.get(),result=SYSCALLS.get(),whence=SYSCALLS.get();var offset=offset_low;FS.llseek(stream,offset,whence);HEAP32[result>>2]=stream.position;if(stream.getdents&&offset===0&&whence===0)stream.getdents=null;return 0}catch(e){if(typeof FS==="undefined"||!(e instanceof FS.ErrnoError))abort(e);return-e.errno}}function ___syscall146(which,varargs){SYSCALLS.varargs=varargs;try{var stream=SYSCALLS.get(),iov=SYSCALLS.get(),iovcnt=SYSCALLS.get();var ret=0;if(!___syscall146.buffers){___syscall146.buffers=[null,[],[]];___syscall146.printChar=(function(stream,curr){var buffer=___syscall146.buffers[stream];assert(buffer);if(curr===0||curr===10){(stream===1?out:err)(UTF8ArrayToString(buffer,0));buffer.length=0}else{buffer.push(curr)}})}for(var i=0;i<iovcnt;i++){var ptr=HEAP32[iov+i*8>>2];var len=HEAP32[iov+(i*8+4)>>2];for(var j=0;j<len;j++){___syscall146.printChar(stream,HEAPU8[ptr+j])}ret+=len}return ret}catch(e){if(typeof FS==="undefined"||!(e instanceof FS.ErrnoError))abort(e);return-e.errno}}function ___syscall54(which,varargs){SYSCALLS.varargs=varargs;try{return 0}catch(e){if(typeof FS==="undefined"||!(e instanceof FS.ErrnoError))abort(e);return-e.errno}}function ___syscall6(which,varargs){SYSCALLS.varargs=varargs;try{var stream=SYSCALLS.getStreamFromFD();FS.close(stream);return 0}catch(e){if(typeof FS==="undefined"||!(e instanceof FS.ErrnoError))abort(e);return-e.errno}}function _evalWasmThunkInJs(){err("missing function: evalWasmThunkInJs");abort(-1)}function _emscripten_memcpy_big(dest,src,num){HEAPU8.set(HEAPU8.subarray(src,src+num),dest);return dest}function ___setErrNo(value){if(Module["___errno_location"])HEAP32[Module["___errno_location"]()>>2]=value;return value}DYNAMICTOP_PTR=staticAlloc(4);STACK_BASE=STACKTOP=alignMemory(STATICTOP);STACK_MAX=STACK_BASE+TOTAL_STACK;DYNAMIC_BASE=alignMemory(STACK_MAX);HEAP32[DYNAMICTOP_PTR>>2]=DYNAMIC_BASE;staticSealed=true;Module["wasmTableSize"]=168;Module["wasmMaxTableSize"]=168;Module.asmGlobalArg={};Module.asmLibraryArg={"abort":abort,"enlargeMemory":enlargeMemory,"getTotalMemory":getTotalMemory,"abortOnCannotGrowMemory":abortOnCannotGrowMemory,"___assert_fail":___assert_fail,"___setErrNo":___setErrNo,"___syscall140":___syscall140,"___syscall146":___syscall146,"___syscall54":___syscall54,"___syscall6":___syscall6,"_emscripten_memcpy_big":_emscripten_memcpy_big,"_evalWasmThunkInJs":_evalWasmThunkInJs,"DYNAMICTOP_PTR":DYNAMICTOP_PTR,"STACKTOP":STACKTOP};var asm=Module["asm"](Module.asmGlobalArg,Module.asmLibraryArg,buffer);Module["asm"]=asm;var ___errno_location=Module["___errno_location"]=(function(){return Module["asm"]["___errno_location"].apply(null,arguments)});var _collectGarbage=Module["_collectGarbage"]=(function(){return Module["asm"]["_collectGarbage"].apply(null,arguments)});var _debugEvaluatorName=Module["_debugEvaluatorName"]=(function(){return Module["asm"]["_debugEvaluatorName"].apply(null,arguments)});var _debugHeapState=Module["_debugHeapState"]=(function(){return Module["asm"]["_debugHeapState"].apply(null,arguments)});var _evalClosure=Module["_evalClosure"]=(function(){return Module["asm"]["_evalClosure"].apply(null,arguments)});var _finishWritingAt=Module["_finishWritingAt"]=(function(){return Module["asm"]["_finishWritingAt"].apply(null,arguments)});var _getFalse=Module["_getFalse"]=(function(){return Module["asm"]["_getFalse"].apply(null,arguments)});var _getJsNull=Module["_getJsNull"]=(function(){return Module["asm"]["_getJsNull"].apply(null,arguments)});var _getMaxWriteAddr=Module["_getMaxWriteAddr"]=(function(){return Module["asm"]["_getMaxWriteAddr"].apply(null,arguments)});var _getNextFieldGroup=Module["_getNextFieldGroup"]=(function(){return Module["asm"]["_getNextFieldGroup"].apply(null,arguments)});var _getNextMain=Module["_getNextMain"]=(function(){return Module["asm"]["_getNextMain"].apply(null,arguments)});var _getNil=Module["_getNil"]=(function(){return Module["asm"]["_getNil"].apply(null,arguments)});var _getTrue=Module["_getTrue"]=(function(){return Module["asm"]["_getTrue"].apply(null,arguments)});var _getUnit=Module["_getUnit"]=(function(){return Module["asm"]["_getUnit"].apply(null,arguments)});var _getWriteAddr=Module["_getWriteAddr"]=(function(){return Module["asm"]["_getWriteAddr"].apply(null,arguments)});var _main=Module["_main"]=(function(){return Module["asm"]["_main"].apply(null,arguments)});var _readF64=Module["_readF64"]=(function(){return Module["asm"]["_readF64"].apply(null,arguments)});var _writeF64=Module["_writeF64"]=(function(){return Module["asm"]["_writeF64"].apply(null,arguments)});var stackAlloc=Module["stackAlloc"]=(function(){return Module["asm"]["stackAlloc"].apply(null,arguments)});var stackRestore=Module["stackRestore"]=(function(){return Module["asm"]["stackRestore"].apply(null,arguments)});var stackSave=Module["stackSave"]=(function(){return Module["asm"]["stackSave"].apply(null,arguments)});Module["asm"]=asm;Module["ccall"]=ccall;Module["cwrap"]=cwrap;function ExitStatus(status){this.name="ExitStatus";this.message="Program terminated with exit("+status+")";this.status=status}ExitStatus.prototype=new Error;ExitStatus.prototype.constructor=ExitStatus;var initialStackTop;var calledMain=false;dependenciesFulfilled=function runCaller(){if(!Module["calledRun"])run();if(!Module["calledRun"])dependenciesFulfilled=runCaller};Module["callMain"]=function callMain(args){args=args||[];ensureInitRuntime();var argc=args.length+1;var argv=stackAlloc((argc+1)*4);HEAP32[argv>>2]=allocateUTF8OnStack(Module["thisProgram"]);for(var i=1;i<argc;i++){HEAP32[(argv>>2)+i]=allocateUTF8OnStack(args[i-1])}HEAP32[(argv>>2)+argc]=0;try{var ret=Module["_main"](argc,argv,0);exit(ret,true)}catch(e){if(e instanceof ExitStatus){return}else if(e=="SimulateInfiniteLoop"){Module["noExitRuntime"]=true;return}else{var toLog=e;if(e&&typeof e==="object"&&e.stack){toLog=[e,e.stack]}err("exception thrown: "+toLog);Module["quit"](1,e)}}finally{calledMain=true}};function run(args){args=args||Module["arguments"];if(runDependencies>0){return}preRun();if(runDependencies>0)return;if(Module["calledRun"])return;function doRun(){if(Module["calledRun"])return;Module["calledRun"]=true;if(ABORT)return;ensureInitRuntime();preMain();if(Module["onRuntimeInitialized"])Module["onRuntimeInitialized"]();if(Module["_main"]&&shouldRunNow)Module["callMain"](args);postRun()}if(Module["setStatus"]){Module["setStatus"]("Running...");setTimeout((function(){setTimeout((function(){Module["setStatus"]("")}),1);doRun()}),1)}else{doRun()}}Module["run"]=run;function exit(status,implicit){if(implicit&&Module["noExitRuntime"]&&status===0){return}if(Module["noExitRuntime"]){}else{ABORT=true;EXITSTATUS=status;STACKTOP=initialStackTop;exitRuntime();if(Module["onExit"])Module["onExit"](status)}Module["quit"](status,new ExitStatus(status))}function abort(what){if(Module["onAbort"]){Module["onAbort"](what)}if(what!==undefined){out(what);err(what);what=JSON.stringify(what)}else{what=""}ABORT=true;EXITSTATUS=1;throw"abort("+what+"). Build with -s ASSERTIONS=1 for more info."}Module["abort"]=abort;if(Module["preInit"]){if(typeof Module["preInit"]=="function")Module["preInit"]=[Module["preInit"]];while(Module["preInit"].length>0){Module["preInit"].pop()()}}var shouldRunNow=true;if(Module["noInitialRun"]){shouldRunNow=false}Module["noExitRuntime"]=true;run()





  return EmscriptenModule;
}
)();
if (typeof exports === 'object' && typeof module === 'object')
    module.exports = EmscriptenModule;
  else if (typeof define === 'function' && define['amd'])
    define([], function() { return EmscriptenModule; });
  else if (typeof exports === 'object')
    exports["EmscriptenModule"] = EmscriptenModule;
  var wasmWrapper;
(function(scope){
'use strict';

var onReadyCallback;
scope['Elm'] = {
  onReady: function(callback) {
    onReadyCallback = callback;
  }
};

scope['EmscriptenModule'] = scope['EmscriptenModule'] || {};
scope['EmscriptenModule'].postRun = function postRun() {


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
var $elm$core$Basics$EQ = {$: 'EQ'};
// Enum
var $elm$core$Basics$GT = {$: 'GT'};
// Enum
var $elm$core$Basics$LT = {$: 'LT'};
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
var $elm$core$Basics$apL = /* Function */F2(
	function (f, x) {
		return /* Call *//* VarLocal */f(/* VarLocal */x);
	});
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
var $elm$json$Json$Decode$succeed = /* VarKernel */_Json_succeed;
// Define, deps=$elm$kernel$Json$$
var $elm$json$Json$Decode$map = /* VarKernel */_Json_map1;
// Define, deps=$elm$kernel$Json$$
var $elm$json$Json$Decode$map2 = /* VarKernel */_Json_map2;
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
function wrapEmscriptenForElm(wasmBuffer, wasmExports, generatedAppTypes, kernelFuncRecord) {
    /* --------------------------------------------------
  
                 INITIALISATION & CONSTANTS
  
    -------------------------------------------------- */
    const mem32 = new Uint32Array(wasmBuffer);
    const mem16 = new Uint16Array(wasmBuffer);
    const wasmConstAddrs = (function () {
        const Unit = wasmExports._getUnit();
        const Nil = wasmExports._getNil();
        const True = wasmExports._getTrue();
        const False = wasmExports._getFalse();
        const JsNull = wasmExports._getJsNull();
        return {
            Unit,
            Nil,
            True,
            False,
            JsNull,
            [Unit]: _Utils_Tuple0,
            [Nil]: _List_Nil,
            [True]: true,
            [False]: false,
            [JsNull]: null
        };
    })();
    const CTOR_KERNEL_ARRAY = 0xffffffff;
    const appTypes = {
        ctors: arrayToEnum(generatedAppTypes.ctors),
        fields: arrayToEnum(generatedAppTypes.fields),
        fieldGroups: generatedAppTypes.fieldGroups.reduce((enumObj, name) => {
            const addr = wasmExports._getNextFieldGroup();
            enumObj[name] = addr;
            enumObj[addr] = name.split(' ');
            return enumObj;
        }, {})
    };
    function arrayToEnum(names) {
        return names.reduce((enumObj, name, index) => {
            enumObj[name] = index;
            enumObj[index] = name;
            return enumObj;
        }, {});
    }
    const kernelFunctions = Object.values(kernelFuncRecord);
    const kernelFunctionNames = Object.keys(kernelFuncRecord); // for debug
    const WORD = 4;
    const TAG_MASK = 0xf0000000;
    const TAG_SHIFT = 28;
    const SIZE_MASK = 0x0fffffff;
    const SIZE_SHIFT = 0;
    const NEVER_EVALUATE = 0xffff;
    const KERNEL_CTOR_OFFSET = 1024 * 1000;
    const textDecoder = new TextDecoder('utf-16le');
    const identity = (f) => f;
    const elmFunctionWrappers = [
        identity,
        identity,
        F2,
        F3,
        F4,
        F5,
        F6,
        F7,
        F8,
        F9
    ];
    let Tag;
    (function (Tag) {
        Tag[Tag["Int"] = 0] = "Int";
        Tag[Tag["Float"] = 1] = "Float";
        Tag[Tag["Char"] = 2] = "Char";
        Tag[Tag["String"] = 3] = "String";
        Tag[Tag["List"] = 4] = "List";
        Tag[Tag["Tuple2"] = 5] = "Tuple2";
        Tag[Tag["Tuple3"] = 6] = "Tuple3";
        Tag[Tag["Custom"] = 7] = "Custom";
        Tag[Tag["Record"] = 8] = "Record";
        Tag[Tag["FieldGroup"] = 9] = "FieldGroup";
        Tag[Tag["Closure"] = 10] = "Closure";
        Tag[Tag["GcException"] = 11] = "GcException";
        Tag[Tag["GcStackEmpty"] = 12] = "GcStackEmpty";
        Tag[Tag["GcStackPush"] = 13] = "GcStackPush";
        Tag[Tag["GcStackPop"] = 14] = "GcStackPop";
        Tag[Tag["GcStackTailCall"] = 15] = "GcStackTailCall";
    })(Tag || (Tag = {}));
    function readWasmValue(addr) {
        if (!addr)
            return null;
        const index = addr >> 2;
        const header = mem32[index];
        const tag = (header & TAG_MASK) >>> TAG_SHIFT;
        const size = (header & SIZE_MASK) >>> SIZE_SHIFT;
        switch (tag) {
            case Tag.Int: {
                return mem32[index + 1];
            }
            case Tag.Float: {
                return wasmExports._readF64(addr + 2 * WORD);
            }
            case Tag.Char:
            case Tag.String: {
                const idx16 = (index + 1) << 1;
                let len16 = (size - 1) << 1;
                if (mem16[idx16 + len16 - 1] === 0)
                    len16--;
                const words16 = mem16.slice(idx16, idx16 + len16);
                const jsString = textDecoder.decode(words16);
                return tag === Tag.Char ? _Utils_chr(jsString) : jsString;
            }
            case Tag.List: {
                return addr === wasmConstAddrs.Nil
                    ? _List_Nil
                    : _List_Cons(readWasmValue(mem32[index + 1]), readWasmValue(mem32[index + 2]));
            }
            case Tag.Tuple2: {
                return _Utils_Tuple2(readWasmValue(mem32[index + 1]), readWasmValue(mem32[index + 2]));
            }
            case Tag.Tuple3: {
                return _Utils_Tuple3(readWasmValue(mem32[index + 1]), readWasmValue(mem32[index + 2]), readWasmValue(mem32[index + 3]));
            }
            case Tag.Custom: {
                const elmConst = wasmConstAddrs[addr]; // True/False/Unit/JsNull
                if (elmConst !== undefined)
                    return elmConst;
                const wasmCtor = mem32[index + 1];
                if (wasmCtor === CTOR_KERNEL_ARRAY) {
                    const kernelArray = [];
                    mem32.slice(index + 2, index + size).forEach(childAddr => {
                        kernelArray.push(readWasmValue(childAddr));
                    });
                    return kernelArray;
                }
                const jsCtor = wasmCtor >= KERNEL_CTOR_OFFSET
                    ? wasmCtor - KERNEL_CTOR_OFFSET
                    : appTypes.ctors[wasmCtor];
                const custom = { $: jsCtor };
                const fieldNames = 'abcdefghijklmnopqrstuvwxyz';
                for (let i = 0; i < size - 2; i++) {
                    const field = fieldNames[i];
                    const childAddr = mem32[index + 2 + i];
                    custom[field] = readWasmValue(childAddr);
                }
                return custom;
            }
            case Tag.Record: {
                const record = {};
                const fgAddr = mem32[index + 1];
                const fieldNames = appTypes.fieldGroups[fgAddr] || readWasmValue(fgAddr);
                fieldNames.forEach((fieldName, i) => {
                    const valAddr = mem32[index + 2 + i];
                    record[fieldName] = readWasmValue(valAddr);
                });
                return record;
            }
            case Tag.FieldGroup: {
                const fieldNames = [];
                mem32.slice(index + 2, index + size).forEach(fieldId => {
                    fieldNames.push(appTypes.fields[fieldId]);
                });
                return fieldNames;
            }
            case Tag.Closure: {
                const idx16 = index << 1;
                const metadata = {
                    n_values: mem16[idx16 + 2],
                    max_values: mem16[idx16 + 3],
                    evaluator: mem32[index + 2],
                    argsIndex: index + 3
                };
                return metadata.max_values === NEVER_EVALUATE
                    ? evalKernelThunk(metadata)
                    : createWasmCallback(metadata);
            }
            default:
                throw new Error('Tried to decode value with unsupported tag ' +
                    (Tag[tag] || '0x' + tag.toString(16)));
        }
    }
    function evalKernelThunk(metadata) {
        let { n_values, evaluator, argsIndex } = metadata;
        let kernelFn = kernelFunctions[evaluator];
        while (n_values) {
            let f;
            let nArgs;
            if (kernelFn.a && kernelFn.f && n_values >= kernelFn.a) {
                f = kernelFn.f;
                nArgs = kernelFn.a;
            }
            else {
                f = kernelFn;
                nArgs = kernelFn.length || 1;
            }
            const args = [];
            mem32.slice(argsIndex, argsIndex + nArgs).forEach(argAddr => {
                args.push(readWasmValue(argAddr));
            });
            n_values -= nArgs;
            argsIndex += nArgs;
            kernelFn = f(...args);
        }
        return kernelFn;
    }
    function createWasmCallback(metadata) {
        const { n_values, max_values, evaluator, argsIndex } = metadata;
        const freeVars = [];
        for (let i = argsIndex; i < argsIndex + n_values; i++) {
            freeVars.push(readWasmValue(mem32[i]));
        }
        function wasmCallback() {
            const args = freeVars.slice();
            for (let i = 0; i < arguments.length; i++) {
                args.push(arguments[i]);
            }
            const n_values = args.length;
            if (n_values !== max_values) {
                console.error({ wasmCallback, args });
                throw new Error(`Trying to call a Wasm Closure with ${n_values} args instead of ${max_values}!`);
            }
            const builder = {
                body: [(max_values << 16) | n_values, evaluator],
                jsChildren: args,
                bodyWriter: null
            };
            const addr = handleWasmWrite((startIndex) => {
                return writeFromBuilder(startIndex, builder, Tag.Closure);
            });
            const resultAddr = wasmExports._evalClosure(addr);
            const resultValue = readWasmValue(resultAddr);
            return resultValue;
        }
        // Attach info in case we have to write this Closure back to Wasm
        wasmCallback.freeVars = freeVars;
        wasmCallback.evaluator = evaluator;
        wasmCallback.max_values = max_values;
        const arity = max_values - n_values;
        const FN = elmFunctionWrappers[arity];
        return FN(wasmCallback);
    }
    /* --------------------------------------------------
  
                  WRITE ELM VALUES TO WASM
  
    -------------------------------------------------- */
    let maxWriteIndex32;
    let maxWriteIndex16;
    const heapOverflowError = new Error('Wasm heap overflow');
    function write32(index, value) {
        if (index > maxWriteIndex32) {
            throw heapOverflowError;
        }
        mem32[index] = value;
    }
    function handleWasmWrite(writer) {
        for (let attempts = 0; attempts < 2; attempts++) {
            try {
                const maxAddr = wasmExports._getMaxWriteAddr();
                maxWriteIndex16 = maxAddr >> 1;
                maxWriteIndex32 = maxAddr >> 2;
                const startAddr = wasmExports._getWriteAddr();
                const startIndex = startAddr >> 2;
                const result = writer(startIndex);
                wasmExports._finishWritingAt(result.nextIndex << 2);
                return result.addr;
            }
            catch (e) {
                if (e === heapOverflowError) {
                    wasmExports._collectGarbage();
                }
                else {
                    console.error(e);
                    throw e;
                }
            }
        }
        throw new Error('Failed to write to Wasm');
    }
    /**
     * Write an Elm value to the Wasm memory
     * Serialises to bytes before writing
     * May throw an error
     */
    function writeWasmValue(nextIndex, value) {
        const typeInfo = detectElmType(value);
        switch (typeInfo.kind) {
            case 'constAddr':
                return { addr: typeInfo.value, nextIndex };
            case 'tag': {
                const tag = typeInfo.value;
                const builder = wasmBuilder(tag, value);
                return writeFromBuilder(nextIndex, builder, tag);
            }
            case 'kernelArray': {
                const builder = {
                    body: [CTOR_KERNEL_ARRAY],
                    jsChildren: value,
                    bodyWriter: null
                };
                return writeFromBuilder(nextIndex, builder, Tag.Custom);
            }
        }
    }
    class FieldGroup {
        constructor(fieldNames) {
            this.fieldNames = fieldNames;
        }
    }
    function detectElmType(elmValue) {
        if (elmValue === null || elmValue === undefined) {
            return { kind: 'constAddr', value: wasmConstAddrs.JsNull };
        }
        switch (typeof elmValue) {
            case 'number': {
                // There's no way to tell `1 : Int` from `1.0 : Float` at this low level. But `1.2` is definitely a Float.
                // So _for now_ take a _horribly unsafe_ guess, by checking if it's a round number or not.
                // Not cool. This is Elm! Long term, the ambiguity needs to be solved at some higher level.
                // Maybe some lib like `JSON.Encode` so the app dev decides? Pity for it not to be automatic though!
                const isRoundNumberSoGuessInt = elmValue === Math.round(elmValue);
                return {
                    kind: 'tag',
                    value: isRoundNumberSoGuessInt ? Tag.Int : Tag.Float
                };
            }
            case 'string':
                return { kind: 'tag', value: Tag.String };
            case 'boolean':
                return {
                    kind: 'constAddr',
                    value: elmValue ? wasmConstAddrs.True : wasmConstAddrs.False
                };
            case 'function':
                return { kind: 'tag', value: Tag.Closure };
            case 'object': {
                if (elmValue instanceof String) {
                    return { kind: 'tag', value: Tag.Char };
                }
                if (elmValue instanceof FieldGroup) {
                    return { kind: 'tag', value: Tag.FieldGroup };
                }
                if (Array.isArray(elmValue)) {
                    return { kind: 'kernelArray' };
                }
                switch (elmValue.$) {
                    case undefined:
                        return { kind: 'tag', value: Tag.Record };
                    case '[]':
                        return { kind: 'constAddr', value: wasmConstAddrs.Nil };
                    case '::':
                        return { kind: 'tag', value: Tag.List };
                    case '#0':
                        return { kind: 'constAddr', value: wasmConstAddrs.Unit };
                    case '#2':
                        return { kind: 'tag', value: Tag.Tuple2 };
                    case '#3':
                        return { kind: 'tag', value: Tag.Tuple3 };
                    default:
                        return { kind: 'tag', value: Tag.Custom };
                }
            }
        }
        console.error(elmValue);
        throw new Error('Cannot determine type of Elm value');
    }
    function wasmBuilder(tag, value) {
        switch (tag) {
            case Tag.Int:
                return {
                    body: [value],
                    jsChildren: [],
                    bodyWriter: null
                };
            case Tag.Float:
                return {
                    body: [],
                    jsChildren: [],
                    bodyWriter: (bodyAddr) => {
                        write32(bodyAddr >> 2, 0);
                        const afterPadding = bodyAddr + WORD;
                        wasmExports._writeF64(afterPadding, value);
                        return 3; // words written
                    }
                };
            case Tag.Char:
            case Tag.String:
                return {
                    body: [],
                    jsChildren: [],
                    bodyWriter: (bodyAddr) => {
                        const s = value;
                        const offset16 = bodyAddr >> 1;
                        const lenAligned = s.length + (s.length % 2); // for odd length, write an extra word (gets coerced to 0)
                        if (offset16 + lenAligned > maxWriteIndex16) {
                            throw heapOverflowError;
                        }
                        for (let i = 0; i < lenAligned; i++) {
                            mem16[offset16 + i] = s.charCodeAt(i);
                        }
                        const wordsWritten = lenAligned >> 1;
                        return wordsWritten;
                    }
                };
            case Tag.Tuple2:
            case Tag.List:
                return {
                    body: [],
                    jsChildren: [value.a, value.b],
                    bodyWriter: null
                };
            case Tag.Tuple3:
                return {
                    body: [],
                    jsChildren: [value.a, value.b, value.c],
                    bodyWriter: null
                };
            case Tag.Custom: {
                const jsCtor = value.$;
                const keys = Object.keys(value).filter(k => k !== '$');
                if (typeof jsCtor === 'string') {
                    return {
                        body: [appTypes.ctors[jsCtor]],
                        jsChildren: keys.map(k => value[k]),
                        bodyWriter: null
                    };
                }
                else {
                    const jsChildren = [];
                    const fieldNames = 'abcdefghijklmnopqrstuvwxyz'.split('');
                    keys.forEach(k => {
                        const i = fieldNames.indexOf(k);
                        if (i === -1) {
                            throw new Error(`Unsupported Kernel Custom field '${k}'`);
                        }
                        jsChildren[i] = value[k];
                    });
                    return {
                        body: [KERNEL_CTOR_OFFSET + jsCtor],
                        jsChildren,
                        bodyWriter: null
                    };
                }
            }
            case Tag.Record: {
                const body = [];
                const jsChildren = [];
                const keys = Object.keys(value).sort();
                const fgName = keys.join(' ');
                const fgAddrStatic = appTypes.fieldGroups[fgName];
                if (fgAddrStatic) {
                    body.push(fgAddrStatic);
                }
                else {
                    jsChildren.push(new FieldGroup(keys));
                }
                keys.forEach(k => jsChildren.push(value[k]));
                return {
                    body,
                    jsChildren,
                    bodyWriter: null
                };
            }
            case Tag.FieldGroup: {
                const fieldNames = value.fieldNames;
                const body = [fieldNames.length];
                fieldNames.forEach(name => body.push(appTypes.fields[name]));
                return {
                    body,
                    jsChildren: [],
                    bodyWriter: null
                };
            }
            case Tag.Closure: {
                const fun = value.f || value;
                if (fun.evaluator) {
                    const { freeVars, max_values, evaluator } = fun;
                    const n_values = freeVars.length;
                    return {
                        body: [(max_values << 16) | n_values, evaluator],
                        jsChildren: freeVars,
                        bodyWriter: null
                    };
                }
                else {
                    let evaluator = kernelFunctions.findIndex(f => f === value);
                    if (evaluator === -1) {
                        kernelFunctions.push(value);
                        evaluator = kernelFunctions.length - 1;
                    }
                    return {
                        body: [NEVER_EVALUATE << 16, evaluator],
                        jsChildren: [],
                        bodyWriter: null
                    };
                }
            }
        }
        console.error(value);
        throw new Error(`Can't write to WebAssembly for tag "${Tag[tag]}"`);
    }
    function writeFromBuilder(nextIndex, builder, tag) {
        if (builder.bodyWriter) {
            /**
             * Special cases: Float (64-bit data) or String/Char (16-bit data)
             */
            const headerIndex = nextIndex;
            const addr = headerIndex << 2;
            const wordsWritten = builder.bodyWriter(addr + WORD);
            const size = 1 + wordsWritten;
            write32(headerIndex, encodeHeader(tag, size));
            return {
                addr,
                nextIndex: headerIndex + size
            };
        }
        else {
            /**
             * Normal cases (32-bit data)
             */
            const { body, jsChildren } = builder;
            const childAddrs = [];
            for (let i = 0; i < jsChildren.length; i++) {
                const child = jsChildren[i];
                const update = writeWasmValue(nextIndex, child); // recurse
                childAddrs.push(update.addr);
                nextIndex = update.nextIndex;
            }
            const addr = nextIndex << 2;
            const size = 1 + body.length + childAddrs.length;
            write32(nextIndex++, encodeHeader(tag, size));
            body.forEach(word => {
                write32(nextIndex++, word);
            });
            childAddrs.forEach(pointer => {
                write32(nextIndex++, pointer);
            });
            return { addr, nextIndex };
        }
    }
    function encodeHeader(tag, size) {
        return (tag << TAG_SHIFT) | (size << SIZE_SHIFT);
    }
    /* --------------------------------------------------
  
                      EXPORTS
  
    -------------------------------------------------- */
    const mains = [];
    while (true) {
        const mainAddr = wasmExports._getNextMain();
        if (!mainAddr)
            break;
        mains.push(readWasmValue(mainAddr));
    }
    return {
        mains,
        // functions for testing
        readWasmValue,
        writeWasmValue: (value) => handleWasmWrite(nextIndex => writeWasmValue(nextIndex, value))
    };
}
wasmWrapper = wrapEmscriptenForElm(
	scope['EmscriptenModule'].buffer,
	scope['EmscriptenModule'].asm,
	{
		ctors: ['Array_elm_builtin', 'EQ', 'Err', 'External', 'Failure', 'False', 'Field', 'GT', 'Http', 'Https', 'Index', 'Internal', 'Just', 'LT', 'Leaf', 'MayPreventDefault', 'MayStopPropagation', 'Normal', 'NotFound', 'Nothing', 'Ok', 'OneOf', 'Perform', 'RBEmpty_elm_builtin', 'SetCounter', 'SubTree', 'True'],
		fields: ['fragment', 'host', 'init', 'nodeList', 'nodeListSize', 'path', 'port_', 'protocol', 'query', 'subscriptions', 'tail', 'update', 'view'],
		fieldGroups: ['fragment host path port_ protocol query', 'init subscriptions update view', 'nodeList nodeListSize tail']
	},
	{_Browser_element: _Browser_element, _JsArray_empty: _JsArray_empty, _JsArray_foldr: _JsArray_foldr, _JsArray_initialize: _JsArray_initialize, _JsArray_initializeFromList: _JsArray_initializeFromList, _JsArray_length: _JsArray_length, _Json_encode: _Json_encode, _Json_map1: _Json_map1, _Json_map2: _Json_map2, _Json_run: _Json_run, _Json_runOnString: _Json_runOnString, _Json_succeed: _Json_succeed, _Platform_batch: _Platform_batch, _Platform_leaf: _Platform_leaf, _Process_sleep: _Process_sleep, _Scheduler_andThen: _Scheduler_andThen, _Scheduler_succeed: _Scheduler_succeed, _VirtualDom_node: _VirtualDom_node, _VirtualDom_on: _VirtualDom_on, _VirtualDom_text: _VirtualDom_text});
var $author$project$Main$main = wasmWrapper.mains[0];
_Platform_export({'Main':{'init':$author$project$Main$main(
	/* Call *//* VarGlobal */$elm$json$Json$Decode$succeed(/* Unit */_Utils_Tuple0))(0)}});

if (onReadyCallback) {
  onReadyCallback();
} else {
  throw new Error(`
    Elm.onReady has not been called.
    Elm Wasm apps are initialised differently. You have to initialize your app using a callback function.
    I'll call that function when the WebAssembly module is ready.
    It's compiled asynchronously in the browser, so we have to do it this way.
    Your code could look something like this, for example:
       Elm.onReady(() => {
          var app = Elm.Main.init({
             node: document.getElementById('elm'),
             flags: Date.now()
          });
          app.ports.cache.subscribe(function(data) {
            localStorage.setItem('cache', JSON.stringify(data));
          });
       });
  `);
}

}
}(this));