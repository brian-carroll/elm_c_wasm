.PHONY: check
check: ./dist/tests/utils_test ./dist/tests/types_test
	./dist/tests/utils_test
	./dist/tests/types_test

./dist/tests/utils_test: tests/utils_test.c build/utils.o build/types.o build/basics.o
	gcc -Wall tests/utils_test.c build/utils.o build/types.o build/basics.o -o ./dist/tests/utils_test

./dist/tests/types_test: tests/types_test.c build/types.o
	gcc -Wall tests/types_test.c build/types.o -o ./dist/tests/types_test

build/types.o: src/types.*
	gcc -Wall -c src/types.c -o build/types.o

build/basics.o: src/basics.*
	gcc -Wall -c src/basics.c -o build/basics.o

build/utils.o: src/utils.*
	gcc -Wall -c src/utils.c -o build/utils.o


.PHONY: clean
clean:
	rm -f build/*
	rm -f dist/tests/*
