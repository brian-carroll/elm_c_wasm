FILENAME='elm'
KERNELS=$(find ../../src/kernel -name '*.c')

extra_opt=""
# extra_opt="$extra_opt -DDEBUG"
# extra_opt="$extra_opt -DDEBUG_LOG"
extra_opt="$extra_opt -O3"

# extra_opt="$extra_opt -g4"
# extra_opt="$extra_opt --source-map-base http://localhost:8080/demos/wrapper/"

FIND='"kernel.h"'
REPLACE='"../../../../src/kernel/kernel.h"'

mkdir -p build/emscripten

node -e "
  const fs = require('fs');
  const orig = fs.readFileSync('build/elm/$FILENAME.c').toString();
  const updated = orig.replace('$FIND', '$REPLACE');
  console.log(updated);
" > build/emscripten/$FILENAME.c

set -x

emcc build/emscripten/$FILENAME.c \
  $KERNELS \
  -o build/emscripten/$FILENAME.js \
  -Wno-incompatible-pointer-types \
  -s MODULARIZE_INSTANCE=1 \
  -s EXPORT_NAME='EmscriptenModule' \
  -s EXPORTED_FUNCTIONS="$(cat ./exported-functions.json)" \
  -s EXTRA_EXPORTED_RUNTIME_METHODS='["ccall", "cwrap"]' \
  $extra_opt

set +x
