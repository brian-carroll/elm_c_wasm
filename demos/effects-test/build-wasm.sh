FILENAME='elm'
KDIR=../../src/kernel
KERNELS="${KDIR}/core/core.c ${KDIR}/elm-test/elm-test.c ${KDIR}/json/json.c ${KDIR}/wrapper/wrapper.c"WRAPPER_DIR=../../src/kernel/wrapper

extra_opt=""
extra_opt="$extra_opt -DDEBUG"
# extra_opt="$extra_opt -DDEBUG_LOG"
extra_opt="$extra_opt -O1"

extra_opt="$extra_opt -g4"
extra_opt="$extra_opt --source-map-base http://localhost:8080/demos/$(basename $(pwd))/dist/"

mkdir -p dist

echo "" > emcc.log

set -x

emcc build/codemods/$FILENAME.c \
  $KERNELS \
  --pre-js $WRAPPER/emscripten-module.js \
  --pre-js $WRAPPER/wrapper.js \
  --js-library $WRAPPER/imports.js \
  --post-js build/elm/$FILENAME.js \
  -o dist/$FILENAME.js \
  -Wno-incompatible-pointer-types \
  -s ASSERTIONS=1 \
  -s ALLOW_MEMORY_GROWTH=1 \
  -s EXPORT_NAME='EmscriptenModule' \
  -s EXPORTED_FUNCTIONS="$(cat $WRAPPER/exported-functions.json)" \
  -s EXPORTED_RUNTIME_METHODS='["ccall", "cwrap"]' \
  -ferror-limit=10000 \
  $extra_opt \
  &> emcc.log

#   -s EXPORT_NAME='EmscriptenModule' \
#   -s MODULARIZE=1 \

set +x


if grep error emcc.log
then
  head -n 20 emcc.log
else
  echo
  echo "SUCCESS"
  echo
fi
