MAKEFLAGS := --jobs=$(shell nproc)
CFLAGS=-Wall -DDEBUG -DTEST
ARGS=-a

ROOT := .
SRC := $(ROOT)/src
KERNEL := $(ROOT)/src/kernel
TEST := $(ROOT)/src/test

BUILD := $(ROOT)/build
DIST := $(ROOT)/dist
DEPLOY := $(ROOT)/../gh-pages

OBJ := core.o elm-test.o json.o virtual-dom.o wrapper.o test.o
BIN_OBJ := $(patsubst %,build/bin/%,$(OBJ))
WASM_OBJ := $(patsubst %,build/wasm/%,$(OBJ))

.PHONY: all check check-bin check-wasm wasm gc-size clean watch build.log benchmark wrapper codegen todo gh-pages

# 'all' = default for `make` with no arguments
all: $(DIST)/bin/test
	@:

check: check-bin check-wasm
	@:

check-bin: $(DIST)/bin/test
	@echo "\n\nRunning tests with binary executable\n"
	$(DIST)/bin/test $(ARGS)

check-wasm: $(DIST)/wasm/test.js
	@echo "\n\nRunning tests with Node.js WebAssembly\n"
	node $(DIST)/wasm/test.js $(ARGS)

wasm: $(DIST)/wasm/test.js
	@:

wasm-release: CFLAGS = -Wall -O3 -DTEST -DPERF_TIMER_ENABLED=1
wasm-release: wasm
	@:

release: CFLAGS = -Wall -O3 -DTEST -DPERF_TIMER_ENABLED=1
release: $(DIST)/bin/test
	@:

gc-size:
	emcc -Wall -O3 -DGC_SIZE_CHECK -s WASM=1 $(SRC)/kernel/gc*.c $(SRC)/kernel/types.c -o $(DIST)/wasm/gc.js
	/bin/ls -lh $(DIST)/wasm/gc.wasm

clean:
	@mkdir -p $(BUILD)/bin $(BUILD)/wasm $(BUILD)/exe
	@mkdir -p $(DIST)/bin $(DIST)/wasm $(DIST)/exe
	@echo 'Deleting generated files'
	@find $(DIST) -type f ! -exec rm {} \;
	@find $(BUILD) -type f ! -exec rm {} \;
	@rm -f $(KERNEL)/wrapper/wrapper.js

gh-pages: CFLAGS=-Wall -O3
gh-pages: wasm
	mkdir -p $(DEPLOY)/unit-tests/dist/wasm
	cp $(ROOT)/index.html $(DEPLOY)/unit-tests/
	cp $(ROOT)/favicon.png $(DEPLOY)/unit-tests/
	cp $(DIST)/wasm/test.js $(DEPLOY)/unit-tests/dist/wasm
	cp $(DIST)/wasm/test.wasm $(DEPLOY)/unit-tests/dist/wasm
	cp $(ROOT)/demos/index.html $(DEPLOY)
	cp $(ROOT)/demos/favicon.png $(DEPLOY)
	cp $(ROOT)/demos/elm-spa-example/index.html $(DEPLOY)/elm-spa-example-wasm/index.html
	cp $(ROOT)/demos/elm-spa-example/dist/* $(DEPLOY)/elm-spa-example-wasm/dist/
	cp $(ROOT)/demos/elm-spa-example/index.html $(DEPLOY)/elm-spa-example-wasm/index.html
	cp $(ROOT)/demos/elm-spa-example/dist/* $(DEPLOY)/elm-spa-example-wasm/dist/
	cd $(DEPLOY) && git add . && git commit -m 'update' && git push


# Makefile squiggles:
#  @CMD  run CMD but don't print it
#  $@    target filename
#  $^    all prerequisite names
#  $<    first prerequisite name
#
# https://www.gnu.org/software/make/manual/html_node/Automatic-Variables.html
# https://www.gnu.org/software/make/manual/html_node/Quick-Reference.html

#----------------------------------------------
# Linux binary
#----------------------------------------------

$(BUILD)/bin/core.o: $(KERNEL)/core/*.c $(KERNEL)/core/*.h $(KERNEL)/core/*/*.c $(KERNEL)/core/*/*.h
	gcc -ggdb $(CFLAGS) -c $(KERNEL)/core/core.c -o $@

$(BUILD)/bin/elm-test.o: $(KERNEL)/elm-test/*.c
	gcc -ggdb $(CFLAGS) -c $(KERNEL)/elm-test/elm-test.c -o $@

$(BUILD)/bin/json.o: $(KERNEL)/json/*.c $(KERNEL)/json/*.h
	gcc -ggdb $(CFLAGS) -c $(KERNEL)/json/json.c -o $@

$(BUILD)/bin/virtual-dom.o: $(KERNEL)/virtual-dom/*.c $(KERNEL)/virtual-dom/*.h
	gcc -ggdb $(CFLAGS) -c $(KERNEL)/virtual-dom/virtual-dom.c -o $@

$(BUILD)/bin/wrapper.o: $(KERNEL)/wrapper/*.c $(KERNEL)/wrapper/*.h
	gcc -ggdb $(CFLAGS) -c $(KERNEL)/wrapper/wrapper.c -o $@

$(BUILD)/bin/test.o: $(TEST)/*.c $(TEST)/*.h $(TEST)/json/*.c $(TEST)/gc/*.c
	gcc -ggdb $(CFLAGS) -c $(TEST)/test.c -o $@


$(DIST)/bin/test: $(BIN_OBJ)
	@mkdir -p $(DIST)/bin
	gcc -ggdb $(CFLAGS) $^ -o $@ -lm


#----------------------------------------------
# WebAssembly module
#----------------------------------------------

$(BUILD)/wasm/core.o: $(KERNEL)/core/*.c $(KERNEL)/core/*.h $(KERNEL)/core/*/*.c $(KERNEL)/core/*/*.h
	emcc $(CFLAGS) -c $(KERNEL)/core/core.c -o $@

$(BUILD)/wasm/elm-test.o: $(KERNEL)/elm-test/*.c
	emcc $(CFLAGS) -c $(KERNEL)/elm-test/elm-test.c -o $@

$(BUILD)/wasm/json.o: $(KERNEL)/json/*.c $(KERNEL)/json/*.h
	emcc $(CFLAGS) -c $(KERNEL)/json/json.c -o $@

$(BUILD)/wasm/virtual-dom.o: $(KERNEL)/virtual-dom/*.c $(KERNEL)/virtual-dom/*.h
	emcc $(CFLAGS) -c $(KERNEL)/virtual-dom/virtual-dom.c -o $@

$(BUILD)/wasm/wrapper.o: $(KERNEL)/wrapper/*.c $(KERNEL)/wrapper/*.h
	emcc $(CFLAGS) -c $(KERNEL)/wrapper/wrapper.c -o $@

$(BUILD)/wasm/test.o: $(TEST)/*.c $(TEST)/*.h $(TEST)/*/*.c
	emcc $(CFLAGS) -c $(TEST)/test.c -o $@


$(KERNEL)/wrapper/wrapper.js: $(KERNEL)/wrapper/wrapper.ts
	npx tsc $<

$(KERNEL)/virtual-dom/virtual-dom.js: $(KERNEL)/virtual-dom/virtual-dom.ts
	npx tsc $<

$(DIST)/wasm/test.js: $(WASM_OBJ) $(KERNEL)/wrapper/wrapper.js $(KERNEL)/virtual-dom/virtual-dom.js $(KERNEL)/wrapper/imports.js $(TEST)/test-imports.js
	@mkdir -p $(DIST)/wasm
	emcc $(CFLAGS) $(WASM_OBJ) -ferror-limit=0 -s NO_EXIT_RUNTIME=0 \
		--pre-js $(TEST)/test-emscripten-config.js \
		--pre-js $(KERNEL)/wrapper/wrapper.js \
		--pre-js $(KERNEL)/virtual-dom/virtual-dom.js \
		--js-library $(KERNEL)/wrapper/imports.js \
		--js-library $(TEST)/test-imports.js \
		-o $@


# handle any other arguments to 'make' by passing them to the executable
# 'make gv' compiles the 'test' executable and runs 'test -gv'
%::
	@make $(DIST)/bin/test --no-print-directory
	$(DIST)/bin/test -$@

