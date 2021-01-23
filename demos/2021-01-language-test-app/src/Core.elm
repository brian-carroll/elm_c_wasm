module Core exposing (..)

import Core.Array as Array
import Core.Basics as Basics
import Core.Bitwise as Bitwise
import Core.Char as Char
import Core.CodeGen as CodeGen
import Core.Dict as Dict
import Core.Equality as Equality
import Core.List as List
import Core.Maybe as Maybe
import Core.Result as Result
import Core.Set as Set
import Core.String as String
import Core.Tuple as Tuple
import Test exposing (..)


tests : Test
tests =
    describe "Elm Standard Library Tests" <|
        []
        ++ [ Maybe.tests ]
        ++ [ CodeGen.tests ]


mostlyWorking : List Test
mostlyWorking =
    []
        ++ [ Basics.tests ]
        ++ [ Bitwise.tests ]
        ++ [ Char.tests ]
        ++ [ CodeGen.tests ]
        ++ [ Equality.tests ]
        ++ [ Result.tests ]
        ++ [ String.tests ]
        ++ [ Maybe.tests ]
        ++ [ Tuple.tests ]
        ++ [ List.tests ]


{-| mostly just broken because of GC!
Tests use structures with thousands of elements
-}
notWorkingYet : List Test
notWorkingYet =
    []
        ++ [ Array.tests ]
        ++ [ Dict.tests ]
        ++ [ Set.tests ]
