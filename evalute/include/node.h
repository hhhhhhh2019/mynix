#ifndef NODE_H
#define NODE_H


#include "token.h"
#include "utils.h"

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


static inline Node* empty_node() {
	Node* node = wzmalloc(sizeof(Node));
	node->token.value = "";
	node->childs = wmalloc(0);
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

	root->childs = wrealloc(root->childs,
	                sizeof(Node*) * (--root->childs_count));
}


static inline void append_node(Node* root, Node* node) {
	root->childs = wrealloc(root->childs,
	               sizeof(Node) * (++root->childs_count));
	root->childs[root->childs_count-1] = node;
	node->parent = root;
}


#endif
