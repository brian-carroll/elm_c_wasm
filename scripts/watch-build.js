const fs = require('fs');
const path = require('path');
const child_process = require('child_process');

const DEBOUNCE_TIME_MS = 500;

const command = process.argv.slice(2).join(' ');
const options = {
  cwd: path.resolve(__dirname, '..')
};

const run = () => {
  child_process.exec(command, options, (err, stdout, stderr) => {
    if (stderr) {
      console.warn(stderr);
    }
    if (err) {
      console.error(err);
    } else {
      console.log(stdout);
    }
  });
};

let timer;
const files = new Set();
fs.watch('src', { recursive: true }, (_event, filepath) => {
  if (!filepath.includes('.')) {
    return;
  }
  if (timer) {
    clearTimeout(timer);
  }
  files.add(file);
  timer = setTimeout(() => {
    console.log('Changes in ' + [...files]);
    files.clear();
    run();
  }, DEBOUNCE_TIME_MS);
});
