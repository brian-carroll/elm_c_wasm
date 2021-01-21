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



-- scopes : Test
-- scopes =
--     describe "scopes"
--         [ test "parallel let scopes should have separate namespaces" <|
--             \() ->
--                 let
--                     x1 =
--                         let
--                             x =
--                                 5
--                         in
--                         x
--                     x2 =
--                         let
--                             x =
--                                 7
--                         in
--                         x
--                 in
--                 Expect.equal (x1 + x2) 12
--         ]


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

        -- In this Wasm implementation, integers have to be 32 bits _total_, including the sign: -2^31 -> 2^31-1
        -- That's the same size as a Wasm pointer, which makes speed optimisations much easier
        hex : Test
        hex =
            describe "Hex"
                [ test "0x7FFFFFFF" <|
                    \() ->
                        0x7FFFFFFF
                            |> Expect.equal 2147483647
                , test "0x0C0010FF" <|
                    \() ->
                        0x0C0010FF
                            |> Expect.equal 201330943
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

        -- , scopes
        ]
