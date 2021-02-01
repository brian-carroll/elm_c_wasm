module Wasm exposing (tests)

import Expect
import Test exposing (..)
import Wasm.Calls
import Wasm.Functions
import Wasm.HttpTests
import Wasm.PatternMatching



{-
   stuff to test
       - core libs
       - ports
       - Cmd (done via HTTP)
       - Json (more complicated structures, passed in from JS)
       - Task
-}


tests : Test
tests =
    describe "Wasm code gen" <|
        []
            --   ++ [ Wasm.Calls.tests ]
            ++ [ Wasm.Functions.tests ]



--   ++ [ Wasm.HttpTests.tests ]
--   ++ [ Wasm.PatternMatching.tests ]
