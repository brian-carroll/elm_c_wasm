emcc src/update.c \
  -O3 \
  -o dist/update-wasm.html \
  -s WASM=1 \
  -s EXPORTED_FUNCTIONS='["update"]' \
  -s EXTRA_EXPORTED_RUNTIME_METHODS='["ccall", "cwrap"]'

wasm2wat dist/update-wasm.wasm >  dist/update-wasm.wat

