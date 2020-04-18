const fs = require('fs');

const filepath = process.argv[2];
const find = '"kernel.h"';
const replace = '"../../../../src/kernel/kernel.h"';

const orig = fs.readFileSync(filepath).toString();
const withKernelPath = orig.replace(find, replace);

const withoutFuncAddrs = withKernelPath;
const mainReturnIndex = withoutFuncAddrs.lastIndexOf('    return 0;');
const before = withoutFuncAddrs.slice(0, mainReturnIndex);
const after = withoutFuncAddrs.slice(mainReturnIndex);
const evalFuncDecls = before.match(/void\s*\*\s*eval_\w+\(/g);
const evalFuncNames = evalFuncDecls.map(s =>
  s.replace('void * ', '').replace('(', '')
);
const incudeStandardIO = '#include <stdio.h>\n';
const printFuncAddrs = evalFuncNames.map(
 fname => `    printf("%zd ${fname}\\n", (size_t)&${fname});\n`
).join('');
const withFuncAddrs = incudeStandardIO + before + printFuncAddrs + after;

const updated = withFuncAddrs
console.log(updated);
