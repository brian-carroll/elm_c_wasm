(function () {
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
  let oldGenTop = 0;

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
  function markWasm(ignoreBelow: number) {
    for (let addr = ignoreBelow; addr < wasmHeap.length; addr++) {
      const val = wasmHeap[addr];
      if (Math.random() < 0.4) {
        val.mark = true;
        if (val.tag === 'JsRef') {
          markJs(val.id);
        }
      } else {
        val.mark = false;
      }
    }
    // console.log(`markWasm from ${ignoreBelow} to ${wasmHeap.length}`, wasmHeap)
  }
  function compactWasm(ignoreBelow: number) {
    const oldGen = wasmHeap.slice(0, ignoreBelow);
    const newGen = wasmHeap.slice(ignoreBelow);
    wasmHeap = oldGen.concat(newGen.filter(val => val.mark));
    wasmHeap.forEach(val => val.mark = false)
  }
  function gcMajor() {
    markWasm(0);
    compactWasm(0);
    sweepJs(true);
    oldGenTop = wasmHeap.length;
  }
  function gcMinor() {
    markWasm(oldGenTop);
    compactWasm(oldGenTop);
    sweepJs(false);
  }

  function getValueFromJs(): number {
    const value = Math.random();
    return storeJsRef(value);
  }

  // --------------------------------------------------
  // Actual JS heap implementation
  // --------------------------------------------------

  interface JsHeapEntry {
    id: number;
    value: any;
    mark: boolean;
    isOldGen: boolean;
  }
  const unusedJsHeapSlot = {};
  const jsHeap: JsHeapEntry[] = [];

  function storeJsRef(value: any): number {
    let id = jsHeap.findIndex(entry => entry.value === unusedJsHeapSlot);
    if (id === -1) {
      id = jsHeap.length;
      jsHeap.push({ id, mark: false, isOldGen: false, value });
    } else {
      jsHeap[id].value = value;
    }
    return id;
  }
  function markJs(id: number) {
    jsHeap[id].mark = true;
  }
  function sweepJs(isFullGc: boolean) {
    let lastMarked = 0;
    jsHeap.forEach((slot, index) => {
      const keepAlive = slot.mark || (!isFullGc && slot.isOldGen);
      if (!keepAlive) {
        slot.value = unusedJsHeapSlot;
        slot.isOldGen = false;
      } else {
        lastMarked = index;
        if (isFullGc) {
          slot.isOldGen = true;
        }
      }
      slot.mark = false;
    });
    jsHeap.splice(lastMarked + 1, jsHeap.length);
  }


  // --------------------------------------------------
  // Run the simulation
  // --------------------------------------------------

  for (let cycle = 0; cycle < 10; cycle++) {
    runWasmApp();
    console.log('run', { oldGenTop, heapSize: wasmHeap.length /*, jsHeap */ });
    gcMajor();
    console.log('GC Major', { oldGenTop, heapSize: wasmHeap.length, jsHeap /*, wasmHeap */ });
    runWasmApp();
    console.log('run', { oldGenTop, heapSize: wasmHeap.length /*, jsHeap */ });
    gcMinor();
    console.log('GC Minor', { oldGenTop, heapSize: wasmHeap.length, jsHeap /*, wasmHeap */ });
    runWasmApp();
    console.log('run', { oldGenTop, heapSize: wasmHeap.length /*, jsHeap */ });
    gcMinor();
    console.log('GC Minor', { oldGenTop, heapSize: wasmHeap.length, jsHeap /*, wasmHeap */ });
  }
})();
