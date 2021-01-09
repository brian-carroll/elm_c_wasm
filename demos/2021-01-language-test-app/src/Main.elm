module Main exposing (main)

import Browser
import Html exposing (..)
import Html.Attributes exposing (..)
import Html.Events exposing (onClick)
import Http
import Json.Decode as JD
import Result exposing (Result(..))



{-
   stuff to test
       - ports
       - Cmd (done via HTTP)
       - Json (more complicated structures, passed in from JS)
       - Task
-}


type alias Model =
    { text : String }


initialModel : Model
initialModel =
    { text = "Initialised" }


type Msg
    = ButtonClicked
    | JsonLoaded (Result Http.Error String)


getJson : Cmd Msg
getJson =
    Http.get
        { url = "./assets/data.json"
        , expect = Http.expectJson JsonLoaded JD.string
        }


update : Msg -> Model -> ( Model, Cmd Msg )
update msg model =
    case msg of
        ButtonClicked ->
            ( { model | text = "Loading..." }
            , getJson
            )

        JsonLoaded (Ok jsonString) ->
            ( { model | text = jsonString }
            , Cmd.none
            )

        JsonLoaded (Err _) ->
            ( { model | text = "Error!" }
            , Cmd.none
            )


view : Model -> Html Msg
view model =
    div [ class "view" ]
        [ button [ onClick ButtonClicked ] [ text "LOAD" ]
        , div [] [ text model.text ]
        ]


main : Program () Model Msg
main =
    Browser.element
        { init = \() -> ( initialModel, Cmd.none )
        , view = view
        , update = update
        , subscriptions = \_ -> Sub.none
        }
