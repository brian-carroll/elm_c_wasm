module WasmWrapper exposing (element)

import Browser
import Html exposing (Html)


{-| element

A "pass-through" wrapper for Browser.element

According to the Elm code, this does nothing...
But in reality it provides a handy line in the generated JS
that can be _overwritten_ to insert the Wasm implementation!
Oooh, sneaky!

This is one of several build-time hacks that are done
after the Elm compiler has run. The guilty party is the
Bash script `build-combine.sh`, called from the `Makefile`

-}
element :
    { init : flags -> ( model, Cmd msg )
    , view : model -> Html msg
    , update : msg -> model -> ( model, Cmd msg )
    , subscriptions : model -> Sub msg
    }
    -> Program flags model msg
element mainRecord =
    Browser.element mainRecord
