module Wasm exposing (tests)

import Expect
import Test exposing (..)
import Wasm.Calls
import Wasm.Functions
import Wasm.JsEffects
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



--   ++ [ Wasm.JsEffects.tests ]
--   ++ [ Wasm.PatternMatching.tests ]
