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

extern Closure Basics_negate;  // is not a kernel function in Elm-JS

extern Closure Basics_add;
extern Closure Basics_sub;
extern Closure Basics_mul;
extern Closure Basics_fdiv;
extern Closure Basics_idiv;
extern Closure Basics_pow;

extern Closure Basics_toFloat;
extern Closure Basics_floor;
extern Closure Basics_ceiling;

extern Closure Basics_not;
extern Closure Basics_and;
extern Closure Basics_or;

extern Closure Basics_modBy;
extern Closure Basics_remainderBy;
extern Closure Basics_log;

extern Closure Basics_identity;

#endif
