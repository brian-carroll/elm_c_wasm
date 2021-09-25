#!/bin/bash -e

DEPLOY=~/Documents/gh-pages


function elm_spa_example_wasm {
    pushd demos/elm-spa-example/

    make clean
    make release

    local target="$DEPLOY/elm-spa-example-wasm"
    rm -rf "$target/*"
    mkdir -p "$target/dist"

    cp -v index.html "$target/"
    cp -v dist/elm.js "$target/dist/"
    cp -v dist/elm.wasm "$target/dist/"

    cd $target
    rm -f assets
    ln -s ../assets

    popd
}


function elm_spa_example_js {
    pushd demos/elm-spa-example/repo

    elm make src/Main.elm --optimize --output elm.js

    echo "uglifying..."
    npx uglifyjs elm.js --compress pure_funcs=\"F2,F3,F4,F5,F6,F7,F8,F9,A2,A3,A4,A5,A6,A7,A8,A9\",pure_getters=true,keep_fargs=false,unsafe_comps=true,unsafe=true,passes=2 --output elm.js
    npx uglifyjs elm.js --mangle --output elm.js

    local target=$DEPLOY/elm-spa-example-js
    mkdir -p $target
    rm -rf $target/*

    cp -v index.html $target
    cp -v elm.js $target

    cd $target
    rm -f assets
    ln -s ../assets

    popd
}


function elm_spa_example_assets {
    rsync -ai demos/elm-spa-example/repo/assets/ "$DEPLOY/assets/"
}


function language_test {
    pushd demos/language-test

    make clean
    make wasm-release

    local target="$DEPLOY/language-test"
    rm -rf "$target/*"
    mkdir -p "$target/dist"
    mkdir -p "$target/assets"

    cp -v index.html $target
    cp -v assets/* "$target/assets"
    cp -v dist/* "$target/dist"

    popd
}


function unit_tests {
    make clean
    make wasm-release

    local target="$DEPLOY/unit-tests"
    rm -rf "$target/*"
    mkdir -p "$target/dist/wasm"
    cp -v index.html "$target"
    cp -v favicon.png "$target"
    cp -v dist/wasm/test.js "$target/dist/wasm"
    cp -v dist/wasm/test.wasm "$target/dist/wasm"
}


function index {
    cp -v demos/index.html $DEPLOY
    cp -v demos/favicon.png $DEPLOY
}

mkdir -p scripts/logs

index &
unit_tests &
language_test &
elm_spa_example_wasm &
elm_spa_example_js &
elm_spa_example_assets &


wait
