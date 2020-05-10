Module.print = (function () {
  var printBuffer = '';
  var printTimeoutId;
  return function (text) {
    if (arguments.length > 1) {
      text = Array.prototype.slice.call(arguments).join(' ');
    }
    printBuffer += text + '\n';
    clearTimeout(printTimeoutId);
    printTimeoutId = setTimeout(() => {
      console.log(printBuffer);
      printBuffer = '';
    }, 300);
  };
})();
