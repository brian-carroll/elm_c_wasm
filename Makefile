
CC=gcc
CFLAGS=-Wall -O1
SRCDIR=src
BUILDDIR=build
TARGETDIR=bin

.PHONY: all check clean


all: check


clean:
	rm -f build/*
	rm -f bin/*


check: ./bin/types_test ./bin/utils_test
	./bin/types_test
	./bin/utils_test


# Makefile squiggles:
#  $@   target filename
#  $^   all prerequisite names
#  $<   first prerequisite name
#
# https://www.gnu.org/software/make/manual/html_node/Automatic-Variables.html


# Compile object files

$(BUILDDIR)/%.o: $(SRCDIR)/%.c $(SRCDIR)/%.h
	$(CC) $(CFLAGS) -c -o $@ $<


# Compile and link binaries

./bin/utils_test: tests/utils_test.c build/utils.o build/types.o build/basics.o
	$(CC) $(CFLAGS) -o $@ $^

./bin/types_test: tests/types_test.c build/types.o
	$(CC) $(CFLAGS) -o $@ $^
