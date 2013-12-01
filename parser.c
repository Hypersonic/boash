#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "parser.h"

#define EOT 3

// TODO deal with lines starting with >
// TODO deal with infinitely long lines

void getNextLine(char * line, int n) {
	int reading, i, quote, dquote, esc, c;

	i = 0;
	esc = 0;
	quote = 0;
	dquote = 0;
	reading = 0;

	while ((c = getchar()) != EOF && i < 1023) {
		if (c == '\'' && !dquote) {
			quote = !quote;
			continue;
		}

		if (c == '"' && !quote) {
			dquote = !dquote;
			continue;
		}

		if (esc) {
			line[i++] = escape(c);
			esc = 0;
			continue;
		}

		if (c == '\\') {
			esc = 1;
			continue;
		}

		if (c == '\n') {
			if (quote) {
				printf("quote> ");
				fflush(stdout);
				line[i++] = '\n';
			} else if (dquote) {
				printf("dquote> ");
				fflush(stdout);
				line[i++] = '\n';
			} else
				break;
		}

		if (c == '\t') {
			tabcomplete(line, n, i);
		}

		line[i++] = c;
	}

	line[i] = 0;
}

char escape(char c) {
	switch (c) {
		case '\\': return '\\';
		default  : return '\0';
	}

	return 0;
}

// TODO add tab completion
void tabcomplete(char * line, int n, int i) {
	/*int last;

	line[i+1] = 0;
	last = strrchr(line, ' ');
	*/
}

char ** getTokens(char * line) {
	char ** tokens, c, * dividers;
	int i, j, k, instring, instring2;

	tokens = malloc(32 * sizeof(char *)) ;
	dividers = " \n|><";
	instring2 = 0;
	instring = 0;
	i = 0;
	j = 0;
	k = 0;

	tokens[0] = malloc(1024 * sizeof(char));

	for (i = 0; line[i]; i++) {
		c = line[i];

		// deal with string-related things
		if (c == '"' && !instring2) {
			instring = !instring;
			continue;
		}

		if (c == '\'' && !instring) {
			instring2 = !instring2;
			continue;
		}

		// TODO handle ending in the middle of a string
		// tokens in a string are not evaluated
		if (instring || instring2) {
			tokens[j][k++] = c;
			continue;
		}

		// non-string non-dividers can pass
		if (!strchr(dividers, c)) {
			tokens[j][k++] = c;
			continue;
		}

		// > must be treated specially because >>
		if (c == '>' && tokens[j-1][0] == '>') {
			tokens[j-1][1] = c;
			tokens[j-1][2] = 0;
			continue;
		}

		// dividers are put in their own token
		tokens[++j] = malloc(1024 * sizeof(char));
		tokens[j][k=0] = c;
		tokens[j++][++k] = 0;
		tokens[j] = malloc(1024 * sizeof(char));
		k=0;
	}

	tokens[j][k] = 0;
	tokens[j+1] = NULL; // terminate the list of args

	return tokens;
}

char ** cleanTokenList(char ** tokenList) {
	char ** tl;
	int len, i;

	len = 0;
	while (tokenList[len++]);
	len--;
	len = (len + 1) / 2;

	tl = malloc((len + 1) * sizeof(char *));
	for (i = 0; i < len; i++) {
		tl[i] = malloc(1024 * sizeof(char));
		strncpy(tl[i], tokenList[i*2], 1023);
		tl[i][1023] = 0;
	}

	tl[len] = NULL;

	return tl;
}

void destroyTokenList(char ** tokenList) {
	int i;
	for (i = 0; tokenList[i]; i++)
		free(tokenList[i]);
	tokenList[0] = 0;
	free(tokenList);
}
