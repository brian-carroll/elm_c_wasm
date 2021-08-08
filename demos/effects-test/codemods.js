const fs = require('fs');

const filepath = process.argv[2];
const find = '#include "kernel.h"';
const replace = [
  '#include "../../../../src/kernel/kernel.h"',
  '#include "../../src/js.c"'
].join('\n');

const orig = fs.readFileSync(filepath).toString();
const withKernelPath = orig.replace(find, replace);

const withPlatformInitialize = withKernelPath.replace(
  'GC_collect_major();',
  `GC_collect_major();
mock_js_Platform_initialize();
`
);

const updated = withPlatformInitialize;
console.log(updated);
