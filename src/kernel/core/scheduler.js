/*

// Task Constructors
_Scheduler_succeed  (Wasm & JS callback)
_Scheduler_fail     (Wasm & JS callback)
_Scheduler_binding  (Wasm & JS callback)
_Scheduler_andThen  (Wasm & JS callback)
_Scheduler_onError  (Wasm & JS callback)
_Scheduler_receive  (Wasm & JS callback)


_Scheduler_guid (private int)

// Process Constructors
_Scheduler_rawSpawn   construct and enqueue
_Scheduler_spawn      create binding to construct, enqueue and callback
_Scheduler_rawSend    push a message into the process' mailbox and enqueue it
_Scheduler_send       create binding to send and register callback
_Scheduler_kill       create binding to kill process and then callback (unused)


// private
_Scheduler_working   boolean lock (I think for recursive calls)
_Scheduler_queue     GC root (FIFO list, just remember the current tail and add to it)
_Scheduler_enqueue   Push to queue, do locking, and exhaust the queue
_Scheduler_step      State machine


*/