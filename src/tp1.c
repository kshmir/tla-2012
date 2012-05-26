#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils/list.h"
#include "utils/cstring.h"
#include "tp1.h"

FILE *fmemopen(void *buf, size_t size, const char *opentype) {
	FILE *f;

	f = tmpfile();
	fwrite(buf, 1, size, f);
	rewind(f);

	return f;
}

#define BUFFER_SIZE 65535

char buffer[BUFFER_SIZE];

void tp_run(int mode) {
	FILE * file = stdout;

	FILE * dotfile = fopen(".tmp", "w+");

	grammar g;
	automatha a;
	switch (mode) {
	case GRAMMAR:
		grammar_print_info(_g);

		a = grammar_to_automatha(_g);

		if (a == NULL) {
			return;
		}

		automatha_print(a, dotfile);

		fflush(dotfile);
		fclose(dotfile);

		system("dot -Tpng .tmp > salida.png");

		break;
	case AUTOMATHA:
		automatha_print_info(_a, stdout);
		g = automatha_to_grammar(_a);
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
			if (pFile == NULL) {
				printf("Archivo no encontrado\n");
				return;
			}
			stdin = pFile;
			ready_to_read = GRAMMAR;
			start_adts(ready_to_read);
		}
		if (strstr(input, ".dot") != NULL) {
			FILE * pFile = fopen(input, "rx3");
			if (pFile == NULL) {
				printf("Archivo no encontrado\n");
				return;
			}
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
