
all: parse.o splitlines.o
	gcc parse.o splitlines.o -o boash

run: all
	./boash
