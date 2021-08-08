OUTNAME='elm'
KDIR=../../src/kernel
KERNELS="${KDIR}/core/core.c ${KDIR}/elm-test/elm-test.c ${KDIR}/json/json.c ${KDIR}/wrapper/wrapper.c"
WRAPPER_DIR=../../src/kernel/wrapper
CFLAGS=$@

OUT_DIR="build/emscripten"

extra_opt=""
# extra_opt="$extra_opt -g4"
# extra_opt="$extra_opt --source-map-base http://localhost:8080/demos/$(basename $(pwd))/$OUT_DIR/"

mkdir -p dist $OUT_DIR

echo "" > emcc.log

set -x

emcc $CFLAGS \
  build/codemods/$OUTNAME.c \
  $KERNELS \
  --pre-js $WRAPPER_DIR/emscripten-module.js \
  --pre-js $WRAPPER_DIR/wrapper.js \
  --js-library $WRAPPER_DIR/imports.js \
  --post-js $WRAPPER_DIR/elm-scopes-open.js.txt \
  --post-js $KDIR/core/scheduler.js \
  --post-js $KDIR/core/platform.js \
  --post-js build/elm/$OUTNAME.js \
  --post-js $WRAPPER_DIR/elm-scopes-close.js.txt \
  -o $OUT_DIR/$OUTNAME.js \
  -Wno-incompatible-pointer-types \
  -s ASSERTIONS=1 \
  -s ALLOW_MEMORY_GROWTH=1 \
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
