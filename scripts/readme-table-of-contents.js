#!/usr/bin/env node
const fs = require('fs');
const path = require('path');

const readme = fs.readFileSync(path.resolve('..', 'README.md')).toString();
const headings = readme.match(/\n#(.*)/g).map(s => s.slice(1));
const toc = headings
  .map(h => {
    const numHashes = h.indexOf(' ');
    const text = h.replace(/#+ /, '');
    const fragment = text.toLowerCase().replace(/ /g, '-');
    const href = '#' + fragment;
    const indent = '  '.repeat(numHashes - 1);
    const markdown = `${indent}- [${text}](${href})`;
    return markdown;
  })
  .join('\n');
console.log(toc);
