module Wasm.Functions exposing (tests)

import Expect exposing (Expectation)
import Test exposing (..)


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


fibGlobalRec : Int -> Int
fibGlobalRec i =
    if i <= 1 then
        1

    else
        fibGlobalRec (i - 1) + fibGlobalRec (i - 2)


testGlobalRecursion : Test
testGlobalRecursion =
    test "non-tail recursion should work in top-level functions" <|
        \() -> Expect.equal (fibGlobalRec 6) 13


testLocalRecursion : Test
testLocalRecursion =
    test "non-tail recursion should work in local functions" <|
        \() ->
            let
                fibLocal i =
                    if i <= 1 then
                        1

                    else
                        fibLocal (i - 1) + fibLocal (i - 2)
            in
            Expect.equal (fibLocal 6) 13


fibLocalTailRec : Int -> Int
fibLocalTailRec n =
    let
        loop : Int -> Int -> Int -> Int
        loop i a b =
            if i == n then
                a

            else
                loop (i + 1) b (a + b)
    in
    loop 0 0 1


testLocalTailRecursion : Test
testLocalTailRecursion =
    test "tail recursion should work in local functions" <|
        \() ->
            Expect.equal 55 (fibLocalTailRec 10)


fibGlobalTailRec : Int -> Int
fibGlobalTailRec n =
    fibGlobalTailRecHelp n 0 0 1


fibGlobalTailRecHelp : Int -> Int -> Int -> Int -> Int
fibGlobalTailRecHelp n i a b =
    if i == n then
        a

    else
        fibGlobalTailRecHelp n (i + 1) b (a + b)


testGlobalTailRecursion : Test
testGlobalTailRecursion =
    test "tail recursion should work in top-level functions" <|
        \() -> Expect.equal 55 (fibGlobalTailRec 10)
