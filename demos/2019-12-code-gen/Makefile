.PHONY: all

KERNELS := $(shell find ../../src/kernel -name '*.c')

.PHONY: all clean

all: dist/elm.wasm

clean:
	rm -rf build/* dist/*

build/elm/elm.c: src/Main.elm
	@mkdir -p build/elm
	elm make src/Main.elm --output build/elm/elm.c

build/emscripten/elm.wasm: build-wasm.sh $(KERNELS) build/elm/elm.c
	@dos2unix -q build-wasm.sh
	bash build-wasm.sh

dist/elm.wasm: build/emscripten/elm.wasm build/elm/elm.c
	@mkdir -p build/emscripten
	cp build/emscripten/elm.wasm dist
	cat build/emscripten/elm.js build/elm/elm.js > dist/elm.js
