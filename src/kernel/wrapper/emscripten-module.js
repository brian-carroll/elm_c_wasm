// Customise the Emscripten implementation of C printf
// For a rapid burst of printf calls, buffer them and call console.log just once at the end.
// Otherwise it's impractically slow to print the debug representation of the Wasm heap.
// The debug string is about 20-25x the Wasm heap size. Typically a few MB.



Module.print = (function () {
  let printBuffer = '';
  let printTimeoutId;
  const print = text => {
    printBuffer += text + '\n';
    clearTimeout(printTimeoutId);
    printTimeoutId = setTimeout(() => {
      console.log(printBuffer);
      printBuffer = '';
    }, 100);
  };
  return print;
})();
