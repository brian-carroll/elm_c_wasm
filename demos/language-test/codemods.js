const fs = require('fs');

const filepath = process.argv[2];
const find = '"kernel.h"';
const replace = '"../../../../src/kernel/kernel.h"\n#include "../../src/print-virtual-dom.c"';

const orig = fs.readFileSync(filepath).toString();
const withKernelPath = orig.replace(find, replace);

const withPrintVdom = withKernelPath.replace(
  'GC_collect_major();',
  'GC_collect_major();\n    print_virtual_dom(gptr_author_project_Main_main);',
);

const updated = withPrintVdom;
console.log(updated);
