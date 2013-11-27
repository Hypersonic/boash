#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct {
    char** results;
    int curr;
    int num_results;
} tcomp_results;

typedef struct {
    char** results;
    int num_results;
} ls_results;

// Take the partial command, an array of paths to look in, the length of that array, and the permissions to match
tcomp_results* genResults(char*, char**, int, int);

// Find how similar s1 is to s2, higher is better
int similarity(char*, char*);

// list files in a directory
ls_results* ls(char*);

// Sort a tcomp_results in place, by similarity to comp
void sort_results(tcomp_results*, char*);
