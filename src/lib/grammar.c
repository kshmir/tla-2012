#include <stdlib.h>
#include <stdio.h>
#include "../utils/list.h"
#include "../utils/map.h"
#include "../utils/cstring.h"
#include "grammar.h"

struct grammar {
	list vt; // Terminals
	list vn; // Non terminals
	cstring s; // Start token
	map p; // Productions, ordered by their key
};

struct production {
	cstring start;
	list tokens; // List of lists containing all the tokens of a production
};

enum boolean {
	false, true
};

#define RIGHT 1
#define LEFT 2

static void print_terminals(grammar g) {
	printf("Simbolos Terminales: ");
	foreach(cstring, token, grammar_get_terminals(g)) {
		printf("%s,", token);
	}
	printf("\n");
}

static void print_non_terminals(grammar g) {
	printf("Simbolos No Terminales: ");
	foreach(cstring, token, grammar_get_non_terminals(g)) {
		printf("%s,", token);
	}
	printf("\n");
}

static void print_start(grammar g) {
	printf("Simbolo Inicial: %s\n", grammar_get_start(g));
}

static void print_is_valid(grammar g) {
	grammar_is_valid(g) ? printf("Gramatica Valida\n") : printf(
			"Gramatica Invalida\n");
}

int grammar_is_valid(grammar g) {
	return check_terminals(g) && check_non_terminals(g);
}

int is_terminal(grammar g, char token) {
	foreach(cstring, gram_term, grammar_get_terminals(g)) {
		if (gram_term[0] == token) {
			return true;
		}
	}
	return false;
}

int is_non_terminal(grammar g, char token) {
	foreach(cstring, gram_not_term, grammar_get_non_terminals(g)) {
		if (gram_not_term[0] == token) {
			return true;
		}
	}
	return false;
}

int check_non_terminals(grammar g) {
	int flag = false;
	int i, j;
	map m = grammar_get_productions(g);
	foreach(cstring, prod_key, map_keys(m)) {
		foreach(cstring, prod_value, ((production)map_get(m, prod_key))->tokens)
		{
			i = cstring_len(prod_value);
			for (j = 0; j < i; j++) {
				flag = false;
				flag = is_non_terminal(g, prod_value[j]);
				if (!flag)
					flag = is_terminal(g, prod_value[j]);
				if (!flag) {
					return false;
				}
			}
		}
	}
	return true;
}

int check_terminals(grammar g) {
	int flag = false;
	int i, j;
	foreach(cstring, prod_token, map_keys(grammar_get_productions(g))) {
		i = cstring_len(prod_token);
		for (j = 0; j < i; j++) {
			printf("token:%c\n", prod_token[j]);
			flag = false;
			flag = is_non_terminal(g, prod_token[j]);
			if (!flag)
				flag = is_terminal(g, prod_token[j]);
			if (!flag) {
				return false;
			}
		}
	}
	return true;
}

void print_is_regular(grammar g) {
	int reg = grammar_is_regular(g);
	reg ? printf("La gramatica es regular %s\n",
			reg == RIGHT ? "derecha" : "izquierda") : printf(
			"La gramatica no es regular\n");
}

int grammar_is_regular(grammar g) {
	int right = true, left = true;
	map m = grammar_get_productions(g);
	foreach(cstring, prod_key, map_keys(m)) {
		if (cstring_len(prod_key) > 1)
			return false;
		foreach(cstring, prod_value, ((production)map_get(m, prod_key))->tokens)
		{
			if (cstring_len(prod_value) > 2)
				return false;
			if (is_non_terminal(g, prod_value[0]))
				right = false;
			else
				left = false;
			if (!left && !right)
				return false;
		}
	}
	if (right)
		return RIGHT;
	if (left)
		return LEFT;

	return false;
}

list grammar_get_terminals(grammar g) {
	return g->vt;
}

list grammar_get_non_terminals(grammar g) {
	return g->vn;
}

cstring grammar_get_start(grammar g) {
	return g->s;
}

map grammar_get_productions(grammar g) {
	return g->p;
}

grammar grammar_init() {
	grammar g = malloc(sizeof(struct grammar));
	g->vt = list_init();
	g->vn = list_init();
	g->s = NULL;
	g->p = map_init(cstring_comparer, NULL);

	return g;
}

void production_add_token(production p, cstring token) {
	list_add(p->tokens, token);
}

void grammar_add_terminal(grammar g, cstring token) {
	//	printf(".%s.\n", token);
	list_add(g->vt, cstring_copy(token));
}

void grammar_add_non_terminal(grammar g, cstring token) {
	//	printf(".%s.\n", token);
	list_add(g->vn, cstring_copy(token));
}

void grammar_set_start_token(grammar g, cstring token) {
	//	printf(".%s.\n", token);
	g->s = cstring_copy(token);
}

void grammar_add_production(grammar g, production production) {
	map_set(g->p, production->start, production);
}

#define next_step()  if (j != len - 1) {\
						fprintf(file, ",");\
						} j++;

void grammar_print(grammar g, FILE * file) {
	printf("Parseo exitoso\n");
	print_terminals(g);
	print_non_terminals(g);
	print_start(g);
	print_is_valid(g);
	print_is_regular(g);
	fprintf(file, "G1 = ({");

	int j = 0;
	int len = list_size(g->vn);
	foreach(cstring, vn, g->vn) {
		fprintf(file, "%s", vn);
		next_step();
	}

	fprintf(file, "},{");

	j = 0;
	len = list_size(g->vt);
	foreachh(cstring, vt, g->vt) {
		fprintf(file, "%s", vt);
		next_step();
	}

	fprintf(file, "},");
	fprintf(file, "%s", g->s);
	fprintf(file, ",{");

	list productions = map_values(g->p);

	j = 0;
	len = list_size(productions);
	foreachh(production, p, productions) {
		fprintf(file, "%s->", p->start);
		int i = 0;
		int lenn = list_size(p->tokens);
		for (i = 0; i < lenn; ++i) {
			fprintf(file, "%s", (char *) list_get(p->tokens, i));
			if (len - 1 != i) {
				fprintf(file, "|");
			}
		}

		next_step();
	}

	fprintf(file, "})");

}

production production_init() {
	production p = malloc(sizeof(struct production));
	p->start = NULL;
	p->tokens = list_init();

	return p;
}

void production_set_start(production p, cstring start) {
	p->start = cstring_copy(start);
}

production production_from_string(cstring string) {
	production p = production_init();
	cstring to_parse = cstring_trim(string);

	// Parto al medio el A->X|Y|Z
	list parse = cstring_split_list(to_parse, "->");

	// "A"
	cstring start = cstring_trim(list_get(parse, 0));

	production_set_start(p, start);

	// "X|Y|Z"
	cstring productions_raw = list_get(parse, 1);

	// Lista de {X, Y, Z}
	list productions = cstring_split_list(productions_raw, "|");

	// Crea una lista con todos los tokens de X, Y ... Z
	// Y los agrega a la producción.
	foreach(cstring, prod, productions) {
		production_add_token(p, cstring_trim(prod));
	}

	return p;
}

list production_get_tokens(production p) {
	return p->tokens;
}

cstring production_get_start(production p) {
	return p->start;
}
