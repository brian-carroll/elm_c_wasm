module Main exposing (main)

import Core
import Html exposing (..)
import Html.Attributes exposing (..)
import Random
import Test exposing (..)
import View
import Wasm


allTests : Test
allTests =
    describe "Wasm" <|
        [ Wasm.tests
        , Core.tests
        ]


main : Html a
main =
    let
        testConfig =
            View.defaultConfig (Random.initialSeed 10000)

        -- |> View.hidePassedTests
    in
    div [ class "view" ]
        [ View.viewResults testConfig allTests
        ]
