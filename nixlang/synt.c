#include "synt.h"
#include "lexer.h"
#include "utils.h"

#include <stdio.h>
#include <string.h>


typedef struct {
	Lexer_result lex;
	unsigned int offset;
	Stack(Node*) stack;
} Synt_state;


typedef struct {
	int count;
	enum Token_type* tokens;
	enum Token_type result;
} Rule;


static int check_rule(Synt_state* state, Node* result, Rule rule) {
	int max_len = -1;

	for (int i = 0; i <
	    min(state->stack.count, rule.count); i++) {
		char ok = 1;

		for (int j = 0; j <= i; j++) {
			Node* node = state->stack.values[
				state->stack.count - 1 - i + j
			];

			if (rule.tokens[j * 2 + 1] != -1 && rule.tokens[j * 2 + 1] != node->next_token) {
				ok = 0;
				continue;
			}

			if (node->token.type != rule.tokens[j * 2 + 0]) {
				ok = 0;
				continue;
			}
		}

		if (ok)
			max_len = i + 1;
	}

	if (max_len == -1)
		return max_len;

	if (max_len == rule.count) {
		result->token.type = rule.result;

		for (int i = 0; i < rule.count; i++)
			append_node(result, stack_pop(state->stack));
	}

	return rule.count - max_len;
}



static Rule rules[] = {
	{
		.count = 2,
		.result = Array,
		.tokens = (enum Token_type[]){
			LSBR, RSBR,
			RSBR, -1,
		},
	},
	{
		.count = 3,
		.result = Array,
		.tokens = (enum Token_type[]){
			LSBR, -1,
			Aarg, RSBR,
			RSBR, -1,
		},
	},
	{
		.count = 5,
		.result = Array,
		.tokens = (enum Token_type[]){
			WITH, -1,
			Name, LSBR,
			LSBR, -1,
			Aarg, RSBR,
			RSBR, -1,
		},
	},
	{
		.count = 4,
		.result = Array,
		.tokens = (enum Token_type[]){
			WITH, -1,
			Name, LSBR,
			LSBR, -1,
			RSBR, -1,
		},
	},

	{
		.count = 1,
		.result = Aarg,
		.tokens = (enum Token_type[]){
			E1, RSBR,
		},
	},
	{
		.count = 3,
		.result = Aarg,
		.tokens = (enum Token_type[]){
			E1, COMMA,
			COMMA, -1,
			Aarg, -1,
		},
	},

	{
		.count = 2,
		.result = Set,
		.tokens = (enum Token_type[]){
			LCBR, RCBR,
			RCBR, -1,
		},
	},
	{
		.count = 3,
		.result = Set,
		.tokens = (enum Token_type[]){
			LCBR, -1,
			Sargs, RCBR,
			RCBR, -1,
		},
	},

	{
		.count = 2,
		.result = Sargs,
		.tokens = (enum Token_type[]){
			Sarg, SEMICOLON,
			SEMICOLON, -1,
		},
	},
	{
		.count = 2,
		.result = Sargs,
		.tokens = (enum Token_type[]){
			Sargs, -1,
			Sargs, -1,
		},
	},

	{
		.count = 3,
		.result = Sarg,
		.tokens = (enum Token_type[]){
			Name, ASSIGN,
			ASSIGN, -1,
			E1, -1,
		},
	},
	{
		.count = 2,
		.result = Sarg,
		.tokens = (enum Token_type[]){
			INHERIT, UNDEFINED,
			UNDEFINED, -1,
		},
	},

	{
		.count = 4,
		.result = Call,
		.tokens = (enum Token_type[]){
			E6, LBR,
			LBR, -1,
			E1, RBR,
			RBR, -1,
		},
	},

	{
		.count = 3,
		.result = Func_decl,
		.tokens = (enum Token_type[]){
			UNDEFINED, COLON,
			COLON, -1,
			E1, -1,
		},
	},
	{
		.count = 6,
		.result = Func_decl,
		.tokens = (enum Token_type[]){
			UNDEFINED, COLON,
			COLON, LET,
			LET, -1,
			Sargs, IN,
			IN, -1,
			E1, -1,
		},
	},

	{
		.count = 3,
		.result = Name,
		.tokens = (enum Token_type[]){
			Name, DOT,
			DOT, UNDEFINED,
			UNDEFINED, -1,
		},
	},
	{
		.count = 3,
		.result = Name,
		.tokens = (enum Token_type[]){
			Set, DOT,
			DOT, UNDEFINED,
			UNDEFINED, -1,
		},
	},
	{
		.count = 1,
		.result = Name,
		.tokens = (enum Token_type[]){
			Eval, -1,
		},
	},
	{
		.count = 3,
		.result = Name,
		.tokens = (enum Token_type[]){
			Name, DOT,
			DOT, -1,
			Eval, -1,
		},
	},
	{
		.count = 3,
		.result = Name,
		.tokens = (enum Token_type[]){
			Set, DOT,
			DOT, UNDEFINED,
			UNDEFINED, -1,
		},
	},
	{
		.count = 1,
		.result = Name,
		.tokens = (enum Token_type[]){
			UNDEFINED, -1,
		},
	},

	{
		.count = 1,
		.result = E6,
		.tokens = (enum Token_type[]){
			DEC_NUMBER, -1,
		},
	},
	{
		.count = 1,
		.result = E6,
		.tokens = (enum Token_type[]){
			FLOAT_NUMBER, -1,
		},
	},
	{
		.count = 1,
		.result = E6,
		.tokens = (enum Token_type[]){
			STRING, -1,
		},
	},
	{
		.count = 1,
		.result = E6,
		.tokens = (enum Token_type[]){
			PATH, -1,
		},
	},
	{
		.count = 1,
		.result = E6,
		.tokens = (enum Token_type[]){
			Set, -1,
		},
	},
	{
		.count = 1,
		.result = E6,
		.tokens = (enum Token_type[]){
			Array, -1,
		},
	},
	{
		.count = 1,
		.result = E6,
		.tokens = (enum Token_type[]){
			Name, -1,
		},
	},
	{
		.count = 3,
		.result = E6,
		.tokens = (enum Token_type[]){
			LBR, -1,
			E1, RBR,
			RBR, -1,
		},
	},
	{
		.count = 1,
		.result = E6,
		.tokens = (enum Token_type[]){
			Func_decl, -1,
		},
	},
	{
		.count = 1,
		.result = E6,
		.tokens = (enum Token_type[]){
			Call, -1,
		},
	},
	{
		.count = 2,
		.result = E6,
		.tokens = (enum Token_type[]){
			EXCLAMATION, -1,
			E6, -1,
		},
	},

	{
		.count = 3,
		.result = E5,
		.tokens = (enum Token_type[]){
			E5, STAR,
			STAR, -1,
			E5, -1,
		},
	},
	{
		.count = 3,
		.result = E5,
		.tokens = (enum Token_type[]){
			E5, SLASH,
			SLASH, -1,
			E5, -1,
		},
	},
	{
		.count = 1,
		.result = E5,
		.tokens = (enum Token_type[]){
			E6, -1,
		},
	},

	{
		.count = 3,
		.result = E4,
		.tokens = (enum Token_type[]){
			E4, PLUS,
			PLUS, -1,
			E4, -1,
		},
	},
	{
		.count = 3,
		.result = E4,
		.tokens = (enum Token_type[]){
			E4, MINUS,
			MINUS, -1,
			E4, -1,
		},
	},
	{
		.count = 1,
		.result = E4,
		.tokens = (enum Token_type[]){
			E5, -1,
		},
	},

	{
		.count = 3,
		.result = E3,
		.tokens = (enum Token_type[]){
			E3, PIPE,
			PIPE, -1,
			E3, -1
		},
	},
	{
		.count = 3,
		.result = E3,
		.tokens = (enum Token_type[]){
			E3, AMPERSAND,
			AMPERSAND, -1,
			E3, -1
		},
	},
	{
		.count = 3,
		.result = E3,
		.tokens = (enum Token_type[]){
			E3, CARET,
			CARET, -1,
			E3, -1
		},
	},
	{
		.count = 1,
		.result = E3,
		.tokens = (enum Token_type[]){
			E4, -1,
		},
	},

	{
		.count = 3,
		.result = E2,
		.tokens = (enum Token_type[]){
			E2, EQUALS,
			EQUALS, -1,
			E2, -1,
		},
	},
	{
		.count = 3,
		.result = E2,
		.tokens = (enum Token_type[]){
			E2, NOT_EQUALS,
			NOT_EQUALS, -1,
			E2, -1,
		},
	},
	{
		.count = 3,
		.result = E2,
		.tokens = (enum Token_type[]){
			E2, LESS,
			LESS, -1,
			E2, -1,
		},
	},
	{
		.count = 3,
		.result = E2,
		.tokens = (enum Token_type[]){
			E2, LESS_EQUALS,
			LESS_EQUALS, -1,
			E2, -1,
		},
	},
	{
		.count = 3,
		.result = E2,
		.tokens = (enum Token_type[]){
			E2, MORE,
			MORE, -1,
			E2, -1,
		},
	},
	{
		.count = 3,
		.result = E2,
		.tokens = (enum Token_type[]){
			E2, MORE_EQUALS,
			MORE_EQUALS, -1,
			E2, -1,
		},
	},
	{
		.count = 1,
		.result = E2,
		.tokens = (enum Token_type[]){
			E3, -1,
		},
	},

	{
		.count = 3,
		.result = E1,
		.tokens = (enum Token_type[]){
			IF, -1,
			E1, THEN,
			THEN, -1,
			E1, ELSE,
			ELSE, -1,
			E1, -1
		},
	},
	{
		.count = 1,
		.result = E1,
		.tokens = (enum Token_type[]){
			E2, -1,
		},
	},

	{
		.count = 3,
		.result = E,
		.tokens = (enum Token_type[]){
			Name, ASSIGN,
			ASSIGN, -1,
			E1, -1,
		},
	},
	{
		.count = 1,
		.result = E,
		.tokens = (enum Token_type[]){
			E1, -1,
		},
	},
};


Node* synt(Lexer_result lex) {
	Synt_state state = {
		.lex = lex,
		.stack.count = 0,
		.stack.values = malloc(0),
	};

	Node* fisrt_node = empty_node();
	fisrt_node->token = lex.tokens[state.offset++];
	fisrt_node->next_token = lex.tokens[state.offset].type;

	stack_push(state.stack, fisrt_node);

	while (1) {
		Node* node = empty_node();
		char find = 0;
		char start_with = 0;

		// printf("\n================\n");
		// for (int i = 0; i < state.stack.count; i++)
		// 	print_node(state.stack.values[i], 0);
		// printf("\n----------------\n");

		for (int i = 0; i < sizeof(rules) / sizeof(Rule); i++) {
			// printf("%d ", i);
			int size = check_rule(&state, node, rules[i]);

			if (size == 0) {
				find = 1;
				// printf("find\n");
				break;
			}

			if (size != -1) {
				start_with = 1;
				// printf("start\n");
				break;
			}
			
			// printf("\n");
		}

		// printf("%d %d\n", find, start_with);

		if (find) {
			node->next_token = lex.tokens[state.offset].type;
			stack_push(state.stack, node);
			continue;
		}

		if (start_with == 0 && state.offset == lex.tokens_count)
			break;

		if (start_with == 0 || state.offset == lex.tokens_count) {
			ERROR("qwerty\n");
			exit(1);
		}
		// printf("%d %d\n", state.offset, lex.tokens_count);
		// fflush(stdout);

		Node* next_node = empty_node();
		next_node->token = lex.tokens[state.offset++];
		next_node->next_token = lex.tokens[state.offset].type;

		stack_push(state.stack, next_node);
	}

	Node* result = stack_pop(state.stack);
	remove_unused_node(result);

	return result;
}


void remove_unused_node(Node* root) {
	if (root->token.type == SEMICOLON ||
	    root->token.type == COLON ||
	    root->token.type == COMMA ||
	    root->token.type == DOLLAR ||
	    root->token.type == IF ||
	    root->token.type == THEN ||
	    root->token.type == ELSE ||
	    root->token.type == LBR ||
	    root->token.type == RBR ||
	    root->token.type == LSBR ||
	    root->token.type == RSBR ||
	    root->token.type == LCBR ||
	    root->token.type == RCBR) {
		remove_node(root->parent, root);
		return;
	}

	int save_childs_count = root->childs_count;
	Node** save_childs = malloc(sizeof(Node*) * save_childs_count);
	memcpy(save_childs, root->childs, sizeof(Node*) * save_childs_count);
	for (int i = 0; i < save_childs_count; i++)
		remove_unused_node(save_childs[i]);
	free(save_childs);

	if (root->childs_count == 1) {
		Node* child = root->childs[0];
		*root = *child;
		free(child);

		return;
	}
}


void print_node(Node* node, int offset) {
	for (int i = 0; i < offset; i++)
		putc('\t', stdout);

	if (node == NULL) {
		printf("NULL\n");
		return;
	}

	if (strlen(node->token.value) == 0)
		printf("%s\n", token_type_names[node->token.type]);
	else
		printf("%s\n", node->token.value);

	// if (token_type_names[node->next_token] != NULL)
	// 	printf("%s\n", token_type_names[node->next_token]);
	// else
	// 	printf("\n");

	for (int i = 0; i < node->childs_count; i++)
		print_node(node->childs[i], offset + 1);
}


void node_to_dot(Node* node) {
	if (strlen(node->token.value) == 0)
		printf("%lu [label=\"%s\"]\n", node->id, token_type_names[node->token.type]);
	else
		printf("%lu [label=\"%s\n%s\"]\n", node->id, token_type_names[node->token.type], node->token.value);

	printf("%lu -> {", node->id);

	for (int i = 0; i < node->childs_count; i++) {
		printf("%lu ", node->childs[i]->id);
	}

	printf("}\n");

	for (int i = 0; i < node->childs_count; i++)
		node_to_dot(node->childs[i]);
}


unsigned long index_node(Node* node, unsigned long id) {
	node->id = id++;

	for (int i = 0; i < node->childs_count; i++)
		id = index_node(node->childs[i], id);

	return id;
}
