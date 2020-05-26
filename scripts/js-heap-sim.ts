/**
 * Simulation of JS and Wasm heaps
 * Maintain a JS store of values that Wasm has references to
 * Garbage-collect it along with Wasm
 */

// --------------------------------------------------
// Simulated Wasm module
// --------------------------------------------------

interface WasmJsRef {
  tag: 'JsRef';
  id: number;
  mark: boolean;
}
interface WasmValue {
  tag: 'WasmValue';
  mark: boolean;
}
type WasmHeapValue = WasmJsRef | WasmValue;

const WASM_HEAP_SIZE = 100;
let wasmHeap: WasmHeapValue[] = [];

function runWasmApp() {
  for (let addr = wasmHeap.length; addr < WASM_HEAP_SIZE; addr++) {
    const rand = Math.random();
    if (rand < 0.1) {
      // create a JS Ref
      const id = getValueFromJs();
      wasmHeap.push({ mark: false, tag: 'JsRef', id });
    } else if (rand < 0.3) {
      // copy a JS Ref if any exist, otherwise create one
      const jsRefs = wasmHeap.filter(val => val.tag === 'JsRef');
      const picked = jsRefs[
        Math.floor(jsRefs.length * Math.random())
      ] as WasmJsRef;
      const id = picked ? picked.id : getValueFromJs();
      wasmHeap.push({ mark: false, tag: 'JsRef', id });
    } else {
      wasmHeap.push({ mark: false, tag: 'WasmValue' });
    }
  }
}
function markWasm() {
  wasmHeap.forEach(val => {
    if (Math.random() < 0.4) {
      val.mark = true;
      if (val.tag === 'JsRef') {
        markJs(val.id);
      }
    } else {
      val.mark = false;
    }
  });
}
function compactWasm() {
  wasmHeap = wasmHeap.filter(val => val.mark);
}

// --------------------------------------------------
// Actual JS heap implementation
// --------------------------------------------------

interface JsHeapEntry {
  id: number;
  value: number | {};
  mark: boolean;
}
const unusedJsHeapSlot = {};
const jsHeap: JsHeapEntry[] = [];

function getValueFromJs(): number {
  const value = Math.random();
  let id = jsHeap.findIndex(entry => entry.value === unusedJsHeapSlot);
  if (id === -1) {
    id = jsHeap.length;
    jsHeap.push({ id, mark: false, value });
  } else {
    jsHeap[id].value = value;
  }
  return id;
}
function markJs(id: number) {
  jsHeap[id].mark = true;
}
function sweepJs() {
  let lastMarked = 0;
  jsHeap.forEach((val, index) => {
    if (!val.mark) {
      jsHeap[index].value = unusedJsHeapSlot;
    } else {
      lastMarked = index;
    }
    val.mark = false;
  });
  jsHeap.splice(lastMarked + 1, jsHeap.length);
}

// --------------------------------------------------
// Run the simulation
// --------------------------------------------------

for (let cycle = 0; cycle < 10; cycle++) {
  runWasmApp();
  console.log('run', { /*wasmHeap,*/ jsHeap });
  markWasm();
  sweepJs();
  compactWasm();
  console.log('GC', { /*wasmHeap,*/ jsHeap });
}
