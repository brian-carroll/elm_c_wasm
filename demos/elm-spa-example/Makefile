MAKEFLAGS := --jobs=$(shell nproc)
CFLAGS=-O1 -Wall -DDEBUG -Wno-unused-variable

SUBMODULE_DIR := "repo"
ELM_SOURCES := $(shell find $(SUBMODULE_DIR)/src -name '*.elm')
KERNELC := $(shell find ../../src/kernel -name '*.c') # only for detecting changes. See script.
KERNELH := $(shell find ../../src/kernel -name '*.h') # only for detecting changes. See script.
KERNELJS := $(shell find ../../src/kernel -name '*.js')
WRAPPER := ../../src/kernel/wrapper/wrapper

.PHONY: all clean

all: dist/elm.wasm dist/elm.js

clean:
	rm -rf build/* dist/* repo/elm-stuff/*

release: CFLAGS = -Wall -Wno-unused-variable -O3 -DPERF_TIMER_ENABLED=1
release: dist/elm.wasm dist/elm.js
	@:

js: $(ELM_SOURCES)
	rm -f dist/*
	cd $(SUBMODULE_DIR); elm make src/Main.elm --output elm.js
	@cp -v $(SUBMODULE_DIR)/elm.js dist

build/elm/elm.c: $(ELM_SOURCES)
	@mkdir -p build/elm
	cd $(SUBMODULE_DIR); elm make src/Main.elm --output ../build/elm/elm.c

build/elm/elm.js: build/elm/elm.c
	@:

build/codemods/elm.c: build/elm/elm.c codemods.js
	@mkdir -p build/codemods
	node codemods.js build/elm/elm.c > build/codemods/elm.c

build/emscripten/elm.wasm: build-wasm.sh build/codemods/elm.c $(KERNELC) $(KERNELH) $(KERNELJS)
	@dos2unix -q build-wasm.sh
	bash build-wasm.sh $(CFLAGS)

build/emscripten/elm.js: build/emscripten/elm.wasm
	@:

dist/elm.wasm: build/emscripten/elm.wasm
	@cp -v build/emscripten/elm.wasm dist

dist/elm.js: build/emscripten/elm.js dist/elm.wasm
	@cp -v build/emscripten/elm.js dist

$(WRAPPER).js: $(WRAPPER).ts
	../../node_modules/.bin/tsc $(WRAPPER).ts
