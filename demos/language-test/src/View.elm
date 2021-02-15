module View exposing
    ( Config
    , defaultConfig
    , hidePassedTests
    , showPassedTests
    , viewResults
    , withFuzzCount
    )

import Array exposing (Array)
import Expect exposing (Expectation)
import Html exposing (Html, br, div, h3, li, p, pre, text, ul)
import Html.Attributes exposing (class, style)
import Maybe.Extra
import Random exposing (Seed, initialSeed)
import Test exposing (Test)
import Test.Runner as Runner exposing (Runner)
import Test.Runner.Failure as Failure


{-| Config object used to define options like the fuzz count, the initial seed, whether or not the passed tests should be displayed...
-}
type Config
    = Config { initialSeed : Seed, fuzzCount : Int, passedTestsDisplay : PassedTestsDisplay }


type PassedTestsDisplay
    = Show
    | Hide


type TestResult
    = Pass
    | Fail (List ( String, Failure.Reason ))



-- type Tree a
--     = Node String (List (Tree a))
--     | Leaf String a
-- nestResults : String -> List ( List String, TestResult ) -> ResultsTree
-- nestResults title flat =
--     nestResultsHelp (SuiteResult title []) flat
-- nestResultsHelp : String -> List ResultsTree -> ( List String, TestResult ) -> ResultsTree
-- nestResultsHelp suiteName subTrees (( flatLabels, flatResult ) as flat) =
--     case flatLabels of
--         [ suiteName, testName ] ->
--             SuiteResult suiteName (SingleResult testName flatResult :: subTrees)
--         _ ->
--             Debug.todo "yeah"
{-

   should capture a value across two scopes -> Functions -> Wasm code gen
   non-tail recursion should work in local functions -> Functions -> Wasm code gen
   tail recursion should work in top-level functions -> Things -> Wasm code gen
   tail recursion should work in local functions -> Stuff -> Foozle

    current subtree = []                         currentTree = ("", [])
    current titles = [Functions, Wasm code gen]  currentTree = ("", [("Wasm code gen", [("Functions", ["should capture"])])])
    current titles = [Functions, Wasm code gen]  currentTree = ("", [("Wasm code gen", [("Functions", ["should capture", "non-tail"])])])
    current titles = [Things, Wasm code gen]  currentTree = ("", [("Wasm code gen", [("Functions", ["should capture", "non-tail"]), ("Things", ["should"])])])

    Node "" []
    Node "" [Node "Wasm" [Node "Function" [Leaf "capture" Pass]]]
    Node "" [Node "Wasm" [Node "Function" [Leaf "non-tail" Pass, Leaf "capture" Pass]]]


    to insert one thing into the tree
        reverse the titles
        traverse the tree to find my place
            if I still have titles left to traverse on
                search first subtree for a match
                if not found, insert
                recurse down with next title
            if I'm out of titles, this is my gaff
                chuck that shit in
-}


type Tree a
    = Node String (List (Tree a))
    | Leaf String a


empty : Tree a
empty =
    Node "" []



-- treeInsert : ( List String, a ) -> Tree a -> Tree a
-- treeInsert ( titles, item ) tree =
--     case titles of
--         [] ->
--             tree
--         [ leafTitle ] ->
--             Leaf leafTitle item
--         first :: rest ->
--             case tree of
--                 Leaf _ _ -> Debug.todo "dammit"
--                 Node nodeTitle subTrees ->
--                     if nodeTitle == first then
-- getLeafInsertionPoint title item trees =
--     case List.Extra.find (titlesMatches title) trees of
--         Nothing ->
--             case titles of
--                 [] -> trees
--                 [leafTitle] ->
--                     Leaf title item :: trees
--                 first :: rest ->
--                     if first ==
--         Just matchingTree ->
--         |> Maybe.map ()
--         |> Maybe.withDefault (Leaf title item)


titleMatches : String -> Tree a -> Bool
titleMatches title tree =
    case tree of
        Leaf _ _ ->
            False

        Node nodeTitle _ ->
            title == nodeTitle


runTest : Runner -> ( List String, TestResult )
runTest runner =
    let
        failures =
            runner.run ()
                |> List.map parseExpectation
                |> List.filterMap identity

        labels =
            List.reverse runner.labels
    in
    case failures of
        [] ->
            ( labels, Pass )

        _ ->
            ( labels, Fail failures )


parseExpectation : Expectation -> Maybe ( String, Failure.Reason )
parseExpectation expectation =
    Runner.getFailureReason expectation
        |> Maybe.map (\record -> ( record.description, record.reason ))


{-| Run and display test results

    viewResults myConfig myTests

-}
viewResults : Config -> Test -> Html a
viewResults (Config config) testsToRun =
    case Runner.fromTest config.fuzzCount config.initialSeed testsToRun of
        Runner.Plain tests ->
            viewTestsResults config.passedTestsDisplay tests

        Runner.Only tests ->
            viewTestsResults config.passedTestsDisplay tests

        Runner.Skipping tests ->
            viewTestsResults config.passedTestsDisplay tests

        Runner.Invalid reason ->
            div []
                [ p [] [ text "Invalid tests:" ]
                , pre [] [ text reason ]
                ]


viewTestsResults : PassedTestsDisplay -> List Runner -> Html a
viewTestsResults passedTestsDisplay runners =
    let
        allResults =
            List.map runTest runners

        ( passResults, failResults ) =
            List.partition
                (\( _, result ) -> result == Pass)
                allResults

        nPass =
            String.fromInt (List.length passResults)

        nFail =
            String.fromInt (List.length failResults)

        summary =
            ul []
                [ li [] [ text (nPass ++ " passed") ]
                , li [] [ text (nFail ++ " failed") ]
                ]
    in
    div [] <|
        [ summary
        , ul [ class "test-fail-list" ]
            (List.map viewFail failResults)
        ] ++
        if passedTestsDisplay == Show then
            [ ul [ class "test-pass-list" ]
                (List.map viewPass passResults)
            , summary
            ]
        else
            []



{-| this string contains non-breaking spaces
-}
labelSeparator : String
labelSeparator =
    " → "


viewPass : ( List String, TestResult ) -> Html a
viewPass ( labels, _ ) =
    li []
        [ text ("✔️ " ++ String.join labelSeparator labels)
        ]


viewFail : ( List String, TestResult ) -> Html a
viewFail ( labels, result ) =
    let
        expectationFailures =
            case result of
                Pass ->
                    []

                Fail failures ->
                    failures
    in
    li []
        [ h3 [] [ text ("❌ " ++ String.join labelSeparator labels) ]
        , ul [] <| List.map viewExpectationFailure expectationFailures
        ]


viewExpectationFailure : ( String, Failure.Reason ) -> Html a
viewExpectationFailure ( name, reason ) =
    li []
        [ text name
        , br [] []
        , viewReason reason
        ]


viewReason : Failure.Reason -> Html msg
viewReason reason =
    case reason of
        Failure.Custom ->
            text ""

        Failure.Equality expected actual ->
            ul []
                [ li [] [ text ("Expected: " ++ expected) ]
                , li [] [ text ("Actual: " ++ actual) ]
                ]

        Failure.Comparison expected actual ->
            ul []
                [ li [] [ text ("Expected: " ++ expected) ]
                , li [] [ text ("Actual: " ++ actual) ]
                ]

        Failure.ListDiff expected received ->
            viewListDiff expected received

        Failure.CollectionDiff details ->
            ul []
                [ li [] [ text ("Expected: " ++ details.expected) ]
                , li [] [ text ("Actual: " ++ details.actual) ]
                ]

        Failure.TODO ->
            text "TODO"

        Failure.Invalid invalidReason ->
            viewInvalidReason invalidReason


viewListDiff : List String -> List String -> Html msg
viewListDiff expected actual =
    let
        expectedArray =
            Array.fromList expected

        actualArray =
            Array.fromList actual
    in
    div []
        [ text "The lists don't match! 😱"
        , div [ style "display" "flex" ]
            [ div [ style "margin-right" "5vw" ]
                [ text "Expected"
                , ul [] (List.indexedMap (viewListDiffPart actualArray) expected)
                ]
            , div []
                [ text "Actual"
                , ul [] (List.indexedMap (viewListDiffPart expectedArray) actual)
                ]
            ]
        ]


viewListDiffPart : Array String -> Int -> String -> Html msg
viewListDiffPart otherList index listPart =
    let
        styles =
            Array.get index otherList
                |> Maybe.Extra.filter (\value -> value == listPart)
                |> Maybe.map (always [ style "color" "green" ])
                |> Maybe.withDefault [ style "color" "red", style "font-weight" "bold" ]
    in
    li styles [ text listPart ]


viewInvalidReason : Failure.InvalidReason -> Html msg
viewInvalidReason invalidReason =
    case invalidReason of
        Failure.EmptyList ->
            text "You should have at least one test in the list"

        Failure.NonpositiveFuzzCount ->
            text "The fuzz count must be positive"

        Failure.InvalidFuzzer ->
            text "The fuzzer is invalid"

        Failure.BadDescription ->
            text "The description of your test is not valid"

        Failure.DuplicatedName ->
            text "At least two tests have the same name, please change at least one"


{-| Create a default config, initializing it with an initial seed used for fuzz testing.

    Random.initialSeed 10000 |> defaultConfig

-}
defaultConfig : Seed -> Config
defaultConfig initialSeed =
    Config
        { initialSeed = initialSeed
        , fuzzCount = 100
        , passedTestsDisplay = Show
        }


{-| Set the fuzz count in the Config. Should not be useful if you don't use fuzz testing.

    Random.initialSeed 10000 |> defaultConfig |> withFuzzCount 50

-}
withFuzzCount : Int -> Config -> Config
withFuzzCount runsCount (Config config) =
    Config { config | fuzzCount = runsCount }


{-| Indicates that the resulting HTML should show passed tests. This is the default behavior.

    Random.initialSeed 10000 |> defaultConfig |> showPassedTests

-}
showPassedTests : Config -> Config
showPassedTests (Config config) =
    Config { config | passedTestsDisplay = Show }


{-| Indicates that the resulting HTML should _NOT_ show passed tests.

    Random.initialSeed 10000 |> defaultConfig |> hidePassedTests

-}
hidePassedTests : Config -> Config
hidePassedTests (Config config) =
    Config { config | passedTestsDisplay = Hide }
