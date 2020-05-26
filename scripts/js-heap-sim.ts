/**
 * Simulation of JS and Wasm heaps
 *
 * Wasm program whirrs along, filling its heap
 * On every timestep it may
 *   - conjure up a JS value
 *   - copy an existing JS Ref
 *   - generate some other Wasm value
 *
 * When the Wasm heap is full
 * Mark a random selection of Wasm values
 * If it's a JS ref then tell JS it's alive
 */

interface WasmJsRef {
  tag: 'JsRef';
  id: number;
  marked?: boolean;
}
interface WasmValue {
  tag: 'WasmValue';
  marked?: boolean;
}
type WasmHeapValue = WasmJsRef | WasmValue;

interface JsHeapItem {
  value: number | null;
  marked: boolean;
}

const WASM_HEAP_VALUES = 100;
const wasmHeap: WasmHeapValue[] = [];
const jsHeap: JsHeapItem[] = [];

for (let addr = 0; addr < WASM_HEAP_VALUES; addr++) {
  const rand = Math.random();
  if (rand < 0.1) {
    // create a JS Ref
    const id = getValueFromJs();
    wasmHeap.push({ tag: 'JsRef', id });
  } else if (rand < 0.3) {
    // copy a JS Ref if any exist, otherwise create one
    const jsRefs = wasmHeap.filter(val => val.tag === 'JsRef');
    const picked = jsRefs[
      Math.floor(jsRefs.length * Math.random())
    ] as WasmJsRef;
    const id = picked ? picked.id : getValueFromJs();
    wasmHeap.push({ tag: 'JsRef', id });
  } else {
    wasmHeap.push({ tag: 'WasmValue' });
  }
}

function markWasm() {
  wasmHeap.forEach(val => {
    if (Math.random() < 0.4) {
      val.marked = true;
      if (val.tag === 'JsRef') {
        markJs(val.id);
      }
    }
  });
}

function getValueFromJs(): number {
  const value = Math.random();
  return jsHeap.push({ marked: false, value }) - 1;
}
function markJs(id: number) {
  jsHeap[id].marked = true;
}
function sweepJs() {
  jsHeap.forEach((val, index) => {
    if (!val.marked) {
      jsHeap[index].value = null;
    }
  });
}

markWasm();
sweepJs();

console.log({ wasmHeap, jsHeap });
