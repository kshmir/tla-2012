#include <stdlib.h>
#include <stdio.h>
#include "../utils/map.h"
#include "../utils/cstring.h"
#include "automatha.h"

typedef struct state * state;
typedef struct transition * transition;

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

automatha automatha_init() {
	automatha a = malloc(sizeof(struct automatha));
	a->states = map_init(cstring_comparer, NULL);
	return a;
}

static transition transition_init(cstring to, cstring token) {
	transition t = malloc(sizeof(struct transition));
	t->to = cstring_copy(to);
	t->token = cstring_copy(token);
	return t;
}

static state state_init(int final, cstring name, cstring label) {
	state s = malloc(sizeof(struct state));
	s->transitions = list_init();
	s->name = cstring_copy(name);
	s->label = cstring_copy(label);
	s->is_final = final;
	return s;
}

void automatha_add_node(automatha g, int final, cstring name, cstring label) {
	state s = state_init(final, name, label);
	map_set(g->states, name, s);
}


void automatha_add_transition(automatha g, cstring from, cstring to, cstring token) {
	transition t = transition_init(to, token);
	state s = map_get(g->states, from);
	list_add(s->transitions, t);
}


