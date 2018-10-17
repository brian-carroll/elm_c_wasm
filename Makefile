CC=gcc
CFLAGS=-Wall -O1
SRCDIR=src

.PHONY: all check dist www clean resources

# 'all' = default with no arguments
all: check

check: resources ./dist/bin/test
	./dist/bin/test

verbose: resources ./dist/bin/test
	./dist/bin/test -v

dist: check www

www: resources ./dist/www/test.html


clean:
	rm -rf build/*
	rm -rf dist/*

resources:
	@mkdir -p build/bin/kernel build/bin/test
	@mkdir -p build/www/kernel build/www/test
	@mkdir -p dist/bin dist/www


# Makefile squiggles:
#  @CMD  run CMD but don't print it
#  $@    target filename
#  $^    all prerequisite names
#  $<    first prerequisite name
#
# https://www.gnu.org/software/make/manual/html_node/Automatic-Variables.html

SOURCES := $(wildcard src/kernel/*.c)
TEST_SRC := $(wildcard src/test/*.c)
HEADERS := $(wildcard src/kernel/*.h)

BIN_OBJ := $(patsubst src/kernel/%.c, build/bin/kernel/%.o, $(SOURCES)) \
			$(patsubst src/test/%.c, build/bin/test/%.o, $(TEST_SRC))

WWW_OBJ := $(patsubst build/bin/%, build/www/%, $(BIN_OBJ))

# Object files

build/bin/kernel/%.o: src/kernel/%.c src/kernel/%.h
	$(CC) $(CFLAGS) -c $< -o $@

build/www/kernel/%.o: src/kernel/%.c src/kernel/%.h
	emcc $(CFLAGS) -c $< -o $@


build/bin/test/%.o: src/test/%.c src/test/%.h
	$(CC) $(CFLAGS) -c $< -o $@

build/www/test/%.o: src/test/%.c src/test/%.h
	emcc $(CFLAGS) -c $< -o $@


# Binary & Wasm

./dist/bin/test: $(BIN_OBJ)
	$(CC) $(CFLAGS) $^ -o $@

./dist/www/test.html: $(WWW_OBJ)
	emcc $(CFLAGS) -s WASM=1 $^ -o $@
