function debug_step(proc) {
  const id = 'e';
  const root_ = 'f';
  const stack = 'g';
  const mailbox = 'h';

  const root = proc[root_];
  const procRoot = new Task(root);
  console.log(`Stepping Process ${proc[id]}: root=\n`, procRoot, procRoot.toString());
}

class Task {
  constructor(t) {
    const value = 'a';
    const callback = 'b';
    const kill = 'c';
    const task = 'd';

    const ctors = [
      'SUCCEED',
      'FAIL',
      'BINDING',
      'AND_THEN',
      'ON_ERROR',
      'RECEIVE'
    ];

    this.$ = ctors[t.$];
    this.value = t[value];
    this.callback = t[callback];
    this.kill = t[kill];
    this.task = t[task] && new Task(t[task]);
  }

  toString(i = 0) {
    const indent = ' '.repeat(i);
    return `
${indent}{
${indent}  $: ${this.$}
${indent}  value: ${this.value}
${indent}  callback: ${this.callback}
${indent}  kill: ${this.kill}
${indent}  task: ${this.task && this.task.toString(i + 4)}
${indent}}`;
  }
}

function _Scheduler_step(proc)
{
	while (proc.f)
	{
    debug_step(proc);

		var rootTag = proc.f.$;
		if (rootTag === 0 || rootTag === 1)
		{
			while (proc.g && proc.g.$ !== rootTag)
			{
				proc.g = proc.g.i;
			}
			if (!proc.g)
			{
				return;
			}
			proc.f = proc.g.b(proc.f.a);
			proc.g = proc.g.i;
		}
		else if (rootTag === 2)
		{
			proc.f.c = proc.f.b(function(newRoot) {
				proc.f = newRoot;
				_Scheduler_enqueue(proc);
			});
			return;
		}
		else if (rootTag === 5)
		{
			if (proc.h.length === 0)
			{
				return;
			}
      var received_msg = proc.h.shift();
      console.log({received_msg})
			proc.f = proc.f.b(received_msg);
		}
		else // if (rootTag === 3 || rootTag === 4)
		{
			proc.g = {
				$: rootTag === 3 ? 0 : 1,
				b: proc.f.b,
				i: proc.g
			};
			proc.f = proc.f.d;
		}
	}
}
