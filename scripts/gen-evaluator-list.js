const fs = require('fs');

const headerFiles = ['./src/kernel/core/core.h', './src/kernel/json/json.h'];
const outFile = './src/kernel/core/debug/core-evaluators.c';

const extractClosures = lines => {
  const rClosure = /^extern Closure (\w+);$/;
  return lines
    .map(line => {
      const m = line.match(rClosure);
      return m && m[1];
    })
    .filter(x => x)
    .flatMap(closure => [
      `  if (p == ${closure}.evaluator) {`,
      `    return "${closure}";`,
      `  }`
    ]);
};

const extractEvaluators = lines => {
  const rEval = /^void\* (eval_\w+)/;
  return lines
    .map(line => {
      const m = line.match(rEval);
      return m && m[1];
    })
    .filter(x => x)
    .flatMap(evaluator => [
      `  if (p == ${evaluator}) {`,
      `    return "${evaluator}";`,
      `  }`
    ]);
};

const extractFile = filename => {
  const text = fs.readFileSync(filename).toString();
  const lines = text.split(/[\r\n]+/);
  return [extractClosures(lines), extractEvaluators(lines)];
};

const outputCode = [
  'char* Debug_evaluator_name_core(void* p) {',
  headerFiles.map(extractFile),
  '  return "(?)";',
  '}'
]
  .flat(3)
  .join('\r\n');

fs.writeFileSync(outFile, outputCode);
