FILENAME="mock-app"
KERNELS=$(find ../../kernel -name '*.c')

extra_opt=""
# extra_opt="$extra_opt -DDEBUG"
# extra_opt="$extra_opt -DDEBUG_LOG"
extra_opt="$extra_opt -O3"

# extra_opt="$extra_opt -g4"
# extra_opt="$extra_opt --source-map-base http://localhost:8080/demos/benchmark/"

set -x

emcc src/$FILENAME.c \
  $KERNELS \
  ../../test/gc/print-heap.c \
  -o build/$FILENAME.html \
  -Wno-incompatible-pointer-types-discards-qualifiers \
  -s MODULARIZE=1 \
  -s EXPORT_NAME='createEmscriptenModule' \
  -s EXPORTED_FUNCTIONS="$(cat ./exported-functions.json)" \
  -s EXTRA_EXPORTED_RUNTIME_METHODS='["ccall", "cwrap"]' \
  $extra_opt

set +x
