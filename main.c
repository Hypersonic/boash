#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include "parser.h"
#include "main.h"

// TODO add support for -c 'command'
// TODO add support for infinite lines
// TODO abort line on ^C
// TODO close on ^D

int main() {
	char ** tokenList, line[1024];
	int pid, s;

	if (signal(SIGINT, sigHandler) == SIG_ERR) {
		printf("SIGINT handler failed");
		exit(-1);
	}

	if (signal(SIGQUIT, sigHandler) == SIG_ERR) {
		printf("SIGQUITE handler failed");
		exit(-1);
	}

	while (1) {
		//get command
		printf("WOOP %% ");
		getNextLine(line, 1024);
		line[strlen(line)] = 0;

		//process command
		tokenList = getTokens(line);
		tokenList = cleanTokenList(tokenList);

		if (!strcmp(tokenList[0], "exit")) {
			exit(0);
		}

		//run command
		if (tokenList[0][0]) {
			pid = fork();
			if (pid) { //parent
				if (pid == -1)
					printf("Aw, shit.");
				wait(&s);
			} else { //child
				execvp(tokenList[0], tokenList);
				printf("boash: no such command: %s\n", tokenList[0]);
				exit(-1);
			}
		}

		destroyTokenList(tokenList);
		line[0] = 0;
	}

	return 0;
}

void sigHandler(int signum) {
	int reading;

	if (signum == SIGINT) {
		printf("^C");
	}

	if (signum == SIGQUIT) {
		fprintf(stderr, "Finally!\n");
	}
}
