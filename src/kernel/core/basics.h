#ifndef ELM_KERNEL_BASICS
#define ELM_KERNEL_BASICS

#include "types.h"

/*
  , (+), (-), (*), (/), (//), (^)
  , toFloat, round, floor, ceiling, truncate
  , (==), (/=)
  , (<), (>), (<=), (>=), max, min, compare, Order(..)
  , Bool(..), not, (&&), (||), xor
  , (++)
  , modBy, remainderBy, negate, abs, clamp, sqrt, logBase, e
  , pi, cos, sin, tan, acos, asin, atan, atan2
  , degrees, radians, turns
  , toPolar, fromPolar
  , isNaN, isInfinite
  , identity, always, (<|), (|>), (<<), (>>), Never, never
*/

Closure Basics_negate;  // is not a kernel function in Elm-JS

Closure Basics_add;
Closure Basics_sub;
Closure Basics_mul;
Closure Basics_fdiv;
Closure Basics_idiv;
Closure Basics_pow;

Closure Basics_toFloat;
Closure Basics_floor;
Closure Basics_ceiling;

Closure Basics_not;
Closure Basics_and;
Closure Basics_or;

Closure Basics_modBy;
Closure Basics_remainderBy;
Closure Basics_log;

Closure Basics_identity;

#endif
