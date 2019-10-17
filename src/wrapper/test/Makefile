KERNELS := $(shell find ../../kernel -name '*.c')

.PHONY: all clean

all: src/*.js build/wrapper.js build/mock-app.wasm
	npx jest src/wrapper.spec.js

clean:
	rm -f build/* dist/*

build/wrapper.js: ../wrapper.ts
	npx tsc ../wrapper.ts --outFile build/wrapper.js
	echo 'module.exports = createElmWasmWrapper;' >> build/wrapper.js

build/mock-app.wasm: build-wasm.sh src/mock-app.c $(KERNELS)
	@dos2unix -q build-wasm.sh
	bash build-wasm.sh
