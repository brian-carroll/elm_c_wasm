module Wasm exposing (tests)

import Test exposing (..)
import Expect


tests : Test
tests =
    describe "Wasm code gen" <|
        [ functionTests
        -- , functionCallTests
        -- , patternMatchingTests
        ]

{-
   TODO
   pattern match tests


   tuple destructuring
   scope tests with `let`, local function vars, nesting...
   TCE free vars, scopes
   functin application tests
    - partial application
    - too many args
    - apply args to returned function

-}


patternMatchingTests : Test
patternMatchingTests =
    describe "Pattern Matching" []


functionCallTests : Test
functionCallTests =
    describe "Function Calls" []


globalNonTailSelfCall : Int -> Int
globalNonTailSelfCall i =
    if i <= 1 then
        1
    else
        globalNonTailSelfCall (i-1) + globalNonTailSelfCall (i-2)


testGlobalNonTailSelfCall : Test
testGlobalNonTailSelfCall =
    test "global non-tail self-call should work" <|
        \() -> Expect.equal (globalNonTailSelfCall 6) 13


testLocalNonTailSelfCall : Test
testLocalNonTailSelfCall =
    test "local non-tail self-call should work" <|
        \() ->
            let
                local i =
                    if i <= 1 then
                        1
                    else
                        local (i-1) + local (i-2)
            in
            Expect.equal (local 6) 13


functionTests : Test
functionTests =
    describe "Functions"
        [ testGlobalNonTailSelfCall
        , testLocalNonTailSelfCall
        ]

{-
- global

- local
- local (with free vars)
- local (with nested scope free vars)

- local anon
- local anon (with free vars)

- global tailcall
- local tailcall
- local tailcall (with free vars)

- global non-tail self-call
- local non-tail self-call
- local non-tail self-call (with free vars)

- global tail & non-tail self-call
- local tail & non-tail self-call
- local tail & non-tail self-call (with free vars)

- global cycles
-}