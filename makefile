OBJ_FILES = parser.o tabcomp.o main.o

all: $(OBJ_FILES)
	gcc $(OBJ_FILES) -o boash.out

run: all
	./boash.out

clean:
	rm $(OBJ_FILES)
