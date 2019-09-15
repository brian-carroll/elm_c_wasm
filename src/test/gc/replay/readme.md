## Finished

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
