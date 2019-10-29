module Main exposing (main)

import Html exposing (Html, button, div, h1, text)
import Html.Events exposing (onClick)
import Process
import Task
import WasmWrapper


type alias Model =
    Int


init : () -> ( Model, Cmd Msg )
init () =
    ( 0, Cmd.none )


type Msg
    = Start
    | Decrement


cmdDecrement =
    Task.perform (\_ -> Decrement) (Process.sleep 1000)


update : Msg -> Model -> ( Model, Cmd Msg )
update msg model =
    case msg of
        Start ->
            ( 5, cmdDecrement )

        Decrement ->
            if model == 0 then
                ( model, Cmd.none )

            else
                ( model - 1, cmdDecrement )


view : Model -> Html Msg
view model =
    let
        str =
            if model == 0 then
                "Click the button!"

            else
                String.fromInt model
    in
    div []
        [ h1 [] [ text str ]
        , button [ onClick Start ] [ text "Start countdown" ]
        ]


main : Program () Model Msg
main =
    WasmWrapper.element
        { init = init
        , view = view
        , update = update
        , subscriptions = \_ -> Sub.none
        }
