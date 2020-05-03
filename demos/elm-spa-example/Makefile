.PHONY: all

SUBMODULE_DIR := "repo"
ELM_SOURCES := $(shell find $(SUBMODULE_DIR)/src -name '*.elm')
KERNELC := $(shell find ../../src/kernel -name '*.c')
KERNELH := $(shell find ../../src/kernel -name '*.h')

# I know, I know, sorry...
ELM_TO_C := $(HOME)/Code/wasm/compiler/.stack-work/install/x86_64-linux/lts-13.28/8.6.5/bin/elm

.PHONY: all clean

all: dist/elm.wasm dist/elm.js

clean:
	rm -rf build/* dist/*

build/elm/elm.c: $(ELM_SOURCES)
	@mkdir -p build/elm
	cd $(SUBMODULE_DIR); $(ELM_TO_C) make src/Main.elm --output ../build/elm/elm.c

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

dist/test: $(KERNELC) $(KERNELH) build/emscripten/elm.c
	gcc -Wall -O0 -g $(KERNELC) build/emscripten/elm.c -o $@ -lm -Wno-unused-variable -Wno-incompatible-pointer-types
