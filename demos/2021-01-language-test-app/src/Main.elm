module Main exposing (main)

import Browser
import Core
import Html exposing (..)
import Html.Attributes exposing (..)
import Html.Events exposing (onClick)
import Http
import Json.Decode as JD exposing (Value)
import Random
import Result exposing (Result(..))
import Test.Runner.Html as TestHtml



{-
   stuff to test
       - core libs
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
    let
        testConfig =
            TestHtml.defaultConfig (Random.initialSeed 10000)
    in
    div [ class "view" ]
        [ button [ onClick ButtonClicked ] [ text "LOAD" ]
        , div [] [ text model.text ]
        , div []
            [ h1 [] [ text "core tests" ]
            , TestHtml.viewResults testConfig Core.tests
            ]
        ]


main : Program () Model Msg
main =
    Browser.element
        { init = \() -> ( initialModel, Cmd.none )
        , view = view
        , update = update
        , subscriptions = \_ -> Sub.none
        }



{-
   TODO
   pattern match tests


   tuple destructuring
   scope tests with `let`, local function vars, nesting...
   TCE free vars, scopes
   functin application tests
    - partial application
    - too many args
    - apply args to returned function

-}
