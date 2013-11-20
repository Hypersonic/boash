OBJ_FILES = parse.o splitlines.o

all: parse.o splitlines.o
	gcc $(OBJ_FILES) -o boash

run: all
	./boash

clean:
	rm $(OBJ_FILES)
