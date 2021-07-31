port module Main exposing (main)

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
    = LoadClicked
    | KeyPressed String
    | JsonLoaded (Result Http.Error String)
    | PortRoundtripClicked
    | ReceivedFromJs ( String, String )


port receiveFromJs : (( String, String ) -> msg) -> Sub msg


port sendToJs : ( String, String ) -> Cmd msg


getJson : Cmd Msg
getJson =
    Http.send JsonLoaded <|
        Http.get "./assets/data.json" JD.string


update : Msg -> Model -> ( Model, Cmd Msg )
update msg model =
    case msg of
        LoadClicked ->
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

        PortRoundtripClicked ->
            ( model
            , sendToJs ( "Round", "Trip" )
            )

        ReceivedFromJs ( a, b ) ->
            ( { model | text = a ++ " " ++ b }
            , Cmd.none
            )


view : Model -> Html Msg
view model =
    div []
        [ ul []
            [ li [] [ text "Load a local JSON file over HTTP" ]
            , li []
                [ text "This app has 3 effect managers and two ports"
                , ul []
                    [ li [] [ text "Http (Cmd & Sub)" ]
                    , li [] [ text "Task (Cmd only)" ]
                    , li [] [ text "Browser.Events (Sub only)" ]
                    ]
                ]
            ]
        , div [ class "effects-test" ]
            [ button [ onClick LoadClicked ] [ text "Load Http" ]
            , button [ onClick PortRoundtripClicked ] [ text "Port round-trip" ]
            , div [] [ text model.text ]
            ]
        ]


keyFieldDecoder : JD.Decoder String
keyFieldDecoder =
    JD.field "key" JD.string


eventValueDecoder : JD.Decoder Msg
eventValueDecoder =
    JD.map KeyPressed keyFieldDecoder


subscriptions =
    \_ ->
        Sub.batch
            [ receiveFromJs ReceivedFromJs
            , onKeyDown eventValueDecoder
            ]


init =
    \() -> ( initialModel, Cmd.none )


main : Program () Model Msg
main =
    Browser.element <|
        WebAssembly.intercept
            { init = init
            , view = view
            , update = update
            , subscriptions = subscriptions
            }
