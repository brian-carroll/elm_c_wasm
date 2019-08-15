EM_DEBUG="-g4 --source-map-base http://localhost:8080/demos/benchmark/"

FILENAME="benchmark"
KERNELS=$(find ../../src/kernel -name '*.c')

emcc src/$FILENAME.c \
  $KERNELS \
  ../../src/test/gc/print-heap.c \
  -DDEBUG \
  -O3 \
  $EM_DEBUG \
  -o build/$FILENAME.html \
  -Wno-incompatible-pointer-types-discards-qualifiers \
  -s MODULARIZE=1 \
  -s EXPORT_NAME='createEmscriptenModule' \
  -s EXPORTED_FUNCTIONS='["main", "export_count", "export_add", "export_add_unboxed"]' \
  -s EXTRA_EXPORTED_RUNTIME_METHODS='["ccall", "cwrap"]' \
