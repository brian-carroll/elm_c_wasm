module Main exposing (main)

import Benchmark exposing (..)
import Benchmark.Runner exposing (BenchmarkProgram, program)


{-| Wrapper type for JS values
We can use this to make Elm-JS integers more similar to
Elm-Wasm integers, by wrapping them in a data structure.
This helps to understand where perf differences come from.
-}
type Box a
    = Box a


main : BenchmarkProgram
main =
    program suite


count : Int
count =
    100


suite : Benchmark
suite =
    describe "" <|
        -- use append syntax for easier commenting-out
        []
            ++ [ addJsVsWasm ]
            ++ [ addJsVsWasmUnboxed ]
            ++ [ addJsVsWasmWithoutA2 ]
            ++ [ countJsVsWasm ]
            ++ [ countBoxedVsUnboxed ]
            ++ [ countTCE ]
            ++ []



-- COUNT


countJsVsWasm : Benchmark
countJsVsWasm =
    Benchmark.compare ("Count to " ++ String.fromInt count ++ " (Wasm vs JS)")
        "Wasm Boxed"
        (\_ -> wasmCount count)
        "JS Boxed"
        (\_ -> jsCountBoxed (Box count))


countTCE : Benchmark
countTCE =
    Benchmark.compare "Wasm tail call elimination (count)"
        "Tail call eliminated"
        (\_ -> wasmCount count)
        "Not eliminated"
        (\_ -> wasmCountNoTCE count)


countBoxedVsUnboxed : Benchmark
countBoxedVsUnboxed =
    Benchmark.compare ("Count to " ++ String.fromInt count ++ " (JS only)")
        "JS Boxed"
        (\_ -> jsCountBoxed (Box count))
        "JS Unboxed"
        (\_ -> jsCountUnboxed count)


jsCountUnboxed : Int -> ()
jsCountUnboxed remaining =
    if remaining == 0 then
        ()

    else
        jsCountUnboxed (remaining - 1)


jsCountBoxed : Box Int -> ()
jsCountBoxed (Box remaining) =
    if remaining == 0 then
        ()

    else
        jsCountBoxed (Box (remaining - 1))


{-| Will be replaced with Wasm version via build-combine.sh & patch.js. Just need something to keep Elm compiler happy.
-}
wasmCount : Int -> ()
wasmCount =
    jsCountUnboxed


{-| Will be replaced with Wasm version via build-combine.sh & patch.js. Just need something to keep Elm compiler happy.
-}
wasmCountNoTCE : Int -> ()
wasmCountNoTCE =
    jsCountUnboxed



-- ADD


{-| Compare Wasm add to JS add
Since Wasm Elm implementation uses "boxed" integers, do the same in JS
-}
addJsVsWasm : Benchmark
addJsVsWasm =
    Benchmark.compare "Add boxed"
        "Wasm"
        (\_ -> wasmAdd 123 456)
        "JS"
        (\_ -> jsAddBoxed (Box 123) (Box 456))


{-| Will be replaced with Wasm version via build-combine.sh & patch.js. Just need something to keep Elm compiler happy.
-}
wasmAdd : Int -> Int -> Int
wasmAdd =
    jsAddUnboxed


{-| Compare Wasm add to JS add, but for Wasm use plain C ints rather than the 'ElmInt' struct
This would not be workable in reality without huge changes to the Elm-Wasm runtime,
but it's an interesting data point for analysis
-}
addJsVsWasmUnboxed : Benchmark
addJsVsWasmUnboxed =
    Benchmark.compare "Add unboxed"
        "Wasm"
        (\_ -> wasmAddUnboxed 123 456)
        "JS"
        (\_ -> jsAddUnboxed 123 456)


{-| Will be replaced with Wasm version via build-combine.sh & patch.js. Just need something to keep Elm compiler happy.
-}
wasmAddUnboxed : Int -> Int -> Int
wasmAddUnboxed =
    jsAddUnboxed


addJsVsWasmWithoutA2 : Benchmark
addJsVsWasmWithoutA2 =
    Benchmark.compare "Add unboxed without A2"
        "Wasm"
        (\_ -> wasmAddUnboxedNoA2 123 456)
        "JS"
        (\_ -> jsAddUnboxedNoA2 123 456)


{-| Will be replaced with Wasm version, and have Elm's 'A2' helper removed from call, via build-combine.sh & patch.js
-}
wasmAddUnboxedNoA2 =
    wasmAddUnboxed


{-| Will have Elm's 'A2' helper removed from call via build-combine.sh
-}
jsAddUnboxedNoA2 =
    jsAddUnboxed


addBoxedVsUnboxed : Benchmark
addBoxedVsUnboxed =
    Benchmark.compare "Add"
        "JS Boxed"
        (\_ -> jsAddBoxed (Box 123) (Box 456))
        "JS Unboxed"
        (\_ -> jsAddUnboxed 123 456)


jsAddUnboxed : Int -> Int -> Int
jsAddUnboxed a b =
    a + b


jsAddBoxed : Box Int -> Box Int -> Box Int
jsAddBoxed (Box a) (Box b) =
    Box (a + b)
