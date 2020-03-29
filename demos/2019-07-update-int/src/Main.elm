module Main exposing (main)

import Browser
import Html exposing (Html, button, div, text)
import Html.Events exposing (onClick)


type alias Model =
    Int


initialModel : Model
initialModel =
    0


type alias Msg =
    Int


update : Msg -> Model -> Model
update msg model =
    if msg > 0 then
        model + 1

    else
        model - 1


view : Model -> Html Msg
view model =
    div []
        [ button [ onClick 1 ] [ text "+1" ]
        , div [] [ text <| String.fromInt model ]
        , button [ onClick (-1) ] [ text "-1" ]
        ]


main : Program () Model Msg
main =
    Browser.sandbox
        { init = initialModel
        , view = view
        , update = update
        }
