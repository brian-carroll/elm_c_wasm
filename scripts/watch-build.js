const fs = require('fs');
const path = require('path');
const child_process = require('child_process');

const DEBOUNCE_TIME_MS = 500;

const command = process.argv.slice(2).join(' ');
const options = {
  cwd: path.resolve(__dirname, '..')
};

let running = false;
const run = () => {
  running = true;
  child_process.exec(command, options, (err, stdout, stderr) => {
    running = false;
    console.warn(stderr);
    console.log(stdout);
    if (!err) {
      console.log('OK');
    } else {
      console.error('ERROR')
    }
  });
};

let timer;
const files = new Set();
fs.watch('src', { recursive: true }, (_event, filepath) => {
  // Note: Sometimes Windows reports 'change' events when I open a file
  if (!filepath.includes('.')) {
    return;
  }
  if (timer) {
    clearTimeout(timer);
  }
  files.add(filepath);
  const callback = () => {
    if (running) {
      timer = setTimeout(callback, DEBOUNCE_TIME_MS);
    } else {
      console.log('Changes in ' + [...files]);
      files.clear();
      run();
    }
  };
  timer = setTimeout(callback, DEBOUNCE_TIME_MS);
});

run();
