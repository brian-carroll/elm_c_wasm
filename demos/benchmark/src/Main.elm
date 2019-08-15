module Main exposing (main)

import Benchmark exposing (..)
import Benchmark.Runner exposing (BenchmarkProgram, program)


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
            ++ [ addJsVsWasmUnboxed ]
            ++ [ addJsVsWasm ]
            ++ [ countJsVsWasm ]



-- ++ [ countBoxedVsUnboxed ]
-- COUNT


countJsVsWasm : Benchmark
countJsVsWasm =
    Benchmark.compare ("Count to " ++ String.fromInt count ++ " Wasm vs JS")
        "Wasm Boxed"
        (\_ -> wasmCount count)
        "JS Boxed"
        (\_ -> jsCountBoxed (Box count))


countBoxedVsUnboxed : Benchmark
countBoxedVsUnboxed =
    Benchmark.compare ("Count to " ++ String.fromInt count ++ " JS only")
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


wasmCount : Int -> ()
wasmCount =
    jsCountUnboxed



-- ADD


addJsVsWasm : Benchmark
addJsVsWasm =
    Benchmark.compare "Add boxed"
        "Wasm"
        (\_ -> wasmAdd 123 456)
        "JS"
        (\_ -> jsAddBoxed (Box 123) (Box 456))


addJsVsWasmUnboxed : Benchmark
addJsVsWasmUnboxed =
    Benchmark.compare "Add unboxed"
        "Wasm"
        (\_ -> wasmAddUnboxed 123 456)
        "JS"
        (\_ -> jsAddUnboxed 123 456)


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


wasmAdd : Int -> Int -> Int
wasmAdd =
    jsAddUnboxed


wasmAddUnboxed : Int -> Int -> Int
wasmAddUnboxed =
    jsAddUnboxed
