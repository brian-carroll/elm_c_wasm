## Finished

Ran out of memory after returning from this call.
Expect intermediate values to be GC'd but return value to be kept.

| eval ops | before GC   | after GC             |
| -------- | ----------- | -------------------- |
|          | empty       | empty                |
|          | push        | push                 |
| alloc 1  | alloc 1     | alloc 2              |
| alloc 2  | alloc 2     | pop alloc 3          |
| alloc 3  | alloc 3     | check replay == NULL |
| pop      | pop alloc 3 | check return alloc 2 |
|          | GC          | check stack depth 0  |
|          | replay      | check stackmap pop   |

## Unfinished_Sat_Normal

Ran out of memory during a call that applied all arguments at once.
Function is "normal", not tail-call-eliminated.
Expect everything to be kept alive and replayed.

| eval ops  | before GC | after GC             |
| --------- | --------- | -------------------- |
|           | empty     | empty                |
|           | push      | push                 |
| alloc 1   | alloc 1   | alloc 1              |
| alloc 2   | alloc 2   | alloc 2              |
| exception |           | check replay == NULL |
|           | GC        | check stack depth 1  |
|           | replay    | check stackmap push  |

## Unfinished_Curried_Normal

Ran out of memory during a call that applied the final argument to a curried closure
Function is "normal", not tail-call-eliminated.
Expect everything to be kept alive and replayed.

| eval ops  | before GC            | after GC             |
| --------- | -------------------- | -------------------- |
|           | empty                | empty                |
|           | closure (arg1)       | closure (arg1)       |
|           | closure (arg1, arg2) | closure (arg1, arg2) |
|           | push                 | push                 |
| alloc 1   | alloc 1              | alloc 1              |
| alloc 2   | alloc 2              | alloc 2              |
| exception |                      | check replay == NULL |
|           | GC                   | check stack depth 1  |
|           | replay               | check stackmap push  |

## Unfinished_Sat_Tail 1st iteration

Ran out of memory while executing a tail-call-eliminated function,
before the first tail call actually happens.
Expect everything to be kept alive and replayed.

| eval ops  | before GC    | after GC             |
| --------- | ------------ | -------------------- |
|           | empty        | empty                |
|           | push         | push                 |
|           | closure full | closure full         |
|           | tailcall     | tailcall             |
| alloc 1   | alloc 1      | alloc 1              |
| alloc 2   | alloc 2      | alloc 2              |
| exception |              | check replay == NULL |
|           | GC           | check stack depth 1  |
|           | replay       | check stackmap push  |

## Unfinished_Sat_Tail 2nd iteration

Ran out of memory while executing a tail-call-eliminated function,
after a tail call.
Expect first iteration values to be GC'd
Expect second iteration to be kept alive and replayed.

| eval ops  | before GC     | after GC                |
| --------- | ------------- | ----------------------- |
|           | empty         | empty                   |
|           | push          | push                    |
|           | closure iter1 | closure iter2           |
|           | tailcall      | tailcall                |
| alloc 0   | alloc 0       | alloc 4                 |
| alloc 1   | alloc 1       | alloc 5                 |
| alloc 2   | alloc 2       | check replay == NULL    |
|           | closure iter2 | check stack depth 1     |
| tailcall  | tailcall      | check stackmap tailcall |
| alloc 4   | alloc 4       |
| alloc 5   | alloc 5       |
| exception |
|           | GC            |
|           | replay        |

## Unfinished_Curried_Tail 1st iteration

Ran out of memory while executing a tail-call-eliminated function,
before the first tail call actually happens.
And the call was applying the final argument to a curried closure
Expect everything to be kept alive and replayed.

| eval ops  | before GC      | after GC             |
| --------- | -------------- | -------------------- |
|           | empty          | empty                |
|           | closure (arg1) | closure (arg1)       |
|           | closure full   | closure full         |
|           | push           | push                 |
| alloc 1   | alloc 1        | alloc 1              |
| alloc 2   | alloc 2        | alloc 2              |
| exception |                | check replay == NULL |
|           | GC             | check stack depth 1  |
|           | replay         | check stackmap push  |

## Unfinished_Curried_Tail 2nd iteration

Ran out of memory while executing a tail-call-eliminated function,
after a tail call.
And the call was applying the final argument to a curried closure
Expect first iteration values to be GC'd
Expect second iteration to be kept alive and replayed.

| eval ops  | before GC        | after GC                |
| --------- | ---------------- | ----------------------- |
|           | empty            | empty                   |
|           | closure (arg1)   | closure (arg1)          |
|           | closure full     | closure (alloc3)        |
|           | push             | push                    |
| alloc 1   | alloc 1          | tailcall                |
| alloc 2   | alloc 2          | alloc 4                 |
| alloc 3   | alloc 3          | alloc 5                 |
|           | closure (alloc3) | check replay == NULL    |
| tailcall  | tailcall         | check stack depth 1     |
| alloc 4   | alloc 4          | check stackmap tailcall |
| alloc 5   | alloc 5          |
| exception |
|           | GC               |
|           | replay           |
