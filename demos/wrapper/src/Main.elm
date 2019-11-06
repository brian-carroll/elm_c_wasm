module Main exposing (main)

import Html exposing (Html, br, button, div, h1, text)
import Html.Events exposing (onClick)
import Process
import Task
import WasmWrapper


type alias Model =
    Int


type Msg
    = SetCounter Int


init : () -> ( Model, Cmd Msg )
init _ =
    ( 0, Cmd.none )


funcSentToJsAndBack : Int -> () -> Msg
funcSentToJsAndBack next _ =
    SetCounter next


delayedSetCounter : Int -> Cmd Msg
delayedSetCounter next =
    let
        partiallyAppliedFuncSentToJsAndBack : () -> Msg
        partiallyAppliedFuncSentToJsAndBack =
            funcSentToJsAndBack next
    in
    Task.perform partiallyAppliedFuncSentToJsAndBack (Process.sleep 1234.5)


update : Msg -> Model -> ( Model, Cmd Msg )
update msg _ =
    case msg of
        SetCounter newModel ->
            let
                cmd =
                    if newModel == 0 then
                        Cmd.none

                    else
                        delayedSetCounter (newModel - 1)
            in
            ( newModel, cmd )


view : Model -> Html Msg
view model =
    let
        isZero =
            model == 0

        str =
            if isZero then
                "Click the button!"

            else
                String.fromInt model
    in
    div []
        [ h1 [] [ text str ]
        , if isZero then
            button [ onClick (SetCounter 5) ] [ text "Start countdown" ]

          else
            br [] []
        ]


subscriptions _ =
    Sub.none


main : Program () Model Msg
main =
    WasmWrapper.element
        { init = init
        , view = view
        , update = update
        , subscriptions = subscriptions
        }
