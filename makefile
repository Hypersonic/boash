
all: parseline splitlines
	gcc parse.c splitlines.c -o boash

parseline: parse.c
	gcc -c parse.c

splitlines: splitlines.c
	gcc -c splitlines.c

run: all
	./boash
