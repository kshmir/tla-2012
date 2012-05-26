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

static int print_is_valid(grammar g) {
	int valid = grammar_is_valid(g);
	if (valid)
		printf("Gramatica Valida\n");
	else
		printf("Gramatica Invalida\n");
	return valid;
}

static void print_is_regular(grammar g) {
	int reg = grammar_is_regular(g);
	reg ? printf("La gramatica es regular %s\n",
					reg == RIGHT ? "derecha" : "izquierda") :
			printf("La gramatica no es regular\n");
}

int grammar_is_valid(grammar g) {
	return check_terminals(g) && check_non_terminals(g);
}

int is_terminal_token(grammar g, char token) {
	if (token == '\\') {
		return true;
	}
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

int grammar_can_become_regular(grammar g) {
	if (grammar_is_regular(g))
		return true;
	int i, right = true, left = true;
	map m = grammar_get_productions(g);
	foreach(cstring, prod_key, map_keys(m)) {
		if (cstring_len(prod_key) > 1)
			return false;
		foreach(cstring, prod_value, ((production)map_get(m, prod_key))->tokens)
		{
			int len = cstring_len(prod_value);
			if (len > 2) {
				for (i = 0; i < len; i++) {
					if (is_non_terminal(g, prod_value[i])) {
						return false;
					}
				}
			} else {
				if (is_non_terminal(g, prod_value[0]))
					right = false;
				else if (prod_value[0] != '\\')
					left = false;
				if (!left && !right)
					return false;
			}
		}
	}
	if (right)
		return RIGHT;
	if (left)
		return LEFT;

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
					flag = is_terminal_token(g, prod_value[j]);
				if (!flag) {
					return false;
				}
			}
		}
	}
	return flag;
}

int check_terminals(grammar g) {
	int flag = false, no_term = false;
	int i, j;
	foreach(cstring, prod_token, map_keys(grammar_get_productions(g))) {
		i = cstring_len(prod_token);
		for (j = 0; j < i; j++) {
			flag = false;
			flag = is_non_terminal(g, prod_token[j]);
			if (flag)
				no_term = true;
			if (!flag)
				flag = is_terminal_token(g, prod_token[j]);
			if (!flag) {
				return false;
			}
		}
	}
	return flag && no_term;
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
	if (list_indexof(g->vt, token, cstring_comparer) == -1) {
		list_add(g->vt, cstring_copy(token));
	}
}

void grammar_add_non_terminal(grammar g, cstring token) {
	if (list_indexof(g->vn, token, cstring_comparer) == -1) {
		list_add(g->vn, cstring_copy(token));
	}
}

void grammar_set_start_token(grammar g, cstring token) {
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
	if (print_is_valid(g))
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
			if (lenn - 1 != i) {
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

	foreachh(cstring, tr, to_remove)
	{
		list_remove_item(p->tokens, tr, NULL);
		production to_add = map_get(productions, tr);

		list_add_list_no_copy(p->tokens, to_add->tokens, cstring_comparer);
	}
}

static cstring end_terminal = NULL;
static end_terminal_i = 0;

static int is_generated(char c) {
	int i = 0;
	for (i = 'Z' - end_terminal_i; i < 'Z' + 1; i++) {
		if (c == i) {
			return 1;
		}
	}
	return 0;
}

static int is_generated_terminal(cstring token, int mode) {
	int i = 0;
	int len = cstring_len(token);

	if (mode == LEFT) {
		if (len >= 1) {
			return islower(token[0]) || is_generated(token[0]);
		} else {
			return islower(token[0]);
		}
	} else {
		if (len >= 1) {
			return islower(token[len - 1]) || is_generated(token[len -1]);
		} else {
			return islower(token[0]);
		}
	}
}

static cstring get_next_end_terminal() {
	char start = 'Z';

	cstring result = cstring_init(1);

	result[0] = start - (end_terminal_i++);

	return result;
}

static void grammar_split_non_terms(grammar g, production p, list productions, int mode) {

	list to_remove = list_init();
	list to_add = list_init();


	foreach(cstring, token, p->tokens) {

		if (is_generated_terminal(token, mode)) {

			int len = cstring_len(token);
			cstring nonTerm;
			if (len == 1) {
				if (end_terminal == NULL) {
					end_terminal = cstring_init(1);
					end_terminal[0] = 'M';
				}
				nonTerm = cstring_copy(end_terminal);
			} else {
				nonTerm = get_next_end_terminal();
			}

			nonTerm = cstring_write(nonTerm, " ");




			if (mode == RIGHT) {
				nonTerm[1] = token[0];
				nonTerm = cstring_reverse(nonTerm);
			} else {
				nonTerm[1] = token[len - 1];
			}



			list_add(to_remove, token);
			list_add(to_add, nonTerm);

			cstring new_token = cstring_copy(token);

			new_token[len - 1] = 0;

			production pr = production_init();
			pr->start = cstring_init(1);

			if (mode == RIGHT) {
				pr->start[0] = nonTerm[len];
			} else {
				pr->start[0] = nonTerm[0];
			}



			if (cstring_len(new_token) > 0) {
				list_add(pr->tokens, cstring_copy(new_token));
			} else {
				list_add(pr->tokens, "\\");
			}


			grammar_add_non_terminal(g, pr->start);
			grammar_add_production(g, pr);
			list_add(productions, pr);
		}
	}

	foreachh(cstring, tok, to_add) {
		list_add(p->tokens, tok);
	}

	foreachh(cstring, t, to_remove)
	{
		list_remove_item(p->tokens, t, cstring_comparer);
	}
}

static grammar grammar_to_right_form(grammar from, grammar to) {
	foreach(cstring, non_terminal, grammar_get_non_terminals(from)) {
		grammar_add_non_terminal(to, non_terminal);
	}

	foreachh(cstring, terminal, grammar_get_terminals(from)) {
		grammar_add_terminal(to, terminal);
	}

	grammar_set_start_token(to, "M");

	// A -> Ba se transoforma en B->aA

	production def = production_init();

	production_set_start(def, cstring_copy("A"));
	production_add_token(def, cstring_copy("\\"));

	grammar_add_production(to, def);

	list productions = map_values(grammar_get_productions(from));
	foreachh(production, prod, productions) {
		foreach_(cstring, token, production_get_tokens(prod))
		{
			if (cstring_compare(token, "\\") != 0) {
				int is_new = 0;
				production new_prod = NULL;
				cstring new_start = cstring_init(1);
				new_start[0] = token[0];

				if (map_get(grammar_get_productions(to), new_start) == NULL) {
					new_prod = production_init();
					production_set_start(new_prod, new_start);
					is_new = 1;
				} else {
					new_prod = map_get(grammar_get_productions(to), new_start);
				}

				cstring new_token = cstring_init(2);

				new_token[0] = token[1];
				new_token[1] = prod->start[0];

				production_add_token(new_prod, new_token);

				if (is_new) {
					grammar_add_production(to, new_prod);
				}
			}
		}
	}
	return to;
}

static int prod_is_terminal(grammar g, production prod) {
	int i = 0;
	foreach_(cstring, token, prod->tokens) {
		if (token[0] == '\\') return 1;
	}
	return 0;
}

automatha grammar_to_automatha(grammar g) {
	automatha a = automatha_init();

	grammar normalized = grammar_clone(g);
	grammar lefted = grammar_init();

	list productions = map_values(normalized->p);



	foreach(production, p, productions) {
		grammar_remove_units(normalized, p, normalized->p);
	}





	int regularity = grammar_can_become_regular(normalized);

	if (regularity == 0) {
		printf("---INV---\n");
		return NULL;
	}

	if (regularity == LEFT) {
		foreachh(production, _p, productions) {
			grammar_split_non_terms(normalized, _p, productions, LEFT);
		}

		lefted = grammar_to_right_form(normalized, lefted);
	} else {
		foreachh(production, _p, productions) {
			grammar_split_non_terms(normalized, _p, productions, RIGHT);
		}

		lefted = normalized;
	}

	productions = map_values(lefted->p);

	foreachh(production, prod, productions) {


		automatha_add_node(a, (prod_is_terminal(g, prod)) ? 1 : 0,
														prod->start, prod->start);

		foreach_(cstring, token, prod->tokens)
		{
			cstring to, _token;
			to = cstring_init(1);
			_token = cstring_init(1);
			to[0] = token[1] ;
			_token[0] = token[0];
			automatha_add_transition(a, prod->start, to, _token);
		}


	}

	return a;
}

