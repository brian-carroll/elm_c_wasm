FILENAME='elm'
KERNELS=$(find ../../src/kernel -name '*.c')

extra_opt=""
# extra_opt="$extra_opt -DDEBUG"
# extra_opt="$extra_opt -DDEBUG_LOG"
extra_opt="$extra_opt -O1"

extra_opt="$extra_opt -g4"
extra_opt="$extra_opt --source-map-base http://localhost:8080/demos/$(basename $(pwd))/"

mkdir -p build/emscripten

set -x

emcc build/emscripten/$FILENAME.c \
  $KERNELS \
  --js-library ../../src/kernel/wrapper/imports.js \
  -o build/emscripten/$FILENAME.js \
  -Wno-incompatible-pointer-types \
  -s MODULARIZE_INSTANCE=1 \
  -s ASSERTIONS=1 \
  -s EXPORT_NAME='EmscriptenModule' \
  -s EXPORTED_FUNCTIONS="$(cat ./exported-functions.json)" \
  -s EXTRA_EXPORTED_RUNTIME_METHODS='["ccall", "cwrap"]' \
  -ferror-limit=10000 \
  $extra_opt \
  &> emcc.log

set +x
