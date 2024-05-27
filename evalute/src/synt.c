#include "synt.h"
#include "token.h"
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


static void remove_unused_node(Node*);
unsigned long index_node(Node*, unsigned long);
void node_to_dot(Node*);
void print_node(Node*, int);


static int check_rule(Synt_state* state, Node* result, Rule rule) {
	if (state->lex.tokens_count - state->offset + state->stack.count < rule.count)
		return -1;

	int max_len = -1;

	for (int i = 0; i <
	    min(state->stack.count, rule.count); i++) {
		char ok = 1;

		for (int j = 0; j <= i; j++) {
			Node* node = state->stack.values[
				state->stack.count - 1 - i + j
			];

			if (rule.tokens[j * 2 + 1] != -1 &&
			    rule.tokens[j * 2 + 1] != node->next_token) {
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
		.count = 1,
		.result = Vargs,
		.tokens = (enum Token_type[]){
			Sarg, IN,
		},
	},
	{
		.count = 1,
		.result = Sargs,
		.tokens = (enum Token_type[]){
			Sarg, RCBR,
		},
	},
	{
		.count = 2,
		.result = Vargs,
		.tokens = (enum Token_type[]){
			Sarg, UNDEFINED,
			Vargs, -1,
		},
	},

	{
		.count = 2,
		.result = Sargs,
		.tokens = (enum Token_type[]){
			Sarg, UNDEFINED,
			Sargs, -1,
		},
	},

	{
		.count = 4,
		.result = Sarg,
		.tokens = (enum Token_type[]){
			Name, ASSIGN,
			ASSIGN, -1,
			E1, SEMICOLON,
			SEMICOLON, -1,
		},
	},
	{
		.count = 3,
		.result = Sarg,
		.tokens = (enum Token_type[]){
			INHERIT, UNDEFINED,
			Name, SEMICOLON,
			SEMICOLON, -1,
		},
	},

	{
		.count = 2,
		.result = Call,
		.tokens = (enum Token_type[]){
			E6, LBR,
			// LBR, -1,
			E1, -1,
			// RBR, -1,
		},
	},
	{
		.count = 2,
		.result = Call,
		.tokens = (enum Token_type[]){
			E6, DEC_NUMBER,
			// LBR, -1,
			E1, -1,
			// RBR, -1,
		},
	},
	{
		.count = 2,
		.result = Call,
		.tokens = (enum Token_type[]){
			E6, FLOAT_NUMBER,
			// LBR, -1,
			E1, -1,
			// RBR, -1,
		},
	},
	{
		.count = 2,
		.result = Call,
		.tokens = (enum Token_type[]){
			E6, UNDEFINED,
			// LBR, -1,
			E1, -1,
			// RBR, -1,
		},
	},
	{
		.count = 2,
		.result = Call,
		.tokens = (enum Token_type[]){
			E6, LCBR,
			// LBR, -1,
			E1, -1,
			// RBR, -1,
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
		.count = 5,
		.result = Func_decl,
		.tokens = (enum Token_type[]){
			LCBR, -1,
			Fargs, RCBR,
			RCBR, COLON,
			COLON, -1,
			E1, -1,
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
		.count = 6,
		.result = Set,
		.tokens = (enum Token_type[]){
			LET, -1,
			Vargs, IN,
			IN, -1,
			LCBR, -1,
			Sargs, RCBR,
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
		.count = 1,
		.result = Fargs,
		.tokens = (enum Token_type[]){
			UNDEFINED, RCBR,
		},
	},
	{
		.count = 1,
		.result = Fargs,
		.tokens = (enum Token_type[]){
			EPSILON, RCBR
		},
	},
	{
		.count = 3,
		.result = Fargs,
		.tokens = (enum Token_type[]){
			UNDEFINED, COMMA,
			COMMA, -1,
			Fargs, RCBR,
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


Synt_result synt(Lexer_result lex) {
	Synt_result result = {
		.ok = 0,
		.root = NULL,
		.malloc_info = {
			.end = NULL,
			.start = NULL,
		},
	};

	malloc_info = &result.malloc_info;

	Synt_state state = {
		.lex = lex,
	};

	state.stack.count = 0,
	state.stack.values = wmalloc(0);


	Node* fisrt_node = empty_node();
	fisrt_node->token = lex.tokens[state.offset++];
	fisrt_node->next_token = lex.tokens[state.offset].type;

	stack_push(state.stack, fisrt_node);

	while (1) {
		Node* node = empty_node();
		char find = 0;
		char start_with = 0;

		printf("\n================\n");
		printf("%d %d\n", state.offset, lex.tokens_count);
		printf("%s %s\n",
		     token_type_names[state.lex.tokens[state.offset].type],
		     state.lex.tokens[state.offset].value);
		     // token_type_names[state.lex.tokens[state.offset + 1].type]);
		for (int i = 0; i < state.stack.count; i++)
			print_node(state.stack.values[i], 0);
		printf("\n----------------\n");

		for (int i = 0; i < sizeof(rules) / sizeof(Rule); i++) {
			// printf("%d %s ", i, token_type_names[rules[i].result]);
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

		if (start_with == 0 && lex.tokens[state.offset].type == EOI)
			break;

		if (start_with == 0 || lex.tokens[state.offset].type == EOI) {
			if (lex.tokens[state.offset].type == EOI) {
				ERROR("%s: %lu:%lu, unexpected end of file\n",
			    lex.tokens[state.offset].filename,
			    lex.tokens[state.offset].line,
			    lex.tokens[state.offset].column);
				return result;
			}

			ERROR("%s: %lu:%lu, unexpected token: %s\n",
			    lex.tokens[state.offset].filename,
			    lex.tokens[state.offset].line,
			    lex.tokens[state.offset].column,
			    token_type_names[lex.tokens[state.offset].type]);
			return result;
		}
		// printf("%d %d\n", state.offset, lex.tokens_count);
		// fflush(stdout);

		Node* next_node = empty_node();
		next_node->token = lex.tokens[state.offset++];
		next_node->next_token = lex.tokens[state.offset].type;

		stack_push(state.stack, next_node);
	}

	if (state.stack.count != 1) {
		ERROR("%s: %lu:%lu, unexpected token: %s\n",
		    lex.tokens[state.offset-1].filename,
		    lex.tokens[state.offset-1].line,
		    lex.tokens[state.offset-1].column,
		    token_type_names[lex.tokens[state.offset-1].type]);
		return result;
	}

	result.root = stack_pop(state.stack);
	remove_unused_node(result.root);

	result.ok = 1;

	return result;
}


static void remove_unused_node(Node* root) {
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
	    root->token.type == RCBR ||
	    root->token.type == IN ||
	    root->token.type == LET) {
		detach_node(root->parent, root);
		return;
	}

	int save_childs_count = root->childs_count;
	Node** save_childs = malloc(sizeof(Node*) * save_childs_count);
	memcpy(save_childs, root->childs, sizeof(Node*) * save_childs_count);
	for (int i = 0; i < save_childs_count; i++)
		remove_unused_node(save_childs[i]);
	free(save_childs);

	if (root->childs_count == 1 && !(
	      root->token.type == Array ||
	      root->token.type == Set ||
	      root->token.type == Name
	)) {
		Node* child = root->childs[0];
		*root = *child;
		// wfree(child);

		return;
	}

	if (root->token.type == E ||
	    root->token.type == E1 ||
	    root->token.type == E2 ||
	    root->token.type == E3 ||
	    root->token.type == E4 ||
	    root->token.type == E5 ||
	    root->token.type == E6) {
		root->token = root->childs[1]->token;
		detach_node(root, root->childs[1]);

		return;
	}
}
