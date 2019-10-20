KERNELS := $(shell find ../../kernel -name '*.c')

.PHONY: all clean

all: src/*.js build/wrapper.js build/mock-app.wasm
	npx jasmine --config=jasmine.json --reporter=jasmine-console-reporter

clean:
	rm -f build/*

build/wrapper.js: ../wrapper.ts
	@rm -f $@
	npx tsc ../wrapper.ts --outFile build/wrapper.js
	echo 'module.exports = createElmWasmWrapper;' >> build/wrapper.js

build/mock-app.wasm: build-wasm.sh src/mock-app.c $(KERNELS) ../../test/gc/print-heap.c
	@rm -f $@
	@dos2unix -q build-wasm.sh
	bash build-wasm.sh
