.PHONY: all

SUBMODULE_DIR := "repo"
ELM_SOURCES := $(shell find $(SUBMODULE_DIR)/src -name '*.elm')
KERNELC := $(shell find ../../src/kernel -name '*.c')
KERNELH := $(shell find ../../src/kernel -name '*.h')

.PHONY: all clean

all: dist/elm.wasm dist/elm.js dist/style.css

clean:
	rm -rf build/* dist/*

build/elm/elm.c: $(ELM_SOURCES)
	@mkdir -p build/elm
	cd $(SUBMODULE_DIR); elm make src/Main.elm --output ../build/elm/elm.c

build/elm/elm.js: build/elm/elm.c
	@:

build/emscripten/elm.c: build/elm/elm.c codemods.js
	@mkdir -p build/emscripten
	node codemods.js build/elm/elm.c > build/emscripten/elm.c

build/emscripten/elm.wasm: build-wasm.sh build/emscripten/elm.c $(KERNELC) $(KERNELH)
	@dos2unix -q build-wasm.sh
	bash build-wasm.sh

build/emscripten/elm.js: build/emscripten/elm.wasm
	@:

dist/elm.wasm: build/emscripten/elm.wasm
	@mkdir -p dist
	cp build/emscripten/elm.wasm dist

dist/elm.js: build/emscripten/elm.js build/elm/elm.js
	cat build/emscripten/elm.js build/elm/elm.js > dist/elm.js

dist/style.css:
	cp repo/style.css dist/style.css

dist/test: $(KERNELC) $(KERNELH) build/emscripten/elm.c
	gcc -Wall -O0 -g $(KERNELC) build/emscripten/elm.c -o $@ -lm -Wno-unused-variable -Wno-incompatible-pointer-types
