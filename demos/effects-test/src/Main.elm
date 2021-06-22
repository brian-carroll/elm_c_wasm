module Main exposing (main)

import Browser
import Browser.Events exposing (onKeyDown)
import Html exposing (..)
import Html.Attributes exposing (class)
import Html.Events exposing (onClick)
import Http
import Json.Decode as JD exposing (Value)
import WebAssembly


type alias Model =
    { text : String }


initialModel : Model
initialModel =
    { text = "Initialised" }


type Msg
    = ButtonClicked
    | KeyPressed String
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

        KeyPressed key ->
            ( { model | text = key }
            , Cmd.none
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
        [ ul []
            [ li [] [ text "Load a local JSON file over HTTP by either clicking the button or pressing any key" ]
            , li [] [ text "This app has 3 effect managers: Http (Cmd & Sub), Task (Cmd only), and Browser.Events (Sub only)" ]
            ]
        , div [ class "effects-test" ]
            [ button [ onClick ButtonClicked ] [ text "LOAD" ]
            , div [] [ text model.text ]
            ]
        ]


main : Program () Model Msg
main =
    Browser.element <|
        WebAssembly.intercept
            { init = \() -> ( initialModel, Cmd.none )
            , view = view
            , update = update
            , subscriptions = \_ -> onKeyDown (JD.map KeyPressed (JD.field "key" JD.string))
            }
