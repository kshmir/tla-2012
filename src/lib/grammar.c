#include <stdlib.h>
#include <stdio.h>
#include "../utils/list.h"
#include "../utils/map.h"
#include "../utils/tree.h"
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

void print_terminals(grammar g) {
	printf("Simbolos Terminales: ");
	foreach(cstring, token, grammar_get_terminals(g)) {
		printf("%s,", token);
	}
	printf("\n");
}

void print_non_terminals(grammar g) {
	printf("Simbolos No Terminales: ");
	foreach(cstring, token, grammar_get_non_terminals(g)) {
		printf("%s,", token);
	}
	printf("\n");
}

void print_start(grammar g) {
	printf("Simbolo Inicial: %s\n", grammar_get_start(g));
}

void print_is_valid(grammar g) {
	grammar_is_valid(g) ? printf("Gramatica Valida\n") : printf(
			"Gramatica Invalida\n");
}

int grammar_is_valid(grammar g) {
	return check_terminals(g);// && check_non_terminals(g);
}

int check_non_terminals(grammar g) {
	int flag = false;
	foreach(cstring, prod_token, map_values(grammar_get_productions(g)))
	{
		{
			flag = false;
			foreach(cstring, gram_not_term, grammar_get_non_terminals(g))
				if (strcmp(gram_not_term, prod_token)) {
					flag = true;
				}
		}
		if (!flag) {
			return false;
		}
		return true;
	}
}

int check_terminals(grammar g) {
	int flag = false;
	map p = grammar_get_productions(g);
	foreach(cstring, prod_token, map_values(p))
	{
		{
			flag = false;
			foreach(cstring, gram_not_term, grammar_get_terminals(g))
				if (strcmp(gram_not_term, prod_token)) {
					flag = true;
				}
		}
		if (!flag) {
			return false;
		}
		return true;
	}
}

void print_is_regular(grammar g) {

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
	fprintf(file, "G1 = ({");

	int j = 0;
	int len = list_size(g->vn);
	foreach(cstring, vn, g->vn) {
		fprintf(file,"%s", vn);
		next_step();
	}

	fprintf(file, "},{");

	j = 0;
	len = list_size(g->vt);
	foreachh(cstring, vt, g->vt) {
		fprintf(file,"%s", vt);
		next_step();
	}

	fprintf(file, "},");
	fprintf(file, "%s", g->s);
	fprintf(file, ",{");

	list productions = map_values(g->p);

	j = 0;
	len = list_size(productions);
	foreachh(production, p, productions) {
		fprintf(file,"%s->", p->start);
		int i = 0;
		int lenn = list_size(p->tokens);
		for (i = 0; i < lenn; ++i) {
			fprintf(file,"%s", (char *)list_get(p->tokens, i));
			if (lenn - 1 != i) {
				fprintf(file,"|");
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

production production_cloner(production p) {
	production _p = production_init();
	_p->start = cstring_copy(p->start);
	_p->tokens = list_copy(p->tokens, (cloner) cstring_copy);
	return _p;
}

grammar grammar_clone(grammar g) {
	grammar _g = grammar_init();

	_g->vt = list_copy(g->vt, (cloner) cstring_copy);
	_g->vn = list_copy(g->vn, (cloner) cstring_copy);
	_g->s = cstring_copy(g->s);
	_g->p = map_init(cstring_comparer, NULL);

	foreach(cstring, p, map_keys(g->p)) {
		map_set(_g->p, cstring_copy(p), production_cloner(map_get(g->p, p)));
	}

	return _g;
}

static void grammar_remove_units(grammar g, production p, map productions) {

	list to_remove = list_init();


	foreach(cstring, token, p->tokens) {

		if (cstring_len(token) == 1 && token[0] != '\\' && isupper(token[0])) {

			production to_solve = map_get(productions, token);

			if (to_solve != p) {

				grammar_remove_units(g, to_solve, productions);
				list_add(to_remove, token);
			}
		}
	}

	foreachh(cstring, tr, to_remove) {
		list_remove_item(p->tokens, tr, NULL);
		production to_add = map_get(productions, tr);

		list_add_list(p->tokens, to_add->tokens);
	}
}


static cstring end_terminal = NULL;

static tree generated_terminals = NULL;

static int is_terminal(cstring token) {
	int i = 0;
	int len = cstring_len(token);

	if (len >= 1) {
		return islower(token[0]);
	} else {
		return islower(token[0]);
	}
}

static void grammar_split_non_terms(grammar g, production p) {
	foreach(cstring, token, p->tokens) {
		if (is_terminal(token)) {
			int len = cstring_len(token);
			if (len == 1) {
				if (end_terminal == NULL) {
					end_terminal = cstring_init(2);
					end_terminal[0] = 'M';

				}
			} else {

			}
		}
	}
}

automatha grammar_to_automatha(grammar g) {
	automatha a = automatha_init();

	generated_terminals = tree_init(cstring_comparer);

	grammar normalized = grammar_clone(g);

	list productions = map_values(normalized->p);

	foreach(production, p, productions) {
		grammar_remove_units(normalized, p, normalized->p);
	}

//	foreachh(production, p, productions) {
//		grammar_split_non_terms(normalized, p);
//	}



	grammar_print(normalized, stdout);


	return a;
}













