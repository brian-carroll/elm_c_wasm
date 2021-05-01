#include "../core.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../gc/internals.h"
#include "../../json/json.h" // for pretty printing JSON values

#include "./log.c"
#include "./debug-pretty.c"
#include "./debug-gc.c"
#include "./debug-elm.c"
