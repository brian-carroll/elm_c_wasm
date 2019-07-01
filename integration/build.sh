emcc src/update.c \
  -O3 \
  -g \
  -o build/update.js \
  -s ASSERTIONS=1 \
  -s MODULARIZE=1 \
  -s EXPORT_NAME='createEmscriptenModule' \
  -s EXPORTED_FUNCTIONS='["update"]' \
  -s EXTRA_EXPORTED_RUNTIME_METHODS='["ccall", "cwrap"]' \
  && echo "emcc success" \
  &

elm make src/Main.elm --output build/elm.js > /dev/null \
  && echo "elm success" \
  &

wait

cp build/update.wasm* dist


echo "combining JS files"

insert_before='var author$project$Main$main'
line_no=$(grep -n "$insert_before" build/elm.js | cut -d: -f1)
before_lines=$(($line_no-1))

elm_code="
$(head -n $before_lines build/elm.js)
$(cat src/patch.js)
$(tail +$line_no build/elm.js)
"
emscripten_code=$(cat build/update.js)
init_code=$(cat src/init.js)

echo "
$emscripten_code
createEmscriptenModule().then(function(Module) {
$elm_code
$init_code
});
" > dist/bundle.js
