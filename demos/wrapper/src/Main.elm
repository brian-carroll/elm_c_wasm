module Main exposing (main)

import Html exposing (Html, br, button, div, text)
import Html.Events exposing (onClick)
import Task
import Time
import WasmWrapper


type alias Model =
    Int


cmdTime : Cmd Msg
cmdTime =
    Task.perform GotTime Time.now


init : () -> ( Model, Cmd Msg )
init () =
    ( 0, cmdTime )


type Msg
    = GetTime
    | GotTime Time.Posix


update : Msg -> Model -> ( Model, Cmd Msg )
update msg model =
    case msg of
        GetTime ->
            ( model, cmdTime )

        GotTime posix ->
            ( Time.posixToMillis posix, Cmd.none )


view : Model -> Html Msg
view model =
    div []
        [ text (String.fromInt model)
        , br [] []
        , button [ onClick GetTime ] [ text "Refresh" ]
        ]


main : Program () Model Msg
main =
    WasmWrapper.element
        { init = init
        , view = view
        , update = update
        , subscriptions = \_ -> Sub.none
        }
