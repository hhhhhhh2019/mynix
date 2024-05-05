#include "synt.h"
#include "lexer.h"
#include "utils.h"

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
		.count = 3,
		.tokens = (enum Token_type[]){
			E, PLUS,
			PLUS, -1,
			E, -1,
		},
		.result = E,
	},

	{
		.count = 3,
		.tokens = (enum Token_type[]){
			E1, STAR,
			STAR, -1,
			E1, -1,
		},
		.result = E1,
	},

	{
		.count = 1,
		.tokens = (enum Token_type[]){
			E1, -1,
		},
		.result = E,
	},

	{
		.count = 1,
		.tokens = (enum Token_type[]){
			E2, -1,
		},
		.result = E1,
	},

	{
		.count = 1,
		.tokens = (enum Token_type[]){
			DEC_NUMBER, -1,
		},
		.result = E2,
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

		// for (int i = 0; i < state.stack.count; i++)
		// 	print_node(state.stack.values[i], 0);
		// printf("================\n");

		for (int i = 0; i < sizeof(rules) / sizeof(Rule); i++) {
			int size = check_rule(&state, node, rules[i]);

			if (size == 0) {
				find = 1;
				break;
			}

			if (size != -1) {
				start_with = 1;
				break;
			}
		}

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

		Node* next_node = empty_node();
		next_node->token = lex.tokens[state.offset++];
		next_node->next_token = lex.tokens[state.offset].type;

		stack_push(state.stack, next_node);
	}

	return stack_pop(state.stack);
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
