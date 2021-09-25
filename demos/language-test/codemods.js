const fs = require("fs");

const filepath = process.argv[2];
const find = '"kernel.h"';
const replace =
  '"../../../../src/kernel/kernel.h"\n#include "../../src/print-virtual-dom.c"';

const orig = fs.readFileSync(filepath).toString();
const withIncludePaths = orig.replace(find, replace);

const withPrintVdom = withIncludePaths.replace(
  "GC_collect_major();",
  `GC_collect_major();
    //perf_print();
    print_virtual_dom(gptr_author_project_Main_main);`
);

const withNativeInitGlobals = withPrintVdom.replace(
  "    return GC_init();",
  `
    int exit_code = GC_init();
    if (exit_code) return exit_code;
    #ifndef EMSCRIPTEN
        init_globals();
    #endif
    return 0;
`
);

const updated = withNativeInitGlobals;
console.log(updated);
