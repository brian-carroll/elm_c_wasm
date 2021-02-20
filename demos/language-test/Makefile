.PHONY: all

ELM_SOURCES := $(shell find ./src -name '*.elm')
KERNEL := ../../src/kernel
ENTRY := $(KERNEL)/core/core.c $(KERNEL)/elm-test/elm-test.c $(KERNEL)/json/json.c $(KERNEL)/wrapper/wrapper.c
KERNELC := $(shell find ../../src/kernel -name '*.c')
KERNELH := $(shell find ../../src/kernel -name '*.h')
WRAPPER := ../../src/kernel/wrapper/wrapper

# I know, I know, sorry...
ELM_TO_C := $(HOME)/Code/wasm/compiler/.stack-work/install/x86_64-linux/lts-13.28/8.6.5/bin/elm

.PHONY: all clean

all: dist/elm.wasm dist/elm.js

clean:
	rm -rf build/* dist/* elm-stuff/*

check: dist/test
	dist/test

build/elm/elm.c: $(ELM_SOURCES)
	@mkdir -p build/elm
	$(ELM_TO_C) make src/Main.elm --output build/elm/elm.c

build/elm/elm.js: build/elm/elm.c
	@:

build/codemods/elm.c: build/elm/elm.c codemods.js
	@mkdir -p build/codemods
	node codemods.js build/elm/elm.c > build/codemods/elm.c

dist/elm.wasm: build-wasm.sh build/codemods/elm.c src/print-virtual-dom.c $(KERNELC) $(KERNELH) $(WRAPPER).js
	@dos2unix -q build-wasm.sh
	bash build-wasm.sh

dist/elm.js: dist/elm.wasm
	@:

$(WRAPPER).js: $(WRAPPER).ts
	../../node_modules/.bin/tsc $(WRAPPER).ts

dist/test: $(KERNELC) $(KERNELH) build/codemods/elm.c ../../src/test/test-imports.c
	gcc -Wall -O0 -DDEBUG -g $(ENTRY) build/codemods/elm.c ../../src/test/test-imports.c -o $@ -lm -Wno-unused-variable -Wno-incompatible-pointer-types
