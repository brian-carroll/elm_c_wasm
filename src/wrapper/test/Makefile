KERNELS := $(shell find ../../kernel -name '*.c')

.PHONY: all clean

all: src/*.js build/wrapper.js build/mock-app.wasm
	npx jasmine --config=jasmine.json

clean:
	rm -f build/* dist/*

build/wrapper.js: ../wrapper.ts
	@rm -f $@
	npx tsc ../wrapper.ts --outFile build/wrapper.js
	echo 'module.exports = createElmWasmWrapper;' >> build/wrapper.js

build/mock-app.wasm: build-wasm.sh src/mock-app.c $(KERNELS)
	@rm -f $@
	@dos2unix -q build-wasm.sh
	bash build-wasm.sh
