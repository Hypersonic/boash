#include "tabcomp.h"

tcomp_results* genResults(char* cmd_part, char** PATH, int PATHLEN, int perms) {
    int i;
    int j;
    tcomp_results* results = malloc(sizeof(tcomp_results));
    int size = sizeof(PATH);
    results->num_results = 0;
    results->results = malloc(1024); // no more results than this
    // Gen all possible commands
    for (i = 0; i < PATHLEN; ++i) {
        ls_results* pos_cmds = ls(PATH[i]);
        for (j = 0; j < pos_cmds->num_results; ++j) {
            char* cmd = pos_cmds->results[j];
            if (strcasestr(cmd, cmd_part) != NULL && cmd[0] != '.') { // ignore case and anything starting with .
                results->results[results->num_results] = cmd;
                results->num_results++;
            }
        }
    }
    sort_results(results, cmd_part);
    return results;
}

// Quicksort
void sort_results(tcomp_results* results, char* comp) {
    // base case, 0 or 1 results
    if (results->num_results < 2) {
        return;
    }
    int halfway = results->num_results/2;
    tcomp_results* lower = malloc(sizeof(tcomp_results));
    tcomp_results* upper = malloc(sizeof(tcomp_results));

    lower->num_results = halfway;
    upper->num_results = results->num_results - halfway;

    lower->results = malloc(lower->num_results);
    upper->results = malloc(upper->num_results);

    lower->curr = 0;
    upper->curr = 0;

    int i;
    for (i = 0; i < halfway; ++i) {
        if (similarity(results->results[i], comp) > similarity(results->results[halfway+i], comp)) {
            lower->results[lower->curr] = results->results[halfway+i];
            upper->results[upper->curr] = results->results[i];
        } else {
            lower->results[lower->curr] = results->results[i];
            upper->results[upper->curr] = results->results[halfway+i];
        }
        lower->curr++;
        upper->curr++;
    }
    if (halfway != results->num_results - halfway) {
        upper->results[upper->curr] = results->results[results->num_results - 1];
    } 
    for (i = 0; i < lower->num_results; ++i) {
        sort_results(lower, comp);
    }
    for (i = 0; i < upper->num_results; ++i) {
        sort_results(upper, comp);
    }
}

int similarity(char* s1, char* s2) {
    int i;
    int l1 = strlen(s1);
    int l2 = strlen(s2);
    int score = 0;
    char* withcase = strstr(s1, s2);
    char* non_case = strcasestr(s1, s2);
    score += strlen(withcase!=NULL ? withcase : "") * 2; // Double score for case-matching
    score += strlen(non_case!=NULL ? non_case : ""); // Single score for non-case matching
    return score;
}

ls_results* ls(char* path) {
    DIR* dirp = opendir(path);
    struct dirent* dp;

    ls_results* results = malloc(sizeof(ls_results));
    results->results = malloc(256);// max # of entries
    results->num_results = 0;
    if (dirp == NULL)
        return NULL; //not a directory
    while ((dp = readdir(dirp)) != NULL) {
        results->results[results->num_results] = dp->d_name;
        results->num_results++;
    }
    closedir(dirp);
    return results;
}

/*int main() {
    char** paths = malloc(1);
    paths[0] = ".";
    tcomp_results* res = genResults("tab", paths, 1, 0); 
    int i;
    for (i=0; i < res->num_results; ++i) {
        printf("%s\n", res->results[i]);
    }
    return 0;
}*/
