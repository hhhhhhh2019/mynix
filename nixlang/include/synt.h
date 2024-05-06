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
void remove_unused_node(Node*);

static inline Node* empty_node() {
	Node* node = calloc(sizeof(Node), 1);
	node->token.value = "";
	return node;
}

static inline void remove_node(Node* root, Node* node) {
	int id;

	for (int i = 0; i < root->childs_count; i++) {
		if (root->childs[i] != node)
			continue;

		id = i;
		break;
	}

	for (int i = id; i < root->childs_count; i++)
		root->childs[i] = root->childs[i + 1];

	root->childs = realloc(root->childs,
	                sizeof(Node*) * (--root->childs_count));
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
	node->parent = root;
}


unsigned long index_node(Node*, unsigned long);
void node_to_dot(Node*);

void print_node(Node*, int);


#endif
