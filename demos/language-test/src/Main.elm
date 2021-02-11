module Main exposing (main)

import Core
import Html exposing (..)
import Html.Attributes exposing (..)
import Random
import View
import Wasm


main : Html a
main =
    let
        testConfig =
            View.defaultConfig (Random.initialSeed 10000)
    in
    div [ class "view" ]
        [ div []
            [ h2 [] [ text "Wasm Code Gen Tests" ]
            , View.viewResults testConfig Wasm.tests
            ]
        , div []
            [ h2 [] [ text "Elm Standard Library Tests" ]
            , View.viewResults testConfig Core.tests
            ]
        ]
