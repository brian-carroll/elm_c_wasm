function debug_step(proc) {
  const TASK_SUCCEED = 0;
  const TASK_FAIL = 1;
  const TASK_BINDING = 2;
  const TASK_AND_THEN = 3;
  const TASK_ON_ERROR = 4;
  const TASK_RECEIVE = 5;

  const value = 'a';
  const callback = 'b'
  const kill = 'c'
  const task = 'd'
  const id = 'e';
  const root_ = 'f';
  const stack = 'g';
  const mailbox = 'h';


  const root = proc[root_];
  switch (root.$) {
    case TASK_SUCCEED:
      console.log(`Stepping Process ${proc[id]}: TASK_SUCCEED\t`, root[value]);
      break;
    case TASK_FAIL:
      console.log(`Stepping Process ${proc[id]}: TASK_FAIL\t`, root[value]);
      break;
    case TASK_BINDING:
      console.log(`Stepping Process ${proc[id]}: TASK_BINDING\t%s`, root[callback]);
      break;
    case TASK_AND_THEN:
      console.log(`Stepping Process ${proc[id]}: TASK_AND_THEN\t%s`, root[callback]);
      break;
    case TASK_ON_ERROR:
      console.log(`Stepping Process ${proc[id]}: TASK_ON_ERROR\t%s`, root[callback]);
      break;
    case TASK_RECEIVE:
      console.log(`Stepping Process ${proc[id]}: TASK_RECEIVE\t%s`, root[callback]);
      break;
    default:
      throw new Error(`Unknown Task ctor ` + proc[root]);
  }
  console.log(root);
}
