#!/usr/bin/env node
const fs = require('fs');
const path = require('path');

const readmePath = path.resolve(__dirname, '..', 'README.md');
const readmeText = fs.readFileSync(readmePath).toString();
const headings = readmeText.match(/\n#(.*)/g).map(s => s.slice(1));

const newTOC = headings
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

const firstHeadingPos = readmeText.indexOf(headings[0]);
const oldTOC = readmeText.slice(0, firstHeadingPos).trimRight();

const newReadmeText = readmeText.replace(oldTOC, newTOC);

fs.writeFileSync(readmePath, newReadmeText);
