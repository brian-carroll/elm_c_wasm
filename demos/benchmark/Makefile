.PHONY: all

KERNELS := $(shell find ../../src/kernel -name '*.c')

.PHONY: all clean

all: dist/bundle.js

clean:
	rm -f build/* dist/*

dist/bundle.js: build-combine.sh build/elm.js build/benchmark.wasm src/*.js
	dos2unix -q build-combine.sh
	bash build-combine.sh

build/elm.js: src/Main.elm
	elm make src/Main.elm --output build/elm.js

build/benchmark.wasm: build-wasm.sh $(KERNELS) src/benchmark.c ../../src/test/gc/print-heap.c
	dos2unix -q build-wasm.sh
	bash build-wasm.sh
