.PHONY: all

KERNELS := $(shell find ../../src/kernel -name '*.c')

# I know, I know, sorry...
ELM_TO_C := $(HOME)/Code/wasm/compiler/.stack-work/install/x86_64-linux/lts-13.28/8.6.5/bin/elm

.PHONY: all clean

all: dist/elm.wasm

clean:
	rm -rf build/* dist/*

build/elm/elm.c: src/Main.elm
	@mkdir -p build/elm
	$(ELM_TO_C) make src/Main.elm --output build/elm/elm.c

build/emscripten/elm.wasm: build-wasm.sh $(KERNELS) build/elm/elm.c
	@dos2unix -q build-wasm.sh
	bash build-wasm.sh

dist/elm.wasm: build/emscripten/elm.wasm build/elm/elm.c
	@mkdir -p build/emscripten
	cp build/emscripten/elm.wasm dist
	cat build/emscripten/elm.js build/elm/elm.js > dist/elm.js
