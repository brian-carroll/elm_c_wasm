.PHONY: all

ELM_SOURCES := $(shell find ./src -name '*.elm')
KERNELC := $(shell find ../../src/kernel -name '*.c')
KERNELH := $(shell find ../../src/kernel -name '*.h')
KERNELJS := $(shell find ../../src/kernel -name '*.js')
WRAPPER := ../../src/kernel/wrapper/wrapper

.PHONY: all clean

all: dist/elm.wasm dist/elm.js

clean:
	rm -rf build/* dist/* elm-stuff/*

build/elm/elm.c: $(ELM_SOURCES)
	@mkdir -p build/elm
	elm make src/Main.elm --output build/elm/elm.c

build/elm/elm.js: build/elm/elm.c
	@:

build/codemods/elm.c: build/elm/elm.c codemods.js
	@mkdir -p build/codemods
	node codemods.js build/elm/elm.c > build/codemods/elm.c

dist/elm.wasm: build-wasm.sh build/codemods/elm.c $(KERNELC) $(KERNELH) $(KERNELJS)
	@dos2unix -q build-wasm.sh
	bash build-wasm.sh

dist/elm.js: dist/elm.wasm
	@:

$(WRAPPER).js: $(WRAPPER).ts
	../../node_modules/.bin/tsc $(WRAPPER).ts

dist/test: $(KERNELC) $(KERNELH) build/codemods/elm.c
	gcc -Wall -O0 -g $(KERNELC) build/codemods/elm.c -o $@ -lm -Wno-unused-variable -Wno-incompatible-pointer-types
