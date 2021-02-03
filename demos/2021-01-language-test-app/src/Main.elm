module Main exposing (main)

import Browser
import Core
import Html exposing (..)
import Html.Attributes exposing (..)
import Random
import Result exposing (Result(..))
import View
import Wasm
import Wasm.HttpTests as HttpTests


type alias Model =
    HttpTests.Model


type alias Msg =
    HttpTests.Msg


view : Model -> Html Msg
view model =
    let
        testConfig =
            View.defaultConfig (Random.initialSeed 10000)
    in
    div [ class "view" ]
        [ div []
            [ HttpTests.view model ]
        , div []
            [ View.viewResults testConfig Wasm.tests

            -- , View.viewResults testConfig Core.tests
            ]
        ]


main : Program () Model Msg
main =
    Browser.element
        { init = \() -> ( HttpTests.initialModel, Cmd.none )
        , view = view
        , update = HttpTests.update
        , subscriptions = \_ -> Sub.none
        }
