#ifndef SYNT_H
#define SYNT_H


#include "lexer.h"

#include <stdlib.h>


struct Node;
typedef struct Node {
	struct Node* parent;
	Token token;
	int childs_count;
	struct Node** childs;
	unsigned long id;
	enum Token_type next_token;
} Node;


Node* synt(Lexer_result);

static inline Node* empty_node() {
	Node* node = calloc(sizeof(Node), 1);
	node->token.value = "";
	return node;
}

// #define empty_node() \
// 	(Node){ \
// 		.childs_count = 0, \
// 		.childs = malloc(0), \
// 		.parent = NULL, \
// 	}


static inline void append_node(Node* root, Node* node) {
	root->childs = realloc(root->childs,
	               sizeof(Node) * (++root->childs_count));
	root->childs[root->childs_count-1] = node;
}


unsigned long index_node(Node*, unsigned long);
void node_to_dot(Node*);

void print_node(Node*, int);


#endif
