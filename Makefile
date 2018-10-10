CC=gcc
CFLAGS=-Wall -O1
SRCDIR=src
TESTDIR=tests
BUILDDIR=build
TARGETDIR=bin

.PHONY: all check clean www resources


all: check www


clean:
	rm -rf build/*
	rm -f bin/*


check: resources ./bin/types_test ./bin/utils_test
	./bin/types_test
	./bin/utils_test


resources:
	mkdir -p build/bin/kernel build/bin/test
	mkdir -p build/www/kernel build/www/test

# Makefile squiggles:
#  $@   target filename
#  $^   all prerequisite names
#  $<   first prerequisite name
#
# https://www.gnu.org/software/make/manual/html_node/Automatic-Variables.html


# Compile object files

$(BUILDDIR)/bin/kernel/%.o: $(SRCDIR)/kernel/%.c $(SRCDIR)/kernel/%.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/www/kernel/%.o: $(SRCDIR)/kernel/%.c $(SRCDIR)/kernel/%.h
	emcc $(CFLAGS) -c $< -o $@


$(BUILDDIR)/bin/test/%.o: $(SRCDIR)/test/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/www/test/%.o: $(SRCDIR)/test/%.c
	emcc $(CFLAGS) -c $< -o $@


# Compile and link binaries

./bin/utils_test: $(BUILDDIR)/bin/test/utils_test.o $(BUILDDIR)/bin/kernel/utils.o $(BUILDDIR)/bin/kernel/types.o $(BUILDDIR)/bin/kernel/basics.o
	$(CC) $(CFLAGS) $^ -o $@

./bin/types_test: $(BUILDDIR)/bin/test/types_test.o $(BUILDDIR)/bin/kernel/types.o
	$(CC) $(CFLAGS) $^ -o $@


# Compile and link Wasm

www: ./www/utils_test.html ./www/types_test.html

./www/utils_test.html: $(BUILDDIR)/www/test/utils_test.o $(BUILDDIR)/www/kernel/utils.o $(BUILDDIR)/www/kernel/types.o $(BUILDDIR)/www/kernel/basics.o
	emcc $(CFLAGS) -s WASM=1 $^ -o $@

./www/types_test.html: $(BUILDDIR)/www/test/types_test.o $(BUILDDIR)/www/kernel/types.o
	emcc $(CFLAGS) -s WASM=1 $^ -o $@
