#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils/list.h"
#include "utils/cstring.h"
#include "tp1.h"

void tp_run(int mode) {
	FILE * file = stdout;

	grammar g;
	switch (mode) {
		case GRAMMAR:
			grammar_print(_g, stdout);

			grammar_to_automatha(_g);
			break;
		case AUTOMATHA:
//			automatha_print(_a, stdout);
			g = automatha_to_grammar(_a);

			printf("END!\n");
			grammar_print(g, stdout);

			break;
	}
}


void run(int len, char ** args) {
	if (len == 2) {
		char * input = args[1];
		int ready_to_read = 0;

		if (strstr(input, ".gr") != NULL) {
			FILE * pFile = fopen(input, "r");
			stdin = pFile;
			ready_to_read = GRAMMAR;
			start_adts(ready_to_read);
		}
		if (strstr(input, ".dot") != NULL) {
			FILE * pFile = fopen(input, "rx3");
			stdin = pFile;
			ready_to_read = AUTOMATHA;
			start_adts(ready_to_read);
		}

		if (ready_to_read) {
			yylex();
			tp_run(ready_to_read);
		}
	}
}



void start_adts(int mode) {
	switch (mode) {
	case GRAMMAR:
		_g = grammar_init();
		parsingGrammar = 1;
		break;
	case AUTOMATHA:
		_a = automatha_init();
		parsingAutomatha = 1;
		break;
	}
}
