const sampleText = document.body.innerHTML;
const nIterations = 1200;
const domElement = document.getElementById('target');

const utf8Encoder = new TextEncoder(); // only utf-8 is supported
const buffers = {
  utf8: utf8Encoder.encode(sampleText),
  utf16: new Uint16Array(sampleText.length)
};
buffers.utf16.forEach((_, i) => {
  buffers.utf16[i] = sampleText.charCodeAt(i);
});

const decoders = {
  utf8: new TextDecoder('utf-8'),
  utf16: new TextDecoder('utf-16le')
};

function testOne(buffer, decoder) {
  domElement.innerText = '';
  const tStart = performance.now();
  const domString = decoder.decode(buffer);
  const tDecode = performance.now();
  domElement.innerText = domString;
  const tEnd = performance.now();
  return {
    tDecode: tDecode - tStart,
    tDOM: tEnd - tDecode,
    tTotal: tEnd - tStart
  };
}

const results = {
  iterations: 0,
  utf8: { tDecode: 0, tDOM: 0, tTotal: 0, iters: 0 },
  utf16: { tDecode: 0, tDOM: 0, tTotal: 0, iters: 0 }
};

function onRAF() {
  const encoding = results.iterations % 2 ? 'utf8' : 'utf16';
  const iterResults = testOne(buffers[encoding], decoders[encoding]);

  Object.keys(iterResults).forEach(k => {
    results[encoding][k] += iterResults[k];
  });

  results[encoding].iters++;
  results.iterations++;

  if (results.iterations < nIterations) {
    window.requestAnimationFrame(onRAF);
  } else {
    printResults();
  }
  if (results.iterations % 120 === 0) {
    console.log(results.iterations);
  }
}

function printResults() {
  ['utf8', 'utf16'].forEach(enc => {
    Object.keys(results[enc]).forEach(key => {
      if (key !== 'iters') {
        results[enc][key] /= results[enc].iters / 1000;
      }
    });
  });

  console.log(JSON.stringify(results, null, 2));
}

window.requestAnimationFrame(onRAF);
