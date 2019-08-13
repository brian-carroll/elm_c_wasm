module Main exposing (main)

import Benchmark exposing (..)
import Benchmark.Runner exposing (BenchmarkProgram, program)


type Box a
    = Box a


main : BenchmarkProgram
main =
    program suite


suite : Benchmark
suite =
    let
        count =
            100

        -- * 1000
    in
    describe "Count"
        [ benchmark "countUnboxed" <|
            \_ -> countUnboxed count
        , benchmark "countBoxed" <|
            \_ -> countBoxed (Box count)
        , benchmark "wasmCount" <|
            \_ -> wasmCount count
        ]


countUnboxed : Int -> ()
countUnboxed remaining =
    if remaining == 0 then
        ()

    else
        countUnboxed (remaining - 1)


countBoxed : Box Int -> ()
countBoxed (Box remaining) =
    if remaining == 0 then
        ()

    else
        countBoxed (Box (remaining - 1))


wasmCount =
    countUnboxed
