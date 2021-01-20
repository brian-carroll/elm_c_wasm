module Core.CodeGen exposing (tests)

import Basics exposing (..)
import Expect
import Maybe exposing (..)
import Test exposing (..)


type Wrapper a
    = Wrapper a


caseUnderscore : Maybe number -> number
caseUnderscore m_ =
    case m_ of
        Just x ->
            x

        Nothing ->
            0


patternUnderscore : number
patternUnderscore =
    case Just 42 of
        Just x_ ->
            x_

        Nothing ->
            0


letQualified : number
letQualified =
    let
        (Wrapper x) =
            Wrapper 42
    in
    x


caseQualified : number
caseQualified =
    case Just 42 of
        Maybe.Just x ->
            x

        Nothing ->
            0


tests : Test
tests =
    let
        -- We don't strictly speaking need annotations in this let-expression,
        -- but having these here exercises the parser to avoid regressions like
        -- https://github.com/elm-lang/elm-compiler/issues/1535
        underscores : Test
        underscores =
            describe "Underscores"
                [ test "case" <| \() -> Expect.equal 42 (caseUnderscore (Just 42))
                , test "pattern" <| \() -> Expect.equal 42 patternUnderscore
                ]

        qualifiedPatterns : Test
        qualifiedPatterns =
            describe "Qualified Patterns"
                [ test "let" <| \() -> Expect.equal 42 letQualified
                , test "case" <| \() -> Expect.equal 42 caseQualified
                ]

        hex : Test
        hex =
            describe "Hex"
                [ test "0x7FFFFFFF" <|
                    \() ->
                        0x7FFFFFFF
                            |> Expect.equal 2147483647
                , test "0x8BADF00D" <|
                    \() ->
                        0x8BADF00D
                            |> Expect.equal 2343432205
                , test "0x00" <|
                    \() ->
                        0x00
                            |> Expect.equal 0
                ]
    in
    describe "CodeGen"
        [ underscores
        , qualifiedPatterns
        , hex
        ]
