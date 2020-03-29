FILENAME="main"
# ELM_OPTIMIZE='--optimize'

# Wasm & sourcemap
mkdir -p dist
cp build/$FILENAME.wasm* dist

echo "combining JS files"

# Modify Elm's generated JavaScript by inserting two files
# into the middle of it.
#
# `wrapper.js`
# Creates a wrapper around the Wasm module that lets it talk to
# the Elm kernel runtime. It translates between the JS and Wasm
# representations of Elm data structures.
#
# `patch.js`
# Replaces the JS implementation of the app with the (wrapped)
# Wasm implementation, by overwriting the JS definition of
# WasmWrapper.element.
# Also provides some app-specific configuration to the wrapper
# (about what custom types it has, and so on).
#
# Eventually the Elm compiler itself would do all of this.
insert_before='var author$project$Main$main'
line_no=$(grep -n "$insert_before" build/elm.js | cut -d: -f1)
before_lines=$(($line_no-1))

elm_code="
$(head -n $before_lines build/elm.js)
$(cat src/patch.js)
$(cat build/wrapper.js)
$(tail +$line_no build/elm.js)
"


# Smash all the JS together in a big lump!
#
# We need to wait for the Wasm module to be compiled
# (which is asynchronous), and then wait for some
# initialisation that happens in the C `main` function.
# Emscripten does both for us and then calls `postRun`.
# Inside that callback, we are free to run the Elm code
# and initialise it as usual.
# Long term, can probably get Elm.Main.init to do this stuff
emscripten_code=$(cat build/$FILENAME.js)
init_code=$(cat src/init.js)

echo "
$emscripten_code
EmscriptenModule.postRun = function() {
$elm_code
$init_code
};
" > dist/bundle.js

