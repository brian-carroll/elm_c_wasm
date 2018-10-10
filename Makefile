CC=gcc
CFLAGS=-Wall -O1
SRCDIR=src
BUILDDIR=build
TARGETDIR=dist

.PHONY: all clean check www resources


all: check www

clean:
	rm -rf build/*
	rm -rf dist/*

check: resources ./dist/bin/test
	./dist/bin/test

www: resources ./dist/www/test.html

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


# Object files

$(BUILDDIR)/bin/kernel/%.o: $(SRCDIR)/kernel/%.c $(SRCDIR)/kernel/%.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/www/kernel/%.o: $(SRCDIR)/kernel/%.c $(SRCDIR)/kernel/%.h
	emcc $(CFLAGS) -c $< -o $@


$(BUILDDIR)/bin/test/%.o: $(SRCDIR)/test/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/www/test/%.o: $(SRCDIR)/test/%.c
	emcc $(CFLAGS) -c $< -o $@


# Binary & Wasm

./dist/bin/test: $(BUILDDIR)/bin/test/test.o $(BUILDDIR)/bin/kernel/utils.o $(BUILDDIR)/bin/kernel/types.o $(BUILDDIR)/bin/kernel/basics.o
	$(CC) $(CFLAGS) $^ -o $@

./dist/www/test.html: $(BUILDDIR)/www/test/test.o $(BUILDDIR)/www/kernel/utils.o $(BUILDDIR)/www/kernel/types.o $(BUILDDIR)/www/kernel/basics.o
	emcc $(CFLAGS) -s WASM=1 $^ -o $@
