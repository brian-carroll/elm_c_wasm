FILENAME='elm'
KDIR=../../src/kernel
KERNELS="${KDIR}/core/core.c ${KDIR}/elm-test/elm-test.c ${KDIR}/json/json.c ${KDIR}/wrapper/wrapper.c"
WRAPPER_DIR=../../src/kernel/wrapper
CFLAGS=$@

extra_opt=""

# extra_opt="$extra_opt -g4"
# extra_opt="$extra_opt --source-map-base http://localhost:8080/demos/$(basename $(pwd))/dist/"

mkdir -p dist

echo "" > emcc.log

set -x


emcc $CFLAGS \
  build/codemods/$FILENAME.c \
  $KERNELS \
  --pre-js $WRAPPER_DIR/emscripten-module.js \
  --pre-js $WRAPPER_DIR/wrapper.js \
  --js-library $WRAPPER_DIR/imports.js \
  --post-js build/elm/$FILENAME.js \
  -o dist/$FILENAME.js \
  -Wno-incompatible-pointer-types \
  -s ASSERTIONS=1 \
  -s EXPORT_NAME='EmscriptenModule' \
  -s EXPORTED_FUNCTIONS="$(cat $WRAPPER_DIR/exported-functions.json)" \
  -s EXIT_RUNTIME=0 \
  -ferror-limit=10000 \
  $extra_opt \
  &> emcc.log

if grep error emcc.log
then
  cat emcc.log
fi

set +x
