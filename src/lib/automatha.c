#include <stdlib.h>
#include <stdio.h>
#include "../utils/map.h"
#include "../utils/cstring.h"
#include "../utils/tree.h"
#include "automatha.h"
#include <ctype.h>

typedef struct state * state;
typedef struct transition * transition;

enum boolean {
	false, true
};

/**
 * Structure representation of the automatha.
 */
struct automatha {
	/**
	 * Map that represents all the states of the automatha.
	 */
	map states;
};

struct state {
	/**
	 * Label of the state.
	 */
	cstring label;
	/**
	 * Name of the state.
	 */
	cstring name;
	/**
	 * List of transitions of the state.
	 */
	list transitions;
	/**
	 * Tells wether a state is final or not
	 */
	int is_final;
};

struct transition {
	/**
	 * Direction of the transition.
	 */
	cstring to;
	/**
	 * Token the transition points to.
	 */
	cstring token;
};

static map _node_to_vn = NULL;

automatha automatha_init() {
	automatha a = malloc(sizeof(struct automatha));
	a->states = map_init(cstring_comparer, NULL);
	return a;
}

static transition transition_init(cstring to, cstring token) {
	transition t = malloc(sizeof(struct transition));
	t->to = cstring_trim(to);
	t->token = cstring_trim(token);
	return t;
}

static state state_init(int final, cstring name, cstring label) {
	state s = malloc(sizeof(struct state));
	s->transitions = list_init();
	s->name = cstring_trim(name);
	s->label = cstring_trim(label);
	s->is_final = final;
	return s;
}

void automatha_add_node(automatha g, int final, cstring name, cstring label) {
	state s = state_init(final, name, label);
	map_set(g->states, name, s);
}

void automatha_add_transition(automatha g, cstring from, cstring to,
		cstring token) {
	transition t = transition_init(to, token);
	state s = map_get(g->states, from);
	list_add(s->transitions, t);
}

static void automatha_print_states(automatha a, FILE * file) {
	list states = map_values(a->states);

	foreach(state, s, states) {
		fprintf(file, "\tnode[shape=%s] %s [label=\"%s\"]\n",
				(s->is_final) ? "doublecircle" : "circle", s->name, s->label);
	}

	list_free(states);
}

static void automatha_print_transitions(automatha a, FILE * file) {
	list states = map_values(a->states);

	foreach(state, s, states) {
		list transitions = s->transitions;

		foreach_(transition, t, transitions) {

			if (strcmp(t->to, "") == 0) {
				continue;
			}

			fprintf(file, "\t%s->%s [label=\"%s\"];\n", s->name, t->to,
					t->token);
		}

		list_free(transitions);
	}

	list_free(states);
}

void automatha_print(automatha a, FILE * file) {
	fprintf(file, "digraph {\n");

	automatha_print_states(a, file);

	automatha_print_transitions(a, file);

	fprintf(file, "}\n");
}

static void print_states(automatha a, FILE * file) {
	fprintf(file, "Estados: ");
	foreach(state, st, map_values(a->states)) {
		fprintf(file, "%s, ", st->label);
	}
	fprintf(file, "\n");
}

static void print_terminals(automatha a, FILE * file) {
	fprintf(file, "Estados Finales: ");
	foreach(state, st, map_values(a->states)) {
		if (st->is_final)
			fprintf(file, "%s, ", st->label);
	}
	fprintf(file, "\n");
}

static void print_first_state(automatha a, FILE * file) {
	int flag = true;
	fprintf(file, "Estado Inicial: ");
	foreach_(state, st, map_values(a->states)) {
		if (flag) {
			fprintf(file, "%s", st->label);
			flag = false;
		}
	}
	fprintf(file, "\n");
}

list get_token_list(automatha a) {
	tree t = tree_init(cstring_comparer);
	foreach(state, st, map_values(a->states)) {
		foreach(transition, trans, st->transitions)
		{
			foreach(cstring, token, cstring_split_list(trans->token,"/"))
			{
				tree_add(t, token);
			}
		}
	}
	return tree_to_list(t);
}

static void print_terminal_symbols(automatha a, FILE * file) {
	list l = get_token_list(a);
	fprintf(file, "Simbolos Terminales: ");
	foreach(cstring, token, l) {
		if (!cstring_compare("\\\\", token))
			fprintf(file, "\\, ");
		else
			fprintf(file, "%s, ", token);
	}
	fprintf(file, "\n");
}

void print_check_label(automatha a, transition t, FILE * file) {
	foreach(state, s, map_values(a->states))
	{
		if (!cstring_compare(s->name, cstring_trim(t->to))) {
			fprintf(file, " %s |", s->label);
			return;
		}
	}
}

void print_table_header(list l, FILE * file) {
	fprintf(file, "   |");
	foreach(cstring, token, l) {
		if (!cstring_compare("\\\\", token))
			fprintf(file, " \\ |");
		else
			fprintf(file, " %s |", token);
	}
	fprintf(file, "\n");

}

static void print_table(automatha a, FILE * file) {
	int i, size, no_transitions, match;
	list l = get_token_list(a);
	size = list_size(l);

	print_table_header(l, file);

	foreach(state, stat, map_values(a->states)) {
		fprintf(file, " %s |", stat->label);
		no_transitions = true;
		i = 0;
		foreach(cstring, token, l) {
			match = false;
			foreach(transition, t, stat->transitions) {
				no_transitions = false;
				foreach(cstring, split, cstring_split_list(t->token, "/"))
					if (!cstring_compare(token, split)) {
						match = true;
						print_check_label(a, t, file);
					}
			}
			if (!no_transitions && !match)
				fprintf(file, "   |");
		}
		if (no_transitions) {
			while (i < size) {
				fprintf(file, "   |");
				i++;
			}
		}
		fprintf(file, "\n");
	}
	fprintf(file, "\n");
}

void automatha_print_info(automatha a, FILE * file) {

	print_terminal_symbols(a, file);
	print_states(a, file);
	print_first_state(a, file);
	print_terminals(a, file);
	print_table(a, file);
}

static cstring node_to_vn(cstring node) {
	cstring trimmed = cstring_trim(node);

	char vn0 = 'A';
	int size = map_size(_node_to_vn);

	if (map_get(_node_to_vn, trimmed) == NULL) {
		cstring converted = cstring_init(1);
		converted[0] = vn0 + size;

		map_set(_node_to_vn, trimmed, converted);
	}

	cstring result = map_get(_node_to_vn, trimmed);

	//	cstring_free(trimmed);

	//	printf("node_to_vn: .%s.%s.\n", trimmed, result);

	return result;
}

static void store_non_terminals(grammar g, tree non_terminals) {
	list l = tree_to_list(non_terminals);

	foreach(cstring, t, l)
	{
		grammar_add_non_terminal(g, t);
	}
}

static void store_terminals(grammar g, tree terminals) {
	list l = tree_to_list(terminals);

	foreach(cstring, t, l)
	{
		grammar_add_terminal(g, t);
	}
}

// Fixes production references for the grammar
void fix_productions(grammar g) {
	cstring q = cstring_init(1);
	map productions = grammar_get_productions(g);
	list production_values = map_values(productions);
	foreach(production, p, production_values)
	{
		list tokens = production_get_tokens(p);
		foreach(cstring, token, tokens)
		{
			cstring new_token = cstring_init(0);
			int i = 0;
			int len = cstring_len(token);
			for (i = 0; i < len; ++i) {
				q[0] = token[i];
				if (islower(q[0]) || map_get(productions, q) != NULL || q[0]
						== '\\') {
					new_token = cstring_write(new_token, q);
				}
			}

			for (i = 0; i < len; ++i) {
				if (i < cstring_len(new_token)) {
					token[i] = new_token[i];
				} else
					token[i] = 0;
			}
		}
	}
}

int automatha_is_connected(automatha a) {
	int flag = true, changed = true, i;

	tree all_nodes = tree_init(cstring_comparer);
	tree old_nodes = tree_init(cstring_comparer);
	tree new_nodes = tree_init(cstring_comparer);

	foreach(state, s, map_values(a->states)) {
		if (flag) {
			tree_add(old_nodes, s->name);
			flag = false;
		}
		tree_add(all_nodes, s->name);
	}

	do {
		list nodes = tree_to_list(old_nodes);
		foreach(cstring, node, nodes) {
			state s = map_get(a->states, node);
			tree_add(new_nodes, node);
			foreach_(transition, t, s->transitions)
			{
				tree_add(new_nodes, t->to);
			}
		}

		if (tree_compare(old_nodes, new_nodes) == 0) {
			list_free(nodes);
			tree_free(old_nodes);
			break;
		}

		list_free(nodes);
		tree_free(old_nodes);

		old_nodes = new_nodes;
		new_nodes = tree_init(cstring_comparer);
	} while (1);

	int result = tree_compare(all_nodes, new_nodes);

	tree_free(all_nodes);
	tree_free(new_nodes);

	return !result;
}

grammar automatha_to_grammar(automatha a) {

	if (!_node_to_vn) {
		_node_to_vn = map_init(cstring_comparer, NULL);
	}

	if (!automatha_is_connected(a)) {
		fprintf(stdout, "El automata no es conexo\n");
		return NULL;
	}

	grammar g = grammar_init();
	tree set_of_terminals = tree_init(cstring_comparer);
	tree set_of_non_terminals = tree_init(cstring_comparer);

	int i = 0;

	list states = map_values(a->states);

	map productions = grammar_get_productions(g);

	foreach(state, s, states) {
		if (i == 0) {
			grammar_set_start_token(g, node_to_vn(s->name));
		}

		tree_add(set_of_non_terminals, node_to_vn(s->name));

		foreach_(transition, t, s->transitions) {
			int j = 0;

			list tokens = cstring_split_list(t->token, "/");

			for (j = 0; j < list_size(tokens); ++j) {
				int is_new = 0;
				cstring prodtok = cstring_init(2);
				cstring tok = list_get(tokens, j);
				if (!cstring_compare(&tok[0], "\\\\")) {
					prodtok[0] = node_to_vn(t->to)[0];
				} else {
					prodtok[0] = tok[0];
					prodtok[1] = node_to_vn(t->to)[0];
				}

				production p = map_get(productions, node_to_vn(s->name));

				if (p == NULL) {
					p = production_init();
					is_new = 1;
				}

				production_set_start(p, node_to_vn(s->name));
				production_add_token(p, prodtok);

				if (is_new) {
					grammar_add_production(g, p);
				}

				if (prodtok[0] != '\\') {
					tree_add(set_of_terminals, tok);
				}

			}

			tree_add(set_of_non_terminals, node_to_vn(t->to));
		}
		i++;
	}

	store_non_terminals(g, set_of_non_terminals);

	store_terminals(g, set_of_terminals);

	fix_productions(g);

	return g;
}

