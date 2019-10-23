module Main exposing (main)

import Browser
import Html exposing (Html, br, button, div, text)
import Html.Events exposing (onClick)
import Task
import Time


type alias Model =
    Int


init : () -> ( Model, Cmd Msg )
init () =
    ( 0, Task.perform GotTime Time.now )


type Msg
    = GetTime
    | GotTime Time.Posix


update : Msg -> Model -> ( Model, Cmd Msg )
update msg model =
    case msg of
        GetTime ->
            ( model, Task.perform GotTime Time.now )

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
    Browser.element
        { init = init
        , view = view
        , update = update
        , subscriptions = \_ -> Sub.none
        }
