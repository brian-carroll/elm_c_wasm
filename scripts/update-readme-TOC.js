#!/usr/bin/env node
const fs = require('fs');
const path = require('path');

const readmePath = path.resolve(__dirname, '..', 'README.md');
const readmeText = fs.readFileSync(readmePath).toString();
const headings = readmeText.match(/\n#(.*)/g).map(s => s.slice(1));

const htmlEntities = /&\w+;/g;
const whitespace = /\s/g;
const invalidIdChars = /[^a-z0-9-]/g;

function titleToId(title) {
  return title
    .toLowerCase()
    .replace(htmlEntities, '')
    .replace(whitespace, '-')
    .replace(invalidIdChars, '');
}

const newTOC = headings
  .map(h => {
    const numHashes = h.indexOf(' ');
    const title = h.replace(/#+ /, '');
    const htmlId = titleToId(title);
    const href = '#' + htmlId;
    const indent = '  '.repeat(numHashes - 1);
    const markdown = `${indent}- [${title}](${href})`;
    return markdown;
  })
  .join('\n');

const firstHeadingPos = readmeText.indexOf(headings[0]);
const oldTOC = readmeText.slice(0, firstHeadingPos).trimRight();

const newReadmeText = readmeText.replace(oldTOC, newTOC);

fs.writeFileSync(readmePath, newReadmeText);
