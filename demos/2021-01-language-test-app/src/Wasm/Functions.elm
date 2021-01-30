module Wasm.Functions exposing (tests)

import Test exposing (..)
import Expect exposing (Expectation)


tests : Test
tests =
    describe "Functions"
        [ testCaptureAcrossTwoScopes
        , testGlobalRecursion
        , testLocalRecursion
        , testGlobalTailRecursion
        , testLocalTailRecursion
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


testCaptureAcrossTwoScopes : Test
testCaptureAcrossTwoScopes =
    test "should capture a value across two scopes" captureAcrossTwoScopes


captureAcrossTwoScopes : () -> Expectation
captureAcrossTwoScopes () =
    let
        outerScopeVar =
            100

        middleFunc middleArg =
            let
                innerFunc innerArg =
                    innerArg + middleArg + outerScopeVar
            in
            innerFunc 1
    in
    Expect.equal (middleFunc 10) 111


globalRecursion : Int -> Int
globalRecursion i =
    if i <= 1 then
        1
    else
        globalRecursion (i-1) + globalRecursion (i-2)


testGlobalRecursion : Test
testGlobalRecursion =
    test "non-tail recursion should work in top-level functions" <|
        \() -> Expect.equal (globalRecursion 6) 13


testLocalRecursion : Test
testLocalRecursion =
    test "non-tail recursion should work in local functions" <|
        \() ->
            let
                local i =
                    if i <= 1 then
                        1
                    else
                        local (i-1) + local (i-2)
            in
            Expect.equal (local 6) 13


globalTailRecursion : Int -> Int -> Int
globalTailRecursion acc x =
    if x <= 1 then
        acc
    else
        globalTailRecursion (acc * x) (x - 1)


testGlobalTailRecursion : Test
testGlobalTailRecursion =
    test "tail recursion should work in top-level functions" <|
        \() -> Expect.equal (globalTailRecursion 1 5) 120


testLocalTailRecursion : Test
testLocalTailRecursion =
    test "tail recursion should work in local functions" <|
        \() ->
            let
                local acc x =
                    if x <= 1 then
                        acc
                    else
                        local (acc * x) (x - 1)
            in
            Expect.equal (local 1 5) 120
