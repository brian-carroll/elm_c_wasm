# EM_DEBUG="-g"
# ELM_OPTIMIZE='--optimize'

emcc src/update.c \
  -O3 \
  $EM_DEBUG \
  -o build/update.js \
  -s MODULARIZE=1 \
  -s EXPORT_NAME='createEmscriptenModule' \
  -s EXPORTED_FUNCTIONS='["update"]' \
  -s EXTRA_EXPORTED_RUNTIME_METHODS='["ccall", "cwrap"]' \
  && echo "emcc success" \
  &

elm make src/Main.elm $ELM_OPTIMIZE --output build/elm.js > /dev/null \
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
if [ -n "$ELM_OPTIMIZE" ] && which google-closure-compiler > /dev/null
then
  echo "Minifying Elm"
  elm_code=$(echo "$elm_code" | google-closure-compiler)
fi

emscripten_code=$(cat build/update.js)
init_code=$(cat src/init.js)

echo "
$emscripten_code
createEmscriptenModule().then(function(Module) {
$elm_code
$init_code
});
" > dist/bundle.js

deploy_dir=../../../gh-pages/update-int
mkdir -p $deploy_dir/dist
cp dist/* $deploy_dir/dist
cp index.html $deploy_dir
