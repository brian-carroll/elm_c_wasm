module Main exposing (main)

import Browser
import Core
import Html exposing (..)
import Html.Attributes exposing (..)
import Random
import Result exposing (Result(..))
import View
import Wasm
import Wasm.JsEffects as JsEffects


type alias Model =
    JsEffects.Model


type alias Msg =
    JsEffects.Msg


view : Model -> Html Msg
view model =
    let
        testConfig =
            View.defaultConfig (Random.initialSeed 10000)
    in
    div [ class "view" ]
        [ div []
            [ h2 [] [ text "Wasm/JS Effects Test" ]
            , JsEffects.view model
            ]
        , div []
            [ h2 [] [ text "Wasm Code Gen Tests" ]
            , View.viewResults testConfig Wasm.tests
            ]
        , div []
            [ h2 [] [ text "Elm Standard Library Tests" ]
            , View.viewResults testConfig Core.tests
            ]
        ]


main : Program () Model Msg
main =
    Browser.element
        { init = \() -> ( JsEffects.initialModel, Cmd.none )
        , view = view
        , update = JsEffects.update
        , subscriptions = \_ -> Sub.none
        }
