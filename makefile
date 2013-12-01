OBJ_FILES = parser.o tabcomp.o main.o

all: $(OBJ_FILES)
	gcc $(OBJ_FILES) -o boash.out

parser.o: parser.c
	gcc -c parser.c

tabcomp.o: tabcomp.c
	gcc -c tabcomp.c

main.o: main.c
	gcc -c main.c

run: all
	./boash.out

clean:
	rm $(OBJ_FILES)
