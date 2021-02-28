.PHONY: all

CORE_DEPS := $(shell find ../src/kernel/core -name '*.c' -o -name '*.h')
JSON_DEPS := $(shell find ../src/kernel/json -name '*.c' -o -name '*.h')
TEST_DEPS := ../src/test/test-compiled.c ../src/test/test-imports.c

all: vdom-alloc.c $(CORE_DEPS) $(JSON_DEPS) $(TEST_DEPS)
	gcc vdom-alloc.c ../src/kernel/core/core.c ../src/kernel/json/json.c $(TEST_DEPS) -o vdom-alloc -lm -Wall -ggdb
	./vdom-alloc
