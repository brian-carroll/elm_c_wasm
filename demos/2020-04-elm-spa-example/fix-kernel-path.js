const fs = require('fs');

const filepath = process.argv[2];
const find = '"kernel.h"';
const replace = '"../../../../src/kernel/kernel.h"';

const orig = fs.readFileSync(filepath).toString();
const updated = orig.replace(find, replace);
console.log(updated);
