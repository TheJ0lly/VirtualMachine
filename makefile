default:
	@echo "Usage: make <compile/run>" 

compile:
	gcc -o bin/vm src/*.c -Wall -Wswitch -Werror -pedantic -std=c11

run: compile
	./bin/vm
