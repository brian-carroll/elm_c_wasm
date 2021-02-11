# Issues found while setting up core tests

## Tests commented out

- Loads of stuff just not implemented yet. Expected.
- Basics checks for 32 bit _unsigned_ max value but we have 32 bit signed

## Bad code gen

- [x] `let` expressions within parens need their own C scope or at least namespace (`Maybe` tests)

- [x] Tail recursive local functions not generated correctly. First arg is the function itself as a free var, but then we mis-count args in the tail call. Probably just need to skip free vars in tail call.

  - this is blocking the test lib itself

- [x] Pattern matching needs more careful guards

  - array bounds checks for indexing into children of a structure

- [x] Point-free global function definition that is part of a cycle

  - No explicit args, so we decide it has none at all?
  - what would we even want here?
  - doesn't need a function body, the body of foldl will do it

- [ ] GC throws during replay. Bumped up the initial heap size for now to avoid it.

```elm
countAllRunnables =
    List.foldl (countRunnables >> (+)) 0


countRunnables : RunnableTree -> Int
countRunnables runnable =
    case runnable of
        Runnable _ ->
            1

        Labeled _ runner ->
            countRunnables runner

        Batch runners ->
            countAllRunnables runners
```

```c
void* ginit_elm_explorations_test_Test_Runner_countAllRunnables();
//...
  tmp0 = A1(ginit_elm_explorations_test_Test_Runner_countAllRunnables(), x_runners);
```

| To test this               | I need to call this           |            |
| -------------------------- | ----------------------------- | ---------- |
| JsArray.empty              | Array.empty                   | empty      |
| JsArray.singleton          | Array.push                    | push       |
| JsArray.length             | Array.push,append,slice       | push       |
| JsArray.initialize         | Array.initialize              | initialize |
| JsArray.initializeFromList | Array.fromList,slice          | fromList   |
| JsArray.unsafeGet          | Array.get                     | get        |
| JsArray.unsafeSet          | Array.set                     | set        |
| JsArray.push               | Array.push                    | push       |
| JsArray.foldl              | Array.foldl,indexedMap,append | append     |
| JsArray.foldr              | Array.foldr,toList            | toList     |
| JsArray.map                | Array.map                     | map        |
| JsArray.indexedMap         | Array.indexedMap              | indexedMap |
| JsArray.slice              | Array.append,slice            | append     |
| JsArray.appendN            | Array.append                  | append     |

Unique Array tests to test JsArray
- [x] empty
- [ ] push
- [x] initialize
- [ ] fromList
- [ ] get
- [ ] set
- [x] toList
- [ ] map
- [ ] indexedMap
- [ ] append
