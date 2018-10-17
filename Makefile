CC=gcc
CFLAGS=-Wall -O1

SOURCES := $(wildcard src/*/*.c)

BIN_OBJ := $(patsubst src/%.c, build/bin/%.o, $(SOURCES))
WWW_OBJ := $(patsubst src/%.c, build/www/%.o, $(SOURCES))

DEPS := $(patsubst src/%.c, build/deps/%.d, $(SOURCES))


.PHONY: all check dist www clean vars

# 'all' = default for `make` with no arguments
all: check

check: ./dist/bin/test
	./dist/bin/test

verbose: ./dist/bin/test
	./dist/bin/test -v

dist: check www

www: ./dist/www/test.html

clean:
	rm -f $(BIN_OBJ) $(WWW_OBJ) $(DEPS)
	find dist -type f ! -name '.gitkeep' -exec rm {} \;


# Makefile squiggles:
#  @CMD  run CMD but don't print it
#  $@    target filename
#  $^    all prerequisite names
#  $<    first prerequisite name
#
# https://www.gnu.org/software/make/manual/html_node/Automatic-Variables.html


# Binary & Wasm

./dist/bin/test: $(BIN_OBJ)
	$(CC) $(CFLAGS) $^ -o $@

./dist/www/test.html: $(SOURCES) $(TEST_SOURCES)
	emcc $(CFLAGS) -s WASM=1 $^ -o $@



# Object files

build/bin/kernel/%.o: src/kernel/%.c
	$(CC) $(CFLAGS) -c $< -o $@

build/www/kernel/%.o: src/kernel/%.c
	emcc $(CFLAGS) -c $< -o $@


build/bin/test/%.o: src/test/%.c
	$(CC) $(CFLAGS) -c $< -o $@

build/www/test/%.o: src/test/%.c
	emcc $(CFLAGS) -c $< -o $@


# Dependencies

# rule to generate a dep file by using the C preprocessor
# (see man cpp for details on the -MM and -MT options)
build/deps/kernel/%.d: src/kernel/%.c
	@$(CPP) $(CFLAGS) $< -MM -MT $(@:.d=.o) >$@

build/deps/test/%.d: src/test/%.c
	@$(CPP) $(CFLAGS) $< -MM -MT $(@:.d=.o) >$@


-include $(DEPS)
