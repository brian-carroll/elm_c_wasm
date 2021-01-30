module Wasm.HttpTests exposing (Model, Msg, initialModel, update, view)

import Browser
import Html exposing (..)
import Html.Events exposing (onClick)
import Http
import Json.Decode as JD exposing (Value)


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
    div []
        [ button [ onClick ButtonClicked ] [ text "LOAD" ]
        , div [] [ text model.text ]
        ]
