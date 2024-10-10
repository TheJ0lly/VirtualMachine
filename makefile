SRC := $(filter-out src/tests.c, $(wildcard src/*.c))
TEST_SRC := $(filter-out src/main.c, $(wildcard src/*.c))

help:
	@echo "Usage: make <compile/run>" 

compile: $(SRC)
	gcc -o bin/vm $(SRC) -Wall -Wswitch -Werror -pedantic -std=c11

run: compile
	./bin/vm

test: $(TEST_SRC)
	gcc -o bin/vm_test $(TEST_SRC) -Wall -Wswitch -Werror -pedantic -std=c11
	./bin/vm_test
	rm bin/vm_test
