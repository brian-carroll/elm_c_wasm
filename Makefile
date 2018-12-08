CC=gcc
CFLAGS=-Wall -O1

SOURCES := $(wildcard src/*/*.c)
HEADERS := $(wildcard src/*/*.h)


.PHONY: all check dist www clean watch

# 'all' = default for `make` with no arguments
all: ./dist/bin/test

check: ./dist/bin/test
	./dist/bin/test

verbose: ./dist/bin/test
	./dist/bin/test -v

dist: check www

www: ./dist/www/test.js

clean:
	find dist -type f ! -name '.gitkeep' -exec rm {} \;

watch:
	(while true ; do make build.log; sleep 1; done) | grep -v make


build.log: $(SOURCES) $(HEADERS)
	make check | tee build.log


# Makefile squiggles:
#  @CMD  run CMD but don't print it
#  $@    target filename
#  $^    all prerequisite names
#  $<    first prerequisite name
#
# https://www.gnu.org/software/make/manual/html_node/Automatic-Variables.html


# Binary & Wasm

./dist/bin/test: $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) $(SOURCES) -o $@ -lm

./dist/www/test.js: $(SOURCES) $(HEADERS)
	emcc $(CFLAGS) -s WASM=1 $(SOURCES) -o $@
