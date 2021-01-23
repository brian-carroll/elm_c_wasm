FILENAME='elm'
KERNELS=$(find ../../src/kernel -name '*.c')

extra_opt=""
extra_opt="$extra_opt -DDEBUG"
# extra_opt="$extra_opt -DDEBUG_LOG"
extra_opt="$extra_opt -O1"

extra_opt="$extra_opt -g4"
extra_opt="$extra_opt --source-map-base http://localhost:8080/demos/$(basename $(pwd))/"

mkdir -p dist

echo "" > emcc.log

set -x

emcc build/codemods/$FILENAME.c \
  $KERNELS \
  --pre-js ../../src/kernel/wrapper/emscripten-module.js \
  --pre-js ../../src/kernel/wrapper/wrapper.js \
  --js-library ../../src/kernel/wrapper/imports.js \
  --post-js build/elm/elm.js \
  -o dist/$FILENAME.js \
  -Wno-incompatible-pointer-types \
  -s ASSERTIONS=1 \
  -s EXPORT_NAME='EmscriptenModule' \
  -s EXPORTED_FUNCTIONS="$(cat ./exported-functions.json)" \
  -s EXPORTED_RUNTIME_METHODS='["ccall", "cwrap"]' \
  -ferror-limit=10000 \
  $extra_opt \
  &> emcc.log

#   -s EXPORT_NAME='EmscriptenModule' \
#   -s MODULARIZE=1 \

set +x