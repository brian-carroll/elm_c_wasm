CC=gcc
CFLAGS=-Wall -O1 -Wno-discarded-qualifiers

ROOT := ..
SRC := $(ROOT)/src
DIST := $(ROOT)/dist
DEPLOY := $(ROOT)/../gh-pages
SOURCES := $(shell find $(SRC) -name '*.c')
HEADERS := $(shell find $(SRC) -name '*.h')
DATA_TSV := $(shell find $(SRC) -name '*.tsv')
DATA_INC := $(DATA_TSV:.tsv=.inc)

.PHONY: all check dist www gc-size clean watch

# 'all' = default for `make` with no arguments
all: $(DIST)/bin/test

check: $(DIST)/bin/test
	$(DIST)/bin/test -a

debug: CFLAGS += -DDEBUG
debug: clean all
	@:

release: clean all
	@:

verbose: $(DIST)/bin/test
	$(DIST)/bin/test -v

dist: clean check www
	@:

www: $(DIST)/www/test.html
	@:

www-debug: CFLAGS += -DDEBUG
www-debug: clean www
	@:

www-release: clean www
	@:

gc-size:
	emcc -Wall -O3 -DGC_SIZE_CHECK -s WASM=1 $(SRC)/kernel/gc*.c $(SRC)/kernel/types.c -o $(DIST)/www/gc.js
	ls -lh $(DIST)/www/gc.wasm

clean:
	@echo 'Deleting generated files'
	@find $(DIST) -type f ! -name '.gitkeep' -exec rm {} \;
	@rm -f $(DATA_INC)

watch:
	(while true ; do make build.log; sleep 1; done) | grep -v make


build.log: $(SOURCES) $(HEADERS)
	make gh-pages | tee build.log

gh-pages: CFLAGS=-Wall -O3
gh-pages: clean www
	mkdir -p $(DEPLOY)/dist/www
	cp $(ROOT)/demos/index.html $(DEPLOY)
	cp $(ROOT)/demos/favicon.png $(DEPLOY)
	cp $(ROOT)/index.html $(DEPLOY)/unit-tests/
	cp $(ROOT)/favicon.png $(DEPLOY)/unit-tests/
	cp $(DIST)/www/test.js $(DEPLOY)/unit-tests/dist/www
	cp $(DIST)/www/test.wasm $(DEPLOY)/unit-tests/dist/www


# Makefile squiggles:
#  @CMD  run CMD but don't print it
#  $@    target filename
#  $^    all prerequisite names
#  $<    first prerequisite name
#
# https://www.gnu.org/software/make/manual/html_node/Automatic-Variables.html
# https://www.gnu.org/software/make/manual/html_node/Quick-Reference.html


# need to first find the tsv, then replace names with .tsv.inc, then make these the deps
$(SRC)/test/gc/stackmap_test.c: $(DATA_INC)
	@:

# ../src/test/gc/stackmap_data/full_completion.tsv.inc: ../src/test/gc/stackmap_data/full_completion.tsv
$(SRC)/%.inc : $(SRC)/%.tsv
	@cd $$(dirname $@) ; xxd -i $$(basename $<) $$(basename $@)

# Binary & Wasm

$(DIST)/bin/test: $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) $(SOURCES) -o $@ -lm

$(DIST)/www/test.html: $(SOURCES) $(HEADERS)
	emcc $(CFLAGS) $(SOURCES) -o $@

# handle any other arguments to 'make' by passing them to the executable
# 'make gv' compiles the 'test' executable and runs 'test -gv'
%::
	@make --no-print-directory
	$(DIST)/bin/test -$@

