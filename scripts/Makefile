CC=gcc
CFLAGS=-Wall -O1 -DDEBUG

ROOT := ..
SRC := $(ROOT)/src
DIST := $(ROOT)/dist
DEPLOY := $(ROOT)/../gh-pages/unit-tests
SOURCES := $(shell find $(SRC) -name '*.c')
HEADERS := $(shell find $(SRC) -name '*.h')

.PHONY: all check dist www gc-size clean watch

# 'all' = default for `make` with no arguments
all: $(DIST)/bin/test

check: $(DIST)/bin/test
	$(DIST)/bin/test

verbose: $(DIST)/bin/test
	$(DIST)/bin/test -v

dist: check www

www: $(DIST)/www/test.html

gc-size:
	emcc -Wall -O3 -DGC_SIZE_CHECK -s WASM=1 $(SRC)/kernel/gc*.c $(SRC)/kernel/types.c -o $(DIST)/www/gc.js
	ls -lh $(DIST)/www/gc.wasm

clean:
	find $(DIST) -type f ! -name '.gitkeep' -exec rm {} \;

watch:
	(while true ; do make build.log; sleep 1; done) | grep -v make


build.log: $(SOURCES) $(HEADERS)
	make check | tee build.log

gh-pages: www
	mkdir -p $(DEPLOY)/dist/www
	cp $(ROOT)/index.html $(DEPLOY)
	cp $(DIST)/www/test.js $(DEPLOY)/dist/www
	cp $(DIST)/www/test.wasm $(DEPLOY)/dist/www


# Makefile squiggles:
#  @CMD  run CMD but don't print it
#  $@    target filename
#  $^    all prerequisite names
#  $<    first prerequisite name
#
# https://www.gnu.org/software/make/manual/html_node/Automatic-Variables.html


# Binary & Wasm

$(DIST)/bin/test: $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) $(SOURCES) -o $@ -lm

$(DIST)/www/test.html: $(SOURCES) $(HEADERS)
	emcc $(CFLAGS) -s ASSERTIONS=1 $(SOURCES) -o $@
