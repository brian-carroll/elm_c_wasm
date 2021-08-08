const fs = require('fs');
const child_process = require('child_process');

const outFile = './src/kernel/core/debug/core-evaluators.c';

const execOptions = undefined;
child_process.exec(
  'find src/kernel -name "*.c"',
  execOptions,
  (err, stdout, stderr) => {
    if (err) {
      throw err;
    }
    const filenames = stdout.split(/[\r\n]+/);
    const c_code = generateCode(filenames);
    fs.writeFileSync(outFile, c_code);
  }
);

const rEval = /^void\* (\w+)\(void\* args\[/;

function generateCode(filenames) {
  const evaluators = [];

  filenames.forEach(filename => {
    if (!filename) return;
    const text = fs.readFileSync(filename).toString();
    const lines = text.split(/[\r\n]+/);
    lines.forEach(line => {
      const m = line.match(rEval);
      if (m) {
        evaluators.push(m[1]);
      }
    });
  });

  const c_code_lines = [];
  evaluators.forEach(evaluator => {
    c_code_lines.push(`void* ${evaluator}(void* args[]);`);
  });
  c_code_lines.push('');
  c_code_lines.push('char* Debug_evaluator_name_core(void* p) {');
  evaluators.forEach(evaluator => {
    const name = evaluator.replace(/(^eval_|_eval$)/, '');
    c_code_lines.push(
      `  if (p == ${evaluator}) {`,
      `    return "${name}";`,
      `  }`
    );
  });
  c_code_lines.push('  return "(?)";');
  c_code_lines.push('}');

  return c_code_lines.join('\r\n');
}
