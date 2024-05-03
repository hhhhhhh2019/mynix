#include "synt.h"
#include "lexer.h"
#include "utils.h"

#include <stdlib.h>
#include <string.h>


struct Synt_state {
	Lexer_result lex;
	unsigned int offset;
};


static Token* match(struct Synt_state* state, int count, enum Token_type types[]) {
	if (state->offset >= state->lex.tokens_count)
		return 0;

	for (int i = 0; i < count; i++) {
		if (state->lex.tokens[state->offset].type == types[i])
			return &state->lex.tokens[state->offset];
	}
	return 0;
};


static Token* require(struct Synt_state* state, int count, enum Token_type types[]) {
	Token* t = match(state, count, types);
	if (t != NULL) {
		state->offset++;
		return t;
	}

	ERROR("%s:%lu:%lu\n",
		 state->lex.tokens[state->offset].filename,
		 state->lex.tokens[state->offset].line,
		 state->lex.tokens[state->offset].column
	);

	exit(1);

	return 0;
}


static Node* state_Set                     (struct Synt_state*);
static Node* state_Set_                    (struct Synt_state*);
static Node* state_Sargs                   (struct Synt_state*);
static Node* state_Sargs_                  (struct Synt_state*);
static Node* state_Array                   (struct Synt_state*);
static Node* state_Array_                  (struct Synt_state*);
static Node* state_Aargs                   (struct Synt_state*);
static Node* state_Aargs_                  (struct Synt_state*);
static Node* state_E                       (struct Synt_state*);
static Node* state_E_                      (struct Synt_state*);
static Node* state_E1                      (struct Synt_state*);
static Node* state_E1_                     (struct Synt_state*);
static Node* state_E2                      (struct Synt_state*);
static Node* state_E2_                     (struct Synt_state*);
static Node* state_E3                      (struct Synt_state*);
static Node* state_E3_                     (struct Synt_state*);
static Node* state_E4                      (struct Synt_state*);
static Node* state_E4_                     (struct Synt_state*);
static Node* state_Name                    (struct Synt_state*);
static Node* state_Name_                   (struct Synt_state*);
static Node* state_Name_Func_decl_Call_Set (struct Synt_state*);


static Node* state_Name_(struct Synt_state* state) {
	if (match(state, 1, (enum Token_type[]){
		DOT,
	}) == 0) return NULL;

	Node* op = empty_node();
	op->token = state->lex.tokens[state->offset++];

	append_node(op, state_Name(state));

	return op;
}


static Node* state_Name(struct Synt_state* state) {
	Token* tleft = require(state, 1, (enum Token_type[]){
		UNDEFINED,
	});

	if (tleft == NULL)
		return NULL;

	// if (tleft->type == LCBR) {
	//
	// }
	
	Node* left = empty_node();
	left->token = *tleft;
	
	Node* op = state_Name_(state);

	if (op == NULL)
		return left;

	append_node(op, left);

	return op;
}


static Node* state_Set(struct Synt_state* state) {
	if (require(state, 1, (enum Token_type[]){
		LCBR
	}) == 0)
		return NULL;

	Node* root = empty_node();
	root->token.type = Set;
	
	while (state->lex.tokens[state->offset].type != RCBR) {
		Node* name = state_Name(state);
		Node* op = empty_node();
		op->token = *require(state, 1, (enum Token_type[]){ASSIGN});

		append_node(op, name);
		append_node(op, state_E1(state));

		append_node(root, op);

		if (require(state, 1, (enum Token_type[]){SEMICOLON}) == 0)
			break;
	}

	if (require(state, 1, (enum Token_type[]){
		RCBR
	}) == 0)
		return NULL;

	return root;
}


static Node* state_Name_Func_decl_Call_Set(struct Synt_state* state) {
	Node* left = NULL;

	if (match(state, 1, (enum Token_type[]){
		LCBR,
	}))
		left = state_Set(state);

	if (left == NULL && state->lex.tokens[state->offset+1].type == COLON) {
		left = empty_node();
		left->token = *require(state, 1, (enum Token_type[]){
			UNDEFINED,
		});

		require(state, 1, (enum Token_type[]){
			COLON
		});

		Node* op = empty_node();
		op->token.type = Func_decl;

		append_node(op, left);
		append_node(op, state_E1(state));

		return op;
	}

	if (left != NULL && state->lex.tokens[state->offset].type == COLON) {
		Node* op = empty_node();
		op->token.type = Func_decl;

		require(state, 1, (enum Token_type[]){
			COLON
		});

		append_node(op, left);
		append_node(op, state_E1(state));

		return op;
	}

	if (left != NULL)
		return left;

	// if (state->lex.tokens[state->offset].type == UNDEFINED &&
	//     state->lex.tokens[state->offset+1].type == DOT)
	// 	left = state_Name(state);

	left = state_Name(state);

	if (match(state, 11, (enum Token_type[]){
		DEC_NUMBER,
		FLOAT_NUMBER,
		STRING,
		PATH,
		TRUE,
		FALSE,
		LCBR,
		LSBR,
		LBR,
		UNDEFINED,
		EXCLAMATION,
		// IF,
	})) {
		Node* op = empty_node();
		op->token.type = Call;

		append_node(op, left);
		append_node(op, state_E1(state));

		return op;
	}

	return left;
}


static Node* state_Array(struct Synt_state* state) {
	if (require(state, 1, (enum Token_type[]){
		LSBR
	}) == 0) return NULL;

	Node* root = empty_node();
	root->token.type = Array;

	while (state->lex.tokens[state->offset].type != RSBR) {
		append_node(root, state_E1(state));
		if (match(state, 1, (enum Token_type[]){COMMA}) == 0)
			break;
		state->offset++;
	}

	if (require(state, 1, (enum Token_type[]){
		RSBR
	}) == 0) return NULL;

	return root;
}


static Node* state_E4(struct Synt_state* state) {
	if (match(state, 6, (enum Token_type[]){
		DEC_NUMBER, FLOAT_NUMBER, STRING, PATH, TRUE, FALSE,
	})) {
		Node* node = empty_node();
		node->token = state->lex.tokens[state->offset++];

		return node;
	}

	if (state->lex.tokens[state->offset].type == LBR) {
		state->offset++;
		Node* node = state_E1(state);
		require(state, 1, (enum Token_type[]){RBR});
		return node;
	}

	if (state->lex.tokens[state->offset].type == LSBR) {
		return state_Array(state);
	}

	if (state->lex.tokens[state->offset].type == EXCLAMATION) {
		Node* op = empty_node();
		op->token = state->lex.tokens[state->offset++];
		append_node(op, state_E1(state));
		return op;
	}

	if (match(state, 2, (enum Token_type[]){
		LCBR, UNDEFINED
	})) {
		return state_Name_Func_decl_Call_Set(state);
	}

	return NULL;
}


static Node* state_E3_(struct Synt_state* state) {
	Token* top = match(state, 2, (enum Token_type[]){
		STAR, SLASH,
	});

	if (top == NULL)
		return NULL;

	state->offset++;

	Node* op = empty_node();
	op->token = *top;

	append_node(op, state_E3(state));

	return op;
}


static Node* state_E3(struct Synt_state* state) {
	Node* left = state_E4(state);
	Node* op = state_E3_(state);

	if (op == NULL)
		return left;

	append_node(op, left);

	Node* op2;

	while ((op2 = state_E3_(state)) != NULL) {
		append_node(op2, op);
		op = op2;
	}

	return op;
}


static Node* state_E2_(struct Synt_state* state) {
	Token* top = match(state, 2, (enum Token_type[]){
		PLUS, MINUS,
	});

	if (top == NULL)
		return NULL;

	state->offset++;

	Node* op = empty_node();
	op->token = *top;

	append_node(op, state_E2(state));

	return op;
}


static Node* state_E2(struct Synt_state* state) {
	Node* left = state_E3(state);
	Node* op = state_E2_(state);

	if (op == NULL)
		return left;

	append_node(op, left);

	Node* op2;

	while ((op2 = state_E2_(state)) != NULL) {
		append_node(op2, op);
		op = op2;
	}

	return op;
}


static Node* state_E1_(struct Synt_state* state) {
	Token* top = match(state, 3, (enum Token_type[]){
		AMPERSAND, CARET, PIPE,
	});

	if (top == NULL)
		return NULL;

	state->offset++;

	Node* op = empty_node();
	op->token = *top;

	append_node(op, state_E1(state));

	return op;
}


static Node* state_E1(struct Synt_state* state) {
	Node* left = state_E2(state);
	Node* op = state_E1_(state);

	if (op == NULL)
		return left;

	append_node(op, left);

	Node* op2;

	while ((op2 = state_E1_(state)) != NULL) {
		append_node(op2, op);
		op = op2;
	}

	return op;
}


static Node* state_E_(struct Synt_state* state) {
	Token* top = match(state, 1, (enum Token_type[]){
		ASSIGN,
	});

	if (top == NULL)
		return NULL;

	state->offset++;

	Node* op = empty_node();
	op->token = *top;

	append_node(op, state_E1(state));

	return op;
}


static Node* state_E(struct Synt_state* state) {
	Node* left = state_E1(state);
	Node* op = state_E_(state);

	if (op == NULL)
		return left;

	append_node(op, left);

	Node* op2;

	while ((op2 = state_E_(state)) != NULL) {
		append_node(op2, op);
		op = op2;
	}

	return op;
}


Node* synt(Lexer_result lex) {
	struct Synt_state state = {
		.lex = lex,
		.offset = 0
	};

	return state_E(&state);
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
